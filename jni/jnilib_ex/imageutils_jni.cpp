/*
 * imageutils_jni.cpp using google-style
 *
 *  Created on: May 24, 2016
 *      Author: Tzutalin
 *
 *  Copyright (c) 2016 Tzutalin. All rights reserved.
 */
#include <common/rgb2yuv.h>
#include <common/types.h>
#include <common/yuv2rgb.h>
#include <glog/logging.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#define IMAGEUTILS_METHOD(METHOD_NAME) \
  Java_com_tzutalin_dlibtest_ImageUtils_##METHOD_NAME  // NOLINT

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL IMAGEUTILS_METHOD(convertYUV420SPToARGB8888)(
    JNIEnv* env, jclass clazz, jbyteArray input, jintArray output, jint width,
    jint height, jboolean halfSize);

JNIEXPORT void JNICALL IMAGEUTILS_METHOD(convertYUV420ToARGB8888)(
    JNIEnv* env, jclass clazz, jbyteArray y, jbyteArray u, jbyteArray v,
    jintArray output, jint width, jint height, jint y_row_stride,
    jint uv_row_stride, jint uv_pixel_stride, jboolean halfSize);

JNIEXPORT void JNICALL
    IMAGEUTILS_METHOD(convertYUV420SPToRGB565)(JNIEnv* env, jclass clazz,
                                               jbyteArray input,
                                               jbyteArray output, jint width,
                                               jint height);

JNIEXPORT void JNICALL
    IMAGEUTILS_METHOD(convertARGB8888ToYUV420SP)(JNIEnv* env, jclass clazz,
                                                 jintArray input,
                                                 jbyteArray output, jint width,
                                                 jint height);

JNIEXPORT void JNICALL
    IMAGEUTILS_METHOD(convertRGB565ToYUV420SP)(JNIEnv* env, jclass clazz,
                                               jbyteArray input,
                                               jbyteArray output, jint width,
                                               jint height);

#ifdef __cplusplus
}
#endif

JNIEXPORT void JNICALL IMAGEUTILS_METHOD(convertYUV420SPToARGB8888)(
    JNIEnv* env, jclass clazz, jbyteArray input, jintArray output, jint width,
    jint height, jboolean halfSize) {
  jboolean inputCopy = JNI_FALSE;
  jbyte* const i = env->GetByteArrayElements(input, &inputCopy);

  jboolean outputCopy = JNI_FALSE;
  jint* const o = env->GetIntArrayElements(output, &outputCopy);

  if (halfSize) {
    ConvertYUV420SPToARGB8888HalfSize(reinterpret_cast<uint8*>(i),
                                      reinterpret_cast<uint32*>(o), width,
                                      height);
  } else {
    ConvertYUV420SPToARGB8888(reinterpret_cast<uint8*>(i),
                              reinterpret_cast<uint8*>(i) + width * height,
                              reinterpret_cast<uint32*>(o), width, height);
  }

  env->ReleaseByteArrayElements(input, i, JNI_ABORT);
  env->ReleaseIntArrayElements(output, o, 0);
}

JNIEXPORT void JNICALL IMAGEUTILS_METHOD(convertYUV420ToARGB8888)(
    JNIEnv* env, jclass clazz, jbyteArray y, jbyteArray u, jbyteArray v,
    jintArray output, jint width, jint height, jint y_row_stride,
    jint uv_row_stride, jint uv_pixel_stride, jboolean halfSize) {
  jboolean inputCopy = JNI_FALSE;
  jbyte* const y_buff = env->GetByteArrayElements(y, &inputCopy);
  jboolean outputCopy = JNI_FALSE;
  jint* const o = env->GetIntArrayElements(output, &outputCopy);

  if (halfSize) {
    ConvertYUV420SPToARGB8888HalfSize(reinterpret_cast<uint8*>(y_buff),
                                      reinterpret_cast<uint32*>(o), width,
                                      height);
  } else {
    jbyte* const u_buff = env->GetByteArrayElements(u, &inputCopy);
    jbyte* const v_buff = env->GetByteArrayElements(v, &inputCopy);

    ConvertYUV420ToARGB8888(
        reinterpret_cast<uint8*>(y_buff), reinterpret_cast<uint8*>(u_buff),
        reinterpret_cast<uint8*>(v_buff), reinterpret_cast<uint32*>(o), width,
        height, y_row_stride, uv_row_stride, uv_pixel_stride);

    env->ReleaseByteArrayElements(u, u_buff, JNI_ABORT);
    env->ReleaseByteArrayElements(v, v_buff, JNI_ABORT);
  }

  env->ReleaseByteArrayElements(y, y_buff, JNI_ABORT);
  env->ReleaseIntArrayElements(output, o, 0);
}

JNIEXPORT void JNICALL
    IMAGEUTILS_METHOD(convertYUV420SPToRGB565)(JNIEnv* env, jclass clazz,
                                               jbyteArray input,
                                               jbyteArray output, jint width,
                                               jint height) {
  jboolean inputCopy = JNI_FALSE;
  jbyte* const i = env->GetByteArrayElements(input, &inputCopy);

  jboolean outputCopy = JNI_FALSE;
  jbyte* const o = env->GetByteArrayElements(output, &outputCopy);

  ConvertYUV420SPToRGB565(reinterpret_cast<uint8*>(i),
                          reinterpret_cast<uint16*>(o), width, height);

  env->ReleaseByteArrayElements(input, i, JNI_ABORT);
  env->ReleaseByteArrayElements(output, o, 0);
}

JNIEXPORT void JNICALL
    IMAGEUTILS_METHOD(convertARGB8888ToYUV420SP)(JNIEnv* env, jclass clazz,
                                                 jintArray input,
                                                 jbyteArray output, jint width,
                                                 jint height) {
  jboolean inputCopy = JNI_FALSE;
  jint* const i = env->GetIntArrayElements(input, &inputCopy);

  jboolean outputCopy = JNI_FALSE;
  jbyte* const o = env->GetByteArrayElements(output, &outputCopy);

  ConvertARGB8888ToYUV420SP(reinterpret_cast<uint32*>(i),
                            reinterpret_cast<uint8*>(o), width, height);

  env->ReleaseIntArrayElements(input, i, JNI_ABORT);
  env->ReleaseByteArrayElements(output, o, 0);
}

JNIEXPORT void JNICALL
    IMAGEUTILS_METHOD(convertRGB565ToYUV420SP)(JNIEnv* env, jclass clazz,
                                               jbyteArray input,
                                               jbyteArray output, jint width,
                                               jint height) {
  jboolean inputCopy = JNI_FALSE;
  jbyte* const i = env->GetByteArrayElements(input, &inputCopy);

  jboolean outputCopy = JNI_FALSE;
  jbyte* const o = env->GetByteArrayElements(output, &outputCopy);

  ConvertRGB565ToYUV420SP(reinterpret_cast<uint16*>(i),
                          reinterpret_cast<uint8*>(o), width, height);

  env->ReleaseByteArrayElements(input, i, JNI_ABORT);
  env->ReleaseByteArrayElements(output, o, 0);
}
