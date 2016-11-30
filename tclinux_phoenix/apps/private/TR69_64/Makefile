#CFLAGS= -msoft-float -mips1 -O2 -fomit-frame-pointer
LDLIBS=-lm -lcrypt -lpthread

ifneq ($(COMPILEOPTION_LDFLAGS), )
		LDLIBS+=$(COMPILEOPTION_LDFLAGS)
endif
INCLUDE = -I$(KERNEL_HEADER)
SRC=.

ifneq ($(TCSUPPORT_CWMP_FAST_GET),)
TCAPILIB=-L ../lib_cwmp -ltcapi_cwmp
TCAPILIB+=-L ../lib/ -lmxml
else
TCAPILIB=-L ../lib -ltcapi
TCAPILIB+=-L ../lib/ -lmxml
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_UPNP_DM)
ifneq ($(TCSUPPORT_CT_UPNP_DM),)
TCAPILIB+=-L ./libsockcmd -lsockcmd
endif
#endif/*TCSUPPORT_COMPILE*/
endif

ifneq ($(TCSUPPORT_CWMP_SSL),)
TCAPILIB+=-L ../lib -lmatrixssl
endif

ifneq ($(TCSUPPORT_CT_DHCPC_OPTION60), )
TCAPILIB += -L ../../../public/lib -lecb3
endif


ifneq ($(TCSUPPORT_CT_SIMCARD_SEPARATION),)
TCAPILIB+=	-L ../lib -lSimCardUtil -lsimcard 
endif


ifneq ($(OPENSOURCE_CFLAGS), )
CFLAGS+=$(OPENSOURCE_CFLAGS)
endif

ifneq ($(OPENSOURCE_LDFLAGS),)
TCAPILIB +=$(OPENSOURCE_LDFLAGS)
endif


#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_E8DDNS)
ifeq ($(TCSUPPORT_CT_E8DDNS),1)
 CFLAGS+= -DTCSUPPORT_CT_E8DDNS
endif
#endif/*TCSUPPORT_COMPILE*/


EXEC=tr69

ifneq ($(TCSUPPORT_CT),)
VPATH=$(SRC)/rompager/Sources:$(SRC)/cwmp/Sources:$(SRC)/cwmp/cwmpParameter_ct/Sources:$(SRC)/clmp:$(SRC)/linos:$(SRC)/main
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_MIDWARE)
ifeq ($(TCSUPPORT_CT_MIDWARE),1)
VPATH +=:$(SRC)/cwmp/midware
endif
#endif/*TCSUPPORT_COMPILE*/
else
VPATH=$(SRC)/rompager/Sources:$(SRC)/cwmp/Sources:$(SRC)/cwmp/cwmpParameter/Sources:$(SRC)/clmp:$(SRC)/linos:$(SRC)/main
endif

