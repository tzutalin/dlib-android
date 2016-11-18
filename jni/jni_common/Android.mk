LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(OPENCV_PATH)/OpenCV.mk


LOCAL_MODULE           := jni_common
$(warning "Build jni_common")
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/..
LOCAL_C_INCLUDES +=  \
           $(OPENCV_INCLUDE_DIR)

$(warning $(LOCAL_PATH))

all_cpp_files_recursively = \
 $(eval src_files = $(wildcard $1/*.cpp)) \
 $(eval src_files = $(src_files:$(LOCAL_PATH)/%=%))$(src_files) \
 $(eval item_all = $(wildcard $1/*)) \
 $(foreach item, $(item_all) $(),\
  $(eval item := $(item:%.cpp=%)) \
  $(call all_cpp_files_recursively, $(item))\
 )

LOCAL_SRC_FILES  := $(call all_cpp_files_recursively, $(LOCAL_PATH))

ifeq ($(MINIGLOG_LIB_TYPE),SHARED)
    LOCAL_SHARED_LIBRARIES += miniglog
else
    LOCAL_STATIC_LIBRARIES += miniglog
endif



include $(BUILD_STATIC_LIBRARY)
