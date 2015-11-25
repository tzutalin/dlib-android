LOCAL_PATH := $(call my-dir)

# =======================================================
include $(CLEAR_VARS)

OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(LOCAL_PATH)/../opencv/jni/OpenCV.mk

LOCAL_MODULE := people_det

LOCAL_C_INCLUDES :=  \
    $(LOCAL_PATH)/../opencv/jni/include \
    $(LOCAL_PATH)/../dlib \


LOCAL_SRC_FILES := ../dlib//dlib/threads/threads_kernel_shared.cpp \
				   ../dlib/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp \
				   ../dlib/dlib/base64/base64_kernel_1.cpp \
				   ../dlib/dlib/threads/threads_kernel_1.cpp \
				   ../dlib/dlib/threads/threads_kernel_2.cpp 

LOCAL_SRC_FILES += people_det.cpp

LOCAL_LDLIBS := -lm -llog -ldl -lz -ljnigraphics
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

LOCAL_ARM_MODE := arm
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
LOCAL_ARM_NEON := true
endif

#LOCAL_CFLAGS += -pie -fPIE
#LOCAL_LDFLAGS += -pie -fPIE
#include $(BUILD_EXECUTABLE)

include $(BUILD_SHARED_LIBRARY)
#-----------------------------------------------------------------------------