OBJS = AsBase64.o AsCustom.o ASMD5.o ASPARSE.o ASCOMMON.o ASDATE.o ASMAIN.o AsSnmp.o \
	AsConvrt.o ASGLOBAL.o ASMIMES.o AsVarAcc.o \
	RpSoap.o RuData.o RuSoapOb.o \
	RxData.o RxFrame.o RxParse.o RxTrans.o \
	IPPSHELL.o RPACCESS.o RPDATA.o RPHTML.o RPIPP.o RPMULTI.o \
    RPQUERY.o  RPCALLBACK.o RPFORM.o RPHTTPDY.o \
	RPPUT.o \
	RPUSER.o \
	rpusrdct.o \
	RPLOG.o RPURL.o SPWD.o \
	IPPSHELL.o RPFILE.o RPFRMITM.o RPHTTP.o RPHTTPPS.o RPHTTPRQ.o \
	WcClient.o WcData.o WcHttp.o WcHttpPs.o \
	main.o open_code.o messageHandle.o tctimer.o mbuf.o cwmpXmlmembuf.o  cwmpXmlparser.o \
	cwmpRpc.o  cwmpNode.o  cwmpAttr.o \
	node.o  nodeList.o  namedNodeMap.o  cwmpSoap.o cwmpDocument.o  cwmpElement.o  cwmpXml.o \
	cwmpParameter_table.o cwmpParamApi.o \
	Sfil.o Stcp.o Sudp.o RpPage1.o utily.o Httpd.o \
	cp_core.o cp_dynamicnodetable.o cp_deviceinfo_nodetable_d.o\
	cp_deviceinfo_func_d.o cp_landev_func.o cp_landev_nodetable.o\
	cp_managerment_func.o cp_managerment_nodetable.o cp_topnodetable.o\
	cp_wandev_func.o cp_wandev_nodetable.o\
	cp_wandev_wanconndev_dslLinkCfg_func.o cp_wandev_wanconndev_dslLinkCfg_nodetable.o\
	cp_wandev_wanconndev_func.o cp_wandev_wanconndev_ip_ppp_conn_func.o\
	cp_wandev_wanconndev_ip_ppp_conn_nodetable.o cp_wandev_wanconndev_nodetable.o\
	cp_wandev_wanconndev_atmF5LBDiag_nodetable.o cp_wandev_wanconndev_atmF5LBDiag_func.o\
	cp_IPPingDiagnostics_func.o cp_IPPingDiagnostics_nodetable.o\
	cp_wandev_wandsldiag_nodetable.o cp_wandev_wandsldiag_func.o\
	cp_DeviceInfo_nodetable.o cp_DeviceInfo_func.o\
	cp_time_nodetable.o cp_time_func.o\
	cp_wandev_wandslInterCfg_nodetable.o cp_wandev_wandslInterCfg_func.o\
	cp_wandev_wandslConnMgmt_nodetable.o cp_wandev_wandslConnMgmt_func.o\
	cp_devConfig_nodetable.o cp_devConfig_func.o\
	cp_layer3forwarding_func.o cp_layer3forwarding_nodetable.o\
	cp_wandev_wandslInterCfg_stats_nodetable.o cp_wandev_wandslInterCfg_stats_func.o\
	cp_lanconfigsecurity_func.o cp_lanconfigsecurity_nodetable.o\
	cp_landev_ipInterface_func.o cp_landev_ipInterface_nodetable.o\
	cp_landev_ethInterCfg_func.o cp_landev_ethInterCfg_nodetable.o\
	cp_landev_hosts_func.o cp_landev_hosts_nodetable.o\
   	cp_userInterface_nodetable.o cp_userInterface_func.o\
	cp_Services_nodetable.o cp_Services_func.o
        #	#RpCgiTs1.o \

#	JavaTest.o JvClass1.o JvClass2.o WcTest.o \
#	RpChassi.o RpImages.o RpNumber.o RpValid.o RpVars.o RpVarTst.o \
# RpFreDct.o RpGerDct.o RpItaDct.o RpPorDct.o RpSpaDct.o
# RpCookie.o RpMarket.o RpPage1.o RpPages.o
#	RxObjct1.o RxTest1.o \
#	RpSvgTst.o \ 	RpTask.o 
#	Igd.o IgdVars.o IgdPg.o IgdFuncs.o IgdUpnp.o RuIcons.o \
#   cwmpParameter.o cwmpParamApi.o \

