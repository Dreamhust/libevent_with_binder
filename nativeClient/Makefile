############################################################# 
# Makefile for shared library.
# Copyright:sky
# Author: heji@skyworth.com
# Date:2016-06-28
# Description: makefile for NativeService Client
#############################################################
include $(OCEANUS_ROOT)/Build/autoconf.mk
CLIENT := $(shell ls -F | grep -v '*\|include')
.PHONY:all clean release_lib release_src
all:
	@for n in $(CLIENT); do $(MAKE) -C $$n ||exit "$$?"; done
release_lib:
	@for n in $(CLIENT); do $(MAKE) -C $$n release ||exit "$$?"; done
release_src:
ifeq ($(TARGET_OS),Android)
	@for n in $(CLIENT); do \
	echo "$$n"; \
	mkdir -p $(OCEANUS_ROOT)/Target/out/O4$(_TARGET_OS)/NativeClient/$$n; \
	cp -r $$n/src $(OCEANUS_ROOT)/Target/out/O4$(_TARGET_OS)/NativeClient/$$n; \
	cp -r $$n/Android.mk $(OCEANUS_ROOT)/Target/out/O4$(_TARGET_OS)/NativeClient/$$n; \
	done
	@cp -r ./Android.mk $(OCEANUS_ROOT)/Target/out/O4$(_TARGET_OS)/NativeClient
endif
clean:
	@for n in $(CLIENT); do $(MAKE) -C $$n clean ||exit "$$?"; done

