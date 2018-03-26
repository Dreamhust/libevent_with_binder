############################################################# 
# Makefile for shared library.
# Author: heji
# Date:2016-06-29
# Description: makefile for Native Service Client
#############################################################
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include ../Build/AndroidCommon.mk
#######################################
MODULE_NAME := ThreadPool
#######################################
LOCAL_SHARED_LIBRARIES += liblog

SRC_DIR := ./src
SRC = $(addprefix $(SRC_DIR)/,$(notdir $(wildcard $(LOCAL_PATH)/$(SRC_DIR)/*.cpp)))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_SRC_FILES := $(SRC)

LOCAL_MODULE := lib$(MODULE_NAME)

LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)