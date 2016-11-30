##################################
#####Configured directories#######
##################################
INCLUDE			= -I$(KERNEL_DIR)/include \
				  -I$(KERNEL_DIR)/arch/mips/include \
				  -I$(APP_API_LIB_OUTPUT_DIR) \
				  -I$(APP_API_LIB_GPON_DIR)
				  
LIBDIR			= -L $(APP_API_LIB_OUTPUT_DIR) -lapi_lib_utility -lecnt_lib_dbg
USRBINDIR		= $(FILESYSTEM_DIR)/userfs/bin

##################################
########Configured flags##########
##################################
RM				= /bin/rm -f
DBGMGR_CFLAGS	= -O2 -Wall $(INCLUDE) $(TC_CFLAGS)
#DBGMGR_CFLAGS	+= -D API_CMD_VDSLMGR_DBG
#DBGMGR_CFLAGS	+= -D API_CMD_QDMAMGR_DBG
#DBGMGR_CFLAGS	+= -D API_CMD_EMI_DBG
DBGMGR_CFLAGS	+= -D API_CMD_GPONMGR_DBG=0
##################################
########Configured output#########
##################################
OBJS	=   dbgcmd.o

##################################
##########interface config########
##################################
ifneq ($(strip $(TCSUPPORT_WAN_GPON)),)
INCLUDE += -I$(GLOBAL_INC_DIR)/modules
OBJS	+= gpon.o gpon_get.o gpon_set.o
LIBDIR  += -lapi_lib_gponmgr
endif

ifneq ($(strip $(TCSUPPORT_WAN_EPON)),)
INCLUDE += -I$(MODULES_XPON_WAN_DIR)/inc
OBJS	+= epon.o
LIBDIR  += -lapi_lib_eponmgr
endif

ifneq ($(strip $(TCSUPPORT_WAN_PTM)),)
INCLUDE  += -I$(MODULES_VDSL_WAN_DIR)
OBJS	+= ptm.o
LIBDIR   += -lapi_lib_vdslmgr

ifneq ($(strip $(TCSUPPORT_BONDING)),)
OBJS	+= bonding.o
endif

endif

ifneq ($(strip $(TCSUPPORT_WAN_ATM)),)
OBJS	+= atm.o
LIBDIR   += -lapi_lib_atmmgr
endif

ifneq ($(strip $(TCSUPPORT_QDMA_BUFMGR)),)
OBJS	+= qdma.o qdma_get.o qdma_set.o
LIBDIR   += -lapi_lib_qdmamgr_wan
LIBDIR   += -lapi_lib_qdmamgr_lan
endif

ifneq ($(strip $(TCSUPPORT_UART2)),)
OBJS	+= uart2.o
endif
OBJS	+= system.o
TARGETS	=	dbgmgr

all: $(TARGETS) install

.SUFFIXES:  .c .o
.c.o:
	$(CC) $(DBGMGR_CFLAGS) -c $<
	
$(TARGETS): clean $(OBJS)
	$(CC) $(DBGMGR_CFLAGS) -o $(TARGETS) $(OBJS) $(LIBDIR)
	$(STRIP) --strip-unneeded $(TARGETS)

install:
	install $(TARGETS) $(USRBINDIR)
	
clean:
	$(RM) $(OBJS) $(TARGETS)
	$(RM) $(USRBINDIR)/$(TARGETS)

