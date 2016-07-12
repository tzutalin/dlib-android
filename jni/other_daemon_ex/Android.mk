LOCAL_PATH := $(call my-dir)

# C++ TestSelectiveSearch
# =======================================================
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(OPENCV_PATH)/OpenCV.mk

LOCAL_MODULE := TestSelectiveSearch

LOCAL_C_INCLUDES += $(OPENCV_INCLUDE_DIR)

# import dlib
LOCAL_STATIC_LIBRARIES += dlib

LOCAL_SRC_FILES := TestSelectiveSearch.cpp

LOCAL_LDLIBS := -lm -llog -ldl -lz
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
    LOCAL_ARM_NEON := true
endif

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)
#-----------------------------------------------------------------------------

# C++ face_landmark
# =======================================================
include $(CLEAR_VARS)
OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := off
OPENCV_LIB_TYPE := STATIC
include $(OPENCV_PATH)/OpenCV.mk

LOCAL_MODULE := face_landmark

# import dlib
LOCAL_STATIC_LIBRARIES += dlib

LOCAL_C_INCLUDES += $(OPENCV_INCLUDE_DIR)

LOCAL_SRC_FILES := face_landmark_detection_ex.cpp

LOCAL_LDLIBS := -lm -llog -ldl -lz
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

LOCAL_ARM_MODE := arm
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
LOCAL_ARM_NEON := true
endif

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)
#-----------------------------------------------------------------------------

