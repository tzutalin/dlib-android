/*
 * jni_people_det.cpp using google-style
 *
 *  Created on: Oct 20, 2015
 *      Author: Tzutalin
 *
 *  Copyright (c) 2015 Tzutalin. All rights reserved.
 */

#include <dlib/image_loader/load_image.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_loader/load_image.h>
#include <glog/logging.h>
#include <jni.h>
#include <memory>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

class OpencvHOGDetctor {
 public:
  OpencvHOGDetctor() {}

  inline int det(std::string path) {
    LOG(INFO) << "det path : " << path;
    cv::Mat src_img = cv::imread(path, 1);
    if (src_img.empty()) return 0;

    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    std::vector<cv::Rect> found, found_filtered;
    hog.detectMultiScale(src_img, found, 0, cv::Size(8, 8), cv::Size(32, 32),
			 1.05, 2);
    size_t i, j;
    for (i = 0; i < found.size(); i++) {
      cv::Rect r = found[i];
      for (j = 0; j < found.size(); j++)
        if (j != i && (r & found[j]) == r) break;
        if (j == found.size()) found_filtered.push_back(r);
    }

    for (i = 0; i < found_filtered.size(); i++) {
      cv::Rect r = found_filtered[i];
      r.x += cvRound(r.width * 0.1);
      r.width = cvRound(r.width * 0.8);
      r.y += cvRound(r.height * 0.06);
      r.height = cvRound(r.height * 0.9);
      cv::rectangle(src_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
    }
    mResultMat = src_img;
    // cv::imwrite(path, mResultMat);
    LOG(INFO) << "det ends";
    mRets = found_filtered;
    return found_filtered.size();
  }

  inline cv::Mat& getResultMat() { return mResultMat; }

  inline std::vector<cv::Rect>& getResult() { return mRets; }

 private:
  cv::Mat mResultMat;
  std::vector<cv::Rect> mRets;
};

class DLibHOGDetector {
 public:
  DLibHOGDetector(std::string modelPath = "/sdcard/person.svm")
      : mModelPath(modelPath) {
    LOG(INFO) << "Model Path: " << mModelPath;
  }

  virtual inline int det(std::string path) {
    cv::Mat src_img = cv::imread(path, 1);

    typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6> > image_scanner_type;
    dlib::object_detector<image_scanner_type> detector;
    dlib::deserialize(mModelPath) >> detector;

    int img_width = src_img.cols;
    int img_height = src_img.rows;
    int im_size_min = MIN(img_width, img_height);
    int im_size_max = MAX(img_width, img_height);

    float scale = float(INPUT_IMG_MIN_SIZE) / float(im_size_min);
    if (scale * im_size_max > INPUT_IMG_MAX_SIZE) {
      scale = (float)INPUT_IMG_MAX_SIZE / (float)im_size_max;
    }

    if (scale != 1.0) {
      cv::Mat outputMat;
      cv::resize(src_img, outputMat, cv::Size(img_width * scale, img_height * scale));
      src_img = outputMat;
    }

    // cv::resize(src_img, src_img, cv::Size(320, 240));
    dlib::cv_image<dlib::bgr_pixel> cimg(src_img);

    double thresh = 0.5;
    std::vector<dlib::rectangle> dets = detector(cimg, thresh);
    return 0;
  }

  inline std::vector<dlib::rectangle> getResult() { return mRets; }

 protected:
  std::vector<dlib::rectangle> mRets;
  std::string mModelPath;
  const int INPUT_IMG_MAX_SIZE = 800;
  const int INPUT_IMG_MIN_SIZE = 600;
};

/*
 * DLib face detect and face feature extractor
 */
class DLibHOGFaceDetector : public DLibHOGDetector {
 private:
  std::string mLandMarkModel;
  dlib::shape_predictor msp;
	std::unordered_map <int, dlib::full_object_detection> mFaceShapeMap;

 public:
  DLibHOGFaceDetector(std::string landmarkmodel = "")
      : mLandMarkModel(landmarkmodel) {
    if (!mLandMarkModel.empty()) {
      dlib::deserialize(mLandMarkModel) >> msp;
      LOG(INFO) << "Load landmarkmodel from " << mLandMarkModel;
    }
  }

  virtual inline int det(std::string path) {
    LOG(INFO) << "Read path from " << path;
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

    cv::Mat src_img = cv::imread(path, 1);
    dlib::cv_image<dlib::bgr_pixel> img(src_img);

    mRets = detector(img);
    LOG(INFO) << "Dlib HOG face det size : " << mRets.size();

		mFaceShapeMap.clear();
    if (mRets.size() != 0 && mLandMarkModel.empty() == false) {
      for (unsigned long j = 0; j < mRets.size(); ++j) {
        dlib::full_object_detection shape = msp(img, mRets[j]);
        LOG(INFO) << "face index:" << j << "number of parts: " << shape.num_parts();
        mFaceShapeMap[j] = shape;
      }
    }

    return mRets.size();
  }

