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
std::shared_ptr<DLibHOGFaceDetector> gDLibHOGFaceLandmarkDetectorPtr;
std::shared_ptr<DLibHOGFaceDetector> gDLibHOGFaceDetectorPtr;

struct VisionDetRetOffsets {
  jfieldID label;
  jfieldID confidence;
  jfieldID left;
  jfieldID top;
  jfieldID right;
  jfieldID bottom;
  jmethodID addLandmark;
} gVisionDetRetOffsets;

}  // end unnamespace

#ifdef __cplusplus
extern "C" {
#endif




#define DLIB_FACE_JNI_METHOD(METHOD_NAME) \
  Java_com_tzutalin_dlib_FaceDet_##METHOD_NAME

void JNIEXPORT DLIB_FACE_JNI_METHOD(jniNativeClassInit)(JNIEnv* env, jclass _this) {
  jclass detRetClass = env->FindClass("com/tzutalin/dlib/VisionDetRet");
  CHECK_NOTNULL(detRetClass);
  gVisionDetRetOffsets.label =
      env->GetFieldID(detRetClass, "mLabel", "Ljava/lang/String;");
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

jobjectArray getFaceRet(JNIEnv* env, std::shared_ptr<DLibHOGFaceDetector> faceDetector, int size){
  LOG(INFO) << "getFaceRet";
  jclass detRetClass = env->FindClass("com/tzutalin/dlib/VisionDetRet");

  jmethodID mid = env->GetMethodID(detRetClass,"<init>", "()V");

  jobjectArray detRetArr = (jobjectArray)env->NewObjectArray(size, detRetClass, NULL);
  for(int i =0; i < size; i++){
      jobject detRet = env->NewObject(detRetClass, mid);
      env->SetObjectArrayElement(detRetArr, i, detRet);

      dlib::rectangle rect = faceDetector->getResult()[i];
      env->SetIntField(detRet, gVisionDetRetOffsets.left, rect.left());
      env->SetIntField(detRet, gVisionDetRetOffsets.top, rect.top());
      env->SetIntField(detRet, gVisionDetRetOffsets.right, rect.right());
      env->SetIntField(detRet, gVisionDetRetOffsets.bottom, rect.bottom());
      env->SetFloatField(detRet, gVisionDetRetOffsets.confidence, 0);
      jstring jstr = (jstring)(env->NewStringUTF("face"));
      env->SetObjectField(detRet, gVisionDetRetOffsets.label, (jobject)jstr);

      std::unordered_map<int, dlib::full_object_detection>& faceShapeMap =
          faceDetector->getFaceShapeMap();
      if (faceShapeMap.find(i) != faceShapeMap.end()) {
        dlib::full_object_detection shape = faceShapeMap[i];
        for (int j = 0; j != shape.num_parts(); j++) {
          // Add facelandmark
          int x = shape.part(j).x();
          int y = shape.part(j).y();
          // Call addLandmark
          env->CallBooleanMethod(detRet, gVisionDetRetOffsets.addLandmark, x, y);
        }
      }
  }
  return detRetArr;
}

JNIEXPORT jobjectArray JNICALL
    DLIB_FACE_JNI_METHOD(jniFaceLandmarkDet)(JNIEnv* env, jobject thiz,
                                          jstring imgPath,
                                          jstring landmarkPath) {
  LOG(INFO) << "jniFaceLandmarkDet";
  const char* img_path = env->GetStringUTFChars(imgPath, 0);
  const char* landmarkmodel_path = env->GetStringUTFChars(landmarkPath, 0);

  if (!gDLibHOGFaceLandmarkDetectorPtr)
    gDLibHOGFaceLandmarkDetectorPtr =
        std::make_shared<DLibHOGFaceDetector>(landmarkmodel_path);

  int size = gDLibHOGFaceLandmarkDetectorPtr->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  env->ReleaseStringUTFChars(landmarkPath, landmarkmodel_path);
  LOG(INFO) << "det face size: " << size;
  return getFaceRet(env,gDLibHOGFaceLandmarkDetectorPtr, size);
}

// Bitmap face detection
// Author:zhao
// Date:2016/5/10
JNIEXPORT jobjectArray JNICALL
    DLIB_FACE_JNI_METHOD(jniBitmapFaceLandmarkDet)(JNIEnv* env, jobject thiz,
                                          jobject bitmap,
                                          jstring landmarkPath) {
  LOG(INFO) << "jniBitmapFaceLandmarkDet";
  cv::Mat rgbaMat;
  cv::Mat bgrMat;
  jnicommon::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
  cv::cvtColor(rgbaMat, bgrMat, cv::COLOR_RGBA2BGR);
  const char* landmarkmodel_path = env->GetStringUTFChars(landmarkPath, 0);
  if (!gDLibHOGFaceLandmarkDetectorPtr) {
    LOG(INFO) << "new DLibHOGFaceDetector, landmarkPath" << landmarkmodel_path;
    gDLibHOGFaceLandmarkDetectorPtr =
        std::make_shared<DLibHOGFaceDetector>(landmarkmodel_path);
  }
  // Debug
  //cv::Mat rgbMat;
  //cv::cvtColor(bgrMat, rgbMat, cv::COLOR_BGR2RGB);
  //cv::imwrite("/sdcard/ret.jpg", rgbaMat);
  jint size = gDLibHOGFaceLandmarkDetectorPtr->det(bgrMat);
  LOG(INFO) << "det face size: " << size;
  env->ReleaseStringUTFChars(landmarkPath, landmarkmodel_path);
  return getFaceRet(env,gDLibHOGFaceLandmarkDetectorPtr,size);
}

JNIEXPORT jobjectArray JNICALL
    DLIB_FACE_JNI_METHOD(jniFaceDet)(JNIEnv* env, jobject thiz, jstring imgPath){
  LOG(INFO) << "jniFaceDet";

  const char* img_path = env->GetStringUTFChars(imgPath, 0);

  if (!gDLibHOGFaceDetectorPtr)
    gDLibHOGFaceDetectorPtr =
            std::make_shared<DLibHOGFaceDetector>();

  int size = gDLibHOGFaceDetectorPtr->det(std::string(img_path));
  env->ReleaseStringUTFChars(imgPath, img_path);
  LOG(INFO) << "det face size: " << size;
  return getFaceRet(env,gDLibHOGFaceDetectorPtr,size);
}

JNIEXPORT jobjectArray JNICALL
    DLIB_FACE_JNI_METHOD(jniBitmapFaceDet)(JNIEnv* env, jobject thiz, jobject bitmap){
  LOG(INFO) << "jniBitmapFaceDet";
  cv::Mat rgbaMat;
  cv::Mat bgrMat;
  jnicommon::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
  cv::cvtColor(rgbaMat, bgrMat, cv::COLOR_RGBA2BGR);
  if (!gDLibHOGFaceDetectorPtr) {
    gDLibHOGFaceDetectorPtr =
        std::make_shared<DLibHOGFaceDetector>();
  }

  jint size = gDLibHOGFaceDetectorPtr->det(bgrMat);
  LOG(INFO) << "det face size: " << size;

  return getFaceRet(env,gDLibHOGFaceDetectorPtr,size);
}


jint JNIEXPORT JNICALL DLIB_FACE_JNI_METHOD(jniInit)(JNIEnv* env, jobject thiz) {
  return JNI_OK;
}

jint JNIEXPORT JNICALL DLIB_FACE_JNI_METHOD(jniDeInit)(JNIEnv* env, jobject thiz) {
  LOG(INFO) << "jniDeInit";
  gDLibHOGFaceDetectorPtr.reset();
  gDLibHOGFaceLandmarkDetectorPtr.reset();
  return JNI_OK;
}

#ifdef __cplusplus
}
#endif