RELEASEOBJS = open_code.o UPNP_VAR.o tctimer.o cwmpParameter_table.o cwmp_customer_operation.o \
			 cp_core.o cp_dynamicnodetable.o cp_deviceinfo_nodetable_d.o\
			 cp_deviceinfo_func_d.o cp_landev_func.o cp_landev_nodetable.o\
			 cp_managerment_func.o cp_managerment_nodetable.o cp_topnodetable.o\
			 cp_wandev_func.o cp_wandev_nodetable.o\
			 cp_wandev_wanconndev_dslLinkCfg_func.o cp_wandev_wanconndev_dslLinkCfg_nodetable.o\
		     cp_wandev_wanconndev_func.o cp_wandev_wanconndev_ip_ppp_conn_func.o\
			 cp_wandev_wanconndev_ip_ppp_conn_nodetable.o cp_wandev_wanconndev_nodetable.o\
			 cp_wandev_wanconndev_atmF5LBDiag_nodetable.o cp_wandev_wanconndev_atmF5LBDiag_func.o\
			 cp_IPPingDiagnostics_func.o cp_IPPingDiagnostics_nodetable.o\
			 cp_wandev_wandsldiag_nodetable.o cp_wandev_wandsldiag_func.o\
			 cp_DeviceInfo_nodetable.o cp_DeviceInfo_func.o\
			 cp_time_nodetable.o cp_time_func.o\
			 cp_wandev_wandslInterCfg_nodetable.o cp_wandev_wandslInterCfg_func.o\
			 cp_wandev_wandslConnMgmt_nodetable.o cp_wandev_wandslConnMgmt_func.o\
			 cp_devConfig_nodetable.o cp_devConfig_func.o\
			 cp_layer3forwarding_func.o cp_layer3forwarding_nodetable.o\
			 cp_wandev_wandslInterCfg_stats_nodetable.o cp_wandev_wandslInterCfg_stats_func.o\
			 cp_lanconfigsecurity_func.o cp_lanconfigsecurity_nodetable.o\
			 cp_landev_ipInterface_func.o cp_landev_ipInterface_nodetable.o\
			 cp_landev_ethInterCfg_func.o cp_landev_ethInterCfg_nodetable.o\
			 cp_landev_hosts_func.o cp_landev_hosts_nodetable.o\
   			 cp_userInterface_nodetable.o cp_userInterface_func.o\
			 cp_Services_nodetable.o cp_Services_func.o cwmpRpc.o

ifneq ($(TCSUPPORT_CWMP_VDSL),)
OBJS+=cp_wandev_wanconndev_ptmLinkCfg_func.o cp_wandev_wanconndev_ptmLinkCfg_nodetable.o
RELEASEOBJS+=cp_wandev_wanconndev_ptmLinkCfg_func.o cp_wandev_wanconndev_ptmLinkCfg_nodetable.o
endif

ifneq ($(TCSUPPORT_CT_DEVICEREG),)
OBJS+=cp_deviceregistration_nodetable.o cp_deviceregistration_func.o
RELEASEOBJS+=cp_deviceregistration_nodetable.o cp_deviceregistration_func.o
endif

ifeq ($(TCSUPPORT_CT),)
	OBJS+=RuUpnp.o RuUpnpAd.o IgdClmp.o IgdStubsClmp.o IgdPg.o IgdUpnp.o UPNP_VAR.o
else
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_MIDWARE)
ifeq ($(TCSUPPORT_CT_MIDWARE),1)
OBJS += midware.o cp_midware_nodetable.o alias.o cp_midware_func.o parse.o opMethodFunc.o  
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PHONEAPP)
ifneq ($(TCSUPPORT_CT_PHONEAPP),)
OBJS += cp_X_CTCOM_Mgt_nodetable.o cp_X_CTCOM_Mgt_func.o
endif
#endif/*TCSUPPORT_COMPILE*/
ifeq ($(CLMP),1)
	OBJS+=RuUpnp.o RuUpnpAd.o IgdClmp.o IgdStubsClmp.o IgdPg.o IgdUpnp.o UPNP_VAR.o
endif
	OBJS+=cp_wandev_wancommInterCfg_nodetable.o cp_wandev_wancommInterCfg_func.o
	RELEASEOBJS+=cp_wandev_wancommInterCfg_nodetable.o cp_wandev_wancommInterCfg_func.o	
endif

ifeq ($(TR143),1)
OBJS+=cp_tr143_nodetable.o cp_tr143_func.o
RELEASEOBJS+=cp_tr143_nodetable.o cp_tr143_func.o
endif

ifeq ($(WITHVOIP),1)
OBJS+= cp_Services_voiceProf_nodetable.o\
	cp_Services_voiceProf_func.o cp_Services_capab_nodetable.o cp_Services_capab_func.o\
	cp_Services_voiceProf_Line_nodetable.o cp_Services_voiceProf_Line_func.o