  std::unordered_map<int, dlib::full_object_detection>& getFaceShapeMap() {
		return mFaceShapeMap;
	}
};

OpencvHOGDetctor* gOpencvHOGDetector = NULL;
DLibHOGDetector* gDLibHOGDetector = NULL;
DLibHOGFaceDetector* gDLibHOGFaceDetector = NULL;

#ifdef __cplusplus
extern "C" {
#endif

struct VisionDetRetOffsets {
  jfieldID label;
  jfieldID confidence;
  jfieldID left;
  jfieldID top;
  jfieldID right;
  jfieldID bottom;
} gVisionDetRetOffsets;

// ========================================================
// JNI Mapping Methods
// ========================================================
jint JNIEXPORT JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
  LOG(INFO) << "JNI On Load";
  JNIEnv* env = NULL;
  jint result = JNI_ERR;

  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
    LOG(FATAL) << "GetEnv failed!";
    return result;
  }

  return JNI_VERSION_1_6;
}

#define DLIB_JNI_METHOD(METHOD_NAME) \
  Java_com_tzutalin_dlib_PeopleDet_##METHOD_NAME

void JNIEXPORT DLIB_JNI_METHOD(jniNativeClassInit)(JNIEnv* _env, jclass _this) {
  jclass detRetClass = _env->FindClass("com/tzutalin/dlib/VisionDetRet");
  gVisionDetRetOffsets.label =
      _env->GetFieldID(detRetClass, "mLabel", "java/lang/String");
  gVisionDetRetOffsets.confidence =
      _env->GetFieldID(detRetClass, "mConfidence", "F");
  gVisionDetRetOffsets.left = _env->GetFieldID(detRetClass, "mLeft", "I");
  gVisionDetRetOffsets.top = _env->GetFieldID(detRetClass, "mTop", "I");
  gVisionDetRetOffsets.right = _env->GetFieldID(detRetClass, "mRight", "I");
  gVisionDetRetOffsets.bottom = _env->GetFieldID(detRetClass, "mBottom", "I");
  LOG(INFO) << "JniNativeClassIni Success";
}

jint JNIEXPORT JNICALL DLIB_JNI_METHOD(jniOpencvHOGDetect)(JNIEnv* env,
							   jobject thiz,
							   jstring imgPath) {
  LOG(INFO) << "com_tzutalin_dlib_PeopleDet jniOpencvHOGDetect";
  const char* img_path = env->GetStringUTFChars(imgPath, 0);
  if (gOpencvHOGDetector == NULL) gOpencvHOGDetector = new OpencvHOGDetctor();
  int nums = gOpencvHOGDetector->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  return nums;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniGetOpecvHOGRet)(JNIEnv* env, jobject thiz,
				       jobject detRet, jint index) {
  if (gOpencvHOGDetector) {
    cv::Rect rect = gOpencvHOGDetector->getResult()[index];
    env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.x);
    env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.y);
    env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.x + rect.width);
    env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.y + rect.height);
    env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
    jstring jstr = (jstring)(env->NewStringUTF("person"));
    env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);
    return JNI_OK;
  }

  return JNI_ERR;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniDLibHOGDetect)(JNIEnv* env, jobject thiz,
				      jstring imgPath, jstring modelPath) {
  LOG(INFO) << "com_tzutalin_dlib_PeopleDet jniDLibHOGDetect";
  const char* img_path = env->GetStringUTFChars(imgPath, 0);
  const char* model_path = env->GetStringUTFChars(imgPath, 0);
  if (gDLibHOGDetector == NULL)
    gDLibHOGDetector = new DLibHOGDetector(std::string(model_path));
  int size = gDLibHOGDetector->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  env->ReleaseStringUTFChars(modelPath, model_path);
  return size;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniGetDLibHOGRet)(JNIEnv* env, jobject thiz, jobject detRet,
				      jint index) {
  if (gDLibHOGDetector) {
    dlib::rectangle rect = gDLibHOGDetector->getResult()[index];
    env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.left());
    env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.top());
    env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.right());
    env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.bottom());
    env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
    jstring jstr = (jstring)(env->NewStringUTF("face"));
    env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);
    return JNI_OK;
  }

  return JNI_ERR;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniDLibHOGFaceDetect)(JNIEnv* env, jobject thiz,
					  jstring imgPath,
					  jstring landmarkPath) {
  LOG(INFO) << "com_tzutalin_dlib_PeopleDet jniDLibHOGFaceDetect";
  const char* img_path = env->GetStringUTFChars(imgPath, 0);
  const char* landmarkmodel_path = env->GetStringUTFChars(landmarkPath, 0);

  if (gDLibHOGFaceDetector == NULL)
    gDLibHOGFaceDetector = new DLibHOGFaceDetector(landmarkmodel_path);
  int size = gDLibHOGFaceDetector->det(std::string(img_path));

  env->ReleaseStringUTFChars(imgPath, img_path);
  env->ReleaseStringUTFChars(landmarkPath, landmarkmodel_path);
  return size;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniGetDLibHOGFaceRet)(JNIEnv* env, jobject thiz,
					  jobject detRet, jint index) {
  if (gDLibHOGFaceDetector) {
    dlib::rectangle rect = gDLibHOGFaceDetector->getResult()[index];
    env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.left());
    env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.top());
    env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.right());
    env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.bottom());
    env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
    jstring jstr = (jstring)(env->NewStringUTF("face"));
    env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);

		std::unordered_map<int, dlib::full_object_detection>& faceShapeMap = gDLibHOGFaceDetector->getFaceShapeMap();
		if (faceShapeMap.find(index) != faceShapeMap.end()) {
			dlib::full_object_detection shape = faceShapeMap[index];
			std::stringstream ss;
			// If landmarks exists, set label as "face_landmarks "
			if (shape.num_parts() > 0) {
				ss << "face_landmarks ";
			}
		  for (int i = 0 ; i != shape.num_parts(); i++) {
			  int x = shape.part(i).x();
			  int y = shape.part(i).y();
				ss << x << "," << y << ":";
		  }
			// TODO: Workaround. No availe time to better. It should be List<Point>
      jstring jstr = (jstring)(env->NewStringUTF(ss.str().c_str()));
      env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);
		}
    return JNI_OK;
  }

  return JNI_ERR;
}

