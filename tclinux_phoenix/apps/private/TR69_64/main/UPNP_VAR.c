/*
  $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/private/TR69_64/main/UPNP_VAR.c#2 $
*/
/******************************************************************************/
/*
 *	Copyright (C) 1994 ZyXEL Communications, Corp.
 *	All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 */
/******************************************************************************/

/*
  $Log: UPNP_VAR.c,v $
  Revision 1.12  2011/07/29 00:46:52  xflu_sz
  for  #11139 compile error

  Revision 1.11  2011/07/28 10:55:39  xflu_sz
   #11139: add compile option keywork "TCSUPPORT_COMPILE"

  Revision 1.10  2011/06/16 11:20:54  xmdai_nj
  #10536

  Revision 1.9  2011/06/13 02:35:51  frankliao_hc
  fix compile error

  Revision 1.8  2011/06/13 01:36:35  frankliao_hc
  fix compile error

  Revision 1.7  2011/06/10 11:12:22  frankliao_hc
  bug 10460

  Revision 1.6  2011/05/19 03:07:01  frankliao_hc
  bug 10020

  Revision 1.5  2011/05/12 12:19:13  frankliao_hc
  10243

  Revision 1.4  2011/04/20 11:47:36  hzbian
  [Enhancement] BugFree ID: #9920
  Display Upnp icon title in WindowsXP according to the content of "Upnpd_Entry HostName"

  Revision 1.3  2011/03/04 12:17:48  lyxue_sz
  #8154

  Revision 1.2  2011/01/31 07:03:25  rclv_nj
  *** empty log message ***

  Revision 1.1.1.1  2010/09/30 21:00:13  josephxu
  apps/public, private

  Revision 1.1.1.1  2010/04/09 09:42:47  feiyan
  New TC Linux Make Flow Trunk

  Revision 1.2  2010/01/22 11:34:20  yzwang_nj
  [Bug#4688] 1.When double click the UPnP icon, it will jump to CPE GUI. 2.Modify UPnP Device Description Info.

  Revision 1.1  2009/12/24 12:21:27  feiyan
  [Feature] BugID:4407
  1.Implement TR069 and TR064 Engine under Linux
   2.Implement TR069 RPC Methods(Reboot, Factory Reset, Download, Upload)

  Revision 1.5  2009/09/10 12:52:40  xyyou
  #3509

  Revision 1.4  2009/09/04 02:08:44  jqzchu
  BugFree ID: 3558
  	Our CPE do not multicasts some devices' "NOTIFY" message.

  Revision 1.3  2008/04/02 16:06:02  here
  Ehance: Siemens request to support 9 TR-064 parameters

  Revision 1.2  2008/01/08 11:27:49  qingpu
  new feature:UPNP function for RT2880 WPS.

  Revision 1.1.1.1  2006/12/29 08:31:52  josephxu
  LiNOS transfer from Hsinchu to Suzhou (20061229)

  Revision 1.2  2005/07/07 06:08:56  jasonlin
  sync with old main trunk

  Revision 1.1.1.1  2005/03/30 14:04:30  jasonlin
  Import Linos source code

  Revision 1.4  2004/02/09 08:49:24  svtseng
  Modify UPNP for Billion, Add Wp1200 support ( add cli and http save)

  Revision 1.3  2003/11/28 07:13:00  svtseng
  Modify the Upnp descricption fro wrong VenderName and ProductionNumber

  Revision 1.2  2003/11/14 06:04:03  lino
  sync with trunk a9 web version, provide UPnP and Half Bridge features

  Revision 1.2  2002/08/07 09:15:13  sandyliu
  Revision 1.1  2002/07/11 05:25:02  sandyliu
  Initial revision
*/
//#include "rpProduct.h"
//#include "AsExtern.h"
//#include "upnp_var.h"
//#if defined(TCSUPPORT_C1_NEW_GUI) || defined(TCSUPPORT_TTNET)
#include "../../lib/libtcapi.h"

#include "libcompileoption.h"

//#endif
/* device */
#ifdef SIEMENS

char *device_UDN_IGD = "00000502-10000000-";
char *device_UDN_WAN = "00000502-10200000-";
char *device_UDN_WANConnection = "00000502-10201000-";
char *device_UDN_LAN = "00000502-10100000-";
#ifdef WSC_SUPPORT
#ifndef CLMP
char *device_UDN_WLAN = "00000502-10101000-";
#else
char *device_UDN_WFA = "00000502-10101000-";
#endif
#endif

