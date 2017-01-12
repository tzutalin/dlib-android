cmake_minimum_required(VERSION 3.4.1)

message("Checking CMAKE_SYSTEM_NAME = '${CMAKE_SYSTEM_NAME}'")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  add_definitions(-DOS_OSX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  add_definitions(-DOS_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  add_definitions(-DOS_WIN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Android")
  add_definitions(-DOS_ANDROID)
  message("Checking CMAKE_ABI_NAME = '${CMAKE_ANDROID_ARCH_ABI}'")
else()
  message("OS not detected.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

## Define each subfolders
set(JNI_DETECTION_INCLUDE jni/jni_detections)
set(JNI_DETECTION_SRC jni/jni_detections)
set(JNI_COMMON_INCLUDE jni)
set(JNI_COMMON_SRC jni/jni_common)
set(DLIB_DIR dlib)
set(EXT_DIR third_party)
set(GLOG_INCLUDE_DIR ${EXT_DIR}/miniglog)
set(OPENCV_PREBUILT ${EXT_DIR}/OpenCV-android-sdk/sdk/native/jni)

# Opencv and it will use static import
set(ANDROID_NDK_ABI_NAME ${CMAKE_ANDROID_ARCH_ABI})
include(${OPENCV_PREBUILT}/OpenCVConfig.cmake)

# Include headers
include_directories(${DLIB_DIR} ${OpenCV_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR} ${JNI_COMMON_INCLUDE} ${JNI_DETECTION_INCLUDE} include)

add_library(android_dlib SHARED
            ${JNI_DETECTION_SRC}/jni_face_det.cpp
            ${JNI_DETECTION_SRC}/jni_imageutils.cpp
            ${JNI_DETECTION_SRC}/jni_pedestrian_det.cpp
            ${JNI_COMMON_SRC}/jni_bitmap2mat.cpp
            ${JNI_COMMON_SRC}/jni_fileutils.cpp
            ${JNI_COMMON_SRC}/jni_utils.cpp
            ${JNI_COMMON_SRC}/rgb2yuv.cpp
            ${JNI_COMMON_SRC}/yuv2rgb.cpp
            ${DLIB_DIR}//dlib/threads/threads_kernel_shared.cpp
            ${DLIB_DIR}/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp
            ${DLIB_DIR}/dlib/base64/base64_kernel_1.cpp
            ${DLIB_DIR}/dlib/threads/threads_kernel_1.cpp
            ${DLIB_DIR}/dlib/threads/threads_kernel_2.cpp
            ${EXT_DIR}/miniglog/glog/logging.cc)

target_link_libraries(android_dlib
                      android
                      jnigraphics
                      z
                      m
                      dl
                      log)