RELEASEOBJS+=cp_Services_voiceProf_nodetable.o\
	cp_Services_voiceProf_func.o cp_Services_capab_nodetable.o cp_Services_capab_func.o\
	cp_Services_voiceProf_Line_nodetable.o cp_Services_voiceProf_Line_func.o\

ifneq ($(TCSUPPORT_CT),)
 OBJS+= cp_Services_PhyInterface_nodetable.o cp_Services_PhyInterface_func.o
 RELEASEOBJS+=cp_Services_PhyInterface_nodetable.o cp_Services_PhyInterface_func.o
endif
endif

ifneq ($(DHCP_PROFILE),)
OBJS+=cp_landev_dhcpOption_nodetable.o cp_landev_dhcpOption_func.o
RELEASEOBJS+=cp_landev_dhcpOption_nodetable.o cp_landev_dhcpOption_func.o
endif

ifneq ($(TCSUPPORT_CT_QOS),)
OBJS+=cp_ctqos_nodetable.o cp_ctqos_func.o
RELEASEOBJS+=cp_ctqos_nodetable.o cp_ctqos_func.o
else
OBJS+=cp_qos_nodetable.o cp_qos_func.o
RELEASEOBJS+=cp_qos_nodetable.o cp_qos_func.o
endif

ifeq ($(TCSUPPORT_CT),)
OBJS+=cwmp_customer_operation.o unopen.o cwmp.o
RELEASEOBJS+=cwmp_customer_operation.o cwmp.o
else
OBJS+=unopen_ct.o cwmp_ct.o
RELEASEOBJS+=cwmp_ct.o
endif
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
ifneq ($(TCSUPPORT_CT_CWMP_E8BPARAMETER),)
OBJS+=cp_landev_dhcpv6_nodetable.o cp_landev_dhcpv6_func.o\
	cp_landev_routeradv_nodetable.o cp_landev_routeradv_func.o\
	cp_landev_dataspeedlimit_nodetable.o cp_landev_dataspeedlimit_func.o
RELEASEOBJS+=cp_landev_dhcpv6_nodetable.o cp_landev_dhcpv6_func.o\
	cp_landev_routeradv_nodetable.o cp_landev_routeradv_func.o\
	cp_landev_loopbackdetect_nodetable.o cp_landev_loopbackdetect_func.o\
	cp_landev_dataspeedlimit_nodetable.o cp_landev_dataspeedlimit_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_LOOPDETECT)
ifneq ($(TCSUPPORT_CT_LOOPDETECT),)
OBJS+=cp_landev_loopbackdetect_nodetable.o cp_landev_loopbackdetect_func.o
RELEASEOBJS+=cp_landev_loopbackdetect_nodetable.o cp_landev_loopbackdetect_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

ifneq ($(TCSUPPORT_CT_IPPINGDIAGNOSTIC),)
OBJS+=ct_IPPingDiagnostics_nodetable.o ct_IPPingDiagnostics_func.o
RELEASEOBJS+=ct_IPPingDiagnostics_nodetable.o ct_IPPingDiagnostics_func.o
endif
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_C1_CUC) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
ifneq ($(strip $(TCSUPPORT_C1_CUC) $(TCSUPPORT_CUC_CWMP_PARAMETER)),)
OBJS+=cp_X_CU_Function_nodetable.o cp_X_CU_Function_func.o
RELEASEOBJS+=cp_X_CU_Function_nodetable.o cp_X_CU_Function_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

ifneq ($(TCSUPPORT_WLAN),)
OBJS+=cp_landev_wlan_func.o cp_landev_wlan_nodetable.o
RELEASEOBJS+=cp_landev_wlan_func.o cp_landev_wlan_nodetable.o
endif
ifneq ($(TCSUPPORT_TRACEROUTE),)
OBJS+=cp_TraceRouteDiagnostics_func.o cp_TraceRouteDiagnostics_nodetable.o
RELEASEOBJS+=cp_TraceRouteDiagnostics_func.o cp_TraceRouteDiagnostics_nodetable.o
endif

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_LAYER2BRIDGING)
ifeq ($(TCSUPPORT_LAYER2BRIDGING),1)
OBJS +=  cp_bridge_nodetable.o cp_bridge_func.o
RELEASEOBJS += cp_bridge_nodetable.o cp_bridge_func.o
endif
#endif/*TCSUPPORT_COMPILE*/
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_TTNET)
ifeq ($(TCSUPPORT_TTNET),1)
OBJS += cp_X_TTNET_Service_nodetable.o cp_X_TTNET_Service_func.o
RELEASEOBJS+=cp_X_TTNET_Service_nodetable.o cp_X_TTNET_Service_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

