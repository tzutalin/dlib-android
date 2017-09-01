LOCAL_PATH := $(call my-dir)
# C++ full library
# =======================================================
## Import JNI
include $(CLEAR_VARS)

LOCAL_MODULE := example

JNI_CC := example.cpp

LOCAL_SRC_FILES += $(foreach jni_cxx_src, $(JNI_CC), $(jni_cxx_src))

#MINIGLOG_LIB_TYPE:=SHARED
MINIGLOG_LIB_TYPE:=STATIC
# glog start
ifeq ($(MINIGLOG_LIB_TYPE),SHARED)
    LOCAL_SHARED_LIBRARIES += miniglog
else
    LOCAL_STATIC_LIBRARIES += miniglog
endif
# glog end

LOCAL_CPPFLAGS += -pthread -fPIC -fexceptions -frtti -std=c++11
LOCAL_LDLIBS += -lm -llog

TARGET_OUT='jniLibs/$(TARGET_ARCH_ABI)'

include $(BUILD_EXECUTABLE)

# Include MINIGLOG.mk
#include $(LOCAL_PATH)/../../MINIGLOG.mk
include $(LOCAL_PATH)/../../Android.mk
