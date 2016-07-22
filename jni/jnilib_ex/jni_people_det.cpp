/*
 * jni_people_det.cpp using google-style
 *
 *  Created on: Oct 20, 2015
 *      Author: Tzutalin
 *
 *  Copyright (c) 2015 Tzutalin. All rights reserved.
 */
#include <android/bitmap.h>
#include <detector.h>
#include <jni.h>

using namespace cv;

namespace {

static void convertBitmapToRgbaMat(JNIEnv * env, jobject& bitmap, Mat& dst, bool needUnPremultiplyAlpha) {
  AndroidBitmapInfo info;
  void* pixels = 0;

  try {
    CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
    CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
              info.format == ANDROID_BITMAP_FORMAT_RGB_565);
    CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
    CV_Assert(pixels);
    dst.create(info.height, info.width, CV_8UC4);
    if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
      LOG(INFO) << "nBitmapToMat: RGBA_8888 -> CV_8UC4";
      Mat tmp(info.height, info.width, CV_8UC4, pixels);
      if (needUnPremultiplyAlpha)
        cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
      else
        tmp.copyTo(dst);
    } else {
      // info.format == ANDROID_BITMAP_FORMAT_RGB_565
      LOG(INFO) << "nBitmapToMat: RGB_565 -> CV_8UC4";
      Mat tmp(info.height, info.width, CV_8UC2, pixels);
      cvtColor(tmp, dst, COLOR_BGR5652RGBA);
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    return;
  } catch (const cv::Exception& e) {
    AndroidBitmap_unlockPixels(env, bitmap);
    LOG(FATAL) << "nBitmapToMat catched cv::Exception:" << e.what();
    jclass je = env->FindClass("org/opencv/core/CvException");
    if (!je) je = env->FindClass("java/lang/Exception");
    env->ThrowNew(je, e.what());
    return;
  } catch (...) {
    AndroidBitmap_unlockPixels(env, bitmap);
    LOG(FATAL) << "nBitmapToMat catched unknown exception (...)";
    jclass je = env->FindClass("java/lang/Exception");
    env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
    return;
  }
}

std::shared_ptr<OpencvHOGDetctor> gOpencvHOGDetectorPtr;
std::shared_ptr<DLibHOGDetector> gDLibHOGDetectorPtr;
std::shared_ptr<DLibHOGFaceDetector> gDLibHOGFaceDetectorPtr;
} // end unnamespace

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
  if (!gOpencvHOGDetectorPtr)
    gOpencvHOGDetectorPtr = std::make_shared<OpencvHOGDetctor>();

  int nums = gOpencvHOGDetectorPtr->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  return nums;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniGetOpecvHOGRet)(JNIEnv* env, jobject thiz,
				       jobject detRet, jint index) {
  if (gOpencvHOGDetectorPtr) {
    cv::Rect rect = gOpencvHOGDetectorPtr->getResult()[index];
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
  if (!gDLibHOGDetectorPtr)
    gDLibHOGDetectorPtr = std::make_shared<DLibHOGDetector>(std::string(model_path));

  int size = gDLibHOGDetectorPtr->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  env->ReleaseStringUTFChars(modelPath, model_path);
  return size;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniGetDLibHOGRet)(JNIEnv* env, jobject thiz, jobject detRet,
				      jint index) {
  if (gDLibHOGDetectorPtr) {
    dlib::rectangle rect = gDLibHOGDetectorPtr->getResult()[index];
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
  LOG(INFO) << "jniDLibHOGFaceDetect";
  const char* img_path = env->GetStringUTFChars(imgPath, 0);
  const char* landmarkmodel_path = env->GetStringUTFChars(landmarkPath, 0);

  if (!gDLibHOGFaceDetectorPtr)
    gDLibHOGFaceDetectorPtr = std::make_shared<DLibHOGFaceDetector>(landmarkmodel_path);

  int size = gDLibHOGFaceDetectorPtr->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  env->ReleaseStringUTFChars(landmarkPath, landmarkmodel_path);
  return size;
}

//Bitmap face detection
//Author:zhao
//Date:2016/5/10
JNIEXPORT jint JNICALL DLIB_JNI_METHOD(jniBitmapFaceDect)
  (JNIEnv *env, jobject thiz, jobject bitmap, jstring landmarkPath){
	LOG(INFO) << "jniBitmapFaceDect";
	cv::Mat rgbaMat;
	cv::Mat brgMat;
  convertBitmapToRgbaMat(env, bitmap, rgbaMat, true);
  cv::cvtColor(rgbaMat, brgMat, cv::COLOR_RGBA2BGR);
	const char* landmarkmodel_path = env->GetStringUTFChars(landmarkPath, 0);
	if (!gDLibHOGFaceDetectorPtr){
      LOG(INFO) << "new DLibHOGFaceDetector, landmarkPath" << landmarkmodel_path;
      gDLibHOGFaceDetectorPtr = std::make_shared<DLibHOGFaceDetector>(landmarkmodel_path);
	}
  //cv::imwrite("/sdcard/ret.jpg", rgbaMat);
	jint size = gDLibHOGFaceDetectorPtr->det(brgMat);
	LOG(INFO) << "det face size: " << size;
	env->ReleaseStringUTFChars(landmarkPath, landmarkmodel_path);
	return size;
}

jint JNIEXPORT JNICALL
    DLIB_JNI_METHOD(jniGetDLibHOGFaceRet)(JNIEnv* env, jobject thiz,
					  jobject detRet, jint index) {
  if (gDLibHOGFaceDetectorPtr) {
    dlib::rectangle rect = gDLibHOGFaceDetectorPtr->getResult()[index];
    env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.left());
    env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.top());
    env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.right());
    env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.bottom());
    env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
    jstring jstr = (jstring)(env->NewStringUTF("face"));
    env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);

		std::unordered_map<int, dlib::full_object_detection>& faceShapeMap = gDLibHOGFaceDetectorPtr->getFaceShapeMap();
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
  LOG(INFO) << "jniDeInit";
  gDLibHOGDetectorPtr.reset();
  gOpencvHOGDetectorPtr.reset();
  gDLibHOGFaceDetectorPtr.reset();
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