#else

char *device_UDN_IGD = "11111111-";
char *device_UDN_WAN = "22222222-";
char *device_UDN_WANConnection = "33333333-";
char *device_UDN_LAN = "44444444-";
#ifdef WSC_SUPPORT
#ifndef CLMP
char *device_UDN_WLAN = "55555555-";
#else
char *device_UDN_WFA = "55555555-";
#endif
#endif

char *device_UDN_IGD_CLMP = "66666666-";


#endif

#define kUpnpUuidSize               40
#define URLBaseLength 32
#define serialNumberLength 64 /* < 64 */
#define UPCLength 13 /* 12-digit */
#define friendlyNameLength 64 /* < 64 */
#define manufacturerLength 64 /* < 64 */
#define manufacturerURLLength 64 /* optional */
#define modelDescriptionLength 128 /* < 128 */
#define modelNameLength 32 /* < 32 */
#define modelNumberLength 32 /* < 32 */
#define modelURLLength 64 /* optional */
#define presentationURLLength 32
#if defined __GLIBC__
#define strlcpy strncpy
#endif
char gRootUDN[kUpnpUuidSize] = "11111111-1111-1111-1111-111111111111";
char gWANUDN[kUpnpUuidSize] = "22222222-2222-2222-2222-222222222222";
char gWANConnectionUDN[kUpnpUuidSize] = "33333333-3333-3333-3333-333333333333";
char gLANUDN[kUpnpUuidSize] = "44444444-4444-4444-4444-444444444444";
#ifdef WSC_SUPPORT
#ifdef CLMP
char gWFAUDN[kUpnpUuidSize] = "55555555-5555-5555-5555-555555555555";
#else
char gWLANUDN[kUpnpUuidSize] = "55555555-5555-5555-5555-555555555555";
#endif
#endif

char gCLMPUDN[kUpnpUuidSize] = "66666666-6666-6666-6666-666666666666"; 


char gRootserialNumber[serialNumberLength] = "00000001";
char gWANserialNumber[serialNumberLength] = "00000001";
char gWANConnectionserialNumber[serialNumberLength] = "00000001";
char gLANserialNumber[serialNumberLength] = "00000001";
#ifdef WSC_SUPPORT
#ifdef CLMP
char gWFAserialNumber[serialNumberLength] = "00000001";
#else
char gWLANserialNumber[serialNumberLength] = "00000001";
#endif
#endif

char gRootUPC[UPCLength] = "000000000001";
char gWANUPC[UPCLength] = "000000000001";
char gWANConnectionUPC[UPCLength] = "000000000001";
char gLANUPC[UPCLength] = "000000000001";
#ifdef WSC_SUPPORT
#ifdef CLMP
char gWFAUPC[UPCLength] = "000000000001";
#else
char gWLANUPC[UPCLength] = "000000000001";
#endif
#endif

#ifdef ZYXEL_TC3160
/*	HuanShiou Modify from specified name to neutral name	20031126*/
/*
char gRootfriendlyName[friendlyNameLength] = TC_OEM_VD " Internet Sharing Gateway";
char gWANfriendlyName[friendlyNameLength] =  TC_OEM_VD " Internet Sharing Gateway";
char gWANConnectionfriendlyName[friendlyNameLength] = TC_OEM_VD " Internet Sharing Gateway";
char gLANfriendlyName[friendlyNameLength] = TC_OEM_VD " Internet Sharing Gateway";

char gRootfriendlyName[friendlyNameLength] = " Internet Sharing Gateway";
char gWANfriendlyName[friendlyNameLength] =  " Internet Sharing Gateway";
char gWANConnectionfriendlyName[friendlyNameLength] = " Internet Sharing Gateway";
char gLANfriendlyName[friendlyNameLength] = " Internet Sharing Gateway";
*/
// modify 20040209
char gRootfriendlyName[friendlyNameLength] = " IGD";
char gWANfriendlyName[friendlyNameLength] =  " IGD";
char gWANConnectionfriendlyName[friendlyNameLength] = " IGD";
char gLANfriendlyName[friendlyNameLength] = " IGD";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANfriendlyName[friendlyNameLength] = " IGD";
#else
char gWFAfriendlyName[friendlyNameLength] = " IGD";
#endif
#endif

