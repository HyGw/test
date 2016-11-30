#CROSS_PATH=mips-linux-gnu-
#CROSS=$(CROSS_PATH)
#CC=$(CROSS)gcc -mips32r2 -msoft-float -muclibc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
#LIBDIR=$(CROSS_PATH)/lib

#KERNEL_DIR=../../linux

ifneq ($(strip $(TCSUPPORT_CT_PON)),)
ifneq ($(strip $(TCSUPPORT_MT7530_SWITCH_API)),)
LIBMTKSWITCH = libmtkswitch
LIBSO = ../lib/$(LIBMTKSWITCH).so.1.0
CFLAGS_LINK_SO = -L. -lmtkswitch
else
LIBSO =
CFLAGS_LINK_SO =
endif

INCLUDE = -I$(KERNEL_HEADER) -I$(APP_PONMGR_DIR)/inc -I$(APP_XPON_DIR)/inc/pmmgr -I$(GLOBAL_INC_DIR)/modules
else
INCLUDE = -I$(KERNEL_DIR)/include
endif
#TCAPILIBDIR = /usr/local/lib/
#TCAPILIB=-L $(TCAPILIBDIR) -ltcapi
TCAPILIB=-ltcapi

CFLAGS= -O2 -Wall $(INCLUDE)
ifeq ($(WITHVOIP),1)
CFLAGS += -DWITHVOIP
endif
ifeq ($(PURE_BRIDGE),1)
  	CFLAGS+= -DPURE_BRIDGE
endif
#CFLAGS+= -DCWMP
#CFLAGS+= -DSSL
#CFLAGS+= -DSTATIC_DHCP 

ifeq ($(TR111),1)
  	CFLAGS+= -DTR111
endif

ifeq ($(ALIAS_IP),1)
  	CFLAGS+= -DALIAS_IP
endif
ifeq ($(IPV6),1)
  	CFLAGS+= -DIPV6
endif
ifeq ($(RT3390),1)
  	CFLAGS+= -DRT3390
endif
ifeq ($(RT3090),1)
  	CFLAGS+= -DRT3090
endif
ifeq ($(WSC_AP_SUPPORT),1)
  	CFLAGS+= -DWSC_AP_SUPPORT
endif
ifeq ($(TR068_LED),1)
  	CFLAGS+= -DTR068_LED
endif
ifeq ($(IGMP_SNOOP_SUPPORT),1)
	CFLAGS+= -DIGMP_SNOOP_SUPPORT
endif
ifeq ($(DHCP_PROFILE),1)
  	CFLAGS+= -DDHCP_PROFILE
endif
ifneq ($(strip $(TCSUPPORT_CT_PON)),)
ifneq ($(strip $(TCSUPPORT_OMCI)),)
	CFLAGS+= -DTCSUPPORT_OMCI
endif
endif
ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(OPENSOURCE_CFLAGS), )
		CFLAGS+=$(OPENSOURCE_CFLAGS)
endif

ifneq ($(COMPILEOPTION_CFLAGS), )
		CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

ifeq ($(TCSUPPORT_DNSEACHPVC),1)
  	CFLAGS+= -DTCSUPPORT_DNSEACHPVC
endif

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_E8DDNS)
ifeq ($(TCSUPPORT_CT_E8DDNS),1)
    CFLAGS+= -DTCSUPPORT_CT_E8DDNS
endif
#endif/*TCSUPPORT_COMPILE*/

#frank add megaco support 2013/9/3 19:49:28
ifneq ($(strip $(VP_MEGACO)),)
CFLAGS += -DVP_MEGACO
endif

LIBS = -lcrypt -L ../lib/ -lmxml -lpthread

ifneq ($(COMPILEOPTION_LDFLAGS), )
		LIBS+=$(COMPILEOPTION_LDFLAGS)
endif

ifneq ($(strip $(TCSUPPORT_CT_PON)),)
ifneq ($(strip $(TCSUPPORT_WAN_EPON)),)
EPON_LIB = -L ../lib -lepon
LIBS += $(EPON_LIB)
endif
endif
	
ifneq ($(strip $(TCSUPPORT_CT_SIMCARD_SEPARATION)),)
INCLUDE += -I$(APP_CMDCI_DIR)/
LIBS  +=  -L ../lib/ -lecb3 -lsimcard  -lSimCardUtil
endif
	
ifneq ($(OPENSOURCE_LDFLAGS),)
LIBS +=$(OPENSOURCE_LDFLAGS)
endif

#CFLAGS+=-DDEBUG

