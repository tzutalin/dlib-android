LOCAL_PATH := $(call my-dir)
SUB_MK_FILES := $(call all-subdir-makefiles)

## Build dlib to static library
include $(CLEAR_VARS)
LOCAL_MODULE := dlib
LOCAL_C_INCLUDES := dlib

LOCAL_SRC_FILES += \
                dlib/dlib/threads/threads_kernel_shared.cpp \
                dlib/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp \
                dlib/dlib/base64/base64_kernel_1.cpp \
                dlib/dlib/threads/threads_kernel_1.cpp \
                dlib/dlib/threads/threads_kernel_2.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_STATIC_LIBRARY)

EXT_INSTALL_PATH = third_party

OPENCV_PATH := $(LOCAL_PATH)/$(EXT_INSTALL_PATH)/OpenCV-android-sdk/sdk/native/jni
OPENCV_INCLUDE_DIR = $(OPENCV_PATH)/include

MINIGLOG_LIB_TYPE := STATIC
MINI_GLOG_PATH = $(EXT_INSTALL_PATH)/miniglog
include $(MINI_GLOG_PATH)/Android.mk

include $(SUB_MK_FILES)