#else
char gRootfriendlyName[friendlyNameLength] = "Econet IGD";
char gWANfriendlyName[friendlyNameLength] =  "Econet IGD";
char gWANConnectionfriendlyName[friendlyNameLength] = "Econet IGD";
char gLANfriendlyName[friendlyNameLength] = "Econet IGD";
#endif


#ifdef ZYXEL_TC3160
/*	HuanShiou Modify from specified name to neutral name	20031126*/
/* we also have to reinit the information from igdfuncs.c	*/
//char gRootmanufacturer[manufacturerLength] = "ADSL Company";
char gWANmanufacturer[manufacturerLength] = "ADSL Company";
char gWANConnectionmanufacturer[manufacturerLength] = "ADSL Company";
char gLANmanufacturer[manufacturerLength] = "ADSL Company";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANmanufacturer[manufacturerLength] = "ADSL Company";
#else
char gWFAmanufacturer[manufacturerLength] = "ADSL Company";
#endif
#endif
#else

#if 0
#ifdef TCSUPPORT_C1_NEW_GUI
char gRootmanufacturer[manufacturerLength] = "ZyXEL Communications Corp.";
#define ENTRY_NAME "GUI_Entry0"
#endif

#ifdef TCSUPPORT_TTNET
char gRootmanufacturer[manufacturerLength] = "TTNET";
#define ENTRY_NAME "Upnpd_Entry"
#endif

#if defined(TCSUPPORT_C1_NEW_GUI) || defined(TCSUPPORT_TTNET)
char *getLANUPnPInformation1()
{
	char tmp[32] = {'\0'};

	if(tcapi_get(ENTRY_NAME, "HostName", tmp) != 0){
		memset(tmp, 0, sizeof(tmp));
	}

	return tmp;
}
char *getLANUPnPInformation2()
{
	char tmp[32] = {'\0'};
	char des[40] = {'\0'};

	if(tcapi_get(ENTRY_NAME, "HostName", tmp) != 0){
		memset(tmp, 0, sizeof(tmp));
		return tmp;
	}
	else
	{
		snprintf(des, 32, "%s %s", tmp, "IGD");
		return des;
	}
}
#else
char gRootmanufacturer[manufacturerLength] = "Ralink";
#endif
#endif
char gWANmanufacturer[manufacturerLength] = "Econet";
char gWANConnectionmanufacturer[manufacturerLength] = "Econet";
char gLANmanufacturer[manufacturerLength] = "Econet";
#endif

#ifdef ZYXEL_TC3160
char gRootmanufacturerURL[manufacturerURLLength] = "";
char gWANmanufacturerURL[manufacturerURLLength] = "";
char gWANConnectionmanufacturerURL[manufacturerURLLength] = "";
char gLANmanufacturerURL[manufacturerURLLength] = "";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANmanufacturerURL[manufacturerURLLength] = "";
#else
char gWFAmanufacturerURL[manufacturerURLLength] = "";
#endif
#endif
#else
char gRootmanufacturerURL[manufacturerURLLength] = "http://www.econet-inc.com";
char gWANmanufacturerURL[manufacturerURLLength] = "http://www.econet-inc.com";
char gWANConnectionmanufacturerURL[manufacturerURLLength] = "http://www.econet-inc.com";
char gLANmanufacturerURL[manufacturerURLLength] = "http://www.econet-inc.com";
#endif

#ifdef ZYXEL_TC3160
/*	HuanShiou Modify from specified name to neutral name	20031126*/
/*
char gRootmodelDescription[modelDescriptionLength] = TC_OEM_VD " Internet Sharing Gateway";
char gWANmodelDescription[modelDescriptionLength] = TC_OEM_VD " Internet Sharing Gateway";
char gWANConnectionmodelDescription[modelDescriptionLength] = TC_OEM_VD " Internet Sharing Gateway";
char gLANmodelDescription[modelDescriptionLength] = TC_OEM_VD " Internet Sharing Gateway";

char gRootmodelDescription[modelDescriptionLength] = " Internet Sharing Gateway";
char gWANmodelDescription[modelDescriptionLength] = " Internet Sharing Gateway";
char gWANConnectionmodelDescription[modelDescriptionLength] = " Internet Sharing Gateway";
char gLANmodelDescription[modelDescriptionLength] = " Internet Sharing Gateway";
*/
char gRootmodelDescription[modelDescriptionLength] = " IGD";
char gWANmodelDescription[modelDescriptionLength] = " IGD";
char gWANConnectionmodelDescription[modelDescriptionLength] = " IGD";
char gLANmodelDescription[modelDescriptionLength] = " IGD";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANmodelDescription[modelDescriptionLength] = " IGD";
#else
char gWFAmodelDescription[modelDescriptionLength] = " IGD";
#endif
#endif