LIBS += $(TCAPILIB)
OBJS = cfg_manager.o interface.o atmcmdc.o wan_monitor.o wan.o advanced.o access.o maintenance.o web.o cc.o tcapi_proc.o utility.o init.o conntrack_delete.o
RELEASEOBJS = interface.o wan_monitor.o wan.o advanced.o access.o maintenance.o web.o cc.o tcapi_proc.o utility.o init.o voip.o conntrack_delete.o
ifneq ($(strip $(TCSUPPORT_CT_PON)),)
OBJS += xpon.o
RELEASEOBJS += xpon.o
ifneq ($(TCSUPPORT_PONMGR),)
OBJS += $(APP_PONMGR_DIR)/bin/libponapi.a
RELEASEOBJS += $(APP_PONMGR_DIR)/bin/libponapi.a
endif
ifneq ($(TCSUPPORT_PMMGR),)
OBJS += $(APP_PMMGR_DIR)/libpmapi.a
RELEASEOBJS += $(APP_PMMGR_DIR)/libpmapi.a
endif
endif
ifneq ($(TCSUPPORT_VOIP),)
#Add by mtk06404 support ecn sip or h248
ifneq ($(strip $(TCSUPPORT_ECN_SIP))_$(strip $(TCSUPPORT_ECN_MEGACO)),_)
OBJS += voip_mtk.o
RELEASEOBJS += voip_mtk.o
else

OBJS += voip.o
RELEASEOBJS += voip.o
ifneq ($(strip $(VP_MEGACO)),)
OBJS += pmegaco_support.o sp_common.a
CFLAGS +=  -I../voip/dare_megaco/darevoip/code/support
endif

ifneq ($(strip $(TCSUPPORT_VOIP_IMSSIP)),)
LIBS += vpcom.a
OBJS += voip_data_util.o
CFLAGS +=  -I$(APP_VOIP_IMSSIP_DIR)/vpcfg/include -I$(APP_VOIP_IMSSIP_DIR)/vpcfg/vpcom/include
endif

endif

endif

ifneq ($(TCSUPPORT_CT_SIMCARD_SEPARATION),)
OBJS += sim.o
endif

ifneq ($(TCSUPPORT_CT_JOYME_BANDWIDTH),)
OBJS += bandwidth.o
endif

#NEWOBJS
TARGET= cfg_manager
%.o:%.c
	$(CC) $(CFLAGS) -c $<
ifneq ($(strip $(TCSUPPORT_CT_PON)),)
all: mxml_lib tcapi_lib pmapi_lib $(TARGET) $(LIBSO) 
else
all: mxml_lib tcapi_lib $(TARGET)
endif

#new: init.o $(NEWOBJS)
#	$(CC) -Wall -o $(TARGET) $(OBJS) $(NEWOBJS) $(LIBS) 
#	$(STRIP) $(TARGET) 

mxml_lib:
	$(MAKE) -C $(APP_MXML_DIR) clean
	$(MAKE) -C $(APP_MXML_DIR)
	$(MAKE) -C $(APP_MXML_DIR) install

tcapi_lib:
	$(MAKE) -C $(APP_TCAPILIB_DIR) clean
	$(MAKE) -C $(APP_TCAPILIB_DIR)
	$(MAKE) -C $(APP_TCAPILIB_DIR)  install
ifneq ($(strip $(TCSUPPORT_CT_PON)),)
ifneq ($(TCSUPPORT_PMMGR), )
pmapi_lib:
ifneq ($(strip $(TCSUPPORT_MT7530_SWITCH_API)),)
	$(MAKE) -C $(APP_ETHCMD_DIR) clean
	$(MAKE) -C $(APP_ETHCMD_DIR)
	$(MAKE) -C $(APP_ETHCMD_DIR) install
endif
	$(MAKE) -C $(APP_PMMGR_DIR) clean
	$(MAKE) -C $(APP_PMMGR_DIR)
else
pmapi_lib:
endif
endif
cfg_manager: $(OBJS)
ifneq ($(strip $(TCSUPPORT_CT_PON)),)
	$(CC) -Wall -o $(TARGET) $(OBJS) $(LIBS) $(CFLAGS_LINK_SO)
else
	$(CC) -Wall -o $(TARGET) $(OBJS) $(LIBS) 
endif
	$(STRIP) $(TARGET)

ifneq ($(strip $(TCSUPPORT_CT_PON)),)
$(APP_PONMGR_DIR)/bin/libponapi.a:
	$(MAKE) -C $(APP_PONMGR_DIR) clean
	$(MAKE) -C $(APP_PONMGR_DIR)

endif

clean:
#	rm -f init.o $(TARGET)
	rm -f $(RELEASEOBJS) $(TARGET)
	rm -f *.o
