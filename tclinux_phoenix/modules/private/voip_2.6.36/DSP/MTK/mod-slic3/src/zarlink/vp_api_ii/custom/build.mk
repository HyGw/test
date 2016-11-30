#!/bin/make
#
#  THIS IS AN UNPUBLISHED WORK CONTAINING D2 TECHNOLOGIES, INC. CONFIDENTIAL
#  AND PROPRIETARY INFORMATION.  IF PUBLICATION OCCURS, THE FOLLOWING NOTICE
#  APPLIES:
#  "COPYRIGHT 2004 D2 TECHNOLOGIES, INC. ALL RIGHTS RESERVED"
#
# $D2Tech$ $Rev: 657 $ $Date: 2011/01/06 15:08:27 $
#



# C Source files

ifeq ($(PLATFORM),m34k3)
CSRC	= \
		vp_hal_trendchip1.c \
		sys_service.c
else

CSRC	= \
		vp_hal.c \
		sys_service.c
endif

# Assembly files
SSRC	= \

# Private Header files
PRIVATE_HEADERS	= 

# Files to export to INCLUDE_DIR
PUBLIC_HEADERS	= \
	hbi_hal.h \
	mpi_hal.h \
	vp_hal.h \
	sys_service.h \
	vp_api_types.h \
	vp_api_profile_type.h

# Files to export to OBJ_DIR
OUTPUT			= archive

include $(TOOL_DIR)/rules.mk

# Build Rule - add custom build commands below this rule
build: default_build

# Clean Rule - add custom clean commands below this rule
clean: default_clean


# END OF MAKEFILE

