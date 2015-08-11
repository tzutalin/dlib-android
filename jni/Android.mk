LOCAL_PATH := $(call my-dir)
SUB_MK_FILES := $(call all-subdir-makefiles)

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


include $(SUB_MK_FILES)