#else
char gRootmodelDescription[modelDescriptionLength] = "Econet IGD";
char gWANmodelDescription[modelDescriptionLength] = "Econet IGD";
char gWANConnectionmodelDescription[modelDescriptionLength] = "Econet IGD";
char gLANmodelDescription[modelDescriptionLength] = "Econet IGD";
#endif

#ifdef ZYXEL_TC3160

/*	HuanShiou Modify from specified name to neutral name	20031126*/
/*
char gRootmodelName[modelNameLength] = TC_OEM_VD " ADSL Router";
char gWANmodelName[modelNameLength] = TC_OEM_VD " ADSL Router";
char gWANConnectionmodelName[modelNameLength] = TC_OEM_VD " ADSL Router";
char gLANmodelName[modelNameLength] = TC_OEM_VD " ADSL Router";

char gRootmodelName[modelNameLength] = "Internet Sharing Gateway";
char gWANmodelName[modelNameLength] = "Internet Sharing Gateway";
char gWANConnectionmodelName[modelNameLength] = "Internet Sharing Gateway";
char gLANmodelName[modelNameLength] = "Internet Sharing Gateway";
*/
char gRootmodelName[modelNameLength] = "IGD";
char gWANmodelName[modelNameLength] = "IGD";
char gWANConnectionmodelName[modelNameLength] = "IGD";
char gLANmodelName[modelNameLength] = "IGD";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANmodelName[modelNameLength] = "IGD";
#else
char gWFAmodelName[modelNameLength] = "IGD";
#endif
#endif

#else
char gRootmodelName[modelNameLength] = "Econet IGD";
char gWANmodelName[modelNameLength] = "Econet IGD";
char gWANConnectionmodelName[modelNameLength] = "Econet IGD";
char gLANmodelName[modelNameLength] = "Econet IGD";
#endif

#ifdef ZYXEL_TC3160
/*	HuanShiou Modify from specified name to neutral name	20031126*/
/* 	reinit it at igdfuncs.c	*/
char gRootmodelNumber[modelNumberLength] = "ADSL Modem/Router";
char gWANmodelNumber[modelNumberLength] = "ADSL Modem/Router";
char gWANConnectionmodelNumber[modelNumberLength] = "ADSL Modem/Router";
char gLANmodelNumber[modelNumberLength] = "ADSL Modem/Router";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANmodelNumber[modelNumberLength] = "ADSL Modem/Router";
#else
char gWFAmodelNumber[modelNumberLength] = "ADSL Modem/Router";
#endif
#endif
#else
char gRootmodelNumber[modelNumberLength] = "Econet";
char gWANmodelNumber[modelNumberLength] = "Econet";
char gWANConnectionmodelNumber[modelNumberLength] = "Econet";
char gLANmodelNumber[modelNumberLength] = "Econet";
#endif

#ifdef ZYXEL_TC3160
char gRootmodelURL[modelURLLength] = "";
char gWANmodelURL[modelURLLength] = "";
char gWANConnectionmodelURL[modelURLLength] = "";
char gLANmodelURL[modelURLLength] = "";
#ifdef WSC_SUPPORT
#ifndef CLMP
char gWLANmodelURL[modelURLLength] = "";
#else
char gWFAmodelURL[modelURLLength] = "";
#endif
#endif
#else
char gRootmodelURL[modelURLLength] = "http://www.econet-inc.com/";
char gWANmodelURL[modelURLLength] = "http://www.econet-inc.com/";
char gWANConnectionmodelURL[modelURLLength] = "http://www.econet-inc.com/";
char gLANmodelURL[modelURLLength] = "http://www.econet-inc.com/";
#endif

