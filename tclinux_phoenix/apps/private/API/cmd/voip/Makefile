##################################
#####Configured directories#######
##################################
ADAM_DIR := $(APP_VOIP_EVA_DIR)/adam
ADAM_INC := -I$(ADAM_DIR)
ADAM_LIB := -L$(APP_VOIP_EVA_DIR)/bin -ladam 

INCLUDE			= -I$(KERNEL_DIR)/include \
				      -I$(APP_VOIP_EVA_DIR)/adam \
				      -I$(APP_VOIP_EVA_DIR)/common \
				      -I$(KERNEL_DIR)/arch/mips/include \
				      -I$(APP_API_LIB_OUTPUT_DIR)
				      				      
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) $(ADAM_LIB)  -lpthread  -lm  -lapi_lib_utility -lapi_lib_voipmgr  -lecnt_lib_dbg -lrt 
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin
##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
ifeq ($(strip $(TCSUPPORT_GLIBC_VER)),220)
	VOIPMGR_CFLAGS	= -O2 -Wall -fPIC -shared  $(INCLUDE) $(TC_CFLAGS)
else
	VOIPMGR_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS)
endif
#VOIPMGR_CFLAGS	+= -D API_CMD_VOIPMGR_DBG

##################################
########Configured output#########
##################################
OBJS	=	voipmgr.o voipmgr_helper.o 
TARGETS	=	voipmgr

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(VOIPMGR_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(VOIPMGR_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

