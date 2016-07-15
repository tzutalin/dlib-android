LOCAL_PATH := $(call my-dir)
SUB_MK_FILES := $(call all-subdir-makefiles)

## Build dlib to static library
include $(CLEAR_VARS)
LOCAL_MODULE := dlib
LOCAL_C_INCLUDE := $(LOCAL_PATH)/dlib

LOCAL_SRC_FILES += \
                ../$(LOCAL_PATH)/dlib//dlib/threads/threads_kernel_shared.cpp \
                ../$(LOCAL_PATH)/dlib/dlib/entropy_decoder/entropy_decoder_kernel_2.cpp \
                ../$(LOCAL_PATH)/dlib/dlib/base64/base64_kernel_1.cpp \
                ../$(LOCAL_PATH)/dlib/dlib/threads/threads_kernel_1.cpp \
                ../$(LOCAL_PATH)/dlib/dlib/threads/threads_kernel_2.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDE)
include $(BUILD_STATIC_LIBRARY)

TOP_LEVEL_PATH := $(abspath $(LOCAL_PATH)/..)
$(info TOP Level Path: $(TOP_LEVEL_PATH))

EXT_INSTALL_PATH = $(TOP_LEVEL_PATH)/third_party

OPENCV_PATH = $(EXT_INSTALL_PATH)/opencv/jni
OPENCV_INCLUDE_DIR = $(OPENCV_PATH)/include

### Test daemon
#==========================svm_ex===================================
include $(CLEAR_VARS)
LOCAL_MODULE := svm_ex
LOCAL_SRC_FILES := dlib/examples/svm_ex.cpp
LOCAL_STATIC_LIBRARIES := dlib
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#==========================svm_ex===================================


#====================max_cost_assignment_ex=========================
include $(CLEAR_VARS)
LOCAL_MODULE := max_cost_assignment_ex
LOCAL_SRC_FILES := dlib/examples/max_cost_assignment_ex.cpp
LOCAL_STATIC_LIBRARIES := dlib
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#====================max_cost_assignment_ex=========================


#============================svr_ex================================
include $(CLEAR_VARS)
LOCAL_MODULE := svr_ex
LOCAL_SRC_FILES := dlib/examples/svr_ex.cpp
LOCAL_STATIC_LIBRARIES := dlib
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#============================svr_ex================================

MINIGLOG_LIB_TYPE := STATIC
MINI_GLOG_PATH = $(EXT_INSTALL_PATH)/miniglog
include $(MINI_GLOG_PATH)/Android.mk

include $(SUB_MK_FILES)
