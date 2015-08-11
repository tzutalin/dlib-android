LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := dlib

LOCAL_C_INCLUDE := $(LOCAL_PATH)/dlib

FILE_LIST := $(wildcard $(LOCAL_PATH)/dlib/image_saver/*.cpp)
#FILE_LIST := $(wildcard $(LOCAL_PATH)/dlib/*.cpp)
#FILE_LIST += $(wildcard $(LOCAL_PATH)/dlib/threads/*.cpp)
#FILE_LIST += $(wildcard $(LOCAL_PATH)/dlib/logger/*.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(BUILD_STATIC_LIBRARY)