jint JNIEXPORT JNICALL DLIB_JNI_METHOD(jniInit)(JNIEnv* env, jobject thiz) {
  return JNI_OK;
}

jint JNIEXPORT JNICALL DLIB_JNI_METHOD(jniDeInit)(JNIEnv* env, jobject thiz) {
  if (gDLibHOGDetector) delete gDLibHOGDetector;

  if (gOpencvHOGDetector) delete gOpencvHOGDetector;

  if (gDLibHOGFaceDetector) delete gDLibHOGFaceDetector;

  return JNI_OK;
}

#ifdef __cplusplus
}
#endif

int main() {
  const int INPUT_IMG_MAX_SIZE = 800;
  const int INPUT_IMG_MIN_SIZE = 600;
  std::string path = "/sdcard/test.jpg";

  cv::Mat src_img = cv::imread(path, 1);

  typedef dlib::scan_fhog_pyramid<dlib::pyramid_down<6> > image_scanner_type;
  dlib::object_detector<image_scanner_type> detector;
  dlib::deserialize("/sdcard/person.svm") >> detector;

  int img_width = src_img.cols;
  int img_height = src_img.rows;
  int im_size_min = MIN(img_width, img_height);
  int im_size_max = MAX(img_width, img_height);

  float scale = float(INPUT_IMG_MIN_SIZE) / float(im_size_min);

  if (scale * im_size_max > INPUT_IMG_MAX_SIZE) {
    scale = (float)INPUT_IMG_MAX_SIZE / (float)im_size_max;
  }

  if (scale != 1.0) {
    cv::Mat outputMat;
    cv::resize(src_img, outputMat, cv::Size(img_width * scale, img_height * scale));
    src_img = outputMat;
  }

  // cv::resize(src_img, src_img, cv::Size(320, 240));
  dlib::cv_image<dlib::bgr_pixel> cimg(src_img);

  double thresh = 0.5;
  std::vector<dlib::rectangle> dets = detector(cimg, thresh);
  std::cout << "size: " << dets.size() << std::endl;

  int i = 0;
  for (i = 0; i < dets.size(); i++) {
    dlib::rectangle dlibrect = dets[i];
    cv::Rect r(dlibrect.left(), dlibrect.top(), dlibrect.width(), dlibrect.height());
    r.x += cvRound(r.width * 0.1);
    r.width = cvRound(r.width * 0.8);
    r.y += cvRound(r.height * 0.06);
    r.height = cvRound(r.height * 0.9);
    cv::rectangle(src_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
  }
  path = "/sdcard/ret.jpg";
  cv::imwrite(path, src_img);
  return 0;
}
