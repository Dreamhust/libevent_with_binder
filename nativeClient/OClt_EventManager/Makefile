############################################################# 
# Makefile for shared library.
# Copyright:sky
# Author: heji@skyworth.com
# Date:2015-12-05
# Description: makefile for interface
#############################################################
include $(OCEANUS_ROOT)/Build/Binder.mk
#set MOUDLE_NAME
MOUDLUE_NAME := Event
#set your src inc and lib

SRC_DIR = ./src
SRC = $(notdir $(wildcard $(SRC_DIR)/*.cpp))

LIB_DIR = ./lib
OBJ_DIR = ./obj
OBJ = $(addprefix $(OBJ_DIR)/,$(patsubst %cpp, %o, $(SRC)))
OBJ_DEP = $(addprefix $(OBJ_DIR)/,$(patsubst %cpp, %d, $(SRC)))


INC += ../include

_TARGE_NAME = libOClt_$(MOUDLUE_NAME)Manager
_TARGE_DIR = $(LIB_DIR)
_TARGE :=  $(_TARGE_NAME).$(TARGET_SUFFIXEX)

.PHONY:all clean release

#all target
all: $(_TARGE)

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@set -e; rm -f $@; \
	$(GXX) -MM $(CC_FLAG) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\$(OBJ_DIR)/$*.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(GXX) $(CC_FLAG) -c  $< -o $@
-include $(OBJ_DEP)

$(_TARGE):$(OBJ)
	@mkdir -p $(_TARGE_DIR)
ifeq ($(TARGET_LINK_TYPE),shared)
		$(GXX) $(CC_FLAG) $^ -o $(_TARGE_DIR)/$@ -shared $(LD_FLAG)
else
		$(AR) -r $(_TARGE_DIR)/$@ $^
endif
release:
	@echo "Release :" $(_TARGE)
	$(call cpdir,$(_TARGE_DIR),$(OCEANUS_OCEANUS_LIB))
clean:
	@rm -fr $(OBJ_DIR) $(_TARGE_DIR)

