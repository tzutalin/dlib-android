/*
 * jni_people_det.cpp using google-style
 *
 *  Created on: Oct 20, 2015
 *      Author: Tzutalin
 *
 *  Copyright (c) 2015 Tzutalin. All rights reserved.
 */
#include <android/bitmap.h>
#include <common/bitmap2mat.h>
#include <detector.h>
#include <jni.h>

using namespace cv;

namespace {
std::shared_ptr<OpencvHOGDetctor> gOpencvHOGDetectorPtr;
std::shared_ptr<DLibHOGDetector> gDLibHOGDetectorPtr;
std::shared_ptr<DLibHOGFaceDetector> gDLibHOGFaceDetectorPtr;
}  // end unnamespace

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
  jmethodID addLandmark;
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

void JNIEXPORT DLIB_JNI_METHOD(jniNativeClassInit)(JNIEnv* env, jclass _this) {
  jclass detRetClass = env->FindClass("com/tzutalin/dlib/VisionDetRet");
  CHECK_NOTNULL(detRetClass);
  gVisionDetRetOffsets.label =
      env->GetFieldID(detRetClass, "mLabel", "java/lang/String");
  gVisionDetRetOffsets.confidence =
      env->GetFieldID(detRetClass, "mConfidence", "F");
  gVisionDetRetOffsets.left = env->GetFieldID(detRetClass, "mLeft", "I");
  gVisionDetRetOffsets.top = env->GetFieldID(detRetClass, "mTop", "I");
  gVisionDetRetOffsets.right = env->GetFieldID(detRetClass, "mRight", "I");
  gVisionDetRetOffsets.bottom = env->GetFieldID(detRetClass, "mBottom", "I");
  gVisionDetRetOffsets.addLandmark =
      env->GetMethodID(detRetClass, "addLandmark", "(II)Z");
  if (gVisionDetRetOffsets.addLandmark == NULL) {
    LOG(FATAL) << "Can't Find Method addLandmark(int,int)";
  }
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
    gDLibHOGDetectorPtr =
        std::make_shared<DLibHOGDetector>(std::string(model_path));

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
    gDLibHOGFaceDetectorPtr =
        std::make_shared<DLibHOGFaceDetector>(landmarkmodel_path);

  int size = gDLibHOGFaceDetectorPtr->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  env->ReleaseStringUTFChars(landmarkPath, landmarkmodel_path);
  return size;
}

// Bitmap face detection
// Author:zhao
// Date:2016/5/10
JNIEXPORT jint JNICALL
    DLIB_JNI_METHOD(jniBitmapFaceDect)(JNIEnv* env, jobject thiz,
                                       jobject bitmap, jstring landmarkPath) {
  LOG(INFO) << "jniBitmapFaceDect";
  cv::Mat rgbaMat;
  cv::Mat bgrMat;
  jnicommon::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
  cv::cvtColor(rgbaMat, bgrMat, cv::COLOR_RGBA2BGR);
  const char* landmarkmodel_path = env->GetStringUTFChars(landmarkPath, 0);
  if (!gDLibHOGFaceDetectorPtr) {
    LOG(INFO) << "new DLibHOGFaceDetector, landmarkPath" << landmarkmodel_path;
    gDLibHOGFaceDetectorPtr =
        std::make_shared<DLibHOGFaceDetector>(landmarkmodel_path);
  }
  // Debug
  //cv::Mat rgbMat;
  //cv::cvtColor(bgrMat, rgbMat, cv::COLOR_BGR2RGB);
  //cv::imwrite("/sdcard/ret.jpg", rgbaMat);
  jint size = gDLibHOGFaceDetectorPtr->det(bgrMat);
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

    std::unordered_map<int, dlib::full_object_detection>& faceShapeMap =
        gDLibHOGFaceDetectorPtr->getFaceShapeMap();
    if (faceShapeMap.find(index) != faceShapeMap.end()) {
      dlib::full_object_detection shape = faceShapeMap[index];
      for (int i = 0; i != shape.num_parts(); i++) {
        // Add facelandmark
        int x = shape.part(i).x();
        int y = shape.part(i).y();
        // Call addLandmark
        env->CallBooleanMethod(detRet, gVisionDetRetOffsets.addLandmark, x, y);
      }
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
    cv::resize(src_img, outputMat,
               cv::Size(img_width * scale, img_height * scale));
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
    cv::Rect r(dlibrect.left(), dlibrect.top(), dlibrect.width(),
               dlibrect.height());
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
