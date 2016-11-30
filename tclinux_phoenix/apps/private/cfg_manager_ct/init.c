/*
 * init.c
 *
 *  Created on: Mar 20, 2009
 *      Author: krammer
 */
#include "cfg_manager.h"
#if defined(TCSUPPORT_CT_PON)
#include "xpon.h"
#endif

#if defined(TCSUPPORT_CT_OMCI_CFG_SHRINK)
node_init init_table[]=
{
#if defined(TCSUPPORT_CT_PON)
#ifdef TCSUPPORT_WAN_EPON
			epon_init,
#endif
#ifdef TCSUPPORT_EPON_OAM
			oam_init,
#endif
#ifdef TCSUPPORT_WAN_GPON
			gpon_init,
#endif
#ifdef TCSUPPORT_PMMGR
			pmmgr_init,
#endif
#ifdef TCSUPPORT_OMCI
			omci_init,
#endif
#if defined(TCSUPPORT_WAN_EPON) || defined(TCSUPPORT_WAN_GPON)
			xpon_node_init,
#endif
#endif
			system_init,
			info_init,
			NULL,
};
#else
node_init init_table[]=
{
#if defined(TCSUPPORT_CT_PON)
#ifdef TCSUPPORT_WAN_EPON
		epon_init,
#endif
#ifdef TCSUPPORT_EPON_OAM
		oam_init,
#endif
#ifdef TCSUPPORT_WAN_GPON
		gpon_init,
#endif
#ifdef TCSUPPORT_PMMGR
		pmmgr_init,
#endif
#ifdef TCSUPPORT_OMCI
		omci_init,
#endif
#if defined(TCSUPPORT_WAN_EPON) || defined(TCSUPPORT_WAN_GPON)
		xpon_node_init,
#endif
#endif
/*pork20090310 added*/
		sysInfo_init,
/*pork20090310 added over*/
#if defined(TCSUPPORT_CT_IPV4_RADIO)		
		Sys_init,
#endif
		lan_init,
		wan_init,
		wanInfo_init,
#ifndef PURE_BRIDGE
#if defined(TCSUPPORT_RIPD)
		ripd_init,
#endif
#ifndef TCSUPPORT_DNSEACHPVC		
		dproxy_init,	
#endif
#if defined(TCSUPPORT_CT_PPPOEPROXY)
		pppoeproxy_init,
#endif
		/* added by rclv_nj 20100519 */
#ifdef IPV6
		radvdCall_init,
		dhcp6sCall_init,
#endif		
		dhcpd_init,
		dhcpRelay_init,
		dhcpLease_init,
		lanHost_init,
		wlan_init,
#if defined(TCSUPPORT_WLAN_AC)
		wlan11ac_init,
#endif
		firewall_init,
		route_init,
#ifdef IPV6
		route6_init,
#endif
		nat_init,//10
		dmz_init,//11
		virServ_init,
#if defined(TCSUPPORT_CT_E8GUI)
		algswitch_init,
#endif
#endif
		adsl_init,
#ifndef PURE_BRIDGE
		snmpd_init,
		upnpd_init,
		ddns_init,
		timezone_init,
		mac_init,
#ifdef CWMP
		cwmp_init,
#if !defined(TCSUPPORT_CT_PON)
		//add by brian for dsl diagnostic
		cwmp_dsl_diagnostic_init,
		//add by brian for atm f5 loopback diagnostic
		cwmp_f5loopback_diagnostic_init,
#endif
		wifiMacTab_init,	//xyzhu_nj_20091209
		tr069Attr_init,
		IPInterface_init,
		cwmpRoute_init,
		switchpara_init,
#endif
#endif
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT) || defined(TCSUPPORT_TRUE_LANDING_PAGE)
		portal_init,
#endif
		account_init,
#if defined(CT_COM_DEVICEREG) || defined(TCSUPPORT_CT_PON_BIND2_WEB)
		deviceAccount_init,
#endif
		system_init,
		webCurSet_init,
		 /* add by tony for webPage customized */
		webCustom_init,
		dynDisp_init,
#if defined(TCSUPPORT_CT_GUIACCESSLIMIT)
		dynCwmpAttr_init,
#endif
#if defined(TCSUPPORT_CT_MONITORCOLLECTOR)
		MonitorCollector_init,
#endif
#if defined(TCSUPPORT_CT_PON_C9)
		reginfo_init,
#endif
#if defined(TCSUPPORT_C7)
		settinginfo_init,
#endif
		diagnostic_init,
		deviceInfo_init,
		info_init,
#ifndef PURE_BRIDGE
		#if QOS_REMARKING  /*Rodney_20090724*/
		qos_init,
		#endif
#ifdef TCSUPPORT_QOS
		qos_init,
#endif
#if defined(TCSUPPORT_CT_VLAN_BIND)
		vlanbind_init,
#endif
#if 0//def TCSUPPORT_PORTBIND
		portbind_init,
