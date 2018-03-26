############################################################# 
# Makefile for shared library.
# Author: heji
# Date:2016-2-26
# Description: makefile for android common var
#############################################################
#set environment option
LOCAL_PROPRIETARY_MODULE := true
$(warning $(LOCAL_OCEANUS_DEVICE))
PREBUILT_BASIC_INCLUDES = include
PREBUILT_JNI_BASIC_LIBS = libandroid_runtime \
						  libnativehelper \
						  libutils \
						  libbinder \
						  liblog \
						  libcutils
LOCAL_OCEANUS_NATIVESERVICE_LD := libThreadPool \
								  libEvent \
								  libOClt_EventManager
LOCAL_LDLIBS += -lm -llog 
SHARED_LIB += lib
LOCAL_LDFLAGS += $(addprefix -L,$(SHARED_LIB)) $(addprefix -L,$(STATIC_LIB)) -fPIC -shared
LOCAL_SHARED_LIBRARIES += libdl
LOCAL_CFLAGS += $(CC_OPTS) -std=c++11 -O2 -D_STLP_USE_NEWALLOC -Wno-unused-parameter -Wno-sign-compare -Wno-type-limits