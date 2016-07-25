LOCAL_PATH := $(call my-dir)

# =======================================================
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(OPENCV_PATH)/OpenCV.mk

LOCAL_MODULE := people_det

LOCAL_C_INCLUDES +=  \
           $(OPENCV_INCLUDE_DIR)

LOCAL_SRC_FILES += \
	       jni_people_det.cpp \
           imageutils_jni.cpp \
           common/rgb2yuv.cpp \
           common/yuv2rgb.cpp \
           common/bitmap2mat.cpp

LOCAL_LDLIBS += -lm -llog -ldl -lz -ljnigraphics
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

# import dlib
LOCAL_STATIC_LIBRARIES += dlib

### import miniglog
ifeq ($(MINIGLOG_LIB_TYPE),SHARED)
    LOCAL_SHARED_LIBRARIES += miniglog
else
    LOCAL_STATIC_LIBRARIES += miniglog
endif

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
	LOCAL_ARM_NEON := true
endif

include $(BUILD_SHARED_LIBRARY)
#-----------------------------------------------------------------------------