#endif
		acl_init,
		ipfilter_init,
		app_filter_init,
		url_filter_init,
#endif
		#if VOIP
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
		voip_sys_param_init,
#endif
		voip_basic_init,
		voip_call_ctrl_init,
		voip_media_init,
		voip_speed_dial_init,
		voip_codec_init,
		voip_log_init,	/* jrchen added 20100222*/	
    voip_digitmap_init,
		voip_advanced_init,
		voip_test_init,
		voip_simulate_test_init,
		voip_h248_init,
		voip_ims_init,
		voip_diagnostic_init,
		voip_poor_ql_init,
		voip_info_init,
#if defined(TCSUPPORT_ECN_MEGACO)
		voip_info_248_init,
#endif
		#endif	
		autopvc_init,//30
#ifdef SSL
		sslca_init,
#endif
#if defined (CWMP) && defined(TR111)
		dhcpClient_init,
		dhcpClientLimit_init,
#endif
#ifdef ALIAS_IP
		lanAlias_init,
#endif
#ifdef TCSUPPORT_DMS
		dms_init,
#endif
#if defined(TCSUPPORT_IGMP_PROXY)
		igmpproxy_init,
#endif
#ifdef TCSUPPORT_MLD_PROXY
		mldproxy_init,
#endif
#ifdef TCSUPPORT_SYSLOG
		syslog_init,/* Add by yzwang 20100719 */
#if defined(TCSUPPORT_C1_CUC)
		alarmlog_init,
#endif
#endif

#if defined(TCSUPPORT_CT_ACCESSLIMIT)
		accesslimit_init,
#endif
#if defined(TCSUPPORT_EPON_OAM_CUC)
		cuc_ping_test_init,
#endif
#if defined(TCSUPPORT_PPPOE_SIMULATE)
		pppoe_simulate_init,
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	ctcom_ippingdiagnotic_init,
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		configAuth_init,
		phyPort_init,
#endif
#if defined(TCSUPPORT_CT_ALARMMONITOR)
		devAlarm_init,
		devMonitor_init,
#endif
#if defined(TCSUPPORT_CT_FTP_DOWNLOADCLIENT)
		appftp_init,/* Add by lucl 20100920 */
#endif
#if defined(TCSUPPORT_CT_USB_BACKUPRESTORE)
		usbrestore_init,/* add by zhanghx 20100929 */
#endif
#ifdef TCSUPPORT_IPV6
#if !defined(TCSUPPORT_CT_IPV4_RADIO)		
		Sys_init,
#endif
#endif
#if defined(TCSUPPORT_CT_LOOPDETECT)
		loopbackdetect_init,
#endif
		dataspeedlimit_init,
#if defined(TCSUPPORT_CT_DSL_EX)
		wanATM_init,
		wanPTM_init,
		virServATM_init,
		virServPTM_init,
		routeATM_init,
		routePTM_init,
		route6ATM_init,
		route6PTM_init,
		dmzATM_init,
		dmzPTM_init,
		igmpproxyATM_init,
		igmpproxyPTM_init,
		mldproxyATM_init,
		mldproxyPTM_init,
#if defined(TCSUPPORT_CT_E8DDNS)
		ddnsATM_init,
		ddnsPTM_init,
#endif
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
		user_init,
		usr_macfilter_init,
		usr_ipfilter_init,
		usr_whiteurl_init,
		usr_blackurl_init,
#endif
#if defined(TCSUPPORT_CT_PHONEAPP)
		mobile_init,
#endif
#if defined(TCSUPPORT_CT_JOYME)
		pluginlist_init,
		usbmount_init,
#endif
#if defined(TCSUPPORT_CT_L2TP_VPN)
		vpnlist_init,
#endif

#if defined(TCSUPPORT_CT_UPNP_DM)
		upnp_dm_init,
		upnpdm_SoftwareProfile_init,
		upnpdm_ServiceProfile_init,
		upnpdm_APInternet_init,
		upnpdm_APIptv_init,
#endif
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
		LogicID_init,
#endif
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
		SimCard_init,	
#endif
#if defined(TCSUPPORT_CT_PPPOE_EMULATOR)
		pppoeEmulator_init,
		pppoeEmulatorBridge_init,
		pppoeEmulatorBridgeInfo_init,
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
		ipoeEmulator_init,
#endif
#if defined(TCSUPPORT_CT_PMINFORM)
		PMInform_init,
#endif
#if defined(TCSUPPORT_CT_STB_TEST)
		StbTest_init,
#endif
#if defined(TCSUPPORT_CT_JOYME)
		storage_access_right_init,
		attach_wan_dnstunnel_init,
		OSGI_Upgrade_init,
		laninfo_init,
		dns_speed_limit_init,
#if defined(TCSUPPORT_CT_JOYME_BANDWIDTH)
		devBandwidth_init,
		devMaxBandwidth_init,
#endif
#endif
		//Node should be added before this line!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		autoexec_init,
		NULL,
};
#endif

