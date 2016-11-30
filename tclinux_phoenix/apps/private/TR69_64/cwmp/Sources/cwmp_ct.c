
/*
** $Id$
*/
/************************************************************************
 *
 *	Copyright (C) 2005 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
/*
** $Log: cwmp.c,v $
** Revision 1.1.1.1.10.37  2011/07/15 05:50:07  denny
** Bug #10956
** Support RandomInform
**
** Revision 1.1.1.1.10.36  2011/07/06 12:10:02  gcyin_nj
** help to commit bug 10820
**
** Revision 1.1.1.1.10.35  2011/07/05 11:59:38  leidong
** Bug#10818:
** 1. Add nodes: AdminWebIP,UserName,UserPasswd,UserWebIP in
** InternetGatewayDevice.X_CU_Function.Web;
** 2. Do not add VALUE CHANGE event code when send  BOOTSTRAP inform packet  if the ACS URL changed. the same as date.
**
** Revision 1.1.1.1.10.34  2011/07/05 03:25:15  hzbian
** #10727
**
** Revision 1.1.1.1.10.33  2011/07/04 11:11:51  taoliu
** #10824, random inform interval arithmetic modify, make sure random interval is more than 30 seconds.
**
** Revision 1.1.1.1.10.32  2011/07/01 14:17:13  xflu_sz
** #10824:support random inform
**
** Revision 1.1.1.1.10.31  2011/06/26 11:35:29  hzbian
** #10727
**
** Revision 1.1.1.1.10.30  2011/06/25 09:40:41  hzbian
** #10727 #10728
**
** Revision 1.1.1.1.10.29  2011/03/11 08:36:16  feiyan
** Bug 9371
**
** Revision 1.1.1.1.10.28  2011/01/30 09:18:26  xmdai_nj
** #8479: If get wan IP failed, init tr69 host tree will be failed.
**
** Revision 1.1.1.1.10.27  2011/01/19 04:59:54  xmdai_nj
** #8553:Power off during TR69 download firmware, CPE did not send download error information to ACS after CPE power on
**
** Revision 1.1.1.1.10.26  2011/01/17 11:45:10  xhshi
** #8478
**
** Revision 1.1.1.1.10.25  2010/12/22 03:24:09  rclv_nj
** bug 8131
**
** Revision 1.1.1.1.10.24  2010/12/17 13:18:35  xhshi
** #8088
**
** Revision 1.1.1.1.10.23  2010/12/10 08:37:07  feiyan
** Bug7947
**
** Revision 1.1.1.1.10.22  2010/11/25 01:35:03  xyzhu_nj
** #7767
**
** Revision 1.1.1.1.10.21  2010/11/16 02:03:23  xhshi
** #7653
**
**  Change the mechanism of fetching device info
**   -fetch data from node "DeviceInfo"
**
** Revision 1.1.1.1.10.20  2010/11/05 10:51:10  xhshi
** fix compile error
**
** Revision 1.1.1.1.10.19  2010/11/05 08:20:45  rclv_nj
** #7483
** TR069 device registration function fails in CT ITMS test.
**
** Revision 1.1.1.1.10.18  2010/11/04 02:51:15  xhshi
** #7466
** If paramter type does not match,cpe will return error information,and
**   test will fail.
**
** Revision 1.1.1.1.10.17  2010/11/03 09:11:54  xhshi
** #7457
**  1.Support three parameter node as follows and related function:
**   -InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.{i}.Enable
**   -InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.{i}.MaxBitRate
**   -InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.{i}.DuplexMode
**
**  2.Support setting of connection request auth mode
**  3.Fix some error about setting of attributes
**
** Revision 1.1.1.1.10.16  2010/11/01 08:57:50  xmdai_nj
** #7453:Wrong event code name of "X CT-COM ACCOUNTCHANGE".
**
** Revision 1.1.1.1.10.15  2010/10/28 10:57:38  xyzhu_nj
** #7330 & 7192: it will timeout if save enable qos page.
**
** Revision 1.1.1.1.10.14  2010/10/28 03:48:28  xhshi
** 7324
** 1.New mechinasm that support the auto setting of parameters,such as productclass,manufacturerOUI and serialnum.
** 2.Add production line cmds.
**
** Revision 1.1.1.1.10.13  2010/09/28 08:50:24  xmdai_nj
** *** empty log message ***
**
** Revision 1.1.1.1.10.12  2010/09/26 08:34:14  rclv_nj
** bug 6667: device registration
**
** Revision 1.1.1.1.10.11  2010/09/26 03:02:56  xmdai_nj
** #6966:Modify the status when acs is null and WAN connection is not right.
**
** Revision 1.1.1.1.10.10  2010/09/26 02:38:36  xhshi
** feature enchancement
**
** Revision 1.1.1.1.10.9  2010/09/20 10:54:46  rclv_nj
** bug 6967:adding mutex lock in wan node commit function
**
** Revision 1.1.1.1.10.8  2010/09/20 08:55:44  yzwang_nj
** [Enhancement] #6692 Add Alarm log and SysLog TR69 Node
**
** Revision 1.1.1.1.10.7  2010/09/20 07:09:31  yzwang_nj
** [Enhancement] #6693 Support Device Alarm & Monitor
**
** Revision 1.1.1.1.10.6  2010/09/20 04:49:35  xmdai_nj
** #6966:Support new functions of TR69 Manual inform and Maitenance inform
**
** Revision 1.1.1.1.10.5  2010/09/19 14:40:52  xmdai_nj
** #6966:Support new functions of TR69 Manual inform and Maitenance inform
**
** Revision 1.1.1.1.10.4  2010/09/19 13:57:55  xmdai_nj
** #6806:modify TR69 IPPingDiagnostics function to add route for differernt interface
**
** Revision 1.1.1.1.10.3  2010/09/17 15:07:58  rclv_nj
** bug 6967:TR069 WAN nodes support on SMUX interface
**
** Revision 1.1.1.1.10.2  2010/09/16 05:03:03  xyzhu_nj
** #6698:
** 1.Add support qos parameters for China telecom req.
**
** Revision 1.1.1.1.10.1  2010/09/13 05:55:29  xyzhu_nj
** #6794:
** 1.Add support portbinding, default route, traffic binding.
** 2.support inform ppp username of 'INTERNEET' interface to itms.
**
** Revision 1.1.1.1  2010/04/09 09:42:44  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.16  2010/04/06 11:54:36  xyzhu_nj
** commit for compile error.
**
** Revision 1.15  2010/03/22 02:38:32  xyzhu_nj
** [bug#4725]: Add TR143 parameters: IGD.Capabilities / IGD.DownloadDiagnostics. / IGD.UploadDiagnostics.
** [bug#4837[: Add TR143 parameters:
** IGD.UDPEchoConfig.
**
** Revision 1.14  2010/02/02 08:22:29  feiyan
** [Feature] BugID:4710 Reduce TR069 Thread number from 5 to 1.
**
** Revision 1.13  2010/01/15 13:01:30  xyzhu_nj
** [#bug4591] Add TR069 Paramters. IGD.DeviceInfo.
**
** Revision 1.12  2010/01/13 04:31:20  xyzhu_nj
** Add tr069 table parameter.
**
** Revision 1.11  2010/01/07 12:34:55  xyzhu_nj
** [bug#4403]:add tr069 parameters
**
** Revision 1.10  2009/12/31 02:08:13  feiyan
** [Feature] BugID:4407
**
** Revision 1.9  2009/12/30 14:42:39  xyzhu_nj
** [bug#4403]:add tr069 parameters
**
** Revision 1.7  2009/12/30 12:20:54  xyzhu_nj
** [bug#4403]:Add tr069 parameters.
**
** Revision 1.6  2009/12/30 08:20:45  jlliu_nj
** Bug#4406:Implement SSL for TR069
**
** Revision 1.5  2009/12/30 08:15:15  xyzhu_nj
** [bug#4403]:Add tr069 parameters.
**
** Revision 1.4  2009/12/30 06:49:29  xhshi
** [#4363]
** Implement  tr069 engine in linux platform and remove some debug messages
**
** Revision 1.3  2009/12/30 03:00:35  xhshi
** [#4366]
** Implement ATM F5 Loopback Diagnostic function in linux platform
**
** Revision 1.2  2009/12/30 02:23:50  xhshi
** [#4365]
** Implement DSL Diagnostic function in linux platform
**
** Revision 1.1  2009/12/24 12:21:28  feiyan
** [Feature] BugID:4407
** 1.Implement TR069 and TR064 Engine under Linux
**  2.Implement TR069 RPC Methods(Reboot, Factory Reset, Download, Upload)
**
** Revision 1.95  2009/10/29 07:07:05  pingtang_sz
** BugFree ID:#3814 & #3924
** ACS server can visit CPE when wifi is configured as open system.
**
** Revision 1.94  2009/08/28 02:24:43  xyzhu_nj
** [Feature]#3321,Generate OUI from the first 3 bytes of MAC.
**
** Revision 1.93  2009/08/21 13:38:59  taoliu
** #3490:os_str_realloc crash
**
** Revision 1.92  2009/08/21 07:46:27  taoliu
** #3393:ZyXEL ACS, add another cmd sys setfaultdetail [<0|1>] inside compile option ZYXEL_TURKEY
**
** Revision 1.91  2009/08/20 11:22:47  taoliu
** #3393:ZyXEL ACS 3.0 will exit current session if faultstring is too long
**
** Revision 1.90  2009/08/14 10:19:21  taoliu
** #3142,Modem has 30K available memory, do SSL download firmware, crash.
**
** Revision 1.89  2009/08/13 07:48:15  jqzchu
** Bugfree 3315:
** 	DLink TR069 request.
**
** Revision 1.88  2009/08/06 02:57:05  pingtang_sz
** 1).TR069 supporting multi-CA,now the number of CA is four
** 2).Change CA automatically when the current CA is invalid
** [Merge from Huawei]
** [release note:[DOC NO]:HRB5.0.125.0-42]
**
** Revision 1.87  2009/07/31 13:32:45  taoliu
** #3286, only CWMP, no SSL, compile error
**
** Revision 1.86  2009/07/31 13:30:10  feiyan
** [BugFix] ID:3347 TC2 TURKISH Version don not have CI: 'sys cwmp fixrstissue' and 'sys cwmp selectca'.
**
** Revision 1.85  2009/07/31 02:30:28  hzbian
** Add CMD for TPLINK_TURKISH
**
** Revision 1.84  2009/07/30 10:41:28  pingtang_sz
** 1).TR069 supporting multi-CA,now the number of CA is four
** 2).Add a new command to select TR069 CA index.
** 3).Add a new compile option CWMP_MULTI_CA.
**
** Revision 1.83  2009/07/30 09:34:08  feiyan
** [BugFix] ID:3290 MT880a can not connect with TTNET Motive ACS successfully.
**
** Revision 1.82  2009/07/24 13:32:58  hzbian
** [ENHANCEMENT] BugFree ID:#3258
** Merge code for random inform from Unicom branch to TC2 trunk
**
** Revision 1.81  2009/07/23 13:24:53  hzbian
** [Enhancement] BugFree ID: #2835
** 1) Make sure only ca with name of "CA Telefonica Moviles Espana.cer" can be
**     put into modem
** 2) Extend CA maximal length from 1280 to 1640
**
** Revision 1.80  2009/07/16 08:01:40  taoliu
** #2404
** Implement UDPEcho plus of TR143.
**
** Revision 1.79  2009/07/15 10:46:15  taoliu
** #3208
** Autotest run stress, serail hours later, continuously print a debug information "The name node does not exist!The device root is NULL"
**
** Revision 1.78  2009/07/13 09:11:08  xqxu
** Commit by Xueqi:Implement by jqzhou:
** #3179
** Fix:Do CI-CMD "sys cwmp setnovaluetype", "sys cwmp periodicinftime", "sys cwmp
** 	sendhostportnum", CMD won't be effected.
**
** Revision 1.77  2009/07/10 10:17:33  xqxu
** Commit by Xueqi: (Modify by kunliu)
** #2862
**  Implement TR069 parameters  for Telefonica Spain.
**
** Revision 1.76  2009/07/03 13:34:45  taoliu
** #2764, The second root cause of TR069 crash
**
** Revision 1.75  2009/07/03 09:47:36  pingtang_sz
** [Bugfix] #2759, Add some TR069 nodes.
**
** Revision 1.74  2009/07/02 09:14:17  taoliu
** #2705
** TOT TR069 test ACS server is unstable
**
** Revision 1.73  2009/06/19 13:36:03  taoliu
** #2404
** implement some TR069 parameter about DHCP option 60 and 240;
**
** Revision 1.72  2009/06/19 12:48:13  taoliu
** #2404
** TR143 HTTP download/upload diagnostics
**
** Revision 1.71  2009/06/19 09:07:31  taoliu
** #2764
** TR069 crash
**
** Revision 1.70  2009/06/05 13:12:39  hzbian
** [Enhancement]BugFree ID:#2619
** Implement TR069 parameters of IP filter for Telefonica Spain.
**
** Revision 1.69  2009/06/05 12:19:54  jlshi
** bug #2665
**
** Revision 1.68  2009/05/22 12:45:08  guangchu
** [feature]#2748 Disable "sys cwmp" command if MRD debug flag is set to 00.
**
** Revision 1.67  2009/05/14 05:56:30  guangchu
** [Bugfix]#2677 CPE cannot connect to ASB ACS.
**
** Revision 1.66  2009/04/30 11:04:39  taoliu
** #2489:
** fix bug for the parameter "Standard" under wireless
**
** Revision 1.65  2009/04/23 07:52:10  jlshi
** Merge from TC2 Branch for bug #2440
**
** Revision 1.64  2009/04/16 07:06:28  taoliu
** #2302,#2303
** TR069 can't add/delete pure bridge pvc
**
** Revision 1.63  2009/04/03 12:39:07  hzbian
** Open Source
**
** Revision 1.62  2009/04/02 08:43:01  taoliu
** #2300
** Enhancement for CLI "sys cwmp disp" delay
**
** Revision 1.61  2009/03/27 09:35:29  jqzchu
** BugFree ID: #2293
** TR064 support for C1.
** Merge form Huawei of Cwmp hardware version,
** change it form dmt version to CPE hardware version.
**
** Revision 1.60  2009/03/26 07:40:20  taoliu
** #2360
** TOT new request, need implement 2 NTP server in TR069;
**  Add the second NTP server on GUI.
**
** Revision 1.59  2009/03/20 01:13:12  xhshi
** [ENHANCEMENT]
** #1821,#1241
** merge from bharti branch and add other new layer2bridging parameters
**
** Revision 1.58  2009/03/06 10:03:04  jqzchu
** Bugfree: 2017
** Add some parameters to rom-t for InternetGatewayDevice.DeviceConfig.ConfigFile use.
**
** Revision 1.57  2009/03/06 05:58:59  feiyan
** [Enhancement]Merge TR069 receive timeout setting function from Huawei Branch 'Branch_for_STC_Linos501080_20090304',  Doc num:HRB5.0.108.50-1.
**
** Revision 1.56  2009/03/05 12:51:22  taoliu
** #2132: add some TR069 parameters:InternetGatewayDevice.QueueManagement.
**
** Revision 1.55  2009/02/12 11:32:33  taoliu
** #2017
** InternetGatewayDevice.DeviceConfig.ConfigFile
**
** Revision 1.54  2009/02/06 09:48:02  lufan
** [enhancement] #2011 set hardware version to bootbase.
**
** Revision 1.53  2009/02/05 04:48:48  jqzchu
** #2072:CPE may automatically reset to default romfile in on-off test.
**
** Revision 1.52  2009/01/09 11:33:36  taoliu
** Can't pass Bharti 0_touch test
**
** Revision 1.51  2008/12/26 10:44:51  jqzchu
** BugFree ID: #1742
**       TC2:add TR064/TR069 node: NewPeriodicInform
**
** Revision 1.50  2008/12/26 06:53:14  hzbian
** [Enhancement]BugFree ID: #1792
** Add a new compile option "MAX_TR69_PARAM=1" to
** support all the parameters of TR069.
**
** Revision 1.49  2008/12/08 02:21:11  jlshi
** bug#1089:Incorrect value of SoftwareVersion parameter.
**
** Revision 1.48  2008/12/05 11:03:07  xhshi
** [Bugfix]	1. Motive ACS can't connect to cpe if connectionrequest username or password is too long.
** 	1.login to Motive ACS test control page,do connection request test,and it failed.
**
** Revision 1.47  2008/11/26 08:38:32  hzbian
** [BUGFIX]#1077 #1082
** Interface is not correct, but diagnostics is still performed.
** "8 DIAGNOSTICS COMPLETE" is not taken out in periodic inform
** after diagnostics  is finished successfully.
**
** Revision 1.46  2008/11/21 08:28:44  hzbian
** [BUGFIX]#1115:    The value of some parameters in tcCwmpAttr_t is lost
** after firmware is upgraded.
**
** Revision 1.45  2008/11/13 10:55:06  jlshi
** bug#750
** Print disordered message when input CI COMMAND "sys cwmp disp".
**
** Revision 1.44  2008/11/03 11:51:45  feiyan
** [Enhancement] Merge C1 LEH(Branch_for_C1_211440_080728) to TC2 main trunk.
**
** Revision 1.43  2008/10/31 09:20:30  jqzchu
** BugFree ID:#207 #1078
** 	1.merge code from TRUE branch.(bug free No:#207)
** 	2.adjust WiFi TX power button in WiFi page
**
** Revision 1.42  2008/10/22 08:46:50  jqzchu
** BugFree ID:713(merge from huawei)
** Unplugging the phone line when downloading firmware by WorksSystem ACS, download failure but ACS returns download success after CPE restarting.
** see [DOC NO]:HRM5.0.78.0-9
**
** Revision 1.41  2008/10/17 11:38:19  xyyou
** C5 request:add a new page for its own version display and modem's
** up/downstream rate display
**
** Revision 1.40  2008/10/09 01:58:30  hzbian
** [BUGFIX]ACS keeps waiting for response and can not get the response after it sets some parameters.
**
** Revision 1.39  2008/09/26 09:47:18  hzbian
** Add function description to code
**
** Revision 1.38  2008/09/26 04:14:38  hzbian
** Fix a bug that CPE can not download a firmware from a HTTP server
** with digest authentication enabled.
**
** Revision 1.37  2008/09/11 02:35:07  haijian
** no message
**
** Revision 1.36  2008/09/10 09:30:36  haijian
** fix value change bug.
**
** Revision 1.35  2008/09/05 02:28:24  jqzchu
** 1.Feature for C0 Tango
** 2.CAT Feature
** 3.bug fix for TR069 parameter
**
** Revision 1.34  2008/08/29 02:19:01  hzbian
** Merge code from HW to TC2 for issues about upload,
** Connection Request URLand memory leakage
**
** Revision 1.33  2008/08/20 06:10:27  feiyan
** [Enhancement] Change Serial Number of TR069 from MAC Address to original Serial number for CC(CSE)
**
** Revision 1.32  2008/08/07 10:27:12  qingpu
** Enhancement:Support TR069 302 Redirect function.(Merge from Branch)
**
** Revision 1.31  2008/08/07 08:07:44  hzbian
** Fix a bug that CPE will crash when we set CI_CMD, sys cwmp cpeusername and sys cwmp cpepassword.
**
** Revision 1.30  2008/07/21 01:45:34  maojianli
** Add some cicmd for C9 TURKISH , Add a string in vlan_group page
**
** Revision 1.29  2008/07/14 12:47:53  haijian
** Fix Filter crash issue
**
** Revision 1.28  2008/07/14 05:30:56  haijian
** fix another potential double free bug.
**
** Revision 1.27  2008/07/11 03:01:06  haijian
** fix a potential double free bug.
**
** Revision 1.26  2008/07/07 02:24:55  jqzchu
** auto generated TR069 OUI form first 3 bytes of LAN Mac address
**
** Revision 1.25  2008/06/02 09:43:38  yyshen
** CB enhancement:
** 1. In TR069 web page, OTE need make all item disable.
** 2. Open CI Command to make people configuring enable/disable of the item.
**    CI Command:
**    "sys cwmp configure <enable | disable>"
**
** Revision 1.24  2008/04/16 02:18:46  qingpu
** Bugfix:
** 1.cpe crash when doing ipping using BEC ACS.
** 2.the path of ExternalIP included in Inform Packet is error.
** 3.set WEPKey fail when the value is 10 or 26 hex string.
**
** Revision 1.23  2008/04/11 08:10:38  qingpu
** Bug fix & Enhancement:
** 1.set ATM Qos type via TR069, its value on webpage do not update.
** 2.some TR069 parameter type is not correct.
** 3.add some node for WKS.
**
** 4.add a cmd "sys cwmp cushwversion [value]" to config customer Hardware version.
**
** Revision 1.22  2008/03/31 08:57:37  qingpu
** Enhance & bug fix:
** 1.add a command to config DeviceInfo.Description.
** 2.we don't send DSLDIAGNOSTICS COMPLETE event in inform packet when we complete dsldiagnostics.
**
** Revision 1.21  2008/02/22 01:14:23  yyshen
** [Enhancement]:
**  1. In TR069 web page, CG need make all item disable.
**  2. Add a CI Command to make people configuring enable/disable of the item.
**     CI Command added as follows:
**     "sys cwmp configure <enable | disable>"
**
** Revision 1.20  2008/01/10 11:20:23  qingpu
** Enhancement:close some TR69 ci-commands for code size.
**
** Revision 1.19  2008/01/10 10:47:00  qingpu
** Enhancement:close some TR69 ci-commands for code size.
**
** Revision 1.18  2008/01/10 01:58:14  jqzhu
** update files
**
** Revision 1.17  2008/01/09 08:43:36  jqzhu
** set the default TR069 username and path by CPE MAC
**
** Revision 1.16  2008/01/07 06:33:25  qingpu
** bug fix:stress test crash when TR069 enabled(merge from Huawei trunk)
**
** Revision 1.15  2007/12/29 16:32:36  qingpu
** workaround: Webpage can't access caused by merging TO2 crash problems.
**
** Revision 1.14  2007/12/28 05:33:32  qingpu
** bug fix:fix crash problems found in Huawei TO2.
**
** Revision 1.13  2007/12/19 05:10:06  yszhang
** add the function checking parameters of tr069
**
** Revision 1.12  2007/11/20 09:18:56  pingzhang
** add WAN_DSL_Diagnostics for tr069
**
** Revision 1.11  2007/11/16 11:55:18  pingzhang
** add F5LoopbackDiagnostics for tr069
**
** Revision 1.10  2007/11/08 08:10:31  qingpu
** bug fix:
** 1.when do GetRPCMethods on SSL with SupportSoft ACS,it will fail.
** 2.when do SetParameterValues, if have one parameter is vaild, other parameter still can set successfully.
** 3.set connectionrequest authencation default to Digest authencation.
**
** Revision 1.9  2007/10/09 03:28:14  qingpu
** Enhance:add digest authentication for TR069 connection request(merge from Huawei trunc).
**
** Revision 1.8  2007/09/30 08:09:24  qingpu
** bug fix(merge from Huawei truck):if we have no default route or default route have no wan ip, cpe can't send inform packet to acs even if some static route have wan ip.
**
** Revision 1.7  2007/09/24 10:55:48  qingpu
** bug fix(merge from Huawei trunc):if cpe use static route for TR069, the connectRequestURL is not correct, and external wan ip sent to ACS will be lan ip if don't have default route.
**
** Revision 1.6  2007/07/27 07:37:33  maojianli
** Merge 2_11_3_0 back to main trunk
**
** Revision 1.5  2007/03/01 10:54:07  xzwang
** Fix bug: On P3/L3 board, CPE may crash for access NULL point in function cwmp_strdup().
**
** Revision 1.4  2007/02/28 03:43:27  xzwang
** Enhancement: Realize Upload RPC.
** Enhancement: Realize ScheduleInform RPC.
**
** Revision 1.3  2007/02/26 08:19:10  xzwang
** Fix bug: In function "processSessionRetry()", character"==" should be "="
**
** Revision 1.2  2007/01/17 06:18:28  xzwang
** - Fix bug: Many global variables Which are used by multi thread are not defined as
** 	volatile varible.
** - Fix bug: If we plug off adsl line during CPE send packet to ACS and then plug on,
** 	CPE will send wrong packet to ACS since then.
**
** Revision 1.1.1.1  2006/12/29 08:27:26  josephxu
** LiNOS transfer from Hsinchu to Suzhou (20061229)
**
** Revision 1.8  2006/12/28 09:41:07  yen
** Merge newest tr069 code from Huawei trunk to Taiwan trunk to fix some bugs.
**
** Revision 1.7  2006/12/08 14:36:16  yen
** - Fix bug: CPE will crash if the length of download url is longer than 64.
**
** Revision 1.6  2006/12/05 12:08:09  yen
** - Merge TR-069 code after 10/1/2006 into Taiwan Trunk.
**
** Revision 1.5  2006/11/16 04:50:37  yen
** - Fix IOTissue: Connection request fails while test with SupportSoft ACS because SupportSoft
**                does not support authentication.
**
** Revision 1.4  2006/10/24 04:20:05  yen
** - Fix bug: Can not open cwmp switch for the first time successfully  if tcrom is not correct.
**
** Revision 1.3  2006/10/24 02:22:17  yen
** - Fix bug: Modem may reboot during testing TR-069 if tcrom is not correct.
**
** Revision 1.2  2006/10/09 03:28:11  yen
** Fix a bug in the code for resetting cookie value.
**
** Revision 1.1  2006/09/30 08:40:11  yen
** Merge TR-069 and TR-064 code into trunk.
**
** Revision 1.70.2.1  2006/09/28 03:28:15  qingpu
** Merge TR064 Phase1 Code
**
** Revision 1.70  2006/09/27 06:30:35  xzwang
** move some download code from cwmpParameter.c/cwmpRpc.c to cwmp.c
**
** Revision 1.69  2006/09/25 02:31:12  xzwang
** add function getCwmpModelName()
**
** Revision 1.68  2006/09/25 00:59:24  hqfeng
** - Add five ci-cmd: frombb, manufacturer,modelname,productclass,clearall.
**
** Revision 1.67  2006/09/22 02:27:49  xzwang
** no message.
**
** Revision 1.66  2006/09/21 10:27:31  xzwang
** add function "getTxBufLenFromMMCT"
**
** Revision 1.65  2006/09/21 07:16:39  hqfeng
** - Modify cwmpInitTxBuf() for getting the cwmpTxBuf length from mmct.tc
**
** Revision 1.64  2006/09/19 05:25:59  xzwang
** Add function for getting device summary.
**
** Revision 1.63  2006/09/19 00:54:00  xzwang
** Modify productClass to be TC3162L2-2M-16M for the version supporting wireless LAN.
**
** Revision 1.62  2006/09/18 08:23:31  xzwang
** - Set connetion request ip to be lan ip if all remote ip is 0.
** - Restore debug level to be 2.
**
** Revision 1.61  2006/09/18 03:07:25  xzwang
** - Modify default value of debug level to be 0.
**
** Revision 1.60  2006/09/14 10:22:23  hqfeng
** - Add WLAN_RT2561_SUPPORT macro for code.
**
** Revision 1.59  2006/09/13 04:41:21  hqfeng
** - Add for WLAN.
**
** Revision 1.58  2006/09/13 00:59:54  xzwang
** Add customerCA into spt.
** Signal inform task if acs url is modified.
**
** Revision 1.57  2006/09/08 10:50:42  xzwang
** Free customer CA before reconfigure it.
**
** Revision 1.56  2006/09/07 12:31:57  xzwang
** - Support download CA by download function.
** - Only disable periodic inform during download.
**
** Revision 1.55  2006/09/06 08:21:54  xzwang
** - Redirect to error message if connection request port is set to be the same with existed server.
**  - Allow connection request port to be the web server port.
**  - Fix the bug that connection request fails if we modify the path in URL.
**
** Revision 1.54  2006/09/05 12:27:28  xzwang
** - Use SpSave_old to replace spSave in ci-command for settiong periodic inform.
** - Store/fetch manufacture oui from spt.
**
** Revision 1.53  2006/09/01 05:32:47  xzwang
** Resolve the bug that get paramter value/name on entire object will cause
** modem reboot by the following means:
**  - Use os_malloc/os_free to replace malloc/free.
**  - Write a function os_str_realloc to replace realloc.
**  - Write a function cwmp_strdup to replace strdup.
**  - If malloc fails, Free response structure and document before build fault structure.
**  - Pre-allocate Tx buffer during cwmp initializaion.
**
** Revision 1.52  2006/08/29 12:08:00  xzwang
** do not read Manufacture and ProductClass/ModelName from bootbase except for C0 version.
**
** Revision 1.51  2006/08/29 08:19:19  xzwang
** - Fetch manufacture and product information from bootbase.
**
** Revision 1.50  2006/08/28 06:58:48  hqfeng
** - Add for HoldRequest.
**
** Revision 1.49  2006/08/27 05:34:50  xzwang
** Add define C0 for C0 version.
**
** Revision 1.48  2006/08/25 05:30:11  xzwang
** - Optimize code.
** - Resolve the problem that if ACS URL is modified by setting URL's value on ACS,
**   The current ACS can not receive response any more.
** - Resolve the problem that if ACS URL is modified by current ACS, the imform to new ACS
**   does not have "0 BOOTSTRAP" event.
**
** Revision 1.47  2006/08/24 04:12:17  xzwang
** Code optimization.
**
** Revision 1.46  2006/08/17 10:29:44  xzwang
** - Add one tcp listen socket for ACS connection request.
**
** Revision 1.45  2006/08/16 07:08:02  xzwang
** Initiate cwmp function if the cwmp switch is turned on.
**
** Revision 1.44  2006/08/15 13:32:32  xzwang
** no message
**
** Revision 1.43  2006/08/15 08:12:55  xzwang
** Enhance retry mechanism.
**
** Revision 1.42  2006/08/14 11:52:17  xzwang
** modify retry count to be 0 for tcp error.
**
** Revision 1.41  2006/08/14 08:28:28  haijian
** Allocate downloadstruct when init global variable rpc.
**
** Revision 1.40  2006/08/14 06:14:42  hqfeng
** - Add Session Retry Policy
** - Reduce code size.
**
** Revision 1.39  2006/08/11 09:08:04  xzwang
** - Optimize session initialize mechanism.
**
** Revision 1.38  2006/08/11 06:51:12  xzwang
** - Optimize session initialize mechanism.
** - add ci-command for set periodenable, periode interval, cwmp switch, provisionning code,
**   connection request port, connection request path,etc.
**
** Revision 1.37  2006/08/11 00:59:45  haijian
** no message
**
** Revision 1.36  2006/07/27 05:09:31  xzwang
** Do not send periodic inform packet to ACS if a request to ACS has already existed.
**
** Revision 1.35  2006/07/26 08:08:28  haijian
** Add save ParameterKey in flash.
**
** Revision 1.34  2006/07/21 15:35:45  hqfeng
** - Modify "7 TRANSFERCOMPLETE" to "7 TRANSFER COMPLETE" in cwmpBuildInformPkt().
**
** Revision 1.33  2006/07/21 14:39:13  hqfeng
** - Merge the code passed the test in UNH.
**
** Revision 1.32  2006/07/14 05:08:36  xzwang
** convert serial number to be high case.
**
** Revision 1.31  2006/07/13 11:42:11  xzwang
** change port during modify https flag by haijian.
**
** Revision 1.30  2006/07/13 05:43:16  xzwang
** - add ci-command "sys cwmp initEmptyPkt [0|1]' to control whether send empty packet the first time.
** - enlarge inform and value change tasks's stack to resolve crash problem after set portmapping attributes.
**
** Revision 1.29  2006/07/12 01:40:12  hqfeng
** - Modify for GetRPCMethods(CPE->ACS)
**
** Revision 1.28  2006/07/11 08:52:07  haijian
** Add UNH CA support.
** - Add ci-command: sys cwmp ca [0|1]
**
** Revision 1.27  2006/07/11 04:07:35  xzwang
** - optimize cwmp initialization.
**
** Revision 1.26  2006/07/11 03:10:27  haijian
** Fixed ACS URL bug.
**
** Revision 1.25  2006/07/10 12:09:33  xzwang
** add ci-command "sys cwmp sessionClear [0/1]" for clear sessions.
**
** Revision 1.24  2006/07/07 04:49:42  xzwang
** - handle https request by haijian.
**
** Revision 1.23  2006/07/07 04:47:47  xzwang
** get connnection request url correctly.
**
** Revision 1.22  2006/07/05 07:22:29  hqfeng
**  - Add getUnLockRealmsUserFlag() and setUnLockRealmsUserFlag().
**  - Modify doCwmpDebug() for debug Level3.
**
** Revision 1.21  2006/07/03 09:17:27  maojianli
** modify getExternalName() to find default route name
**
** Revision 1.20  2006/07/03 02:36:56  hqfeng
**  - Modify the value of ProductClass from 'TC3162L-2M-8M' to 'TC3162L2-2M-8M'.
**
** Revision 1.19  2006/07/02 05:41:19  xzwang
** - modify download mechanism by haijian.
** - add ci-command "sys cwmp httsEnabel" control whether we send http packet based on SSL.
**
** Revision 1.18  2006/06/30 09:31:59  maojianli
** modify getWANExternalName()
**
** Revision 1.17  2006/06/30 06:00:22  hqfeng
**  - Save the ACS's Username and Password to SPT.
**
** Revision 1.16  2006/06/29 07:43:45  hqfeng
**  - Recover the getCwmpRebootFlag().
**
** Revision 1.15  2006/06/28 06:10:25  hqfeng
** - Modify doSetCPEUserName() for  reject setting  'admin' for CPE Username.
**
** Revision 1.14  2006/06/28 03:18:21  hqfeng
** - Modify the getCwmpRebootFlag() for Reboot.
**
** Revision 1.13  2006/06/26 11:22:14  xzwang
** modify defautl value of periodenable flag to be 1.
**
** Revision 1.12  2006/06/26 10:14:34  haijian
** Enhance download(not test);
**
** Revision 1.11  2006/06/26 05:26:48  haijian
** Add SSLv3 support.
**
** Revision 1.10  2006/06/25 09:22:51  xzwang
** reset cookie for new session.
**
** Revision 1.9  2006/06/25 09:21:45  xzwang
** add function for configure session complete flag.
**
** Revision 1.8  2006/06/24 13:48:43  xzwang
** no message
**
** Revision 1.7  2006/06/23 10:43:20  xzwang
** reset authentication account for new inform session.
**
** Revision 1.6  2006/06/22 14:11:41  xzwang
** do not create new sesion to send inform brefore old session is closed.
**
** Revision 1.5  2006/06/22 03:06:12  xzwang
** enhance period inform mechanism.
** enhance inform signal mechanism.
** add one ci-command "sys cwmp lantest [0 | 1]" for test in lan environment.
** add one ci-command "sys cwmp disParaFault" for display the parameters with fault.
**
** Revision 1.4  2006/06/22 01:27:11  hqfeng
** - Mark cwmpCommandParse(char * theBufferPtr).
**
** Revision 1.3  2006/06/20 09:22:49  hqfeng
** - Modify for dealing with BOOTSTRAP.
** - Save path and port to SPT.
**
** Revision 1.2  2006/06/19 13:05:16  xzwang
** merge with the newest code of old trunk.
**
** Revision 1.1.1.1  2006/06/19 12:25:55  hqfeng
** - Created 3162L/L2 trunk for TR-069(Merge to new trunk).
**
** Revision 1.43  2006/06/15 11:00:38  xzwang
** modify spsave() to be spsave_old().
**
** Revision 1.42  2006/06/15 10:40:43  xzwang
** modify function name from "cwmpCommandParse_new" to "cwmpParseSoapPaket".
**
** Revision 1.41  2006/06/14 13:51:52  xzwang
**  - make debug message clear.
**  - add inform task.
**
** Revision 1.40  2006/06/14 09:15:05  maojianli
** add cicmd sys cwmp disp
**
** Revision 1.39  2006/06/14 07:44:31  hqfeng
**  - Modify sendInform() for sending BOOTSTRAP.
**
** Revision 1.38  2006/06/14 07:01:26  hqfeng
**  - Modify "sys cwmp url" for set the BootTrap Flag.
**
** Revision 1.37  2006/06/14 06:05:53  xzwang
** - modify WcRetryCount definition.
** - Realize value change inform function.
**
** Revision 1.36  2006/06/14 02:33:23  hqfeng
**  - Add ci-command "setcpeusername" and "setcpepassword".
**
** Revision 1.35  2006/06/13 08:07:41  hqfeng
**  - Add getACSInitiatedflag() and setACSInitiatedflag().
**
** Revision 1.34  2006/06/12 05:54:05  maojianli
** modify some parameter interface function
**
** Revision 1.33  2006/06/09 08:50:44  hqfeng
** - Modify sendInform() for sengding "M Reboot"
**
** Revision 1.32  2006/06/09 06:18:21  hqfeng
** - Modify for sending "M Reboot" in sendInform().
**
** Revision 1.31  2006/06/09 05:51:43  xzwang
** - resolve problem that connection will be closed after receive unathorized 401 packet.
**
** Revision 1.30  2006/06/09 01:35:03  hqfeng
** - Add some get/set function for get/set parameters.
**
** Revision 1.29  2006/06/08 13:07:51  xzwang
** - remove some debug messages.
** - add download RPC function.
** - close connection while receive one empty packet.
** - realize basic authentication for download from file server.
** - realize two active connections.
**
** Revision 1.28  2006/06/01 08:21:54  hqfeng
** - Add CI-Command "sys cwmp correctACSPkt 0|1" to correct the wrong ContentLength.
**
** Revision 1.27  2006/05/29 01:02:40  maojianli
** finish some parameter interface functions
**
** Revision 1.26  2006/05/09 02:53:04  hqfeng
** - Add cwmpAuthenticationInit().
**
** Revision 1.25  2006/04/30 06:11:47  maojianli
** finish some node interface function
**
** Revision 1.24  2006/04/29 02:38:58  maojianli
** finish some node interface function
**
** Revision 1.23  2006/04/21 08:53:15  hqfeng
**  - Set the default value of InformPeriod to 0.
**
** Revision 1.22  2006/04/20 10:15:14  hqfeng
** no message
**
** Revision 1.21  2006/04/17 07:01:46  hqfeng
** - Remove "case MethodListStructType" from doCwmpGetValue().
**
** Revision 1.20  2006/04/17 04:49:09  hqfeng
** - Add the CI-Command:
**    sys cwmp get value/attribute ...
**    sys cwmp set value/attribute ...
**
*/

#include "Global_res.h"
#include "cwmpDef.h"
#include "../../../../lib/libtcapi.h"
#include "upnp_parse.h"
#ifdef CWMP
#ifdef TR143
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#endif
#endif
#include <time.h>
#include <dirent.h>
#include "libcompileoption.h"

/************************************************************************
*                        			MACRO							
************************************************************************
*/
#define MIN_RANDOM_INTERVAL 30		//change 35 to 30 

/************************************************************************
*				E X T E R N A L   R E F E R E N C E S
*************************************************************************
*/
char* cwmpTxBuf = NULL;
/************************************************************************
*						G L O B A L   D A T A
*************************************************************************
*/
//add for Rostelecom 5_28 session retry test, lishuang, 2016.02.02
int connsentflag = 0;

extern int cwmpGetACSFlag;
extern int PrivACSInitiatedflag;

int sockfd = -1;
int upnpDMsockfd = -1;
extern int cwmpParseUrl(char *acsHostName, Unsigned32 *pacsHostAddress, 
                           Unsigned16 *pacsPort, char *acsPath, 
                           char *redirectacsHostName, Unsigned32 *predirectacsHostAddress,
                           Unsigned16 *predirectacsPort, char *redirectacsPath);
extern int cwmpGetACS(char *acsHostName, char *redirectacsHostName, Unsigned16 *pacsPort, Unsigned16 *predirectacsPort);
extern int getDevregFlag();

//modify by brian
spSysParam_t spSysParam;

#ifdef CD_WEBPAGE
extern uint8 getHwVersion(char* ver);
#endif
#if defined(TCSUPPORT_CT)
int getISPIndex(void)
{
	char default_route[16]={0};
	char active[8] = {0};
	char nodeName[48] = {0};
	char serviceType[SERVICE_LIST_LEN] = {0};
	int defRouterIf = -1;
	int i, j;
	int	op_flag = 1;
	/*
		Add your handler code here.
	*/
	memset(default_route,0,sizeof(default_route));

	tcapi_get("WanInfo_Common", "DefRouteIndexv4", default_route);
	if(strlen(default_route) != 0 && strncmp(default_route, "N/A", 3)) {
		defRouterIf =  atoi(default_route);
	}
	else {	/*no pvc active default route, find the first actived Internet interface*/
		for(i = 0; i < PVC_NUM; i++) {
			for(j = 0; j < MAX_SMUX_NUM; j++) {
				memset(active, 0, sizeof(active));
				
				sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
				tcapi_get(nodeName, "Active", active);
				tcapi_get(nodeName, "ServiceList", serviceType);
				
				op_flag = !strcmp(active, "Yes");
				if(TCSUPPORT_CT_SERVICELIST_E8C_VAL)
					op_flag = op_flag && (strstr(serviceType,INTERNET)!=NULL);
				else
					op_flag = op_flag && (!strcmp(serviceType, INTERNET) || !strcmp(serviceType, TR069_INTERNET));

				if(op_flag) {
					defRouterIf = i * MAX_SMUX_NUM +j;
					break; 
				}
			}
		}
	}

	return defRouterIf;	

}
#endif

static atmF5LoopbackDiagnostis_t atmF5Loopback_t[MAX_WAN_NODE];
/*the current Wannode which Diagnostics will implement on*/
static uint8 F5LoopbackDiagnosticsWannodeindex;
/*DiagnosingFlag represent current test state
**      0   no F5LoopbackDiagnostics test
**      1   F5LoopbackDiagnostics test is in process*/
static uint8 F5LoopbackDiagnosingFlag = 0;
/*DiagnosingFlag represent whether need terminate current test(if exist) and restart a new diagnosis
**      0   donn't need restart a new diagnosis
**      1   need terminate current test(if exist) and restart a new diagnosis*/
static uint8 F5LoopbackDiagnosticsResetFlag = 0;
static int web_prdenable_flag = 0;//for web config inform prd
static int web_prdPeriod_flag = 0;

extern boolean checkPeriodInformTimeFormat(char *sTime);

#ifdef IP_MAC_FILTER_ENHANCEMENT
extern int bindIpFilterRuleSetwithIface(void *value, uint8 setIndex);
#endif

extern uint8 VersionStyle;
cwmp_rpc_t       rpc;
uint8 authenticationFlag = 0;
//uint8 dbg_cwmp = DBG_CWMP_L0;
uint8 dbg_cwmp = DBG_CWMP_L0;
#ifndef CODE_SHRINK  //qingpu.20080110
uint8 serial_num_flag=0 ;
#endif

#if CWMP_REDUCE_CODE
char* InformEvent[11] = {
"0 BOOTSTRAP",
"1 BOOT",
"2 PERIODIC",
"3 SCHEDULED",
"4 VALUE CHAGE",
"5 KICKED",
"6 CONNECTION REQUEST",
"7 TRANSFER COMPLETE",
"8 DIAGNOSTICS COMPLETE",
"M %s",
"X %s %s"
};
#endif

#ifdef THREAD_REDUCE
uint8 ValueChangeFlag = 0;
#endif

uint8 DelaySetBootTrapFlags = 0;
uint8 DelayResetMBootFlags = 0;

uint8 RpcMethod = 0;

volatile uint32 cwmpValueChangeTaskId = 0;
volatile uint32 InformTaskId = 0;

volatile uint32 cwmpReInitHostTableTaskId = 0; //qingpu.20071129

#if defined (TR069_CA_EXTENTION)
char SPTCustomerCA[CUSTOMER_CA_LENGTH_V2];  /*CUSTOMER_CA_LENGTH_V2==1640*/
#endif

uint8 gLanHostNum = 0;	
static uint32 lanHostTimerID = 0;


/************************************************************************
*						P R I V A T E   D A T A
*************************************************************************
*/
static boolean cwmpSwitch = 1;
static boolean cwmpInitFlag = 0;
static boolean initNotifyListFlag = 0;

static boolean cwmpRepeatFlag = 0; 
static int customTimerID = 0;
static int customStopTimerID = 0;
static uint8 repeatTimes = 2;
static uint8 repeatPeriodicVal = 25;

static int startTimerID = 0;
//static int randomInformMinInterval = 40;

int informTimerID = 0;
static uint32 bootTimerID = 0;
//add by brian
static   uint32 informRetryID = 0;
//added by xyzhu_nj_20091209
static uint32 wifiMacTabTimerID = 0;
static volatile uint8 BootFlag = 1;
static volatile uint8 PeriodicFlag = 0;
static volatile uint8 ActiveNotifyFlag = 0;

char cwmpURL[CwmpURLMaxLen] = "http://122.193.99.166:80/comserver/node1/tr069";//URL

#if CWMP_REDUCE_CODE
char  fEventMessageBuffer[CwmpWebClientBufferSize]="<?xml version='1.0' encoding='UTF-8'?>\r\n<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\r\n<SOAP-ENV:Body>\r\n<ns1:getWelcomeValue xmlns:ns1=\"urn:hello.hellosoap\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n<name xsi:type=\"xsd:string\">Penny</name>\r\n</ns1:getWelcomeValue>\r\n</SOAP-ENV:Body>\r\n</SOAP-ENV:Envelope>\r\n";
#endif
char  fResponeBuffer[CwmpWebClientBufferSize];
static boolean CorrectACSPktLengthEnable = 0;//added for correct ACS packet(SetparameterValues,ContentLenth is wrong. ),by hqfeng,2006.06.01.



static char HardwareVer[12]="3.5.0.2";
char  ConRequestURL[256]="http://0.0.0.0:80/";
static char SerialNum[129]="998877665544";
char wanindex = 0;  //qingpu,20070920
char wanipaddr[32]="0.0.0.0";
static char wanMacAddr[40] = "a4:29:40:96:56:26";
static char updatetime[32]="0001-01-01T00:00:00";
static char wanname[128]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.ExternalIPAddress";
static char wanMacName[128]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.MACAddress";

#ifdef BHARTI
static char pppoeUsername[80];
static char wanPPPoEUsername[128]="InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username";
#endif

static volatile int8 ACSInitiatedflag = 0;//added for ACS Initiated,by hqfeng,2006.06.13
static uint8 cwmpRetryNum = 0;
static uint8 SendHostPortNumFlag = 0; //chuguang 20090512 add for ChinaUnicom ASB ACS test
#ifndef CODE_SHRINK  //qingpu.20080110
static uint8 cwmpWaitingReceiveFlag = 0;
static uint8 cwmpLanTestFlag = 0;
static uint8 sessionCompleteFlag = 0;

static uint8 httpsFlag = 0;
static uint8 sessionClearFlag = 0;
static uint8 initEmptyPktFlag = 0;
#endif

static int UnLockRealmsUserFlag = 0;
static uint8 sysBootFlag = 1;

//For Retry Policy, hqfeng, 2006.07.10
static int cwmpSessionRetryTimer = 0;//Caculate the Retry wait interval by httpd timer(HTTPDTM).
static volatile int cwmpSessionRetryWaitInterval = 0;//store the wait interval for Retry Policy.
static volatile int cwmpSessionRetryFlag = 0;
volatile int cwmpSessionRetryCount = 0;//For store the Post reboot session retry count.
static volatile uint32 cwmpSessionRetryTaskId = 0;
static volatile uint32 cwmpRetryClkId = 0;
static int InformCompleteFlag = 1;

//For HoldRequest header, hqfeng, 2006.08.14
static boolean gHoldRequestFlag = 0;

char* customerCA = NULL;
static int8 mw_downStart = 0;

uint8 inform_task_start = 0;
char DownloadURL[256];
static int8 cwmp_download = 0;
static int8 cwmp_firmwareupgrade = 0;
char gDownLoadFileName[256] = "";//added by hqfeng,2006.11.03
static int8 cwmp_romfile_upLoad = 0;

static uint8 cwmpConnectionIndex;
#ifdef CWMP_CFG
static uint8 EnableConfigure = 0;
#endif

#ifdef BHARTI
static boolean noValueTypeFlag = 1;
#else
static boolean noValueTypeFlag = 0;
#endif
static boolean faultDetailFlag = 1;

static boolean noArrayTypeFlag = 0;

//static uint8 inform_task_start = 0;
//static uint8 signal_valueChg_flag = 0;
static StringStructPtr valueChangeNameList = NULL;

static uint8 motiveRSTFlag = 0;	//0 means off, 1 means on

#ifdef SSL
static uint8 cwmp_ssl_check_mem = 0;
#endif
pthread_mutex_t value_change_lock;

DeviceAlarmNodeInfoPtr gAlarmNodeList = NULL;
DeviceMonitorNodeInfoPtr gMonitorNodeList = NULL;
static boolean gDeviceAlarmFlag = 0;
static boolean 	gDeviceClearAlarmFlag = 0;
static boolean gDeviceMonitorFlag = 0;
uint8 gAlarmEntryNum = 0;
uint8 gMonitorEntryNum = 0;
/*add by robbie.jin*/
uint8 g_tr069TcapiMode = 0;
pthread_mutex_t asyn_tcapi_lock;
int g_AlarmEnable = 0;
int g_AlarmEntryNum = 0;
int g_MonitorEnable = 0;
int g_MonitorEntryNum = 0;
/*end add by robbie.jin*/

struct timeval gAlarmLast;
struct timeval gMonitorLast;	

pthread_t p_AlarmMonitor = 0;

MonitorCollectorNodeInfoPtr gMonitorCollectorNodeList = NULL;
static boolean gMonitorCollectorFlag = 0;
uint8 gMonitorCollectorEntryNum = 0;
struct timeval gMonitorCollectorLast;	
int gMinSleepSec = 10;

static boolean gUPnPDMDownloadFlag = 0;

/************************************************************************
*				P R I V A T E    F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
int cwmpGetInformTimerID();
#if CWMP_REDUCE_CODE
void  cwmpSendHttpMessage(void); 
#endif
#if CWMP_REDUCE_CODE
void  cwmpInitWebClientRequest(rpDataPtr theTaskDataPtr,wcRequestPtr theWebRequestPtr); 
#endif

void stopRepeatInform(void);

boolean getRepeatFlag(void);
void setRepeatFlag(boolean value);
uint8 getRepeatTimesValue(void);
void setRepeatTimesValue(uint8 value);
uint8 getRepeatPeriodicVal(void);
void setRepeatPeriodicVal(uint8 value);

//modify by brian
void processSessionRetry(void);
//add by brian
static int cwmpCalculateRetryInterval(int retryCount);

#if CWMP_REDUCE_CODE
static void sendTransferCompleteRPC(void);
#endif
void processTheHostAddressExt(Unsigned32* theHostAddress, Unsigned16* theHostPort, char* theHostPath, int8 saveType);
void processTheHostAddress(Unsigned32* theHostAddress, int8 saveType);

char * getCwmpUrl(void);
int  setCwmpUrl(char *value);
char * getCwmpPath(void);
int  getCwmpPort(void);
char * getCwmpManufacture(void);
char * getCwmpOUI(void);
char *getCwmpHardwareVersion(void);
char *getCwmpSoftwareVersion(void);

/*xyzhu_nj 20090819
	zte requirement:cwmp oui get first six characters of mac address
*/
#ifdef DARE_ZTE_NJ
int setMacToOui(void);
int setMacToOuiFlag(uint8 flag);
int getMacToOuiFlag(void);
#endif
char * getCwmpProductClass(void);
char * getCwmpSerialNum(void);
char * getHardwareVer(void);
char * getUpDateTime(void);
char * getDefaultWANExternalIPAddr(void);
char * getConRequestURL(void);
char * getCwmpUserName(void);
char * getCwmpPassword(void);
void cwmpAuthenticationInit(void);
boolean  getCorrectACSPktLengthEnable(void);

int8 getACSInitiatedflag(void);
void setACSInitiatedflag(int8 value);

int8  getBootTrapFlag(void);
void  setBootTrapFlag(int8 value);
void cwmpBuildCwmpUrl(void);
uint8 getCwmpCA(void);
void setCwmpCA(uint8 val);

uint8 getRegITMSFlag(void);
uint8 getBootFlag(void);
void setBootFlag( uint8 value);
uint8 getActiveNotifyFlag(void);
void setActiveNotifyFlag(uint8 value);
uint8 getPeriodicFlag(void);
void setPeriodicFlag(uint8 value);

uint8 getAccountChangeFlag(void);
extern void setAccountChangeFlag(uint8 value);

boolean getNoValueTypeFlag(void);
void setNoValueTypeFlag(boolean value);

#ifndef CODE_SHRINK  //qingpu.20080110
char * getCwmpDownloadUrl(void);
void setCwmpDownloadUrl(char * url);
#endif

//static int16 doRandInform(int argc, char *argv[], void *p);
//static int16 doRandInformMinInterval(int argc, char *argv[], void *p);
//boolean  getRandomInformFlag(void);
//void  setRandomInformFlag(boolean value);

uint8 SetDownloadStart(uint8 flag);
uint8 GetDownloadStart(void);
uint8 SetUploadStart(uint8 flag);
uint8 GetUploadStart(void);

int startPeriodicInform(void);
void lanHostStatsCheck(void);

int8 cwmpBuildAlarmParameters(void);
int8 cwmpBuildMonitorParameters(void);

/************************************************************************
				E X T E R N A L    F U N C T I O N   D E C L A R A T I O N S
*************************************************************************/

extern int/*8*/ cwmpDownloadResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);

extern int8 StartSSLConnection(void);
//extern void SetDownloadUrl(int n);
extern char * fetchConRequestURL_detail(void);
extern mbuf_t *FwUpMbufDeQue(void);

#ifdef CSE
extern char *retSerialNumPtr(void);
#endif

#ifdef TR069_CA_EXTENTION
char* getSPTCustomerCA(void);
int setSPTCustomerCA(char *ptr);
#endif

int cwmpAlarmMonitorTaskStart(void);
void checkAndDoAlarmMonitorTask(void);
void cwmpDeviceAlarmRoutine(void);
void cwmpDeviceMonitorRoutine(void);

int cwmpMonitorCollectorTaskStart(void);
void cwmpMonitorCollectorRoutine(void);

#define TOTAL_REMOTE_DEVICE_NODE 	8
#define TOTAL_REMOTE_ENTRY_NODE 	8

uint32 PVCMappingIndex[TOTAL_REMOTE_DEVICE_NODE+1];
uint32 PVCMappingTable[TOTAL_REMOTE_DEVICE_NODE][TOTAL_REMOTE_ENTRY_NODE+1];

#define PVC_INDEX 		"index"
#define ENTRY_INDEX		"index"

uint8 InitWanNodeMapping();
uint8 FindWanNodeMapping(uint32 *PVCValue, uint32 *EntryValue);

void DumpMappingInfo(uint8 flag)
{
	int i, j;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		tcdbg_printf("\r\n====================================================================================");
		tcdbg_printf("\r\nPVCMappingITable =  | ");
		
		if(flag == 1){
			for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE+1; i++){
				tcdbg_printf("%d |  ", PVCMappingIndex[i]);
			}
		}
		if(flag == 2){
			for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++){
				tcdbg_printf("\r\nPVC%d==> | ", i);
				for(j = 0; j < TOTAL_REMOTE_ENTRY_NODE+1; j++){
					tcdbg_printf("%d |  ", PVCMappingTable[i][j]);
				}
			}
		}
		tcdbg_printf("\r\n====================================================================================\r\n");
	}
}

uint32 AddIndexValue(uint32 MappingTable[]){
	uint8 i, j;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		for(i = 1; i < TOTAL_REMOTE_DEVICE_NODE+1; i++){
			for(j = 0; j < TOTAL_REMOTE_DEVICE_NODE; j++){
				if(MappingTable[j] == i){
					break;
				}
			}
			
			if(j == TOTAL_REMOTE_DEVICE_NODE){
				return i;
			}
		}
	}
}

uint8 CheckIndexValue(uint32 MappingTable[], uint8 ignore_index, uint32 index)
{
	uint8 i;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++){
			if(i != ignore_index && MappingTable[i] == index){
				return 1;
			}
		}
	}
	return 0;
}

uint8 UpdatePVCMapping(uint32 PVC_index)
{
	char nodeName[20] = {'\0'};
	char PVCIndexTmp[10] = {'\0'};
	char VLANID[10] = {'\0'};
	uint32 PVCValue;
	uint8 i, j, ret=0;
		
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		sprintf(nodeName, "Wan_PVC%d", PVC_index);
		ret = tcapi_get(nodeName, "VLANID", VLANID);
		if(!ret){//this PVC is exist.
			ret = tcapi_get(nodeName, PVC_INDEX, PVCIndexTmp);
			if(!ret && (PVCValue = atoi(PVCIndexTmp)) && (!CheckIndexValue(PVCMappingIndex, PVC_index, PVCValue))){//have PVC Index information
				if(PVCMappingIndex[PVC_index] == 0){
					PVCMappingIndex[PVC_index] = PVCValue;
					PVCMappingIndex[8]++;
				}
			}
			else{//Not have PVC Index information
				PVCValue = AddIndexValue(PVCMappingIndex);
				PVCMappingIndex[PVC_index] = PVCValue;
				PVCMappingIndex[8]++;
				
				memset(PVCIndexTmp, 0, sizeof(PVCIndexTmp));
				sprintf(PVCIndexTmp, "%d", PVCValue);
				tcapi_set(nodeName, PVC_INDEX, PVCIndexTmp);
			}			
		}
		else{//this PVC is not exist, or detele
			if(PVCMappingIndex[PVC_index] != 0){
				PVCMappingIndex[PVC_index] = 0;
				PVCMappingIndex[8]--;
				for(j = 0; j < TOTAL_REMOTE_ENTRY_NODE+1; j++){
					PVCMappingTable[PVC_index][j] = 0;
				}
			}
		}
	}

	return 1;
}

uint8 UpdateEntryMapping(uint32 PVC_index, uint32 Entry_index)
{
	char nodeName[20] = {'\0'};
	char EntryIndexTmp[10] = {'\0'};
	char Active[10] = {'\0'};
	uint32 EntryValue;
	uint8 i, j, ret=0;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		sprintf(nodeName, "Wan_PVC%d_Entry%d", PVC_index, Entry_index);
		ret = tcapi_get(nodeName, "Active", Active);
		if(!ret){//this Entry is exist.
			ret = tcapi_get(nodeName, ENTRY_INDEX, EntryIndexTmp);
			if(!ret && (EntryValue = atoi(EntryIndexTmp)) && (!CheckIndexValue(PVCMappingTable[PVC_index], Entry_index, EntryValue))){//have Entry Index information
				if(PVCMappingTable[PVC_index][Entry_index] != EntryValue){
					PVCMappingTable[PVC_index][Entry_index] = EntryValue; 
					PVCMappingTable[PVC_index][8]++; 
				}
			}
			else{//Not have PVC Index information
				EntryValue = AddIndexValue(PVCMappingTable[PVC_index]);
				PVCMappingTable[PVC_index][Entry_index] = EntryValue;
				PVCMappingTable[PVC_index][8]++;
				
				memset(EntryIndexTmp, 0, sizeof(EntryIndexTmp));
				sprintf(EntryIndexTmp, "%d", EntryValue);
				tcapi_set(nodeName, ENTRY_INDEX, EntryIndexTmp);
			}			
		}
		else{//this Entry is not exist, or detele
			if(PVCMappingTable[PVC_index][Entry_index] != 0){
				PVCMappingTable[PVC_index][Entry_index] = 0;
				PVCMappingTable[PVC_index][8]--;
				if(PVCMappingTable[PVC_index][8] == 0)
					PVCMappingIndex[8]--;
			}
		}
	}

	return 1;
}

uint8 InitWanNodeMapping()
{
	uint8 i, j;
	
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		memset(&PVCMappingIndex, 0, sizeof(PVCMappingIndex));
		
		for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++){
			memset(&PVCMappingTable[i], 0, sizeof(PVCMappingTable[i]));
		}
		
		for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++){
			UpdatePVCMapping(i);
			for(j = 0; j < TOTAL_REMOTE_ENTRY_NODE; j++){
				UpdateEntryMapping(i, j);
			}
		}
	}
	
	return 1;
}

void GetPVCMapping(uint32 *PVCValue)
{
	uint32 PVCIndex = 0;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		PVCIndex = PVCMappingIndex[*PVCValue];
		*PVCValue= PVCIndex;
	}
}

void GetWanNodeMapping(uint32 *PVCValue, uint32 *EntryValue)
{
	uint32 PVCIndex, EntryIndex;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		PVCIndex = PVCMappingIndex[*PVCValue];
		EntryIndex = PVCMappingTable[*PVCValue][*EntryValue];
		*EntryValue = EntryIndex;
		*PVCValue= PVCIndex;
	}
}

uint8 FindPVCMapping(uint32 *PVCValue)
{
	int i;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){	
		for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++){
			if(*PVCValue == PVCMappingIndex[i]){
				*PVCValue = i;
				return 0;
			}
		}
		*PVCValue = TOTAL_REMOTE_DEVICE_NODE;
	}
	return -1;
}

uint8 FindWanNodeMapping(uint32 *PVCValue, uint32 *EntryValue)
{
	int i, j;

	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++){
			if(*PVCValue == PVCMappingIndex[i]){
				for(j = 0; j < TOTAL_REMOTE_ENTRY_NODE; j++){
					if(*EntryValue == PVCMappingTable[i][j]){
						*EntryValue = j;
						*PVCValue = i;
						return 0;
					}
				}
			}
		}
		*EntryValue = TOTAL_REMOTE_ENTRY_NODE;
		*PVCValue = TOTAL_REMOTE_DEVICE_NODE;
	}
	return -1;
}

uint8 AddMappingTable(uint32 *PVCIndex, uint32 *EntryIndex)
{
	return -1;
	
}
uint8 DelWanNodeMapping(uint32 *PVCValue, uint32 *EntryValue)
{
	return -1;
}

/********************************** FUNCTION DEFINITION ***************************************/
void setCwmpInformTaskStart(int value)
{
	pthread_mutex_lock(&value_change_lock);
	inform_task_start = 1;
	pthread_mutex_unlock(&value_change_lock);
}

void cwmpInit(void){
	uint32	second = 1000;	/* ticks for 1 second */
	int	op_flag = 1;
	uint32     period = getInformPeriod() *second;
#if defined(CSE_TANGO) || defined(SZTELE_WORKSSYS) ||defined(MAX_TR69_PARAM)//add F5LoopbackDiagnostics for tr069.pingzhang_071109
	int nodeIndex;
#endif
#ifdef LAYER2BRIDGING	
#ifdef VLAN_8021Q
	spVlanCtl_t *vlanCtl = spSysParam.vlanCtl;
#endif
#endif
	uint8       j = 0;	
	
       
	if(!getCwmpInitFlag()){
 		if(dbg_cwmp >= DBG_CWMP_L2 ){
    	      		tc_printf("\r\n[cwmpInit] Enter cwmp init, and periodic inform interval: %ld ,periodic inform enable :%ld\n", getInformPeriod(),getInformPeriodEnable());
        	}

		
		if(TCSUPPORT_CT_UPNP_DM_VAL){
			upnpDMsockfd = create_socket_client_Open();
			if(upnpDMsockfd < 0 )
				tcdbg_printf("===>create_socket_client error!!!upnpDMsockfd=%d\n",upnpDMsockfd);
		}

		cwmpRpcInit(&rpc);
		//cwmpConfigInit();
		//added by xyzhu_nj_20091228
		setDefaultPeriodicInformTime();
		
	#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
		if(getPreAllocTxBufFlag()){
			if(cwmpInitTxBuf() == -1)
				return;
		}
	#endif
		
	//added by brian 20090220
	#ifdef LAYER2BRIDGING	
	#ifdef VLAN_8021Q
		//do when reset default 
		if(!(vlanCtl->flags & VLAN_TRANSFER_CWMP))
		{
			tc_printf("\r\nInit layer2bridging parameters ...");
			if(InitDefaultL2BParameter() < 0)
			{
				if(dbg_cwmp >= DBG_CWMP_L2 ){
    	      		tc_printf("\r\nError when init default l2b parameters!");
        		}
			}
			
		}
	#endif
	#endif
	/*init attribute struct*/
	InitAttrs();
	
	if(TCSUPPORT_CT_WANNINDEX_INCREASE_VAL){
		initUsedWanIndex();
	}
	if(rpc.IGDDeviceRootPtr == NULL)
	      cwmpInitParameterTree();
		tc_printf("Init Parameters Tree ok.\n");

		if(rpc.fileTransferStruct == NULL){
			if( cwmpInitFileTransferStruct() != 0){
				tc_printf("\r\n Warning: cwmpInitFileTransferStruct fail. ");
			}
		}
#ifdef TR143
		cwmpInitDiagnosticsStruct();
#endif
		//modify by brian
		cwmpInformTaskStart();

#ifndef THREAD_REDUCE
		//modify by brian
		cwmpValueChangeTaskStart();
#endif

#ifdef WLAN_RT2561_SUPPORT	
		cwmpWLANTaskStart();//added by hqfeng,2006.09.05
#endif	
		//mark by brian
		//cwmpReInitHostTableTaskStart();  //qingpu.20071129
        /*Add by robbie.jin 20150429*/
        g_tr069TcapiMode = getTr069TcapiMode();
        tcdbg_printf("tr069 tcapi mode is %u\n",g_tr069TcapiMode);
        cwmpAsynTcapiTaskStart();

		if(TCSUPPORT_CT_ALARMMONITOR_VAL)
        {      
            g_AlarmEnable = checkAlarmEnable();
            g_AlarmEntryNum = checkAlarmEntryNum();
            g_MonitorEnable = checkMonitorEnable();
            g_MonitorEntryNum = checkMonitorEntryNum();
			cwmpAlarmMonitorTaskStart();
        }

		if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL)
        {      
            g_MonitorCollectorEnable = checkMonitorCollectorEnable();
			cwmpMonitorCollectorTaskStart();
        }

		if ( TCSUPPORT_CT_PMINFORM_VAL )
        {
            g_PMInformEnable = checkPMInformEnable();
			cwmpPMInformTaskStart();
        }

		if ( TCSUPPORT_CT_JOYME_VAL )
			cwmpPhoneRPCTaskStart();

#if defined(CSE_TANGO) || defined(SZTELE_WORKSSYS) ||defined(MAX_TR69_PARAM)//add F5LoopbackDiagnostics for tr069.pingzhang_071109
		for(nodeIndex=0; nodeIndex<MAX_WAN_NODE; nodeIndex++)
		{
			atmF5Loopback_t[nodeIndex].DiagnosticsState = eDiagnosticsStateNone;
			atmF5Loopback_t[nodeIndex].NumberOfRepetitions = 0;
			atmF5Loopback_t[nodeIndex].Timeout = 0;
			atmF5Loopback_t[nodeIndex].SuccessCount = 0;
			atmF5Loopback_t[nodeIndex].FailureCount = 0;
			atmF5Loopback_t[nodeIndex].AverageResponseTime = 0;
			atmF5Loopback_t[nodeIndex].MinimumResponseTime = 0;
			atmF5Loopback_t[nodeIndex].MaximumResponseTime = 0;
		}
#endif
		if(TCSUPPORT_CT_MIDWARE_VAL){
			setBootFlag(0);
			op_flag=(getTr069RestartItself() && !bootTimerID);
		}
		else{
			op_flag=(!bootTimerID);
		}
   		if(op_flag){
			bootTimerID = timerPeriodicStartS(1000,  (TimerCallback)cwmpBootInform, NULL);
		}

		if(TCSUPPORT_WLAN_VAL){
			//added by xyzhu_nj_20091209
			if(!wifiMacTabTimerID){
				wifiMacTabTimerID = timerPeriodicStartS(5000, (TimerCallback)wifiMacTabChanged, NULL);
			}
		}
		if(!lanHostTimerID) {
			lanHostTimerID = timerPeriodicStartS(5000, (TimerCallback)lanHostStatsCheck, NULL);
		}
		/*	Remove it by dennyxiang for support the unicom test	*/
		if(!TCSUPPORT_RANDOM_INFORM_VAL){
			/* ToT bug #2705
			 * customer ACS server is unstable, sometimes CPE must repeat send inform in small interval for 
			 * several times to connect ACS when CPE boot-strap
			 * This feature is close by default, customer can open it with CI-Command
			 * Liutao@20090702
			 */
			if(cwmpRepeatFlag && repeatTimes && repeatPeriodicVal){
				if(!customTimerID && !customStopTimerID){
				tc_printf("\r\ncwmpInit");
					customTimerID = AddPeriodicInformTimer( repeatPeriodicVal * second,  (TimerCallback)cwmpPeriodInform, NULL );
					customStopTimerID = timerStartS((repeatPeriodicVal * repeatTimes)*second, (TimerCallback)stopRepeatInform, NULL );
				}
			}
			else{//sam_20081225 merge from huawei
					startPeriodicInform();
			}
		}	
		informRetryID = timerPeriodicStartS( 1000,  (TimerCallback)processSessionRetry, NULL );
		setCwmpInitFlag(1);
	}
}

int checkAlarmEnable(void) {
	char enableValue[8]= {0};

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){	
		/* Check Alarm is enable or not */
		tcapi_get(ALARM_COMMON, "Enable", enableValue);
		if( !strncmp(enableValue, "Yes", 3) )
		      return 0;
	}
	return -1;
}

int checkAlarmEntryNum(void) {
	char tmpBuf[16] = {0};
	int maxNum = 0;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		tcapi_get(ALARM_COMMON, "MaxInstanceNum", tmpBuf);
		if(strlen(tmpBuf) != 0)
			maxNum = atoi(tmpBuf);
	}
	return maxNum;	
}

int checkMonitorEnable(void) {
	char enableValue[8]= {0};
	
	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		/* Check Alarm is enable or not */
		tcapi_get(MONITOR_COMMON, "Enable", enableValue);
		if( !strncmp(enableValue, "Yes", 3) )
			return 0;
	}
	return -1;
}

int checkMonitorEntryNum(void) {
	char tmpBuf[16] = {0};
	int maxNum = 0;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		tcapi_get(MONITOR_COMMON, "MaxInstanceNum", tmpBuf);
		if(strlen(tmpBuf) != 0)
			maxNum = atoi(tmpBuf);
	}

	return maxNum;	
}

int checkAndInitAlarm(void) {
	char tmpBuf[16] = {0};
	int maxEntryNum = 0;
	int i;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		/* Check Alarm is enable or not */
#if 0	
		if(checkAlarmEnable() < 0) {
			tcdbg_printf("\n==>cwmpDeviceAlarmRoutine Error, checkAndInitAlarm, Alarm not enable\n");
			return -1;
		}
#endif

		/* Check the Max Alarm Entry number */
		maxEntryNum = g_AlarmEntryNum;
		if(maxEntryNum == 0) {
			tc_printf("\n==>cwmpDeviceAlarmRoutine Error, maxEntryNum is 0\n");
			return -1;
		}

		/* Allocate memory for struct of each alarm entry */	
		if((gAlarmNodeList = (DeviceAlarmNodeInfoPtr)os_malloc(maxEntryNum * sizeof(DeviceAlarmNodeInfo))) == NULL){
			tc_printf("\n==>cwmpDeviceAlarmRoutine Error, malloc request Alarm struct fails!\r\n");
			return -1;
		}

		/* Init struct */
		for(i=0; i<maxEntryNum; i++) {
			memset(&gAlarmNodeList[i], 0, sizeof(DeviceAlarmNodeInfo));
		}
	}
	return 0;
}

int checkAndInitMonitor(void) {
	char tmpBuf[16] = {0};
	int maxEntryNum = 0;
	int i;
	
	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
#if 0	
		/* Check Monitor is enable or not */
		if(checkMonitorEnable() < 0) {
			tcdbg_printf("\n==>cwmpDeviceAlarmRoutine Error, checkAndInitMonitor, Alarm not enable\n");
			return -1;
		}
#endif
		/* Check the MAx Monitor Entry number */
		maxEntryNum = g_MonitorEntryNum;
		if(maxEntryNum == 0) {
			tc_printf("\n==>cwmpDeviceMonitorRoutine Error, maxEntryNum is 0\n");
			return -1;
		}
		/* Allocate memory for struct of each monitor entry */	
		if((gMonitorNodeList = (DeviceMonitorNodeInfoPtr)os_malloc(maxEntryNum * sizeof(DeviceMonitorNodeInfo))) == NULL){
			tc_printf("\n==>cwmpDeviceMonitorRoutine Error, malloc request Monitor struct fails!\r\n");
			return -1;
		}
		
		/* Init struct */
		for(i=0; i<maxEntryNum; i++) {
			memset(&gMonitorNodeList[i], 0, sizeof(DeviceMonitorNodeInfo));
		}
	}
	return 0;
}

void cwmpDeviceAlarmRoutine(void) {
	char tmpBuf[16] = {0};
	char paraListValue[MAX_ATTRIBUTE_LEN] = {0};
	char nodeName[32] = {0};
	int i, ret = 0;
	int maxEntryNum = 0;
	int limitMax = 0, limitMin = 0;
	int paraValue = 0;
	int tmpValue = 0;
	int periodTime = 0;
	int valueMode = 0;
	DeviceNodePtr currentDeviceNode = NULL;
	char* curParaValue = NULL;
	ParameterType paraType;	
	struct timeval now;
	struct timeval last;
	DeviceNodePtr rootptr = NULL;
	int dval = 0;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		/* Check Alarm is enable or not */
		if(checkAlarmEnable() < 0) {
			tc_printf("\n==>cwmpDeviceAlarmRoutine Error, Alarm not enable\n");
			return;
		}
		
		if( gAlarmNodeList == NULL ) {
			tc_printf("\n==>cwmpDeviceAlarmRoutine Error, Alarm node not allocate\n");
			return;
		}
		
		maxEntryNum = checkAlarmEntryNum();

		pthread_mutex_lock(&value_change_lock);
		if(rpc.IGDDeviceRootPtr == NULL){
			pthread_mutex_unlock(&value_change_lock);
			//here, we need to check rpc, and free something, do it later
			tc_printf("\r\n Error!,  cwmp parameter tree is NULL!! \r\n ");
			return ;
		}else{
			inform_task_start =1; //hjzhang: before handling the RPC, set inform flag to 1.
			pthread_mutex_unlock(&value_change_lock);
		}

		for(i=0; i<maxEntryNum; i++) {
			memset(nodeName, 0, sizeof(nodeName));

			/* Check entry is used or not */
			sprintf(nodeName, ALARM_ENTRY, i);
			ret = tcapi_get(nodeName, "ParaList", paraListValue);
			if(ret < 0 || strlen(paraListValue) == 0) {
				gAlarmNodeList[i].usedFlag = 0;
				continue;
			}

			gAlarmNodeList[i].usedFlag = 1;

			/* Get limit max*/
			memset(tmpBuf, 0, sizeof(tmpBuf));
			tcapi_get(nodeName, "Limit-Max", tmpBuf);
			if(strlen(tmpBuf) != 0)
				limitMax = atoi(tmpBuf);

			/* Get limit min*/
			memset(tmpBuf, 0, sizeof(tmpBuf));
			tcapi_get(nodeName, "Limit-Min", tmpBuf);
			if(strlen(tmpBuf) != 0)
				limitMin = atoi(tmpBuf);

			/* Get timeList*/
			memset(tmpBuf, 0, sizeof(tmpBuf));
			tcapi_get(nodeName, "TimeList", tmpBuf);
			if(strlen(tmpBuf) != 0)
				periodTime = atoi(tmpBuf) * 60;

			/* Get timeList*/
			memset(tmpBuf, 0, sizeof(tmpBuf));
			tcapi_get(nodeName, "Mode", tmpBuf);
			if(strlen(tmpBuf) != 0)
				valueMode = atoi(tmpBuf);

			/* Search node and get value */
			currentDeviceNode = NULL;
			cwmpSearchDeviceNodeByName(paraListValue, rpc.IGDDeviceRootPtr, &currentDeviceNode);
			if(currentDeviceNode == NULL)
				continue;
			
			if(currentDeviceNode->getValueInfo){
				currentDeviceNode->getValueInfo(&curParaValue, &paraType, currentDeviceNode);
			}

			if(curParaValue){
				if(gAlarmNodeList[i].count == 0)/* Record the first time value */
					memcpy(gAlarmNodeList[i].oriValue, curParaValue, 16);

				gAlarmNodeList[i].count++;
				
				/* Calculate the value */
				if(valueMode == ACCUMULATE_MODE) {
					paraValue = atoi(curParaValue);
				}
				else if(valueMode == AVERAGE_MODE) {
					if(gAlarmNodeList[i].count == 1)
						paraValue = atoi(curParaValue);
					else
						paraValue = (atoi(curParaValue) + atoi(gAlarmNodeList[i].oriValue)) / gAlarmNodeList[i].count;
				}
				else if(valueMode == BURST_MODE) {
					paraValue = abs(atoi(gAlarmNodeList[i].paramValue) - atoi(curParaValue)); 
				}

				gettimeofday(&now, NULL);
				/*Check CPE time changed or not*/
				dval = now.tv_sec - gAlarmLast.tv_sec;
				if(abs(dval) >= TIMECHANGERANGE && gAlarmNodeList[i].startTime.tv_sec != 0)	{
					/*Update last time*/				
					gAlarmNodeList[i].startTime.tv_sec += dval;
				}
				gAlarmLast = now;

				if((paraValue > limitMax) || (paraValue < limitMin)) {
	//Exceed the value limit
					if(gAlarmNodeList[i].typeFlag == ALARM_NONE_FLAG) {
						memcpy(gAlarmNodeList[i].paramValue, curParaValue, 16);
						memcpy(&gAlarmNodeList[i].type, &paraType, sizeof(paraType));
						gAlarmNodeList[i].startTime.tv_sec = now.tv_sec;
						gAlarmNodeList[i].typeFlag = ALARM_FLAG;
						
						setDeviceAlarmFlag(1);
						sem_post(&SEM_IMFORM);
						//tcdbg_printf("\n==>CWMP, Exceed, set DeviceAlarm flag, and wait Inform\n");
					}
					else if(gAlarmNodeList[i].typeFlag == ALARM_FLAG) {
							if((now.tv_sec - gAlarmNodeList[i].startTime.tv_sec) >= periodTime) {
								memcpy(gAlarmNodeList[i].paramValue, curParaValue, 16);
								memcpy(&gAlarmNodeList[i].type, &paraType, sizeof(paraType));
								gAlarmNodeList[i].startTime.tv_sec = now.tv_sec;
								
								setDeviceAlarmFlag(1);
								sem_post(&SEM_IMFORM);	
								//tcdbg_printf("\n==>CWMP, Exceed, flag is already ALARM and reach period time,  wait Inform\n");
							}
							else {
								memcpy(gAlarmNodeList[i].paramValue, curParaValue, 16);
								memcpy(&gAlarmNodeList[i].type, &paraType, sizeof(paraType));
								//tcdbg_printf("\n==>CWMP, Exceed, flag is already ALARM, but not reach period time\n");
							}						
					}
				}
				else if((paraValue >= limitMin) && (paraValue <= limitMax)) {
						if(gAlarmNodeList[i].typeFlag == ALARM_FLAG) {
							if(now.tv_sec - gAlarmNodeList[i].startTime.tv_sec <= periodTime) {
								memcpy(gAlarmNodeList[i].paramValue, curParaValue, 16);
								memcpy(&gAlarmNodeList[i].type, &paraType, sizeof(paraType));
								gAlarmNodeList[i].startTime.tv_sec = now.tv_sec;
								gAlarmNodeList[i].typeFlag = CLEARALARM_FLAG;
								
								setDeviceClearAlarmFlag(1);
								sem_post(&SEM_IMFORM);	
								//tcdbg_printf("\n==>CWMP, not Exceed, flag is already ALARM and not reach period time, set DeviceClearAlarm flag, and wait Inform\n");
							}
					}
				}
				os_free(curParaValue);
				curParaValue = NULL;
			}
			
		}
		
		pthread_mutex_lock(&value_change_lock);
		inform_task_start = 0;
		pthread_mutex_unlock(&value_change_lock);
	}
	return;
}


void cwmpDeviceMonitorRoutine(void) {
	char tmpBuf[16] = {0};
	char paraListValue[MAX_ATTRIBUTE_LEN] = {0};
	char nodeName[32] = {0};
	int i, j, ret = 0;
	int maxEntryNum = 0;
	int limitMax = 0, limitMin = 0;
	int paraValue = 0;
	int periodTime = 0;
	DeviceNodePtr currentDeviceNode = NULL;
	char* curParaValue = NULL;
	ParameterType paraType;	
	struct timeval now;
	DeviceNodePtr rootptr = NULL;
	int dval = 0;
	char parasValue[10][MAX_ATTRIBUTE_LEN] = {0};
	int count = 0,idx = 0;
	
	struct timeval begin;
	struct timeval end;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){	
		gettimeofday(&begin, NULL);
		
		if(checkMonitorEnable() < 0) {
			tc_printf("\n==>cwmpDeviceMonitorRoutine Error, Monitor not enable\n");
			return;
		}

		if( gMonitorNodeList == NULL ) {
			tc_printf("\n==>cwmpDeviceMonitorRoutine Error, Monitor node not allocate\n");
			return;
		}
		
		maxEntryNum = checkMonitorEntryNum();

		pthread_mutex_lock(&value_change_lock);
		if(rpc.IGDDeviceRootPtr == NULL){
			pthread_mutex_unlock(&value_change_lock);
			//here, we need to check rpc, and free something, do it later
			tc_printf("\r\n Error!,  cwmp parameter tree is NULL!! \r\n ");
			return ;
		}else{
			inform_task_start =1; //hjzhang: before handling the RPC, set inform flag to 1.
			pthread_mutex_unlock(&value_change_lock);
		}

		for(i=0; i<maxEntryNum; i++) {
			memset(nodeName, 0, sizeof(nodeName));
			
			sprintf(nodeName, MONITOR_ENTRY, i);
			ret = tcapi_get(nodeName, "ParaList", paraListValue);
			if(ret < 0 || strlen(paraListValue) == 0) {
				gMonitorNodeList[i].usedFlag = 0;
				continue;
			}
			if(TCSUPPORT_CMCC_VAL){
				memset(parasValue,0,10*MAX_ATTRIBUTE_LEN);
				idx = 0;
				count = 0;
				for(j=0;j<strlen(paraListValue);j++)
				{
					if(paraListValue[j] == ','){
						if(idx == 0)
							strncpy(parasValue[count],paraListValue,j-idx);
						else
							strncpy(parasValue[count],paraListValue+idx+1,j-idx-1);
						idx = j;
						if(strlen(parasValue[count]) != 0)
						{
							count++;
						}
						j++;
					}
				}
				if(idx < (strlen(paraListValue)-1)){
					if(idx==0)
					  strcpy(parasValue[count],paraListValue+idx);
					else
					  strcpy(parasValue[count],paraListValue+idx+1);
					if(strlen(parasValue[count]) == 0)
						count--;
				}
			}

			gMonitorNodeList[i].usedFlag = 1;

			/* Get timeList*/
			memset(tmpBuf, 0, sizeof(tmpBuf));
			tcapi_get(nodeName, "TimeList", tmpBuf);
			if(strlen(tmpBuf) != 0)
			{
				if(TCSUPPORT_CT_PON_GDV20_VAL)
					periodTime = (int)(atof(tmpBuf) * 60);
				else
				periodTime = atoi(tmpBuf) * 60;
			}
			
			gettimeofday(&now, NULL);
			/*Check CPE time changed or not*/
			dval = now.tv_sec - gMonitorLast.tv_sec;
			if(abs(dval) >= TIMECHANGERANGE && gMonitorNodeList[i].startTime.tv_sec != 0)	{
				/*Update last time*/				
				gMonitorNodeList[i].startTime.tv_sec += dval;
			}
			gMonitorLast = now;
				
			if(gMonitorNodeList[i].typeFlag == MONITOR_NONE_FLAG) {
				if(gMonitorNodeList[i].startTime.tv_sec == 0) {
					gMonitorNodeList[i].startTime.tv_sec = now.tv_sec;
				}
				else {
					if((now.tv_sec - gMonitorNodeList[i].startTime.tv_sec) >= periodTime) {
						if(TCSUPPORT_CMCC_VAL){
							for(j=0;j<count+1;j++){
								currentDeviceNode = NULL;
								cwmpSearchDeviceNodeByName(parasValue[j], rpc.IGDDeviceRootPtr, &currentDeviceNode);
								if(currentDeviceNode == NULL)
									break;
								curParaValue = NULL;
								if(currentDeviceNode->getValueInfo){
									currentDeviceNode->getValueInfo(&curParaValue, &paraType, currentDeviceNode);
								}
								if(curParaValue == NULL)
									break;
								memcpy(gMonitorNodeList[i].paramValue[j], curParaValue, 16);
								memcpy(&gMonitorNodeList[i].type[j], &paraType, sizeof(paraType));
							}
							if(!currentDeviceNode || !curParaValue)
								continue;
						}
						else{
						/* Search node and get value */
						currentDeviceNode = NULL;
						cwmpSearchDeviceNodeByName(paraListValue, rpc.IGDDeviceRootPtr, &currentDeviceNode);
						if(currentDeviceNode == NULL)
							continue;
						if(currentDeviceNode->getValueInfo){
							currentDeviceNode->getValueInfo(&curParaValue, &paraType, currentDeviceNode);
						}
						if(curParaValue == NULL)
							continue;
								
							memcpy(gMonitorNodeList[i].paramValue[0], curParaValue, 16);
							memcpy(&gMonitorNodeList[i].type[0], &paraType, sizeof(paraType));
						}
						
						gMonitorNodeList[i].startTime.tv_sec = now.tv_sec;
						gMonitorNodeList[i].typeFlag = MONITOR_FLAG;
							
						setDeviceMonitorFlag(1);
						sem_post(&SEM_IMFORM);

						os_free(curParaValue);
						curParaValue = NULL;
						//tcdbg_printf("\n==>CWMP, reach period time, set DeviceMonitor flag, and wait Inform\n");			
					}
				}
			}
		}
		
		pthread_mutex_lock(&value_change_lock);
		inform_task_start = 0;
		pthread_mutex_unlock(&value_change_lock);
	}
	return;
}

void *cwmpAlarmMonitorTask(void){
	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		while(1){

			/* If Alarm and Monitor both are not enable, break */
			if((checkAlarmEnable() < 0) && (checkMonitorEnable() < 0))
				break;
			
			/* Start Alarm and Monitor Routine*/
			cwmpDeviceAlarmRoutine();
			cwmpDeviceMonitorRoutine();

			sleep(10);
		}
		
		/*Set p_AlarmMonitor varible to 0 means thread exit*/
		tc_printf("\n======>cwmpAlarmMonitorTask, do thread exit\n");
		p_AlarmMonitor = 0;
	}
}

int cwmpAlarmMonitorTaskStart(void)
{
	int ret;
	pthread_attr_t p_alarmMonitorHandle_attr;
	int retAlarm = -1, retMonitor = -1;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		/* Check Alarm is enable or not */
		retAlarm = g_AlarmEnable;
		if(retAlarm == 0) {
			/*get the start time*/
			gettimeofday(&gAlarmLast, NULL);

			/*check entry num and allocate memory for each entry*/
			checkAndInitAlarm();
		}
		
		/* Check Monitor is enable or not */
		retMonitor = g_MonitorEnable;
		if(retMonitor == 0) {
			/*get the start time*/
			gettimeofday(&gMonitorLast, NULL);

			/*check entry num and allocate memory for each entry*/
			checkAndInitMonitor();
		}
		
		if((retAlarm == -1) && (retMonitor == -1)) {
			tc_printf("\n==>cwmpAlarmMonitorTaskStart Error, Alarm and Monitor  not enable no need to creat task\n");
			return -1;
		}		

		/*create thread*/
		ret = pthread_attr_init(&p_alarmMonitorHandle_attr);
		if(ret != 0)
		{
			tc_printf("\r\nThread(AlarmMonitor) attribute creation fail!");
			return -1;
		}

		ret = pthread_attr_setdetachstate(&p_alarmMonitorHandle_attr,PTHREAD_CREATE_DETACHED);
		if(ret != 0)
		{
			tc_printf("\r\nThread(AlarmMonitor):Set attribute fail!");
			return -1;
		}

		ret = pthread_create(&p_AlarmMonitor,&p_alarmMonitorHandle_attr,(void *)cwmpAlarmMonitorTask,NULL);
		if(ret != 0)
		{
			tc_printf("\r\nThread(AlarmMonitor):Create thread fail!");
			return -1;
		}

		pthread_attr_destroy(&p_alarmMonitorHandle_attr);
	}
	return 0;
}

void checkAndDoAlarmMonitorTask(void) {
	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		if((g_AlarmEnable == 0) || 
			(g_MonitorEnable== 0)) {
			if(p_AlarmMonitor == 0) {//alarm task is not running
				tc_printf("\r\ncreate thread to do device alarm and monitor!");
				cwmpAlarmMonitorTaskStart();
			}
		}
	}
}

#define UPNP_DEVNUM_MAX 255
void CheckUPnPDev(void){
	char filePath[32] = "";
	FILE *fp = NULL;
	int i = 1, ret = 0;
	
	if(TCSUPPORT_CT_UPNP_DM_VAL){	
		for(i = 1; i < UPNP_DEVNUM_MAX; i++){
			memset(filePath,0,sizeof(filePath));
			sprintf(filePath,"/tmp/dev%02x/f0.xml",i);
			fp = fopen(filePath, "r");
			if(fp == NULL){
				continue;
			}
			fclose(fp);
			ret = ProcessUPnPAddDelDevAction_Open(OP_ADD_DEVICE,i);
			if(ret){
				tcdbg_printf("%s :ProcessUPnPAddDelDevAction(adddev=%d)!\n",__FUNCTION__, i);  
			}

		}
	}
}

//modify by brian

void *cwmpInformTask(){
	uint8 status = 0;
	int i_state;
	int	op_flag = 1;
	uint32 second = 1000;

	if(TCSUPPORT_CT_MIDWARE_VAL)
		pthread_mutex_lock(&midware_task_lock);

	while(1){
		if(TCSUPPORT_CT_MIDWARE_VAL)
			pthread_mutex_unlock(&midware_task_lock);

		sem_wait(&SEM_IMFORM);
		if(TCSUPPORT_CT_MIDWARE_VAL){
			midware_resovle_Open();
			pthread_mutex_lock(&midware_task_lock);
			if(midwareValueChangeRoutine()){
				sem_post(&SEM_MW_IMFORM);
			}
			if(iTr069Enable == 0){  // cwmp not start
				continue;
			}
		}

	
		if(TCSUPPORT_CT_UPNP_DM_VAL){
			if(iUPnPDMModifyFlag){			
				tcdbg_printf("%s:iUPnPDMModifyFlag = 1, need do cwmpUPnPDMRoutine()!!!\n",__FUNCTION__);	
				if(cwmpUPnPDMRoutine() == 0){
					tcdbg_printf("%s:do cwmpUPnPDMRoutine() success!!!\n",__FUNCTION__);	
					iUPnPDMModifyFlag = 0;
				}
				//continue;
			}
		}
		getdbgflag();
		
		if(getServerUpFlag() == FALSE)
		{
			tc_printf("\r\n[cwminformtask]Server is not Up, we will wait for next singal\n");
			continue;
		}
#ifdef THREAD_REDUCE
		if(ValueChangeFlag)
		{
			tc_printf("\r\n[cwminformtask]cwmpValueChangeRoutine entered\n");
			ValueChangeFlag = 0;
			if(cwmpValueChangeRoutine() == 0)
			{
				tc_printf("\r\n[cwminformtask]cwmpValueChangeRoutine return 0, we will not signal value change inform\n");
				continue;
			}
			/*Value change will cause inform Task*/
		}

		if(cwmpGetF5LoopbackDiagnosingFlag() == 1)
		{
			tc_printf("\r\n[cwminformtask]F5 Diagnostic In Process, we will not signal value change inform\n");
			continue;			
		}
#endif		

		if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && cardNotifyFlag)
		{
			tc_printf("\r\n[cwminformtask]cardNotifyFlag entered\n");
			cardNotifyFlag = 0;
			if(cwmpValueChangeRoutine() == 0)
			{
				tc_printf("\r\n[cwminformtask]cardNotifyFlag: cwmpValueChangeRoutine return 0\n");
			}
			/*Value change will cause inform Task*/
		}

		//modify by brian
		pthread_mutex_lock(&value_change_lock);
		if(rpc.IGDDeviceRootPtr == NULL){
			/*hjzhang: if root ptr is NULL means the tree is not available now,
					   do nothing and continue.
			*/
			pthread_mutex_unlock(&value_change_lock);
			tc_printf("\r\n competitor: cwmp tree is NULL! end inform. \r\n ");
			continue;
		}else{
			inform_task_start =1; //hjzhang: set flag to 1. 
			pthread_mutex_unlock(&value_change_lock);
		}

		resetWebpageFlag();
		if(!getCwmpSwitch())	
		{
			pthread_mutex_lock(&value_change_lock);
			inform_task_start = 0;  
			pthread_mutex_unlock(&value_change_lock);
			continue;
		}
		if(TCSUPPORT_CT_INFORM_NODE_VAL){
			if(!strcmp(getCwmpUrl(),""))
			{
				tcapi_set("Cwmp_Entry", "Inform_Status","4");  //acs is empty
				tc_printf("\r\n url is null,do not build inform!\n");
				pthread_mutex_lock(&value_change_lock);
				inform_task_start = 0;  
				pthread_mutex_unlock(&value_change_lock);
				continue;
			}
		}	
		if(!initNotifyListFlag){
			cwmpCheckIPValid();
			pthread_mutex_lock(&value_change_lock);
			inform_task_start = 0;  
			pthread_mutex_unlock(&value_change_lock);
			if(TCSUPPORT_CT_UPNP_DM_VAL){
				if(iUPnPDMModifyFlag && initNotifyListFlag){
					CheckUPnPDev();
					freeUPnPRoutineActionList_Open();
					iUPnPDMModifyFlag = 0;
				}
			}
			if(TCSUPPORT_CT_MIDWARE_VAL)
				op_flag=getTr069RestartItself();
			if(op_flag)
			{
				continue;
			}
		}

		if(TCSUPPORT_CT_INFORM_NODE_VAL){
			if(cwmpCheckWanIP()){
				pthread_mutex_lock(&value_change_lock);
				inform_task_start = 0;  
				pthread_mutex_unlock(&value_change_lock);
				continue;
			}
		}
		//Stop the priodic timer for checking wan ip and sending boot inform. xzwang_060804
		if(bootTimerID){
			timerPeriodicStop(bootTimerID);
			bootTimerID = 0;

			/* Bugfree #10956 add by dennyxiang @ 20110715
			 * Now we start periodicinform after the boot inform can be send 
			 * for unicom test	*/
			if(TCSUPPORT_RANDOM_INFORM_VAL){
				if(cwmpRepeatFlag && repeatTimes && repeatPeriodicVal){
					if(!customTimerID && !customStopTimerID){
						tc_printf("\r\ncwmpInit");
						customTimerID = AddPeriodicInformTimer( repeatPeriodicVal * second,  (TimerCallback)cwmpPeriodInform, NULL );
						customStopTimerID = timerStartS((repeatPeriodicVal * repeatTimes)*second, (TimerCallback)stopRepeatInform, NULL );
					}
				}
				else{//sam_20081225 merge from huawei
					startPeriodicInform();
				}
			}		
		}

		status = cwmpBuildInformPkt();
		
		if(status){
			cwmpSendInform();
		}

		pthread_mutex_lock(&value_change_lock);
		inform_task_start = 0; //hjzhang: set flag to 0. 
		pthread_mutex_unlock(&value_change_lock);
	}
}

int cwmpInformTaskStart(void)
{
int ret;
	pthread_t p_informHandle;
	pthread_attr_t p_informHandle_attr;

	/*create thread*/
	ret = pthread_attr_init(&p_informHandle_attr);
	if(ret != 0)
	{
		tc_printf("\r\nThread(inform) attribute creation fail!");
		return -1;
	}

	ret = pthread_attr_setdetachstate(&p_informHandle_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tc_printf("\r\nThread(inform):Set attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_informHandle,&p_informHandle_attr,(void *)cwmpInformTask,NULL);
	if(ret != 0)
	{
		tc_printf("\r\nThread(inform):Create thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_informHandle_attr);
}
int8 cwmpFreeStringStruct(StringStructPtr* requestNameLstPtr){
	StringStructPtr curArgPtr = NULL;
	StringStructPtr curArgHead = NULL;

	curArgPtr = *requestNameLstPtr;
	if(curArgPtr == NULL)
		return 0;
	
	while(curArgPtr){
		curArgHead = curArgPtr;
		if(curArgPtr->value != NULL){
			os_free(curArgPtr->value);
			curArgPtr->value= NULL;
		}
		curArgPtr = curArgPtr->next;
		os_free(curArgHead);
	}

	*requestNameLstPtr = NULL;

	return 0;
}

//extern int searchNodeInTree(DeviceNodePtr deviceNode, DeviceNodePtr nodeTree);
int cwmpGetNodeName(DeviceNodePtr rootNode, DeviceNodePtr deviceNode, char* deviceName){
	DeviceNodePtr tempDeviceNode = deviceNode;
	char subNodeName[PARAMETER_NUM][PARAMETER_NAME_LEN];
	int8 i = 0, j = 0;

	if(rootNode == NULL){
		tc_printf("The root device node is NULL and get name fails!\r\n");
		return -1;
	}
	if(deviceNode == NULL){
		tc_printf("The device node is NULL and get name fails!\r\n");
		return -1;
	}
	if(deviceName == NULL){
		tc_printf("The Pointer for store device node name is NULL!\r\n");
		return -1;
	}
//	if(searchNodeInTree(deviceNode, rootNode) != 0){
//		tc_printf("\r\n ****************dirty deviceNode, avoid crash**********");
//		return -1;
//	}
	
	memset(deviceName, 0, strlen(deviceName)+1);

	while(tempDeviceNode != rootNode){
		strcpy(subNodeName[i], tempDeviceNode->name);
		tempDeviceNode = tempDeviceNode->parentNode;
		i++;
	}
	strcpy(subNodeName[i], tempDeviceNode->name);

	for(j = i; j >= 0; j--){
		strcat(deviceName,subNodeName[j]);
		if(j != 0)
			strcat(deviceName,".");
	}

	return 0;
}

/*
 * pop the node from stack and transfer node point to full name, store full name
*/
uint8 popNodeToName(DeviceNodePtr rootNode)
{
	DeviceNodePtr  deviceNode = NULL;
	int8 ret= 0;
	int8 state = 0;
	int len;
	char *paraName = NULL;
	StringStructPtr requestNameLst = NULL;
	StringStructPtr requestNameLstPtr = NULL;
	char nameBuf[256] = "";

	if(rootNode == NULL){//if rootNode is NULL, pop all the node in stack and drop them.
		tc_printf("The root device node is NULL and popNodeToName fails!\r\n");
		cwmpPopValueChangeStack(&deviceNode);
		while(deviceNode){
			deviceNode = NULL;
			cwmpPopValueChangeStack(&deviceNode);
		}
		return 0;
	}
	
	cwmpPopValueChangeStack(&deviceNode);
	
	while(deviceNode){
		memset(nameBuf, 0, 256);
		ret = cwmpGetNodeName(rootNode, deviceNode, nameBuf);
		if(ret != 0)
			goto Error_Handler;
		len = strlen(nameBuf);
		//modify by brian for removing warning
		if((requestNameLst = (StringStructPtr)os_malloc(sizeof(StringStruct))) == NULL){
			tc_printf("malloc request namelist fails!\r\n");
			ret = -1;
			goto Error_Handler;
		}
		if((paraName = (char*)os_malloc(len+1)) == NULL){
			tc_printf("malloc name buf fails!\r\n");
			ret = -1;
			goto Error_Handler;
		}
		memset(paraName, 0 , len+1);
		strncpy(paraName, nameBuf, len);
		
		requestNameLst->value = paraName;
		requestNameLst->next = NULL;

		if(valueChangeNameList == NULL){
			valueChangeNameList = requestNameLst;
		}
		else{
			requestNameLstPtr->next = requestNameLst;
		}
		requestNameLstPtr = requestNameLst;
		
		deviceNode = NULL;
		cwmpPopValueChangeStack(&deviceNode);
	}	
	return ret;
	
Error_Handler:
	cwmpFreeStringStruct(&valueChangeNameList);
	return ret;
}

/*
 * push the node into stack
*/
uint8 pushNameToStack(void)
{
	DeviceNodePtr currentDeviceNode = NULL;
	StringStructPtr requestNameLstPtr = valueChangeNameList;

	while(requestNameLstPtr){
		cwmpSearchDeviceNodeByName(requestNameLstPtr->value, rpc.IGDDeviceRootPtr, &currentDeviceNode);
		if(currentDeviceNode == NULL){
			tc_printf("\r\nThe name node does not exist!");
			requestNameLstPtr = requestNameLstPtr->next;
			continue;
		}
		cwmpPushValueChangeStack(currentDeviceNode);
		requestNameLstPtr = requestNameLstPtr->next;
	}

	cwmpFreeStringStruct(&valueChangeNameList);

	return 0;
}

int valueModify_Nofity = 0;
#ifndef THREAD_REDUCE
//modify by brian
void *cwmpValueChangeTask(void){
	NotifyType notify = Active;
	DeviceNodePtr rootptr = NULL;
	//int i_state;

	while(1){
		//modify by brian
		sem_wait(&SEM_VALCHANGE);
		//Re-init the parameter tree for sync with web and console, hqfeng, 2006.11.02
		/* hjzhang: here will free the whole tr69 parameter tree*/
		//modify by brian
		//i_state = dirps();
		pthread_mutex_lock(&value_change_lock);
		if(inform_task_start == 1){
			/* if inform task is running, quit and do nothing */
//			signal_valueChg_flag = 1; //spSave() will call this task again, no need set flag to re-do it.
			pthread_mutex_unlock(&value_change_lock);
			tc_printf("\r\n inform is performing.... set value change flag.\r\n ");
			continue;
		}else{
			rootptr = rpc.IGDDeviceRootPtr;
			rpc.IGDDeviceRootPtr = NULL;
			pthread_mutex_unlock(&value_change_lock);
		}

		popNodeToName(rootptr);

		//Re-init the parameter tree for sync with web and console, hqfeng, 2006.11.02
		cwmpFreeParameterTree(rootptr);
		rootptr = NULL;
		cwmpInitParameterTree();
//		signal_valueChg_flag = 0;
		pushNameToStack();
	
		if(cwmpSearchChangedValue(notify) == 1){
			if(dbg_cwmp >= DBG_CWMP_L2){
			tc_printf("\r\n =======> send value change inform\r\n ");
			}
			setActiveNotifyFlag(1);
			//modify by brian
			sem_post(&SEM_IMFORM);
		}	
	}
}


int cwmpValueChangeTaskStart(void)
{
	int ret;
	pthread_t p_valchangeHandle;
	pthread_attr_t p_valchangeHandle_attr;

	/*create thread*/
	ret = pthread_attr_init(&p_valchangeHandle_attr);
	if(ret != 0)
	{
		tc_printf("\r\nThread(valuechange) attribute creation fail!");
		return -1;
	}

	ret = pthread_attr_setdetachstate(&p_valchangeHandle_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tc_printf("\r\nThread(valuechange):Set attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_valchangeHandle,&p_valchangeHandle_attr,(void *)cwmpValueChangeTask,NULL);
	if(ret != 0)
	{
		tc_printf("\r\nThread(valuechange):Create thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_valchangeHandle_attr);
}
#else
/*return 1 mean need signal inform, others return 0*/
int cwmpValueChangeRoutine(void)
{
	NotifyType notify = Active;
	DeviceNodePtr rootptr = NULL;

	pthread_mutex_lock(&value_change_lock);
	if(inform_task_start == 1){
		/* if inform task is running, quit and do nothing */
//			signal_valueChg_flag = 1; //spSave() will call this task again, no need set flag to re-do it.
		pthread_mutex_unlock(&value_change_lock);
		tc_printf("\r\n inform is performing.... set value change flag.\r\n ");
		return 0;
	}else{
		rootptr = rpc.IGDDeviceRootPtr;
		rpc.IGDDeviceRootPtr = NULL;
		//pthread_mutex_unlock(&value_change_lock);
	}

	popNodeToName(rootptr);

	//Re-init the parameter tree for sync with web and console, hqfeng, 2006.11.02
	cwmpFreeParameterTree(rootptr);
	rootptr = NULL;
	cwmpInitParameterTree();
//		signal_valueChg_flag = 0;
	pushNameToStack();

	pthread_mutex_unlock(&value_change_lock);

	if(cwmpSearchChangedValue(notify) == 1){
		if(dbg_cwmp >= DBG_CWMP_L2){
		tc_printf("\r\n =======> send value change inform\r\n ");
		}
		setActiveNotifyFlag(1);
		pthread_mutex_lock(&value_change_lock);
		inform_task_start = 0;
		pthread_mutex_unlock(&value_change_lock);
		return 1;
		//modify by brian
		//sem_post(&SEM_IMFORM);
	}	
	pthread_mutex_lock(&value_change_lock);
	inform_task_start = 0;
	pthread_mutex_unlock(&value_change_lock);

	if(CT_COM_DEVICEREG_VAL){
		//we will force value change routine when device is not registered
		if(/*getInitialDeviceRegisterFlag() == 1 &&*/ getDevregInformFlag() == 1)
		{
			if(dbg_cwmp >= DBG_CWMP_L2){
				tc_printf("\r\n =======> device NOT registered, send forced value change inform\r\n ");
			}
			//setDevregInformFlag(0);
			return 1;
		}
	}

	/* bootstrap flag is set, send inform */
	if (getforceInformFlag() == 1) {
		if(dbg_cwmp >= DBG_CWMP_L1){
			tc_printf("\r\n =======> flag is set, send forced value change inform\r\n ");
		}
		//setforceInformFlag(0);
		return 1;
	}

	if(cwmpSearchChangedValue(Passive) == 1)
		valueModify_Nofity = 1; 	
	cwmpClearNotifyNodeList(Passive);
	cwmpClearNotifyNodeList(Active);
	cwmpInitNotifyNodeList();

	return 0;
}

#endif

void *cwmpAsynTcapiTask(void)
{
    while(1)
    {
        sem_wait(&SEM_ASYNTCAPI);
        
        tr069AsynNodeExecute();

        pthread_mutex_lock(&asyn_tcapi_lock);
        asynOutQueue();
        pthread_mutex_unlock(&asyn_tcapi_lock);
    }
    return;
}

int cwmpAsynTcapiTaskStart(void)
{
	int ret;
	pthread_t p_asynTcapiHandle;
	pthread_attr_t p_asynTcapiHandle_attr;

	/*create thread*/
	ret = pthread_attr_init(&p_asynTcapiHandle_attr);
	if(ret != 0)
	{
		tc_printf("\r\nThread(asynTcapi) attribute creation fail!");
		return -1;
	}

	ret = pthread_attr_setdetachstate(&p_asynTcapiHandle_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tc_printf("\r\nThread(asynTcapi):Set attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_asynTcapiHandle,&p_asynTcapiHandle_attr,(void *)cwmpAsynTcapiTask,NULL);
	if(ret != 0)
	{
		tc_printf("\r\nThread(asynTcapi):Create thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_asynTcapiHandle_attr);
}


int midwareValueChangeRoutine(void)
{	
	int ret = 0;
	NotifyType notify = Active;
	DeviceNodePtr rootptr = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(iTr069Enable == 1){
			return 0;
		}
		if(ValueChangeFlag ){
			if(midwareSearchChangedValue(TRActive)){
				setMWActiveNotifyFlag(1);
				if(dbg_cwmp >= DBG_CWMP_L2) tc_printf("midware change:send mw inform!\n");
				ret = 1;
			}
			if(iTr069Enable == 0){ //cwmp not start, informITMS here
				ValueChangeFlag = 0;
				if(midwareSearchChangedValue(ITMSActive)){
					setITMSActiveNotifyFlag(1);
					if(dbg_cwmp >= DBG_CWMP_L2) tc_printf("midware change:send itms inform!\n");
			ret = 1;
					//sem_post(&SEM_MW_IMFORM);
				}
		}
		}
		if(MWValueChangeFlag && (midwareSearchChangedValue(MWActive) || getMWWanChangeIndex(0xff))){
			ret = 1;
			setMWActiveNotifyFlag(1);
			if(dbg_cwmp >= DBG_CWMP_L2) tc_printf("midware change:send mw inform!\n");
		}
		if(ret == 1){
			if(dbg_cwmp >= DBG_CWMP_L2){
				tc_printf("\r\n =======> midware send value change inform\r\n ");
			}
			sem_post(&SEM_MW_IMFORM);
		}
		MWValueChangeFlag = 0;
	}	
	return ret;
}

/*qingpu.20071129.for fix crash found in Huawei*/
#if 0 //mark by brian
void cwmpReInitHostTableTask(void)
{
	DeviceNodePtr HostNode = NULL;
	DeviceNodePtr rootptr = NULL;
	int i_state;
	char *objectName = "InternetGatewayDevice.LANDevice.1.Hosts.Host.";	

/*	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &HostNode);
	if(HostNode == NULL){
		return;
	}	
*/
	while(1){
		wwait();
		//i_state = dirps();
		if(inform_task_start == 1){
			/* if inform task is running, quit and do nothing 
			  * if inform_task_start locked, at the end of current http session will call spSave(), to call cwmpValueChangeTask reInit tree
			  */
			//restore(i_state);
			continue;
		}else{
			rootptr = rpc.IGDDeviceRootPtr;
			rpc.IGDDeviceRootPtr = NULL;
			//restore(i_state);
		}
		HostNode = NULL;
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.LANDevice.1.Hosts.Host.", rootptr, &HostNode);
		if(HostNode != NULL){
			popNodeToName(rootptr);
			cwmpFreeDeviceTable(HostNode);
			cwmpInitHostTable(HostNode);
			pushNameToStack();
		}
		
		//i_state = dirps();
		rpc.IGDDeviceRootPtr = rootptr;	
		//restore(i_state);
	}
}
#endif
#if 0 //mark by brian
void cwmpReInitHostTableTaskStart(){

	if (!cwmpReInitHostTableTaskId) {
		if ((cwmpReInitHostTableTaskId = sysCreateTask("cwmpReInitHostTableTask",
									  (void *) cwmpReInitHostTableTask,
									  4096, TASK_PRI_NORMAL+2)) != 0) {
			sysStartTask(cwmpReInitHostTableTaskId);
		} else
			tc_printf("\r\nCreate ReInitHostTable task fails!");			
	}
}


uint32 getCwmpReInitHostTableTaskId(void)
{
	return cwmpReInitHostTableTaskId;
}
#endif
#ifdef TR143
char gServerIP[32] = {0};

uint8 *
put32(
	uint8	*cp,
	uint32	x
)
{
	*cp++ = x >> 24;
	*cp++ = x >> 16;
	*cp++ = x >> 8;
	*cp++ = x;
	return cp;
} /* put32 */

int checkUDPEchoEnable( void )
{
	int rtnCode = 0;
	char enable[10];

	memset( enable, 0, sizeof(enable) );
	tcapi_get( "Cwmp_tr143UDPEcho", "Enable", enable );

	if ( !strcmp(enable, "Yes") ) {
		rtnCode = 1;
	}

	return rtnCode;
}

int checkUDPEchoPlusEnable( void )
{
	int rtnCode = 0;
	char enable[10];

	memset( enable, 0, sizeof(enable) );
	tcapi_get( "Cwmp_tr143UDPEcho", "EchoPlusEnabled", enable );

	if ( !strcmp(enable, "Yes") ) {
		rtnCode = 1;
	}

	return rtnCode;
}

int getUDPEchoPort( void )
{
	int port = 0;
	char udpEchoPort[10];

	memset( udpEchoPort, 0, sizeof(udpEchoPort) );

	tcapi_get( "Cwmp_tr143UDPEcho", "UDPPort", udpEchoPort );

	port = atoi( udpEchoPort );

	return port;
}
char UDPEchoSourceIP[32];
char *getUDPEchoSourceIP( void )
{
	char srcIp[32];
	memset( srcIp, 0, sizeof( srcIp) );

	tcapi_get( "Cwmp_tr143UDPEcho", "SourceIPAddress", srcIp );

	strcpy( UDPEchoSourceIP, srcIp );

	return UDPEchoSourceIP;
}

void clearUDPEchoStats( void )
{

	rpc.UDPEchoInfo.packetReceived = 0;
	rpc.UDPEchoInfo.packetResponsed = 0;
	rpc.UDPEchoInfo.bytesReceived = 0;
	rpc.UDPEchoInfo.bytesResponsed = 0;
	rpc.UDPEchoInfo.respReplyFailCnt = 0;
	//rpc.UDPEchoInfo.sTimeFirstRcv = 0;
	//rpc.UDPEchoInfo.sysTimeFirstRcv = 0;	
	//rpc.UDPEchoInfo.sysTimeRecentRcv = 0;
	memset( &rpc.UDPEchoInfo.sysTimeFirstRcv, 0, sizeof(rpc.UDPEchoInfo.sysTimeFirstRcv) );
	memset( &rpc.UDPEchoInfo.sysTimeRecentRcv, 0, sizeof(rpc.UDPEchoInfo.sysTimeRecentRcv) );
	
}
int getServerIPFromInterface( char *serverIP )
{
	int rtn = 0, lanIfLen, wanIfLen;
	uint32 pvcNum = 0,entryNum = 0;
	char nodeName[32], setInterface[128];
	char defLanInterface[] = "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1";
	char defWanInterface[] = "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.";
	char *entryPtr = NULL, *pvcPtr = NULL;

	memset( nodeName, 0, sizeof(nodeName) );
	memset( setInterface, 0, sizeof(setInterface) );
	lanIfLen = strlen( defLanInterface );
	wanIfLen = strlen( defWanInterface );

	if ( serverIP == NULL ) {
		return -1;
	}
	
	tcapi_get( "Cwmp_tr143UDPEcho", "Interface", setInterface );

	
	if ( strncmp(defLanInterface, setInterface, lanIfLen) == 0 )
	{
		tcapi_get( "Lan_Entry0", "IP", serverIP );
		if ( strcmp(serverIP, "") == 0 ) {
			return -1;
		}
	}
	else if ( strncmp(defWanInterface, setInterface, wanIfLen) == 0 )  
	{
		pvcPtr = strstr(setInterface, "WANConnectionDevice");
		if(pvcPtr != 0)
		{
			pvcPtr += sizeof("WANConnectionDevice") ;
		}
		else 
		{
			return -1;
		}

		
		if( (entryPtr = strstr(setInterface,"WANPPPConnection") ) != NULL )
		{
			entryPtr += sizeof("WANPPPConnection") ;
		}
		else if ( (entryPtr = strstr(setInterface,"WANIPConnection") ) != NULL )
		{
			entryPtr += sizeof("WANIPConnection");
		}
		else
		{
			return -1;
		}
		
		pvcNum = atoi(pvcPtr);
		entryNum = atoi(entryPtr);
		
		if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
			if(FindWanNodeMapping(&pvcNum, &entryNum) != 0)
			{	
				tc_printf( "The pvc & entry does not exist.\n" );
				return -1;
			}
			sprintf( nodeName, "WanInfo_Entry%d", (pvcNum) <<3 |(entryNum));			
		}
		else{
			if ( pvcNum < 1 || pvcNum > 8 || entryNum < 1 || entryNum > 8) {
				tc_printf( "The pvc & entry does not exist.\n" );
				return -1;
			}
			sprintf( nodeName, "WanInfo_Entry%d", (pvcNum - 1) <<3 |(entryNum -1) );
		}
		tcapi_get( nodeName, "IP", serverIP );
		if ( strcmp(serverIP, "") == 0 ) {
			return -1;
		}	
		
	}
	else if ( strcmp( setInterface, "" ) == 0 ) {
		return 0;
	}
	else {
		return -1;
	}

	return 0;
}

void UDPEchoStart( void )
{
	struct sockaddr_in my_addr;
	int udpEchoPort = 0, rtn = 0;
	char serverIP[32];

	memset( serverIP, 0, sizeof(serverIP) );

	/*if UDPEcho is disabled, just return*/
	if ( checkUDPEchoEnable() == 0 ) {
		//tc_printf( "UDPEcho is disabled.\n" );
		closeUDPSocket();
		return;
	}

	rtn = getServerIPFromInterface( serverIP );
	if ( rtn == -1 ) {
		closeUDPSocket();
	    return;
	}
	
	/*socket has been opened*/
	if ( sockfd != -1 ) {
		
		/*if ip changed,we should create socket again*/
		if ( strcmp(gServerIP, serverIP) != 0 ) {
			closeUDPSocket();
		}
		else {
			return;
		}
	}

	/*open udp socket and bind to a specific address*/
	if ( (sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) == -1 ) {
		tc_printf( "open socket fail.\n" );
	    closeUDPSocket();
	    return;
	}
	
	/*set this socket nonblock*/
	fcntl( sockfd, F_SETFL, O_NONBLOCK );
	
	/*init address that need to bind to this socket*/
	my_addr.sin_family = AF_INET;

	udpEchoPort = getUDPEchoPort();
	
	if ( udpEchoPort > 0 ) {
		my_addr.sin_port = htons( udpEchoPort );
	}
	else {
		my_addr.sin_port = htons( 7 );
	}

	if ( strcmp(serverIP, "") == 0 ) {
		my_addr.sin_addr.s_addr = INADDR_ANY;
	}
	else {
		my_addr.sin_addr.s_addr = inet_addr( serverIP );
	}

	//bzero( &(my_addr.sin_zero), 8 );
	memset( &(my_addr.sin_zero), 0, 8 );

	/*bind this socket to this address*/
	if ( bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1 ) {
		tc_printf( "bind socket fail.\n" );
		closeUDPSocket();
	    return;
	}

	/*save the current serverIP*/
	strcpy( gServerIP, serverIP );
	
	return;
}

void closeUDPSocket( void ) 
{
 	if ( sockfd != -1 ) {
		close( sockfd );
		sockfd = -1;
 	}
	return;
}

void UDPEchoRecvSendProc( void )
{
	struct sockaddr_in client_addr;
	int addr_len, numbytes, length, i = 0;
	char buf[ MAXBUFLEN ], *cp = NULL;
	time_t ltime;

	memset( buf, 0, sizeof(buf) );
	
	/*invalid socket, may be init fails*/
	if ( sockfd == -1 ) {
		return;
	}

	addr_len = sizeof( struct sockaddr );

	/*we will try to reveive ten packets every time*/
	while ( (i++) < PACKNUM ) {
		
		if ( (numbytes = recvfrom( sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&client_addr, &addr_len)) == -1 ) {
			tc_printf( "recv data fail, may client donot sent.\n" );
		}
		else {
			getUDPEchoSourceIP();
			
			/*check if the source ip is right or not*/
			if ( strcmp( UDPEchoSourceIP, (char*)inet_ntoa(client_addr.sin_addr) ) != 0 ) {
				tcdbg_printf("The src ip is invalid.\n");
				return;
			}
			
			/*reveive a packet success, check if udpplus is enabled or that is a valid udpplus packet*/
			if( checkUDPEchoPlusEnable() == 0 || ( numbytes < 20 ) ) {
				
				/*udp echo plus is disbled, or the packet is not a udpplus packet so we send what we reveive*/
				length = numbytes + UDPHEADERLEN;

				if ( rpc.UDPEchoInfo.packetReceived == 0 ) {//first received packet
					gettimeofday( &rpc.UDPEchoInfo.sysTimeFirstRcv, NULL );
				}
				
				rpc.UDPEchoInfo.packetReceived++;
				rpc.UDPEchoInfo.bytesReceived += length;
				
				gettimeofday( &rpc.UDPEchoInfo.sysTimeRecentRcv, NULL );

				/*start to send to client*/
				if ( numbytes == sendto( sockfd, buf, numbytes, 0, (struct sockaddr*)&client_addr, sizeof(struct sockaddr)) ) {
					/*send data to client success*/
					rpc.UDPEchoInfo.packetResponsed++;
					rpc.UDPEchoInfo.bytesResponsed += length;
				}
				else {
					tc_printf( "send fail.\n" );
					rpc.UDPEchoInfo.respReplyFailCnt++;
				}
			}
			else {
				
				/*udp echo plus is enabled, so we should add info to response*/
				length = numbytes + UDPHEADERLEN;

				if ( rpc.UDPEchoInfo.packetReceived == 0 ) {	//first received packet
					gettimeofday( &rpc.UDPEchoInfo.sysTimeFirstRcv, NULL );
				}
				
				rpc.UDPEchoInfo.packetReceived++;
				rpc.UDPEchoInfo.bytesReceived += length;
				
				gettimeofday( &rpc.UDPEchoInfo.sysTimeRecentRcv, NULL );

				/*fill the data that need to send to client*/
				cp = buf;
				cp += 4; 			//skip TestGenSN
				cp = put32( cp, (rpc.UDPEchoInfo.packetResponsed+1) ); //TestRespSN
				ltime = rpc.UDPEchoInfo.sysTimeRecentRcv.tv_sec;
				cp = put32(cp, (uint32)ltime);
				time(&ltime);
				cp = put32(cp, (uint32)ltime);
				cp = put32(cp, rpc.UDPEchoInfo.respReplyFailCnt);

				/*start to send to client*/
				if ( numbytes == sendto( sockfd, buf, numbytes, 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) ) {
					/*send data to client success*/
					rpc.UDPEchoInfo.packetResponsed++;
					rpc.UDPEchoInfo.bytesResponsed += length;
				}
				else {
					tc_printf( "send fail.\n" );
					rpc.UDPEchoInfo.respReplyFailCnt++;
				}
			}
		}
	}
	
	return;
}
#endif

//modify by brian
void	cwmpParseSoapPacket(void){
	unsigned int theDataLength;
	int i_state;

        if(dbg_cwmp >= DBG_CWMP_L2 ){
    		   tc_printf("\r\n[cwmpParseSoapPacket] Begin parse the received data and build data for sending \r\n ");
        }
		
		pthread_mutex_lock(&value_change_lock);
	if(rpc.IGDDeviceRootPtr == NULL){
				pthread_mutex_unlock(&value_change_lock);
		//here, we need to check rpc, and free something, do it later
		tc_printf("\r\n Error!,  cwmp parameter tree is NULL!! \r\n ");
		if(rpc.rxBuf){
			os_free(rpc.rxBuf);
			rpc.rxBuf = NULL;
		}
		
		setACSFaultCode(ACS_ResourceExceed); //set this value to make this http session fPersistentConnection = 0, end this session.
		
		return ;
	}else{
		inform_task_start =1; //hjzhang: before handling the RPC, set inform flag to 1.
		pthread_mutex_unlock(&value_change_lock);
	}
	
	Parser_LoadDocument( &rpc.requestDocPtr, rpc.rxBuf, FALSE );

	cwmpRpcDispatch(&rpc);

	pthread_mutex_lock(&value_change_lock);
	inform_task_start = 0;
	pthread_mutex_unlock(&value_change_lock);

	return ;
}

extern cwmpSendUpLoadRequest();
void cwmpBootInform(void){
//modified by brian 
	 	setBootFlag(1);
	 	sem_post(&SEM_IMFORM);
	
}

int getLANHostCount()
{
	FILE *fp = NULL;
	char buf[MAX_BUF_SIZE] = {0};
	int i = 0, iflag = 0, icounts = 0;
	char tmp[6][40] = {0};
#define ATF_COM		0x02		/* completed entry (ha valid)	*/

	fp = fopen("/proc/net/arp", "r");
	if ( NULL == fp)
		return;
	while ( fgets(buf, MAX_BUF_SIZE, fp) )
	{
		if ( 0 == i )
		{
			i ++;
			continue;
		}
		//IP address	   HW type	   Flags	   HW address			 Mask	  Device
		sscanf(buf, "%s %s %s %s %s %s", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
		sscanf(tmp[2], "%x", &iflag);
		if ( (iflag & ATF_COM) && strstr(tmp[5], "br0"))
		{
			icounts ++;
		}
	}
	fclose(fp);

	return icounts;
}
void lanHostStatsCheck(void) {
	uint8 hostNum = 0;

	hostNum = getLANHostCount();
#if 0
	fp=fopen("/proc/net/arp", "r");
	if(fp == NULL){
		//tcdbg_printf("\nlanHostStatsCheck: open /proc/net/arp failed!\n");
		return;
	}

	while (fgets(buf, MAX_BUF_SIZE, fp)) {
		if (strstr(buf, "br0"))
		{
			hostNum++;
		}
	}
	fclose(fp);
#endif
	if(hostNum != gLanHostNum) {
		gLanHostNum = hostNum;
		if(cwmpGetNotifyFlag()) {
			//tcdbg_printf("\nlanHostStatsCheck, do RebuildHostTable\n");
			RebuildHostTable();
		}
	}

	return;

}

void cwmpPeriodInform(void){	
	int	op_flag = 1;
	
//modified by brian 
	setPeriodicFlag(1);
	if(TCSUPPORT_CT_MIDWARE_VAL)
		op_flag=iTr069Enable;
	if(op_flag)
		sem_post(&SEM_IMFORM);
}

void cwmpAccountChangeInform(void){	
	if(TCSUPPORT_CT_INFORM_NODE_VAL){
		setAccountChangeFlag(1);
		sem_post(&SEM_IMFORM);
	}
}

void cwmpSignalTimerInform(uint32 time){
	if(time == 0) 
		cwmpSignalInform();
	else  
		timerStartS( time*1000, (TimerCallback)cwmpSignalInform, NULL );
}

void cwmpSendScheduleInform(void)
{
	setScheduleInformFlag(1);
	cwmpSignalInform();

	return;
}

void cwmpSignalScheduleInform(unsigned int delaySeconds)
{
	if(delaySeconds == 0) 
		cwmpSendScheduleInform();
	else  
		timerStartS( delaySeconds*1000, (TimerCallback)cwmpSendScheduleInform, NULL );
}

void cwmpSignalInform(void){
//modified by brian
		sem_post(&SEM_IMFORM);

}

int checkCanConnectDemandWan(char *wan_node, char *waninfo_node)
{
	char wan_connection[32] = {0}, wan_lasterror[128] = {0};
	char cmdbuf[128] = {0};
	char ppphisaddr[64] = {0};
	char ifname[32] = {0};

	if(TCSUPPORT_CT_PPP_ONDEMAND_VAL){
		if( 0 == tcapi_get(wan_node, "CONNECTION", wan_connection)
			&& 0 == strcmp(wan_connection, "Connect_on_Demand")
#if 0
			&& 0 == tcapi_get(wan_node, "ConnectionError", wan_lasterror)
			&& (0 == strcmp(wan_lasterror, "ERROR_NO_ANSWER")
				|| 0 == strcmp(wan_lasterror, "ERROR_IDLE_DISCONNECT")
				|| 0 == strcmp(wan_lasterror, "ERROR_ISP_DISCONNECT")
				|| 0 == strcmp(wan_lasterror, "ERROR_USER_DISCONNECT"))
#endif
			&& 0 == tcapi_get(wan_node, "IFName", ifname)
			&& '\0' != ifname[0]
			&& 0 == tcapi_get(waninfo_node, "OndemandHisAddr", ppphisaddr)
			&& '\0' != ppphisaddr[0]
			&& 0 != strcmp(ppphisaddr, "N/A"))
		{
			sprintf(cmdbuf, "/sbin/route add %s dev %s", ppphisaddr, ifname);
			system(cmdbuf);

			sprintf(cmdbuf, "ping -q -c 1 -W 1 %s", ppphisaddr);
			system(cmdbuf);
		}
	}

	return 0;
}

void cwmpCheckIPValid(void)
{

	char nodeName[50], wan_node[50];
	char  externalIP[32], lanIP[20];
#if defined(TCSUPPORT_CT)
	char service_app[SERVICE_LIST_LEN];
#else
	char value[10];
#endif
	int valid = 0;
	int i, pvc_index, entry_index, ret;
	int flag = 0;

	strcpy(lanIP, "0.0.0.0");
	tcapi_get("Lan_Entry0","IP",lanIP);
#if 0
	for(i = 0; i < pvcnumber; i++)
	{
		sprintf(nodeName, "Wan_PVC%d", i);
		tcapi_get(nodeName,"DEFAULTROUTE",value);
		if(!strcmp(value,"Yes"))
		{
			sprintf(nodeName, "DeviceInfo_PVC%d", i);
			tcapi_get(nodeName, "WanIP", externalIP);
	
			if(strlen(externalIP) == 0)
			{
				strcpy(externalIP, "0.0.0.0");
			}
			if(strcmp(externalIP, "0.0.0.0") != 0 && strcmp(externalIP, lanIP) != 0)
			{  
			        valid = 1;
			        break;
		        }
		}

	}
#endif

	for (i = 0; i < MAX_WAN_INFO_ENTRY_NUM; i++) {
		pvc_index = i / MAX_PVC_NUM;
		entry_index = i - pvc_index * MAX_PVC_NUM;
		sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
#if defined(TCSUPPORT_CT)
		ret = tcapi_get(wan_node, "ServiceList", service_app);
		if (ret < 0) {
			continue;
		}
		if(TCSUPPORT_CT_SERVICELIST_E8C_VAL){
			if	( (strstr(service_app, TR069) == NULL)) {
				continue;
			}
		}
		else{
			if  ( (0 != strcmp(service_app, TR069)) && (0 != strcmp(service_app, TR069_INTERNET)) ) {
				continue;
			}
		}
		if(TCSUPPORT_CT_INFORM_NODE_VAL)
			flag = 1;
#else
		ret = tcapi_get(wan_node, "DEFAULTROUTE", value);
		if (ret < 0) {
			continue;
		}
		if (0 != strcmp(value, "Yes")) {
			continue;
		}
#endif

		sprintf(nodeName, "WanInfo_Entry%d", i);
		ret = tcapi_get(nodeName, "IP", externalIP);
		if(TCSUPPORT_CT_PPP_ONDEMAND_VAL){
			if (ret < 0)
				strcpy(externalIP, "0.0.0.0");
		}
		else{
			if (ret < 0) {
				continue;
			}
		}
		
#ifdef CT_COM_DBUG
		tcdbg_printf("%s: external ip address is %s\n", __FUNCTION__, externalIP);
#endif

		if ( (0 != strcmp(externalIP, "0.0.0.0")) && (0 != strcmp(externalIP, lanIP)) ) {  
	        valid = 1;
	        break;
		}
		if(TCSUPPORT_CT_PPP_ONDEMAND_VAL)
			checkCanConnectDemandWan(wan_node, nodeName);
	}
	if(TCSUPPORT_CT_INFORM_NODE_VAL){
		if(flag == 1 && i ==  MAX_WAN_INFO_ENTRY_NUM){
			tcapi_set("Cwmp_Entry","Inform_Status","2");       //wan connect is not available
		}
		else if( i ==  MAX_WAN_INFO_ENTRY_NUM){
			tcapi_set("Cwmp_Entry","Inform_Status","1");       //wan connect no tr69 route
		}
	}

#ifndef CODE_SHRINK  //qingpu.20080110
	if((getCwmpLanTestFlag() || valid) && initNotifyListFlag == 0){
#else
	if( valid && initNotifyListFlag == 0){
#endif
			
#if defined(TCSUPPORT_CT)
		cwmpSearchChangedValue(Passive);
		cwmpSearchChangedValue(Active);
		cwmpClearNotifyNodeList(Passive);
		cwmpClearNotifyNodeList(Active);
#endif
		cwmpInitNotifyNodeList();     
		initNotifyListFlag = 1;
	}
}
#ifdef REDIRECT_WLAN_WEB
#ifdef WLAN_RT2561_SUPPORT
Unsigned32  acsHost;
uint32 getAcsHost(){
	return acsHost;
}
#endif
#endif
#if defined(TCSUPPORT_CT)
static char acs_url_for_route[128];
char cwmp_interface[16];
/*_____________________________________________________________________________
**      function name: del_tr069_route
**      descriptions:
**			 delete tr069 route.
**
**      parameters:
**            void
**
**      global:
**             None
**
**      return:
**             0: success
**			   -1: fail
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
void del_tr069_route(void)
{
	FILE *fp = NULL;
	char line[128];

	fp = fopen(TR069_ROUTE_CONF, "r");
	if (NULL == fp) {
		return;
	}

	memset(line, 0, sizeof(line));
	while (fgets(line, sizeof(line), fp)) {
		if ('\0' != line[0]) {
		#ifdef WAN_DEBUG
			tcdbg_printf("%s: line is %s\n", __FUNCTION__, line);
		#endif
			system(line);
		}
	}

	fclose(fp);
	return;
}

/*_____________________________________________________________________________
**      function name: update_route_conf
**      descriptions:
**			 update route for tr069 app.
**
**      parameters:
**            acsHostAddress: 	the acs ip address.
**
**      global:
**             None
**
**      return:
**             0: success
**			   -1: fail
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
int update_cwmp_route(uint32 acsHostAddress)
{
	char cwmp_node[34], new_acs_url[128];
	char cwmp_route_node[34];
	char acs_ip[32];
	FILE *fp = NULL;
	char route_cmd[128];
	char route_num[10], if_name_attr[10], if_gateway_attr[20], if_status_attr[10], 
		 if_name[10], if_gateway[20], if_status[10];
	int  ret, i, acs_url_change = 0, cwmp_if_change = 0;

#ifdef CT_COM_DBUG
	tcdbg_printf("%s:acs url for route is %s.\n", __FUNCTION__, acs_url_for_route);
#endif

	/* if acs url is invalid, build inform failed */
	if (0 == acsHostAddress) {
#ifdef CT_COM_DBUG
	tcdbg_printf("%s:acs ip address is zero\n", __FUNCTION__);
#endif
		if(TCSUPPORT_CT_INFORM_NODE_VAL)
	        tcapi_set("Cwmp_Entry", "Inform_Status","5");   //ACL URL is invalid

		return -1;
	}

	/* get route num from cwmpRoute node */
	strcpy(cwmp_route_node, "CwmpRoute_Entry");
	memset(route_num, 0, sizeof(route_num));
	ret = tcapi_get(cwmp_route_node, "RouteNum", route_num);
	if (ret < 0) {
		return -1;
	}
	/* if there is no tr069 route */
	if (0 >= atoi(route_num)) {
		if(TCSUPPORT_CT_INFORM_NODE_VAL)
			tcapi_set("Cwmp_Entry", "Inform_Status","1");   //No tr69 route

		return -1;
	}
	
	/* check if acs url has been changed */
	memset(new_acs_url, 0, sizeof(new_acs_url));
	strcpy(cwmp_node, "Cwmp_Entry");
	tcapi_get(cwmp_node, "acsUrl", new_acs_url);
	if (0 != strcmp(acs_url_for_route, new_acs_url)) {
		acs_url_change = 1;
	}

	/* check if cwmp route interface changed */
	for (i = 0; i < atoi(route_num); i++) {
		sprintf(if_name_attr, "ifName%d", i);
		sprintf(if_status_attr, "ifStatus%d", i);
		
		memset(if_name, 0, sizeof(if_name));
		tcapi_get(cwmp_route_node, if_name_attr, if_name);
		tcapi_get(cwmp_route_node, if_status_attr, if_status);

		if (!strcmp(cwmp_interface, if_name)) {
			break;
		}
	}
	if (i == atoi(route_num)) {
		cwmp_if_change = 1;
	}

	else if (!strcmp(if_status, "new")) {
		cwmp_if_change = 1;
		tcapi_set(cwmp_route_node, if_status_attr, "old");
	}
	
	/* if acs url not change, cwmp route interface not change */
	if (0 == acs_url_change && 0 == cwmp_if_change) {
		return 0;
	}

#if 0
	/* if interface change, route will delete automaticlly */
	if (1 == acs_url_change) {
		/* delete old route for tr069 */
		del_tr069_route();
	}
	
	fp = fopen(TR069_ROUTE_CONF, "w");
	if (NULL == fp) {
		return -1;
	}
#endif

	/* get acs ip address */
	strcpy(acs_ip, (char*)inet_ntoa(acsHostAddress));
#ifdef CT_COM_DBUG
	tcdbg_printf("%s:acs ip address is %s\n", __FUNCTION__, acs_ip);
#endif

	/* only add one route for tr06 app */
	memset(if_name, 0, sizeof(if_name));
	memset(if_gateway, 0, sizeof(if_gateway));

	tcapi_get(cwmp_route_node, "ifName0", if_name);
	tcapi_get(cwmp_route_node, "ifGateway0", if_gateway);
#if 0
	if ('p' == if_name[0]) {
		sprintf(route_cmd, "/sbin/route add -net %s netmask 255.255.255.255 dev %s", acs_ip, if_name);
		system(route_cmd);

		/* update tr069 route to route config file */
		sprintf(route_cmd, "/sbin/route del -net %s netmask 255.255.255.255 dev %s 2>/dev/null", acs_ip, if_name);
		fputs(route_cmd, fp);
	}
	else if ('n' == if_name[0]) {
		sprintf(route_cmd, "/sbin/route add -net %s netmask 255.255.255.255 gw %s dev %s", acs_ip, if_gateway, if_name);
	#ifdef CT_COM_DBUG
		tcdbg_printf("%s: route cmd is %s.\n", __FUNCTION__, route_cmd);
	#endif
		system(route_cmd);

		/* update tr069 route to route config file */
		sprintf(route_cmd, "/sbin/route del -net %s netmask 255.255.255.255 gw %s dev %s 2>/dev/null", acs_ip, if_gateway, if_name);
		fputs(route_cmd, fp);
	}
	else {
		/* it will come to normally */
		fclose(fp);
		return -1;
	}
	
	fclose(fp);
#endif
	/* update cwmp route interface */
	strcpy(cwmp_interface, if_name);
	
	/* update acs url */
	strcpy(acs_url_for_route, new_acs_url);
	
	return 0;
}

/*_____________________________________________________________________________
**      function name: update_qos_rule
**      descriptions:
**			 update qos rule.
**
**      parameters:
**             void
**
**      global:
**             None
**
**      return:
**             return
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100828
**____________________________________________________________________________
*/
static char acs_url_for_qos[128];
void update_qos_rule(void)
{
	char node_name[34],enable[10], app_name[20], node_common[34];
	char cwmp_node[34], new_acs_url[128];
	char web_commit[4];
	int i;
	
	/* check if QoS is active */
	memset(node_name, 0, sizeof(node_name));
	memset(enable, 0, sizeof(enable));
	strcpy(node_name, "QoS_Common");
	
	tcapi_get(node_name, "Active", enable);
	if (0 != strcmp(enable, "Yes")) {
		/* reset qos url */
		memset(acs_url_for_qos, 0, sizeof(acs_url_for_qos));
		return;
	}

	/* get if web page change tr069 queue priority */
	memset(web_commit, 0, sizeof(web_commit));
	tcapi_get(node_name, "webCommitFlag", web_commit);
	if (!strcmp(web_commit, "1")) {
		tcapi_set(node_name, "webCommitFlag", "0");
		goto Handle_commit;
	}

	/* check if acs url has been changed */
	memset(new_acs_url, 0, sizeof(new_acs_url));
	strcpy(cwmp_node, "Cwmp_Entry");
	tcapi_get(cwmp_node, "acsUrl", new_acs_url);
	if (!strcmp(acs_url_for_qos, new_acs_url)) {
		return;
	}
Handle_commit:
	/* we only support four application */
	memset(app_name, 0, sizeof(app_name));
	for (i = 0; i < MAX_APP_NUM; i++) {
		sprintf(node_name, "QoS_Entry%d", i + MAX_APP_OFFSET);
		tcapi_get(node_name, "AppName", app_name);

		if (!strcmp(app_name, "TR069")) {
			strcpy(node_common, "QoS_Common");
			tcapi_set(node_common, "QoSOptType", "appRule");
			tcapi_set(node_common, "cwmpCommitFlag", "1");
			tcapi_commit(node_name);
		}
	}

	/* update acs url for qos */
	strcpy(acs_url_for_qos, new_acs_url);
	return;
}

/*_____________________________________________________________________________
**      function name: isPPPConnectionExist
**      descriptions:
**			 check if there is a vaild ppp connection with route mode
**
**      parameters:
**             void
**
**      global:
**             None
**
**      return:
**             return
**
**      call:
**
**      revision:
**      
**____________________________________________________________________________
*/
int isPPPConnectionExist(void)
{
	int if_index = 0;
	char wan_node[32];
	char isp[32], service_app[SERVICE_LIST_LEN], active[8];
	int pvc_index, entry_index, ret;

	if(TCSUPPORT_CT_PON_GD_VAL){
		for(if_index = 0; if_index < 64; if_index++)
		{
			pvc_index = if_index / MAX_PVC_NUM;
			entry_index = if_index - pvc_index * MAX_PVC_NUM;

			sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);

			/* only for active connection */
			ret = tcapi_get(wan_node, "Active", active);
			if (ret < 0)
			{
				continue;
			}
			if (0 != strcmp(active, "Yes"))
			{
				continue;
			}
			
			/* only for ppp connetion with route mode */
			memset(isp, 0, sizeof(isp));
			ret = tcapi_get(wan_node, "ISP", isp);
			if (ret < 0) 
			{
				continue;
			}
			if (0 != strcmp(isp, "2"))
			{
				continue;
			}

			/* only for INTERNET type connection */
			memset(service_app, 0, sizeof(service_app));
			ret = tcapi_get(wan_node, "ServiceList", service_app);
			if (ret < 0) 
			{
				continue;	
			}
			if (NULL == strstr(service_app, INTERNET))
			{
				continue;
			}
			else
			{
				break;         // found
			}
				
		}
		
		if(if_index < 64)
		{
			return 1;
		}
	}
	return 0;
}

/*_____________________________________________________________________________
**      function name: get_ppp_username
**      descriptions:
**			 get ppp user name 
**
**      parameters:
**             void
**
**      global:
**             None
**
**      return:
**             return
**
**      call:
**
**      revision:
**      1. xyzhu_nj 20100902
**____________________________________________________________________________
*/
int get_ppp_username(int if_index,char *conn_name, char *user_name)
{
	char wan_node[32];
	char isp[32], service_app[SERVICE_LIST_LEN];
	uint32 pvc_index, entry_index;
	int ret;
	char waninfo_node[20];
	char wanif_status[10];
	
	if (NULL == conn_name || NULL == user_name) {
		return -1;
	}

	if (if_index < 0 || if_index > 63) {
		return -1;
	}

	pvc_index = if_index / MAX_PVC_NUM;
	entry_index = if_index - pvc_index * MAX_PVC_NUM;

	sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);

	memset(isp, 0, sizeof(isp));
	ret = tcapi_get(wan_node, "ISP", isp);
	if (ret < 0) {
		return -1;
	}

	/* only for ppp connetion */
	if (0 != strcmp(isp, "2")) {
#ifdef CT_COM_DBUG
		tcdbg_printf("%s:It's not ppp connection.\n", __FUNCTION__);
#endif
		return -1;
	}

	/* only for INTERNET type connection */
	memset(service_app, 0, sizeof(service_app));
	ret = tcapi_get(wan_node, "ServiceList", service_app);
	if (ret < 0) {
		return -1;	
	}

	if (NULL == strstr(service_app, INTERNET)) {
#ifdef CT_COM_DBUG
		tcdbg_printf("%s:It's not internet connection.\n", __FUNCTION__);
#endif
		return -1;
	}
	
	if(TCSUPPORT_CT_PPPINFORM_VAL){
	sprintf(waninfo_node, "WanInfo_Entry%d", if_index);
	memset(wanif_status, 0, sizeof(wanif_status));
	ret = tcapi_get(waninfo_node, "Status", wanif_status);
	if (ret < 0) {
		return -1;	
	}
	if(0 != strcmp(wanif_status, "up"))
		return -1;
	}
	
	/* get ppp user name */
	ret = tcapi_get(wan_node, "USERNAME", user_name);
	if (ret < 0) {
		return -1;
	}
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		GetWanNodeMapping(&pvc_index, &entry_index);
		sprintf(conn_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.Username", pvc_index, entry_index);
	}
	else
		sprintf(conn_name, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.Username", pvc_index + 1, entry_index + 1);

	return 0;
}

int get_ppp_connectiontype(int if_index, char *conn_type, char *type_name){
	if(TCSUPPORT_CT_PPPINFORM_VAL){
	char wan_node[32];
	char isp[32], service_app[SERVICE_LIST_LEN],bridgeType[10];
	uint32 pvc_index, entry_index;
	int ret;
		
	if (NULL == conn_type) {
		return -1;
	}
	
	if (if_index < 0 || if_index > 63) {
		return -1;
	}
	
	pvc_index = if_index / MAX_PVC_NUM;
	entry_index = if_index - pvc_index * MAX_PVC_NUM;
	
	sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
	
	memset(isp, 0, sizeof(isp));
	ret = tcapi_get(wan_node, "ISP", isp);
	if (ret < 0) {
		return -1;
	}
	
	/* only for INTERNET type connection */
	memset(service_app, 0, sizeof(service_app));
	ret = tcapi_get(wan_node, "ServiceList", service_app);
	if (ret < 0) {
		return -1;	
	}	
	if (NULL == strstr(service_app, INTERNET)) {
		return -1;
	}

	/*check interface isp*/
	if(!strcmp(isp, WAN_ENCAP_DYN) || !strcmp(isp, WAN_ENCAP_STATIC) || !strcmp(isp, WAN_ENCAP_PPP)){
		strcpy(type_name, "IP_Routed");
	}
	else if(!strcmp(isp, WAN_ENCAP_BRIDGE)){
		memset(bridgeType, 0, sizeof(bridgeType));
		ret = tcapi_get(wan_node, "BridgeMode", bridgeType);
		if (ret < 0) {
		return -1;
		}
		if(!strcmp(bridgeType, "IP_Bridged")) {
			strcpy(type_name, "IP_Bridged");
		}
		else if(!strcmp(bridgeType, "PPPoE_Bridged")) {
			strcpy(type_name, "PPPoE_Bridged");
		}
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
	
	/* get ppp connection type */
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		GetWanNodeMapping(&pvc_index, &entry_index);
		sprintf(conn_type, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.ConnectionType", pvc_index, entry_index);
	}
	else
		sprintf(conn_type, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.ConnectionType", pvc_index + 1, entry_index + 1);

	return 0;
}
}

#endif

int setPreDslOpmode(){
	char Opmode[32] = {0};

	if(TCSUPPORT_CT_WAN_PTM_VAL){	
		if(tcapi_get("Info_Adsl", "Opmode", Opmode) != 0)
		{
			strcpy(Opmode,"");
		}

		tcapi_set("Adsl_Entry","OldOpmode",Opmode);
	}
	return 0;
}

char gDslOpmode[32] = {0};
char* getDslOpmode(void)
{
	if(TCSUPPORT_CT_WAN_PTM_VAL)
		getModulationType(gDslOpmode,sizeof(gDslOpmode));
	return gDslOpmode;
}

static char wanipaddr_old[32]="0.0.0.0";
static char accessType_old[64] = {0};
/*______________________________________________________________________________
**	checkNameChange
**
**	descriptions:
**		To check the parameter value is changed or not.
**
**	parameters:
**		void
**
**	return:
**		Changed: 	1
**		No change: 	0
**____________________________________________________________________________
*/
uint8 checkNameChange(void){
	int isChange =0;
	int i = 0;
	int ret = 0;
	char connection_name[128] = {0};
	char accessType[64] = {0};
	char nodeName[64] = {0};

	if(TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_NAMECHGNOTIFY_VAL){	
		if(strcmp("0.0.0.0",wanipaddr_old) == 0)
			strcpy(wanipaddr_old,wanipaddr);
		if (strcmp(wanipaddr_old, wanipaddr) != 0){
			strcpy(wanipaddr_old, wanipaddr);
			isChange = 1;
		}	
		
		getWANAccessType(accessType);
		if('\0' == accessType_old[0])
			strcpy(accessType_old, accessType);
		if (strcmp(accessType_old, accessType) != 0){
			strcpy(accessType_old, accessType);
			isChange = 1;
		}

		if(isChange == 1)
			return 1;
	}
	return 0;
}

/*_____________________________________________________________________________
**      function name: getInformValue
**      descriptions:
**            Get inform value.
**
**      parameters:
**             p_MonitorCollector_Enable: 	Enable value of MonitorCollector node.
**             p_Wan_IPMode: 				IPMode value of Wan node.
**             p_Wan_ConnectionType: 		ConnectionType value of Wan node.
**             p_Voice_ServerType: 			ServerType value of Voice node.
**             p_WLan_Enable: 				Enable value of WLan node.
**             pvcid: 						Internet pvc id.
**             entryid: 					Internet entry id.
**
**      return:
**            None
**____________________________________________________________________________
*/
void getInformValue(char *p_MonitorCollector_Enable,
	char p_Wan_IPMode[][4], char p_Wan_ConnectionType[][16],
	char p_Voice_ServerType[][8], char p_WLan_Enable[][4])
{
	int if_index = 0;
	char nodeName[32] = {0};
	char isp[8] = {0}, service_app[SERVICE_LIST_LEN] = {0}, active[8] = {0};
	int pvc_index = 0, entry_index = 0, ret = 0;
	char attrValue[64] = {0};
	int i = 0;
	int voipLineNum = 1;
	int ssidNum = 4;

	if ( TCSUPPORT_CT_PON_GDV20_VAL ){
		strcpy(nodeName,"MonitorCollector_Common");
		memset(attrValue, 0, sizeof(attrValue));
		if(tcapi_get(nodeName, "Enable", attrValue) == 0 ){			
			strcpy(p_MonitorCollector_Enable, attrValue);
		}else{
			strcpy(p_MonitorCollector_Enable,"");
		}

		for(if_index = 0; if_index < 64; if_index++)
		{
			strcpy(p_Wan_IPMode[if_index],"");
			strcpy(p_Wan_ConnectionType[if_index],"");
			pvc_index = if_index / MAX_PVC_NUM;
			entry_index = if_index - pvc_index * MAX_PVC_NUM;	
			sprintf(nodeName, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
	
			/* only for active connection */
			ret = tcapi_get(nodeName, "Active", active);
			if (ret < 0)
			{
				continue;
			}
			if (0 != strcmp(active, "Yes"))
			{
				continue;
			}			
			/* only for ppp connetion with route mode */
			memset(isp, 0, sizeof(isp));
			ret = tcapi_get(nodeName, "ISP", isp);
			if (ret < 0) 
			{
				continue;
			}
			if (0 != strcmp(isp, WAN_ENCAP_PPP))
			{
				continue;
			}	
			/* only for INTERNET type connection */
			memset(service_app, 0, sizeof(service_app));
			ret = tcapi_get(nodeName, "ServiceList", service_app);
			if (ret < 0) 
			{
				continue;	
			}
			if (NULL == strstr(service_app, INTERNET))
			{
				continue;
			}
			else
			{
				memset(attrValue, 0, sizeof(attrValue));
				if(tcapi_get(nodeName, "IPVERSION", attrValue) ==0){	
					if ( 0 == strcmp(attrValue, "IPv4") )
						strcpy(p_Wan_IPMode[if_index], "1");
					else if ( 0 == strcmp(attrValue, "IPv6") )
						strcpy(p_Wan_IPMode[if_index], "2");
					else if ( 0 == strcmp(attrValue, "IPv4/IPv6") )
						strcpy(p_Wan_IPMode[if_index], "3");
					else
						strcpy(p_Wan_IPMode[if_index], "1");					
				}			
				if(strcmp(isp, WAN_ENCAP_DYN)==0 || strcmp(isp, WAN_ENCAP_STATIC)==0 ||
					strcmp(isp, WAN_ENCAP_PPP)==0){
					strcpy(p_Wan_ConnectionType[if_index], "IP_Routed");
					tcapi_get(nodeName, "BridgeInterface", attrValue);
					if ( 0 == strcmp("Yes", attrValue) )
						strcpy(p_Wan_ConnectionType[if_index], "PPPoE_Hybrid");

				}
				else if(strcmp(isp, WAN_ENCAP_BRIDGE)==0){
					memset(attrValue, 0, sizeof(attrValue));
					if(tcapi_get(nodeName, "BridgeMode", attrValue) == 0){	
						if(strcmp(attrValue, "IP_Bridged")==0) {
							strcpy(p_Wan_ConnectionType[if_index], "IP_Bridged");
						}
						else if(strcmp(attrValue, "PPPoE_Bridged")==0) {
							strcpy(p_Wan_ConnectionType[if_index], "PPPoE_Bridged");
						}
					}	
				}
			}				
		}

		for (i=0; i < 2; i++){
			strcpy(p_Voice_ServerType[i],"");
		}
		if(TCSUPPORT_VOIP_VAL){
			strcpy(nodeName,"VoIPBasic_Common");
			memset(attrValue, 0, sizeof(attrValue));
			if(tcapi_get(nodeName, "VoIPLine2Enable", attrValue)==0){
				if(strcmp(attrValue,"Yes") == 0)
				{
					voipLineNum = 2;
				}
			}
			for (i=0; i < voipLineNum; i++){			
				strcpy(nodeName,"VoIPBasic_Common");
				memset(attrValue, 0, sizeof(attrValue));
				if(tcapi_get(nodeName, "SIPProtocol", attrValue)==0){
					if(strcmp(attrValue,"IMSSIP")==0){
						strcpy(p_Voice_ServerType[i],"0");
					}else if(strcmp(attrValue,"SIP")==0){
						strcpy(p_Voice_ServerType[i],"1");
					}else if(strcmp(attrValue,"H.248")==0){
						strcpy(p_Voice_ServerType[i],"2");
					}
				}
			}
		}

		for (i=0; i < 4; i++){
			strcpy(p_WLan_Enable[i],"");
		}
		if(TCSUPPORT_WLAN_VAL){
			strcpy(nodeName, "WLan_Common");
			memset(attrValue, 0, sizeof(attrValue));
			if(tcapi_get(nodeName, "BssidNum", attrValue)==0 ){			
				ssidNum = atoi(attrValue);
			}
			for (i=0; i < ssidNum; i++){
				#if 0
				sprintf(nodeName, "WLan_Entry%d", i);
				memset(attrValue, 0, sizeof(attrValue));
				if(tcapi_get(nodeName, "EnableSSID", attrValue)==0 ){			
					strcpy(p_WLan_Enable[i], attrValue);
				}	
				#else
				//tcapi_get("WLan_Common","APOn",attrValue);
				//strcpy(p_WLan_Enable[i], attrValue);
				sprintf(nodeName, "WLan_Common");
				memset(attrValue, 0, sizeof(attrValue));
				if(tcapi_get(nodeName, "APOn", attrValue)==0 ){			
					strcpy(p_WLan_Enable[i], attrValue);
				}	
				#endif
			}
		}
	}
}

/*_____________________________________________________________________________
**      function name: cwmpBuildInformValueChangeParameters
**      descriptions:
**            Build inform value change parameters.
**
**      parameters:
**            None
**
**      return:
**            0 	: Success
**            -1 	: Fail
**____________________________________________________________________________
*/
int8 cwmpBuildInformValueChangeParameters(void){
	char l_MonitorCollector_Enable[4] = {0};
	char l_Wan_IPMode[64][4] = {0};
	char l_Wan_ConnectionType[64][16] = {0};
	char l_Voice_ServerType[2][8] = {0};
	char l_WLan_Enable[4][4] = {0};
	int8 state = 0;
	int pvcid = 0, entryid = 0;
	char cwmpNodeName[128] = {0};
	int i = 0;

	if ( TCSUPPORT_CT_PON_GDV20_VAL ){
		getInformValue(l_MonitorCollector_Enable, l_Wan_IPMode, l_Wan_ConnectionType, l_Voice_ServerType, l_WLan_Enable);
		state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_MonitorCollector.Enable", l_MonitorCollector_Enable, BooleanType);
		if (state!=0)
			return -1;
		for(i = 0; i < 64; i++)
		{
			if(strlen(l_Wan_IPMode[i])>0){
				pvcid = i / MAX_PVC_NUM;
				entryid = i - pvcid * MAX_PVC_NUM;
				sprintf(cwmpNodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.X_CT-COM_IPMode", pvcid+1, entryid+1);
				state = cwmpAddInformParameter(cwmpNodeName, l_Wan_IPMode[i], UnsignedIntType);
				if (state!=0)
					return -1;
				sprintf(cwmpNodeName, "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.%d.ConnectionType", pvcid+1, entryid+1);
				state = cwmpAddInformParameter(cwmpNodeName, l_Wan_ConnectionType[i], StringType);
				if (state!=0)
					return -1;
			}
		}
		if(TCSUPPORT_VOIP_VAL){
			for(i = 0; i < 2; i++)
			{
				if(strlen(l_Voice_ServerType[i])>0){
					sprintf(cwmpNodeName, "InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.%d.X_CT-COM_ServerType", i+1);
					state = cwmpAddInformParameter(cwmpNodeName, l_Voice_ServerType[i], StringType);
					if (state!=0)
						return -1;
				}
			}
		}
		if(TCSUPPORT_WLAN_VAL){
			for(i = 0; i < 4; i++)
			{
				if(strlen(l_WLan_Enable[i])>0){
					#if  1
					sprintf(cwmpNodeName, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d.X_CT-COM_APModuleEnable", i+1);
					#else
					sprintf(cwmpNodeName, "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%d.Enable", i+1);
					#endif
					state = cwmpAddInformParameter(cwmpNodeName, l_WLan_Enable[i], BooleanType);
					if (state!=0)
						return -1;
				}
			}
		}
	}

	return 0;
}

int cwmpBuildInformPkt(void){
	int8 status = 0;
	int8 activeValueChange = 0;
	int op_flag = 1;
	int8 activeBootStarp =0;
	int8 isNeedDslModeChange = 0;
	int8 isBootStarp =0;
	int8 isNeedBind2 = 0, loidReport = 0, tmp_st = 1,isManualInform = 0;
	uint8 theSessionIndex, theRequestIndex;
	char        acsHostName[64];
	Unsigned32  acsHostAddress;
	Unsigned16 acsPort;
	char        acsPath[kWcMaxPathLength];
	char        redirectacsHostName[64];
	Unsigned32  redirectacsHostAddress;
	Unsigned16 redirectacsPort;
	char        redirectacsPath[kWcMaxPathLength];
	char * 	conRequestURLPtr = NULL;
	EventStructPtr  informEventPtr = NULL;
	ParameterValueStructPtr informParameterPtr = NULL;
	ValueInfoPtr valueInfoPtr = NULL;
	char * name = NULL;
	int8 state = 0;
#ifdef BHARTI
	uint32 ii = 0;
#endif
	int needCheckDNS = 1;

#ifdef TR143
	char cmd[64] = {0};
#endif
	char softwareVersion[128] = {0};
	char hardwareVersion[128] = {0};
	int alarm_flag = 0;
	int ctdev_regflag = 0;
	char accessType[64] = {0};
#if defined(TCSUPPORT_CT)
	char connection_name[128], user_name[128];
	int ret, i;
#endif

        char connection_type[128], type_name[128];	
	int8 brPPPConnectionFlag = 0;

/* add by kunliu @ 20090624 */
#ifdef TELEFONICA_SPAIN_TR069
	char  LANCfgMACAddr_Telefonica[32];
	memset(LANCfgMACAddr_Telefonica , 0 , sizeof(LANCfgMACAddr_Telefonica) );
#endif

#if 1
	if(get_firmwareupgrade_flag())
	{
		tc_printf("\r\ncwmpBuildInformPkt: Firmware upgrade, we will wait for restart-----------------------------\n");
		return 0;
	
	}
#endif
	if(get_romfile_upLoad_flag())
	{
		tc_printf("\r\ncwmpBuildInformPkt: during romfile upLoad, we will wait for upload success-----------------------------\n");
		return 0;
	}

	//Add for dealing with HoldRequests header, by hqfeng, 2006.11.23
	if(getHoldRequestFlag()){
		cwmpFreeSoapMemory(rpc);
		rpc.informStruct.eventList = NULL;
		rpc.informStruct.parameterList = NULL;

		return 0;		
	}
	
	if(!TCSUPPORT_CT_INFORM_NODE_VAL){
		//add by brian
		if(!strcmp(getCwmpUrl(),""))
		{
			tc_printf("\r\n url is null,do not build inform!");
			return 0;
		}
	}
	
    cwmpParseUrl(acsHostName, &acsHostAddress, &acsPort, acsPath, 
                    redirectacsHostName, &redirectacsHostAddress,
                    &redirectacsPort, redirectacsPath);
    
#if defined(TCSUPPORT_CT)
	/* update route and qos rule for tr069 application */
	if (-1 == update_cwmp_route(acsHostAddress)) {
		goto InformError;
	}
	update_qos_rule();

	/* update current acshostaddress */
	tc_printf("\r\ncwmpBuildInformPkt,update hostaddress");
	if(TCSUPPORT_CMCC_VAL){
		processTheHostAddressExt(&acsHostAddress, &acsPort, acsPath, 0);
	}
	else{
		processTheHostAddress(&acsHostAddress, 0);
	}
#endif

	//if(TCSUPPORT_CUC_VAL && isCwmpUrlIP(acsHostName){
	if(TCSUPPORT_CUC_VAL){
		if(isCwmpUrlIP(acsHostName)){
			needCheckDNS = 0;
		}
	}

	if(TCSUPPORT_CT_INFORM_NODE_VAL){
		if(needCheckDNS && checkDNSExist()){
			tcapi_set("Cwmp_Entry", "Inform_Status","3");
			return 0; 
		}
	}

#ifdef REDIRECT_WLAN_WEB
#ifdef WLAN_RT2561_SUPPORT
	acsHost=acsHostAddress;
#endif
#endif
	//tc_printf("\r\nBuild Inform Packet: acsHostName is [%s],acsHostAddress is [%s],port is [%lu],path is [%s]\n",acsHostName,inet_ntoa(acsHostAddress),acsPort,acsPath);
	//Judge whether a request to ACS has already existed. //xzwang_060725
    if(-1 == cwmpGetACS(acsHostName, redirectacsHostName, &acsPort, &redirectacsPort))
    {
	cwmpGetACSFlag = 1;
        return 0;
    }
	else{
		cwmpGetACSFlag = 0;
		PrivACSInitiatedflag = 0;
	}
	//add inform event code. 
    if(getBootTrapFlag()){//Added for BootTrap.by hqfeng,2006.06.14
    	if(TCSUPPORT_C1_CUC_VAL){
			activeValueChange = 1;
			activeBootStarp = 1;
    	}
		if(TCSUPPORT_CT_WAN_PTM_VAL){
			isNeedDslModeChange = 1;
			isBootStarp = 1;
		}
		if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL || TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL)
			isNeedBind2 = 1;
		state = cwmpAddInformEvent("0 BOOTSTRAP", "");
		if (state!=0)
			 goto InformError;
				
		status = 1;
	}

	if(getBootFlag() == 1){
		if(TCSUPPORT_CT_WAN_PTM_VAL)
			isNeedDslModeChange = 1;
		if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL || TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL)
			isNeedBind2 = 1;
		if(TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_CZ_NX_VAL)
			getLoid();
		state = cwmpAddInformEvent("1 BOOT", "");
		if (state!=0)
		    goto InformError;	

		status = 1;
	}

	if ( TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_CZ_NX_VAL)
	{
		if ( (1 == isNeedBind2) && (0 == getDevregFlag())
			&& (1 != getLoidEmptyFlag()) )
			loidReport = 1;
		else if ( (0 == isNeedBind2)
			&& (1 == getLoidChangeFlag()) )
		{
			isNeedBind2 = 1;
			if ( (0 == getDevregFlag()) && (1 != getLoidEmptyFlag()) )
				loidReport = 1;

		}
	}

	if(getPeriodicFlag()){
	 	state = cwmpAddInformEvent("2 PERIODIC", "");		
		if (state!=0)
			goto InformError;

		status = 1;
	}

	if(getScheduleInformFlag() == 1){
	 	state = cwmpAddInformEvent("3 SCHEDULED", "");
		if (state!=0)
			 goto InformError;
		
	 	state = cwmpAddInformEvent("M ScheduleInform", GetCommandKey(M_ScheduleInform));
		if (state!=0)
			 goto InformError;

		status = 1;
	}
	
	if(getActiveNotifyFlag() || valueModify_Nofity || getforceInformFlag() == 1){
		op_flag = 1;
		if(TCSUPPORT_C1_CUC_VAL)
			op_flag = (!activeBootStarp);
		if(op_flag){
			state = cwmpAddInformEvent("4 VALUE CHANGE", "");
			if (state!=0)
				goto InformError;
			activeValueChange = 1;
			if(TCSUPPORT_CT_WAN_PTM_VAL && TCSUPPORT_C7_VAL)	
				isNeedDslModeChange = 1;
			status = 1;
		}
	}

	if(TCSUPPORT_CT_PPPINFORM_VAL){
		if(getRoutePPPFlag() && (activeValueChange == 0)){
			state = cwmpAddInformEvent("4 VALUE CHANGE", "");
			if (state!=0)
				goto InformError;
			activeValueChange = 1;
			if(TCSUPPORT_CT_WAN_PTM_VAL && TCSUPPORT_C7_VAL)	
				isNeedDslModeChange = 1;
			status = 1;
		}
	}
	
	op_flag = getACSInitiatedflag();
	if(CWMP_REDIRECT_VAL)
		op_flag = op_flag || GetCwmpRedirectConnReqFlag(); 
	if(op_flag){
//add for Rostelecom 5_28 session retry test, lishuang, 2016.02.02		
		if(TCSUPPORT_PON_ROSTELECOM_VAL)
		{
			connsentflag = 1;
		}
		if(!CWMP_REDIRECT_VAL){
			state = cwmpAddInformEvent("6 CONNECTION REQUEST", "");
			if (state!=0)
				 goto InformError;

	                setACSInitiatedflag(0);  //should not retry the event. xzwang_060912
			status = 1;
		}
		else{
			if(GetCwmpRedirectFlag()){
				state = cwmpAddInformEvent("6 CONNECTION REQUEST", "");
				if (state!=0)
				      goto InformError;

	                        setACSInitiatedflag(0);
				SetCwmpRedirectConnReqFlag(0);	
				status = 1;
			}
			else if(getACSInitiatedflag()){
				state = cwmpAddInformEvent("6 CONNECTION REQUEST", "");
				if (state!=0)
	                               goto InformError;
	                        
	                        setACSInitiatedflag(0);
				SetCwmpRedirectConnReqFlag(1);	
				status = 1;
			}
		}	
	}

	if( (GetFileTransferCompleteFlag() & EVENT_TransferComplete) != 0 ){  /* download successfully and it's delayed download */
#ifdef SSL
		set_ssl_check_mem_flag(0); //reset the flag
#endif
//		if(GetFileTransferTypeFlag() == 1){ //other type will send this evnet too.
			state = cwmpAddInformEvent("7 TRANSFER COMPLETE", "");
			if (state!=0)
			 	goto InformError;
//              }	

		 if(rpc.fileTransferStruct->transferType == Download){
			if (TCSUPPORT_CT_PON_GD_VAL || TCSUPPORT_CT_PON_GX_VAL || TCSUPPORT_CT_PON_SN_VAL)
				 tcapi_set("VoIPAdvanced_Common", "FaxPassThruCodec", "PCMA");
		        state = cwmpAddInformEvent("M Download", GetCommandKey(M_Download));
			 if (state!=0)
			 	goto InformError;
			SetDownloadStart(0);
			SaveCwmpFlags();
		 }
		 else if(rpc.fileTransferStruct->transferType == Upload){
			 state = cwmpAddInformEvent("M Upload", GetCommandKey(M_Upload));
			 if (state!=0)
			 	goto InformError;
			 SetUploadStart(0);
			 SaveCwmpFlags();
		 }
		
		status = 1;
	}
		
	if(getCwmpRebootFlag(NULL)){//added for Reboot,by hqfeng,2006.06.09	
		state = cwmpAddInformEvent("M Reboot", GetCommandKey(M_Reboot));
		if (state!=0)
			 goto InformError;
		
		status = 1;
	}

	 //Inform passeive value change.
	if(cwmpSearchChangedValue(Passive) == 1){
		if(!activeValueChange){
			state = cwmpAddInformEvent("4 VALUE CHANGE", "");
			if (state!=0)
			 	goto InformError;
			if(TCSUPPORT_CT_PON_GDV20_VAL)
				activeValueChange = 1;
			if(TCSUPPORT_CT_WAN_PTM_VAL && TCSUPPORT_C7_VAL)	
				isNeedDslModeChange = 1;
		}
		status = 1;
	}	

	/*Added by hqfeng, 2006.10.23*/
	if(getIPPingCompleteFlag() == 1){
#ifdef TR143
		if(rpc.loadDiagInfo.RTAddFlag == 1){ //delete the route
	#if 0
			if(sp_ip_route_drop(rpc.loadDiagInfo.transferIPAddress, cnt_mask_bits(aton("255.255.255.255")), 0)==0)
				rpc.loadDiagInfo.RTAddFlag = 0;
			else
				tc_printf("\r\n [cwmpBuildInformPkt] drop route failed!");
		}
#endif
			sprintf( cmd, "route delete %s", inet_ntoa(rpc.loadDiagInfo.transferIPAddress) );
			system( cmd );

			//tcdbg_printf("==> cwmpBuildInformPkt() delete route.\n");
			
			rpc.loadDiagInfo.RTAddFlag = 0;
		}
#endif
		state = cwmpAddInformEvent("8 DIAGNOSTICS COMPLETE", "");
		if (state!=0)
			 goto InformError;
		
//		setIPPingCompleteFlag(0);
		status = 1;
	}
	if(CT_COM_DEVICEREG_VAL){
		if(/*getInitialDeviceRegisterFlag()*/getDevregInformFlag() == 1){
			if(TCSUPPORT_CT_WAN_PTM_VAL)	
				isNeedDslModeChange = 1;
			if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL){
				state = cwmpAddInformEvent("X CU BIND", "");
			}else{
				state = cwmpAddInformEvent(X_EVENT_CODE("BIND"), "");
			}
			if (state!=0)
				 goto InformError;
			
			status = 1;
		}
	}

	if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL || TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL){
		if (isNeedBind2 == 1) {
			state = cwmpAddInformEvent(X_EVENT_CODE("BIND2"), "");
			if (state!=0)
				 goto InformError;
			
			status = 1;
		}
	}

	// for CT Guangdong, if there is a route ppp connection, should report bind1
	if(TCSUPPORT_CT_PON_GD_VAL && !TCSUPPORT_CT_PON_CZ_GDCS_VAL){
		if(isPPPConnectionExist())
		{
			state = cwmpAddInformEvent(X_EVENT_CODE("BIND 1"), "");
			if (state!=0)
			{
				 goto InformError;
			}
			
			status = 1;
		}
	}

	if(TCSUPPORT_CT_PPPINFORM_VAL){
		if(getBridgePPPFlag()) {
			brPPPConnectionFlag = 1;
			state = cwmpAddInformEvent(X_EVENT_CODE("BIND 1"), "");
			if (state!=0)
			 goto InformError;		
			status = 1;
		}
	}

	if(TCSUPPORT_CT_INFORM_NODE_VAL){
		if(getAccountChangeFlag()){
			if(TCSUPPORT_C1_CUC_VAL)
				state = cwmpAddInformEvent("X_CU_ADMINPASSWORDCHANGE", "");		
			else{
				if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL)
					state = cwmpAddInformEvent(X_EVENT_CODE("ADMINPASSWORDCHANGE"), "");
				else
			 		state = cwmpAddInformEvent(X_EVENT_CODE("ACCOUNTCHANGE"), "");	
			}
			if (state!=0){
				goto InformError;
			}
			status = 1;
		}
	}

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
			alarm_flag = ( getDeviceAlarmFlag() || getSimCardRstFailAlarmFlag() );
		else
			alarm_flag = getDeviceAlarmFlag();
		if ( alarm_flag )
		{
			state = cwmpAddInformEvent(X_EVENT_CODE("ALARM"), "");
			if (state!=0)
				 goto InformError;
			status = 1;	
		}

		if(getDeviceClearAlarmFlag()) {
			state = cwmpAddInformEvent(X_EVENT_CODE("CLEARALARM"), "");
			if (state!=0)
				 goto InformError;
			status = 1;	
		}	

		if(getDeviceMonitorFlag()) {
			state = cwmpAddInformEvent(X_EVENT_CODE("MONITOR"), "");
			if (state!=0)
				 goto InformError;
			status = 1;	
		}	
	}

	if(TCSUPPORT_CT_WAN_PTM_VAL){	
		if(isDslOpmodeChange()== 1 || isBootStarp == 1){		//the first inform after boot need contain Event code ""
				isNeedDslModeChange = 1;
				state = cwmpAddInformEvent(X_EVENT_CODE("DSLMODECHANGE"), "");
				if (state!=0)
				 	goto InformError;
			status = 1;
		}
	}

	if(TCSUPPORT_C1_CUC_VAL && TCSUPPORT_SYSLOG_VAL){
		if(getAlarmBuildFlag()) {
			state = cwmpAddInformEvent("M X_CU_ALARM", "");
			if (state!=0)
				 goto InformError;
			status = 1;	
		}
	}

	if(TCSUPPORT_CT_LONG_RESETBTN_VAL){
		if( 1 == getLongResetBtnFlag() )
		{
			state = cwmpAddInformEvent(X_EVENT_CODE("LONGRESET"), "");
			if ( state!=0 )
				 goto InformError;
			status = 1;
		}
	}

	if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && 1 == getCardNotifyFlag() )
	{
		state = cwmpAddInformEvent(X_EVENT_CODE("CARDNOTIFY"), "");
		if ( state!=0 )
			 goto InformError;
		status = 1;
	}

	if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && 1 == getCardWriteFlag() )
	{
		state = cwmpAddInformEvent(X_EVENT_CODE("CARDWRITE"), "");
		if ( state!=0 )
			 goto InformError;
		status = 1;
	}


	if(TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_NAMECHGNOTIFY_VAL){
		if(checkNameChange())
			setNameChangeFlag(1);
		if( 1 == getNameChangeFlag() )
		{
			state = cwmpAddInformEvent(X_EVENT_CODE("NAME CHANGE"), "");
			if ( state!=0 )
				 goto InformError;
			status = 1;
		}
	}

	if( TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_STBMAC_REPORT_VAL )
	{
		if ( 1 == getSTBListNotifyFlag() )
		{
			state = cwmpAddInformEvent(X_EVENT_CODE("STBBIND"), "");
			if ( 0 != state )
				 goto InformError;
			status = 1;
		}
	}

	if ( TCSUPPORT_CT_ADSL_BIND1_VAL && 1 == getXBIND1NotifyFlag() )
	{
		state = cwmpAddInformEvent(X_EVENT_CODE("BIND1"), "");
		if ( 0 != state )
			 goto InformError;
		status = 1;
	}

	if ( TCSUPPORT_CT_JOYME_VAL && 1 == getDnsSpeedLimitFlag() )
	{
		state = cwmpAddInformEvent(X_EVENT_CODE("DNSLIMITALERT"), "");
		if ( 0 != state )
			 goto InformError;
		status = 1;
	}

	if ( TCSUPPORT_CUC_PON_SD_VAL && TCSUPPORT_CT_INFORM_NODE_VAL)
	{
		if(1 == getManualInformFlag())
			isManualInform = 1;
	}


	if(status == 0){
		if(CWMP_REDIRECT_VAL)
			SetCwmpRedirectFlag(0); //clear 302 redirect flag
		return 0;
	}	

	//reset the authenticationFlag, by hqfeng, 2006.03.20
	cwmpResetAuthenFlag();

	 /*Build inform argument*/
	#if 1//modify by brian
	cwmpSetDeviceID(getCwmpManufacture(), getCwmpOUI(), getCwmpProductClass(), getCwmpSerialNum());
	#else
	/*follew is for test*/
	cwmpSetDeviceID("trendchip", "0017A6", "DSL-GATEWAY-WIN847", "998877665543");
	#endif
	cwmpSetMaxEnvelopes(1);
	
	if(TCSUPPORT_CWMP_PRECISE_TIME_VAL)
		cwmpSetCurrentTime((char *)getCurrentTime());
	else{	
	#if 1
	cwmpSetCurrentTime((char *)getUpDateTime());
	#else
	/*follew is for test*/
	cwmpSetCurrentTime("0001-01-01T00:27:03");
	#endif
	}
        cwmpSetRetryCount(cwmpGetSessionRetryCount());//modified by hqfeng,2006.12.18	
	/* parameter list*/
	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceSummary", getDeviceSummary(), StringType);
	if (state!=0)
		goto InformError;	
        state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.SpecVersion", "1.0", StringType);
 	if (state!=0)
		goto InformError;
 
	//if(getFirmwareVersionFlag()){//customer's firmware version 
	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.HardwareVersion", getCwmpHardwareVersion(), StringType);
	if (state!=0)
		goto InformError;
	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.SoftwareVersion", getCwmpSoftwareVersion(), StringType);
	if (state!=0)
		goto InformError;

	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.ProvisioningCode", (char *)getProvisionCode(), StringType);
	if (state!=0)
		goto InformError;

	//modify by brian
	#if 1
	conRequestURLPtr = fetchConRequestURL_detail();
	if (strcmp(conRequestURLPtr, "0.0.0.0") == 0){
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\n we can not find a valid WAN IP of CPE!\r\n");
        		}
		goto InformError;
	}else{
		state = cwmpAddInformParameter("InternetGatewayDevice.ManagementServer.ConnectionRequestURL", conRequestURLPtr, StringType);
		if (state!=0)
			goto InformError;
	}
	#else
	//follow is for test
	state = cwmpAddInformParameter("InternetGatewayDevice.ManagementServer.ConnectionRequestURL", /*conRequestURLPtr*/"http://122.96.153.236:5555/tr069", StringType);
		if (state!=0)
			goto InformError;
	#endif
#if defined(TCSUPPORT_CT)
if(TCSUPPORT_CT_PPPINFORM_VAL && 1 == activeValueChange) {
	/* there are 64 interface at most */
	for (i = 0; i < 64; i++) {
		memset(connection_name, 0, sizeof(connection_name));
		memset(user_name, 0, sizeof(user_name));
		memset(connection_type, 0, sizeof(connection_type));		
		memset(type_name, 0, sizeof(type_name));
		ret = get_ppp_connectiontype(i, connection_type,type_name);
		if (0 == ret) {
			state = cwmpAddInformParameter(connection_type, type_name, StringType);
			if (state!=0)
				goto InformError;
		}
		ret = get_ppp_username(i, connection_name, user_name);
		if (0 == ret) {
			state = cwmpAddInformParameter(connection_name, user_name, StringType);
			if (state!=0)
				goto InformError;
		}
	}
}
else{
	for (i = 0; i < 64; i++) {
		memset(connection_name, 0, sizeof(connection_name));
		memset(user_name, 0, sizeof(user_name));
		ret = get_ppp_username(i, connection_name, user_name);
		if (0 == ret) {
			state = cwmpAddInformParameter(connection_name, user_name, StringType);
			if (state!=0)
				goto InformError;
		}
	}
}
#endif

	state = cwmpAddInformParameter("InternetGatewayDevice.ManagementServer.ParameterKey", (char *)getParameterKey(), StringType);
	if (state!=0)
		goto InformError;

	if (TCSUPPORT_CT_ECN_GZ_VAL || TCSUPPORT_CT_SIMCARD_SEPARATION_VAL || TCSUPPORT_CT_DEV_ACCESS_TYPE_VAL || TCSUPPORT_CT_PPPINFORM_VAL){
		state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.DeviceType", (char *)getECNDeviceType(), StringType);
		if (state!=0)
			 goto InformError;

		if(!TCSUPPORT_CT_PPPINFORM_VAL){
			state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.AccessType", (char *)getECNAccessType(), StringType);
			if (state!=0)
				 goto InformError;
		}
	}

//	cwmpAddInformParameter((char *)getWANExternalName(), (char *)getDefaultWANExternalIPAddr(), StringType);
	//modify by brian
	#if 1
	state = cwmpAddInformParameter((char *)getWANExternalName(wanindex), wanipaddr, StringType);
	#else
	state = cwmpAddInformParameter("InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANIPConnection.1.ExternalIPAddress", "122.96.153.236", StringType);
	#endif
	
	if (state!=0)
		goto InformError;

	if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL
		|| TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL
		|| TCSUPPORT_CT_ADSL_TJ_VAL ){
		state = cwmpAddInformParameter((char *)getWANMacName(wanindex), getCPEMac(wanindex), StringType);
		if (state != 0)
			goto InformError;
	}

	if ( TCSUPPORT_CT_ADSL_TJ_VAL )
	{
		memset(accessType,0,sizeof(accessType));
		getWANAccessType(accessType);

		state = cwmpAddInformParameter("InternetGatewayDevice.WANDevice"
			".1.WANCommonInterfaceConfig.WANAccessType", accessType, StringType);
		if ( 0 != state )
			goto InformError;
	}

#ifdef BHARTI
	for (ii=0; ii<8; ii++)
	{
		if (flagAddWanPPPoEUserName(ii))
		{
			state = cwmpAddInformParameter((char *)getWANPPPoEUserName(ii), pppoeUsername, StringType);
			if (state!=0)
				goto InformError;
		}
	}
#endif

/* add by kunliu @ 20090624 */
#ifdef TELEFONICA_SPAIN_TR069
	state = cwmpAddInformParameter("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1.MACAddress", (char *)getMACAddress(LANCfgMACAddr_Telefonica), StringType);
	if (state!=0)
		goto InformError;
#endif

	if(CT_COM_DEVICEREG_VAL){
		if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
			ctdev_regflag = ( (getDevregInformFlag() == 1) || (getBootFlag() == 1 && (isCardExist() == 1)) );
		else
			ctdev_regflag = (/*getInitialDeviceRegisterFlag() */getDevregInformFlag() == 1
			|| (1 == TCSUPPORT_CT_PON_YN_VAL && (strlen((char *)getDeviceregUserName()) > 0 || strlen((char *)getDeviceregUserPasswd()) > 0)));

		if ( ctdev_regflag )
		{
			if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL)
	        	state = cwmpAddInformParameter("InternetGatewayDevice.X_CU_UserInfo.UserName", (char *)getDeviceregUserName(), StringType);
			else if(TCSUPPORT_CMCC_VAL == 0)
				state = cwmpAddInformParameter("InternetGatewayDevice.X_CT-COM_UserInfo.UserName", (char *)getDeviceregUserName(), StringType);

			if (state!=0)
				 goto InformError;
			
			status = 1;
			if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL)
	        	state = cwmpAddInformParameter("InternetGatewayDevice.X_CU_UserInfo.UserId", (char *)getDeviceregUserPasswd(), StringType);
			else if(TCSUPPORT_CMCC_VAL)
				state = cwmpAddInformParameter("InternetGatewayDevice.X_CMCC_UserInfo.UserId", (char *)getDeviceregUserPasswd(), StringType);
			else
				state = cwmpAddInformParameter("InternetGatewayDevice.X_CT-COM_UserInfo.UserId", (char *)getDeviceregUserPasswd(), StringType);

			if (state!=0)
				 goto InformError;

			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && isCardExist() == 1){
			status = 1;
			state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_Cardmanage.CardNo",(char *)getSimCardCardNo() , StringType);
			if (state!=0)
				goto InformError;
			}

			GetITMSRegStartTime();
			status = 1;
			//setDevregInformFlag(0);
		}
		
		if (TCSUPPORT_CT_PON_GD_VAL && (ctdev_regflag == 0)) {
			state = cwmpAddInformParameter("InternetGatewayDevice.X_CT-COM_UserInfo.UserName", (char *)getDeviceregUserName(), StringType);
			if (state!=0)
				 goto InformError;
			state = cwmpAddInformParameter("InternetGatewayDevice.X_CT-COM_UserInfo.UserId", (char *)getDeviceregUserPasswd(), StringType);
			if (state!=0)
				 goto InformError;
		}
	}

	if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && 1 == getCardWriteFlag() )
	{
		state = cwmpBuildCardmanageParameters();
		if (state!=0)
			 goto InformError;
	}

	if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL || TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL || TCSUPPORT_CUC_PON_SD_VAL){
		if(TCSUPPORT_CT_PON_NMG_VAL && CT_COM_DEVICEREG_VAL)
			op_flag = (isNeedBind2 == 1 && getDevregInformFlag() != 1);
		else if(TCSUPPORT_CUC_PON_SD_VAL)
			op_flag = (isManualInform == 1);
		else
			op_flag = (isNeedBind2 == 1);
		if (op_flag)
		{
			if ( TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_CZ_NX_VAL)
			{
				tmp_st = loidReport;
			}

			if ( tmp_st )
			{
				if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL)
		        	state = cwmpAddInformParameter("InternetGatewayDevice.X_CU_UserInfo.UserName", (char *)getLoid(), StringType);
				else
					state = cwmpAddInformParameter("InternetGatewayDevice.X_CT-COM_UserInfo.UserName", (char *)getLoid(), StringType);
				if (state!=0)
					 goto InformError;
				
				status = 1;
			}
		}
	}

	if(TCSUPPORT_CT_UPNP_DM_VAL){
		if(getUPnPDMDownloadFlag()){
			state = cwmpBuildUPnPDMDownloadParameters();
			if (state!=0)
				 goto InformError;

		}
	}

	op_flag = 1;
	if(TCSUPPORT_C1_CUC_VAL)
		op_flag = (!activeBootStarp);
	if(op_flag){
		state = cwmpBuildValueChangeParameters();
	}
	if (state == -1) /* both 0 and 1 are normal case. -1 is error case. zhj 2008-9-10 */
		goto InformError;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
			alarm_flag = ( getDeviceAlarmFlag() || getDeviceClearAlarmFlag() || getSimCardRstFailAlarmFlag() );
		else
			alarm_flag = ( getDeviceAlarmFlag() || getDeviceClearAlarmFlag() );
		if ( alarm_flag )
		{
			state = cwmpBuildAlarmParameters();
			if (state == -1) 
				goto InformError;
		}
		
		if(getDeviceMonitorFlag()) {
			state = cwmpBuildMonitorParameters();
			if (state == -1) 
				goto InformError;
		}
	}
	if(TCSUPPORT_C1_CUC_VAL && TCSUPPORT_SYSLOG_VAL){
		if(getAlarmBuildFlag()) {			
			state = cwmpAddAlarmParameters();
			if (state == -1) 
				goto InformError;
		}
	}
	if(TCSUPPORT_CT_WAN_PTM_VAL){
		if(isNeedDslModeChange == 1){ 	//the first inform after boot need contain Event code ""
			state = cwmpAddInformParameter("InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.ModulationType", (char *)getDslOpmode(), StringType);
			if (state == -1) 
				goto InformError;
		}	
	}

	if(TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_NAMECHGNOTIFY_VAL){
		if(getBootFlag() == 1 ||getBootTrapFlag()==1 ||getNameChangeFlag()==1){
			state=cwmpBuildNameChangeParameters();
			if (state == -1) 
				goto InformError;
		}
	}

	if(TCSUPPORT_CT_PON_GDV20_VAL){
		if(getBootTrapFlag()==1 || getBootFlag()==1 || getPeriodicFlag()==1
			|| getCwmpRebootFlag(NULL) || activeValueChange){
			state=cwmpBuildInformValueChangeParameters();
			if (state == -1) 
				goto InformError;
		}
	}

	if ( TCSUPPORT_CT_PON_JS_VAL|| TCSUPPORT_CT_STBMAC_REPORT_VAL )
	{
		if ( 1 == getSTBListNotifyFlag() )
		{
			state = cwmpAddInformParameter("InternetGatewayDevice.Services.X_CT-COM_IPTV.STBMAC", getSTBMACList(), StringType);
			if ( 0 != state )
				 goto InformError;

			if ( TCSUPPORT_CT_PON_JX_VAL )
			{
				state = cwmpAddInformParameter("InternetGatewayDevice.Services.X_CT-COM_IPTV.STBNumber", getSTBCountStr(), UnsignedIntType);
				if ( 0 != state )
					 goto InformError;

				state = cwmpAddInformParameter("InternetGatewayDevice.Services.X_CT-COM_IPTV.IGMPEnable", getIGMPEnableStr(), BooleanType);
				if ( 0 != state )
					 goto InformError;

				state = cwmpAddInformParameter("InternetGatewayDevice.Services.X_CT-COM_IPTV.ProxyEnable", getProxyEnableStr(), BooleanType);
				if ( 0 != state )
					 goto InformError;

				state = cwmpAddInformParameter("InternetGatewayDevice.Services.X_CT-COM_IPTV.SnoopingEnable", getSnoopingEnableStr(), BooleanType);
				if ( 0 != state )
					 goto InformError;

			}
		}
	}
	if(TCSUPPORT_CT_PPPINFORM_VAL){
		if(1 == brPPPConnectionFlag) {
			state = cwmpAddInformParameter("InternetGatewayDevice.X_CT-COM_UserInfo.UserName", (char *)getBrpppusername(), StringType);       //(char *)getBrpppusername()
			if (state!=0)
				goto InformError;
		}
	}

	if ( TCSUPPORT_CT_ADSL_BIND1_VAL && 1 == getXBIND1NotifyFlag() )
	{
		state = cwmpBuildXBIND1Parameters();
		if ( 0 != state )
			 goto InformError;
	}

	if ( TCSUPPORT_CT_JOYME_VAL && 1 == getDnsSpeedLimitFlag() )
	{
		state = cwmpBuildDnsSpeedLimitParameters();
		if ( 0 != state )
			 goto InformError;
	}

	if(TCSUPPORT_CT_FJ_VAL)
	{
		state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_MACAddress", (char *)getDevMACAddress(), StringType);
		if (state!=0)
			goto InformError;
	}	

	if(getPeriodicFlag())
	{
		state = CwmpAddCustomInformParameter();
		if (state == -1) 
			goto InformError;
	}

	/*Build inform packet*/
       cwmpRpcInform(&rpc);

	return status;
InformError:

	tc_printf("\r\n Fail to build CWMP inform packet!");

	
#if 0//def CT_COM_DEVICEREG
	setDevregInformFlag(0);
#endif
	/* 
		reset other memebers of informStruct	
	*/
	rpc.informStruct.maxEnvelopes = 0;
	rpc.informStruct.retryCount = 0;

	memset(&rpc.informStruct.deviceId , 0, sizeof(DeviceIdStruct));
	memset(rpc.informStruct.currentTime, 0, sizeof(rpc.informStruct.currentTime));

	
	/* 
		free informStruct event list	
	*/
	informEventPtr = rpc.informStruct.eventList;

	while (informEventPtr != NULL)
	{
		informEventPtr = informEventPtr->next;
		os_free(rpc.informStruct.eventList);
		rpc.informStruct.eventList = informEventPtr;
	}


	/* 
		free informStruct parameter list
	*/	
	informParameterPtr =  rpc.informStruct.parameterList;
	
	while (informParameterPtr != NULL)
	{
		informParameterPtr = informParameterPtr->next;

		valueInfoPtr = rpc.informStruct.parameterList->valueInfoPtr;
		name = rpc.informStruct.parameterList->name;
		
		/* 
			free rpc.informStruct.parameterList->valueInfoPtr 
		*/
		if (valueInfoPtr!=NULL)
		{
			if (valueInfoPtr->value!=NULL)
			{
				os_free(valueInfoPtr->value);
				valueInfoPtr->value =  NULL;		
			}
			
			os_free(valueInfoPtr);
			valueInfoPtr = NULL;
			rpc.informStruct.parameterList->valueInfoPtr = NULL;
		}
		
		/* 
			free rpc.informStruct.parameterList->name 
		*/
		if (name!=NULL)
		{
			os_free(name);
			name = NULL;
			rpc.informStruct.parameterList->name = NULL;
		}
		
		os_free(rpc.informStruct.parameterList);

		rpc.informStruct.parameterList = informParameterPtr;		
	}

	return 0;			


}

void cwmpSetCurACSInfo(Unsigned32 theHostAddress, Unsigned16 thePort, char* thePath){
	rpc.acsCurInfo.theHostAddress = theHostAddress;
	rpc.acsCurInfo.thePort = thePort;
	strcpy(rpc.acsCurInfo.thePath, thePath);
}

void cwmpGetCurACSInfo(Unsigned32 *theHostAddressP, Unsigned16 *thePortP, char *thePath){
	*theHostAddressP = rpc.acsCurInfo.theHostAddress;
	*thePortP = rpc.acsCurInfo.thePort;
	strcpy(thePath, rpc.acsCurInfo.thePath);
}

//modify by brian
void processSessionRetry(void)
{
	int RetryWaitInterval = 0;
	int waitIntervalCounter;
	int retryCount;
#ifdef CWMP
	cwmpPeriodicInformTime();
#endif
	if(dbg_cwmp >= DBG_CWMP_L3 ){
		tc_printf("\r\ncwmp.c---[processSessionRetry]---Enter!");
	}

	if(cwmpSessionRetryCount == 0){
		if(dbg_cwmp >= DBG_CWMP_L3 ){
			tc_printf("\r\ncwmp.c---[processSessionRetry]---cwmpSessionRetryCount = 0");
		}
		return;
	}
	
	if(cwmpGetSessionRetryFlag()){
		if(dbg_cwmp >= DBG_CWMP_L3 ){
			tc_printf("\r\ncwmp.c---[processSessionRetry]---SessionRetryFlag = 1");
		}
		cwmpSessionRetryWaitIntervalCounter();
		waitIntervalCounter = cwmpGetSessionRetryWaitInterval();
	#if 0 //marked by xzwang_060815	
		if(cwmpGetBootInformFlag()){
			if(waitIntervalCounter == getInformPeriod()){
				cwmpResetSessionRetryWaitInterval();
				cwmpSetBootInformFlag(0);
			}
			return;			
		}
	#endif	
		retryCount = cwmpSessionRetryCount;  //merge steven's correction, modify "==" to be "=". xzwang_20070225
		RetryWaitInterval = cwmpCalculateRetryInterval(cwmpSessionRetryCount);
		if(dbg_cwmp >= DBG_CWMP_L3 ){
			tc_printf("\r\ncwmp.c---[processSessionRetry]---SessionRetryCount = %ld", cwmpSessionRetryCount);
			tc_printf("\r\ncwmp.c---[processSessionRetry]---waitIntervalCounter = %ld", waitIntervalCounter);
			tc_printf("\r\ncwmp.c---[processSessionRetry]---RetryWaitInterval = %ld", RetryWaitInterval);
		}
	
		if(waitIntervalCounter >= RetryWaitInterval){
			if(dbg_cwmp >= DBG_CWMP_L3 ){
				tc_printf("\r\ncwmp.c---[processSessionRetry]---Do Session Retry--- ");
			}
			cwmpResetSessionRetryWaitInterval();
			cwmpDoSessionRetry();
//			wsignal(cwmpSessionRetryTaskId);
		}
	}
	
}

void cwmpDoSessionRetry(void)
{
	int op_flag = 0;

	if(cwmpGetACSFlag == 1){
		setACSInitiatedflag(PrivACSInitiatedflag);
		cwmpGetACSFlag = 0;
		PrivACSInitiatedflag = 0;
	}
	op_flag = ( getBootFlag() == 0) 
	    && (getActiveNotifyFlag() == 0)
	    && (getBootTrapFlag() == 0)
	    && (getCwmpRebootFlag(NULL) == 0)
	    && ((GetFileTransferCompleteFlag() & EVENT_TransferComplete)== 0)
	    && (getACSInitiatedflag() == 0)
	    &&(getIPPingCompleteFlag() == 0)
	    &&(getScheduleInformFlag() == 0);

	if(TCSUPPORT_CT_INFORM_NODE_VAL)
	    op_flag = op_flag && (getAccountChangeFlag() == 0);
	    
	if(op_flag)
	{
		tc_printf("\r\nsession retry!!");
		setPeriodicFlag(1);
	}
		//modify by brian
	      sem_post(&SEM_IMFORM);
}

static int cwmpCalculateRetryInterval(int retryCount)
{
	int i;
	int retryInterval = 5;

	if(retryCount <= 0){
		return 0;
	}
	
	if (retryCount == 1){
		return retryInterval;
	}

	if(retryCount > 10){
		retryCount = 10;
	}
		
	for ( i = 0; i < retryCount - 1; i++){
		retryInterval = retryInterval * 2;
	}
	
	return retryInterval;
	
}

void cwmpResetInformFlag(void)
{
	if(getBootTrapFlag())
	{
		DelaySetBootTrapFlags = 1;
		//setBootTrapFlag(0);
	}

	setforceInformFlag(0);
	
	setBootFlag(0);
	setActiveNotifyFlag(0);
	if (!TCSUPPORT_CT_PON_C9_VAL)
	{
		setACSInitiatedflag(0);
	}
	setPeriodicFlag(0);
	setIPPingCompleteFlag(0);
	setScheduleInformFlag(0);//added by hqfeng,2007.01.08
	if(TCSUPPORT_CT_INFORM_NODE_VAL)
		setAccountChangeFlag(0);
	if(getCwmpRebootFlag(NULL)){
		
		DelayResetMBootFlags = 1;
		//setCwmpRebootFlag(0);	
	}
	if(CWMP_REDIRECT_VAL){
		SetCwmpRedirectConnReqFlag(0);
		ClearCwmpRedirectCnt();
	}

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		setDeviceAlarmFlag(0);
		setDeviceClearAlarmFlag(0);
		setDeviceMonitorFlag(0);
	}
	if(TCSUPPORT_C1_CUC_VAL && TCSUPPORT_SYSLOG_VAL)
		setAlarmBuildFlag(0);
	if(TCSUPPORT_CT_UPNP_DM_VAL)
		setUPnPDMDownloadFlag(0);
	if(TCSUPPORT_CT_WAN_PTM_VAL){	
		if(isDslOpmodeChange())
			setPreDslOpmode();
	}
	if(TCSUPPORT_CT_LONG_RESETBTN_VAL){
		if ( 1 == getLongResetBtnFlag() )
			setLongResetBtnFlag(0);
	}
	if(CT_COM_DEVICEREG_VAL){
			if( 1 == getDevregInformFlag() )
				setDevregInformFlag(0);
	}

	if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
	{
		if(TCSUPPORT_CT_ALARMMONITOR_VAL){
			if ( 1 == getSimCardRstFailAlarmFlag() )
				setSimCardRstFailAlarmFlag(0);
		}
		if ( 1 == getCardNotifyFlag() )
			setCardNotifyFlag(0);

		if( 1 == getCardWriteFlag() ){
			setCardWriteFlag(0);
			//setSimCardFirstBootFlag(0);
		}
		if ( 1 == getSimCardAuthFlag() )
			setSimCardAuthFlag(0);
	}

	if(TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_CZ_NX_VAL){
		if ( 1 == getLoidChangeFlag() )
			setLoidChangeFlag(0);
		if ( 1 == getLoidEmptyFlag() )
			setLoidEmptyFlag(0);
	}

	if ( (TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_NAMECHGNOTIFY_VAL )
		&& (1 == getNameChangeFlag()) )
		setNameChangeFlag(0);

	if ( TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_STBMAC_REPORT_VAL )
	{
		if ( 1 == getSTBListNotifyFlag() )
			setSTBListNotifyFlag(0);	
	}
	if ( TCSUPPORT_CUC_PON_SD_VAL )
	{
		if(1 == getManualInformFlag())
			setManualInformFlag(0);
	}

if(TCSUPPORT_CT_PPPINFORM_VAL){
	if ( 1 == getRoutePPPFlag() )
		setRoutePPPFlag(0);	
	if ( 1 == getBridgePPPFlag() )
		setBridgePPPFlag(0);
}

	if ( TCSUPPORT_CT_ADSL_BIND1_VAL && 1 == getXBIND1NotifyFlag() )
		setXBIND1NotifyFlag(0);

	if ( TCSUPPORT_CT_JOYME_VAL && 1 == getDnsSpeedLimitFlag() )
		setDnsSpeedLimitFlag(0);

	valueModify_Nofity = 0;
	//SaveCwmpFlags();
	return;
}

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
int cwmpInitTxBuf(void){
	cwmpTxBuf = (char*)os_malloc(TX_BUFFER_LENGTH);
	
	if(cwmpTxBuf == NULL){
		tc_printf("\r\nFailis to allocate TX Buffer!");
		return -1;
	}

       if(dbg_cwmp >= DBG_CWMP_L2 ){
	tc_printf("\r\nCWMP.c---[cwmpInitTxBuf]--The pre-allocated Tx buffer length is: %ld", TX_BUFFER_LENGTH);
       }
	memset(cwmpTxBuf, 0, TX_BUFFER_LENGTH);

	return 0;
}
#endif

int cwmpInitFileTransferStruct(void)
{
	FileTransferStructPtr dsp;
	int dsize;
		
	dsize = sizeof(FileTransferStruct);
	dsp = rpc.fileTransferStruct = (FileTransferStructPtr)os_malloc(dsize);
	if(dsp == NULL)
		return -1;
	memset(dsp, 0, dsize);

	dsp->sCmdKey = 16;
	dsp->CmdKey = (char *)os_malloc(dsp->sCmdKey);
	if(dsp->CmdKey == NULL)
		goto error;

	dsp->sFileType = 32;
	dsp->FileType = (char *)os_malloc(dsp->sFileType); // recommended size is 64.
	if(dsp->FileType == NULL)
		goto error;

	dsp->sURL = 64;
	//dsp->sURL = 256;
	dsp->URL = (char *)os_malloc(dsp->sURL); // recommended size is 256.
	if(dsp->URL == NULL)
		goto error;
	memset(dsp->URL, 0, dsp->sURL);

	dsp->sUsername = 16;
	dsp->Username = (char *)os_malloc(dsp->sUsername);  // recommended size is 256.	
	if(dsp->Username == NULL)
		goto error;

	dsp->sPassword= 16;
	dsp->Password= (char *)os_malloc(dsp->sPassword);  // recommended size is 256.	
	if(dsp->Password == NULL)
		goto error;

	dsp->sTargetFileName = 16;
	//dsp->sTargetFileName = 256;
	dsp->TargetFileName= (char *)os_malloc(dsp->sTargetFileName);  // recommended size is 256.	
	if(dsp->TargetFileName== NULL)
		goto error;
	memset(dsp->TargetFileName, 0, dsp->sTargetFileName);

	dsp->sSuccessURL = 32;
	dsp->SuccessURL = (char *)os_malloc(dsp->sSuccessURL);  // recommended size is 256.	
	if(dsp->SuccessURL== NULL)
		goto error;

	dsp->sFailureURL = 32;
	dsp->FailureURL = (char *)os_malloc(dsp->sFailureURL);  // recommended size is 256.	
	if(dsp->FailureURL == NULL)
		goto error;

	/*Add for dealing with Download error, hqfeng,2006.10.26*/
	dsp->faultStructPtr= (FaultDetailPtr)os_malloc(sizeof(FaultDetail));	
	if(dsp->faultStructPtr == NULL)
		goto error;
	memset(dsp->faultStructPtr, 0, sizeof(FaultDetail));
	
	dsp->faultStructPtr->faultString = (char *)os_malloc(128);  // recommended size is 64.	
	if(dsp->faultStructPtr->faultString == NULL)
		goto error;
	memset(dsp->faultStructPtr->faultString, 0, 128);
	strcpy(dsp->faultStructPtr->faultString, "");
	
	return 0;
error:
	if(dsp->FailureURL != NULL){
		os_free(dsp->FailureURL);
		dsp->FailureURL=NULL;
	}
	if(dsp->SuccessURL != NULL){
		os_free(dsp->SuccessURL);
		dsp->SuccessURL=NULL;
	}
	if(dsp->TargetFileName !=NULL){
		os_free(dsp->TargetFileName);
		dsp->TargetFileName=NULL;
	}
	if(dsp->Password!=NULL){
		os_free(dsp->Password);
		dsp->Password=NULL;
	}
	if(dsp->Username !=NULL){
		os_free(dsp->Username);
		dsp->Username=NULL;
	}
	if(dsp->URL != NULL){
		os_free(dsp->URL);
		dsp->URL=NULL;
	}
	if(dsp->FileType != NULL){
		os_free(dsp->FileType);
		dsp->FileType=NULL;
	}
	if(dsp->CmdKey != NULL){
		os_free(dsp->CmdKey);
		dsp->CmdKey=NULL;
	}
		
	if(dsp->faultStructPtr != NULL){
		if(dsp->faultStructPtr->faultString != NULL){
			os_free(dsp->faultStructPtr->faultString);
			dsp->faultStructPtr->faultString=NULL;
		}
		os_free(dsp->faultStructPtr);	
		dsp->faultStructPtr=NULL;
	}
	if(dsp != NULL){
		os_free(dsp);
		dsp=NULL;
	}
	rpc.fileTransferStruct = NULL;
	return -1;
}

#ifndef CODE_SHRINK  //qingpu.20080110
uint8 cwmpGetSessionCompleteFlag(void){
	return sessionCompleteFlag;
}

void cwmpSetSessionCompleteFlag(uint8 value){
	 sessionCompleteFlag = value;
}
#endif

uint8 cwmpGetAuthenFlag(void){
	return authenticationFlag;
}

void cwmpSetAuthenFlag(uint8 value){
	 authenticationFlag = value;
}

void cwmpResetAuthenFlag(void){
	authenticationFlag = 0;
}

#ifndef CODE_SHRINK  //qingpu.20080110
uint8 cwmpGetSerialNumFlag(void){
	return serial_num_flag;
}
#endif

#ifndef THREAD_REDUCE
uint32 GetValueChangeTaskID(void){
	return cwmpValueChangeTaskId;
}
#endif

uint32 GetInformTaskID(void){
	return InformTaskId;
}

//modify by brian,rewrite f5 loopback diagnostic function
void cwmpGetF5LoopbackDiagnosticsResult(atmF5LoopbackDiagnostis_t *F5LoopbackDiagnostics,uint8 wannode)
{
	F5LoopbackDiagnostics->DiagnosticsState = atmF5Loopback_t[wannode].DiagnosticsState;
	F5LoopbackDiagnostics->NumberOfRepetitions = atmF5Loopback_t[wannode].NumberOfRepetitions;
	F5LoopbackDiagnostics->Timeout = atmF5Loopback_t[wannode].Timeout;
	F5LoopbackDiagnostics->SuccessCount= atmF5Loopback_t[wannode].SuccessCount;
	F5LoopbackDiagnostics->FailureCount= atmF5Loopback_t[wannode].FailureCount;
	F5LoopbackDiagnostics->AverageResponseTime= atmF5Loopback_t[wannode].AverageResponseTime;
	F5LoopbackDiagnostics->MinimumResponseTime= atmF5Loopback_t[wannode].MinimumResponseTime;
	F5LoopbackDiagnostics->MaximumResponseTime= atmF5Loopback_t[wannode].MaximumResponseTime;
}
//add for judge if the diagnostic is inprocess
uint8 cwmpGetF5LoopbackDiagnosingFlag(void)
{
	return F5LoopbackDiagnosingFlag;
}
void cwmpSetF5LoopbackDiagnosingFlag(uint8 value){
	F5LoopbackDiagnosingFlag = value;
}
//add for if need to reset the diagnostic
uint8 cwmpGetF5LoopbackDiagnosticsResetFlag(void){
	return F5LoopbackDiagnosticsResetFlag;
}
void cwmpSetF5LoopbackDiagnosticsResetFlag(uint8 value){
	F5LoopbackDiagnosticsResetFlag = value;
}
//add for set the wanindex
uint8 cwmpGetF5LoopbackDiagnosticsWannodeindex(void)
{
	return F5LoopbackDiagnosticsWannodeindex;
}
void cwmpSetF5LoopbackDiagnosticsWannodeindex(uint8 value){
	F5LoopbackDiagnosticsWannodeindex= value;
}
//add for set num
void cwmpSetF5LoopbackDiagnosticsNumOfRep(uint32 NumOfRepetions, uint8 wannode)
{
	atmF5Loopback_t[wannode].NumberOfRepetitions = NumOfRepetions;
}
//add for set timeout
void cwmpSetF5LoopbackDiagnosticsTimeout(uint32 Timeout, uint8 wannode)
{
	atmF5Loopback_t[wannode].Timeout = Timeout;
}
#ifdef THREAD_REDUCE
pthread_t p_F5Diag = 0;

void *F5LoopbackDiagnosticsTask(void)
{
	int index = 0;
	uint32 wannodeindex = 0;
	uint8 resetflag = 1;
	uint32 timeindex = 0;
	char arg[32] = {0};
	char ping_cmd[64] = {0};
	uint32 cwmpflag = 1;
	char atmbuf[32] = {0};
	char nodeName[15] = {0};
	char waninfo[2][10] = {0};
	uint32 vpi_value = 0,vci_value = 0;
	uint32 SuccessCount = 0;
	uint32 FailureCount = 0;
	uint32 CurrentResponseTime = 0;
	uint32 AverageResponseTime = 0;
	uint32 MinResponseTime= 0;
	uint32 MaxResponseTime = 0;
	int	op_flag = 1;
	
	tc_printf("\r\ncwmpPerformF5LoopbackDiagnostics");
	
	wannodeindex = cwmpGetF5LoopbackDiagnosticsWannodeindex();
	//set F5LoopbackDiagnosingFlag indicate that diagnosis is in process
	if(cwmpGetF5LoopbackDiagnosingFlag() == 0)/*set F5LoopbackDiagnosingFlag indicate Diagnosis is in process.*/
		cwmpSetF5LoopbackDiagnosingFlag(1);

	//while the diagnosis is in process, set DiagnosticsState to "Requested"
	atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateRequested;
		
	sprintf(nodeName,"Wan_PVC%d",wannodeindex);
	tcapi_get(nodeName,"VPI",waninfo[0]);
	tcapi_get(nodeName,"VCI",waninfo[1]);

	vpi_value = strtoul(waninfo[0],NULL,0);
	vci_value = strtoul(waninfo[1],NULL,0);
	if((atmF5Loopback_t[wannodeindex].NumberOfRepetitions>=1 && atmF5Loopback_t[wannodeindex].NumberOfRepetitions<=0xFFFFFFFF) 
		&& (atmF5Loopback_t[wannodeindex].Timeout>=1 && atmF5Loopback_t[wannodeindex].Timeout<=0xFFFFFFFF))
	{
		for(index = 0; index < atmF5Loopback_t[wannodeindex].NumberOfRepetitions; index++)
		{
		
			timeindex = 0;//reset the index when excute every time
			//reset some variables in the sar driver
			sprintf(ping_cmd,"echo %d > %s",resetflag,ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH);
			system(ping_cmd);
			
			/*usage: <wanindex> <f5> <end-to-end> <type:0(AIS) 1(RDI) 2(Loopback)> <flag>*/
			sprintf(arg,"%lu %lu 1 1 2 %lu", vpi_value, vci_value, cwmpflag);
			sprintf(ping_cmd,"echo %s > %s",arg,ATM_F5LOOPBACK_DIAGNOSTIC_PATH);
			system(ping_cmd);
			for(;;)
			{
				usleep(200000);
				timeindex++;
				/*if F5LoopbackDiagnoseResetFlag is reset by DiagnosticsState/NumberOfRepetitions/Timeout resetting, Reset Diagnosis.*/
				if(F5LoopbackDiagnosticsResetFlag == 1)
				{
					cwmpSetF5LoopbackDiagnosticsResetFlag(0);
					memset(&atmF5Loopback_t[wannodeindex],0,sizeof(atmF5LoopbackDiagnostis_t));
					atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateNone;	
					cwmpSetF5LoopbackDiagnosingFlag(0);
					//usleep(200000);
					tc_printf("\r\nreset the diagnostic,we don not send Diagnostic complete Inform packet!\n");
					/*Set Thread id 0*/
					p_F5Diag = 0;
					return NULL;
				}
				tcapi_get("CATMDiagnostic_Common","DiagnosticState",atmbuf);
				if(strcmp(atmbuf,"Complete") == 0)
				{
					memset(atmbuf,0,sizeof(atmbuf));
					tcapi_get("CATMDiagnostic_Common","DiagnosticTime",atmbuf);
					//CurrentResponseTime = atoi(atmbuf);
					CurrentResponseTime = strtoul(atmbuf,NULL,0);
					if(MinResponseTime > CurrentResponseTime)
						MinResponseTime = CurrentResponseTime;
					if(MaxResponseTime < CurrentResponseTime)
						MaxResponseTime = CurrentResponseTime;
					SuccessCount ++;
					if(SuccessCount == 1)
					{
						MinResponseTime = CurrentResponseTime;
						MaxResponseTime = CurrentResponseTime;
					}
					AverageResponseTime = (AverageResponseTime*(SuccessCount-1) + CurrentResponseTime)/SuccessCount;
					break;
				}
				else if(strcmp(atmbuf,"Error_internal") == 0)
				{
					//tc_printf("\r\ninternal error,record the failure count!");
					FailureCount++;	
					//if all failure,we set the statue to eDiagnosticsStateError_internal
					if(FailureCount == (atmF5Loopback_t[wannodeindex].NumberOfRepetitions))
					{
						atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateError_internal;
					}
					
					break;
				}
				else if(strcmp(atmbuf,"InProgress") == 0)
				{
					if(timeindex*200 >= atmF5Loopback_t[wannodeindex].Timeout)//because usleep(200)
					{
						//tc_printf("\r\ntimeout,record the failure count!");
						FailureCount++;	
						break;
					}
					else
					{
						//tc_printf("\r\nthe ping progress is in,judge when the next time");
						continue;
					}
				}
				else
				{
					/*Enter here means driver error*/
					tc_printf("\r\nDriver do not contain ATM F5 Diagnostic function!\n");
					FailureCount++;	
					break;
				}
			}
			
		}
		//record the information
		if(eDiagnosticsStateError_internal != (atmF5Loopback_t[wannodeindex].DiagnosticsState))
		{
			atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateComplete;
		}
			atmF5Loopback_t[wannodeindex].SuccessCount = SuccessCount;
			atmF5Loopback_t[wannodeindex].FailureCount = FailureCount;
			atmF5Loopback_t[wannodeindex].AverageResponseTime = AverageResponseTime;
			atmF5Loopback_t[wannodeindex].MinimumResponseTime = MinResponseTime;
			atmF5Loopback_t[wannodeindex].MaximumResponseTime = MaxResponseTime;
	}
	else//NumberOfRepetitions or Timeout is invalid
	{
		atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateError_other;
		//tc_printf("\r\nparameter is not right!");
		setIPPingCompleteFlag(1);
		cwmpSetF5LoopbackDiagnosingFlag(0);
		if(TCSUPPORT_CT_MIDWARE_VAL){
			setOperationFlag(2);
			op_flag=iTr069Enable;
		}
		if(op_flag)
			sem_post(&SEM_IMFORM);
		/*Set Thread id 0*/
		p_F5Diag = 0;
		return NULL;
	}
	
	setIPPingCompleteFlag(1);
	cwmpSetF5LoopbackDiagnosingFlag(0);
	if(TCSUPPORT_CT_MIDWARE_VAL){
		setOperationFlag(2);
		op_flag=iTr069Enable;
	}
	if(op_flag)
		sem_post(&SEM_IMFORM);

	/*Set Thread id 0*/
	p_F5Diag = 0;
	return NULL;
}

int cwmpPerformF5LoopbackDiagnostics(void)
{
	int ret = 0;
	pthread_attr_t p_F5Diag_attr;

	if(p_F5Diag != 0)
	{
		tc_printf("\r\ncwmpPerformF5LoopbackDiagnostics: Thread already run, we will exit!");
		return -1;
	}
	/*create thread*/
	ret = pthread_attr_init(&p_F5Diag_attr);
	if(ret != 0)
	{
		tc_printf("\r\ncwmpPerformF5LoopbackDiagnostics: Thread attribute creation fail!");
		return -1;
	}
	//need to set deached way
	ret = pthread_attr_setdetachstate(&p_F5Diag_attr,PTHREAD_CREATE_DETACHED);
	if(ret != 0)
	{
		tc_printf("\r\ncwmpPerformF5LoopbackDiagnostics: Set attribute fail!");
		return -1;
	}

	ret = pthread_create(&p_F5Diag, &p_F5Diag_attr,(void *)F5LoopbackDiagnosticsTask,NULL);
	if(ret != 0)
	{
		tc_printf("\r\ncwmpPerformF5LoopbackDiagnostics: Create thread fail!");
		return -1;
	}

	pthread_attr_destroy(&p_F5Diag_attr);
}

#else
//atm f5 loopback diagnostic main function
int cwmpPerformF5LoopbackDiagnostics(void)
{
	int index = 0;
	uint32 wannodeindex = 0;
	uint8 resetflag = 1;
	uint32 timeindex = 0;
	char arg[32] = {0};
	char ping_cmd[64] = {0};
	uint32 cwmpflag = 1;
	char atmbuf[32] = {0};
	char nodeName[15] = {0};
	char waninfo[2][10] = {0};
	uint32 vpi_value = 0,vci_value = 0;
	uint32 SuccessCount = 0;
	uint32 FailureCount = 0;
	uint32 CurrentResponseTime = 0;
	uint32 AverageResponseTime = 0;
	uint32 MinResponseTime= 0;
	uint32 MaxResponseTime = 0;
	int	op_flag = 1;
	
	tc_printf("\r\ncwmpPerformF5LoopbackDiagnostics");
	
	wannodeindex = cwmpGetF5LoopbackDiagnosticsWannodeindex();
	//set F5LoopbackDiagnosingFlag indicate that diagnosis is in process
	if(cwmpGetF5LoopbackDiagnosingFlag() == 0)/*set F5LoopbackDiagnosingFlag indicate Diagnosis is in process.*/
		cwmpSetF5LoopbackDiagnosingFlag(1);

	//while the diagnosis is in process, set DiagnosticsState to "Requested"
	atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateRequested;
		
	sprintf(nodeName,"Wan_PVC%d",wannodeindex);
	tcapi_get(nodeName,"VPI",waninfo[0]);
	tcapi_get(nodeName,"VCI",waninfo[1]);

	vpi_value = strtoul(waninfo[0],NULL,0);
	vci_value = strtoul(waninfo[1],NULL,0);
	if((atmF5Loopback_t[wannodeindex].NumberOfRepetitions>=1 && atmF5Loopback_t[wannodeindex].NumberOfRepetitions<=0xFFFFFFFF) 
		&& (atmF5Loopback_t[wannodeindex].Timeout>=1 && atmF5Loopback_t[wannodeindex].Timeout<=0xFFFFFFFF))
	{
		for(index = 0; index < atmF5Loopback_t[wannodeindex].NumberOfRepetitions; index++)
		{
		
			timeindex = 0;//reset the index when excute every time
			//reset some variables in the sar driver
			sprintf(ping_cmd,"echo %d > %s",resetflag,ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH);
			system(ping_cmd);
			/*if F5LoopbackDiagnoseResetFlag is reset by DiagnosticsState/NumberOfRepetitions/Timeout resetting, Reset Diagnosis.*/
			if(F5LoopbackDiagnosticsResetFlag == 1)
			{
				cwmpSetF5LoopbackDiagnosticsResetFlag(0);
				memset(&atmF5Loopback_t[wannodeindex],0,sizeof(atmF5LoopbackDiagnostis_t));
				atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateNone;	
				cwmpSetF5LoopbackDiagnosingFlag(0);
				usleep(200000);
				tc_printf("\r\nreset the diagnostic");
				setIPPingCompleteFlag(1);

				if(TCSUPPORT_CT_MIDWARE_VAL){
					setOperationFlag(2);
					op_flag=iTr069Enable;
				}
				if(op_flag)
					sem_post(&SEM_IMFORM);
				return -1;
			}
			/*usage: <wanindex> <f5> <end-to-end> <type:0(AIS) 1(RDI) 2(Loopback)> <flag>*/
			sprintf(arg,"%lu %lu 1 1 2 %lu",vpi_value,vci_value,cwmpflag);
			sprintf(ping_cmd,"echo %s > %s",arg,ATM_F5LOOPBACK_DIAGNOSTIC_PATH);
			system(ping_cmd);
			for(;;)
			{
				usleep(200000);
				timeindex++;
				tcapi_get("CATMDiagnostic_Common","DiagnosticState",atmbuf);
				if(strcmp(atmbuf,"Complete") == 0)
				{
					memset(atmbuf,0,sizeof(atmbuf));
					tcapi_get("CATMDiagnostic_Common","DiagnosticTime",atmbuf);
					//CurrentResponseTime = atoi(atmbuf);
					CurrentResponseTime = strtoul(atmbuf,NULL,0);
					if(MinResponseTime > CurrentResponseTime)
						MinResponseTime = CurrentResponseTime;
					if(MaxResponseTime < CurrentResponseTime)
						MaxResponseTime = CurrentResponseTime;
					SuccessCount ++;
					if(SuccessCount == 1)
					{
						MinResponseTime = CurrentResponseTime;
						MaxResponseTime = CurrentResponseTime;
					}
					AverageResponseTime = (AverageResponseTime*(SuccessCount-1) + CurrentResponseTime)/SuccessCount;
					break;
				}
				else if(strcmp(atmbuf,"Error_internal") == 0)
				{
					//tc_printf("\r\ninternal error,record the failure count!");
					FailureCount++;	
					//if all failure,we set the statue to eDiagnosticsStateError_internal
					if(FailureCount == (atmF5Loopback_t[wannodeindex].NumberOfRepetitions))
					{
						atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateError_internal;
					}
					
					break;
				}
				else if(strcmp(atmbuf,"InProgress") == 0)
				{
					if(timeindex*200 >= atmF5Loopback_t[wannodeindex].Timeout)//because usleep(200)
					{
						//tc_printf("\r\ntimeout,record the failure count!");
						FailureCount++;	
						break;
					}
					else
					{
						//tc_printf("\r\nthe ping progress is in,judge when the next time");
						continue;
					}
				}
				else
				{
					//cannot enter here,for use in the future
				}
			}
			
		}
		//record the information
		if(eDiagnosticsStateError_internal != (atmF5Loopback_t[wannodeindex].DiagnosticsState))
		{
			atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateComplete;
		}
			atmF5Loopback_t[wannodeindex].SuccessCount = SuccessCount;
			atmF5Loopback_t[wannodeindex].FailureCount = FailureCount;
			atmF5Loopback_t[wannodeindex].AverageResponseTime = AverageResponseTime;
			atmF5Loopback_t[wannodeindex].MinimumResponseTime = MinResponseTime;
			atmF5Loopback_t[wannodeindex].MaximumResponseTime = MaxResponseTime;
	}
	else//NumberOfRepetitions or Timeout is invalid
	{
		atmF5Loopback_t[wannodeindex].DiagnosticsState = eDiagnosticsStateError_other;
		//tc_printf("\r\nparameter is not right!");
		setIPPingCompleteFlag(1);
		if(TCSUPPORT_CT_MIDWARE_VAL)
			setOperationFlag(2);
		cwmpSetF5LoopbackDiagnosingFlag(0);
		if(TCSUPPORT_CT_MIDWARE_VAL)
			op_flag=iTr069Enable;
		if(op_flag)
			sem_post(&SEM_IMFORM);
		return -1;
	}
	if(TCSUPPORT_CT_MIDWARE_VAL)
		setOperationFlag(2);
	setIPPingCompleteFlag(1);
	cwmpSetF5LoopbackDiagnosingFlag(0);
	if(TCSUPPORT_CT_MIDWARE_VAL)
		op_flag=iTr069Enable;
	if(op_flag)
		sem_post(&SEM_IMFORM);
	return 0;
}
#endif

void  cwmpSetWebprdenable(int value){
	web_prdenable_flag = value;
}

int  cwmpGetWebprdenable(void){
	return web_prdenable_flag;
}

void cwmpSetWebprdPeriod(int value){
	web_prdPeriod_flag = value;
}

int cwmpGetWebprdPeriod(void){
	return web_prdPeriod_flag;
}

//modify by brian for linux dsl diagnostic

int cwmpPerformWANADSLDiagnostics(void)
{
	int ret;
	char cmdbuf[40];
	tc_printf("\r\ncwmpPerformWANADSLDiagnostics");
	sprintf(cmdbuf, "w ghs set ldm on");
	system(cmdbuf);
	setIPPingCompleteFlag(1);
	if(TCSUPPORT_CT_MIDWARE_VAL)
		setOperationFlag(3);
	if ( ret == 0 )
		return 0;
	else
		return -1;
}

#ifdef TR143
uint8 cwmpInitDiagnosticsStruct(void)
{
	rpc.loadDiagInfo.transferURL[0] = '\0';
	//rpc.loadDiagInfo.ifaceChannName[0] = '\0';
	rpc.loadDiagInfo.interfaceName[0] = '\0';
	rpc.loadDiagInfo.transferIPAddress = 0;
	rpc.loadDiagInfo.lengToSend = 0;
	//rpc.loadDiagInfo.sTime = 0;

	//tcdbg_printf("==>cwmpInitDiagnosticsStruct()\n");
#if 0
	rpc.loadDiagInfo.sysTimeTCPOpenRequest = 0;
	rpc.loadDiagInfo.sysTimeTCPOpenResponse = 0;
	rpc.loadDiagInfo.sysTimeROM = 0;
	rpc.loadDiagInfo.sysTimeBOM = 0;
	rpc.loadDiagInfo.sysTimeEOM = 0;
#else
	memset( &rpc.loadDiagInfo.sysTimeTCPOpenRequest, 0, sizeof(rpc.loadDiagInfo.sysTimeTCPOpenRequest) );
	memset( &rpc.loadDiagInfo.sysTimeTCPOpenResponse, 0, sizeof(rpc.loadDiagInfo.sysTimeTCPOpenResponse) );
	memset( &rpc.loadDiagInfo.sysTimeROM, 0, sizeof(rpc.loadDiagInfo.sysTimeROM) );
	memset( &rpc.loadDiagInfo.sysTimeBOM, 0, sizeof(rpc.loadDiagInfo.sysTimeBOM) );
	memset( &rpc.loadDiagInfo.sysTimeEOM, 0, sizeof(rpc.loadDiagInfo.sysTimeEOM) );
#endif
	rpc.loadDiagInfo.startCountReceiveFlag = 0;
	rpc.loadDiagInfo.RTAddFlag = 0;
	rpc.loadDiagInfo.testBytesReceived = 0;
	rpc.loadDiagInfo.BOMInOctet = 0;
	rpc.loadDiagInfo.EOMInOctet = 0;
	rpc.loadDiagInfo.BOMOutOctet = 0;
	rpc.loadDiagInfo.EOMOutOctet = 0;
#if 0
	rpc.loadDiagInfo.diagGateway = 0;
#endif
	strcpy( rpc.loadDiagInfo.diagGateway, "" );

	setDiagnosticsCompleteFlag(0);
	return 0;
}

void cwmpInitDiagInfo(void)
{
	memset( &rpc.loadDiagInfo.sysTimeTCPOpenRequest, 0, sizeof(rpc.loadDiagInfo.sysTimeTCPOpenRequest) );
	memset( &rpc.loadDiagInfo.sysTimeTCPOpenResponse, 0, sizeof(rpc.loadDiagInfo.sysTimeTCPOpenResponse) );
	memset( &rpc.loadDiagInfo.sysTimeROM, 0, sizeof(rpc.loadDiagInfo.sysTimeROM) );
	memset( &rpc.loadDiagInfo.sysTimeBOM, 0, sizeof(rpc.loadDiagInfo.sysTimeBOM) );
	memset( &rpc.loadDiagInfo.sysTimeEOM, 0, sizeof(rpc.loadDiagInfo.sysTimeEOM) );

	rpc.loadDiagInfo.testBytesReceived = 0;
	rpc.loadDiagInfo.BOMInOctet = 0;
	rpc.loadDiagInfo.EOMInOctet = 0;
	rpc.loadDiagInfo.BOMOutOctet = 0;
	rpc.loadDiagInfo.EOMOutOctet = 0;
}
#endif


char* GetCwmpRedirectURL(void){
	return rpc.redirectacsCurInfo.cwmpRedirectURL;
}

uint8 GetCwmpRedirectCnt(void){
	return rpc.redirectacsCurInfo.cwmpRedirectCnt;
}

void cwmpCalculateReirectCnt(void){
	rpc.redirectacsCurInfo.cwmpRedirectCnt++;
}

void ClearCwmpRedirectCnt(void){
	rpc.redirectacsCurInfo.cwmpRedirectCnt = 0;
}

void SetCwmpRedirectFlag(uint8 flag){
	rpc.redirectacsCurInfo.cwmpRedirectFlag = flag;
}

uint8 GetCwmpRedirectFlag(void){
	return rpc.redirectacsCurInfo.cwmpRedirectFlag;
}

uint8 GetCwmpRedirectHttpFlag(void){
	return rpc.redirectacsCurInfo.cwmpRedirectHttpFlag;
}

Unsigned32 GetCwmpRedirectHostAddress(void){
	return rpc.redirectacsCurInfo.cwmpRedirectHostAddress;
}

Unsigned16 GetCwmpRedirectPort(void){
	return rpc.redirectacsCurInfo.cwmpRedirectPort;
}

char* GetCwmpRedirectPath(void){
	return rpc.redirectacsCurInfo.cwmpRedirectPath;
}

char* GetCwmpRedirectHostName(void){
	return rpc.redirectacsCurInfo.cwmpRedirectHostName;
}

uint8 cwmpCheckValidURL(char* URL){
	char tempurl[257];

	if(strlen(URL) > 255)
		return 0;
	
	strcpy(tempurl, URL);
	if(strncmp(getCapitalStr(tempurl), "HTTP://", 7) == 0){
		if( (URL[7] >= '0' && URL[7] <= '9') ||(URL[7] >= 'A' && URL[7] <= 'Z')  || (URL[7] >= 'a' && URL[7] <= 'z') )
			return 1;
		else
			return 0;
	}
	else if(strncmp(getCapitalStr(tempurl), "HTTPS://", 8) == 0){
		if( (URL[8] >= '0' && URL[8] <= '9') ||(URL[8] >= 'A' && URL[8] <= 'Z')  || (URL[8] >= 'a' && URL[8] <= 'z') )
			return 1;
		else
			return 0;
	}
	else{
		return 0;
	}
	
	return 0;

}	

//modify by brian
void cwmpBuildCwmpUrl(void)
{
	uint8 ipAddr[4];
	int port;
	char * path;
	char tmp[20];
	int url_len = 0;
	port = 0;
	path = NULL;
	char cwmpbuf[256] = {0};
	strcpy(cwmpbuf,getCwmpHostName());
	url_len = strlen(cwmpbuf);
	if(getHttpsFlag()){
		if(url_len + 8 > CwmpURLMaxLen-1){//"https://" length is 8
			tc_printf("Can not build URL, cwmp host name too long!=>%d\r\n", (url_len + 8));
			return;
		} else {
			sprintf(cwmpURL, "https://%s", cwmpbuf); 
			url_len += 8;
		}
	} else {
		if(url_len + 7 > CwmpURLMaxLen-1){//"http://" length is 7
			tc_printf("Can not build URL, cwmp host name too long!=>%d\r\n", (url_len + 7));
			return;
		} else {
			sprintf(cwmpURL, "http://%s", cwmpbuf); 
			url_len += 7;
		}
	}
	port = getCwmpPort();
	if(port != kDefaultHttpPort && port != kDefaultTlsPort){
		sprintf(tmp, ":%d",port);
		url_len += strlen(tmp);
		if(url_len > CwmpURLMaxLen-1){
			tc_printf("Can not build URL, cwmp port too long!=>%d\r\n", url_len);
			return;
		} else {
			strcat(cwmpURL, tmp);
		}
	}
	memset(cwmpbuf,0,sizeof(cwmpbuf));
	strcpy(cwmpbuf,getCwmpPath());
	path = cwmpbuf;
	if( path != NULL) {
		url_len += strlen(path);
		if(url_len > CwmpURLMaxLen-1){
			tc_printf("Can not build URL, cwmp path too long!=>%d\r\n", url_len);
			return;
		} else {
			strcat(cwmpURL, path);
		}
	}
	
	return;
}

/*______________________________________________________________________________
**	processTheHostAddress
**
**	description:
**	This function for saving the Host Address.  
**
**	parameters:
**	* theHostAddress: The Host Address for save.
**	saveType:  0:save it to flash by ci-cmd
**	                 1:save it to flash after receiving InformResponse
**
**	return:
**	
**	Created by hqfeng.
**____________________________________________________________________________*/
void processTheHostAddress(Unsigned32* theHostAddress, int8 saveType)
{
	int i;
	int flag1 = -1;
	int flag2 = -1;

	if(saveType == 0){//Save the current host address to vHostAddressTable[0],and set the BootTrapFlag
		if (*theHostAddress == 0) return; /* fix not send 0 bootstrap when reset to default */
		spSysParam.tcCwmpAttr->vHostAddressTable[0] = *theHostAddress;
		for( i = 1; i < 20; i++){
			if(*theHostAddress == spSysParam.tcCwmpAttr->vHostAddressTable[i]){
				flag1 = 1;
				break;
			}
		}
		if(flag1 == -1){
			setBootTrapFlag(1);		
		}else{
			setBootTrapFlag(0);		
		}

	}else if(saveType == 1){
		if(getBootTrapFlag() == 0)
			return;
		//BootTrapFlag == 1,we need to save the current host address to flash.
		for( i = 1; i < 20; i++){
			//printf("\r\n vHostAddressTable[%d] = %x", i, spSysParam.tcCwmpAttr->vHostAddressTable[i]);
			if(*theHostAddress == spSysParam.tcCwmpAttr->vHostAddressTable[i]){
				flag1 = 1;
				break;
			}
		}
		if(flag1 == -1){//Can't find theHostAddress,save it to the table,and set the BootTrap Flag to 0.
			if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL){
				memset(spSysParam.tcCwmpAttr->vHostAddressTable,0,sizeof(spSysParam.tcCwmpAttr->vHostAddressTable[0])*20);				
				spSysParam.tcCwmpAttr->vHostAddressTable[1] = *theHostAddress;
			}
			else{
				for(i = 0; i < 20; i++){
					if(spSysParam.tcCwmpAttr->vHostAddressTable[i] == 0){
						flag2 = 1;
						break;
					}
				}
				if(flag2 == 1){
					spSysParam.tcCwmpAttr->vHostAddressTable[i] = *theHostAddress;
				}else{
					spSysParam.tcCwmpAttr->vHostAddressTable[1] = *theHostAddress;
				}
			}
			setBootTrapFlag(0);
			SaveCwmpFlags();	
			spSave();		
		}
	}

	return;
}

void processTheHostAddressExt(Unsigned32* theHostAddress, Unsigned16* theHostPort, char *theHostPath, int8 saveType)
{
	int i;
	int flag1 = -1;
	int flag2 = -1;
	Unsigned8 tmp[16];
	char Md5Digest[17];
	char theString[261];
	memset(Md5Digest, 0x00, sizeof(Md5Digest));
	memset(tmp, 0x00, sizeof(tmp));
	memset(theString, 0x00, sizeof(theString));	
	if(theHostPort && theHostPath){	
		sprintf(theString, "%u%s", *theHostPort, theHostPath);
		RpMD5(theString, tmp);
		memset(theString, 0x00, sizeof(theString));
		RpHexToString(tmp, theString, 16);
		for(i = 0; i < 16; i++)
		{
			Md5Digest[i] = theString[i + 8];
		}
	}
	if(saveType == 0){//Save the current host address to vHostAddressTable[0],and set the BootTrapFlag
		if (*theHostAddress == 0) return; /* fix not send 0 bootstrap when reset to default */
		spSysParam.tcCwmpAttr->vHostAddressTable[0] = *theHostAddress;
		strcpy(spSysParam.tcCwmpAttr->vHostMD5Table[0], Md5Digest);
		for( i = 1; i < 20; i++){
			if(*theHostAddress == spSysParam.tcCwmpAttr->vHostAddressTable[i]){
				if(strcmp(Md5Digest, spSysParam.tcCwmpAttr->vHostMD5Table[i]) == 0){
					flag1 = 1;
					break;
				}
			}
		}
		if(flag1 == -1){
			setBootTrapFlag(1);		
		}else{
			setBootTrapFlag(0);		
		}

	}else if(saveType == 1){
		if(getBootTrapFlag() == 0)
			return;
		//BootTrapFlag == 1,we need to save the current host address to flash.
		for( i = 1; i < 20; i++){
			//printf("\r\n vHostAddressTable[%d] = %x", i, spSysParam.tcCwmpAttr->vHostAddressTable[i]);
			if(*theHostAddress == spSysParam.tcCwmpAttr->vHostAddressTable[i]){
				if(strcmp(Md5Digest, spSysParam.tcCwmpAttr->vHostMD5Table[i]) == 0){
					flag1 = 1;
					break;
				}
			}
		}
		if(flag1 == -1){//Can't find theHostAddress,save it to the table,and set the BootTrap Flag to 0.
			if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL){
				memset(spSysParam.tcCwmpAttr->vHostAddressTable,0,sizeof(spSysParam.tcCwmpAttr->vHostAddressTable[0])*20);				
				spSysParam.tcCwmpAttr->vHostAddressTable[1] = *theHostAddress;
			}
			else{
				for(i = 0; i < 20; i++){
					if(spSysParam.tcCwmpAttr->vHostAddressTable[i] == 0){
						flag2 = 1;
						break;
					}
				}
				if(flag2 == 1){
					spSysParam.tcCwmpAttr->vHostAddressTable[i] = *theHostAddress;
					strcpy(spSysParam.tcCwmpAttr->vHostMD5Table[i], Md5Digest);
				}else{
					spSysParam.tcCwmpAttr->vHostAddressTable[1] = *theHostAddress;
					strcpy(spSysParam.tcCwmpAttr->vHostMD5Table[1], Md5Digest);
				}
			}
			setBootTrapFlag(0);
			SaveCwmpFlags();	
			spSave();		
		}
	}

	return;
}

char* cwmp_strdup(char * str){
	char *cstr = NULL;	

	if(str == NULL)
		return NULL;
	
	cstr = (char*) os_malloc(strlen(str)+1);
	if(cstr != NULL)
		strcpy(cstr, str);

	return cstr;
}

uint8 cwmpGetDownloadFileType(void){
	if(strcmp(rpc.fileTransferStruct->FileType, "1 Firmware Upgrade Image") == 0)
		return CWMP_FIRMWARE;
	if(strcmp(rpc.fileTransferStruct->FileType, "2 Web Content") == 0)
		return CWMP_WEB_CONTENT;
	if(strcmp(rpc.fileTransferStruct->FileType, "3 Vendor Configuration File") == 0)
		return CWMP_CONFIGURATION_FILE;

	return 0;
}

char* cwmpGetDownloadTargetFileName(void){
	return rpc.fileTransferStruct->TargetFileName;
}

/*Added for the condition: no TargetFileName, by hqfeng, 2006.11.03*/
char* cwmpGetFileNameFromDownloadURL(void)
{
	char  url[257];
	char  *p = NULL;

	memset( url, 0, sizeof(url) );

	strcpy(url, rpc.fileTransferStruct->URL);
	p = strrchr(url, kAscii_Slash);//get the last '/' char pointer.

	if ( p != NULL ) {
	strcpy(gDownLoadFileName, p+1);
	}

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncwmp.c---[cwmpGetFileNameFromDownloadURL]----gDownLoadFileName = %s", gDownLoadFileName);
	}
	return gDownLoadFileName;
}

uint8 cwmpSupportDownloadFile(char* downloadFileName){
	if(!get_file_transfer_flag())
		return 0;
	else{
		if(cwmpGetDownloadFileType() == CWMP_FIRMWARE)
			return 1;
		else if(cwmpGetDownloadFileType() == CWMP_WEB_CONTENT)
			return 0;
		else if(cwmpGetDownloadFileType() == CWMP_CONFIGURATION_FILE){
			if(strcmp(getCapitalStr(downloadFileName), "CA") == 0 
			    || strcmp(getCapitalStr(downloadFileName), "ROM-0") == 0)
				return 1;
			else
				return 0;
		}			
	}

	return 0;
}

uint8 getMotiveRSTFlag(void)
{	
	return motiveRSTFlag;
}

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
static int doPreAllocTxBufFlag(int argc, char *argv[], void *p)
{
	if (argc < 2) {
		printf("Usage: sys cwmp preAllocTxBuf [0|1]\n");
		printf("Cur Flag: %d\n", getPreAllocTxBufFlag());
		return -1;
	}
	if( argc == 2 ){
		if ((strcmp(argv[1], "0") == 0) || (strcmp(argv[1], "1") == 0)) {
			setPreAllocTxBufFlag(atoi(argv[1]));
			spSave();
		}else
			printf("Usage: sys cwmp preAllocTxBuf [0|1]\n");
	}else{
		printf("Usage: sys cwmp preAllocTxBuf [0|1]\n");
		printf("Cur Flag: %d\n", getPreAllocTxBufFlag());
		return -2;
	}	

	return 0;
}
#endif


uint8 getCwmpCA(void)
{
	return spSysParam.tcCwmpAttr->ca;
}

void setCwmpCA(uint8 val)
{
	spSysParam.tcCwmpAttr->ca = val;
	SaveCwmpFlags();
	//spSave();
	return;
}

uint8 GetRetryCount(void){
	return cwmpRetryNum;
}

char CwmpSwitch[10] = "Yes";
uint8 getCwmpSwitch(void)
{
	uint8 flag = 0;
	if(getCwmpAttrValue(CPSwh,CwmpSwitch,sizeof(CwmpSwitch)) != 0)
	{
		tc_printf("\r\ngetCwmpSwitch error!");
		memset(CwmpSwitch,0,sizeof(CwmpSwitch));
		setCwmpAttrValue(CPSwh,CwmpSwitch);
	}
	//tc_printf("\r\nthe CwmpSwitch is %s",CwmpSwitch);
	if(!strcmp(CwmpSwitch,"Yes"))
		flag = 1;
	else
		flag = 0;
	return flag; 
}

void setCwmpSwitch(uint8 value)
{
	if(value == 1)
		strcpy(CwmpSwitch,"Yes");
	else 
		strcpy(CwmpSwitch,"No");
		
	setCwmpAttrValue(CPSwh,CwmpSwitch);
	return;
}



uint8 getHttpsFlag(void)
{
     return spSysParam.tcCwmpAttr->https;	
}

void setHttpsFlag(uint8 value)
{
	uint8 old;
	int port;

	old = spSysParam.tcCwmpAttr->https;
	port = getCwmpPort();
	if(value == 0 && old == 1){ // https -> http
		if(port == kDefaultTlsPort)
			setCwmpPort(kDefaultHttpPort);
	}else if(value == 1 && old == 0){ // http->https
		if( port == kDefaultHttpPort )
			setCwmpPort(kDefaultTlsPort);
	}
	spSysParam.tcCwmpAttr->https = value;	
	//spSave_old();
}

//chuguang 20090512 add for ChinaUnicom ASB ACS test
uint8 getSendHostPortNumFlag(void)
{
     return SendHostPortNumFlag;	
}

void setSendHostPortNumFlag(uint8 value)
{
	SendHostPortNumFlag = value;	
 }


char CRCPEPort[12] = "7547";
uint32 getCPEPort(void)
{
	char *end;
	if(getCwmpAttrValue(CRPort,CRCPEPort,sizeof(CRCPEPort)) != 0)
	{
		tc_printf("\r\ngetCPEPort error!");
		//memset(CRCPEPort,'\0',sizeof(CRCPEPort));
		strcpy(CRCPEPort,"7547");
		setCwmpAttrValue(CRPort,CRCPEPort);
	}
	
	return strtoul(CRCPEPort,&end,0);
}

int setCPEPort(uint32 value)
{
	sprintf(CRCPEPort,"%lu",value);
	//may be we need to judge if some ports(such telnet,ftp and so n) can be set!
	setCwmpAttrValue(CRPort,CRCPEPort);
	return 0;
}

char CRCPEPath[128] = "/tr069";
char* getCPEPath(void)
{
	if(getCwmpAttrValue(CRPath,CRCPEPath,sizeof(CRCPEPath)) != 0)
	{
		tc_printf("\r\ngetCPEPath error!");
		memset(CRCPEPath,0,sizeof(CRCPEPath));
		setCwmpAttrValue(CRPath,CRCPEPath);
	}
	return CRCPEPath;
}

void setCPEPath(char* value)
{
	if(strlen(value) > 127)
	{
		tc_printf("\r\n ConnectionRequestPath length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(CRPath,value);
	return;
}

char cwmpProvisionCode[64] = "default";
char* getProvisionCode(void)
{
	if(getCwmpAttrValue(PrvCode,cwmpProvisionCode,sizeof(cwmpProvisionCode)) != 0)
	{
		tc_printf("\r\ngetProvisionCode error!");
		memset(cwmpProvisionCode,0,sizeof(cwmpProvisionCode));
		setCwmpAttrValue(PrvCode,cwmpProvisionCode);
	}
	return cwmpProvisionCode;
}

int setProvisionCode(char * value)
{
	if(strlen(value) > 63)
	{
		tc_printf("\r\n provision code length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(PrvCode,value);
	return 0;
}

char * getCwmpUrl(void)
{
	return cwmpURL;
}

int  setCwmpUrl(char *value)
{	
	strcpy(cwmpURL,value);
}

char CwmpPath[256] = "default";
char* getCwmpPath(void)
{
	if(getCwmpAttrValue(CPPath,CwmpPath,sizeof(CwmpPath)) != 0)
	{
		tc_printf("\r\ngetCwmpPath error!");
		memset(CwmpPath,0,sizeof(CwmpPath));
		setCwmpAttrValue(CPPath,CwmpPath);
	}
	return CwmpPath;
}

void setCwmpPath(char* value)
{
	if(strlen(value) > 255)
	{
		tc_printf("\r\n CwmpPath length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(CPPath,value);
	return;
}

char CwmpPort[12] = "80";
int getCwmpPort(void)
{
	char *end;
	if(getCwmpAttrValue(CPPort,CwmpPort,sizeof(CwmpPort)) != 0)
	{
		tc_printf("\r\ngetCwmpPort error!");
		memset(CwmpPort,'\0',sizeof(CwmpPort));
		setCwmpAttrValue(CPPort,CwmpPort);
	}
	
	return strtoul(CwmpPort,&end,0);
}

void setCwmpPort(int value)
{
	sprintf(CwmpPort,"%lu",value);
	//may be we need to judge if some ports(such telnet,ftp and so n) can be set!
	setCwmpAttrValue(CPPort,CwmpPort);
	return;
}

char Manufacture[64] = "test";
char * getCwmpManufacture(void)
{
	if(TCSUPPORT_CT_PON_C9_VAL){
		memset(Manufacture, 0, sizeof(Manufacture));
		tcapi_get("RegInfo_Entry", "Manufacturer", Manufacture);
		if(strlen(Manufacture)>0)
			return Manufacture;
	}
	if ( getUserCustomizeFlag() == 1 ) {
		/*read from DeviceInfo*/
		return (char*)GetVendorString();
	}
	else {
		
		/*read from romfile*/
		return getManufacturer();
	}	
}

char cwmpOUI[7] = "0017A6";
char * getCwmpOUI(void)
{
	if ( getUserCustomizeFlag() == 1 ) {
		tcapi_get("DeviceInfo_devParaDynamic","ManufacturerOUI",cwmpOUI);
	}
	else
	{
		if(getCwmpAttrValue(ManufacturerOUI,cwmpOUI,sizeof(cwmpOUI)) != 0)
		{
			tc_printf("\r\ngetCwmpOUI error!");
			memset(cwmpOUI,0,sizeof(cwmpOUI));
			setCwmpAttrValue(ManufacturerOUI,cwmpOUI);
		}
	}

	return cwmpOUI;
}

char * getCwmpProductClass(void)
{
	if ( getUserCustomizeFlag() == 1 ) {
		/*read from DeviceInfo*/
		return  (char*)GetProductString();
	}
	else {
	
		/*read from romfile*/
		return getProductClass();
	}	
}

char * getCwmpModelName(void)
{
	if ( getUserCustomizeFlag() == 1 ) {
		/*read from DeviceInfo*/
		return (char*)GetModelNameString();
	}
	else {
	
		/*read from romfile*/
		return getModelName();
	}	
}

char * getCwmpSerialNum(void)
{
	int i = 0, length = 0;
	char theChar;
	
	uint8 nm_MAC[7] ;
	if ( getUserCustomizeFlag() == 1 ) {
		tcapi_get("DeviceInfo_devParaDynamic","SerialNum",SerialNum);
		return SerialNum;
	}
	else{
		/*read from romfile*/
		return getSerialNumValue();
	}
}


char HardwareVersion[128] = {0};
char *getCwmpHardwareVersion(void)
{
	int idx = 0;
	
	if(TCSUPPORT_CT_PON_C9_VAL){
		memset(HardwareVersion, 0, sizeof(HardwareVersion));
		tcapi_get("RegInfo_Entry", "CustomerHWVersion", HardwareVersion);
		if(strlen(HardwareVersion)>0)
			return HardwareVersion;
	}
	if ( 1 == getUserCustomizeFlag() )
		tcapi_get("DeviceInfo_devParaStatic", "CustomerHWVersion", HardwareVersion);
	else
		strcpy(HardwareVersion, getHardwareVer());

	if(TCSUPPORT_CT_MIDWARE_VAL){
		for ( idx = 0; idx < strlen(HardwareVersion); idx ++ )
		{
			if ( ' ' == HardwareVersion[idx] )
				HardwareVersion[idx] = '_';
		}
	}

	return HardwareVersion;
}

char SoftwareVersion[128] = {0};
char *getCwmpSoftwareVersion(void)
{
	int idx = 0;
	
	if(TCSUPPORT_CT_PON_C9_VAL){
		memset(SoftwareVersion, 0, sizeof(SoftwareVersion));
		tcapi_get("RegInfo_Entry", "CustomerSWVersion", SoftwareVersion);
		if(strlen(SoftwareVersion)>0)
			return SoftwareVersion;
	}
	if ( 1 == getUserCustomizeFlag() )
		tcapi_get("DeviceInfo_devParaStatic", "CustomerSWVersion", SoftwareVersion);
	else
		strcpy(SoftwareVersion, RetRASStrPtr2());

	if(TCSUPPORT_CT_MIDWARE_VAL){
		for ( idx = 0; idx < strlen(SoftwareVersion); idx ++ )
		{
			if ( ' ' == SoftwareVersion[idx] )
				SoftwareVersion[idx] = '_';
		}
	}

	return SoftwareVersion;
}



#ifndef C1_LEH
char*  getCapitalStr(char* str){
	int i = 0, length = 0;
	char theChar;

	length = strlen(str);
	while (i < length) {
	        theChar = str[i];
	        if (theChar >= 0x61 & theChar <= 0x7A) {
	            str[i] = theChar ^ 0x20;
	        }
		i++;
	 }

	return str;
}
#endif

char * getUpDateTime(void)
{
	uint16	years,months,days,hrs,mins,secs;
	int	minus;
	uint32 uptime;

	uptime=secclock();
	secs = (uint16) (uptime % 60);
	uptime /= 60;
	mins = (uint16) (uptime % 60);
	uptime /= 60;
	hrs = (uint16) (uptime % 24);
	uptime /= 24;
	days = (uint16) uptime;
	sprintf(updatetime,"0001-01-%02uT%02u:%02u:%02u",days+1,hrs,mins,secs);   //xzwang060715
	return updatetime;
}

static char curtime[64] = {0};

char * getCurrentTime(void)
{
	uint16	years,months,days,hrs,mins,secs;
	time_t timep;
	char buf[20] = {0};
	char timezone[10] = {0};
	char type[10] = {0};
	int ret;
	struct tm *p;
	
	if(TCSUPPORT_CWMP_PRECISE_TIME_VAL){
		time(&timep);
		p=gmtime(&timep);
		ret = tcapi_get("Timezone_Entry","TYPE",type);
		if( ret < 0 || !strcmp(type,"2" ))
		{
			strcpy(timezone,"");
		}
		else
		{
			if(tcapi_get("Timezone_Entry","TZ",buf) < 0){
				strcpy(timezone,"");
			}else if(strlen(buf) > 9){
				strncpy(timezone,buf+3,6);
			}else if(strlen(buf) == 5){
				strcpy(timezone,"");
			}else{
				strcpy(timezone,buf+3);
			}
		}
		if (TCSUPPORT_CT_PON_SC_VAL == 1) {
			struct timeval tv;
			gettimeofday(&tv, NULL);
			sprintf(curtime, "%04d-%02d-%02dT%02d:%02d:%02d.%06d", 
			(1900+p->tm_year),  (1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min,  p->tm_sec, tv.tv_usec);
		}
		else {
		sprintf(curtime,"%04u-%02u-%02uT%02u:%02u:%02u%s",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,timezone);
		}
		//tcdbg_printf("\r\getCurrentTime:  time is [%s] \n",  curtime);  
	}
	return curtime;
}


char * getHardwareVer(void)
{
	char buf[50];
	int len = 0;
	char *cp = NULL;
	char *delim = "\r\n";
	char *flag = "HwVer:";
	char *p = NULL;
	tcapi_get("Info_Adsl","fwVer",buf);
	p = strstr(buf,flag);
	strcpy(HardwareVer,p+strlen(flag));
	#if 0
	len = strlen(HardwareVer);
	HardwareVer[len - 1] = '\0';
	#endif
	cp = strtok(HardwareVer,delim);
	while(cp)
	{
		cp = strtok(NULL,delim);
	}
	
	return HardwareVer;
}

uint8 getWANIndex(void)
{
	return wanindex;
}

char * getWANExternalName(uint8 index)
{
#if 0
	char temp[20];
	char buf[10];
	char connType[24];
	sprintf(temp,"Wan_PVC%d",index);
	tcapi_get(temp,"Active",buf);
	//tc_printf("\r\nthe buf is %s",buf);
	if(!strcmp(buf,"Yes"))
	{
		memset(buf,0,sizeof(buf));
		sprintf(temp,"/etc/isp%d.conf",index);
			//tc_printf("\r\nthe temp is %d",temp);
		get_profile_str("ISP=",buf,sizeof(buf),1,temp);
			//tc_printf("\r\nthe buf is %d",atoi(buf));
		switch(atoi(buf))
		{
			case 0://dynamic ip address
			case 1://static ip address
				strcpy(connType,"WANIPConnection");
				break;
			case 2://pppoe
				strcpy(connType,"WANPPPConnection");
				break;
			
			default:
				tc_printf("\r\nerror type!");
				break;		
		}
		sprintf(wanname,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.1.ExternalIPAddress",index+1,connType);
	}
	return wanname;
#endif
	char wan_node[32], isp[10];
	char conn_type[24];
	uint32 pvc_index, entry_index;

	pvc_index = index / MAX_PVC_NUM;
	entry_index = index - pvc_index * MAX_PVC_NUM;

	sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
	
	tcapi_get(wan_node, "ISP", isp);

	memset(conn_type, 0, sizeof(conn_type));
	/* dynamic or static */
	if (!strcmp(isp, "0") || !strcmp(isp, "1")) {
		strcpy(conn_type, "WANIPConnection");
	}
	/* ppp */
	else if (!strcmp(isp, "2")) {
		strcpy(conn_type, "WANPPPConnection");
	}
	else {
		/* it will not happen in generally */
		return wanname;
	}
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		GetWanNodeMapping(&pvc_index, &entry_index);
		sprintf(wanname,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d.ExternalIPAddress", pvc_index, conn_type, entry_index);
	}
	else
		sprintf(wanname,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d.ExternalIPAddress", pvc_index + 1, conn_type, entry_index + 1);

	return wanname;
}

char * getWANMacName(uint8 index)
{
#if 0
	char temp[20];
	char buf[10];
	char connType[24];
	sprintf(temp,"Wan_PVC%d",index);
	tcapi_get(temp,"Active",buf);
	//tc_printf("\r\nthe buf is %s",buf);
	if(!strcmp(buf,"Yes"))
	{
		memset(buf,0,sizeof(buf));
		sprintf(temp,"/etc/isp%d.conf",index);
			//tc_printf("\r\nthe temp is %d",temp);
		get_profile_str("ISP=",buf,sizeof(buf),1,temp);
			//tc_printf("\r\nthe buf is %d",atoi(buf));
		switch(atoi(buf))
		{
			case 0://dynamic ip address
			case 1://static ip address
				strcpy(connType,"WANIPConnection");
				break;
			case 2://pppoe
				strcpy(connType,"WANPPPConnection");
				break;

			default:
				tc_printf("\r\nerror type!");
				break;		
		}
		sprintf(wanname,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.1.ExternalIPAddress",index+1,connType);
	}
	return wanname;
#endif
	char wan_node[32], isp[10];
	char conn_type[24];
	uint32 pvc_index, entry_index;

	if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL
		|| TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL
		|| TCSUPPORT_CT_ADSL_TJ_VAL ){
		pvc_index = index / MAX_PVC_NUM;
		entry_index = index - pvc_index * MAX_PVC_NUM;

		sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
		
		tcapi_get(wan_node, "ISP", isp);

		memset(conn_type, 0, sizeof(conn_type));
		/* dynamic or static */
		if (!strcmp(isp, "0") || !strcmp(isp, "1")) {
			strcpy(conn_type, "WANIPConnection");
		}
		/* ppp */
		else if (!strcmp(isp, "2")) {
			strcpy(conn_type, "WANPPPConnection");
		}
		else {
			/* it will not happen in generally */
			return wanMacName;
		}
		if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
			GetWanNodeMapping(&pvc_index, &entry_index);
			sprintf(wanMacName,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d.MACAddress", pvc_index, conn_type, entry_index);
		}
		else
			sprintf(wanMacName,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.%s.%d.MACAddress", pvc_index + 1, conn_type, entry_index + 1);
	}
	return wanMacName;
}

char * getCPEMac(int index)
{
	char mac[40] = {0};
	char macnode[32];
	int pvc_index, entry_index;

	if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL
		|| TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL
		|| TCSUPPORT_CT_ADSL_TJ_VAL ){
		pvc_index = index / MAX_PVC_NUM;
		entry_index = index - pvc_index * MAX_PVC_NUM;
		sprintf(macnode, "Info_nas%d.%d", pvc_index, entry_index);

		tcapi_get(macnode, "hwaddr", mac);

		if (mac[0] != '\0')
			strcpy(wanMacAddr, mac);

		tcdbg_printf("\r\nwan mac address = %s", wanMacAddr);
	}

	return wanMacAddr;
}

#ifdef BHARTI
boolean flagAddWanPPPoEUserName(uint8 index)
{
	spRemoteNode_t	*rn_p;
	rn_p = &spSysParam.remoteNode[index];
	
	if ( (rn_p->flags & SP_CONFIGURED) &&(rn_p->encapType==SP_ENCAP_POE || rn_p->encapType==SP_ENCAP_RFC2364))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

char * getWANPPPoEUserName(uint32 index)
{
	spRemoteNode_t	*rn_p;
	char connType[24];
	rn_p = &spSysParam.remoteNode[index];
	
	strcpy(pppoeUsername, rn_p->user);
	strcat(pppoeUsername, rn_p->extUserName);
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){
		GetPVCMapping(&index);
		sprintf(wanPPPoEUsername,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.2.Username", index);
	}
	else
		sprintf(wanPPPoEUsername,"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.%d.WANPPPConnection.2.Username",index+1);

	return wanPPPoEUsername;
}
#endif

char * getDefaultWANExternalIPAddr(void)
{
	return "0.0.0.0";
}



int ifPVCExist(uint32 theHostAddress)
{
	char routeInfo[8][16];
	FILE *fp = NULL;
	int pvcindex = -1;
	uint32 tmp1,tmp2;
	char *buf = NULL;
	char *p = NULL;
	char pvcname[15] = {0};

	int i = 0;
	enum routeInfo_en
	{
		DstIP=0,
		Gw,
		Mask,
		Flags,
		Metric,
		Ref,
		Use,
		Iface,
		Usr_def,
	};
	
	memset(routeInfo,0, sizeof(routeInfo));
	system(SYSROUTE_CMD);
	
	if(theHostAddress == 0)
		return -1;
	
	fp = fopen(SYSROUTE_PATH, "r");
	if(fp == NULL){
		return -1;
	}
	else
	{
		buf = os_malloc(512);
		if(buf == NULL)
		{
			fclose(fp);
			unlink(SYSROUTE_PATH);
			return -1;
		}
		/*
		Kernel IP routing table
		Destination	Gateway	Genmask			Flags	Metric	Ref	Use	Iface
		10.10.10.0	0.0.0.0	255.255.255.0		U		0		0	0	br0
		127.0.0.0		0.0.0.0	255.255.0.0		U		0		0	0	lo
		*/
		while (fgets (buf, 512, fp) != NULL)
		{
			i++;
			if(i++ > 2)
			{/*Skip the comment information*/
				sscanf(buf, "%s %s %s %s %s %s %s %s",
				routeInfo[DstIP], routeInfo[Gw], routeInfo[Mask],
				routeInfo[Flags], routeInfo[Metric], routeInfo[Ref],
				routeInfo[Use], routeInfo[Iface]);
				if(!strncmp(routeInfo[Iface],NASNAME,3))
					strcpy(pvcname,NASNAME);
				else if(!strncmp(routeInfo[Iface],PPPNAME,3))
					strcpy(pvcname,PPPNAME);
				else
					continue;
						
				if(strcmp(routeInfo[Flags],PVCFLAG))
				{
					tmp1 = (theHostAddress & (inet_addr(routeInfo[Mask])));
					tmp2 = ((inet_addr(routeInfo[DstIP])) & (inet_addr(routeInfo[Mask])));
					if(tmp1 == tmp2)
					{
						p = strstr(routeInfo[Iface],pvcname);
						pvcindex = atoi(p+strlen(pvcname));
						break;
					}
				}		
			}
		}
	}
	
	os_free(buf);
	fclose(fp);
	unlink(SYSROUTE_PATH);
	return pvcindex;	
}

char * getConRequestURL(void)
{
	return ConRequestURL;
}

void  setTr069Enable(uint8 value)
{
	uint8 prevTr069Enable;
	char CwmpTr069Enable[10]="0";
	
	if(TCSUPPORT_CT_MIDWARE_VAL){
		prevTr069Enable=GetTr069Enable();
		sprintf(CwmpTr069Enable,"%d",value);
		if((prevTr069Enable==0 && value==1) && (prevTr069Enable==1 && value==0)){
			setCwmpAttrValue(Tr069Enable,CwmpTr069Enable);
			sysRestart(0);
		}
		else
			setCwmpAttrValue(Tr069Enable,CwmpTr069Enable);
	}
	return ;
}

char CwmpInformPeriodEnable[10] = "Yes";
boolean  getInformPeriodEnable(void)
{
	boolean flag;
	if(getCwmpAttrValue(PrdEnable,CwmpInformPeriodEnable,sizeof(CwmpInformPeriodEnable)) != 0)
	{
		tc_printf("\r\ngetInformPeriodEnable error!");
		memset(CwmpInformPeriodEnable,'\0',sizeof(CwmpInformPeriodEnable));
		setCwmpAttrValue(PrdEnable,CwmpInformPeriodEnable);
	}
	tc_printf("\r\nthe CwmpInformPeriodEnable is %s",CwmpInformPeriodEnable);
	if(!strcmp(CwmpInformPeriodEnable,"Yes"))
		flag = 1;
	else
		flag = 0;
	return flag; 
}

void  setInformPeriodEnable(boolean value)
{
	if(value == 1)
		strcpy(CwmpInformPeriodEnable,"Yes");
	else 
		strcpy(CwmpInformPeriodEnable,"No");
		
	setCwmpAttrValue(PrdEnable,CwmpInformPeriodEnable);
	return;
}

char InformPeriodInterval[12] = "60";
uint32  getInformPeriod(void)
{
	char *end;
	if(getCwmpAttrValue(PrdInterval,InformPeriodInterval,sizeof(InformPeriodInterval)) != 0)
	{
		tc_printf("\r\ngetInformPeriod error!");
		memset(InformPeriodInterval,'\0',sizeof(InformPeriodInterval));
		setCwmpAttrValue(PrdInterval,InformPeriodInterval);
	}
	
	return strtoul(InformPeriodInterval,&end,0);
}

void  setInformPeriod(uint32 value)
{
	sprintf(InformPeriodInterval,"%lu",value);
	//may be we need to judge if some ports(such telnet,ftp and so n) can be set!
	setCwmpAttrValue(PrdInterval,InformPeriodInterval);
	return ;
}


char cwmpUserName[256] = "default";
char* getCwmpUserName(void)
{
	if(getCwmpAttrValue(CPUsername,cwmpUserName,sizeof(cwmpUserName)) != 0)
	{
		tc_printf("\r\ngetCwmpUserName error!");
		memset(cwmpUserName,0,sizeof(cwmpUserName));
		setCwmpAttrValue(CPUsername,cwmpUserName);
	}
	return cwmpUserName;
}

char cwmpPassord[256] = "default";
char* getCwmpPassword(void)
{
	if(getCwmpAttrValue(CPPassword,cwmpPassord,sizeof(cwmpPassord)) != 0)
	{
		tc_printf("\r\ngetCwmpPassword error!");
		memset(cwmpPassord,0,sizeof(cwmpPassord));
		setCwmpAttrValue(CPPassword,cwmpPassord);
	}
	return cwmpPassord;
}

boolean  getCorrectACSPktLengthEnable(void)
{
	return CorrectACSPktLengthEnable;
}

void setCorrectACSPktLengthEnable(boolean val)
{
	CorrectACSPktLengthEnable = val;
}

int cwmpGetInformTimerID(void)
{
	return informTimerID;
}

void cwmpSetInformTimerID(int value)
{
	informTimerID = value;
}

boolean getCwmpInitFlag(void)
{
	return cwmpInitFlag;
}

void setCwmpInitFlag(boolean value)
{
	cwmpInitFlag = value;
}

char* getParameterKey(void)
{
	if (strlen(spSysParam.tcCwmpAttr->vParameterKey) > 32)
	{
		tc_printf("\r\n vParameterKey is not correct!");
		memset(spSysParam.tcCwmpAttr->vParameterKey, 0, 32);
	}
	
	return spSysParam.tcCwmpAttr->vParameterKey;
}

int setParameterKey(char* value)
{
	strcpy(spSysParam.tcCwmpAttr->vParameterKey, value);
	//SaveCwmpFlags();
	//spSave();
	return 0;
}

/* add by kunliu @ 20090624 */
#ifdef TELEFONICA_SPAIN_TR069
char* getMACAddress(char* value )
{
	chann_t 	*enet_p = NULL;

	enet_p = rmChannLookup("enet0");
	pether(value, ((etherChann_t *)enet_p)->ether_addr); 
	return value;
}
#endif

char devregUserName[256] = "trendchip";
char* getDeviceregUserName(void)
{
	if(CT_COM_DEVICEREG_VAL){
		memset(devregUserName, 0, sizeof(devregUserName));
		if( tcapi_get("deviceAccount_Entry","userName",devregUserName) != 0 )
		{
			memset(devregUserName,0,sizeof(devregUserName));
			strcpy(devregUserName,"errorNam");
		}
	}
	return devregUserName;
}

char devregUserPasswd[256] = "trendchip";
char* getDeviceregUserPasswd(void)
{
	if(CT_COM_DEVICEREG_VAL){
		memset(devregUserPasswd, 0, sizeof(devregUserPasswd));
		if( tcapi_get("deviceAccount_Entry","userPasswordDEV",devregUserPasswd) != 0 )
		{
			memset(devregUserPasswd,0,sizeof(devregUserPasswd));
			strcpy(devregUserPasswd,"errorPass");
		}
		if(TCSUPPORT_CT_PON_GD_VAL){
			if(0 == strlen(devregUserPasswd))
			{
				char devregUserNameClone[256] = {0};
				if( tcapi_get("deviceAccount_Entry","userName",devregUserNameClone) == 0 )
				{
					snprintf(devregUserPasswd,sizeof(devregUserPasswd),"%s",devregUserNameClone);
				}
				else
				{
					// do nothing
				}
			}
		}
	}
	return devregUserPasswd;
}

char loid[256] = "trendchip";
char* getLoid(void)
{
	if(TCSUPPORT_CY_E8_SFU_VAL || TCSUPPORT_CT_PON_NMG_VAL || TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_BIND2_VAL || TCSUPPORT_CUC_PON_SD_VAL){
		memset(loid, 0, sizeof(loid));
		if( tcapi_get("GPON_LOIDAuth","LOID",loid) != 0 )
		{
			if(TCSUPPORT_CUC_PON_SD_VAL == 0)
			{
				memset(loid,0,sizeof(loid));
				strcpy(loid,"errorLoid");
			}
		}

		if(TCSUPPORT_CT_PON_JS_VAL || TCSUPPORT_CT_PON_CZ_NX_VAL){
			if (loid[0] == '\0'){
				setLoidEmptyFlag(1);
			}
		}

		tc_printf("\r\nloid = %s", loid);
	}
	return loid;
}

extern char devreg_devType[32];
char *getECNDeviceType()
{
	char buf[32] = {0};
	tcapi_get("Cwmp_Entry", "deviceType", buf);
	if (strlen(buf) > 0)
		strcpy(devreg_devType, buf);
	
	return devreg_devType;
}

char devreg_accType[32] = "Epon";
char *getECNAccessType()
{
	char nodeName[33] = {0};
	char linkSta[8] = "0";
	int ponsw = 0;
	
	if (TCSUPPORT_CT_ECN_GZ_VAL || TCSUPPORT_CT_SIMCARD_SEPARATION_VAL || TCSUPPORT_CT_DEV_ACCESS_TYPE_VAL){
		strcpy(nodeName, "XPON_LinkCfg");
		if ( 0 != tcapi_get(nodeName, "LinkSta", linkSta) )
			strcpy(linkSta, "2");
		ponsw = atoi(linkSta);
		if ( 1 == ponsw ) // gpon
			strcpy(devreg_accType, "Gpon");
		else
			strcpy(devreg_accType, "Epon");
	}
	return devreg_accType;
}

char vacsUserName[256] = "default";
char* getCPEUserName(void)
{
	if(getCwmpAttrValue(CRUsername,vacsUserName,sizeof(vacsUserName)) != 0)
	{
		tc_printf("\r\ngetCPEUserName error!");
		memset(vacsUserName,0,sizeof(vacsUserName));
		setCwmpAttrValue(CRUsername,vacsUserName);
	}
	return vacsUserName;
}

void setCPEUserName(char* value)
{
	if(strlen(value) > 255)
	{
		tc_printf("\r\n ConnectionRequestUsername length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(CRUsername,value);
	return;
}

char vacsPassord[256] = "default";
char* getCPEPassword(void)
{
	if(getCwmpAttrValue(CRPassword,vacsPassord,sizeof(vacsPassord)) != 0)
	{
		tc_printf("\r\ngetCPEPassword error!");
		memset(vacsPassord,0,sizeof(vacsPassord));
		setCwmpAttrValue(CRPassword,vacsPassord);
	}
	return vacsPassord;
}

void setCPEPassword(char* value)
{
	if(strlen(value) > 255)
	{
		tc_printf("\r\n ConnectionRequestPassord length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(CRPassword,value);
	return;
}

int8 getACSInitiatedflag(void)
{
	return ACSInitiatedflag;
}

void setACSInitiatedflag(int8 value)
{
	ACSInitiatedflag = value;
	return;
}

int8  getBootTrapFlag(void)
{
	int8 bootTrapFlag;
	
	bootTrapFlag = spSysParam.tcCwmpAttr->vBootTrapFlag;
	return bootTrapFlag;
}

void  setBootTrapFlag(int8 value)
{
	spSysParam.tcCwmpAttr->vBootTrapFlag = value;
	if(value == 1)
	{
		//if ACS set Boot Trap Flag In Boottrap transaction, we do not need Reset BootTrap flag in WcHttp State Machine
		if(DelaySetBootTrapFlags == 1)
		{
			tc_printf("setBootTrapFlag: we do not need Reset BootTrap flag in WcHttp State Machine\n");
			DelaySetBootTrapFlags = 0;
		}
	}
	//SaveCwmpFlags();
	//spSave();
	return;
}

int getUnLockRealmsUserFlag(void)
{
	return UnLockRealmsUserFlag;
}

void setUnLockRealmsUserFlag(int value)
{
	UnLockRealmsUserFlag = value;
}

uint8 getSysBootFlag(void){
	return sysBootFlag;
}

void setSysBootFlag(uint8 value){
	sysBootFlag = value;
}

uint8 SetUploadErrType(uint8 flag)
{
	spSysParam.tcCwmpAttr->uploadErr = flag;
	return 0;
}

uint8 GetUploadErrType(void)
{
	return spSysParam.tcCwmpAttr->uploadErr;
}


uint8 SetDownloadErrType(uint8 flag)
{
	spSysParam.tcCwmpAttr->downloadErr = flag;
	return 0;
}


uint8 GetDownloadErrType(void)
{
	return spSysParam.tcCwmpAttr->downloadErr;
}

uint8 SetDownloadStart(uint8 flag)
{
	spSysParam.tcCwmpAttr->downloadStart= flag;
	return 0;
}


uint8 GetDownloadStart(void)
{
	return spSysParam.tcCwmpAttr->downloadStart;
}
uint8 SetUploadStart(uint8 flag)
{
	spSysParam.tcCwmpAttr->uploadStart= flag;
	return 0;
}


uint8 GetUploadStart(void)
{
	return spSysParam.tcCwmpAttr->uploadStart;
}

uint8 SetFileTransferCompleteFlag(uint8 flag)
{
/*
	TransferComplete (0x01)
	InternalError = 9002 (0x01<<1)
	DownloadFailure = 9010 (0x01<<2)
	FileTransferAuthenFailure = 9012 (0x01<<3)
	UnsupportedProtocol = 9013 (0x01<<4)
*/
	spSysParam.tcCwmpAttr->downloadComplete = flag;
	//SaveCwmpFlags();
	//spSave();
	return 0;
}


uint8 GetFileTransferCompleteFlag(void)
{
	return spSysParam.tcCwmpAttr->downloadComplete;
}

uint8 SetFileTransferTypeFlag(uint8 flag)
{
	spSysParam.tcCwmpAttr->downloadType = flag;
	//SaveCwmpFlags();
	//spSave();
	return 0;
}

uint8 GetFileTransferTypeFlag(void)
{
	return spSysParam.tcCwmpAttr->downloadType;
}

uint8 SetFileTransferStartTime(uint32 start)
{
	spSysParam.tcCwmpAttr->startTime = start;
	//SaveCwmpFlags();
	//spSave();
	return 0;
}

uint32 GetFileTransferStartTime(void)
{
	return spSysParam.tcCwmpAttr->startTime;
}


uint8 SetFileTransferCompleteTime(uint32 end)
{
	spSysParam.tcCwmpAttr->endTime = end;
	//SaveCwmpFlags();
	//spSave();
	return 0;
}

uint32 GetFileTransferCompleteTime(void)
{
	return spSysParam.tcCwmpAttr->endTime;
}

uint8 SetCommandKey(char* value, int index)
{
	int num = 0;
	
	if(strlen(value) > 32){
		return 1;
	}

	switch(index){
		case M_ScheduleInform:
			num = 0;
			break;
		case M_Reboot:
			num = 1;
			break;
		case M_Download:
			num = 2;
			break;
		case M_Upload:
			num = 3;
			break;
		default:
			return 1;
	}
	
	strcpy(spSysParam.tcCwmpAttr->commandKey[num], value);
	//SaveCwmpFlags();
	//spSave();
	return 0;
}

char* GetCommandKey(int index)
{
	int num = 0;

	switch(index){
		case M_ScheduleInform:
			num = 0;
			break;
		case M_Reboot:
			num = 1;
			break;
		case M_Download:
			num = 2;
			break;
		case M_Upload:
			num = 3;
			break;
		default:
			break;
	}
	
	if (strlen(spSysParam.tcCwmpAttr->commandKey[num]) > 33)
	{
		tc_printf("\r\n commandKey is not correct!");
		memset(spSysParam.tcCwmpAttr->commandKey[num], 0, 33);
	}
	
	
	return spSysParam.tcCwmpAttr->commandKey[num];
}

int SetSpecifiedCommandKey(char* value, int index){
	if(value == NULL)
		return -1;

	strcpy(spSysParam.tcCwmpAttr->commandKey[index], value);
	spSave();

	return 0;
}

int GetSpecifiedCommandKey(char* value, int index){
	if(value == NULL)
		return -1;
	
	if (strlen(spSysParam.tcCwmpAttr->commandKey[index]) > 33)
	{
		tc_printf("\r\n commandKey is not correct!");
		memset(spSysParam.tcCwmpAttr->commandKey[index], 0, 33);
	}
		
	strcpy(value, spSysParam.tcCwmpAttr->commandKey[index]);

	return 0;
}

int get_file_transfer_flag(void)
{
	return cwmp_download;
}

void set_file_transfer_flag(int value){
	cwmp_download = value;
	cwmpDownloadPreset();
	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(value == 1)
	 		 mw_downStart = 1;
	}
}

int8 get_mw_downStart_flag(void)
{
	return mw_downStart;
}

void set_mw_downStart_flag(int8 value){
	mw_downStart = value;	
}

int get_romfile_upLoad_flag(void)
{
	return cwmp_romfile_upLoad;
}

void set_romfile_upLoad_flag(int value){
	cwmp_romfile_upLoad = value;
	if(TCSUPPORT_CT_CWMP_ZIPROMFILE_VAL){
		if ( 0 == cwmp_romfile_upLoad )
			unlink(CT_CWMP_UPLOAD_ROMFILE_FILE_NAME);
	}
}

int get_firmwareupgrade_flag(void)
{
	return cwmp_firmwareupgrade;
}

void set_firmwareupgrade_flag(void){
	cwmp_firmwareupgrade = 1;
}

#ifdef SSL
uint8 get_ssl_check_mem_flag(void)
{
	return cwmp_ssl_check_mem;
}

void set_ssl_check_mem_flag(uint8 value){
	cwmp_ssl_check_mem = value;
}
#endif

//Added for Session Retry Policy, hqfeng, 2006.07.25
int  cwmpSessionRetryWaitIntervalCounter(void)
{
	cwmpSessionRetryWaitInterval++;
}

void cwmpResetSessionRetryWaitInterval(void)
{
	cwmpSessionRetryWaitInterval = 0;
}

int cwmpGetSessionRetryWaitInterval(void)
{
	return cwmpSessionRetryWaitInterval;
}

int cwmpGetSessionRetryFlag(void)
{
	return cwmpSessionRetryFlag;
}

void cwmpSetSessionRetryFlag(int value)
{
	cwmpSessionRetryFlag = value;
}

int cwmpGetSessionRetryCount(void)
{
	return cwmpSessionRetryCount;
}

void cwmpCalculateSessionRetryCount(int value)
{
	if (value == 1){//if the value==1,add 1 to the Retry Count.
		cwmpSessionRetryCount++;
	} else if (value == 0){ //if the value==0,reset the Retry Count.
		cwmpSessionRetryCount = 0;
	}
}

void cwmpSetInformCompleteFlag(int value)
{
	InformCompleteFlag = value;
}

int cwmpGetInformCompleteFlag(void)
{
	return InformCompleteFlag;
}

uint8 getRegITMSFlag(void)
{
	int RegSFlag = 0;
	int RegRFlag = 0;
	char tmp[8] = {0};

	if (tcapi_get("deviceAccount_Entry","registerStatus",tmp) != 0)
	{
		return 0;
	}
	RegSFlag = atoi(tmp);
	
	memset(tmp,0,sizeof(tmp));
	if (tcapi_get("deviceAccount_Entry","registerResult",tmp) != 0)
	{
		return 0;
	}
	RegRFlag = atoi(tmp);

	if ((0 == RegSFlag) && (1 == RegRFlag))
	{
		return 1;
	}
	
	return 0;
}

uint8 getBootFlag(void)
{
	return BootFlag;
}

void setBootFlag( uint8 value)
{
	BootFlag = value;
	return;
}

uint8 getActiveNotifyFlag(void)
{
	return ActiveNotifyFlag;
}

void setActiveNotifyFlag(uint8 value)
{
	ActiveNotifyFlag = value;
	return;
}

uint8 getPeriodicFlag(void)
{
	return PeriodicFlag;
}

void setPeriodicFlag(uint8 value)
{
	PeriodicFlag = value;
	return;
}

uint8 getIPPingCompleteFlag(void)
{
	if(rpc.flags.theInformFlag & INFORM_DIAGNOSTICSCOMPLETE){
		return 1;
	}else{
		return 0;
	}
}

#if 0
uint8 getInitialDeviceRegisterFlag(void)
{
	char tmp[16];
	int status = 0, result = 0;
	int times = 0, limit = 0;

	memset(tmp, 0, sizeof(tmp));
	if( tcapi_get("deviceAccount_Entry", "registerStatus",tmp) != 0)
	{
		return 0;
	}
	else
	{
		status = atoi(tmp);
	}
	
	memset(tmp, 0, sizeof(tmp));
	if( tcapi_get("deviceAccount_Entry", "registerResult",tmp) != 0)
	{
		return 0;
	}
	else
	{
		result = atoi(tmp);
	}

	memset(tmp, 0, sizeof(tmp));
	if( tcapi_get("deviceAccount_Entry", "retryTimes",tmp) != 0)
	{
		return 0;
	}
	else
	{
		times = atoi(tmp);
	}
	
	memset(tmp, 0, sizeof(tmp));
	if( tcapi_get("deviceAccount_Entry", "retryLimit",tmp) != 0)
	{
		return 0;
	}
	else
	{
		limit = atoi(tmp);
	}

	if( 
		( (0 == status) && (2 == result) &&  (times < limit) ) ||
		( (0 == status) && (99 == result) && (times < limit)  ) ||
		( (99 == status)  && (times < limit) )
	)
	{
		return 1;
	}
	
	return 0;
}
#endif

int getDevregInformFlag(void)
{
	int informFlag = 0;
	char tmp[8] = {0};

	if(CT_COM_DEVICEREG_VAL){	
		memset(tmp, 0, sizeof(tmp));
		if( tcapi_get("Cwmp_Entry", "devregInform",tmp) != 0)
		{
			return 0;
		}
		else
		{
			informFlag = atoi(tmp);
		}
	}
	return informFlag;
}

void setDevregInformFlag(int flag)
{
	if(CT_COM_DEVICEREG_VAL){
		if(flag)
			tcapi_set("Cwmp_Entry", "devregInform","1");
		else
			tcapi_set("Cwmp_Entry", "devregInform","0");
	}
	if(TCSUPPORT_CT_FJ_VAL){
		tcapi_save();
	}	
	return;
}

int getLongResetBtnFlag()
{
	char tmp[8] = {0};

	if(TCSUPPORT_CT_LONG_RESETBTN_VAL){
		memset(tmp, 0, sizeof(tmp));

		if( 0 != tcapi_get("Cwmp_Entry", "longresetbtn", tmp) )
			return 0;
		else
			return atoi(tmp);
	}
	return 0;
}

void setLongResetBtnFlag(int flag)
{
	if(TCSUPPORT_CT_LONG_RESETBTN_VAL){
		if( flag )
			tcapi_set("Cwmp_Entry", "longresetbtn", "1");
		else
			tcapi_set("Cwmp_Entry", "longresetbtn", "0");

		spSave();
	}
}

void setIPPingCompleteFlag(uint8 value)
{
	if(value == 0){
		rpc.flags.theInformFlag &= ~INFORM_DIAGNOSTICSCOMPLETE;
	}else{
		rpc.flags.theInformFlag |= INFORM_DIAGNOSTICSCOMPLETE;
	}

	return;
}

void setIADCompleteFlag(uint8 value)
{
	if(value == 0){
		rpc.flags.theInformFlag &= ~INFORM_DIAGNOSTICSCOMPLETE;
	}else{
		rpc.flags.theInformFlag |= INFORM_DIAGNOSTICSCOMPLETE;
	}

	return;
}


#ifdef TR143

void setDiagnosticsCompleteFlag(uint8 value)
{
	setIPPingCompleteFlag(value);
	return;
}
#endif

uint8 getDownloadRomFileFlag(void)
{
	if(rpc.flags.theSettingFlag & CWMP_DOWNLOAD_TCROM){
		return 1;
	}else{
		return 0;
	}
}

void setDownloadRomFileFlag(uint8 value)
{
	if(value == 0){
		rpc.flags.theSettingFlag &= ~CWMP_DOWNLOAD_TCROM;
	}else{
		rpc.flags.theSettingFlag |= CWMP_DOWNLOAD_TCROM;
	}

	return;
}

//For HoldRequest header, by hqfeng, 2006.08.14

boolean  getHoldRequestFlag(void){
	return gHoldRequestFlag;
}

void  setHoldRequestFlag(boolean  value){
	gHoldRequestFlag = value;
}

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0  //qingpu.20080110
boolean  getPreAllocTxBufFlag(void){
	//return cwmpPreAllocateTxBufFlag;
	return spSysParam.tcCwmpAttr->preAllocTxBufFlag;
}

void  setPreAllocTxBufFlag(boolean  value){
	//cwmpPreAllocateTxBufFlag = value;
	spSysParam.tcCwmpAttr->preAllocTxBufFlag = value;
}
#endif

char* getCustomerCA(void){
	return NULL;
}

char* getDeviceSummary(void)
{
	if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL)
	{
		/*here we should handle other device like usb device in future.*/
		return achieveDeviceSummary();
	}
	else
	{
	/*here we should handle other device like usb device in future.*/
		if(TCSUPPORT_WLAN_VAL)
			return "InternetGatewayDevice:1.0[](Baseline:1, IPPing:1, WiFiLAN:1)";
		else
			return "InternetGatewayDevice:1.0[](Baseline:1, IPPing:1)";
	}
}

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
uint32 getTxBufLenFromMMCT(void){
	sysData_t	*sd_p;

	sd_p = getSysData();
	return  sd_p->nCwmpTxBufLen;
}
#endif

#if 0
/*
*(1) if ConfigFlag equals 1, it means we want to get dev inf from config file /etc/devInf.conf.
*    if ConfigFlag equals 0, it means we want to get dev inf from romfile.
*(2) the function name getGetFromBootbaseFlag/setGetFromBootbaseFlag is merge from linos, 
*	 but it means we want to get devinfo from config file /etc/devInf.conf.
*/
#endif
int getUserCustomizeFlag(void)
{
	char cfgFlagStr[10];

	memset( cfgFlagStr, 0, sizeof(cfgFlagStr) ); 

	tcapi_get( "Cwmp_Entry", "DevInfCfgFlag", cfgFlagStr );
	
	if(strcmp(cfgFlagStr,"0") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int getGetFromBootbaseFlag(void)
{
	int rtnCode = 0;
	char cfgFlagStr[10];

	memset( cfgFlagStr, 0, sizeof(cfgFlagStr) ); 

	tcapi_get( "Cwmp_Entry", "DevInfCfgFlag", cfgFlagStr );
	
	rtnCode = atoi(cfgFlagStr);

	return rtnCode;
}

void  setGetFromBootbaseFlag(int  value)
{
	if ( value == 1 ) {
		tcapi_set( "Cwmp_Entry", "DevInfCfgFlag", "1" );
	}
	else if ( value == 0 ) {
		tcapi_set( "Cwmp_Entry", "DevInfCfgFlag", "0" );
	}
}

char cwmpManuName[64] = "test";
char* getManufacturer(void)
{
	if(getCwmpAttrValue(ManufacturerName,cwmpManuName,sizeof(cwmpManuName)) != 0)
	{
		tc_printf("\r\ngetManufacturer error!");
		memset(cwmpManuName,0,sizeof(cwmpManuName));
		setCwmpAttrValue(ManufacturerName,cwmpManuName);
	}
	return cwmpManuName;
}

int  setManufacturer(char *value)
{
	if(strlen(value) > 63)
	{
		tc_printf("\r\n manufacturer length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(ManufacturerName,value);
	return 0;
}

char cwmpSerialNum[129] = "123456654321";
char* getSerialNumValue(void)
{
	if(getCwmpAttrValue(SerialNumValue,cwmpSerialNum,sizeof(cwmpSerialNum)) != 0)
	{
		tc_printf("\r\ngetSerialNumValue error!");
		memset(cwmpSerialNum,0,sizeof(cwmpSerialNum));
		setCwmpAttrValue(SerialNumValue,cwmpSerialNum);
	}
	return cwmpSerialNum;
}

int  setSerialNumValue(char *value)
{
	if(strlen(value) > 63)
	{
		tc_printf("\r\n serialnum length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(SerialNumValue,value);
	return 0;
}

char cwmpModuleName[64] = "Gateway";
char* getModelName(void)
{
	if(getCwmpAttrValue(ModelName,cwmpModuleName,sizeof(cwmpModuleName)) != 0)
	{
		tc_printf("\r\ngetModelName error!");
		memset(cwmpModuleName,0,sizeof(cwmpModuleName));
		setCwmpAttrValue(ModelName,cwmpModuleName);
	}
	return cwmpModuleName;
}

int  setModelName(char *value)
{
	if(strlen(value) > 63)
	{
		tc_printf("\r\n module name length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(ModelName,value);
	return 0;
}

char cwmpDescription[256] = "HG111";
char* getDescription(void)
{
	if(getCwmpAttrValue(Description,cwmpDescription,sizeof(cwmpDescription)) != 0)
	{
		tc_printf("\r\ngetDescription error!");
		memset(cwmpDescription,0,sizeof(cwmpDescription));
		setCwmpAttrValue(Description,cwmpDescription);
	}
	return cwmpDescription;
}

int  setDescription(char *value)
{
	if(strlen(value) > 255)
	{
		tc_printf("\r\n description length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(Description,value);
	return 0;
}

char cwmpProductClass[64] = "HG111";
char* getProductClass(void)
{
	if(getCwmpAttrValue(ProductClass,cwmpProductClass,sizeof(cwmpProductClass)) != 0)
	{
		tc_printf("\r\ngetProductClass error!");
		memset(cwmpProductClass,0,sizeof(cwmpProductClass));
		setCwmpAttrValue(ProductClass,cwmpProductClass);
	}
	return cwmpProductClass;
}

int  setProductClass(char *value)
{
	if(strlen(value) > 63)
	{
		tc_printf("\r\n product class length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(ProductClass,value);
	return 0;
}

uint8 getConnRequestAuthFlag(void)
{
	char buf[5] = {0};
	#if 0
	if(spSysParam.tcCwmpAttr->flag & CWMP_CONN_REQUEST_AUTH){
		return 0;
	}else{
		return 1;
	}
	#endif
	tcapi_get("Cwmp_Entry","CRAuthFlag",buf);
	return (uint8)atoi(buf);
}

void setConnRequestAuthFlag(uint8 value)
{
	if(value == 1){
		spSysParam.tcCwmpAttr->flag &= ~CWMP_CONN_REQUEST_AUTH;
	}else{
		spSysParam.tcCwmpAttr->flag |= CWMP_CONN_REQUEST_AUTH;
	}
	//SaveCwmpFlags();
	//spSave();
	return;
}


uint8 getNoArrayTypeFlag(void)
{
	char arrayFlag[8];
	tcapi_get("Cwmp_Entry","arrayType",arrayFlag);
	return atoi(arrayFlag); 
}

void setNoArrayTypeFlag(uint8 value)
{
	noArrayTypeFlag = value;
	return;
}

int getFirmwareVersionFlag(void)
{
#if 0
	if(spSysParam.tcCwmpAttr->flag & CWMP_FIRMWARE_VERSION){
		return 1;
	}else{
		return 0;
	}
#endif
	if ( getUserCustomizeFlag() == 1 ) {
		return 1;
	}
	else {
		return 0;
	}
}

void setFirmwareVersionFlag(uint8 value)
{
#if 0
	if(value == 0){
		spSysParam.tcCwmpAttr->flag &= ~CWMP_FIRMWARE_VERSION;
	}else{
		spSysParam.tcCwmpAttr->flag |= CWMP_FIRMWARE_VERSION;
	}
	//SaveCwmpFlags();
	//spSave();
	return;
#endif
	if ( value == 1 ) {
		setGetFromBootbaseFlag( 0 );
	}
	else if ( value == 0 ) {
		setGetFromBootbaseFlag( 1 );
	}
}

char cwmpCustomerHWver[64] = "T_1.2.3.4";
char* getCustomerHardwareVer(void)
{
	if(getCwmpAttrValue(CusHWVersion,cwmpCustomerHWver,sizeof(cwmpCustomerHWver)) != 0)
	{
		tc_printf("\r\ngetCustomerHardwareVer error!");
		memset(cwmpCustomerHWver,0,sizeof(cwmpCustomerHWver));
		setCwmpAttrValue(CusHWVersion,cwmpCustomerHWver);
	}
	return cwmpCustomerHWver;
}

int  setCustomerHardwareVer(char *value)
{
	if(strlen(value) > 63)
	{
		tc_printf("\r\n customer hardware version length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(CusHWVersion,value);
	return 0;
}

uint8 getCwmpUpgradeManaged(void){

	return	spSysParam.tcCwmpAttr->vUpgradesManaged;
}

uint8 getFactoryResetFlag(void)
{
	if(rpc.flags.theSettingFlag & CWMP_FACTORY_RESET){
		return 1;
	}else{
		return 0;
	}
}

void setFactoryResetFlag(uint8 value)
{
	if(value == 0){
		rpc.flags.theSettingFlag &= ~CWMP_FACTORY_RESET;
	}else{
		rpc.flags.theSettingFlag |= CWMP_FACTORY_RESET;
	}
	return;
}
void setCwmpConnectionIndex(uint8 index){
	cwmpConnectionIndex = index;
}

uint8 getCwmpConnectionIndex(void){
	return cwmpConnectionIndex;
}
uint8 getScheduleInformFlag(void)
{
	if(rpc.flags.theInformFlag & INFORM_SCHEDULED){
		return 1;
	}else{
		return 0;
	}
}

void setScheduleInformFlag(uint8 value)
{
	if(value == 0){
		rpc.flags.theInformFlag &= ~INFORM_SCHEDULED;
	}else{
		rpc.flags.theInformFlag |= INFORM_SCHEDULED;
	}
	return;
}

/*______________________________________________________________________________
**	function name
**				getNoValueTypeFlag()
**	description:	get the value of noValueTypeFlag
**	parameters:
**	global:		noValueTypeFlag
**	return:		the value of noValueTypeFlag
**				1 means the packets without value types will not be filtered
**				0 means the packets without value types will be filtered
**	call:
**	revision:
**____________________________________________________________________________*/
boolean getNoValueTypeFlag(void)
{
	char buf[5] = {0};
	//return noValueTypeFlag;
	tcapi_get("Cwmp_Entry","ValueTypeFlag",buf);
	return (uint8)atoi(buf);
}

/*______________________________________________________________________________
**	function name
**				setNoValueTypeFlag()
**	description:	set the value of noValueTypeFlag,
**				1 means the packets without value types will not be filtered
**				0 means the packets without value types will be filtered		
**	parameters:	
**	global:		noValueTypeFlag
**	return:
**	call:
**	revision:
**____________________________________________________________________________*/
void setNoValueTypeFlag(boolean value)
{
	noValueTypeFlag = value;
	return;
}

boolean getFaultDetailFlag(void)
{
	return faultDetailFlag;
}

void setFaultDetailFlag(boolean value)
{
	faultDetailFlag = value;
	return;
}

int setDefaultPeriodicInformTime()
{

	if ((strlen(getcwmpPrdInformTime()) == 0)
		|| (!checkPeriodInformTimeFormat(getcwmpPrdInformTime())))
	{
		setcwmpPrdInformTime("0001-01-01T00:00:00Z");
		spSave();
	}

	return 0;
}

boolean checkPeriodicInformTime()
{
	if (!strncmp(getcwmpPrdInformTime(), "0001-01-01T00:00:00Z", 20))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


boolean isPITUnknownValue(char *strTime)
{
	if (!strncmp(strTime, "0001-01-01T00:00:00Z", 20))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void cwmpPeriodicInformTime(void)
{
	char strTemp[12];
	char strPeriodicInformTime[64];
	time_t			ltime;
	struct tm*		sTime;
	uint8			hour, minute, second, month, day;
	uint16			year;
	static int ii = 0;
	ii++;
	
	time(&ltime);
	sTime = localtime(&ltime);
	year = sTime->tm_year+1900;
	if (year < 2000)
		year = 2000;
	month = sTime->tm_mon+1;
	day = sTime->tm_mday;
	hour = sTime->tm_hour;
	minute = sTime->tm_min;
	second = sTime->tm_sec;
	strncpy(strTemp, getcwmpPrdInformTime(), 11);
	strTemp[11] = '\0';
	sprintf(strPeriodicInformTime, "%s%02d:%02d:%02d", strTemp,hour, minute, second);
	if (!strcmp(getcwmpPrdInformTime(), strPeriodicInformTime))
	{
		if(getInformPeriodEnable() && getInformPeriod()*1000  && !informTimerID)
		{
			tc_printf("cwmpPeriodicInformTime.\n");
			informTimerID = AddPeriodicInformTimer(getInformPeriod()*1000,  (TimerCallback)cwmpPeriodInform, NULL);
		}
	}
}

#ifdef TR069_CA_EXTENTION
/*______________________________________________________________________________
**	function name: getSPTCustomerCA
**
**	description:   this function is used to take the place of 
			      strcpy(ptr, spSysParam.tcCwmpAttr->customerCA), 
			      because customer CA is divided into two parts : customerCA
			      and customerCA_extra when customer CA is too large.
**	parameters:	
**	global:
**	return: 	 SPTCustomerCA
**	call:
**	revision:
**		
**____________________________________________________________________________*/
char* getSPTCustomerCA(void)
{
	if (CUSTOMER_CA_LENGTH == CUSTOMER_CA_LENGTH_V2)
	{
		memcpy(SPTCustomerCA, spSysParam.tcCwmpAttr->customerCA, 
				 	CUSTOMER_CA_LENGTH_V1);
		
		memcpy(SPTCustomerCA+CUSTOMER_CA_LENGTH_V1, 
					spSysParam.tcCwmpAttr_1->customerCA_extra, CUSTOMER_CA_LENGTH_EXTRA);

		return SPTCustomerCA;
	}
	else
	{
		return spSysParam.tcCwmpAttr->customerCA;
	}
}



/*______________________________________________________________________________
**	function name: setSPTCustomerCA	
**
**	description:   this function is used to take the place of 
			      strcpy(spSysParam.tcCwmpAttr->customerCA, ptr);
			      because customer CA is divided into two parts : customerCA
			      and customerCA_extra when customer CA is too large.
**	parameters:	
**	global:
**	return: 	 SPTCustomerCA
**	call:
**	revision:
**		
**____________________________________________________________________________*/
int setSPTCustomerCA(char *ptr)
{
	if (ptr == NULL)
	{
		tc_printf("\r\n setSPTCustomerCA(), pointer is NULL!");
		return -1;
	}
	
	if (strlen(ptr) >= CUSTOMER_CA_LENGTH)
	{
		tc_printf("\r\n setSPTCustomerCA(), CA length is out of range!");
		return -1;
	}
		
	if (CUSTOMER_CA_LENGTH == CUSTOMER_CA_LENGTH_V2)
	{
		memcpy(spSysParam.tcCwmpAttr->customerCA,ptr, CUSTOMER_CA_LENGTH_V1);
		
		if (strlen(ptr) >= CUSTOMER_CA_LENGTH_V1)
		{
			strncpy(spSysParam.tcCwmpAttr_1->customerCA_extra, ptr+CUSTOMER_CA_LENGTH_V1, CUSTOMER_CA_LENGTH_EXTRA);
			spSysParam.tcCwmpAttr_1->customerCA_extra[CUSTOMER_CA_LENGTH_EXTRA-1] = '\0';
		}			
	}
	else
	{
		 strcpy(spSysParam.tcCwmpAttr->customerCA, ptr);
	}	

	return 0;
}
#endif

//add by brian(merge from cwmpParameter.c,for test)
/********************test begin************************/
int isBooleanType(IN char *value)
{
	if(strcmp(value,"1") == 0
	   ||strcmp(value,"true") == 0
	   ||strcmp(value,"True") == 0
	   ||strcmp(value,"TRUE") == 0)
	{
		return 1;
	}

	if(strcmp(value,"0") == 0
	   ||strcmp(value,"false") == 0
	   ||strcmp(value,"False") == 0
	   ||strcmp(value,"FALSE") == 0)
	{
		return 0;
	}

	return -1;
}

boolean checkPeriodInformTimeFormat(char *sTime)
{
	int i;
	char strTime[32];
	char hour[3];
	char minute[3];
	char second[3];
	uint32 temp;
	int timeLen = strlen(sTime);
	if(isPITUnknownValue(sTime)){// Unknown Time check
		return TRUE;
	}
	
	if (timeLen < 19)
	{
		return FALSE;
	}
	strcpy(strTime, sTime);
	if ((strTime[4] != '-') || (strTime[7] != '-') || (strTime[13] != ':') 
		|| (strTime[16] != ':') || (strTime[10] != 'T'))
	{
		return FALSE;
	}
	for (i=0; i<timeLen; i++)
	{
		if (i==4 || i==7 ||i==13 || i==16 ||i==10 || i==19)
		{
			continue;
		}
		if (strTime[i] >'9' || strTime[i] <'0')
		{
			return FALSE;
		}
	}
	
	strncpy(hour, strTime+11, 2);
	hour[2] = '\0';
	temp = atoi(hour);
	if(temp >= 24){//hour should less than 24
		return FALSE;
	}
	strncpy(minute, strTime+14, 2);
	minute[2] = '\0';
	temp = atoi(minute);
	if(temp >= 60){//minute should less than 60
		return FALSE;
	}
	strncpy(second, strTime+17, 2);
	second[2] = '\0';
	temp = atoi(second);
	if(temp >= 60){//second should less than 60
		return FALSE;
	}

	return TRUE;
}
/********************test end************************/
/*below is needed*/
int isValidAddr(char *s)
{
	char c;
	if(s == NULL)
		return 0;
	
	while((c = *s++) != '\0')
	{
		if(!isdigit(c) && c!='.')
			return 0;
	}
	return 1;
}

void SignalInform(void)
{
 	sem_post(&SEM_IMFORM);
}


//add by brian
char cwmpHostName[256] = "default";
char* getCwmpHostName(void)
{
	if(getCwmpAttrValue(CPHostName,cwmpHostName,sizeof(cwmpHostName)) != 0)
	{
		tc_printf("\r\ngetCwmpHostName error!");
		memset(cwmpHostName,0,sizeof(cwmpHostName));
		setCwmpAttrValue(CPHostName,cwmpHostName);
	}
	return cwmpHostName;
}

void setCwmpHostName(char* value)
{
	if(strlen(value) > 255)
	{
		tc_printf("\r\n host name length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(CPHostName,value);
	return;
}

char cwmpAcsUrl[256] = "http://www.google.com";
char* getcwmpAcsUrl(void)
{
	if(getCwmpAttrValue(acsUrl,cwmpAcsUrl,sizeof(cwmpAcsUrl)) != 0)
	{
		tc_printf("\r\ngetcwmpAcsUrl error!");
		memset(cwmpAcsUrl,0,sizeof(cwmpAcsUrl));
		setCwmpAttrValue(acsUrl,cwmpAcsUrl);
	}
	return cwmpAcsUrl;
}

void setcwmpAcsUrl(char* value)
{
	if(strlen(value) > 255)
	{
		tc_printf("\r\nacs url length is not correct!");	
		return;
	}
	
	setCwmpAttrValue(acsUrl,value);
	return;
}

char cwmpPrdInformTime[33] = "0001-01-01T00:00:00Z";
char* getcwmpPrdInformTime(void)
{
	if(getCwmpAttrValue(prdInformTime,cwmpPrdInformTime,sizeof(cwmpPrdInformTime)) != 0)
	{
		tc_printf("\r\ngetcwmpAcsUrl error!");
		memset(cwmpPrdInformTime,0,sizeof(cwmpPrdInformTime));
		setCwmpAttrValue(prdInformTime,cwmpPrdInformTime);
	}
	return (char*)cwmpPrdInformTime;
}

void setcwmpPrdInformTime(char *value)
{
	if(strlen(value) > 32)
	{
		tc_printf("prdInfromTime length is not correct.\n");
	}
	setCwmpAttrValue(prdInformTime,value);
	return;
}
int cwmpDealWithFaultCode(int errorCode)
{
	switch(errorCode){
		case RequestDeny:
		case ACS_RequestDeny:
			setCwmpAttrValue(PrdInterval,CwmpInformPeriodOndDay);		
			//cwmpSetSessionRetryFlag(0);
			cwmpResetSessionRetryWaitInterval();
			cwmpCalculateSessionRetryCount(0);
			if(informTimerID)
			{
				timerPeriodicStop(informTimerID);
				cwmpSetInformTimerID(0);
			}
			else{
				startPeriodicInform();
			}
			break;	
		case ExceptError:
		case MethodNotSupport:		
		case InternalError:
		case InvalidArgument:
		case ResourceExceed:
		case InvalidParameterName:
		case InvalidParameterType:
		case InvalidParameterValue:
		case SetReadOnlyParameter:
		case RejectRequestNotification:
		case DownloadFailure:
		case UploadFailure:
		case FileTransferAuthenFailure:
		case UnsupportedProtocol:
		case InvalidRequestUsername:
		case NoGetValueFunction:
		case NoGetAttributeFunction:			
		case ACS_Error:
		case ACS_MethodNotSupport:		
		case ACS_InternalError:
		case ACS_InvalidArgument:
		case ACS_ResourceExceed:
		case ACS_RetryRequest:				
			break;	
		default:
			break;			
	}
	return 0;
}

#if 1   //IPPING_DIAGNOSTICS
int isInRouteTable(char *ip, char *dev){
	char routePath[] = "/tmp/tmp_ipping_diagnostic_route";
	char cmdbuf[200] = {'\0'}, buf[200] = {'\0'}, tmp[200] = {'\0'};
	char routeInfo[7][16];
	int route_number = 0;
	FILE *fp = NULL;
	enum routeInfo_en{dstIP=0,gw,mask,metric,use,iface,usr_def};

	sprintf(cmdbuf, "/sbin/route -n > %s", routePath);
	system(cmdbuf);
	memset(routeInfo,0, sizeof(routeInfo));
	fp = fopen(routePath, "r");
	if(fp != NULL)
	{
		while (fgets(buf, sizeof(buf), fp))
		{
			if(route_number > 1)
			{
				sscanf(buf, "%s %s %s %s %s %s %s %s",
					routeInfo[dstIP], routeInfo[gw], routeInfo[mask],
					tmp, routeInfo[metric], tmp,
					routeInfo[use], routeInfo[iface]);
				if(strcmp(routeInfo[dstIP], ip) == 0 && strcmp(routeInfo[iface], dev) == 0){ //is aleardy in route table
					fclose(fp);					
					return 1;	
				}
			}
			route_number++;	
		}
		fclose(fp);
	}
	unlink(routePath);
	return 0;
}
int
get_entry_number(char *buffer, char *keyword, int *number){
	char *tmp=NULL;
	int test=0;
	if((buffer == 0)||(keyword == 0)||(number == 0)){
		return -1;
	}
	if(strstr(buffer,keyword)){
		tmp=buffer+strlen(keyword);
		(*number)=atoi(tmp);
		test=atoi(tmp);
		return 0;
	}
	else{
		return -1;
	}
}
int get_wanindex_by_name(char *wan_if_name)
{
	int pvc_index = -1, entry_index = -1;
	int if_index = -1;
	char pvc[5] = {0}, entry[5] = {0};

	if(strstr(wan_if_name, "ppp")){  /*ppp interface*/
		if(get_entry_number(wan_if_name,"ppp",&if_index) != 0){
			return -1;
		}		
	}
	else{   /*nas interface,ext:nas0_1*/
		sscanf(wan_if_name, "nas%1s_%1s", pvc, entry);	
		if(strlen(pvc) == 0 || strlen(entry) == 0){
			return -1;
		}
		if_index = atoi(pvc) * MAX_SMUX_NUM + atoi(entry);
	}
	if(if_index < 0 || if_index > 64)  //PVC_NUM*MAX_SMUX_NUM=8*8
		return -1;
	
	return if_index;
}
#endif

int checkDNSExist(void){
	int if_index;
	int rtDelStatus = 0;
	char nodeName[33] = {'\0'};
	char cmd_rmRoute[200] = {'\0'};
	char dns[20];

	if(TCSUPPORT_CT_INFORM_NODE_VAL){
		if((if_index = get_wanindex_by_name(cwmp_interface)) < 0){
			return -1;
		}

		sprintf(nodeName, "WanInfo_Entry%d", if_index);
		memset(dns, 0, sizeof(dns));
		tcapi_get(nodeName, "DNS", dns);
		
		if(_checkIp(dns)){
			return -1;
		}
	}

	return 0;
}

int cwmpCheckWanIP(void){
	int i, pvc_index, entry_index, ret, flag=0;
	char nodeName[50], wan_node[50],  externalIP[32],	service_app[SERVICE_LIST_LEN];
	
	if(TCSUPPORT_CT_INFORM_NODE_VAL){
		for (i = 0; i < MAX_WAN_INFO_ENTRY_NUM; i++) {
			pvc_index = i / MAX_PVC_NUM;
			entry_index = i - pvc_index * MAX_PVC_NUM;
			sprintf(wan_node, "Wan_PVC%d_Entry%d", pvc_index, entry_index);
			
			ret = tcapi_get(wan_node, "ServiceList", service_app);
			if (ret < 0) {
				continue;
			}
			if(TCSUPPORT_CT_SERVICELIST_E8C_VAL){
				if	( (strstr(service_app, TR069) == NULL)) {
					continue;
				}
			}
			else{
				if  ( (0 != strcmp(service_app, TR069)) && (0 != strcmp(service_app, TR069_INTERNET)) ) {		
					continue;
				}
			}

			flag = 1;

			sprintf(nodeName, "WanInfo_Entry%d", i);
			ret = tcapi_get(nodeName, "IP", externalIP);
			if (ret == 0 && !_checkIp(externalIP)) {
				return 0;
			}

			if(TCSUPPORT_CT_PPP_ONDEMAND_VAL)
				checkCanConnectDemandWan(wan_node, nodeName);
		}

		if(flag == 1 && i ==  MAX_WAN_INFO_ENTRY_NUM){
			tcapi_set("Cwmp_Entry","Inform_Status","2");       //wan connect is not available
		}
		else if( i ==  MAX_WAN_INFO_ENTRY_NUM){
			tcapi_set("Cwmp_Entry","Inform_Status","1");       //wan connect no tr69 route
		}
	}
	return -1;
}

extern int AlarmNumberInfo[11];
int getSimCardRstFailAlarmFlag()
{
	char tmp[8] = {0};
	int i;

	if(TCSUPPORT_CT_ALARMMONITOR_VAL){	
		memset(tmp, 0, sizeof(tmp));
		if( 0 != tcapi_get("Cwmp_Entry", "simcardrstfail", tmp) )
			return 0;
		else{
			for(i = 0; i < 10; i++){
				if(AlarmNumberInfo[i] == 56){
					break;
				}
			}
			if(i == 10){
				for(i = 0; i < 10; i++){
					if(AlarmNumberInfo[i] == 0){
						AlarmNumberInfo[i] = 56;
						AlarmNumberInfo[10]++;
						break;
					}
				}
			}
			return atoi(tmp);
		}
	}
	return 0;
}

void setSimCardRstFailAlarmFlag(int flag)
{
	if(TCSUPPORT_CT_ALARMMONITOR_VAL){
		if( flag )
			tcapi_set("Cwmp_Entry", "simcardrstfail", "1");
		else
			tcapi_set("Cwmp_Entry", "simcardrstfail", "0");

		spSave();
	}
}

void setDeviceAlarmFlag(boolean value)
{
	gDeviceAlarmFlag = value;
	return;
}

boolean getDeviceAlarmFlag(void)
{
	return gDeviceAlarmFlag;
}

void setDeviceClearAlarmFlag(boolean value)
{
	gDeviceClearAlarmFlag = value;
	return;
}

boolean getDeviceClearAlarmFlag(void)
{
	return gDeviceClearAlarmFlag;
}

void setDeviceMonitorFlag(boolean value)
{
	gDeviceMonitorFlag = value;
	return;
}

boolean getDeviceMonitorFlag(void)
{
	return gDeviceMonitorFlag;
}

void setUPnPDMDownloadFlag(boolean value)
{
	if(TCSUPPORT_CT_UPNP_DM_VAL)
		gUPnPDMDownloadFlag = value;
	return;
}

boolean getUPnPDMDownloadFlag(void)
{
	return gUPnPDMDownloadFlag;
}


#if 0//def CT_COM
int getCRAuthTimeout(void)
{
	#define SERVER_TIMEOUT 10
	
	char buf[64] = {0};
	int to_val = 0;
	if(tcapi_get("Cwmp_Entry", "CRAuthTimeout", buf) != 0)
	{
		tc_printf("\r\n get attribute(CRAuthTimeout) error, set default 10 second!\n");
		to_val = SERVER_TIMEOUT;
	}
	else
	{
		to_val = atoi(buf);
		if( (to_val < 0) || (to_val > 60) )
		{
			to_val = 10;
		}
	}
	return to_val;
}
#endif

int cwmpUPnPDMRoutine(void)
{
	char *objectName = "InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.";
	DeviceNodePtr DeviceListNode = NULL;
	DeviceNodePtr rootptr = NULL;
	
	if(TCSUPPORT_CT_UPNP_DM_VAL){
		tcdbg_printf("%s:=================>\n",__FUNCTION__);
		pthread_mutex_lock(&value_change_lock);
		if(inform_task_start == 1){
			/* if inform task is running, quit and do nothing */
	//			signal_valueChg_flag = 1; //spSave() will call this task again, no need set flag to re-do it.
			pthread_mutex_unlock(&value_change_lock);
			tcdbg_printf("\r\n inform is performing.... set value change flag.\r\n ");
			return -1;
		}else{
			rootptr = rpc.IGDDeviceRootPtr;
			rpc.IGDDeviceRootPtr = NULL;
			pthread_mutex_unlock(&value_change_lock);
		}

		cwmpSearchDeviceNodeByName(objectName, rootptr, &DeviceListNode);
		if(DeviceListNode == NULL){
			tcdbg_printf("%s:search node fail\n",__FUNCTION__);
			return -1;
		}

		//Del/Add object
		if(doUPnPRoutineActionList_Open(DeviceListNode)){
			tcdbg_printf("%s:do doUPnPRoutineActionList() fail!!!\n",__FUNCTION__);
			return -1;
		}
		
		pthread_mutex_lock(&value_change_lock);
		rpc.IGDDeviceRootPtr = rootptr;	
		pthread_mutex_unlock(&value_change_lock);
	}
	return 0;
}

void getLoidValue(char* loid){
	char linkSta[32] = {0};
	int ponsw = 0;
	
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		tcapi_get("XPON_LinkCfg", "LinkSta", linkSta);
		ponsw = atoi(linkSta);				
		if ( 2 == ponsw ){ // epon
			tcapi_get("GPON_LOIDAuth", "LOID", loid);
		}
		else{
			tcapi_get("EPON_LOIDAuth", "LOID0", loid);
		}

		tcdbg_printf("[%s:%d]loid=%s\n",__FUNCTION__,__LINE__,loid);
	}
	return;
}
				

int checkMonitorCollectorEnable(void) {
	char enableValue[8]= {0};

	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		/* Check MonitorCollector is enable or not */
		tcapi_get(MONITOR_COLLECTOR_COMMON, "Enable", enableValue);
		if( strcmp(enableValue, "1") == 0)
			return 0;
	}

	return -1;
}


int checkAndInitMonitorCollector(void) {
	int i;

	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		/* Allocate memory for struct of each monitor entry */	
		if((gMonitorCollectorNodeList = (MonitorCollectorNodeInfoPtr)os_malloc(MONITOR_COLLECTOR_MAXNUM* sizeof(MonitorCollectorNodeInfo))) == NULL){
			tcdbg_printf("\n==>cwmpMonitorCollectorRoutine Error, malloc request Monitor struct fails!\r\n");
			return -1;
		}
		
		/* Init struct */
		for(i=0; i<MONITOR_COLLECTOR_MAXNUM; i++) {
			memset(&gMonitorCollectorNodeList[i], 0, sizeof(MonitorCollectorNodeInfo));
		}
	}

	return 0;
}

//if more than 3 files delete the oleest one
int checkMonitorCollectorFiles(int instance){
	char fileDir[64] = {0};
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	int num = 0;
	char oldestFile[64] = {0};
	char filePath[256] = {0};
	char* p1 = NULL;
	char* p2 = NULL;

	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		sprintf(fileDir,"%s%d",MONITOR_COLLECTOR_DIR,instance);
		if(!(dir = opendir(fileDir))){
			tcdbg_printf("[%s:%d]opendir fail!!! need create dir filePath=%s\n",__FUNCTION__,__LINE__,fileDir);

			if(!(dir = opendir(MONITOR_COLLECTOR_DIR))){
				if(mkdir(MONITOR_COLLECTOR_DIR, 0777) != 0){
					tcdbg_printf("[%s:%d]mkdir fail!!! filePath=%s\n",__FUNCTION__,__LINE__,MONITOR_COLLECTOR_DIR);
					return FAIL;
				}
			}
			
			if(mkdir(fileDir, 0777) != 0){
				tcdbg_printf("[%s:%d]mkdir fail!!! filePath=%s\n",__FUNCTION__,__LINE__,fileDir);
				return FAIL;
			}
			
			if(!(dir = opendir(fileDir))){
				tcdbg_printf("[%s:%d]opendir fail!!!filePath=%s\n",__FUNCTION__,__LINE__,fileDir);
				return FAIL;
			}
		}

		while(entry = readdir(dir)){		
			if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
				continue;

			if(strlen(oldestFile)== 0){
				memset(oldestFile,0,sizeof(oldestFile));
				strcpy(oldestFile,entry->d_name);
			}else{
				p1 = strstr(oldestFile,"-status");
				p2 = strstr(entry->d_name,"-status");
				if(p1 == NULL || p2 ==NULL){
					continue;
				}

				if(strcmp(p1,p2) > 0){
				memset(oldestFile,0,sizeof(oldestFile));
				strcpy(oldestFile,entry->d_name);
			}
			}

			num++;
		}

		if(num >= 3){
			sprintf(filePath,"%s/%s",fileDir,oldestFile);
			tcdbg_printf("[%s:%d]num >= 3,remove filePath %s\n",__FUNCTION__,__LINE__,filePath);
			remove(filePath);
		}

		closedir(dir);
	}
	return SUCCESS;
}

void removeOldFilesByIndex(int instance){		
	char filePath[128] = {0};
	char fileDir[64] = {0};	
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		sprintf(fileDir,"%s%d",MONITOR_COLLECTOR_DIR,instance);
		if(!(dir = opendir(fileDir))){
			tcdbg_printf("[%s:%d]filePath=%s not exist\n",__FUNCTION__,__LINE__,fileDir);
			return;
		}

		
		while(entry = readdir(dir)){		
			if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
				continue;
		
			sprintf(filePath,"%s%d/%s",MONITOR_COLLECTOR_DIR,instance,entry->d_name);
			remove(filePath);
		}
	}
	return;
}

int saveMonitorCollectorStatueFile(char* path){
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	char filePath[128] = {0};
	char cmdBuf[512] = {0};
	
	if(!(dir = opendir(MONITOR_COLLECTOR_DIR))){
		tcdbg_printf("[%s:%d]opendir fail!!!filePath=%s\n",__FUNCTION__,__LINE__,MONITOR_COLLECTOR_DIR);
		return FAIL;
	}
	
	while(entry = readdir(dir)){		
		if(strstr(entry->d_name,"status") != NULL){
			/*find status file, del it first*/
			sprintf(filePath,"%s%s",MONITOR_COLLECTOR_DIR,entry->d_name);
			tcdbg_printf("[%s:%d]del filePath=%s\n",__FUNCTION__,__LINE__,filePath);
			remove(filePath);
		}	
	}

	tcdbg_printf("[%s:%d]save filePath=%s\n",__FUNCTION__,__LINE__,path);
	sprintf(cmdBuf, "cp -rf %s %s", path, MONITOR_COLLECTOR_DIR);
	system(cmdBuf);

	return 0;
}

int doTftpRoutine(int instance){		
	char nodeName[32] = {0};
	char strServerUrl[128] = {0};
	char strServerIP[32] = {0};
	char strTftpPort[32] = {0};
	char cmdBuf[256] = {0};	
	char filePath[128] = {0};
	char fileDir[64] = {0};
	long offset =  0;
	DIR *dir = NULL;
	struct dirent *entry = NULL;
	char interface[32] = {0};
	ip4a dnsQueryResult = 0;	
	struct sockaddr_in addr = {0};
	char tftpRet[8] = {0};
	
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		sprintf(nodeName, MONITOR_COLLECTOR_COMMON, instance);
		tcapi_get(nodeName, "reportUrl", strServerUrl);
		tcapi_get("CwmpRoute_Entry", "ifName0", interface);
		
		memset(cmdBuf, 0, sizeof(cmdBuf));
		sprintf(cmdBuf, "/bin/echo \"%s %s\" > %s", strServerUrl, interface, MONITOR_COLLECTOR_CONF_PATH);
		system(cmdBuf);
		dnsQueryResult = resolveName(strServerUrl);
		unlink(MONITOR_COLLECTOR_CONF_PATH);	
		
		if(!dnsQueryResult)
		{
			tcdbg_printf("[%s:%d]resolveName fail!!!strServerUrl=%s\n",__FUNCTION__,__LINE__,strServerUrl);
			return FAIL;
		}
		else
		{
			strcpy(strServerIP,inet_ntoa(dnsQueryResult));
		}
		
		tcapi_get(nodeName, "TftpPort", strTftpPort);
		sprintf(fileDir,"%s%d",MONITOR_COLLECTOR_DIR,instance);
		if(!(dir = opendir(fileDir))){
			tcdbg_printf("[%s:%d]opendir fail!!!filePath=%s\n",__FUNCTION__,__LINE__,fileDir);
			return FAIL;
		}
		while(entry = readdir(dir)){	
			offset = entry->d_off;
		}
		if(offset <= 0)
			return FAIL;
		rewinddir(dir);

		while(entry = readdir(dir)){		
			if(!strcmp(entry->d_name,".") || !strcmp(entry->d_name,".."))
				continue;
			if(entry->d_off != offset)
				continue;
		
			tcapi_set(MONITOR_COLLECTOR_COMMON,"startMonitorCollect","1");
			tcapi_set(MONITOR_COLLECTOR_COMMON,"tftpRet","0");
			sprintf(cmdBuf,"%s %s%d %s %s %s",MONITOR_COLLECTOR_TFTP_PATH,MONITOR_COLLECTOR_DIR,instance,entry->d_name,strServerIP,strTftpPort);
		//	tcdbg_printf("[%s:%d]cmdBuf=%s\n",__FUNCTION__,__LINE__,cmdBuf);
			system(cmdBuf);		
			tcapi_get(nodeName, "tftpRet", tftpRet);
			if(strcmp(tftpRet,"1") == 0){
				tcdbg_printf("Do tftp success, remove file:%s \n",entry->d_name);
				sprintf(filePath,"%s%d/%s",MONITOR_COLLECTOR_DIR,instance,entry->d_name);
				if(TCSUPPORT_CT_PON_GDV20_VAL){
				saveMonitorCollectorStatueFile(filePath);
				}
				remove(filePath);
				tcapi_set(MONITOR_COLLECTOR_COMMON,"tftpRet","0");
			}
			
			tcapi_set(MONITOR_COLLECTOR_COMMON,"startMonitorCollect","0");		

			if(--offset <= 0)
				break;
			rewinddir(dir);
		}
	}
	return SUCCESS;
}

int isNTPSeverSyncSuccess(){
	char ret[8]= {0};

	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		/* Check MonitorCollector is enable or not */
		tcapi_get(MONITOR_COLLECTOR_COMMON, "ntpSync", ret);
		if( strcmp(ret, "1") == 0 )
			return 1;
	}
	return 0;
}

int writeMonitorCollectorFile(char* filePath, int instance,int num){
	DeviceNodePtr currentDeviceNode = NULL;	
	ParameterType paraType;	
	char* curParaValue = NULL;
	char paraListValue[256] = {0};	
	char nodeName[32] = {0};	
	char attr[32] = {0};
	FILE *fp = NULL;	
	int k = 0;
	
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		fp = fopen(filePath, "w+");
		if(fp == NULL){ 				
			tcdbg_printf("[%s:%d]fopen filePath=%s fail\n",__FUNCTION__,__LINE__,filePath);
			return FAIL;
		}

		memset(nodeName, 0, sizeof(nodeName));
		sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, instance);

		if(TCSUPPORT_CT_PON_CZ_GDCS_VAL)
			pthread_mutex_lock(&value_change_lock);
		while(k < num){ 	
			memset(paraListValue, 0, sizeof(paraListValue));		
			sprintf(attr,"ParaList%d",k);
			tcapi_get(nodeName, attr, paraListValue);
			
			/* Search node and get value */
			currentDeviceNode = NULL;
			cwmpSearchDeviceNodeByName(paraListValue, rpc.IGDDeviceRootPtr, &currentDeviceNode);
			if(currentDeviceNode == NULL){					
				tcdbg_printf("%s, search node fail\n",__FUNCTION__,__LINE__,paraListValue); 				
				k++;
				continue;
			}

			if(currentDeviceNode->getValueInfo){
				currentDeviceNode->getValueInfo(&curParaValue, &paraType, currentDeviceNode);
			}

			if(curParaValue == NULL){
				tcdbg_printf("%s, getValueInfo fail\n",__FUNCTION__,__LINE__,paraListValue);
				k++;
				continue;
			}

			fputs(paraListValue,fp);
			fputs("=\"",fp);					
			fputs(curParaValue,fp);
			fputs("\";\r\n",fp);	
			os_free(curParaValue);
			curParaValue = NULL;
			k++;
		}
		if(TCSUPPORT_CT_PON_CZ_GDCS_VAL)
			pthread_mutex_unlock(&value_change_lock);
		fclose(fp);
	}
	return 0;
}

void cwmpMonitorCollectorRoutine(void) {
	char tmpBuf[1024] = {0};
	char nodeName[32] = {0};	
	int i, j, ret = 0;
	int periodTime[MONITOR_COLLECTOR_MAXNUM] = {0};
	DeviceNodePtr currentDeviceNode = NULL;	
	char* curParaValue = NULL;	
	struct timeval now;
	time_t			ltime;
    struct  tm*		sTime;
	char filePath[256] = {0};
	FILE *fp = NULL;
	int num = 0;
	char loid[32] = {0};
		
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		if(checkMonitorCollectorEnable() < 0) {
			tcdbg_printf("[%s:%d]Error, Monitor collector not enable\n",__FUNCTION__,__LINE__);
			return;
		}

		if( gMonitorCollectorNodeList == NULL ) {		
			tcdbg_printf("[%s:%d]Error, Monitor collector node not allocate\n",__FUNCTION__,__LINE__);
			return;
		}

		if(!isNTPSeverSyncSuccess()){
			tcdbg_printf("[%s:%d]NTP Server Sync is not Success!!!\n",__FUNCTION__,__LINE__);
			return;
		}

		for(i=0; i<MONITOR_COLLECTOR_MAXNUM; i++) {
			memset(nodeName, 0, sizeof(nodeName));
			memset(tmpBuf, 0, sizeof(tmpBuf));
			sprintf(nodeName, MONITOR_COLLECTOR_ENTRY, i);
			ret = tcapi_get(nodeName, "ParaListNum", tmpBuf);
			if(ret < 0 || (num = atoi(tmpBuf)) == 0) {
				continue;
			}

			/* Get timeList*/
			memset(tmpBuf, 0, sizeof(tmpBuf));
			tcapi_get(nodeName, "TimeList", tmpBuf);
			if(strlen(tmpBuf) != 0)
				periodTime[i] = atoi(tmpBuf) * 60;

			if (i == 0)
			{
				gMinSleepSec = periodTime[i];
			}
			else
			{
				if (periodTime[i] < gMinSleepSec)
				{
					gMinSleepSec = periodTime[i];
				}
			}
			
			gettimeofday(&now, NULL);			
			if(gMonitorCollectorNodeList[i].startTime.tv_sec == 0) {
				gMonitorCollectorNodeList[i].startTime.tv_sec = now.tv_sec;
			}
			else {
				if((now.tv_sec - gMonitorCollectorNodeList[i].startTime.tv_sec) >= periodTime[i]) {
					gMonitorCollectorNodeList[i].startTime.tv_sec = now.tv_sec;
					checkMonitorCollectorFiles(i+1);
					
					time(&ltime);
					sTime = localtime(&ltime);
					
					getLoidValue(loid);				
					if (TCSUPPORT_CT_PON_CY_JX_VAL)
						sprintf(filePath, MONITOR_COLLECTOR_PATH_JX, i+1, getCwmpOUI(), getCwmpSerialNum(), loid,i+1,sTime->tm_year + 1900,  
						(sTime->tm_mon + 1), sTime->tm_mday, sTime->tm_hour, sTime->tm_min,  sTime->tm_sec);	
					else
					sprintf(filePath, MONITOR_COLLECTOR_PATH, i+1,loid,i+1,sTime->tm_year + 1900,  (sTime->tm_mon + 1), sTime->tm_mday, sTime->tm_hour, sTime->tm_min,  sTime->tm_sec);	
					writeMonitorCollectorFile(filePath,i,num);
					doTftpRoutine(i+1);
				}
			}
		}

		gettimeofday(&now, NULL);
		for(i=0; i<MONITOR_COLLECTOR_MAXNUM; i++) {
	                if (gMonitorCollectorNodeList[i].startTime.tv_sec + periodTime[i] < now.tv_sec)
			{
				gMinSleepSec = 0;
				return;
			}
			if (gMonitorCollectorNodeList[i].startTime.tv_sec + periodTime[i] - now.tv_sec < gMinSleepSec)
			{
				gMinSleepSec = gMonitorCollectorNodeList[i].startTime.tv_sec + periodTime[i] - now.tv_sec;
			}
		}
	}
	return;
}

void *cwmpMonitorCollectorTask(void){
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		while(1){

			/* If Monitor Collector are not enable, break */
			if(checkMonitorCollectorEnable() < 0)
				break;
			
			/* Start Monitor Collector Routine*/
			cwmpMonitorCollectorRoutine();

			sleep(gMinSleepSec);
		}
		
		/*Set p_MonitorCollector varible to 0 means thread exit*/
		tcdbg_printf("\n======>cwmpMonitorCollectorTask, do thread exit\n");
		p_MonitorCollector = 0;
	}
}

int cwmpMonitorCollectorTaskStop(void)
{
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		if (0 == p_MonitorCollector)
		{
			return -1;
		}
		
		if (0 == pthread_cancel(p_MonitorCollector))
		{
			p_MonitorCollector = 0;
			return 0;
		}
	}
	return -1;	
}

int cwmpMonitorCollectorTaskStart(void)
{
	int ret = 0;
	pthread_attr_t p_MonitorCollectorHandle_attr;
	int retMonitorCollector = -1;
	
	if(TCSUPPORT_CT_MONITORCOLLECTOR_VAL){
		/* Check Monitor Collector is enable or not */
		retMonitorCollector = g_MonitorCollectorEnable;
		if(retMonitorCollector == 0) {
			/*get the start time*/
			//gettimeofday(&gMonitorCollectorLast, NULL);

			/*check entry num and allocate memory for each entry*/
			checkAndInitMonitorCollector();
		}
		else {
			tcdbg_printf("\n==>cwmpMonitorCollectorTaskStart Error, Monitor Collector  not enable no need to creat task\n");
			return -1;
		}		

		/*create thread*/
		ret = pthread_attr_init(&p_MonitorCollectorHandle_attr);
		if(ret != 0)
		{
			tcdbg_printf("\r\nThread(cwmpMonitorCollectorTaskStart) attribute creation fail!");
			return -1;
		}

		ret = pthread_attr_setdetachstate(&p_MonitorCollectorHandle_attr,PTHREAD_CREATE_DETACHED);
		if(ret != 0)
		{
			tcdbg_printf("\r\nThread(cwmpMonitorCollectorTaskStart):Set attribute fail!");
			return -1;
		}

		ret = pthread_create(&p_MonitorCollector,&p_MonitorCollectorHandle_attr,(void *)cwmpMonitorCollectorTask,NULL);
		if(ret != 0)
		{
			tcdbg_printf("\r\nThread(cwmpMonitorCollectorTaskStart):Create thread fail!");
			return -1;
		}

		pthread_attr_destroy(&p_MonitorCollectorHandle_attr);
	}
	return ret;
}

//struct wanIndexRecord usedWanIndex = {-1};
void initUsedWanIndex(){
	char nodeName[32] = {0};	
	char active[16] = {0};
	int ret = -1;	
	int i = 0;
	int	j = 0;	
	char lastUsedPVCIndex[8] = {0};
	char lastUsedEntryIndex[8] = {0};
	int pvcIndex = -1;
	int entryIndex = -1;
	char wan_pvc_node[32] = {0};

	if(TCSUPPORT_CT_WANNINDEX_INCREASE_VAL){	
		ret = tcapi_get("Wan_Common","lastUsedPVCIndex",lastUsedPVCIndex);	
		if(ret == 0){
			tcdbg_printf("[%s]lastUsedPVCIndex is already exist, no need to init again!!!\n",__FUNCTION__);
			return;
		}
		
		//usedWanIndex.pvcIndex = -1;	
		for(i = 0; i < TOTAL_REMOTE_DEVICE_NODE; i++) {
			//usedWanIndex.entryIndex[i] = -1;
			entryIndex = -1;
			for(j = 0; j < TOTAL_REMOTE_ENTRY_NODE; j++) {
				memset(active, 0, sizeof(active));
				
				sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
				ret = tcapi_get(nodeName, "Active", active);
				if(ret < 0){
					continue;
				}else {
					//usedWanIndex.entryIndex[i] = j;
					entryIndex = j;
					sprintf(wan_pvc_node,"Wan_PVC%d",i);
					sprintf(lastUsedEntryIndex,"%d",entryIndex);
					tcapi_set(wan_pvc_node,"lastUsedEntryIndex",lastUsedEntryIndex);	
				}
			}
			
			//if(usedWanIndex.entryIndex[i] != -1){
			if(entryIndex != -1){
				//usedWanIndex.pvcIndex = i;
				pvcIndex = i;
				sprintf(lastUsedPVCIndex,"%d",pvcIndex);
				tcapi_set("Wan_Common","lastUsedPVCIndex",lastUsedPVCIndex);	
			}
		}
		
		//dumpUsedWanIndex();
	}
}

int findUnusedPvcIndex(){
	char wan_node[32] = {0};
	char tmpValue[32] = {0};
	int i = 0;
	int ret = -1;
	char lastUsedPVCIndex[8] = {0};
	int pvcIndex = -1;
	
	if(TCSUPPORT_CT_WANNINDEX_INCREASE_VAL){
		ret = tcapi_get("Wan_Common","lastUsedPVCIndex",lastUsedPVCIndex);
		if (ret < 0) {
			pvcIndex = -1;
		}else{
			pvcIndex = atoi(lastUsedPVCIndex);
		}

		for (i = pvcIndex + 1; i < TOTAL_REMOTE_DEVICE_NODE; i++) {		
			sprintf(wan_node, "Wan_PVC%d", i);
			
			if(TCSUPPORT_CT_PON_VAL)
				ret = tcapi_get(wan_node, "VLANMode", tmpValue);
			else{
				if(TCSUPPORT_CT_WAN_PTM_VAL)
					ret = tcapi_get(wan_node, "PTMEnable", tmpValue);
				else
					ret = tcapi_get(wan_node, "VPI", tmpValue);
			}
		
			if (ret < 0) {
				return i;
			}
		}


		for (i = 0; i < pvcIndex; i++) {		
			sprintf(wan_node, "Wan_PVC%d", i);
			
			if(TCSUPPORT_CT_PON_VAL)
				ret = tcapi_get(wan_node, "VLANMode", tmpValue);
			else{
				if(TCSUPPORT_CT_WAN_PTM_VAL)
					ret = tcapi_get(wan_node, "PTMEnable", tmpValue);
				else
					ret = tcapi_get(wan_node, "VPI", tmpValue);
			}
		
			if (ret < 0) {
				return i;
			}
		}
	}

	return TOTAL_REMOTE_DEVICE_NODE;
}

int findUnUsedEntryIndex(int pvcIndex){
	char wan_node[32] = {0};
	char tmpValue[32] = {0};
	int i = 0;
	int ret = -1;

	char lastUsedPVCIndex[8] = {0};
	char lastUsedEntryIndex[8] = {0};
	int entryIndex = -1;

	if(TCSUPPORT_CT_WANNINDEX_INCREASE_VAL){
		sprintf(wan_node, "Wan_PVC%d", pvcIndex);
		ret = tcapi_get(wan_node, "lastUsedEntryIndex", lastUsedEntryIndex);
		if (ret < 0) {
			entryIndex = -1;
		}else{
			entryIndex = atoi(lastUsedEntryIndex);
		}
		
		for (i = entryIndex+1; i < TOTAL_REMOTE_ENTRY_NODE; i++) {
			sprintf(wan_node, "Wan_PVC%d_Entry%d", pvcIndex, i);
			ret = tcapi_get(wan_node, "Active", tmpValue);
			if (ret < 0) {
				return i;
			}
		}

		for (i = 0; i < entryIndex; i++) {
			sprintf(wan_node, "Wan_PVC%d_Entry%d", pvcIndex, i);
			ret = tcapi_get(wan_node, "Active", tmpValue);
			if (ret < 0) {
				return i;
			}
		}
	}

	return TOTAL_REMOTE_ENTRY_NODE;
}

char bridge_pppusername[65] = {0};
int  br_pppconnection_stat = 0 ;
char* getBrpppusername(void)
{
	if(TCSUPPORT_CT_PPPINFORM_VAL){
	FILE *fp = NULL;
	int i = 0;
	char line_buf[128] = {0}, line_string[2][128] = {0}, pppusername_temp[20] = {0};
	
	fp=fopen("/proc/tc3162/pppusername","r");
	if(fp==NULL){
		return NULL;
	}
	while(fgets(line_buf, 128, fp) != NULL && (i < 2)){
		if(line_buf[0] != '\n'){
			/*replace "\n" by NULL*/
			strtok(line_buf, "\n");
			strcpy(line_string[i], line_buf);
		}
		else{
			strcpy(line_string[i], "");
		}
		i++;
		memset(line_buf, 0, sizeof(line_buf));
	}
	
	fclose(fp);
	if(strstr(line_string[0], "up") != NULL){
		br_pppconnection_stat = 1;
		strcpy(bridge_pppusername, line_string[1]+10);		   //string format: Username: xxxx
	}
	
	return bridge_pppusername;
}
}

int8 cwmpBuildAlarmParameters(void){
	int i, ret = 0;
	char tmpBuf[MAX_ATTRIBUTE_LEN] = {0};
	char nodeName[32] = {0};
	int8 state = 0;
	char enableValue[8]= {0};
	int maxEntryNum = 0;
	char tmpBuf1[10] = {0};
	int8 simAlarmflag = 0;

	if ( TCSUPPORT_CT_ALARMMONITOR_VAL ){
		if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
		{
			memset(tmpBuf, 0, sizeof(tmpBuf));
			for(i = 0; i < 10; i++) {
				if(AlarmNumberInfo[i] == 0)
					continue;
				sprintf(tmpBuf1, "10%d00%d", AlarmNumberInfo[i]/10, AlarmNumberInfo[i]%10);
				strcat(tmpBuf, tmpBuf1);
				if(i != AlarmNumberInfo[10]-1)
					strcat(tmpBuf, ",");
			}
			
			if(sizeof(tmpBuf) != 0){
				simAlarmflag = 1;
				state =cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_Alarm.AlarmNumber", tmpBuf, StringType);
				if (state!=0) {
					tc_printf("\r\n==>cwmpBuildValueChangeParameters() fails!");
					return -1;
				}
			}
		}

		if(checkAlarmEnable() < 0) {
			tc_printf("\n==>cwmpBuildAlarmParameters Error, Alarm not enable\n");

			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && simAlarmflag)
				return 0;

			return -1;
		}

		if( gAlarmNodeList == NULL ){

			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && simAlarmflag)
				return 0;

			return -1;
		}
		
		maxEntryNum = checkAlarmEntryNum();

		for(i=0; i<maxEntryNum; i++) {
			if(gAlarmNodeList[i].usedFlag == 0)
				continue;
			
			if(gAlarmNodeList[i].typeFlag == ALARM_FLAG || gAlarmNodeList[i].typeFlag == CLEARALARM_FLAG) {
				memset(nodeName, 0, sizeof(nodeName));
				memset(tmpBuf, 0, sizeof(tmpBuf));
						
				sprintf(nodeName, ALARM_ENTRY, i);
				tcapi_get(nodeName, "ParaList", tmpBuf);
				if(strlen(tmpBuf) == 0)
					continue;
				
				state =cwmpAddInformParameter(tmpBuf, gAlarmNodeList[i].paramValue, gAlarmNodeList[i].type);
				if (state!=0) {
					tc_printf("\r\n==>cwmpBuildValueChangeParameters() fails!");
					return -1;
				}

				if(gAlarmNodeList[i].typeFlag == CLEARALARM_FLAG) {
					gAlarmNodeList[i].typeFlag = ALARM_NONE_FLAG;
					gAlarmNodeList[i].startTime.tv_sec = 0;				
				}
					
			}
		}
	}
	return 0;
}

int8 cwmpBuildMonitorParameters(void){
	int i, ret = 0;
	char tmpBuf[MAX_ATTRIBUTE_LEN] = {0};
	char nodeName[32] = {0};
	int8 state = 0;
	char enableValue[8]= {0};
	int maxEntryNum = 0;
	int j=0,idx = 0,count = 0;
	char parasValue[10][MAX_ATTRIBUTE_LEN] = {0};

	if ( TCSUPPORT_CT_ALARMMONITOR_VAL ){
		if(checkMonitorEnable() < 0)  {
			tc_printf("\n==>cwmpBuildMonitorParameters Error, Monitor not enable\n");
			return -1;
		}
		
		if( gMonitorNodeList == NULL )
			return -1;

		maxEntryNum = checkMonitorEntryNum();

		for(i=0; i<maxEntryNum; i++) {
			if(gMonitorNodeList[i].usedFlag == 0) {
				continue;
			}
			if(gMonitorNodeList[i].typeFlag == MONITOR_FLAG) {
				memset(nodeName, 0, sizeof(nodeName));
				memset(tmpBuf, 0, sizeof(tmpBuf));
						
				sprintf(nodeName, MONITOR_ENTRY, i);
				tcapi_get(nodeName, "ParaList", tmpBuf);
				if(strlen(tmpBuf) == 0)
					return -1;
				if(TCSUPPORT_CMCC_VAL){
					memset(parasValue,0,10*MAX_ATTRIBUTE_LEN);
					idx = 0;
					count = 0;
					for(j=0;j<strlen(tmpBuf);j++)
					{
						if(tmpBuf[j] == ','){
							if(idx == 0)
								strncpy(parasValue[count],tmpBuf,j-idx);
							else
								strncpy(parasValue[count],tmpBuf+idx+1,j-idx-1);
							idx = j;
								if(strlen(parasValue[count]) != 0)
								{
									count++;
								}
							j++;
						}
					}
					if(idx < (strlen(tmpBuf)-1)){
						if(idx == 0)
							strcpy(parasValue[count],tmpBuf+idx);
						else
							strcpy(parasValue[count],tmpBuf+idx+1);
						if(strlen(parasValue[count]) == 0)
							count--;
					}
					for(j=0;j<count+1;j++)
					{
						state =cwmpAddInformParameter(parasValue[j], gMonitorNodeList[i].paramValue[j], gMonitorNodeList[i].type[j]);
						if (state!=0) {
							tc_printf("\r\n==>cwmpBuildValueChangeParameters() fails!");
							return -1;
						}
					}
				}
				else{
					state =cwmpAddInformParameter(tmpBuf, gMonitorNodeList[i].paramValue[0], gMonitorNodeList[i].type[0]);
					if (state!=0) {
						tc_printf("\r\n==>cwmpBuildValueChangeParameters() fails!");
						return -1;
					}
				}
				gMonitorNodeList[i].typeFlag = MONITOR_NONE_FLAG;
					
			}
		}
	}

	return 0;
}

