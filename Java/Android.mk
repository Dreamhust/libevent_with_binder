LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/JNI_LIBS.mk
include ../Build/AndroidCommon.mk
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE :=false
LOCAL_SRC_FILES := $(call all-java-files-under,com)
LOCAL_REQUIRED_MODULES += EventManager.xml
$(warning $(LOCAL_REQUIRED_MODULES))
LOCAL_MODULE := EventManager
LOCAL_JAVACFLAGS := -Xlint:all
LOCAL_CERTIFICATE := platform
include $(BUILD_JAVA_LIBRARY)
################################################################
include $(CLEAR_VARS)
LOCAL_MODULE := EventManager.xml
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE :=false
LOCAL_MODULE_CLASS :=ETC
#this will install the file in /system/etc/premission
LOCAL_MODULE_PATH :=$(TARGET_OUT_ETC)/permissions
LOCAL_SRC_FILES :=$(LOCAL_MODULE)
include $(BUILD_PREBUILT)