ifneq ($(TCSUPPORT_CWMP_ACL),)
OBJS += cp_X_TC_Service_nodetable.o cp_X_TC_Service_func.o
RELEASEOBJS+=cp_X_TC_Service_nodetable.o cp_X_TC_Service_func.o
endif

#if defined(TCSUPPORT_FON)
ifeq ($(TCSUPPORT_FON),1)
OBJS += cp_FON_nodetable.o cp_FON_func.o
RELEASEOBJS+=cp_FON_nodetable.o cp_FON_func.o
endif
#endif

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_PON)
ifneq ($(TCSUPPORT_CT_PON),)
OBJS += cp_wandev_wanconndev_xponLinkCfg_nodetable.o cp_wandev_wanconndev_xponLinkCfg_func.o\
	cp_wandev_wanxponInterCfg_nodetable.o cp_wandev_wanxponInterCfg_func.o\
	cp_wandev_wanxponInterCfg_stats_nodetable.o cp_wandev_wanxponInterCfg_stats_func.o
RELEASEOBJS+=cp_wandev_wanconndev_xponLinkCfg_nodetable.o cp_wandev_wanconndev_xponLinkCfg_func.o\
	cp_wandev_wanxponInterCfg_nodetable.o cp_wandev_wanxponInterCfg_func.o\
	cp_wandev_wanxponInterCfg_stats_nodetable.o cp_wandev_wanxponInterCfg_stats_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_WAN_PTM)
ifneq ($(TCSUPPORT_CT_WAN_PTM),)
OBJS += cp_wandev_wanconndev_vdslLinkCfg_nodetable.o cp_wandev_wanconndev_vdslLinkCfg_func.o\
	cp_wandev_wanconndev_ptmLinkCfg_func.o cp_wandev_wanconndev_ptmLinkCfg_nodetable.o\
	cp_wandev_wandslInterCfg_testParams_func.o cp_wandev_wandslInterCfg_testParams_nodetable.o
RELEASEOBJS+=cp_wandev_wanconndev_vdslLinkCfg_nodetable.o cp_wandev_wanconndev_vdslLinkCfg_func.o\
	cp_wandev_wanconndev_ptmLinkCfg_func.o cp_wandev_wanconndev_ptmLinkCfg_nodetable.o\
	cp_wandev_wandslInterCfg_testParams_func.o cp_wandev_wandslInterCfg_testParams_nodetable.o
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_UPNP_DM)
ifneq ($(TCSUPPORT_CT_UPNP_DM),)
OBJS += cp_X_CTCOM_ProxyDevice_func.o cp_X_CTCOM_ProxyDevice_nodetable.o
RELEASEOBJS+=cp_X_CTCOM_ProxyDevice_func.o cp_X_CTCOM_ProxyDevice_nodetable.o
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CUC_CWMP_PARAMETER)
ifneq ($(TCSUPPORT_CUC_CWMP_PARAMETER),)
OBJS+=cp_Services_user_func.o cp_Services_user_nodetable.o cp_Services_ftp_func.o cp_Services_ftp_nodetable.o
RELEASEOBJS+=cp_Services_user_func.o cp_Services_user_nodetable.o cp_Services_ftp_func.o cp_Services_ftp_nodetable.o 
endif
#endif/*TCSUPPORT_COMPILE*/

ifeq ($(TCSUPPORT_CT),)
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_USBHOST)
ifneq ($(TCSUPPORT_USBHOST),)
OBJS += cp_landev_usbInterCfg_func.o cp_landev_usbInterCfg_nodetable.o
RELEASEOBJS+=cp_landev_usbInterCfg_func.o cp_landev_usbInterCfg_nodetable.o
endif
#endif/*TCSUPPORT_COMPILE*/
endif

