LOCAL_PATH:= $(call my-dir)
#***********************************************#
#** libJNI_OceanusTv_OClt_TemplateManager********#
#***********************************************#
include $(CLEAR_VARS)
include ../Build/AndroidCommon.mk
LOCAL_JNI_MODULE_NAME := Template
LOCAL_SRC_FILES:= JNI_OceanusTv_OClt_$(LOCAL_JNI_MODULE_NAME)Manager.cpp
LOCAL_SHARED_LIBRARIES += $(PREBUILT_JNI_BASIC_LIBS)
LOCAL_SHARED_LIBRARIES += libOClt_$(LOCAL_JNI_MODULE_NAME)Manager
LOCAL_REQUIRED_MODULES := libOClt_$(LOCAL_JNI_MODULE_NAME)Manager
LOCAL_STATIC_LIBRARIES :=
LOCAL_C_INCLUDES += frameworks/base/core/jni \
					$(JNI_H_INCLUDE)
LOCAL_CFLAGS +=
LOCAL_MODULE:= libJNI_OceanusTv_OClt_$(LOCAL_JNI_MODULE_NAME)Manager
include $(BUILD_SHARED_LIBRARY)
