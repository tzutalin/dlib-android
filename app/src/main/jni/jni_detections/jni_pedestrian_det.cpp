/*
 * jni_people_det.cpp using google-style
 *
 *  Created on: Oct 20, 2015
 *      Author: Tzutalin
 *
 *  Copyright (c) 2015 Tzutalin. All rights reserved.
 */
#include <android/bitmap.h>
#include <jni_common/jni_bitmap2mat.h>
#include <jni_common/jni_primitives.h>
#include <jni_common/jni_fileutils.h>
#include <jni_common/jni_utils.h>
#include <detector.h>
#include <jni.h>

using namespace cv;
extern JNI_VisionDetRet* g_pJNI_VisionDetRet;

namespace {

#define JAVA_NULL 0
using DetectorPtr = OpencvHOGDetctor*;

class JNI_PedestrianDet {
 public:
  JNI_PedestrianDet(JNIEnv* env) {
    jclass clazz = env->FindClass(CLASSNAME_PEDESTRIAN_DET);
    mNativeContext = env->GetFieldID(clazz, "mNativeDetContext", "J");
    env->DeleteLocalRef(clazz);
  }

  DetectorPtr getDetectorPtrFromJava(JNIEnv* env, jobject thiz) {
    DetectorPtr const p = (DetectorPtr)env->GetLongField(thiz, mNativeContext);
    return p;
  }

  void setDetectorPtrToJava(JNIEnv* env, jobject thiz, jlong ptr) {
    env->SetLongField(thiz, mNativeContext, ptr);
  }

  jfieldID mNativeContext;
};

// Protect getting/setting and creating/deleting pointer between java/native
std::mutex gLock;

std::shared_ptr<JNI_PedestrianDet> getJNI_PedestrianDet(JNIEnv* env) {
  static std::once_flag sOnceInitflag;
  static std::shared_ptr<JNI_PedestrianDet> sJNI_PedestrianDet;
  std::call_once(sOnceInitflag, [env]() {
    sJNI_PedestrianDet = std::make_shared<JNI_PedestrianDet>(env);
  });
  return sJNI_PedestrianDet;
}

DetectorPtr const getDetectorPtr(JNIEnv* env, jobject thiz) {
  std::lock_guard<std::mutex> lock(gLock);
  return getJNI_PedestrianDet(env)->getDetectorPtrFromJava(env, thiz);
}

// The function to set a pointer to java and delete it if newPtr is empty
void setDetectorPtr(JNIEnv* env, jobject thiz, DetectorPtr newPtr) {
  std::lock_guard<std::mutex> lock(gLock);
  DetectorPtr oldPtr =
      getJNI_PedestrianDet(env)->getDetectorPtrFromJava(env, thiz);
  if (oldPtr != JAVA_NULL) {
    DLOG(INFO) << "setMapManager delete old ptr : " << oldPtr;
    delete oldPtr;
  }

  if (newPtr != JAVA_NULL) {
    DLOG(INFO) << "setMapManager set new ptr : " << newPtr;
  }

  getJNI_PedestrianDet(env)->setDetectorPtrToJava(env, thiz, (jlong)newPtr);
}

}  // end unnamespace

#ifdef __cplusplus
extern "C" {
#endif

// ========================================================
// JNI Mapping Methods
// ========================================================
#define DLIB_JNI_METHOD(METHOD_NAME) \
  Java_com_tzutalin_dlib_PedestrianDet_##METHOD_NAME

jobjectArray getDetRet(JNIEnv* env, DetectorPtr detectorPtr, const int& size) {
  LOG(INFO) << "getDetRet";
  jobjectArray jDetRetArray = JNI_VisionDetRet::createJObjectArray(env, size);
  for (int i = 0; i < size; i++) {
    jobject jDetRet = JNI_VisionDetRet::createJObject(env);
    env->SetObjectArrayElement(jDetRetArray, i, jDetRet);
    cv::Rect rect = detectorPtr->getResult()[i];
    g_pJNI_VisionDetRet->setRect(env, jDetRet, rect.x, rect.y,
                                 rect.x + rect.width, rect.y + rect.height);
    g_pJNI_VisionDetRet->setLabel(env, jDetRet, "Pedestrian");
  }
  return jDetRetArray;
}

JNIEXPORT jobjectArray JNICALL
    DLIB_JNI_METHOD(jniDetect)(JNIEnv* env, jobject thiz, jstring jImgPath) {
  LOG(INFO) << "jniPeopleDet";
  std::string path = jniutils::convertJStrToString(env, jImgPath);
  cv::Mat src_img = cv::imread(path, CV_LOAD_IMAGE_COLOR);
  DetectorPtr detPtr = getDetectorPtr(env, thiz);
  int size = detPtr->det(src_img);
  LOG(INFO) << "jniPeopleDet size: " << size;
  return getDetRet(env, detPtr, size);
}

JNIEXPORT jobjectArray JNICALL
    DLIB_JNI_METHOD(jniBitmapDetect)(JNIEnv* env, jobject thiz,
                                     jobject bitmap) {
  LOG(INFO) << "jniBitmapPeopleDet";
  cv::Mat rgbaMat;
  cv::Mat bgrMat;
  jniutils::ConvertBitmapToRGBAMat(env, bitmap, rgbaMat, true);
  cv::cvtColor(rgbaMat, bgrMat, cv::COLOR_RGBA2BGR);
  DetectorPtr detPtr = getDetectorPtr(env, thiz);
  jint size = detPtr->det(bgrMat);
  LOG(INFO) << "jniBitmapPeopleDet size: " << size;
  return getDetRet(env, detPtr, size);
}

jint JNIEXPORT JNICALL DLIB_JNI_METHOD(jniInit)(JNIEnv* env, jobject thiz) {
  LOG(INFO) << "jniInit";
  DetectorPtr detPtr = new OpencvHOGDetctor();
  setDetectorPtr(env, thiz, detPtr);
  ;
  return JNI_OK;
  return JNI_OK;
}

jint JNIEXPORT JNICALL DLIB_JNI_METHOD(jniDeInit)(JNIEnv* env, jobject thiz) {
  LOG(INFO) << "jniDeInit";
  return JNI_OK;
}

#ifdef __cplusplus
}
#endif
