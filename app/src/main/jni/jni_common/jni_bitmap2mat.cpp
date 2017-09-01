/*
 * bitmap2mat.cpp using google-style
 *
 *     Author : TzuTaLin
 * Created on : 07/25 2016
 *
 * Copyright (c) 2016 Tzutalin. All rights reserved.
 */

#include "jni_bitmap2mat.h"
namespace jniutils {

using namespace cv;

void ConvertBitmapToRGBAMat(JNIEnv* env, jobject& bitmap, Mat& dst,
                            bool needUnPremultiplyAlpha) {
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
    if (!je)
      je = env->FindClass("java/lang/Exception");
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

}  // end jniutils
