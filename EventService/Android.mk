############################################################# 
# Makefile for shared library.
# Author: heji
# Date:2016-06-29
# Description: makefile for Native Service Event
#############################################################
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include ../Build/AndroidCommon.mk
#######################################
MODULE_NAME := Event
#######################################
LOCAL_REQUIRED_MODULES := ThreadPool 
LOCAL_REQUIRED_MODULES += libOClt_EventManager
LOCAL_SHARED_LIBRARIES += \
    libcutils \
    libutils \
    libbinder 
	
LOCAL_STATIC_LIBRARIES += libevent_static
#LOCAL_STATIC_LIBRARIES += libevent_pthreads_static
#LOCAL_STATIC_LIBRARIES += libevent_core_static
#LOCAL_STATIC_LIBRARIES += libevent_extra_static
LOCAL_SHARED_LIBRARIES += libThreadPool
LOCAL_SHARED_LIBRARIES += libOClt_EventManager

SRC_DIR := ./src
SRC = $(addprefix $(SRC_DIR)/,$(notdir $(wildcard $(LOCAL_PATH)/$(SRC_DIR)/*.cpp)))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_SRC_FILES := $(SRC)
LOCAL_MODULE := lib$(MODULE_NAME)

LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)