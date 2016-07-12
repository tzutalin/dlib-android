LOCAL_PATH := $(call my-dir)

# =======================================================
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(OPENCV_PATH)/OpenCV.mk

# import dlib
LOCAL_STATIC_LIBRARIES += dlib

LOCAL_MODULE := people_det

LOCAL_C_INCLUDES +=  \
           $(MINI_GLOG_INCLUDE_DIR) \
           $(OPENCV_INCLUDE_DIR)

LOCAL_SRC_FILES += \
           $(MINI_GLOG_PATH)/glog/logging.cc \
	       jni_people_det.cpp \
           imageutils_jni.cpp \
           common/rgb2yuv.cpp \
           common/yuv2rgb.cpp

LOCAL_LDLIBS += -lm -llog -ldl -lz -ljnigraphics
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
	LOCAL_ARM_NEON := true
endif

include $(BUILD_SHARED_LIBRARY)
#-----------------------------------------------------------------------------