ifneq ($(TCSUPPORT_CT_PPPOE_EMULATOR),)
OBJS += cp_pppoeEmulator_nodetable.o cp_pppoeEmulator_func.o
RELEASEOBJS += cp_pppoeEmulator_nodetable.o cp_pppoeEmulator_func.o
endif

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_IPOE_EMULATOR)
ifneq ($(TCSUPPORT_CT_IPOE_EMULATOR),)
OBJS+=cp_ipoeEmulator_nodetable.o cp_ipoeEmulator_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_JOYME)
ifneq ($(TCSUPPORT_CT_JOYME),)
OBJS+=cp_xctcom_dns_speed_limit_nodetable.o cp_xctcom_dns_speed_limit_func.o
endif
#endif/*TCSUPPORT_COMPILE*/

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CMCC)
ifneq ($(TCSUPPORT_CMCC),)
OBJS += cp_laninterfaces_func.o cp_laninterfaces_nodetable.o cp_security_func.o cp_security_nodetable.o
RELEASEOBJS += cp_laninterfaces_func.o cp_laninterfaces_nodetable.o cp_security_func.o cp_security_nodetable.o
endif
#endif/*TCSUPPORT_COMPILE*/

#INCLUDES = -I/home/vlinux/vLinux/apps/romupnp/Engine/Includes
INCLUDES = -I.\
	-I./rompager/Includes \
	-I./cwmp/Includes \
	-I./linos \
	-I./main \

#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_UPNP_DM)
ifneq ($(TCSUPPORT_CT_UPNP_DM),)
INCLUDES+=-I../lib
endif
#endif/*TCSUPPORT_COMPILE*/
ifeq ($(TCSUPPORT_CT_UPNP_DM),)
INCLUDES+=-I../ctc-upnp-dm/libsockcmd
endif

ifneq ($(TCSUPPORT_CT),)
INCLUDES+=-I./cwmp/cwmpParameter_ct/Includes
else
INCLUDES+=-I./cwmp/cwmpParameter/Includes
endif

CFLAGS+= $(INCLUDES)
CFLAGS+= $(INCLUDE)
ifeq ($(TR111),1)
	CFLAGS += -DTR111
endif

#ifeq ($(TR143),1)
#	CFLAGS += -DTR143
#endif
#ifeq ($(DHCP_PROFILE),1)
#	CFLAGS += -DDHCP_PROFILE
#endif
ifeq ($(TCSUPPORT_CT),)
	CFLAGS+=-DCODE_SHRINK -DUPNP -DCWMP -DCLMP -DTHREAD_REDUCE
else
ifeq ($(CLMP),1)
	CFLAGS+=-DCODE_SHRINK -DUPNP -DCWMP -DCLMP -DTHREAD_REDUCE
else
	CFLAGS+=-DCODE_SHRINK -DUPNP -DCWMP -DTHREAD_REDUCE
endif 
endif
LDFLAGS+= $(INCLUDE)

#ifeq ($(SSL),1)
#CFLAGS += -DSSL
#endif
#ifeq ($(CWMP_MULTI_CA),1)
#CFLAGS += -DCWMP_MULTI_CA
#endif


ifeq ($(RT3390),1)
	CFLAGS += -DRT3390
endif
ifeq ($(WSC_AP_SUPPORT),1)
	CFLAGS += -DWSC_AP_SUPPORT
endif

ifneq ($(COMPILEOPTION_CFLAGS), )
		CFLAGS+=$(COMPILEOPTION_CFLAGS)
endif

ifneq ($(TC_CFLAGS),)
CFLAGS+=$(TC_CFLAGS)
endif
all: $(EXEC)

$(EXEC): $(OBJS)
	#$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)
	#$(CONVERT)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $(OBJS) $(EXTRALIBS) $(LDLIBS) $(TCAPILIB)
	$(CROSS)strip tr69

clean:
	$(RM) $(EXEC) *.elf $(RELEASEOBJS)