char gURLBase[URLBaseLength] = "/";
char gpresentationURL[presentationURLLength] = "/";

#if !defined(TCSUPPORT_C1_NEW_GUI) 
#define ENTRY_NAME "Upnpd_Entry"
#endif

#if defined (ZYXEL_TC3160)
char gRootmanufacturer[manufacturerLength] = "ADSL Company";
#else
#if !defined(TCSUPPORT_C1_NEW_GUI) 
#if !defined(TCSUPPORT_TTNET) 
char gRootmanufacturer[manufacturerLength] = "Econet";
#endif
#endif
#endif

char des_manufacturer[256] = {'\0'};

#if 0
char *getLANUPnPInformation1()
{
	char tmp[32] = {'\0'};

	if(tcapi_get(ENTRY_NAME, "HostName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmanufacturer);
		return des_manufacturer;
//		memset(tmp, 0, sizeof(tmp));
	}

	snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	return des_manufacturer;
}

char *getLANUPnPInformation2()
{
	char tmp[32] = {'\0'};
//	char des[40] = {'\0'};

	if(tcapi_get(ENTRY_NAME, "HostName", tmp) != 0){
//		memset(tmp, 0, sizeof(tmp));
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s %s", gRootmanufacturer, "IGD");
		return des_manufacturer;
//		return tmp;
	}
	else
	{
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s %s", tmp, "IGD");
		return des_manufacturer;
	}
}
#endif

void DeviceInfoReinit(void){

	char tmp[32] = {'\0'};	
	if(TCSUPPORT_CT_PON_C9_VAL){
		
		if(tcapi_get("DeviceInfo_devParaStatic","ModelName",tmp) == 0){
		
			sprintf(gRootmodelName, "%s %s",tmp, "IGD");
			sprintf(gWANmodelName, "%s %s",tmp, "IGD");
			sprintf(gWANConnectionmodelName, "%s %s",tmp, "IGD");
			sprintf(gLANmodelName, "%s %s",tmp, "IGD");
			
			sprintf(gRootfriendlyName, "%s %s",tmp, "IGD");
			sprintf(gWANfriendlyName, "%s %s",tmp, "IGD");
			sprintf(gWANConnectionfriendlyName, "%s %s",tmp, "IGD");
			sprintf(gLANfriendlyName, "%s %s",tmp, "IGD");

			sprintf(gRootmodelNumber, "%s",tmp);
			sprintf(gWANmodelNumber, "%s",tmp);
			sprintf(gWANConnectionmodelNumber, "%s",tmp);
			sprintf(gLANmodelNumber, "%s",tmp);

			sprintf(gRootmodelDescription, "%s %s",tmp, "IGD");
			sprintf(gWANmodelDescription, "%s %s",tmp, "IGD");
			sprintf(gWANConnectionmodelDescription, "%s %s",tmp, "IGD");
			sprintf(gLANmodelDescription, "%s %s",tmp, "IGD");

			sprintf(gRootmodelURL, "%s","http://www.tp-link.com/");
			sprintf(gWANmodelURL, "%s","http://www.tp-link.com/");
			sprintf(gWANConnectionmodelURL, "%s","http://www.tp-link.com/");
			sprintf(gLANmodelURL, "%s","http://www.tp-link.com/");

			sprintf(gRootmanufacturer, "%s","TP-Link");

		}

	}	
	return;
}

/* frank added for bug 10020 */
char *getLANUPnP_FriendlyName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gRootfriendlyName;
#endif

	if (tcapi_get(ENTRY_NAME, "HostName", tmp) != 0){
#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootfriendlyName);
#endif
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

/* frank added for bug 10020 */
char *getLANUPnP_ModelName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gRootmodelName;
#endif


#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
	if (tcapi_get(ENTRY_NAME, "ModelName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmodelName);
#endif
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

/* frank added for bug 10020 */
char *getLANUPnP_ModelNumber()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gRootmodelNumber;
#endif

#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
	if (tcapi_get(ENTRY_NAME, "ModelNumber", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmodelNumber);
#endif
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

/* frank added for bug 10020 */
char *getLANUPnP_ModelDescription()
{
	char tmp[32] = {'\0'};
	
#if defined(TCSUPPORT_CT)
	return gRootmodelDescription;
#endif

#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
	if (tcapi_get(ENTRY_NAME, "Description", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmodelDescription);
#endif
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

/* frank added for bug 10020 */
char *getLANUPnP_Manufacturer()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gRootmanufacturer;
#endif

#if !defined(TCSUPPORT_C1_OBM) && !defined(WIZARD_OI_GUI) 
#if !defined(TCSUPPORT_C1_NEW_GUI) && !defined(TCSUPPORT_TTNET) 
	if (tcapi_get(ENTRY_NAME, "Manufacturer", tmp) != 0){
#endif
#endif
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmanufacturer);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

/* frank added for bug 10460 */
char *getUPnP_ManufacturerURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gRootmanufacturerURL;
#endif

	if (tcapi_get(ENTRY_NAME, "ManufacturerURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmanufacturerURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_WANManufacturerURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANmanufacturerURL;
#endif

	if (tcapi_get(ENTRY_NAME, "WANmanufacturerURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANmanufacturerURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_WANConnectionManufacturerURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionmanufacturerURL;
#endif

	if (tcapi_get(ENTRY_NAME, "WANconnection_manufacturerURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionmanufacturerURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_LANManufacturerURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANmanufacturerURL;
#endif

	if (tcapi_get(ENTRY_NAME, "LANmanufacturerURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANmanufacturerURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_ModelURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gRootmodelURL;
#endif

	if (tcapi_get(ENTRY_NAME, "ModelURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gRootmodelURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_WANModelURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANmodelURL;
#endif

	if (tcapi_get(ENTRY_NAME, "WANmodelURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANmodelURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_WANConnectionModelURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionmodelURL;
#endif

	if (tcapi_get(ENTRY_NAME, "WANconnection_modelURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionmodelURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

/* frank added for bug 10460 */
char *getUPnP_LANModelURL()
{
	char tmp[256] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANmodelURL;
#endif

	if (tcapi_get(ENTRY_NAME, "LANmodelURL", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANmodelURL);
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
	}
	return des_manufacturer;
}

char *getWANfriendlyName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANfriendlyName;
#endif

	if (tcapi_get(ENTRY_NAME, "WANfriendlyName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANfriendlyName);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getWANmanufacturer()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANmanufacturer;
#endif

	if (tcapi_get(ENTRY_NAME, "WANmanufacturer", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANmanufacturer);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

char *getWANmodelDescription()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANmodelDescription;
#endif

	if (tcapi_get(ENTRY_NAME, "WANmodelDescription", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANmodelDescription);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getWANmodelName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANmodelName;
#endif

	if (tcapi_get(ENTRY_NAME, "WANmodelName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANmodelName);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getWANmodelNumber()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANmodelNumber;
#endif

	if (tcapi_get(ENTRY_NAME, "WANmodelNumber", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANmodelNumber);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

char *getWANConnectionfriendlyName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionfriendlyName;
#endif

	if (tcapi_get(ENTRY_NAME, "WANConnectionfriendlyName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionfriendlyName);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getWANConnectionmodelDescription()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionmodelDescription;
#endif

	if (tcapi_get(ENTRY_NAME, "WANConnectionmodelDescription", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionmodelDescription);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}


char *getWANConnectionmodelName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionmodelName;
#endif

	if (tcapi_get(ENTRY_NAME, "WANConnectionModelName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionmodelName);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getWANConnectionmodelNumber()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionmodelNumber;
#endif

	if (tcapi_get(ENTRY_NAME, "WANConnectionModelNumber", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionmodelNumber);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

char *getWANConnectionmanufacturer()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gWANConnectionmanufacturer;
#endif

	if (tcapi_get(ENTRY_NAME, "WANConnectionmanufacturer", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gWANConnectionmanufacturer);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}


char *getLANfriendlyName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANfriendlyName;
#endif

	if (tcapi_get(ENTRY_NAME, "LANfriendlyName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANfriendlyName);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getLANmanufacturer()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANmanufacturer;
#endif

	if (tcapi_get(ENTRY_NAME, "LANmanufacturer", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANmanufacturer);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

char *getLANmodelDescription()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANmodelDescription;
#endif

	if (tcapi_get(ENTRY_NAME, "LANmodelDescription", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANmodelDescription);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getLANmodelName()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANmodelName;
#endif

	if (tcapi_get(ENTRY_NAME, "LANmodelName", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANmodelName);
		return des_manufacturer;
	} else {
#if !defined(TCSUPPORT_C1_OBM) 
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
#endif
		return des_manufacturer;
	}
}

char *getLANmodelNumber()
{
	char tmp[32] = {'\0'};

#if defined(TCSUPPORT_CT)
	return gLANmodelNumber;
#endif

	if (tcapi_get(ENTRY_NAME, "LANmodelNumber", tmp) != 0){
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", gLANmodelNumber);
		return des_manufacturer;
	} else {
		snprintf(des_manufacturer, sizeof(des_manufacturer), "%s", tmp);
		return des_manufacturer;
	}
}

char *getUPnP_SerialNumber()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gRootserialNumber;
#endif
}
char *getUPnP_WANSerialNumber()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gWANserialNumber;
#endif
}
char *getUPnP_WANConnectionSerialNumber()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gWANConnectionserialNumber;
#endif
}
char *getUPnP_LANSerialNumber()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gLANserialNumber;
#endif
}
char *getUPnP_ProCode()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gRootUPC;
#endif
}
char *getUPnP_WANProCode()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gWANUPC;
#endif
}
char *getUPnP_WANConnectionProCode()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gWANConnectionUPC;
#endif
}
char *getUPnP_LANProCode()
{
#if !defined(TCSUPPORT_C1_OBM) 
	return gLANUPC;
#endif
}

void initUpnpGlobalValue()
{
	if(TCSUPPORT_CT_PON_SK_VAL)
	{
		strlcpy(gRootfriendlyName, "Skyworth IGD", friendlyNameLength);
		strlcpy(gWANfriendlyName, "Skyworth IGD", friendlyNameLength);
		strlcpy(gWANConnectionfriendlyName, "Skyworth IGD", friendlyNameLength);
		strlcpy(gLANfriendlyName, "Skyworth IGD", friendlyNameLength);

		strlcpy(gRootmodelDescription, "Skyworth IGD", modelDescriptionLength);
		strlcpy(gWANmodelDescription, "Skyworth IGD", modelDescriptionLength);
		strlcpy(gWANConnectionmodelDescription, "Skyworth IGD", modelDescriptionLength);
		strlcpy(gLANmodelDescription, "Skyworth IGD", modelDescriptionLength);

		strlcpy(gRootmodelName, "Skyworth IGD", modelNameLength);
		strlcpy(gWANmodelName, "Skyworth IGD", modelNameLength);
		strlcpy(gWANConnectionmodelName, "Skyworth IGD", modelNameLength);
		strlcpy(gLANmodelName, "Skyworth IGD", modelNameLength);

		strlcpy(gRootmodelNumber, "Skyworth", modelNumberLength);
		strlcpy(gWANmodelNumber, "Skyworth", modelNumberLength);
		strlcpy(gWANConnectionmodelNumber, "Skyworth", modelNumberLength);
		strlcpy(gLANmodelNumber, "Skyworth", modelNumberLength);

		strlcpy(gRootmanufacturer, "Skyworth", manufacturerLength);
		strlcpy(gWANmanufacturer, "Skyworth", manufacturerLength);
		strlcpy(gWANConnectionmanufacturer, "Skyworth", manufacturerLength);
		strlcpy(gLANmanufacturer, "Skyworth", manufacturerLength);

		strlcpy(gRootmanufacturerURL, "http://www.skyworthdigital.com", manufacturerURLLength);
		strlcpy(gWANmanufacturerURL, "http://www.skyworthdigital.com", manufacturerURLLength);
		strlcpy(gWANConnectionmanufacturerURL, "http://www.skyworthdigital.com", manufacturerURLLength);
		strlcpy(gLANmanufacturerURL, "http://www.skyworthdigital.com", manufacturerURLLength);

		strlcpy(gRootmodelURL, "http://www.skyworthdigital.com", modelURLLength);
		strlcpy(gWANmodelURL, "http://www.skyworthdigital.com", modelURLLength);
		strlcpy(gWANConnectionmodelURL, "http://www.skyworthdigital.com", modelURLLength);
		strlcpy(gLANmodelURL, "http://www.skyworthdigital.com", modelURLLength);
	}
}

