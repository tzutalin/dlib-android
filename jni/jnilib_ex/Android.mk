LOCAL_PATH := $(call my-dir)

# =======================================================
include $(CLEAR_VARS)

# Define 3rd party path
TOP_LEVEL_PATH = $(LOCAL_PATH)/..
$(info TOP_LEVEL_PATH: $(TOP_LEVEL_PATH))
THIRD_PARTY_PATH = $(LOCAL_PATH)/../../third_party
OPENCV_PATH = $(THIRD_PARTY_PATH)/opencv
MINI_GLOG_PATH = $(THIRD_PARTY_PATH)/miniglog

OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(OPENCV_PATH)/jni/OpenCV.mk

LOCAL_MODULE := people_det

LOCAL_C_INCLUDES :=  \
	$(MINI_GLOG_PATH) \
	$(OPENCV_PATH)/jni/include \
	$(LOCAL_PATH)/../dlib

LOCAL_SRC_FILES := \
	../../third_party/miniglog/glog/logging.cc \
	../dlib//dlib/threads/threads_kernel_shared.cpp \
	../dlib/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp \
	../dlib/dlib/base64/base64_kernel_1.cpp \
	../dlib/dlib/threads/threads_kernel_1.cpp \
	../dlib/dlib/threads/threads_kernel_2.cpp

LOCAL_SRC_FILES += \
	       jni_people_det.cpp \
               jni_face_landmark.cpp

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
