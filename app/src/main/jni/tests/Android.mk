CURRENT_ABS_FOLDER_PATH = /$(patsubst /%,%,$(subst $($(abspath $(USER_LOCAL_PATH))),,$(abspath $1)))
USER_LOCAL_PATH:=$(LOCAL_PATH)
THIS_LOCAL_ABS_PATH := $(call CURRENT_ABS_FOLDER_PATH, $(call my-dir))
LOCAL_PATH:=$(subst ?,,$(firstword ?$(subst \, ,$(subst /, , $(abspath $(call my-dir))))))
DLIB_PATH:=$(THIS_LOCAL_ABS_PATH)/../../dlib

ifeq ($(Test),true)
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

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
endif

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)
#-----------------------------------------------------------------------------

### Test daemon
#==========================svm_ex===================================
include $(CLEAR_VARS)
LOCAL_MODULE := svm_ex
LOCAL_SRC_FILES := $(DLIB_PATH)/examples/svm_ex.cpp
LOCAL_STATIC_LIBRARIES := dlib
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#==========================svm_ex===================================
$(info $(Test))
#====================max_cost_assignment_ex=========================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(DLIB_PATH)/examples/max_cost_assignment_ex.cpp
LOCAL_MODULE := max_cost_assignment_ex
LOCAL_STATIC_LIBRARIES := dlib
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#====================max_cost_assignment_ex=========================

#============================svr_ex================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(DLIB_PATH)/examples/svr_ex.cpp
LOCAL_MODULE := svr_ex
LOCAL_STATIC_LIBRARIES := dlib
LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#============================svr_ex================================
else
$(info "Won't build test's executable file")
endif
