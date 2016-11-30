/*
** $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/private/TR69_64/cwmp/Sources/cwmpRpc.c#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
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
** $Log: cwmpRpc.c,v $
** Revision 1.17  2011/08/19 13:19:53  janey
** [Bugfix]:#11369
** un open code compile option issue
**
** Revision 1.16  2011/08/05 01:53:48  xyzhu_nj
** #11006: ct com merge to main trunk.(phase 3)
** 1. Add TCSUPPORT_COMPILE tag for ct com compile option;
** 2. Delete files in filesystem that CT_COM not used;
** 3. Merge the lastes bugfix from ct com branch to main trunk.(TLB7.1.2.20-51 -- 57);
** 4. update mic.
**
** Revision 1.15  2011/07/29 10:30:01  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.(phase 2)
**
** Revision 1.14  2011/07/29 04:35:29  xmdai_nj
** #10739:Support to interactive with Arcadyan ACS success.
**
** Revision 1.13  2011/05/26 02:24:17  xyzhu_nj
** #bug7877:
** 1. Add support syslog save to flash;
** 2. Add reomte syslog server;
** 3. Display and save  by log level.
**
** Revision 1.12  2011/04/22 05:00:14  xmdai_nj
** #9816:Add a new interface to deal with the error code from ACS.
**
** Revision 1.11  2011/04/08 05:57:22  xmdai_nj
** #9784:Power off during firmware upload, CPE can't send event code of "7 TRANSFER COMPLETE" and "M Upload" after reboot.
**
** Revision 1.10  2011/03/17 13:51:42  xmdai_nj
** #9463:tr69 download/upload starttime complete should be current CPE time
**
** Revision 1.9  2011/02/10 05:55:05  agyi_sz
** #8759
**
** Revision 1.8  2011/01/18 13:27:48  xmdai_nj
** #8553:Power off during TR69 download firmware, CPE did not send download error information to ACS after CPE power on
**
** Revision 1.7  2010/12/30 03:22:41  lyxue_sz
** upgrade firmware for 16M
**
** Revision 1.6  2010/12/29 12:25:17  xflu_sz
** merge code from branch_sz to main trunk
** bug: #2132&7710,7655,7480
**
** Revision 1.5  2010/12/22 04:09:44  rclv_nj
** bug 8131
**
** Revision 1.4  2010/12/22 01:38:09  xhshi
** #8124
**
** Revision 1.3  2010/11/23 13:18:41  xmdai_nj
** #7757:ParameterKey still be updated when RPC method failed
**
** Revision 1.2  2010/10/26 09:34:47  xmdai_nj
** #7325:processes will be killed when doing upload in ACS.
**
** Revision 1.1.1.1  2010/09/30 21:00:12  josephxu
** apps/public, private
**
** Revision 1.1.1.1  2010/04/09 09:42:46  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.6  2010/02/03 07:23:10  xyzhu_nj
** [#bug4703]: Modify commit or delay commit mechanism on tr69 app.
**
** Revision 1.5  2010/02/02 08:22:29  feiyan
** [Feature] BugID:4710 Reduce TR069 Thread number from 5 to 1.
**
** Revision 1.4  2010/02/01 10:29:42  xyzhu_nj
** [#bug4703]: Modify commit or delay commit mechanism on tr69 app.
**
** Revision 1.3  2010/01/15 13:26:01  yzwang_nj
** [Bug#4616] Add TR069 Parameters IGD.Time.
**
** Revision 1.2  2009/12/30 06:49:29  xhshi
** [#4363]
** Implement  tr069 engine in linux platform and remove some debug messages
**
** Revision 1.1  2009/12/24 12:21:29  feiyan
** [Feature] BugID:4407
** 1.Implement TR069 and TR064 Engine under Linux
**  2.Implement TR069 RPC Methods(Reboot, Factory Reset, Download, Upload)
**
** Revision 1.33  2009/08/20 11:22:47  taoliu
** #3393:ZyXEL ACS 3.0 will exit current session if faultstring is too long
**
** Revision 1.32  2009/08/14 10:19:22  taoliu
** #3142,Modem has 30K available memory, do SSL download firmware, crash.
**
** Revision 1.31  2009/07/30 09:34:09  feiyan
** [BugFix] ID:3290 MT880a can not connect with TTNET Motive ACS successfully.
**
** Revision 1.30  2009/07/03 13:34:46  taoliu
** #2764, The second root cause of TR069 crash
**
** Revision 1.29  2009/06/19 13:36:05  taoliu
** #2404
** implement some TR069 parameter about DHCP option 60 and 240;
**
** Revision 1.28  2009/04/23 07:52:10  jlshi
** Merge from TC2 Branch for bug #2440
**
** Revision 1.27  2009/04/03 10:34:35  hzbian
** Open Source
**
** Revision 1.26  2009/03/20 01:13:16  xhshi
** [ENHANCEMENT]
** #1821,#1241
** merge from bharti branch and add other new layer2bridging parameters
**
** Revision 1.25  2009/03/05 12:51:22  taoliu
** #2132: add some TR069 parameters:InternetGatewayDevice.QueueManagement.
**
** Revision 1.24  2009/02/13 10:45:05  taoliu
** #1796, #1980
** ACS set a non-writable parameter value, fault code format is wrong
**
** Revision 1.23  2008/11/26 11:38:27  hzbian
** [BUGFIX]#1077 #1082
** Interface is not correct, but diagnostics is still performed.
** "8 DIAGNOSTICS COMPLETE" is not taken out in periodic inform
** after diagnostics  is finished successfully.
**
** Revision 1.22  2008/11/26 08:38:32  hzbian
** [BUGFIX]#1077 #1082
** Interface is not correct, but diagnostics is still performed.
** "8 DIAGNOSTICS COMPLETE" is not taken out in periodic inform
** after diagnostics  is finished successfully.
**
** Revision 1.21  2008/10/22 08:46:53  jqzchu
** BugFree ID:713(merge from huawei)
** Unplugging the phone line when downloading firmware by WorksSystem ACS, download failure but ACS returns download success after CPE restarting.
** see [DOC NO]:HRM5.0.78.0-9
**
** Revision 1.20  2008/10/09 01:58:31  hzbian
** [BUGFIX]ACS keeps waiting for response and can not get the response after it sets some parameters.
**
** Revision 1.19  2008/09/26 04:14:38  hzbian
** Fix a bug that CPE can not download a firmware from a HTTP server
** with digest authentication enabled.
**
** Revision 1.18  2008/08/29 02:19:01  hzbian
** Merge code from HW to TC2 for issues about upload,
** Connection Request URLand memory leakage
**
** Revision 1.17  2008/08/07 10:27:12  qingpu
** Enhancement:Support TR069 302 Redirect function.(Merge from Branch)
**
** Revision 1.16  2008/04/11 08:10:39  qingpu
** Bug fix & Enhancement:
** 1.set ATM Qos type via TR069, its value on webpage do not update.
** 2.some TR069 parameter type is not correct.
** 3.add some node for WKS.
**
** 4.add a cmd "sys cwmp cushwversion [value]" to config customer Hardware version.
**
** Revision 1.15  2008/03/31 05:38:29  qingpu
** bug fix:the format of unkown time in TransferResponse packet is error.
**
** Revision 1.14  2008/01/10 10:47:00  qingpu
** Enhancement:close some TR69 ci-commands for code size.
**
** Revision 1.13  2008/01/07 06:33:25  qingpu
** bug fix:stress test crash when TR069 enabled(merge from Huawei trunk)
**
** Revision 1.12  2007/12/29 16:32:37  qingpu
** workaround: Webpage can't access caused by merging TO2 crash problems.
**
** Revision 1.10  2007/09/24 10:55:48  qingpu
** bug fix(merge from Huawei trunc):if cpe use static route for TR069, the connectRequestURL is not correct, and external wan ip sent to ACS will be lan ip if don't have default route.
**
** Revision 1.9  2007/07/27 07:37:33  maojianli
** Merge 2_11_3_0 back to main trunk
**
** Revision 1.8  2007/02/28 03:43:04  xzwang
** Enhancement: Realize Upload RPC.
** Enhancement: Realize ScheduleInform RPC.
** Fix bug: Memory leaks while SOAP fault occurs.
** Fix bug: Can not handle SOAP fault packet from ACS correctly while IOT with Comtrend ACS.
**
** Revision 1.7  2007/02/26 08:22:16  xzwang
** Fix bug: The scope of int and unsignedInt is not correct in function cwmpSetDeviceNodeValue.
**
** Revision 1.6  2007/02/26 02:01:04  xzwang
** Fix bug: The time format in TransferComplete packet is incorrect.
**
** Revision 1.5  2007/02/26 01:45:03  xzwang
** Fix bug: Fails in downloading romfile while IOT with WorksSystem ACS.
**
** Revision 1.4  2007/02/08 02:27:54  hqfeng
** -Bug fixed: The negative and 0 can be set to the parameters of Unsigned int type.
**
** Revision 1.3  2007/01/26 11:28:31  xzwang
** Fix bug: After download romfile, CPE sends DownloadResponse not TransferComplete to
**               ACS.
**
** Revision 1.2  2007/01/17 06:18:28  xzwang
** - Fix bug: Many global variables Which are used by multi thread are not defined as
** 	volatile varible.
** - Fix bug: If we plug off adsl line during CPE send packet to ACS and then plug on,
** 	CPE will send wrong packet to ACS since then.
**
** Revision 1.1.1.1  2006/12/29 08:27:27  josephxu
** LiNOS transfer from Hsinchu to Suzhou (20061229)
**
** Revision 1.6  2006/12/28 09:40:58  yen
** Merge newest tr069 code from Huawei trunk to Taiwan trunk to fix some bugs.
**
** Revision 1.5  2006/12/25 05:29:41  yen
** Fix bug:  CPE(P3/L3) will crash during run download RPC though ACS.
**
** Revision 1.4  2006/12/08 14:35:25  yen
** - Fix bug: Dowload firmware  over SSL fails.
** - Fix bug: ACS can not set write-only parameter(like password) attribute to be passive notification.
**
** Revision 1.3  2006/12/05 12:08:16  yen
** - Merge TR-069 code after 10/1/2006 into Taiwan Trunk.
**
** Revision 1.2  2006/10/24 07:20:12  yen
** - Fix bug: If GPV on one invalid parameter whose length is longer than 40 and without ".",
**              the cpe will reboot.
**
** Revision 1.1  2006/09/30 08:40:17  yen
** Merge TR-069 and TR-064 code into trunk.
**
** Revision 1.58  2006/09/27 06:30:34  xzwang
** move some download code from cwmpParameter.c/cwmpRpc.c to cwmp.c
**
** Revision 1.57  2006/09/20 02:50:45  xzwang
** Return fault code if set attribute to be active for some parameters.
**
** Revision 1.56  2006/09/18 03:06:06  xzwang
** Use "ReleaseSoapDoc" to free request ducument.
**
** Revision 1.55  2006/09/13 01:02:03  xzwang
** Do not stop periodic inform timer during download.
**
** Revision 1.54  2006/09/07 12:31:58  xzwang
** - Support download CA by download function.
** - Only disable periodic inform during download.
**
** Revision 1.53  2006/09/07 02:41:30  xzwang
** - Reject to set some specific parameters like DeviceInfo.Uptime to be active notification while the device node's flag is 0.
** - Alway delete old node from active and passive notification list and then add it if the new attribute is not off.
**
** Revision 1.52  2006/09/05 12:26:04  xzwang
** - Return correct value for get rpc method in inform response function.
** - Do not disable periodic inform after receive download request of delay mode.
**
** Revision 1.51  2006/09/01 05:32:48  xzwang
** Resolve the bug that get paramter value/name on entire object will cause
** modem reboot by the following means:
**  - Use os_malloc/os_free to replace malloc/free.
**  - Write a function os_str_realloc to replace realloc.
**  - Write a function cwmp_strdup to replace strdup.
**  - If malloc fails, Free response structure and document before build fault structure.
**  - Pre-allocate Tx buffer during cwmp initializaion.
**
** Revision 1.50  2006/08/28 06:58:39  hqfeng
** - Add for HoldRequest.
**
** Revision 1.49  2006/08/27 05:34:49  xzwang
** Add define C0 for C0 version.
**
** Revision 1.48  2006/08/25 07:14:40  xzwang
** - Use cwmpGetCurACSInfo to replace WcParaseURL to get current hot address.
**
** Revision 1.47  2006/08/22 10:00:24  xzwang
** - set attribute of portmapping rule to be relative with wannode index.
** -  Clease the attribute to be 0 for the new added object.
**
** Revision 1.46  2006/08/16 05:51:27  xzwang
** - Fix one bug in value change mechanism.
**
** Revision 1.45  2006/08/15 13:32:21  xzwang
** Modify startTime and CompleteTime to be empty.
**
** Revision 1.44  2006/08/15 10:20:59  xzwang
** no message
**
** Revision 1.42  2006/08/15 08:12:55  xzwang
** Enhance retry mechanism.
**
** Revision 1.41  2006/08/14 08:28:29  haijian
** Allocate downloadstruct when init global variable rpc.
**
** Revision 1.40  2006/08/14 06:15:03  hqfeng
** - Add Session Retry Policy
** - Reduce code size.
**
** Revision 1.39  2006/08/11 06:46:45  xzwang
** Realize value change inform mechanism for forced active notification parameters.
**
** Revision 1.38  2006/08/11 01:01:48  haijian
** no message
**
** Revision 1.37  2006/08/03 06:39:34  xzwang
** - Modify value change inform mechanism to resolve the problem that CPE can not send
**   value change inform to ACS if we add/delete object after set attributes.
**
** Revision 1.36  2006/08/01 05:54:35  xzwang
** - do not judge "detail" node as array type.
**
** Revision 1.35  2006/07/31 07:39:33  xzwang
** - Add fault argument in TransferComplete RPC.
**
** Revision 1.34  2006/07/31 04:21:01  xzwang
** - Extract fault information of ACS fault code correctly.
**
** Revision 1.33  2006/07/26 08:08:29  haijian
** Add save ParameterKey in flash.
**
** Revision 1.32  2006/07/21 14:39:32  hqfeng
** - Merge the code passed the test in UNH.
**
** Revision 1.31  2006/07/14 01:45:48  xzwang
** modify "Manufature" to be "Manufaturer".
**
** Revision 1.30  2006/07/13 05:44:01  xzwang
** - judge pointer whether is null before free it.
**
** Revision 1.29  2006/07/12 01:40:20  hqfeng
** - Modify for GetRPCMethods(CPE->ACS)
**
** Revision 1.28  2006/07/11 08:52:07  haijian
** Add UNH CA support.
** - Add ci-command: sys cwmp ca [0|1]
**
** Revision 1.27  2006/07/11 06:37:54  xzwang
** - Set attribute for all parameters under partial object.
**
** Revision 1.26  2006/07/11 03:17:08  xzwang
** - judge whether the parameter's deleteobject/addobject func is NULL before reference it.
**
** Revision 1.25  2006/07/10 12:13:01  xzwang
** - add commandKey for "M reboot"  inform.
** - do not response fault while set attribute for readonly parameter.
**
** Revision 1.24  2006/07/07 03:10:35  xzwang
** - allcate rxBuf dynamically instead of array.
**
** Revision 1.23  2006/07/07 03:06:57  xzwang
** - modify some problem in download rpc:
**    1. the status value is wrong in download resoponse.
**    2. Without commandKey in TransferComplete packet.
**    3. delay seconds is not got correctly.
**    4. Time format does not meet spec requirement.
** - resolve the problem that can not set multiParamter one time.
** - resolve crash prolem while set the value of ACS username and password.
**
** Revision 1.22  2006/07/06 08:38:57  hqfeng
**  - Modify for Fault Code.
**
** Revision 1.21  2006/07/05 07:24:47  hqfeng
**  - Modify for InvalidRequestUsername.
**
** Revision 1.20  2006/07/02 08:11:29  xzwang
** mark the modification of hqfeng in fault handle.
**
** Revision 1.19  2006/07/02 05:38:49  xzwang
** - modify download mechanism by haijian.
** - do not parse SOAP-Header: NoMoreRequest for it is not handled correctly.
** -  resolve crash problem while we receive error packets and not able to fetch paramter name.
**
** Revision 1.18  2006/06/29 07:45:09  hqfeng
**  - Recover the getCwmpRebootFlag() and setCwmpRebootFlag().
**
** Revision 1.17  2006/06/28 06:11:26  hqfeng
** - Modify for dealing with InvalidRequestUsername.
**
** Revision 1.16  2006/06/28 03:18:40  hqfeng
** - Modify the getCwmpRebootFlag() for Reboot.
**
** Revision 1.15  2006/06/27 07:54:18  haijian
** Rewrite the function which is used to get parameters from DownloadRequest.
**
** Revision 1.14  2006/06/26 10:14:35  haijian
** Enhance download(not test);
**
** Revision 1.13  2006/06/26 05:26:48  haijian
** Add SSLv3 support.
**
** Revision 1.12  2006/06/25 09:21:06  xzwang
** Modify CWMP_RX_BUF_SIZE from 2048 to 3072.
**
** Revision 1.11  2006/06/24 13:46:55  xzwang
** User pointer replace array in parameter structure to reduce memory occupation.
**
** Revision 1.10  2006/06/23 11:13:23  ylwang
** - Add Port Mapping (Another Method of Dynamic Allocation Memory) .
** - Add/delete Object .
**
** Revision 1.9  2006/06/22 12:46:20  xzwang
** no message.
**
** Revision 1.8  2006/06/22 07:03:31  xzwang
** resolve the problem that response flag can not be set 1.
**
** Revision 1.7  2006/06/22 03:03:41  xzwang
** add function "cwmpGetFaultParaList()"
** enhance period inform mechanism.
**
** Revision 1.6  2006/06/22 01:31:47  hqfeng
** - Modify for dealling with (ignor) the ACS Fault Code.
**
** Revision 1.5  2006/06/21 09:51:02  penny
** merge from 3612 Trunk
**
** Revision 1.4  2006/06/21 07:10:25  hqfeng
** - Replace InternalError by ResourceExceed.
** - If it is an ExceptError, we return an InternalError .
**
** Revision 1.3  2006/06/20 09:21:50  hqfeng
** - Add processTheHostAddress() in cwmpRpcInformResponseResponse() for
**  dealing with BOOTSTRAP.
**
** Revision 1.2  2006/06/19 13:05:16  xzwang
** merge with the newest code of old trunk.
**
** Revision 1.1.1.1  2006/06/19 12:25:55  hqfeng
** - Created 3162L/L2 trunk for TR-069(Merge to new trunk).
**
** Revision 1.31  2006/06/15 11:01:34  xzwang
** do not display "InformResponse" and "TransferCompleteResponse" for GetRPCMethods.
**
** Revision 1.30  2006/06/15 10:40:43  xzwang
** modify function name from "cwmpCommandParse_new" to "cwmpParseSoapPaket".
**
** Revision 1.29  2006/06/14 13:51:52  xzwang
**  - make debug message clear.
**  - add inform task.
**
** Revision 1.28  2006/06/14 06:06:06  xzwang
** - Realize value change inform function.
**
** Revision 1.27  2006/06/13 08:17:48  hqfeng
**  - Replace "printf" by "tc_printf" .
**
** Revision 1.26  2006/06/09 08:52:52  hqfeng
** - Modify getCwmpRebootFlag() and setCwmpRebootFlag().
**
** Revision 1.25  2006/06/09 06:37:18  hqfeng
** - Modify  for Reboot RPC.
**
** Revision 1.24  2006/06/09 06:16:23  hqfeng
** - Modify for Reboot RPC.
**
** Revision 1.23  2006/06/08 13:07:51  xzwang
** - remove some debug messages.
** - add download RPC function.
** - close connection while receive one empty packet.
** - realize basic authentication for download from file server.
** - realize two active connections.
**
** Revision 1.22  2006/06/06 09:36:06  hqfeng
** - Modify cwmpReleaseFaultStruct() for release .
**
** Revision 1.21  2006/06/06 02:20:41  hqfeng
** - Modify for perfect the Fault Code machine.
**
** Revision 1.20  2006/06/05 04:34:26  hqfeng
** - Modify cwmpBuildResponseTree() for fault code.
**
** Revision 1.19  2006/06/03 07:40:21  hqfeng
**  - Add Fault Code Machine.
**
** Revision 1.18  2006/05/24 06:02:41  hqfeng
** - Add getCwmpRebootFlag().
**
** Revision 1.17  2006/04/29 02:03:06  hqfeng
**  - Add AddObject, DeleteObject and  Reboot.
**
** Revision 1.16  2006/04/28 06:52:55  xzwang
** do not build and send head node if the request soap has not head node.
**
** Revision 1.15  2006/04/25 10:08:21  xzwang
** - modify stack multipulate function.
** - add judement for parameter interface function.
**
** Revision 1.14  2006/04/21 08:54:51  hqfeng
**  - Add spSave() in cwmpSetParameterValuesResponse() and
**   cwmpSetParameterAttributeResponse().
**
** Revision 1.13  2006/04/17 06:04:31  hqfeng
** *** empty log message ***
**
** 
*/


#include "cwmpDef.h"  
#include "Global_res.h"
#include <sys/time.h>
#include "cwmpRpc.h"
#include "cp_topnodetable.h"
#include "libcompileoption.h"
/************************************************************************
*						D E F I N I T I O N
**************************************************************************/

//#define DefaultNodeName  (getWANExternalName())
//#define malloc() os_malloc()
//#define free() os_free()


/************************************************************************
*						E X T E R N A L   D A T A
**************************************************************************/
int needReboot = 0;
int cwmpGetACSFlag = 0;
extern spSysParam_t	spSysParam;
extern cwmp_rpc_t rpc;
uint8 downloadInProgress = 0;
int PrivCwmpGetDataFlag = 0;
SoapInfoPtr PrivResponseStructPtr = NULL;
int eWcHttpTemporaryRedirectFlag = 0;
#if CWMP_REDUCE_CODE
extern char  fEventMessageBuffer[CwmpWebClientBufferSize];
#endif
extern char  fResponeBuffer[CwmpWebClientBufferSize];
extern int AlarmNumberInfo[11];
extern int itms_reboot_flag;
extern void* cwmpGetRequestPtr(void* theTaskDataPtr);
extern void* cwmpGetTaskDataPtr();
extern wcStatus cwmpQueueRequest(void *theTaskDataPtr, void *theWebRequestPtr);
extern wcStatus cwmpReleaseRequestPtr(void *theTaskDataPtr, void *theWebRequestPtr);
extern char* fetchConRequestURL_detail(void);
/************************************************************************
*						P R I V A T E   D A T A
**************************************************************************/
static char chartime[32]="0001:01:01T00:00:00";
static char gParameterKey[32]="\0";//changed by zhj 0725

static FaultDetailPtr faultDetailPtr = NULL;
static RPCFaultPtr rpcFaultListHead = NULL;
static RPCFaultPtr rpcFaultListTail = NULL;
static FaultCode_String_Map FaultCode_String_MapTable[] =
{
	{MethodNotSupport, "Method not supported"},
	{RequestDeny, "Request denied"},
	{InternalError, "Internal error"},
	{InvalidArgument, "Invalid arguments"},
	{ResourceExceed, "Resource exceeded"},
	{InvalidParameterName, "Invalid parameter name"},
	{InvalidParameterType, "Invalid parameter type" },
	{InvalidParameterValue, "Invalid parameter value"},
	{InvalidRequestUsername, "Invalid RequestUsername "},
	{SetReadOnlyParameter, "Attemp to set a non-writable parameter"},
	{RejectRequestNotification, "Notification request rejected"},
	{DownloadFailure, "Download failure"},
	{UploadFailure, "Upload failure" },
	{FileTransferAuthenFailure, "File transfer server authentication Failure"},
	{UnsupportedProtocol, "Unsupported protocol for file transfer"},
	{IllegalFirmware, "Firmware File is illegal for CPE!"},
	{NoGetValueFunction, "No get value function"},
	{WriteSimCardFail, "Write Sim Card Fail"},
	{SimCardAuthenticationFail, "Sim Card Authentication Fail"},
	{NoGetAttributeFunction, "No get attribute function"},
	{0, NULL}
		
};

static cwmp_rpc_methodmap rpc_MethodMapTable[] = {
	{"InformResponse", cwmpRpcInformResponseResponse},
	{"Reboot", cwmpRebootResponse},
	{"GetRPCMethods", cwmpGetRPCMethodsResponse},
	{"SetParameterValues", cwmpSetParameterValuesResponse},
	{"GetParameterValues", cwmpGetParameterValuesResponse},
	{"GetParameterNames", cwmpGetParameterNamesResponse},
	{"SetParameterAttributes", cwmpSetParameterAttributesResponse},
	{"GetParameterAttributes", cmwpGetParameterAttributesResponse},
	{"AddObject", cwmpAddObjectResponse},
	{"DeleteObject", cwmpDeleteObjectResponse},
	{"Download", cwmpDownloadResponse},
	{"TransferCompleteResponse", cwmpTransferCompleteResponseResponse},
	{"Fault", cwmpFaultCodeResponse},
	{"GetRPCMethodsResponse", cwmpProcessGetRPCMethodsResponse},
	{"FactoryReset", cwmpFactoryResetResponse},
	{"Upload", cwmpUploadResponse},
 	{"ScheduleInform", cwmpScheduleInformResponse},
        {NULL, NULL}
};

static char *MWforcedNotifyNode[] = {	
	//"Midware.CTMgtIPAddress",
	//"Midware.MgtDNS",
	//"Midware.MWSURL", 
	//"Midware.ITMSURL", 
	//"Midware.InternetPvc",
	//"Midware.CTEvent",
	//"Midware.PushKeyEvent",
	NULL
};

extern char *forcedNotifyNode[];

char *downloadErrString[] = {
        NULL,
        "Firmware File is illegal for CPE!",
        "Failed to open the connection to the server!",
        "File Transfer Authen Failure!",
	"Download Failure",
};

char *uploadErrString[] = {
        NULL,
	"Upload file to server fails!",
};

//static DeviceNodeStackPtr deviceNodeStackHead = NULL;
DeviceNodeStackPtr deviceNodeStackHead = NULL; //hqfeng,2006.12.15
#if CWMP_REDUCE_CODE
static DeviceNodeStackPtr deviceNodeStackTail = NULL;
#endif

static uint8 nextLevelFalg = 0;
static int cwmpHeadID = 16001;
//uint8 Head_Flag = 0;//added for the packet has the head tag or not. by hqfeng, 2006.03.17
static volatile uint8 cwmpRebootFlag = 0;
static int gACSFaultCode = 0;

static NotifyDeviceNodeInfoPtr activeNotifyNodeListHeadP = NULL;
static NotifyDeviceNodeInfoPtr activeNotifyNodeListTailP = NULL;
static NotifyDeviceNodeInfoPtr passiveNotifyNodeListHeadP = NULL;
static NotifyDeviceNodeInfoPtr passiveNotifyNodeListTailP = NULL;

static DeviceNodeStackPtr valueChangeStackHeadP = NULL;
#if CWMP_REDUCE_CODE
static DeviceNodeStackPtr valueChangeStackTailP = NULL;
#endif

/* midware node for MW */
static NotifyDeviceNodeInfoPtr MWActiveNotifyNodeListHeadP = NULL;
static NotifyDeviceNodeInfoPtr MWActiveNotifyNodeListTailP = NULL;
static DeviceNodeStackPtr MWvalueChangeStackHeadP = NULL;
/* tr069 node for MW*/
static NotifyDeviceNodeInfoPtr TRActiveNotifyNodeListHeadP = NULL;
static NotifyDeviceNodeInfoPtr TRActiveNotifyNodeListTailP = NULL;
static DeviceNodeStackPtr TRvalueChangeStackHeadP = NULL;
/* tr069 node for ITMS*/
static NotifyDeviceNodeInfoPtr ITMSActiveNotifyNodeListHeadP = NULL;
static NotifyDeviceNodeInfoPtr ITMSActiveNotifyNodeListTailP = NULL;
static DeviceNodeStackPtr ITMSvalueChangeStackHeadP = NULL;

static uint8 emptyPaket = 0;
static uint8 notifyFlag = 1;
static uint8 spSaveFlag = 1;
static uint8 resetAttributeAfeterAddObject = 0;
static boolean gACSGetRPCMethods = 1; //for getting the ACS's RPC Methods, by hqfeng, 2006.08.15
static int upgradeDelayTimerID = 0;

static int root_name_flag = 0;

/************************************************************************
*						GLOBAL FUNCTION
**************************************************************************/
extern uint8 cwmpSetRpcResponseFlag(cwmp_rpc_t *rpc);
extern uint8 cwmpGetRpcResponseFlag(cwmp_rpc_t *rpc);
extern uint8 cwmpResetRpcResponseFlag(cwmp_rpc_t *rpc);
//extern void	cwmpCommandParse(char * theBufferPtr, cwmp_rpc_t *rpc);
extern void	cwmpParseSoapPacket(void);
extern ip4a resolveName( char *name );
extern char*	inet_ntoa( ip4a );
extern boolean  getCorrectACSPktLengthEnable(void);
extern void setCorrectACSPktLengthEnable(boolean val);
extern mbuf_t *ixmlPrintNode_to_mbuf( IN IXML_Node * node );

//added by xyzhu_nj20091204
extern attr_ptr pTcAttrs;
extern void ReadAttr_OP(attr_ptr tcAttr_ptr, int *saveFlag);
extern void SaveAttr(attr_ptr tcAttr_ptr);
extern int cwmpFastGet;

extern cfgNode_ptr pTcCfgNodes;
/************************************************************************
*						P R I V A T E   FUNCTION
**************************************************************************/
#if CWMP_REDUCE_CODE
static int  cwmpRpcNodeDump(IXML_Node *);
#endif

int cwmpBuildFaultStruct(int* errorCode);
void	cwmpFreeSoapMemory(cwmp_rpc_t *rpc);
static int8 cwmpReleaseFaultDetailStruct(FaultDetailPtr faultDetailtPtr);
uint8 getCwmpRebootFlag(uint8* rebootFlag);
uint8 setCwmpRebootFlag(uint8 flag);
int  getACSFaultCode(void);
void  setACSFaultCode(int value);
char * TimeFormatConvert(uint32 time);
int  cwmpCheckDeviceNode(DeviceNodePtr deviceNode, ParameterValueStructPtr reqPara);
static void  freeRPCFaultStruct( RPCFault **rpcFaultPtr);
//Added by hqfeng,2006.12.30
int8 cwmpFreeSingleParameterValueStruct(ParameterValueStructPtr valueStruct);
int8 cwmpFreeSingleParameterInfoStruct(ParameterInfoStructPtr infoSruct);
int8 cwmpFreeSingleParameterAttributeStruct(ParameterAttributeStructPtr attributeStruct);
int8 cwmpFreeSingleParameterAttributesSetStruct(ParameterAttributesSetStructPtr attributeSetStruct);

static void ClearFlagsForDelayedProcess(void);

/************************************************************************
*						FUNCTION DEFINITION
**************************************************************************/

int cwmpRpcInit(cwmp_rpc_t* rpc)
{
	memset(rpc,0,sizeof(cwmp_rpc_t));

	return 0;
}


/************************** Inform mechanism *******************************/
int8 cwmpRpcInform(cwmp_rpc_t* rpc){
	int rc = -1;

	if(rpc == NULL){
		tc_printf("The rpc is NULL!\r\n");
		return -1;
	}

	if(rpc->txBuf){
#if CWMP_USING_MBUF_FOR_TXBUF
		cwmpResetMbufList();
#else
		if(!getPreAllocTxBufFlag())
		    os_free(rpc->txBuf);  //do not free it for it is pre-allocated. xzwang_060831
#endif
		rpc->txBuf = NULL;
	}

#if 0 //This part will be moved to cwmpBuildInformPkt(),hqfeng.2006.11.23
	//Add for dealing with HoldRequests header, by hqfeng, 2006.08.15
	if(getHoldRequestFlag()){
		cwmpFreeSoapMemory(rpc);
		rpc->informStruct.eventList = NULL;
		rpc->informStruct.parameterList = NULL;

		return 0;		
	}
#endif
	
//Build response structure which includes information for building SOAP packet.
	if(rpc->responseStructPtr){
		cwmpReleaseSoapStruct(rpc->responseStructPtr);//hqfeng,2006.08.15
		rpc->responseStructPtr = NULL;
	}

	
	if((rpc->responseStructPtr = (SoapInfoPtr)os_malloc(sizeof(SoapInfo))) == NULL){
		tc_printf("malloc response info struct error!\r\n");
		goto InformError;
	}
	memset(rpc->responseStructPtr, 0, sizeof(SoapInfo));

	if(cwmpBuildInformStruct(rpc->responseStructPtr) == -1){
		tc_printf("Building Inform struct fails!\r\n");
		goto InformError;
	}
	
//Build SOAP tree by using the information in response structure.
	rc = ixmlDocument_createDocumentEx( &rpc->responseDocPtr);
	if( rc != IXML_SUCCESS ) {
		tc_printf("creat respons doc fails!\r\n");
		goto InformError;
	}
	if(TCSUPPORT_PON_ROSTELECOM_VAL && CWMP_REDIRECT_VAL){
		if(PrivCwmpGetDataFlag ==1 && eWcHttpTemporaryRedirectFlag == 1 && PrivResponseStructPtr != NULL){
			if(cwmpBuildResponseTree(PrivResponseStructPtr, rpc->responseDocPtr) != 0){
				tc_printf("Building response Tree fails!\r\n");
				goto InformError;
			}
			cwmpReleaseSoapStruct(PrivResponseStructPtr);
			PrivResponseStructPtr = NULL;
		}
		else{	
			if(cwmpBuildResponseTree(rpc->responseStructPtr, rpc->responseDocPtr) != 0){
				tc_printf("Building response Tree fails!\r\n");
				goto InformError;
			}
			eWcHttpTemporaryRedirectFlag = 0;	
			PrivCwmpGetDataFlag = 0;
			if(PrivResponseStructPtr != NULL){
				cwmpReleaseSoapStruct(PrivResponseStructPtr);
				PrivResponseStructPtr = NULL;
			}
		}
	}
	else{
	if(cwmpBuildResponseTree(rpc->responseStructPtr, rpc->responseDocPtr) != 0){
		tc_printf("Building response Tree fails!\r\n");
		goto InformError;
	}
	}
	
	cwmpReleaseSoapStruct(rpc->responseStructPtr);
	rpc->responseStructPtr = NULL;

//Build SOAP buffer with SOAP tree .
	if(cwmpFrameTreeToBuf(rpc->responseDocPtr, &rpc->txBuf) != 0)
	{
		tc_printf("\r\nbuild txbuf fails!\r\n");
		goto InformError;
	}

	cwmpReleaseSoapDoc_new(rpc->responseDocPtr);
	rpc->responseDocPtr = NULL;
	
	rpc->informStruct.eventList = NULL;
	rpc->informStruct.parameterList = NULL;

	cwmpSetInformCompleteFlag(0);
	
	return 0;
	
InformError:
	cwmpReleaseSoapStruct(rpc->responseStructPtr);
	rpc->responseStructPtr = NULL;

	cwmpReleaseSoapDoc_new(rpc->responseDocPtr);
	rpc->responseDocPtr = NULL;

	rpc->informStruct.eventList = NULL;
	rpc->informStruct.parameterList = NULL;


	return -1;
	
}

int8 cwmpBuildInformStruct(SoapInfoPtr InformStructPtr){
	SoapHeadInfoPtr informHeadStructPtr;
	SoapBodyInfoPtr informBodyStructPtr;
	

	if(InformStructPtr == NULL){
		tc_printf("responseStructPtr is NULL!\r\n");
		return -1;
	}
	
	if((InformStructPtr->headStructPtr = (SoapHeadInfoPtr)os_malloc(sizeof(SoapHeadInfo))) == NULL){
		tc_printf("malloc inform head struct error!\r\n");
		return -1;
	}
	memset(InformStructPtr->headStructPtr, 0, sizeof(SoapHeadInfo));

	if((InformStructPtr->bodyStructPtr = (SoapBodyInfoPtr)os_malloc(sizeof(SoapBodyInfo))) == NULL){
		tc_printf("malloc inform body struct error!\r\n");
		return -1;
	}
	memset(InformStructPtr->bodyStructPtr , 0, sizeof(SoapBodyInfo));
	
	informHeadStructPtr = InformStructPtr->headStructPtr;
	informBodyStructPtr = InformStructPtr->bodyStructPtr;
	
	if(cwmpBuildInformHeadStruct(informHeadStructPtr) == -1){
		tc_printf("build response head struct fails!\r\n");
		return -1;
	}

	if(cwmpBuildInformBodyStruct(informBodyStructPtr) == -1){
		tc_printf("build response body struct fails!\r\n");
		return -1;
	}

	return 0;
}

int8 cwmpBuildInformHeadStruct(SoapHeadInfoPtr  informHeadStructPtr){
	char id[16];

	if(informHeadStructPtr == NULL){
		tc_printf("responseHeadStructPtr is NULL!\r\n");
		return -1;
	}

    	cwmpGetInformHeadId(id);
	strcpy(informHeadStructPtr->id, id);
	strcpy(informHeadStructPtr->holdRequest, "");
	cwmpSetNoMoreRequest(informHeadStructPtr->noMoreRequest);

	return 0;
}

int8 cwmpGetInformHeadId(char* headId){
	if(headId == NULL){
		tc_printf("inform headID is NULL!\r\n");
		return -1;
	}

	if(cwmpHeadID == 65535)
		cwmpHeadID = 16001;
	
	if(!CWMP_REDIRECT_VAL){
		cwmpHeadID++;  //here need further handle.
		cwmpItoa(cwmpHeadID, headId, 10);
	}
	else{
		if(GetCwmpRedirectFlag()){
			cwmpItoa(cwmpHeadID, headId, 10);
		}
		else{
		cwmpHeadID++;  //here need further handle.
		cwmpItoa(cwmpHeadID, headId, 10);
		}
	}

	return 0;
}

int8 cwmpBuildInformBodyStruct(SoapBodyInfoPtr informBodyStructPtr){
	RPCArgumentPtr curArgPtr = NULL, tempArgPtr = NULL;
	DeviceIdStructPtr deviceIdPtr = NULL;
	EventStructPtr     eventPtr;
	ParameterValueStructPtr paraValuePtr = NULL;

	if(informBodyStructPtr == NULL){
		tc_printf("inform body struct is NULL!\r\n");
		return -1;
	}
      // rpc name
	strcpy(informBodyStructPtr->rpcName, "Inform");

     //Argument.
       // DeviceID
	if(cwmpInitArgument("DeviceId", 1, DeviceIdStructType, 0, &tempArgPtr) == -1){
		tc_printf("Initiate inform argument DeviceId fails!\r\n");
		return -1;
	}
	cwmpGetDeviceID(&deviceIdPtr);
	tempArgPtr->value = deviceIdPtr;
	curArgPtr = tempArgPtr;
	informBodyStructPtr->argumentPtr = tempArgPtr;

	//Event
	if(cwmpInitArgument("Event", 1, EventStructType, 1, &tempArgPtr) == -1){
		tc_printf("Initiate inform argument fails!\r\n");
		return -1;
	}
	cwmpGetInformEventParaList(&eventPtr, &tempArgPtr->number);
	tempArgPtr->parameterList = eventPtr;
	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;

	//MaxEnvelopes
	if(cwmpInitArgument("MaxEnvelopes", 1, UnsignedIntType, 0,  &tempArgPtr) == -1){
		tc_printf("Initiate inform argument fails!\r\n");
		return -1;
	}
	cwmpGetMaxEnvelopes(&tempArgPtr->value);
	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;

	//CurrentTime
	if(cwmpInitArgument("CurrentTime", 1, DateTimeType, 0,  &tempArgPtr) == -1){
		tc_printf("Initiate inform argument fails!\r\n");
		return -1;
	}
	cwmpGetCurrentTime(&tempArgPtr->value);
	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;

	//RetryCount
	if(cwmpInitArgument("RetryCount", 1, UnsignedIntType, 0, &tempArgPtr) == -1){
		tc_printf("Initiate inform argument fails!\r\n");
		return -1;
	}
	cwmpGetRetryCount(&tempArgPtr->value);
	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;

	//ParameterList
	if(cwmpInitArgument("ParameterList", 1, ParameterValueStructType, 1, &tempArgPtr) == -1){
		tc_printf("Initiate inform argument fails!\r\n");
		return -1;
	}
	cwmpGetInformParameterList(&paraValuePtr, &tempArgPtr->number);
	tempArgPtr->parameterList = paraValuePtr;
	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;
	
	return 0;
	
}

int cwmpBuildGetRPCMethodsBodyStruct(SoapBodyInfoPtr BodyStructPtr)
{
	if(BodyStructPtr == NULL){
		tc_printf("TransferComplete  body struct is NULL!\r\n");
		return -1;
	}
      // rpc name
	strcpy(BodyStructPtr->rpcName, "GetRPCMethods");

	return 0;

}

int8 cwmpInitArgument(char* name, int number,  ParameterType type,  uint8 beArray, RPCArgumentPtr* rpcArgPtrP){
	RPCArgumentPtr  tempArgPtr = NULL;

	if((tempArgPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc inform argument fails!\r\n");
		return -1;
	}
	memset(tempArgPtr, 0, sizeof(RPCArgument));

	if((tempArgPtr->name = (char*)os_malloc(strlen(name)+1)) != NULL)   //xzwang_060624
		strcpy(tempArgPtr->name, name);
	else{
		tc_printf("malloc inform argument's name fails!\r\n");
		os_free(tempArgPtr);
		tempArgPtr = NULL;
		return -1;
	}
	tempArgPtr->number = number;
	tempArgPtr->type = type;
	tempArgPtr->beArray = beArray;

	*rpcArgPtrP = tempArgPtr;

	return 0;
}

int8 cwmpGetDeviceID(DeviceIdStructPtr *idPtrP){
	DeviceIdStructPtr idPtr = NULL;
	DeviceIdStruct id = rpc.informStruct.deviceId;
	
	if((idPtr = (DeviceIdStructPtr) os_malloc(sizeof(DeviceIdStruct))) == NULL){
		tc_printf("malloc device id struct fails!\r\n");
		return -1;
	}
	
	strcpy(idPtr->manufacture, id.manufacture);
	strcpy(idPtr->OUI, id.OUI);
	strcpy(idPtr->productClass, id.productClass);
	strcpy(idPtr->serialNumber, id.serialNumber);

	*idPtrP = idPtr;

	return 0;
}

int8 cwmpSetDeviceID(char* manufacture, char* oui, char* productClass, char* serialNumber){
	DeviceIdStructPtr idPtr = &(rpc.informStruct.deviceId);

	strncpy(idPtr->manufacture, manufacture, MANUFACTURELEN);
	strncpy(idPtr->OUI, oui, MANUFACUREROUILEN);
	strncpy(idPtr->productClass, productClass, PRDDUCTCLASSLEN);
	strncpy(idPtr->serialNumber, serialNumber, SERIALNUMLEN);

	return 0;
}

int8 cwmpGetInformEventParaList(EventStructPtr *eventPtrP, int* numP){
	EventStructPtr rootEventPtr = NULL, tempEventPtr = NULL;
	EventStructPtr curEvent = rpc.informStruct.eventList;
	int count = 0;
	
	while(curEvent){
		curEvent = curEvent->next;
		count++;
	}

	*eventPtrP = rpc.informStruct.eventList;
	*numP = count;
	
	return 0;
}

int8 cwmpAddInformEvent(char* eventCode, char* commandKey){
	EventStructPtr rootEventPtr = NULL, tempEventPtr = NULL;
	EventStructPtr curEvent = rpc.informStruct.eventList;

	if(strlen(eventCode) > 64){
		tc_printf("the length of setting event code should be smaller than 64!\r\n");
		return -1;
	}
	if(strlen(commandKey) > 32){
		tc_printf("the length of inform event commandKey should be smaller than 32!\r\n");
		return -1;
	}
	if((tempEventPtr = (EventStructPtr) os_malloc(sizeof(EventStruct))) == NULL){
		tc_printf("malloc device id struct fails!\r\n");
		return -1;
	}
	memset(tempEventPtr, 0, sizeof(EventStruct));
	
	strcpy(tempEventPtr->eventCode, eventCode);
	strcpy(tempEventPtr->commandKey, commandKey);

	if(curEvent == NULL){
		rpc.informStruct.eventList = tempEventPtr;
	}
	else{
		while(curEvent->next != NULL)
			curEvent = curEvent->next;

		curEvent->next = tempEventPtr;
	}

	return 0;
}

#if CWMP_REDUCE_CODE
int8 cwmpClearInformEvent(void){
	EventStructPtr curEvent = rpc.informStruct.eventList;
	EventStructPtr  tempEventPtr = NULL;
	
	while(curEvent){
		tempEventPtr = curEvent->next;
		os_free(curEvent);

		curEvent = tempEventPtr;
	}

	rpc.informStruct.eventList = NULL;
	
	return 0;
}
#endif

int8 cwmpGetMaxEnvelopes(void** valuePtrP){
	unsigned int *tempValue = NULL;

	if((tempValue = (unsigned int*)os_malloc(sizeof(unsigned int))) == NULL){
		tc_printf("malloc maxEnvelopes value fails!\r\n");
		return -1;
	}

	*tempValue = rpc.informStruct.maxEnvelopes;
	*valuePtrP = tempValue;

	return 0;
}

int8 cwmpSetMaxEnvelopes(unsigned int maxEnvelope){
	//if(RPC.informStruct.maxEnvelopes > maxEnvelope)
		rpc.informStruct.maxEnvelopes = maxEnvelope;
	
	return 0;
}

int8	cwmpGetCurrentTime(void** valuePtrP){
	char* tempTime;

	if((tempTime = (char*)os_malloc(64)) == NULL){
		tc_printf("malloc maxEnvelopes value fails!\r\n");
		return -1;
	}
	memset(tempTime, 0, 64);
	
	strcpy(tempTime, rpc.informStruct.currentTime);
	*valuePtrP = tempTime;
		
	return 0;
}

int8	cwmpSetCurrentTime(char* curTime){

	if(strlen(curTime) > 63){
		tc_printf("the length of current time should be smaller than 64!\r\n");
		return -1;
	}

	strcpy(rpc.informStruct.currentTime, curTime);
	return 0;
}

int8	cwmpGetRetryCount(void** valuePtrP){
	unsigned int *tempValue = NULL;

	if((tempValue = (unsigned int*)os_malloc(sizeof(unsigned int))) == NULL){
		tc_printf("malloc retry count value fails!\r\n");
		return -1;
	}

	*tempValue = rpc.informStruct.retryCount;
	*valuePtrP = tempValue;

	return 0;
}

int8	cwmpSetRetryCount(unsigned int  retryCount){
	rpc.informStruct.retryCount = retryCount;

	return 0;
}

int8	cwmpGetInformParameterList(ParameterValueStructPtr* paraValuePtrP,  int* numP){
	ParameterValueStructPtr curPara = rpc.informStruct.parameterList;
	int count = 0;

	while(curPara){
		curPara = curPara->next;
		count++;
	}
	
	*paraValuePtrP = rpc.informStruct.parameterList;
	*numP = count;
	
	return 0;
}

int8	cwmpAddInformParameter(char* name, char* value, ParameterType type){
	ParameterValueStructPtr rootParaPtr = NULL, tempParaPtr = NULL;
	ValueInfoPtr tempValueInfo;
	char* tempValue;
	ParameterValueStructPtr curPara = rpc.informStruct.parameterList;

	if (value == NULL)
	{
		tc_printf("\r\n value should not be NULL!\r\n");
		return -1;
	}

	if(strlen(name) > 255){
		tc_printf("the length of parameter name should be less than 255!\r\n");
		return -1;
	}
	
	if((tempParaPtr = (ParameterValueStructPtr ) os_malloc(sizeof(ParameterValueStruct))) == NULL){
		tc_printf("malloc inform ParameterValue struct fails!\r\n");
		return -1;
	}
	memset(tempParaPtr, 0, sizeof(ParameterValueStruct));
	if((tempParaPtr->name = (char*)os_malloc(strlen(name)+1)) != NULL) //xzwang_060623
	strcpy(tempParaPtr->name, name);
	else
        {
		tc_printf("malloc inform ParameterValue name  fails!\r\n");
		goto Error;
	}

	if((tempValueInfo= (ValueInfoPtr)os_malloc(sizeof(ValueInfo))) == NULL)
        {
		tc_printf("malloc inform Parameter value info struct fails!\r\n");
		goto Error;
	}
	memset(tempValueInfo, 0, sizeof(ValueInfo));
	tempParaPtr->valueInfoPtr = tempValueInfo;
	tempValueInfo->type = type;

	if((tempValue= (char*)os_malloc(strlen(value)+1)) == NULL)
        {
		tc_printf("malloc inform Parameter value string fails!\r\n");
		goto Error;
	}
	memset(tempValue, 0, strlen(value)+1);
	strcpy(tempValue, value);
	tempValueInfo->value = tempValue;
	

	if(curPara == NULL){
		rpc.informStruct.parameterList = tempParaPtr;
	}
	else{
		while(curPara->next != NULL)
			curPara = curPara->next;

		curPara->next = tempParaPtr;
	}

	return 0;
	
Error:
	/*
		free memory already allocated before return.
	*/
	if (tempParaPtr !=  NULL)
	{
		if (tempParaPtr->name != NULL)
		{
			os_free(tempParaPtr->name);
			tempParaPtr->name = NULL;
		}

		if(tempParaPtr->valueInfoPtr != NULL)
		{
			if (tempParaPtr->valueInfoPtr ->value != NULL)
			{
				os_free(tempParaPtr->valueInfoPtr->value);
				tempParaPtr->valueInfoPtr->value = NULL;		
			}
			
			tempParaPtr->valueInfoPtr->type = 0;

			os_free(tempParaPtr->valueInfoPtr);
			tempParaPtr->valueInfoPtr =NULL;
		}
				
		os_free(tempParaPtr);
		tempParaPtr = NULL;
		tempValue = NULL;
		tempValueInfo = NULL;
	}		

	return -1;
}

boolean cwmpHaveNoHeadandBody(IXML_Document* requestDocPtr)
{
	char *names1[5];
	char *names2[5];
	IXML_Node* rootNode = NULL;
	IXML_Node* requestHeadNode = NULL;
	IXML_Node* requestBodyNode = NULL;

	rootNode = ixmlNode_getFirstChild( ( IXML_Node * ) requestDocPtr);
	if( rootNode == NULL ) {
		if(dbg_cwmp >= DBG_CWMP_L1 )
			tc_printf("The roote node of tree is NULL!\r\n");
		return TRUE;
	}

	names1[0] = "Envelope";
	names1[1] = "Header";

	names2[0] = "Envelope";
	names2[1] = "Body";
	if( (cwmpFindDeepNode( names1, 2, rootNode, &requestHeadNode ) !=CWMP_E_SUCCESS ) &&
	     (cwmpFindDeepNode( names2, 2, rootNode, &requestBodyNode ) != CWMP_E_SUCCESS ) ){
	     return TRUE;
	}
	return FALSE;

	
}

/********************************* Dispatch RPC request ******************************************/
int cwmpRpcDispatch(cwmp_rpc_t *rpc)
{
	int rc = -1;
	int err_state = CWMP_E_SUCCESS;
	int acsFaultCode = 0;

	if(rpc == NULL){
		tc_printf("The rpc is NULL!\r\n");
		return -1;
	}
	
#if 1   //dynamic rxBuf.xzwang_060707
	if(rpc->rxBuf){
		os_free(rpc->rxBuf);
		rpc->rxBuf = NULL;
	}
#else
	memset(rpc->rxBuf, 0, CWMP_RX_BUF_SIZE);
#endif	
	//xzwang_060712
	if(rpc->requestDocPtr == NULL){
		tc_printf("[cwmpRpcDispatch] request tree is NULL!\r\n");
		goto EndDispatch;
	}
	
	if(cwmpHaveNoHeadandBody(rpc->requestDocPtr)  == TRUE){
		cwmpReleaseSoapDoc(rpc->requestDocPtr);  
         	rpc->requestDocPtr = NULL;		
		goto EndDispatch;
	}
	
	if((rpc->requestStructPtr = (SoapInfoPtr)os_malloc(sizeof(SoapInfo))) == NULL){
		 tc_printf("malloc request info struct error!\r\n");
	        cwmpReleaseSoapDoc(rpc->requestDocPtr);  
         	rpc->requestDocPtr = NULL;
		return -1;//Don't response fault code.
	}
	memset(rpc->requestStructPtr, 0, sizeof(SoapInfo));

	if((rpc->responseStructPtr = (SoapInfoPtr)os_malloc(sizeof(SoapInfo))) == NULL){
		tc_printf("malloc response info struct error!\r\n");
		os_free(rpc->requestStructPtr); 
              rpc->requestStructPtr = NULL;
              cwmpReleaseSoapDoc(rpc->requestDocPtr);  
	       rpc->requestDocPtr = NULL;
		return -1;//Don't response fault code.
	}
	memset(rpc->responseStructPtr, 0, sizeof(SoapInfo));
	
        cwmpSetRpcResponseFlag(rpc);//Added for sending the Fault Code,by hqfeng,2006.02.21

	//Extract useful information from SOAP tree and put them into specific structures. 
	err_state = cwmpExtractTreeInfoToStruct(rpc->requestDocPtr, rpc->requestStructPtr);
	if(err_state != CWMP_E_SUCCESS){
		tc_printf("Extract tree info into struct fails!\r\n");
		goto Error_Handler;
	}

       cwmpReleaseSoapDoc(rpc->requestDocPtr);  
	rpc->requestDocPtr = NULL;
	
	//Build response structure which includes information for building SOAP packet.
	cwmpSetNotifyFlag(0);  //Close notify function. xzwang_20061012
	err_state = cwmpBuildResponseStruct(rpc->requestStructPtr, rpc->responseStructPtr);
	if(TCSUPPORT_PON_ROSTELECOM_VAL && CWMP_REDIRECT_VAL){
		if(PrivCwmpGetDataFlag == 1){
			if(PrivResponseStructPtr == NULL){
				if((PrivResponseStructPtr = (SoapInfoPtr)os_malloc(sizeof(SoapInfo))) != NULL){
					cwmpBuildResponseStruct(rpc->requestStructPtr, PrivResponseStructPtr);
					eWcHttpTemporaryRedirectFlag = 0;
				}
			}
		}
		else{
			if(PrivResponseStructPtr != NULL){
				cwmpReleaseSoapStruct(PrivResponseStructPtr);
				PrivResponseStructPtr = NULL;
			}
			eWcHttpTemporaryRedirectFlag = 0;
		}
	}
	cwmpSetNotifyFlag(1);  //Open notify function. xzwang_20061012
	if( err_state != CWMP_E_SUCCESS){
		tc_printf("Building response information struct fails!\r\n");
		gParameterKey[0] = '\0';
		goto Error_Handler;
	}

        cwmpReleaseSoapStruct(rpc->requestStructPtr);
	rpc->requestStructPtr = NULL;

	//Added for Retry Policy, by hqfeng,2006.08.01
	acsFaultCode = getACSFaultCode();
	err_state = dealACSFaultCode(acsFaultCode, &rpc);
	if(err_state != 0){
		return 0;
	}

 	//Build SOAP tree by using the information in response structure.
	err_state = ixmlDocument_createDocumentEx( &rpc->responseDocPtr);
	if( err_state != CWMP_E_SUCCESS ) {
		tc_printf("create respons doc fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}
	err_state = cwmpBuildResponseTree(rpc->responseStructPtr, rpc->responseDocPtr);
	if( err_state != CWMP_E_SUCCESS){
		tc_printf("Building response Tree fails!\r\n");
		goto Error_Handler;
	}
        cwmpReleaseSoapStruct(rpc->responseStructPtr);
	rpc->responseStructPtr = NULL;

 	//Build SOAP buffer(txBuf) with SOAP tree .
	err_state = cwmpFrameTreeToBuf(rpc->responseDocPtr, &rpc->txBuf);
	if( err_state != CWMP_E_SUCCESS){
		tc_printf("\r\nbuild txbuf fails!\r\n");
		goto Error_Handler;
	}

        cwmpReleaseSoapDoc_new(rpc->responseDocPtr);
	rpc->responseDocPtr = NULL;

      //for sending empty packet. xzwang_060621
	if(emptyPaket && rpc->txBuf){    
	#if CWMP_USING_MBUF_FOR_TXBUF
		cwmpResetMbufList();
	#else	
		if(!getPreAllocTxBufFlag())
		    os_free(rpc->txBuf);  //do not free it for it is pre-allocated. xzwang_060831
	#endif
		rpc->txBuf = NULL;
		emptyPaket = 0;
	}

	if(gParameterKey[0]  != '\0'){
		cwmpSetNotifyFlag(0);  //Close notify function. xzwang_20061012
		setParameterKey(gParameterKey);
		cwmpSetNotifyFlag(1);  //Open notify function. xzwang_20061012
		gParameterKey[0] = '\0';
	}

 	return CWMP_E_SUCCESS;

Error_Handler:
	
	if(rpc->txBuf){
#if CWMP_USING_MBUF_FOR_TXBUF
		cwmpResetMbufList();
#else		
		if(!getPreAllocTxBufFlag())
			os_free(rpc->txBuf);  //do not free it if it is pre-allocated. xzwang_060831
#endif
		rpc->txBuf = NULL;
	}		
	cwmpReleaseSoapDoc(rpc->requestDocPtr);  
	rpc->requestDocPtr = NULL;
	cwmpReleaseSoapStruct(rpc->requestStructPtr);
	rpc->requestStructPtr = NULL;
	cwmpReleaseSoapBodyStruct(&rpc->responseStructPtr->bodyStructPtr);
 	cwmpReleaseSoapDoc_new(rpc->responseDocPtr);
	rpc->responseDocPtr = NULL;

	if(getCwmpRebootFlag(NULL)){//reset the Reboot flag, if the flag =1.hqfeng,2006.06.09
		cwmpSetNotifyFlag(0);  //Close notify function. xzwang_20061012
		setCwmpRebootFlag(0);
		cwmpSetNotifyFlag(1);  //Open notify function. xzwang_20061012
	}
	
	rc = cwmpBuildFaultStruct(&err_state);

	if(rc == CWMP_E_SUCCESS){
		if(cwmpInitFaultStruct(rpc) != 0 ){
			cwmpFreeSoapMemory(rpc);
			return -1;
		}
	}else{
		cwmpFreeSoapMemory(rpc);				
		return -1;
	}
	
	/*Build FaultCode Response Tree*/
	if(rpc->responseDocPtr == NULL)
		ixmlDocument_createDocumentEx( &rpc->responseDocPtr);
	if(cwmpBuildResponseTree(rpc->responseStructPtr, rpc->responseDocPtr) == -1){
		tc_printf("Building FaultCode response Tree fails!\r\n");
		cwmpFreeSoapMemory(rpc);				
		return -1;
	}
	cwmpFrameTreeToBuf(rpc->responseDocPtr, &rpc->txBuf);
	cwmpFreeSoapMemory(rpc);

EndDispatch:
	
	return err_state;
}


/********************************** Extract information to struct *****************************/

int cwmpExtractTreeInfoToStruct(IXML_Document* requestDocPtr, SoapInfoPtr  requestStructPtr)
{
	char *names[5];
	IXML_Node* rootNode = NULL;
	IXML_Node* requestHeadNode = NULL;
	IXML_Node* requestBodyNode = NULL;
	SoapHeadInfoPtr  requestHeadStructPtr = NULL;
	SoapBodyInfoPtr  requestBodyStructPtr = NULL;
	int err_state = CWMP_E_SUCCESS;

	if(requestDocPtr == NULL){
		tc_printf("request tree is NULL!\r\n");
		return -1;
	}

	if(requestStructPtr == NULL){
		tc_printf("request struct is NULL!\r\n");
		return -1;
	}
		
	rootNode = ixmlNode_getFirstChild( ( IXML_Node * ) requestDocPtr);
	if( rootNode == NULL ) {
		tc_printf("The roote node of tree is NULL!\r\n");
		err_state = InvalidArgument;
		goto Error_Handler;
	}
	
	names[0] = "Envelope";
	names[1] = "Header";
	if( cwmpFindDeepNode( names, 2, rootNode, &requestHeadNode ) !=CWMP_E_SUCCESS ){
		if(dbg_cwmp >= DBG_CWMP_L1 )
			tc_printf("Can not find head node in request packet!\r\n");
		
		//add for dealing with HoldRequests header, by hqfeng, 2006.08.14
		setHoldRequestFlag(0);
	}
	else{
		if((requestStructPtr->headStructPtr = (SoapHeadInfoPtr)os_malloc(sizeof(SoapHeadInfo))) == NULL){
			tc_printf("malloc head struct error!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}
		memset(requestStructPtr->headStructPtr, 0, sizeof(SoapHeadInfo));
	}

	names[0] = "Envelope";
	names[1] = "Body";
	if( cwmpFindDeepNode( names, 2, rootNode, &requestBodyNode ) != CWMP_E_SUCCESS ){
		tc_printf("Can not find body node in request packet!\r\n");
		err_state = InvalidArgument;
		goto Error_Handler;
	}
	else{
		if((requestStructPtr->bodyStructPtr = (SoapBodyInfoPtr)os_malloc(sizeof(SoapBodyInfo))) == NULL){
			tc_printf("malloc body struct error!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}
		memset(requestStructPtr->bodyStructPtr, 0, sizeof(SoapBodyInfo));
	}
	
	requestHeadStructPtr = requestStructPtr->headStructPtr;
	requestBodyStructPtr = requestStructPtr->bodyStructPtr;

	if(requestHeadStructPtr != NULL){
		err_state = cwmpExtractHeadInfoToStruct(requestHeadNode, requestHeadStructPtr);
		if( err_state != CWMP_E_SUCCESS){
			tc_printf("Fails in extrating head info to struct!\r\n");
			goto Error_Handler;
		}
	}

	if(requestBodyStructPtr != NULL){
		err_state = cwmpExtractBodyInfoToStruct(requestBodyNode, requestBodyStructPtr);
		if( err_state != CWMP_E_SUCCESS){
			tc_printf("Fails in extrating body info to struct!\r\n");
			goto Error_Handler;
		}
	}

	return CWMP_E_SUCCESS;
	
Error_Handler:

	return err_state;
	
}


int cwmpExtractHeadInfoToStruct(IXML_Node* requestHeadNode, SoapHeadInfoPtr  requestHeadStructPtr)
{
	IXML_Node* curHeadNode; 
	IXML_Node* tempHeadNode;
	boolean holdRequestFlag = 0;

	if(requestHeadNode == NULL){
		tc_printf("request head node is NULL!\r\n");
		return -1;
	}
	if(requestHeadStructPtr == NULL){
		tc_printf("request head struct is NULL!\r\n");
		return -1;
	}

	curHeadNode = ixmlNode_getFirstChild(requestHeadNode);

	strcpy(requestHeadStructPtr->id, "");
	strcpy(requestHeadStructPtr->holdRequest, "");
	strcpy(requestHeadStructPtr->noMoreRequest, "");

	while(curHeadNode){
		if(strcmp(curHeadNode->localName,"ID") == 0){
			tempHeadNode = ixmlNode_getFirstChild(curHeadNode);
			if(tempHeadNode != NULL){
				if(tempHeadNode->nodeValue != NULL)
			                  strcpy(requestHeadStructPtr->id, tempHeadNode->nodeValue);
	         	}  
		}
		else if(strcmp(curHeadNode->localName,"HoldRequests") == 0){
			tempHeadNode = ixmlNode_getFirstChild(curHeadNode);
			if(tempHeadNode != NULL){
				if(tempHeadNode->nodeValue != NULL)
			                strcpy(requestHeadStructPtr->holdRequest, tempHeadNode->nodeValue);
			//add for dealing with HoldRequests header, by hqfeng, 2006.08.14
			if(isBooleanType(tempHeadNode->nodeValue) == 1)
			{
				holdRequestFlag = 1;
			}else{
				holdRequestFlag = 0;
			}
			}	
		}
		else if(strcmp(curHeadNode->localName,"NoMoreRequests") == 0){
			tempHeadNode = ixmlNode_getFirstChild(curHeadNode);
			if(tempHeadNode != NULL){
				if(tempHeadNode->nodeValue != NULL)
			                  strcpy(requestHeadStructPtr->noMoreRequest, tempHeadNode->nodeValue);
	         	}
		}
		else{
			//Ignor Invalid header.hqfeng,2006.06.02
			tc_printf("Invalid head argumentPtr!\r\n");
//			return -1;
		}
		curHeadNode = curHeadNode->nextSibling;
	}

	//add for dealing with HoldRequests header, by hqfeng, 2006.08.14
	setHoldRequestFlag(holdRequestFlag);
	
	return 0;
}

typedef struct
{
	int prio;
	const char* key_word;

} Reorder_info ,*pReorder_info;

/*
InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.MinAddress','10.0.0.25
InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress','10.0.0.10
InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.MaxAddress','10.0.0.55
InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.DHCPServerEnable','1
InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.SubnetMask','255.255.255.0
*/

const static Reorder_info g_reorder_table[]=
{

	{ 0,"LANHostConfigManagement.DHCPServerEnable"},
	{ 1,"LANHostConfigManagement.IPInterface"},
	{ 2,"LANHostConfigManagement.SubnetMask"},	
	{ 3,"LANHostConfigManagement.MinAddress"},	
	{ 4,"LANHostConfigManagement.MaxAddress"},	

	{-1,NULL}
};


Reorder_info* map_order_element(ParameterValueStructPtr element)
{
	Reorder_info* curr_row=NULL;

	if(!AZTECH_CWMP_REORDER_VAL)
		return NULL;	
	curr_row=&g_reorder_table;
	while(curr_row->key_word !=NULL)
	{
		if(strstr(element->name,curr_row->key_word) !=NULL)
		{
			tcdbg_printf("#### %s:%d #### HIT !! %s : %d\n",__FUNCTION__,__LINE__,curr_row->key_word,curr_row->prio);		
			return curr_row;
		}
		curr_row++;
	}
	tcdbg_printf("#### %s:%d ####\n",__FUNCTION__,__LINE__);	
	return NULL;
}

int Print_ParameterValueStruct(ParameterValueStructPtr element)
{
	if(!AZTECH_CWMP_REORDER_VAL)
		return 0;
	tcdbg_printf("%s  ",element->name);	
	tcdbg_printf("\n");
	return 1;	
}

int Print_ParameterValue_List(ParameterValueStructPtr List)
{
	ParameterValueStructPtr  curr_element=NULL;
	
	if(!AZTECH_CWMP_REORDER_VAL)
		return 0;	
	curr_element=List;
	int i=0;		
	tcdbg_printf("#### %s:%d ####\n",__FUNCTION__,__LINE__);
	while(curr_element != NULL){
		tcdbg_printf("No:%d ",i++);			
		Print_ParameterValueStruct(curr_element);
		curr_element=curr_element->next;
	}
	tcdbg_printf("#### %s:%d ####\n",__FUNCTION__,__LINE__);		
	return 1;
}


ParameterValueStructPtr get_list_tail(ParameterValueStructPtr list)
{
	ParameterValueStructPtr  curr_element=NULL;

	if(!AZTECH_CWMP_REORDER_VAL)
		return NULL;	
	curr_element=list;
	while(curr_element != NULL){
//			Print_ParameterValueStruct(curr_element);
		if(curr_element->next==NULL)
			return curr_element;
		curr_element=curr_element->next;
	}
	return NULL;
}

//add_list(list_1,get_list_tail(list_1),list_2);

ParameterValueStructPtr add_list(ParameterValueStructPtr list_1,ParameterValueStructPtr pos,ParameterValueStructPtr list_2)
{
	if(!AZTECH_CWMP_REORDER_VAL)
		return NULL;
	if(pos  !=NULL && list_1 !=NULL)
	{		
		if(list_2==NULL)
		{//list_1-->NULL
				return list_1;
		}else
		{//list_1-->list_1_pos -->list_2 -->list_1_pos+1
			ParameterValueStructPtr  tail_element=NULL;		
			if(tail_element=get_list_tail(list_2))
			{
//				ParameterValueStructPtr  pre_pos_next_element=NULL;		
				tail_element->next=pos->next;
				pos->next=list_2;
				return list_1;				
			}
			//impossiable !!!~~~
			assert(0);
		}
		//impossiable !!!~~~
		assert(0);		
	}else
	{
		if(pos ==NULL && list_1!=NULL)
		{
			ParameterValueStructPtr  tail_element=NULL;		
			if(tail_element=get_list_tail(list_2))
			{//list_2-->list_1
				return add_list(list_2,tail_element,list_1);
			}
			//impossiable !!!~~~
			assert(0);
		}

		else if(list_2==NULL)
		{
			return list_1;		
		}
		else
		{//list_2
			return list_2;
		}
		
	}
	return NULL;

}

ParameterValueStructPtr take_and_remove_list(ParameterValueStructPtr *head_p,ParameterValueStructPtr pos)
{
	ParameterValueStructPtr  take_out_element=NULL;		

	if(!AZTECH_CWMP_REORDER_VAL)
		return NULL;	
	if(pos  !=NULL  )
	{
		if(pos->next !=NULL)
		{
			take_out_element=pos->next;
			pos->next=take_out_element->next;
		}
	}else
	{
		take_out_element=(*head_p);
		if((*head_p)!=NULL)
			(*head_p)=(*head_p)->next;
	}
	
	if(take_out_element!=NULL)
		take_out_element->next=NULL;	
	return take_out_element;
}


ParameterValueStructPtr add_sub_sort_list(ParameterValueStructPtr head, ParameterValueStructPtr new_element,pReorder_info new_element_info)
{
/*
2 find prio
3 find pos
4 insert to sub train
*/
	ParameterValueStructPtr  curr_element=NULL,prev_pos=NULL;
	pReorder_info curr_element_info=NULL;

	if(!AZTECH_CWMP_REORDER_VAL)
		return NULL;	
	curr_element=head;
	while(curr_element != NULL){
		if(curr_element_info=map_order_element(curr_element))  
		{
			if(curr_element_info->prio >=new_element_info->prio)
			{
				break;
			}
		}
		prev_pos=curr_element;
		curr_element=curr_element->next;
	}

	return add_list(head, prev_pos,new_element);
					
}


ParameterValueStructPtr resort_ParameterValue_list(ParameterValueStructPtr List)
{
	ParameterValueStructPtr New_List=List,next=NULL;
	ParameterValueStructPtr  curr_element=NULL,prev_element=NULL;
	Reorder_info *insert_element_info=NULL, *pri_curr=NULL;  
	ParameterValueStructPtr sub_head=NULL;  
	ParameterValueStructPtr pos_for_sub_list=NULL;
	ParameterValueStructPtr takeout_element=NULL;

	if(!AZTECH_CWMP_REORDER_VAL)
		return NULL;
	curr_element=New_List;
	while(curr_element != NULL){
	//			Print_ParameterValueStruct(curr_element);
	//			if(map_order_element(curr_element)!=NULL)  
			//if(curr_element->next!=NULL)
				next=curr_element->next;
		if(insert_element_info=map_order_element(curr_element))  
		{//takeout element form main list
			//next=curr_element->next;
			takeout_element=take_and_remove_list(&New_List, prev_element);
			tcdbg_printf("#### %s:%d ####\n",__FUNCTION__,__LINE__);								
			if(takeout_element!=NULL)
			{
				// find pos for sub list in main list	
				if(pos_for_sub_list==NULL)
					pos_for_sub_list=prev_element;					
				//todo : add_sub_sort_list					
				sub_head=add_sub_sort_list(sub_head,takeout_element,insert_element_info);
			}
			prev_element=prev_element;
			//curr_element=next;
		}else
		{			
			prev_element=curr_element;
			//curr_element=next;		
		}
		curr_element=next;
	}
	//todo :	put sub list to original list
	if(sub_head!=NULL)
	{
		tcdbg_printf("#### %s:%d ####\n",__FUNCTION__,__LINE__);										
		return add_list(New_List, pos_for_sub_list,sub_head);
	}
	else
		return New_List;
}

int cwmpExtractBodyInfoToStruct(IXML_Node* requestBodyNode, SoapBodyInfoPtr  requestBodyStructPtr)
{
	IXML_Node* curBodyNode, *oldCurBodyNode; 
	IXML_Node* tempBodyNode;
	RPCArgumentPtr requestArgumentPtr = NULL;
	RPCArgumentPtr curRequestArgumentPtr =NULL;
	int err_state = CWMP_E_SUCCESS;

	if(requestBodyNode == NULL){
		tc_printf("request Body node is NULL!\r\n");
		return -1;
	}
	if(requestBodyStructPtr == NULL){
		tc_printf("request Body struct is NULL!\r\n");
		return -1;
	}

	if((curBodyNode = ixmlNode_getFirstChild(requestBodyNode)) == NULL){
		tc_printf("No rpc name in request packet!\r\n");
		err_state = InvalidArgument;
		goto Error_Handler;
	}
	strcpy(requestBodyStructPtr->rpcName, curBodyNode->localName);

	if((curBodyNode = ixmlNode_getFirstChild(curBodyNode)) == NULL){	//No request parameter
		err_state = cwmpBuildDefautReqArg(requestBodyStructPtr);
		if(err_state != CWMP_E_SUCCESS){
			tc_printf("Build default request argument fails!");
			goto Error_Handler;
		}
	}
	else{     // have request parameters
	  // build request arguments.
		while(curBodyNode != NULL){
			void* curPara = NULL;
			void* rootPara = NULL;
			
			oldCurBodyNode = curBodyNode;
			if((requestArgumentPtr= (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
				tc_printf("malloc request argument fails!\r\n");
				err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				goto Error_Handler;
			}
			memset(requestArgumentPtr, 0, sizeof(RPCArgument));

			if(requestBodyStructPtr->argumentPtr == NULL){
				requestBodyStructPtr->argumentPtr = requestArgumentPtr;
				curRequestArgumentPtr = requestArgumentPtr;
			}
			else{
				curRequestArgumentPtr->next = requestArgumentPtr;
//				curRequestArgumentPtr = requestArgumentPtr;
			}
			
			err_state = cwmpBuildRequestArgument(requestArgumentPtr, curBodyNode);
			if( err_state != CWMP_E_SUCCESS){
					tc_printf("build request argument fails!\r\n");
				goto Error_Handler;
			}
			if(!requestArgumentPtr->beArray)
				goto SetArgument;
		
			// build parameter list
			curBodyNode = ixmlNode_getFirstChild(curBodyNode);
			while(curBodyNode != NULL){
				switch (requestArgumentPtr->type){
					case String256Type:
					case StringType:
						{
						StringStructPtr tempCurPara = (StringStructPtr)curPara;
						StringStructPtr tempPara = NULL;

						if((tempPara = (StringStructPtr)os_malloc(sizeof(StringStruct))) == NULL){
							tc_printf("malloc string struct fails!\r\n");
							err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
							goto Error_Handler;
						}
						memset(tempPara, 0, sizeof(StringStruct));

						if (rootPara == NULL){
							curPara = tempPara;
							rootPara = tempPara;
							requestArgumentPtr->parameterList = rootPara;
						}
						else{
							tempCurPara ->next = tempPara;
//							curPara = tempPara;
						}
						
						tempBodyNode = ixmlNode_getFirstChild(curBodyNode);
						if(tempBodyNode != NULL){
							if((tempPara->value = (char*)os_malloc(strlen(tempBodyNode->nodeValue)+1)) != NULL)   //xzwang_060624
							strcpy(tempPara->value, tempBodyNode->nodeValue);
							else{
								/* added by hqfeng,2006.08.08 */
								if(tempPara != NULL){
									os_free(tempPara);
									tempPara = NULL;
								}
								tc_printf("malloc value of string type parameter fails!\r\n");
								err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
								goto Error_Handler;
							}
							tempPara->next = NULL;
						}
						else{
							//Ignor no parameter value,hqfeng,2006.06.02
							tc_printf("can not get parameter value of string type argument!\r\n");
						//	return -1;
						}

/*
						if (rootPara == NULL){
							curPara = tempPara;
							rootPara = tempPara;
						}
						else{
							tempCurPara ->next = tempPara;
							curPara = tempPara;
						}
*/
						curPara = tempPara;
						requestArgumentPtr->number++;
						break;
						}
					case ParameterValueStructType:
					{
						ParameterValueStructPtr tempCurPara = (ParameterValueStructPtr)curPara;
						ParameterValueStructPtr tempPara = NULL;

						err_state = cwmpExtractBodyInfoToValueStruct(curBodyNode, &tempPara);
						if(tempPara != NULL){
							if (rootPara == NULL){
								curPara = tempPara;
								rootPara = tempPara;
								requestArgumentPtr->parameterList = rootPara;
							}
							else{
								tempCurPara ->next = tempPara;
//								curPara = tempPara;
							}
						}

						if( err_state != CWMP_E_SUCCESS){
							tc_printf("Extrat body info to value struct fails!\r\n");
							goto Error_Handler;
						}
/*						
						if (rootPara == NULL){
							curPara = tempPara;
							rootPara = tempPara;
						}
						else{
							tempCurPara ->next = tempPara;
							curPara = tempPara;
						}
*/
						curPara = tempPara;
						requestArgumentPtr->number++;
						break;
					}

					case SetParameterAttributesStructType:
					{
						ParameterAttributesSetStructPtr tempCurPara = (ParameterAttributesSetStructPtr)curPara;
						ParameterAttributesSetStructPtr tempPara = NULL;

						err_state = cwmpExtractBodyInfoToAttriStruct(curBodyNode, &tempPara);
						if(tempPara != NULL){
							if (rootPara == NULL){
								curPara = tempPara;
								rootPara = tempPara;
								requestArgumentPtr->parameterList = rootPara;
							}
							else{
								tempCurPara ->next = tempPara;
//								curPara = tempPara;
							}

						}

						if( err_state != CWMP_E_SUCCESS){
							tc_printf("Extrat body info to attribute struct fails!\r\n");
							goto Error_Handler;
						}						
/*
						if (rootPara == NULL){
							curPara = tempPara;
							rootPara = tempPara;
						}
						else{
							tempCurPara ->next = tempPara;
							curPara = tempPara;
						}
*/
						curPara = tempPara;
						requestArgumentPtr->number++;
						break;
					}
					default:
						tc_printf("Invalid argument type or the argument is not surpported!\r\n");
						break;
				}
				curBodyNode = curBodyNode->nextSibling;
			}

			if(AZTECH_CWMP_REORDER_VAL){
				if(requestArgumentPtr->type==ParameterValueStructType)
				{
					if(rootPara!=NULL)
					{

						Print_ParameterValue_List((ParameterValueStructPtr)rootPara);
						rootPara=resort_ParameterValue_list((ParameterValueStructPtr)rootPara);
						Print_ParameterValue_List((ParameterValueStructPtr)rootPara);
						requestArgumentPtr->parameterList = rootPara;
					}
				}
			}
		SetArgument:
//			requestArgumentPtr->parameterList = rootPara;
/*				
			if(requestBodyStructPtr->argumentPtr == NULL){
				requestBodyStructPtr->argumentPtr = requestArgumentPtr;
				curRequestArgumentPtr = requestArgumentPtr;
			}
			else{
				curRequestArgumentPtr->next = requestArgumentPtr;
				curRequestArgumentPtr = requestArgumentPtr;
			}
*/
			curRequestArgumentPtr = requestArgumentPtr;

			curBodyNode = oldCurBodyNode->nextSibling;
		}
	}

	return CWMP_E_SUCCESS;

Error_Handler:

#if 0
	/* added by hqfeng,2006.08.08 */
	if(requestArgumentPtr != NULL){
		cwmpFreeArgment(&requestArgumentPtr);
	}
#endif

	return err_state;
}


int cwmpBuildDefautReqArg( SoapBodyInfoPtr  requestBodyStructPtr){
	RPCArgumentPtr requestArgumentPtr = NULL;
	StringStructPtr requestParaLst = NULL;
	int err_state = CWMP_E_SUCCESS;
		
	if(requestBodyStructPtr == NULL)
		return -1;
	
	if((requestArgumentPtr= (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc request argument fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(requestArgumentPtr, 0, sizeof(RPCArgument));
	requestArgumentPtr->type = StringType;

	if(strcmp(requestBodyStructPtr->rpcName,"GetParameterValues") == 0
		|| strcmp(requestBodyStructPtr->rpcName,"GetParameterAttributes") == 0){
		
		if((requestParaLst = (StringStructPtr)os_malloc(sizeof(StringStruct))) == NULL){
			tc_printf("malloc request parameterlist fails!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}

		if((requestArgumentPtr->name = (char*)os_malloc(strlen("ParameterNames")+1)) != NULL){   //xzwang_060624
			strcpy(requestArgumentPtr->name,"ParameterNames");
		}else{
			tc_printf("malloc request argument name fails!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}

		if((requestParaLst->value = (char*)os_malloc(strlen("InternetGatewayDevice.")+1)) != NULL){   //xzwang_060624
			strcpy(requestParaLst->value,"InternetGatewayDevice.");
		}else{
			tc_printf("malloc value of string type parameter fails for cwmpBuildDefautReqArg!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}
		requestArgumentPtr->number = 1;
		requestArgumentPtr->beArray = 1;
		requestParaLst->next = NULL;
	}
	else if(strcmp(requestBodyStructPtr->rpcName,"GetRPCMethods") == 0){
		requestParaLst = NULL;
	}	
	else if(strcmp(requestBodyStructPtr->rpcName,"GetParameterNames") == 0){
		
		strcpy(requestArgumentPtr->name,"ParameterPath");
		if((requestArgumentPtr->value=os_malloc(strlen("InternetGatewayDevice.") + 1)) == NULL){
			tc_printf("malloc request parameterlist fails!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}
              memset(requestArgumentPtr->value, 0, strlen("InternetGatewayDevice.") + 1);
		strcpy((char*)requestArgumentPtr->value,"InternetGatewayDevice."); 
		requestArgumentPtr->number = 1;
		requestArgumentPtr->beArray = 0;

		cwmpClearNextLevelFlag();
	}else{
		//Ignor Invalid argument.hqfeng,2006.06.02
		tc_printf("Ignor Invalid request soap packet!\r\n");
//		return -1;
	}
	requestArgumentPtr->parameterList = requestParaLst;

	requestBodyStructPtr ->argumentPtr = requestArgumentPtr;

	return CWMP_E_SUCCESS;

Error_Handler:
	
	//added by hqfeng,2006.08.08
	if(requestArgumentPtr != NULL){
		cwmpFreeArgment(&requestArgumentPtr);
	}
	if(requestParaLst != NULL){
		os_free(requestParaLst);
		requestParaLst = NULL;
	}
	
	return err_state;
}

int cwmpBuildRequestArgument(RPCArgumentPtr requestArgumentPtr , IXML_Node* curBodyNode){
	IXML_Node* tempBodyNode = NULL;
	int err_state = 0;

	if(requestArgumentPtr == NULL)
		return -1;
	if(curBodyNode == NULL)
		return -1;
	
	//get argument name.
	if((requestArgumentPtr->name = (char*)os_malloc(strlen(curBodyNode->localName)+1)) != NULL){   //xzwang_060624
		strcpy(requestArgumentPtr->name, curBodyNode->localName);
	}else{
		tc_printf("malloc request argument name fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	// get argument attribute.
	tempBodyNode = curBodyNode->firstAttr;
	while(tempBodyNode != NULL && cwmpBeArrayType(tempBodyNode->nodeName) != 1/*strcmp(tempBodyNode->nodeName,"SOAP-ENC:arrayType") != 0*/)
		tempBodyNode = tempBodyNode->nextSibling;

	if(tempBodyNode != NULL){  //with array attribute node.
		if(cwmpGetArgumentType(tempBodyNode->nodeValue, &requestArgumentPtr->type) != CWMP_E_SUCCESS){ //get argument type
			tc_printf("extract argument type fails!\r\n");
			return InvalidArgument;
		}
		else{
			requestArgumentPtr->beArray = 1;
		}	
	}	
	else{ //without array attribute node
		//judge whether the argument is array type by its name. xzwang_060720
		if(cwmpBeArrayArgument(curBodyNode, &requestArgumentPtr->type)){  //array type
			requestArgumentPtr->beArray = 1;
		}
		else{  //not array type
			//handle fault packet. xzwang_060731
			if(strcmp(curBodyNode->localName, "detail") == 0){
				FaultDetailPtr tempArg = NULL;
				IXML_Node* tempNode = NULL;

				tempNode = curBodyNode->firstChild;
				if(tempNode == NULL){
					tc_printf("Wrong packet format for fault detail from ACS!\r\n");
					return 0;
				}
				if(strcmp(tempNode->localName, "Fault") == 0){
					if((err_state = cwmpExtractArgInfoToFaultDetailStruct(tempNode, &tempArg)) != 0)
						return err_state;
					requestArgumentPtr->value = tempArg;
					requestArgumentPtr->type = FaultDetailType;
					requestArgumentPtr->beArray = 0;
				}
				else{
					tc_printf("Invalid Fault detail format!\r\n");
				}
			}
			else{ //handle others.
				curBodyNode = ixmlNode_getFirstChild(curBodyNode);
				if(curBodyNode && curBodyNode->nodeType == eTEXT_NODE){
					if((requestArgumentPtr->value = os_malloc(strlen(curBodyNode->nodeValue)+1)) != NULL){
					        memset(requestArgumentPtr->value, 0, strlen(curBodyNode->nodeValue) + 1);
						requestArgumentPtr->beArray = 0;
						strcpy((char*)requestArgumentPtr->value, curBodyNode->nodeValue);
						if(strcmp(requestArgumentPtr->name, "ParameterKey") == 0){
							//tc_printf("\r\n ParameterKey = ");
							//tc_printf(requestArgumentPtr->value);
							/* put parameterkey on this temp variable */
							strcpy(gParameterKey, requestArgumentPtr->value);
						}
					}
					else{
						tc_printf("malloc request argument value fails!\r\n");
						return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
					}
				}
				else{
					tc_printf("without argument value node!\r\n");
		//			return -1; //marked by hqfeng, for parsing no value argument.
				}
			}
		}
	}

	return CWMP_E_SUCCESS;

}

int cwmpExtractArgInfoToFaultDetailStruct(IXML_Node* curBodyNode, FaultDetailPtr* ArgP){
	FaultDetailPtr tempArg = NULL;
	IXML_Node* tempBodyNode;
	IXML_Node* curTempBodyNode;
	int err_state = CWMP_E_SUCCESS;

	if((tempArg = (FaultDetailPtr)os_malloc(sizeof(FaultDetail))) == NULL){
		tc_printf("malloc string struct fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}	
	memset(tempArg, 0, sizeof(FaultDetail));

	curTempBodyNode = curBodyNode->firstChild;
	while(curTempBodyNode != NULL){
		if(strcmp(curTempBodyNode->localName,"FaultCode") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode != NULL){
				tempArg->faultCode = atoi(tempBodyNode->nodeValue);
				if(dbg_cwmp >= DBG_CWMP_L2){
					tc_printf("\r\ncwmpRpc.c---[cwmpExtractArgInfoToFaultDetailStruct]--faultCode = %ld",tempArg->faultCode);
				}
			}	
			else{
				tc_printf("can not get Name parameter value of ParameterValueStruct!\r\n");
			}
		}
		else if(strcmp(curTempBodyNode->localName,"FaultString") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode != NULL){
				if((tempArg->faultString= (char*)os_malloc(strlen(tempBodyNode->nodeValue)+1)) == NULL){
					tc_printf("malloc parmeter value fails!\r\n");
					err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
					goto Error_Handler;
				}
				strcpy(tempArg->faultString, tempBodyNode->nodeValue);
			}
			else{
				tc_printf("\r\nWithout faultString value node\n");
			}	
		}
		else{
			tc_printf("Invalid Fault argument member!\r\n");
		}
		curTempBodyNode = curTempBodyNode->nextSibling;
	}


	*ArgP = tempArg;

	return err_state;

Error_Handler:
	
	if(tempArg != NULL){
		if(tempArg->faultString != NULL){
			os_free(tempArg->faultString);
			tempArg->faultString = NULL;
		}
		os_free(tempArg);
		tempArg = NULL;
	}
	
	return err_state;
}

int cwmpBeArrayArgument(IXML_Node* argNode, ParameterType *argTypeP){
	IXML_Node* curNode = argNode, *tempNode = NULL;
	ParameterType type;
	char *name;
	
	if(argNode == NULL){
		tc_printf("\r\n Argument node is NULL!");
		return 0;
	}

	name = argNode->localName;

	if(strcmp(name, "ParameterList") == 0){
		tempNode = argNode->firstChild;
		if(tempNode == NULL){
			tc_printf("Wrong packet format for parameterList from ACS!\r\n");
			return 0;
		}
		if(strcmp(tempNode->localName, "ParameterValueStruct") == 0){
			*argTypeP = ParameterValueStructType;
			return 1;
		}
		if(strcmp(tempNode->localName, "SetParameterAttributesStruct") == 0){
			*argTypeP = SetParameterAttributesStructType;
			return 1;
		}
		else{
			tc_printf("\r\nUnknown parameter list type!");
		}
	}
	else if(strcmp(name, "ParameterNames") == 0){
		*argTypeP = StringType;
		return 1;
	}
	else if(strcmp(name, "MethodList") == 0){
		*argTypeP = StringType;
		return 1;
	}
	

	return 0;
}

int cwmpBeArrayType(char* nodeName){
	char* tempName = nodeName;

	if(getNoArrayTypeFlag() == 1){
		if(dbg_cwmp >= DBG_CWMP_L2 ){
    		   	tc_printf("\r\n attr is including arrayType, but force to return 0 for ITMS testing \r\n ");
        	}
		return 0; 
	}
	
	if(nodeName == NULL)
		return 0;
	
	while(*tempName != 0){
		if(*tempName == ':')
			break;
		tempName++;
	}

	if(*tempName != 0){
		tempName++;
		if(strcmp(tempName,"arrayType") == 0)
			return 1;
	}

	return 0;
}

int cwmpExtractBodyInfoToValueStruct(IXML_Node* curBodyNode, ParameterValueStructPtr* tempParaP){
	ParameterValueStructPtr tempPara = NULL;
	IXML_Node* tempBodyNode;
	IXML_Node* curTempBodyNode;
	int err_state = CWMP_E_SUCCESS;

	if((tempPara = (ParameterValueStructPtr)os_malloc(sizeof(ParameterValueStruct))) == NULL){
		tc_printf("malloc string struct fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}	
	memset(tempPara, 0, sizeof(ParameterValueStruct));
	*tempParaP = tempPara;

	curTempBodyNode = curBodyNode->firstChild;
	while(curTempBodyNode != NULL){
		if(strcmp(curTempBodyNode->localName,"Name") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode){
				if((tempPara->name = (char*)os_malloc(strlen(tempBodyNode->nodeValue)+1)) != NULL) //xzwang_060623
				strcpy(tempPara ->name,tempBodyNode->nodeValue);
			else{
					tc_printf("malloc ParameterValue name  fails for extract body info!\r\n");
					err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
					goto Error_Handler;
				}
			}	
			else{
			#if 0 //xzwang_060719	
				if(dbg_cwmp >= DBG_CWMP_L1 )
					tc_printf("can not get Name parameter value of ParameterValueStruct!\r\n");
				err_state = InvalidArgument;
				goto Error_Handler;
			#else
				tc_printf("can not get Name parameter value of ParameterValueStruct!\r\n");
			#endif
			}
		}
		else if(strcmp(curTempBodyNode->localName,"Value") == 0){
			if((tempPara->valueInfoPtr = (ValueInfoPtr)os_malloc(sizeof(ValueInfo))) == NULL){
				tc_printf("malloc parameter value info fails!\r\n");
				err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				goto Error_Handler;
			}
			memset(tempPara->valueInfoPtr, 0, sizeof(ValueInfo));
			
			tempBodyNode = curTempBodyNode->firstAttr;
			while(tempBodyNode != NULL && strcmp(tempBodyNode->nodeName,"xsi:type") != 0)
				tempBodyNode = tempBodyNode->nextSibling;
			if(tempBodyNode){//get argument type
				err_state = cwmpGetArgumentType(tempBodyNode->nodeValue, &tempPara->valueInfoPtr->type);
				if( err_state != CWMP_E_SUCCESS){ 
					tc_printf("extract parameter value type fails!\r\n");
					goto Error_Handler;
				}	
			}
			else{
				tc_printf("without parameter value attribute node!\r\n");
				//	return -1;
			}	
				
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode){
				if((tempPara->valueInfoPtr->value = (char*)os_malloc(strlen(tempBodyNode->nodeValue)+1)) == NULL){
					tc_printf("malloc parmeter value fails!\r\n");
					err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
					goto Error_Handler;
				}
				strcpy(tempPara->valueInfoPtr->value, tempBodyNode->nodeValue);
			}
			else{
				//Set empty string for parameters of string type. xzwang_20061121
				if(tempPara->valueInfoPtr->type == StringType){
					if((tempPara->valueInfoPtr->value = (char*)os_malloc(2)) == NULL){
						tc_printf("malloc parmeter value fails!\r\n");
						err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
						goto Error_Handler;
					}
					tempPara->valueInfoPtr->value[0] = 0;
				}
					
				tc_printf("without parameter value node:%s\r\n",tempPara->name);
			#if 0 //xzwang_060719	
				err_state = InvalidArgument;     //xzwang_060704
				goto Error_Handler;
			#endif	
			}	
		}
		else{
			tc_printf("Invalid parameter member!\r\n");
		}
		curTempBodyNode = curTempBodyNode->nextSibling;
	}
	tempPara->next = NULL;

//	*tempParaP = tempPara;

Error_Handler:
	
	return err_state;
}

int cwmpExtractBodyInfoToAttriStruct(IXML_Node* curBodyNode, ParameterAttributesSetStructPtr* tempParaP){
	ParameterAttributesSetStructPtr tempPara = NULL;
	IXML_Node* curTempBodyNode;
	IXML_Node* tempBodyNode;
	int err_state = CWMP_E_SUCCESS;
	
	if((tempPara = (ParameterAttributesSetStructPtr)os_malloc(sizeof(ParameterAttributesSetStruct))) == NULL){
		tc_printf("malloc parameter attribute set fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}
	memset(tempPara, 0, sizeof(ParameterAttributesSetStruct));
	*tempParaP = tempPara;
	
	curTempBodyNode = curBodyNode->firstChild;
	while(curTempBodyNode != NULL){
		if(strcmp(curTempBodyNode->localName,"Name") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode){
				if((tempPara->name = (char*)os_malloc(strlen(tempBodyNode->nodeValue)+1)) != NULL) //xzwang_060623
				strcpy(tempPara->name, tempBodyNode->nodeValue);
			else{
					tc_printf("malloc ParameterAttribute's name  fails for extract body info!\r\n");
					err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
					goto Error_Handler;
				}
			}	
			else{
		#if 0 //do not handle as fault. xzwang_060719
				
				if(dbg_cwmp >= DBG_CWMP_L1 )
					tc_printf("name node of attribute is NULL!\r\n ");
				err_state = InvalidArgument;
				goto Error_Handler;
		#else
				tc_printf("name node of attribute is NULL!\r\n ");
		#endif
			}
		}
		else if(strcmp(curTempBodyNode->localName,"NotificationChange") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode){
			#if 1  // for understanding "true" and "false". xzwang_061012	
				int8 nfchange = cwmpGetBooleanValue(tempBodyNode->nodeValue);
				if(nfchange == -1){
					int rtnStatus, rtncode;
					char faultString[FaultStringLength];
					
					sprintf(faultString, "The setting Attribute for %s is rejected!", tempPara->name);
					rtnStatus = InvalidParameterValue;
					rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
					if(rtncode != 0)
						return rtncode;
					return rtnStatus;
				}
				tempPara->notificationChange = (uint8)nfchange;
			#else
				tempPara->notificationChange = atoi(tempBodyNode->nodeValue);
			#endif
			}	
			else{
			#if 0 //do not handle as fault. xzwang_060719
				if(dbg_cwmp >= DBG_CWMP_L1 )
					tc_printf("NotificationChange node of attribute is NULL!\r\n ");
				err_state = InvalidArgument;
				goto Error_Handler;
			#else
				tc_printf("NotificationChange node of attribute is NULL!\r\n ");
			
			#endif	
			}
		}
		else if(strcmp(curTempBodyNode->localName,"Notification") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode)
				tempPara->notification = atoi(tempBodyNode->nodeValue);
			else{
			#if 0 //do not handle as fault. xzwang_060719
				if(dbg_cwmp >= DBG_CWMP_L1 )
					tc_printf("Notification node of attribute is NULL!\r\n ");
				err_state = InvalidArgument;
				goto Error_Handler;
			#else
				tc_printf("Notification node of attribute is NULL!\r\n ");
			#endif	
			}
		}
		else if(strcmp(curTempBodyNode->localName,"AccessListChange") == 0){
			tempBodyNode = curTempBodyNode->firstChild;
			if(tempBodyNode)
				tempPara->accessListChange = atoi(tempBodyNode->nodeValue);
			else{
			#if 0 //do not handle as fault. xzwang_060719
				if(dbg_cwmp >= DBG_CWMP_L1 )
					tc_printf("AccessListChange node of attribute is NULL!\r\n ");
				err_state = InvalidArgument;
				goto Error_Handler;
			#else
				tc_printf("AccessListChange node of attribute is NULL!\r\n ");

			#endif	
			}
		}
		else if(strcmp(curTempBodyNode->localName,"AccessList") == 0){
			AccessEntryPtr tempEntryPtr;
			if((tempEntryPtr = (AccessEntryPtr)os_malloc(sizeof(AccessEntry))) == NULL){
				tc_printf("malloc access entry fails!\r\n");
				err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				goto Error_Handler;
			}
			memset(tempEntryPtr, 0, sizeof(AccessEntry));
			tempPara->accessList = tempEntryPtr;
			
			tempBodyNode = curTempBodyNode->firstChild;
			err_state = cwmpBuildAccessList(tempBodyNode, tempPara->accessList);
			if( err_state != CWMP_E_SUCCESS){
				tc_printf("AccessList node of attribute is NULL!\r\n ");
				goto Error_Handler;
			}
		}
		else
			tc_printf("Invalid paramter attribute member!\r\n");
		
		curTempBodyNode = curTempBodyNode->nextSibling;
	}
	tempPara->next = NULL;

//	*tempParaP = tempPara;

Error_Handler:
	
	return err_state;
}


int cwmpBuildAccessList(IN IXML_Node* bodyNode, OUT AccessEntry* accessList){
	IXML_Node* curNode = bodyNode;
	IXML_Node* tempNode = NULL;
    AccessEntryPtr curAccessEntry = accessList;
	AccessEntryPtr tempEntryPtr;

	if(bodyNode == NULL){
		tc_printf("The access list node is NULL!\r\n");
		return 0;       /*InvalidArgument*/ //do not handle as fault. xzwang_060719
	}
	if(accessList == NULL){
		tc_printf("The access list struct pointer is NULL!\r\n");
		return InvalidArgument;
	}

	while(curNode){
		tempNode = curNode->firstChild;
		if(tempNode){
			strcpy(curAccessEntry->entryName,tempNode->nodeValue);
		}
		else{
			tc_printf("The text node of accesslist is NULL!\r\n");
			curNode = curNode->nextSibling; //modified by xzwang_060818
			continue;
			//return InvalidArgument; 
		}
		curNode = curNode->nextSibling;
		if(curNode){
			if((tempEntryPtr = (AccessEntryPtr)os_malloc(sizeof(AccessEntry))) == NULL){
				tc_printf("malloc access entry fails!\r\n");
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			}
			memset(tempEntryPtr, 0, sizeof(AccessEntry));
			curAccessEntry->next = tempEntryPtr;
			curAccessEntry = tempEntryPtr;
		}
	}

	return 0;
}

int cwmpGetArgumentType(char* attributeValue, ParameterTypePtr argumentTypePtr){
	char* tempValue;
	int i = 0,typeStart = -1,typeEnd = -1;
	int len;
	int op_flag = 0;

	if(attributeValue == NULL){
		tc_printf("attribute value is NULL!\r\n");
		return InvalidArgument;
	}
	if(argumentTypePtr == NULL){
		tc_printf("The argument type pointer is NULL!\r\n");
		return InvalidArgument;
	}

	if((tempValue = (char*)os_malloc(strlen(attributeValue)+1)) == NULL){
		tc_printf("[cwmpGetArgumentType] malloc fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	//memset(tempValue,0,strlen(tempValue));

    while (attributeValue[i] != '\0'){
		if(attributeValue[i] == ':')
			typeStart = i+1;
		if(attributeValue[i] == '[')
			typeEnd = i-1;
		i++;
	}
	if(typeStart == -1) //for without character ':'
		typeStart = 0;
	if(typeEnd == -1)  // for without character '['
		typeEnd = i-1;

	if(typeEnd == 0 || typeEnd < typeStart){
		tc_printf("argument attribute string error!\r\n");
		/*added for free tempValue, by hqfeng, 2006.08.08*/
		if(tempValue != NULL){
			os_free(tempValue);
			tempValue = NULL;
		}
		return InvalidArgument;
	}

	strncpy(tempValue,&attributeValue[typeStart],typeEnd-typeStart+1);
	tempValue[typeEnd-typeStart+2] = 0;

	len = typeEnd-typeStart+1;
	if(TCSUPPORT_CWMP_IOT_STRINGTYPE_VAL)
		op_flag = (strncmp(tempValue, "string",strlen("string")) == 0);
	else
		op_flag = (strncmp(tempValue, "string",len) == 0);
	if(op_flag)
		*argumentTypePtr = StringType;
	else if(strncmp(tempValue, "int", len) == 0)
		*argumentTypePtr = IntType;
	else if(strncmp(tempValue, "unsignedInt", len) == 0)
		*argumentTypePtr = UnsignedIntType;
	else if(strncmp(tempValue, "boolean",len) == 0)
		*argumentTypePtr = BooleanType;
	else if(strncmp(tempValue, "dateTime",len) == 0)
		*argumentTypePtr = DateTimeType;
	else if(strncmp(tempValue, "base64",len) == 0)
		*argumentTypePtr = Base64Type;
	else if(TCSUPPORT_ARC_CWMP_VAL && strncmp(tempValue, "string(256)",len) == 0)
		*argumentTypePtr = String256Type;
	else if(strncmp(tempValue, "ParameterValueStruct",len) == 0)
		*argumentTypePtr = ParameterValueStructType;
	else if(strncmp(tempValue, "SetParameterAttributesStruct",len) == 0)
		*argumentTypePtr = SetParameterAttributesStructType;
	else{
		tcdbg_printf("The argument type \"%s\" is not supported by cpe\r\n!", tempValue);
		/*added for free tempValue, by hqfeng, 2006.08.08*/
		if(tempValue != NULL){
			os_free(tempValue);
			tempValue = NULL;
		}
		return InvalidArgument;
	}

	if(tempValue != NULL){
		os_free(tempValue);
		tempValue = NULL;
	}
	
	return CWMP_E_SUCCESS;
}


/************************************ Build response struct **********************************/

int cwmpBuildResponseStruct(SoapInfoPtr  requestStructPtr, SoapInfoPtr  responseStructPtr){
	SoapHeadInfoPtr requestHeadStructPtr = NULL;
	SoapBodyInfoPtr requestBodyStructPtr = NULL;
	SoapHeadInfoPtr responseHeadStructPtr = NULL;
	SoapBodyInfoPtr responseBodyStructPtr = NULL;
	int err_state = CWMP_E_SUCCESS;

	if(requestStructPtr == NULL){
		tc_printf("requestStructPtr is NULL!\r\n");
		return -1;
	}
	if(responseStructPtr == NULL){
		tc_printf("responseStructPtr is NULL!\r\n");
		return -1;
	}
	
	requestHeadStructPtr = requestStructPtr->headStructPtr;
	requestBodyStructPtr = requestStructPtr->bodyStructPtr;

	if(requestHeadStructPtr != NULL){
		if((responseStructPtr->headStructPtr = (SoapHeadInfoPtr)os_malloc(sizeof(SoapHeadInfo))) == NULL){
			tc_printf("malloc response head struct error!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}
		memset(responseStructPtr->headStructPtr, 0, sizeof(SoapHeadInfo));
	}

	if(requestBodyStructPtr != NULL){
		if((responseStructPtr->bodyStructPtr = (SoapBodyInfoPtr)os_malloc(sizeof(SoapBodyInfo))) == NULL){
			tc_printf("malloc response body struct error!\r\n");
			err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			goto Error_Handler;
		}
		memset(responseStructPtr->bodyStructPtr, 0, sizeof(SoapBodyInfo));
	}
	
	responseHeadStructPtr = responseStructPtr->headStructPtr;
	responseBodyStructPtr = responseStructPtr->bodyStructPtr;
	
	if(responseHeadStructPtr){
		err_state = cwmpBuildResponseHeadStruct(requestHeadStructPtr,responseHeadStructPtr);
		if( err_state != CWMP_E_SUCCESS){
			tc_printf("build response head struct fails!\r\n");
			goto Error_Handler;
		}
	}
	if(responseBodyStructPtr){
		err_state = cwmpBuildResponseBodyStruct(requestBodyStructPtr, responseBodyStructPtr);
		if( err_state != CWMP_E_SUCCESS){
			tc_printf("build response body struct fails!\r\n");
			goto Error_Handler;
		}
	}

Error_Handler:
	
	return err_state;
}

//response head struct.
int cwmpBuildResponseHeadStruct(SoapHeadInfoPtr  requestHeadStructPtr, SoapHeadInfoPtr  responseHeadStructPtr){
	if(requestHeadStructPtr == NULL){
		tc_printf("requestHeadStructPtr is NULL!\r\n");
		return -1;
	}
	if(responseHeadStructPtr == NULL){
		tc_printf("responseHeadStructPtr is NULL!\r\n");
		return -1;
	}

	strcpy(responseHeadStructPtr->id, requestHeadStructPtr->id);
	strcpy(responseHeadStructPtr->holdRequest,"");
	cwmpSetNoMoreRequest(responseHeadStructPtr->noMoreRequest);

	return 0;
}

int cwmpSetNoMoreRequest(char* noMoreRequest){
	strcpy(noMoreRequest,"");  // need further handle.
	return 0;
}

//response body struct.
int cwmpBuildResponseBodyStruct(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCMethodFun RPCMethodResponse = NULL;
	int err_state = CWMP_E_SUCCESS;
	char faultString[FaultStringLength];
	int rtncode;

	if(requestBodyStructPtr == NULL){
		tc_printf("requestBodyStructPtr is NULL!\r\n");
		return -1;
	}
	if(responseBodyStructPtr == NULL){
		tc_printf("responseBodyStructPtr is NULL!\r\n");
		return -1;
	}

	if(strcmp(requestBodyStructPtr->rpcName, "InformResponse") !=0){
		strcpy(responseBodyStructPtr->rpcName, requestBodyStructPtr->rpcName);
		strcat(responseBodyStructPtr->rpcName, "Response");
	}
	else{
		if(dbg_cwmp >= DBG_CWMP_L2 )
			tc_printf("The request is informresponse!\r\n");
	}
	
	if(cwmpGetRPCResponseFun(requestBodyStructPtr->rpcName,&RPCMethodResponse) == -1){
		tc_printf("\r\nDo not support the RPC: %s",requestBodyStructPtr->rpcName);
		sprintf(faultString, "Not Support the RPC Method:%s", requestBodyStructPtr->rpcName);//by hqfeng,2006.07.06
		err_state = MethodNotSupport;
		rtncode = cwmpCreateDetailFaultStruct(err_state, faultString);
		if(rtncode != 0)
			return rtncode;
		return err_state;
	}

	if(RPCMethodResponse != NULL){
		err_state = RPCMethodResponse(requestBodyStructPtr, responseBodyStructPtr);
		if( err_state != CWMP_E_SUCCESS){
			tc_printf("The RPC response fails!\r\n");
			return err_state;
		}
		if(TCSUPPORT_PON_ROSTELECOM_VAL && CWMP_REDIRECT_VAL){
			if(rpc_MethodMapTable[4].RPC_Method == RPCMethodResponse){
				PrivCwmpGetDataFlag = 1;
			}
			else{
				PrivCwmpGetDataFlag = 0;
			}
		}
	}
	
	return CWMP_E_SUCCESS;
}

// Get rpc response function.
int cwmpGetRPCResponseFun(char* rpcName, RPCMethodFun* RPCMethodResponsePtr){
	uint8 mapflag = -1;
	cwmp_rpc_methodmap* tempRpcM =  &rpc_MethodMapTable[0];

	/*Added by hqfeng, 2006.08.08*/
	if(rpcName == NULL){
		return -1;
	}
	
	while(tempRpcM->name != NULL){
		if( strcmp(rpcName,tempRpcM->name) == 0){
			mapflag = 0;
			if(tempRpcM->RPC_Method != NULL){
//				cwmpSetRpcResponseFlag(&rpc);//marked by hqfeng,2006.06.21
				*RPCMethodResponsePtr = tempRpcM->RPC_Method;
			}
			break;
		}
		tempRpcM++;
	}	

	return mapflag;
}


// GetRPCMethodsResponse
int cwmpGetRPCMethodsResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	cwmp_rpc_methodmap* tempRpcM =  &rpc_MethodMapTable[0];
	StringStructPtr rootResPara = NULL;
	StringStructPtr resPara = NULL;
	StringStructPtr tempResPara = NULL;
	int err_status = CWMP_E_SUCCESS;


	if(responseBodyStructPtr == NULL){
		return -1;
	}
	
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;

	if((err_status = cwmpConfigArgumentStruct("MethodList", StringType, 0, 1, responseArgumentPtr)) != 0){
		tc_printf("Configure  response argument fails for cwmpGetRPCMethodsResponse !\r\n");
		return err_status;//replace InternalError by ResourceExceed,hqfeng,2006.06.21

	}
	
	responseArgumentPtr->next = NULL;

	while(tempRpcM->name != NULL){
		if(strcmp(tempRpcM->name, "InformResponse") == 0 
		   || strcmp(tempRpcM->name, "TransferCompleteResponse") == 0
		   || strcmp(tempRpcM->name, "Fault") == 0 //added by hqfeng,2006.08.09
		   || strcmp(tempRpcM->name, "GetRPCMethodsResponse") == 0)//added by hqfeng,2006.08.09
		{
			tempRpcM++;
			continue;
		}
		
		tempResPara = NULL;
		if((tempResPara= (StringStructPtr)os_malloc(sizeof(StringStruct))) == NULL){
			tc_printf("malloc method string struct fails!\r\n");
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}
		
		if((tempResPara->value = (char*)os_malloc(strlen(tempRpcM->name)+1)) != NULL){   //xzwang_060624
			strcpy(tempResPara->value, tempRpcM->name);
		}else{
			tc_printf("malloc value of string type parameter fails for cwmpGetRPCMethodsResponse!\r\n");
			os_free(tempResPara);
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}

		tempResPara->next = NULL;
		
		if (resPara == NULL){
//			rootResPara = tempResPara;
			responseArgumentPtr->parameterList = tempResPara;
			resPara = tempResPara;
		}
		else{
			resPara->next = tempResPara;
			resPara = tempResPara;
		}			
	
		responseArgumentPtr->number++;
		tempRpcM++;
	}	

//	responseArgumentPtr->parameterList = rootResPara;
	
	return 0;
}

#if defined(TC2_WEBPAGE) || defined(CZ_WEBPAGE) || defined(CD_WEBPAGE)
extern void SetWanReloadFlag(boolean flag);
#endif
/*______________________________________________________________________________
**	ClearFlagsForDelayedProcess
**
**	description:
**	   This function is used to clear flags that are used to delay to operate 
**      according to the RPC of set. When many parameters are set together but 
**      one of them is not correct,any parameter including the delayed one  
**      can not be applied in this case,
**      
**	parameters:
**	return:
**____________________________________________________________________________*/
void ClearFlagsForDelayedProcess(void)
{
	uint32 i = 0;
	/*
		clear flags for these parameters which will break the connection between
		CPE and ACS
	*/
	
	/*
		clear flags for ATM LoopbackDiagnostics
	*/	
	rpc.flags.theLoopBackFlag &=~ CWMP_LOOPBACK_ACTIVE;
	

	/*
		clear flags for IPPing
	*/	
	rpc.flags.theIPPingFlag &=~CWMP_IPPING_ACTIVE;

	/*
		clear flags for IAD, mtk06404, 20140418
	*/	
	rpc.flags.theIADFlag &=~CWMP_IAD_ACTIVE;

	/*
		clear flags for DSL Diagnositcs
	*/	
	rpc.flags.theDSLLoopFlag &=~CWMP_DSLLOOP_ACTIVE;

	return;
}

//SetParameterValuesResponse.
int cwmpSetParameterValuesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	IntStructPtr tempResPara = NULL;
	int  *statusPtr = NULL;
	int err_status = CWMP_E_SUCCESS;
	
	AccessType accessType = Write;
	int isSimCardNodeModify = 0;

	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;

	if((err_status = cwmpConfigArgumentStruct("Status", IntType, 0, 0, responseArgumentPtr)) != 0){
		tc_printf("Configure  response argument fails for cwmpSetParameterValuesResponse !\r\n");
		return err_status;//replace InternalError by ResourceExceed,hqfeng,2006.06.21

	}

	responseArgumentPtr->next = NULL;

	if((responseArgumentPtr->value = (int*)os_malloc(sizeof(int))) == NULL){
		tc_printf("malloc status pointer fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	statusPtr = (int*)(responseArgumentPtr->value);

	err_status = cwmpProcessRPCResponse(requestArgumentPtr, accessType, responseArgumentPtr, statusPtr);

	
	tc_printf("err_status is %d \n",err_status);
	switch (err_status)
	{
		case CWMP_E_SUCCESS:
		case CWMP_E_DELAY:
			//processUpdateSimCard(pTcCfgNodes);
			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && isSimFileNodeModify(pTcCfgNodes)){
				isSimCardNodeModify = 1;
			}

			err_status = HandleParametersSetting( pTcCfgNodes, COMMIT, NODELAY );	

			if (TCSUPPORT_CT_PON_VAL && TCSUPPORT_CT_PON_CQ_VAL == 0 && itms_reboot_flag == 0)
				*statusPtr = 0;
			else
				*statusPtr = err_status;
			if(TCSUPPORT_CT_VAL){
				if(needReboot)
				{
					*statusPtr = err_status + 1;
					needReboot = 0;
				}
			}

			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && isSimCardNodeModify )
			{
				if ( 0 == *statusPtr || 1 == *statusPtr )
					*statusPtr += 2;
				else if ( 2 == *statusPtr )
					*statusPtr = 3;
			}

			break;
#if 0
		/*Add for fixing the problem:
		**The Wireless lan won't work if the configuration is not in a specific order.
		** by hqfeng,2006.10.12
		*/
#ifdef WLAN_RT2561_SUPPORT
			if(rpc.flags.theSettingFlag & CWMP_SET_WLAN){
				setWlanSSID();
				rpc.flags.theSettingFlag &= ~CWMP_SET_WLAN;
			}
#endif
#endif
		default:

			if(TCSUPPORT_CT_CWMP_WORK_COMPATIBLE_VAL)
			{
				char attrValue[8] = {0};
				int compatible = 0;
				int ret = 0;

				ret = tcapi_get("Cwmp_Entry", "workCompatible", attrValue);
				if(ret){
					compatible = 0;
				}
				else
				{
					compatible = atoi(attrValue);
				}		

				if(compatible == 1)
				{
					HandleParametersSetting( pTcCfgNodes, COMMIT, NODELAY );
				}
				else
				{
					ClearFlagsForDelayedProcess();
					HandleParametersSetting( pTcCfgNodes, READ, NODELAY );
					if(TCSUPPORT_CT_PON_GD_VAL)
						spSave();
				}
			}
			else
			{
			ClearFlagsForDelayedProcess();
			HandleParametersSetting( pTcCfgNodes, READ, NODELAY );
			if(TCSUPPORT_CT_PON_GD_VAL)
				spSave();
			}
			return err_status;
#if 0		
#ifdef WLAN_RT2561_SUPPORT		
			if(rpc.flags.theSettingFlag & CWMP_SET_WLAN){
  			       rpc.flags.theSettingFlag &= ~CWMP_SET_WLAN;
			}
#endif
#endif
	}
	SetCustomReponseValue(statusPtr);
	if(TCSUPPORT_CT_PON_GD_VAL)
		spSave();
				
	return 0;
}

//GetParameterValuesResponse.
int cwmpGetParameterValuesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	int err_status = CWMP_E_SUCCESS;
	
	AccessType accessType = Read;
	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;

	if((err_status = cwmpConfigArgumentStruct("ParameterList", ParameterValueStructType, 0, 1, responseArgumentPtr)) != 0){
		tc_printf("Configure  response argument fails for cwmpGetParameterValuesResponse !\r\n");
		return err_status;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	responseArgumentPtr->next = NULL;
	//argumentType = ParameterValueStruct;

	err_status = cwmpProcessRPCResponse(requestArgumentPtr, accessType, responseArgumentPtr, NULL);
	if( err_status != CWMP_E_SUCCESS){
		tc_printf("prcess getParameterValues response fails!\r\n");
		return err_status;
	}

	return CWMP_E_SUCCESS;
}

//GetParameterNameResponse
int cwmpGetParameterNamesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	int err_status = CWMP_E_SUCCESS;
	
	AccessType accessType = Read;
	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;

	if((err_status = cwmpConfigArgumentStruct("ParameterList", ParameterInfoStructType, 0, 1, responseArgumentPtr)) != 0){
		tc_printf("Configure  response argument fails for cwmpGetParameterValuesResponse !\r\n");
		return err_status;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	responseArgumentPtr->next = NULL;
	//argumentType = ParameterValueStruct;

	err_status = cwmpProcessRPCResponse(requestArgumentPtr, accessType, responseArgumentPtr, NULL);
	if( err_status != CWMP_E_SUCCESS){
		tc_printf("prcess rpc response fails!\r\n");
		return err_status;
	}

	return 0;
}

//added by xyzhu_nj_20091204
void attrCfgNode(int type)
{
	int fIndex = -1;
	int isSet = 0;
	attr_ptr pAttr = NULL;
	char fBuf[10] = {'\0'};
	int saveFlag = 0;

	for(pAttr = pTcAttrs; pAttr->attrName != NULL; pAttr++){
		fIndex = pAttr->fIndex;
		switch(fIndex){
			case ATTRFLAG0:
			#ifdef TR069DEBUG
				printf("fIndex is ATTRFLAG0.\n");
			#endif
				if(rpc.attrFlags.theSettingFlag0 & pAttr->fBit){
					rpc.attrFlags.theSettingFlag0 &= ~pAttr->fBit; 
					isSet = 1;
				}
				break;
			case ATTRFLAG1:
				if(rpc.attrFlags.theSettingFlag1 & pAttr->fBit){
					rpc.attrFlags.theSettingFlag1 &= ~pAttr->fBit;
					isSet = 1;
				}
				break;
			case ATTRFLAG2:
				if(rpc.attrFlags.theSettingFlag2 & pAttr->fBit){
					rpc.attrFlags.theSettingFlag2 &= ~pAttr->fBit;
					isSet = 1;
				}
				break;
			default:
				break;
		}
		if(isSet){
			if(type == READ){
				tc_printf("do ReadAttr().\n");
				tc_printf("pAttr->nodeName is %s\n", pAttr->attrName);
				if(TCSUPPORT_CT_VAL){
					ReadAttr_OP(pAttr, &saveFlag);
					if(saveFlag)
					{
						//tcapi_save();
						spSave();
					}
				}
				else
					ReadAttr_OP(pAttr, &saveFlag);
			}
			else if(type == SAVE){
				tc_printf("do SaveAttr().\n");
				tc_printf("pAttr->nodeName is %s\n", pAttr->attrName);
				SaveAttr(pAttr);
			}
			isSet = 0;
		}
	}
	//handle special attribute. modified by xyzhu_nj_20091210	
	tcapi_get("TR069Attr_SpeAttr", "flag", fBuf);
	if(!strcmp(fBuf, "1")){
		if(type == READ){
			tcapi_read("TR069Attr_SpeAttr");
		}
		else if(type == SAVE){
			tcapi_set("TR069Attr_SpeAttr", "flag", "0");
		}
	}
#if 0
	int i = 0;
	uint32 attrFlag = 0;
	attr_ptr pAttr = NULL;
	
	printf("coming into attrCfgNode.\n");
	for(i = 0; i < MAX_ATTRS_NUM; i++){
		printf("i is %d\n",i);
		pAttr = pTcAttrs + i; 
		if(pAttr == NULL || pAttr->attrName == NULL)
			break;
		attrFlag = (1 << i);
		if(rpc.attrFlags.theSettingFlag0 & attrFlag){
			printf("set attr is wrong.\n");
			if(type == READ){
				printf("do ReadAttr in attfCfgNode,and attr is %s\n",pAttr->attrName);
				ReadAttr(pAttr);
			}
			else if(type == SAVE){
				printf("do SaveAttr in attrCfgNode,and attr is %s\n",pAttr->attrName);
				SaveAttr(pAttr);
			}
			rpc.attrFlags.theSettingFlag0 &= ~attrFlag;
		}
	}
	if(i == MAX_ATTRS_NUM){
		for(i = 0; i < MAX_ATTRS_NUM; i++){
			pAttr = pTcAttrs + i + MAX_ATTRS_NUM; 
			if(pAttr == NULL || pAttr->attrName == NULL)
				break;	
			attrFlag = (1 << i);
			if(rpc.attrFlags.theSettingFlag1 & attrFlag){
				printf("set attr is wrong.\n");
				if(type == READ){
					ReadAttr(pAttr);
				}
				else if(type == SAVE){
					SaveAttr(pAttr);
				}
				rpc.attrFlags.theSettingFlag1 &= ~attrFlag;
			}
		}
	}
#endif
}
//SetParameterAttributesResponse
int cwmpSetParameterAttributesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	int err_status = CWMP_E_SUCCESS;
	
	AccessType accessType = Write;
	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr of SetParameterAttributes is NULL!\r\n");
		return -1;
	}
	
	//argumentType = ParameterValueStruct;
	err_status = cwmpProcessRPCResponse(requestArgumentPtr, accessType, NULL, NULL);
	if( err_status != CWMP_E_SUCCESS){
		tc_printf("prcess SetParameterAttributes Response fails!\r\n");
		cwmpSearchChangedValue(Passive);
		cwmpSearchChangedValue(Active);
		cwmpClearNotifyNodeList(Passive);
		cwmpClearNotifyNodeList(Active);
		//mark by brian
		//spReadL();
		//added by xyzhu_nj
		attrCfgNode(READ);
		//end by xyzhu_nj_20091204
		cwmpInitNotifyNodeList();
		
		return err_status;
	}

	attrCfgNode(SAVE);
	if(TCSUPPORT_CT_FJ_VAL || TCSUPPORT_CT_PON_GDV20_VAL)
		tr069SpSave();
	else
		spSave();//Added by hqfeng, 2006.04.21

	//added by xyzhu_nj_20091204
	/*if set attribute success.*/
	
	return 0;
}


//GetParameterAttributesResponse
int cmwpGetParameterAttributesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	int err_status = CWMP_E_SUCCESS;
	
	AccessType accessType = Read;
	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return -1;
	}
	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;

	if((err_status = cwmpConfigArgumentStruct("ParameterList", ParameterAttributeStructType, 0, 1, responseArgumentPtr)) != 0){
		tc_printf("Configure  response argument fails for cmwpGetParameterAttributesResponse !\r\n");
		return err_status;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	responseArgumentPtr->next = NULL;
	//argumentType = ParameterValueStruct;

	err_status = cwmpProcessRPCResponse(requestArgumentPtr, accessType, responseArgumentPtr, NULL);
	if( err_status != CWMP_E_SUCCESS){
		tc_printf("prcess rpc response fails!\r\n");
		return err_status;
	}

	return CWMP_E_SUCCESS;
}

//AddObjectResponse
int cwmpAddObjectResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	RPCArgumentPtr statusArgumentPtr;
	int  *statusPtr = NULL;
	int  *instanceNumberPtr = NULL;
	AccessType accessType = Add;
	uint32 instanceNumber;
	int err_state = CWMP_E_SUCCESS;
	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;
	
	if((err_state = cwmpConfigArgumentStruct("InstanceNumber", UnsignedIntType, 0, 0, responseArgumentPtr)) != 0){
		tc_printf("Configure  response InstanceNumber argument fails for cwmpAddObjectResponse !\r\n");
		return err_state;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	
	if((statusArgumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}
	memset(statusArgumentPtr, 0, sizeof(RPCArgument));

	responseArgumentPtr->next = statusArgumentPtr;

	if((err_state = cwmpConfigArgumentStruct("Status", IntType, 0, 0, statusArgumentPtr)) != 0){
		tc_printf("Configure  response status argument fails for cwmpAddObjectResponse !\r\n");
		return err_state;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	statusArgumentPtr->next = NULL;
	
       if((statusPtr = (int*)os_malloc(sizeof(int))) == NULL){
		tc_printf("malloc status pointer fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}
//	*statusPtr = 0; 
       if((instanceNumberPtr = (int*)os_malloc(sizeof(uint32))) == NULL){
		tc_printf("malloc status pointer fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}
	   
	instanceNumber = cwmpProcessRPCResponse(requestArgumentPtr, accessType, responseArgumentPtr, statusPtr );
	if(instanceNumber >= 9000){//Build AddObject response error. 
		err_state = instanceNumber;
		goto Error_Handler;
	}
	*instanceNumberPtr = instanceNumber;	
	responseArgumentPtr->value = instanceNumberPtr;
	statusArgumentPtr->value = statusPtr;
	
	spSave();
	return 0;
	
Error_Handler:


	if(instanceNumberPtr){
		os_free(instanceNumberPtr);
		instanceNumberPtr = NULL;
	}
	if(statusPtr){
		os_free(statusPtr);
		statusPtr = NULL;
	}
#if 0 //xzwang_060624	
	if(statusArgumentPtr){
		os_free(statusArgumentPtr);
		statusArgumentPtr = NULL;
	}
	if(responseBodyStructPtr->argumentPtr){
		os_free(responseBodyStructPtr->argumentPtr);
		responseBodyStructPtr->argumentPtr = NULL;
	}
#endif		
	return err_state;
}

//DeleteObjectResponse
int cwmpDeleteObjectResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	RPCArgumentPtr statusArgumentPtr;
	int  *statusPtr = NULL;
	int  *instanceNumberPtr = NULL;
	AccessType accessType = Delete;
	uint32 instanceNumber;
	int err_state = CWMP_E_SUCCESS;
		
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if((responseBodyStructPtr->argumentPtr = (RPCArgumentPtr)os_malloc(sizeof(RPCArgument))) == NULL){
		tc_printf("malloc response argument fails!\r\n");
		return -1;
	}

	memset(responseBodyStructPtr->argumentPtr, 0, sizeof(RPCArgument));
	responseArgumentPtr = responseBodyStructPtr->argumentPtr;

	if((err_state = cwmpConfigArgumentStruct("Status", IntType, 0, 0, responseArgumentPtr)) != 0){
		tc_printf("Configure  response status argument fails for cwmpDeleteObjectResponse !\r\n");
		return err_state;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	responseArgumentPtr->next = NULL;
       if((statusPtr = (int*)os_malloc(sizeof(int))) == NULL){
		tc_printf("malloc status pointer fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}
	   
	err_state = cwmpProcessRPCResponse(requestArgumentPtr, accessType, responseArgumentPtr, statusPtr );
	if(err_state != CWMP_E_SUCCESS){
		goto Error_Handler;	
	}
	if(TCSUPPORT_CUC_CWMP_PARAMETER_VAL){
		tc_printf("HandleParametersSetting( pTcCfgNodes,COMMIT,DELAY)");
		HandleParametersSetting( pTcCfgNodes, COMMIT, DELAY );
	}
	responseArgumentPtr->value = statusPtr;

	spSave();
	return 0;
	
Error_Handler:

	if(statusPtr){
		os_free(statusPtr);
		statusPtr = NULL;
	}
#if 0	//xzwang_060624
	if(responseBodyStructPtr->argumentPtr){
		os_free(responseBodyStructPtr->argumentPtr);
		responseBodyStructPtr->argumentPtr = NULL;
	}
#endif	
	return err_state;
}

int cwmpConfigArgumentStruct(char* name, ParameterType paraType, int number, uint8 beArray, RPCArgumentPtr argumentPtr){

	if(argumentPtr == NULL){
		tc_printf("The argument pointer for configuration is NULL!\r\n");
		return -1;
	}
	if((argumentPtr->name = (char*)os_malloc(strlen(name)+1)) != NULL)   //xzwang_060624
		strcpy(argumentPtr->name,name);
	else{
		tc_printf("malloc argument  name fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	argumentPtr->type = paraType;
	argumentPtr->number = number;
	argumentPtr->beArray = beArray;

	return 0;
}

int getUpgradeDelaySwitchFlag()
{
	char buf[8] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;	
	tcapi_get("Cwmp_Entry","FwUpgradeDelay",buf);
	if (atoi(buf) == 1){
		return 1;
	}else{
		return 0;
	}
}

int getUpgradeDelayConfigFlag()
{
	char buf[8] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;	
	tcapi_get("Cwmp_Entry","FwUpgradeCfg",buf);
	return atoi(buf);
}

int getUpgradeRXCellCounter()
{
	char buf[8] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;	
	tcapi_get("Cwmp_Entry","FwUpgradeRxCnt",buf);
	if (strlen(buf) == 0){
		return 50;
	}
	return atoi(buf);
}

/*
 * name: isBridgePvc
 * input: pvc
 * output: 1(is),0(is not)
 * discription: to judge whether the specified pvc is ACTIVE and BRIDGE mode.
 * */
static int isBridgePvc(int pvc)
{
	char strBuf[64] = {0};
	char ifNodeName[64] = {0};
	
	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;
	sprintf(ifNodeName, "Wan_PVC%d", pvc);
	tcapi_get(ifNodeName,"Active",strBuf);
	if (strcmp(strBuf,"Yes") == 0){
		tcapi_get(ifNodeName,"ISP",strBuf);
		if (strcmp(strBuf,WAN_ENCAP_BRIDGE) == 0){
			return 1;
		}
	}
	return 0;
}

/*
 * name: getVpi 
 * input: pvc
 * output: vpi
 * discription: get vpi of specific pvc
 * */
static int getVpi(int pvc)
{
	char strBuf[64] = {0};
	char ifName[64] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;	
	sprintf(ifName, "Wan_PVC%d", pvc);
	tcapi_get(ifName,"VPI",strBuf);
	return atoi(strBuf);
}

/*
 * name: getVci
 * input: pvc
 * output: vci
 * discription: get vci of specific pvc
 * */
static int getVci(int pvc)
{
	char strBuf[64] = {0};
	char ifName[64] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;	
	sprintf(ifName, "Wan_PVC%d", pvc);
	tcapi_get(ifName,"VCI",strBuf);
	return atoi(strBuf);
}

/*
 * name: initPvcTbl
 * input: an 8*3 matrix which is used to contain pvc,vpi,vci
 * output: none
 * discription: fill the 8*3 matrix with all the bridge pvc's pvc index, vpi, vci
 * */
static void initPvcTbl(int (*PvcTbl)[3], int size)
{
	int pvcIndex;

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return;
	for (pvcIndex = 0; pvcIndex < size; pvcIndex++){
		if (isBridgePvc(pvcIndex)){
			(*PvcTbl)[0] = pvcIndex;
			(*PvcTbl)[1] = getVpi(pvcIndex);
			(*PvcTbl)[2] = getVci(pvcIndex);
			PvcTbl++;
		}
	}
}

/*
 * name: initVcTbl
 * input: an 8*3 matrix which is used to contain vc,vpi,vci
 * output: none
 * discription: fill the 8*3 matrix with all the vc's vc index, vpi, vci
 * */
static void initVcTbl(int (*VcTbl)[3], int size)
{
	int vcIndex;
	FILE *fd = NULL;
	char *rcvData_p = NULL;
	int rcvData;
	int vpi;
	int vci;
	char lineBuf[128] = {0};
	char ifName[64] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return;
	for (vcIndex = 0; vcIndex < size; vcIndex++){
		sprintf(ifName,"/proc/tc3162/tsarm_vc%d",vcIndex);
		fd = fopen(ifName,"r");
		if (fd == NULL){
			continue;
		}
		while (fgets(lineBuf,sizeof(lineBuf),fd) != NULL){
			if ((rcvData_p = strstr(lineBuf,"VC Configuration reg")) != NULL){
				rcvData_p = strstr(rcvData_p,"=");
				sscanf(rcvData_p+2,"%x",&rcvData);
				if (rcvData != 0){
					(*VcTbl)[0] = vcIndex;
					/* 
					 * +----+---------+----------------+----+
					 * |4bit|vpi(8bit)|  vci (16 bit)  |4bit|
					 * +----+---------+----------------+----+
					 */
					vpi = (rcvData << 4) >> 24;
					vci = (rcvData << 12) >> 16;
					(*VcTbl)[0] = vcIndex;
					(*VcTbl)[1] = vpi;
					(*VcTbl)[2] = vci;
					VcTbl++;
				}
			}
		}
		fclose(fd);
	}
}
/*
 * this function is to check all the active route PVC, whether there is live NAT session
 * If there is no live NAT session in any route PVC, it return 1;
 * else, it return 0;
 * add for firmware upgrade delay, Taoliu_20090119
*/
int isNoLiveNATSession(void)
{
	FILE *fd = NULL;
	char natEntry[256] = {0};
	char timeBuf[16] = {0};
	char code[16] = {0};
	char *protocol_p = NULL;
	int timeLive;
	static int tcpTimeout = 0;
	static int udpTimeout = 0;
	static int udpStreamTimeout = 0;
	int timeout = 0;
	char state[16] = {0};
	char ipBuf[16] = {0};
	int sPort;
	int dPort;

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;
	if (tcpTimeout == 0){
		fd = fopen("/proc/sys/net/netfilter/nf_conntrack_tcp_timeout_established","r");
		if (fd == NULL || fgets(timeBuf,sizeof(timeBuf),fd) == NULL){
			tcpTimeout = 432000;
		}else{
			tcpTimeout = atoi(timeBuf);
		}
		fclose(fd);
		if(!TCSUPPORT_BHARTI_VAL){
			fd = fopen("/proc/sys/net/netfilter/nf_conntrack_udp_timeout","r");
			if (fd == NULL || fgets(timeBuf,sizeof(timeBuf),fd) == NULL){
				udpTimeout = 30;
			}else{
				udpTimeout = atoi(timeBuf);
			}
			fclose(fd);
			fd = fopen("/proc/sys/net/netfilter/nf_conntrack_udp_timeout_stream","r");
			if (fd == NULL || fgets(timeBuf,sizeof(timeBuf),fd) == NULL){
				udpStreamTimeout = 180;
			}else{
				udpStreamTimeout = atoi(timeBuf);
			}
			fclose(fd);
		}
	}
	fd = fopen("/proc/net/nf_conntrack","r");
	if (fd == NULL){
		tcdbg_printf("\t\n isNoLiveNATSession: open nf_conntrack fail");
		return 1;
	}
	while(fgets(natEntry,sizeof(natEntry),fd) != NULL){
		if(!TCSUPPORT_BHARTI_VAL){
			if ((protocol_p = strstr(natEntry,"tcp")) == NULL && (protocol_p = strstr(natEntry,"udp")) == NULL){
				continue;
			}
			sscanf(protocol_p+3,"%d %d",&code,&timeLive);
			if (strncmp(protocol_p,"tcp",3) == 0){
				if (strstr(protocol_p,"ESTABLISHED") == NULL){
					continue;
				}
				timeout = tcpTimeout; 
			}else{
				if (strstr(protocol_p,"UNREPLIED")){
					timeout = udpTimeout;
				}else{
					timeout = udpStreamTimeout;
				}
			}
			if (timeout - timeLive < 30){
				fclose(fd);
				return 0;
			}
		}
		else{
			if ((protocol_p = strstr(natEntry,"tcp")) == NULL)
				continue;
			sscanf(protocol_p+3,"%d %d %s src=%s dst=%s sport=%d dport=%d",&code,&timeLive,state,ipBuf,ipBuf,&sPort,&dPort);
			if (strcmp(state,"ESTABLISHED") != 0){
				continue;
			}
			if (tcpTimeout - timeLive >= 30){
				continue;
			}
			if (sPort == 80 || sPort == 20 || sPort == 21 || sPort == 443
				|| dPort == 80 || dPort == 20 || dPort == 21 || dPort == 443){
				fclose(fd);
				return 0;
			}
		}
		memset(natEntry,0,sizeof(natEntry));
	}
	fclose(fd);
	return 1;
}

/*
 * this function is to check all the active bridge PVC, whether there is IPTV data.
 * If there is no IPTV data in any bridge PVC, it return 1;
 * else, it return 0;
 *
 * IPTV is broadcast packets, and also there are many other broadcase packets on the internet.
 * at present, we considerate that there is no IPTV data when the received data rate is less than a certain value.
 * Considerating the minimal rate Stream Media, 100kb/s, MTU is 1492, remove the header, the overload is about 1450,
 * in 10 seconds, there are ((10second * 100Kb/s  * 1024/k)  / (8bits/Byte * 1450Bytes/cell))=88cells,
 * that is say, at least 88 cells per 10 seconds the modem received when LAN user watch IPTV program.
 * And in our suzhou office enviroment, the bridged PVC will receive about 10 broadcast cells  in 10 seconds.
 * So, when the bridge PVC received less than upgradeRXCellCounter  cells in 10 seconds, we considerate that 
 * there is no IPTV data in this PVC.
 * The default value of upgradeRXCellCounter is 50, we can change it via ci-command "sys cwmp upgradedelay rxcellcounter"
 * 
 *
 * add for firmware upgrade delay, Taoliu_20090119
*/
int isNoIPTVData(void)
{
	int pvc = 0;
	int vc = 0;
	int i = 0;
	static int firstFlag = 1;
	static int PvcToVc[8][2];
	int PvcVpiVci[8][3];
	int VcVpiVci[8][3];
	char *rcvData_p = NULL;
	int rcvData = 0;
	FILE *fd = NULL;
	char lineBuf[128] = {0};
	char ifName[64] = {0};

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;
	if (firstFlag == 1){
		/* We do two things the first time coming here:
		 * 1 determine the relationship between active bridge PVC and VC
		 * 2 clear the current value of RX Total Data Count*/
		memset(PvcToVc,0,sizeof(PvcToVc));
		/*init the first rank as "-1" to indicate whether a row is valid*/
		for (i = 0; i < 8; i++){
			PvcToVc[i][0] = -1;
		}
		memset(PvcVpiVci,0,sizeof(PvcVpiVci));
		memset(VcVpiVci,0,sizeof(VcVpiVci));
		/* 1st thing
		 * build two tables: one for bridge PVC and its vpi,vci; the other for VC and
		 * its vpi,vci;
		 * then use these two tables to establish the relationship between PVC and VC.
		 **/
		initPvcTbl(PvcVpiVci,8);
		initVcTbl(VcVpiVci,8);
		i = 0;
		while(PvcVpiVci[pvc][2] != 0 && pvc < 8){ /*since vci can not be 0, use it to indicate the end*/
			vc = 0;
			while(VcVpiVci[vc][2] != 0 && vc < 8){
				if (PvcVpiVci[pvc][1] == VcVpiVci[vc][1] && PvcVpiVci[pvc][2] == VcVpiVci[vc][2]){
					PvcToVc[i][0] = PvcVpiVci[pvc][0];
					PvcToVc[i][1] = VcVpiVci[vc][0];
					i++;
					break;
				}
				vc++;
			}
			pvc++;
		}
		/* 2nd thing
		 * we can only get a random number of RX Total Data Count 
		 * since the number is peroidicly overflowed as it ascends, but when we access 
		 * it once, it will be reset to 0. So we access it the second time to get the 
		 * MAYBE(it is still RARELY possible to be overflow) right number
		 * In doing the 1st thing, we have already access it when accessing tsarm_vc#i, 
		 * so the 2nd thing is already done*/
		firstFlag = 0;
		return 0;
	}/*firstFlag == 1*/
		
	while (PvcToVc[i][0] != -1){
		vc = PvcToVc[i++][1];
		sprintf(ifName,"/proc/tc3162/tsarm_vc%d",vc);
		fd = fopen(ifName,"r");
		if (fd == NULL){
			continue;
		}
		while (fgets(lineBuf,sizeof(lineBuf),fd) != NULL){
			if ((rcvData_p = strstr(lineBuf,"RX Total Data Count")) != NULL){
				rcvData_p = strstr(rcvData_p,"=");
				sscanf(rcvData_p+2,"%x",&rcvData);
				if (rcvData >= getUpgradeRXCellCounter()){
					tcdbg_printf("\r\n There is IPTV data in pvc");
					fclose(fd);
					return 0;
				}
				break;
			}
		}
		fclose(fd);
	}
	return 1;
}

extern int RemoveTimer_tmp(int timer_id);
int checkUpgradeDelay(void)
{
	int cfgFlag = getUpgradeDelayConfigFlag();

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;
	if(cfgFlag == 1){ /*nat only*/
		tcdbg_printf("\r\n firmware upgrade delay config is nat only");
		if(isNoLiveNATSession() == 0){ /*has live nat session*/
			tcdbg_printf("\r\n There are still live nat sessions");
			return 1;
		}
	}
	else if(cfgFlag == 2){ /*nat and bridge*/
		tcdbg_printf("\r\n firmware upgrade delay config is nat and IPTV");
		if(isNoLiveNATSession() == 0 || isNoIPTVData() == 0){
			return 1;
		}
	}
	else{
		tcdbg_printf("\r\n invalid value: upgradedelayconfigflag");
	}
	
//	timerPeriodicStop(upgradeDelayTimerID);
	RemoveTimer_tmp(upgradeDelayTimerID);
	cwmpPerformFileTransfer();
	return 0;
}

int checkUpgradeDelayEntry(uint32 delaySecond)
{
	uint16 checkInterval = 5;

	if(!TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		return 0;
	if(getUpgradeDelaySwitchFlag() == 0){ /*firmware upgrade delay is disable*/
		tcdbg_printf("\r\n firmware upgrade delay is disable");
		timerStartS( delaySecond*1000, (TimerCallback)cwmpPerformFileTransfer, NULL );
		return 0;
	}
	tcdbg_printf("\r\n firmware upgrade delay is enable, =>timerPeriodicStartS checkUpgradeDelay");
	upgradeDelayTimerID = timerPeriodicStartS( checkInterval*1000, (TimerCallback)checkUpgradeDelay, NULL );
	return 0;
}

int/*8*/ cwmpDownloadResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	uint32	   DelaySecond;
	RPCArgumentPtr  requestArgumentPtr = NULL; 
	uint8 n;
	int/*8*/ ret;
	char * acsUrl = NULL;
//	boolean dd = FALSE; //delay download //removed by hqfeng,2006.12.08
	FileTransferStructPtr dsp = NULL;
	/*TCSUPPORT_PON_ROSTELECOM begin*/
	char readbuf[64] = {'\0'};
	/*TCSUPPORT_PON_ROSTELECOM end*/

	if(downloadInProgress == 1){
		return 0;
	}
	if(dbg_cwmp >= DBG_CWMP_L1){
		tc_printf("\r\n => cwmpDownloadResponse() ");	
	}

	requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	if(requestArgumentPtr == NULL){
		tc_printf("\r\n requestArgumentPtr is NULL!");
		return -1;
	}

	dsp = rpc.fileTransferStruct;
	dsp->transferType = Download;
	ret = cwmpGetFileTransferArgument(requestArgumentPtr);
	if(TCSUPPORT_CROSS_REBOOT_VAL){
		setDownloadStartFlag(1);
		setDownloadContinueFlag(0);
	}
	if(ret != 0){
		tc_printf("\r\n cwmpGetFileTransferArgument error! ");
		return -1;
	}

	if(dsp->DelaySeconds  == 0){
//		dd = FALSE;  // no delay
		DelaySecond = 1;
	}else{
//		dd = TRUE;
		DelaySecond = dsp->DelaySeconds;
	}

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n the delay seconds is:%ld", dsp->DelaySeconds);
	}
	
#if 0
	if( dd == FALSE){ // no delay
		SetFileTransferTypeFlag(1);
		cwmpBuildFileTransferResponse(responseBodyStructPtr, 1);
		DelaySecond = 1;
		timerStartS( DelaySecond*1000, cwmpPerformFileTransfer, NULL );
		//cwmpPerformFileTransfer();
		//cwmpBuildFileTransferResponse(responseBodyStructPtr, 0);
		//cwmpResetRpcResponseFlag(&rpc);

	}else{ //delay
		SetFileTransferTypeFlag(1);
		cwmpBuildFileTransferResponse(responseBodyStructPtr, 1);
		/* close current session */
		//setNoMoreRequestFlag(1);
		/* delayed-download */
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			dbg_plinel_1("\r\n second the delay seconds is:", dsp->DelaySeconds);
		}
		timerStartS( dsp->DelaySeconds*1000, cwmpPerformFileTransfer, NULL );
	}
#endif
	SetFileTransferTypeFlag(1);
	downloadInProgress = 1;
	cwmpBuildFileTransferResponse(responseBodyStructPtr, 1);

	if(TCSUPPORT_FW_UPGRADE_DELAY_VAL)
		checkUpgradeDelayEntry(DelaySecond);
	else
	{
		if( (!TCSUPPORT_PON_ROSTELECOM_VAL) || ( (Download == rpc.fileTransferStruct->transferType) && (CWMP_FIRMWARE != cwmpGetDownloadFileType()) ) )
		{
			timerStartS( DelaySecond*1000, (TimerCallback)cwmpPerformFileTransfer, NULL );
		}
		else/*only download FW ,use wget*/
		{
			if( tcapi_get("Cwmp_Entry", "DownloadTool", readbuf) == 0 && atoi(readbuf) == 1 ) /*use wget to download fw*/
				timerStartS( DelaySecond*1000, (TimerCallback)cwmpPerformWgetFirmwareDownloadTask, NULL );   /*download fw by wget*/
			else
				timerStartS( DelaySecond*1000, (TimerCallback)cwmpPerformFileTransfer, NULL );
		}
	}

	return 0;

}

uint8 DelayFileTransferFlag = 0;
uint32 UploadDelaySecond = 0;
int cwmpUploadResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	uint32	   DelaySecond;
	RPCArgumentPtr  requestArgumentPtr = NULL; 
	uint8 n;
	int/*8*/ ret;
	char * acsUrl = NULL;
//	boolean dd = FALSE; //delay download
	FileTransferStructPtr dsp = NULL;
		
	if(dbg_cwmp >= DBG_CWMP_L1 ){
		tc_printf("\r\n => cwmpUploadResponse()");	
	}
	
	requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	if(requestArgumentPtr == NULL){
		tc_printf("\r\n requestArgumentPtr is NULL!");
		return -1;
	}

	dsp = rpc.fileTransferStruct;
	dsp->transferType = Upload;
	ret = cwmpGetFileTransferArgument(requestArgumentPtr);
	if(ret != 0){
		tc_printf("\r\n cwmpGetFileTransferArgument error! ");
		return -1;
	}

	if(dsp->DelaySeconds  == 0){
//		dd = FALSE;  // no delay
		DelaySecond = 1;
	}else{
//		dd = TRUE;
		DelaySecond = dsp->DelaySeconds;
	}

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n the delay seconds is:%ld", dsp->DelaySeconds);
	}
#if 0
	if( dd == FALSE){ // no delay
		SetFileTransferTypeFlag(1);
		cwmpBuildFileTransferResponse(responseBodyStructPtr, 1);
		DelaySecond = 1;
		timerStartS( DelaySecond*1000, cwmpPerformFileTransfer, NULL );
		//cwmpPerformFileTransfer();
		//cwmpBuildFileTransferResponse(responseBodyStructPtr, 0);
		//cwmpResetRpcResponseFlag(&rpc);

	}else{ //delay
		SetFileTransferTypeFlag(1);
		cwmpBuildFileTransferResponse(responseBodyStructPtr, 1);
		/* close current session */
		//setNoMoreRequestFlag(1);
		/* delayed-download */
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			dbg_plinel_1("\r\n second the delay seconds is:", dsp->DelaySeconds);
		}
		timerStartS( dsp->DelaySeconds*1000, cwmpPerformFileTransfer, NULL );
	}
#endif
	SetFileTransferTypeFlag(1);
	cwmpBuildFileTransferResponse(responseBodyStructPtr, 1);
	if(!TCSUPPORT_CMCC_GANSU_VAL){
	timerStartS( DelaySecond*1000, (TimerCallback)cwmpPerformFileTransfer, NULL );
	}
	else{
		DelayFileTransferFlag=1;
		UploadDelaySecond = DelaySecond;
	}
	return 0;

}


int/*8*/ cwmpGetFileTransferArgument(RPCArgumentPtr arg)
{
	int strlength;
	RPCArgumentPtr p=arg;
	char * str;
	FileTransferStructPtr dsp;
	int rtnCode = 0;
	int cmdKeyIndex;
	char temp[64] = {0};
	time_t startTime;
		
	if(TCSUPPORT_CROSS_REBOOT_VAL){
		time(&startTime);
		sprintf(temp, "%d", startTime);
		tc_printf("\r\n startTime = %d", startTime);
		tcapi_set("Cwmp_Entry", "DownloadStartTime", temp);
	}
	dsp = rpc.fileTransferStruct;
	if( dsp == NULL){
		tc_printf("\r\n FileTransferStruct == NULL!");
		return -1;
	}

	if(rpc.fileTransferStruct->transferType == Download){
		cmdKeyIndex = M_Download;
      	}
	else if(rpc.fileTransferStruct->transferType == Upload){
		cmdKeyIndex = M_Upload;
	}
	else{
		tc_printf("\r\n[cwmpGetFileTransferArgument]File transfer type  is unknow!");
		return -1;
	}
	
	while(p != NULL){
		if( strcmp(p->name, "CommandKey") ==0 /*&& p->value != NULL*/){
			//Modified for empty string,by hqfeng,2007.01.09
			if(p->value == NULL){
				rtnCode = SetCommandKey("", cmdKeyIndex);
				if(TCSUPPORT_CROSS_REBOOT_VAL)
					tcapi_set("Cwmp_Entry", "Cmdkey", "");
			}else{
				strlength = strlen((char *)p->value);
				if( (strlength + 1) > dsp->sCmdKey ){ 
					dsp->CmdKey = (char *)os_str_realloc(dsp->CmdKey, strlength+1, dsp->sCmdKey);		
					dsp->sCmdKey = strlength + 1;
				}
				if( dsp->CmdKey == NULL)
					goto error;
				strcpy(dsp->CmdKey, (char*)p->value);
				rtnCode = SetCommandKey(dsp->CmdKey, cmdKeyIndex);   //xzwang_060703
				if(TCSUPPORT_CROSS_REBOOT_VAL)
					tcapi_set("Cwmp_Entry", "Cmdkey", dsp->CmdKey);
			}

			if(rtnCode != 0){
				return InvalidArgument;
			}
			
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n cmdkey = %s",dsp->CmdKey);
			}
		}
		else if( strcmp(p->name, "FileType") ==0 && p->value != NULL){
			strlength = strlen((char*)p->value);	
			if( (strlength + 1) > dsp->sFileType){
				dsp->FileType = (char *)os_str_realloc(dsp->FileType, strlength+1, dsp->sFileType);
				dsp->sFileType = strlength + 1;
			}
			if(dsp->FileType== NULL)
				goto error;
			strcpy(dsp->FileType, (char*)p->value);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n FileType = %s",dsp->FileType);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadFileType", dsp->FileType);
		}
		else if( strcmp(p->name, "URL") ==0 && p->value != NULL){
			strlength = strlen((char*)p->value) ;	
			if( (strlength + 1) > dsp->sURL){
				dsp->URL = (char *)os_str_realloc(dsp->URL, strlength+1, dsp->sURL);
				dsp->sURL = strlength + 1;
			}
			if(dsp->URL  == NULL)
				goto error;
			strcpy(dsp->URL, (char*)p->value);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n URL = %s",dsp->URL);
			}
			if(TCSUPPORT_UPNP_ENHANCE_VAL){
				if((cmdKeyIndex == M_Download)&&((strstr(dsp->URL,"ftp")!=NULL) ||(strstr(dsp->URL,"FTP")!=NULL) ||(strstr(dsp->URL,"Ftp")!=NULL)))
				{
					tc_printf("\r\n[cwmpGetFileTransferArgument]Not Support FTP UP!");
					return -1;
				}
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadURL", dsp->URL);
		}
		else if( strcmp(p->name, "Username") ==0 && p->value != NULL){
			strlength = strlen((char*)p->value);	
			if( (strlength + 1) > dsp->sUsername){
				dsp->Username= (char *)os_str_realloc(dsp->Username, strlength+1, dsp->sUsername);
				dsp->sUsername = strlength + 1;
			}
			if(dsp->Username== NULL)
				goto error;
			strcpy(dsp->Username, (char*)p->value);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n username = %s",dsp->Username);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadUserName", dsp->Username);
		}
		else if( strcmp(p->name, "Password") ==0 && p->value != NULL){
			strlength = strlen((char*)p->value);	
			if( (strlength + 1) > dsp->sPassword){
				dsp->Password= (char *)os_str_realloc(dsp->Password, strlength+1, dsp->sPassword);
				dsp->sPassword = strlength + 1;
			}
			if(dsp->Password== NULL)
				goto error;
			strcpy(dsp->Password, (char*)p->value);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n passwd = %s",dsp->Password);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadPassword", dsp->Password);
		}
		else if( strcmp(p->name, "FileSize") ==0 && p->value != NULL){
			str = (char*)p->value;
			dsp->FileSize = (unsigned int)atoi(str);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n FileSize = %ld", dsp->FileSize);	
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL){
				sprintf(temp, "%ld", dsp->FileSize);
				tcapi_set("Cwmp_Entry", "DownloadFileSize", str);
			}
		}
//		else if( strcmp(p->name, "TargetFileName") ==0 && p->value != NULL){
		else if( strcmp(p->name, "TargetFileName") ==0){
			if( p->value != NULL){
			strlength = strlen((char*)p->value);	
			if( (strlength + 1) > dsp->sTargetFileName){
				dsp->TargetFileName = (char *)os_str_realloc(dsp->TargetFileName, strlength+1, dsp->sTargetFileName);
				dsp->sTargetFileName = strlength + 1;
			}				
			if(dsp->TargetFileName == NULL)
				goto error;
			strcpy(dsp->TargetFileName, (char*)p->value);
			}else{
				strcpy(dsp->TargetFileName,"");
			}
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n TargetFileName = %s",dsp->TargetFileName);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadTargetFileName", dsp->TargetFileName);
		}
		else if( strcmp(p->name, "DelaySeconds") ==0 && p->value != NULL){
			str = (char*)p->value;
			dsp->DelaySeconds = (unsigned int)atoi(str);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n DelaySeconds = %ld", dsp->DelaySeconds);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL){
				sprintf(temp, "%ld", dsp->DelaySeconds);
				tcapi_set("Cwmp_Entry", "DownloadDelaySeconds", str);
			}
		}
		else if( strcmp(p->name, "SuccessURL") ==0 && p->value != NULL){
			strlength = strlen((char*)p->value);	
			if( (strlength + 1) > dsp->sSuccessURL){
				dsp->SuccessURL = (char *)os_str_realloc(dsp->SuccessURL, strlength+1, dsp->sSuccessURL);
				dsp->sSuccessURL = strlength + 1;
			}				
			if(dsp->SuccessURL == NULL)
				goto error;
			strcpy(dsp->SuccessURL, (char*)p->value);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n SuccessURL = %s",dsp->SuccessURL);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadSuccessURL", dsp->SuccessURL);
		}
		else if( strcmp(p->name, "FailureURL") ==0 && p->value != NULL){
			strlength = strlen((char*)p->value);	
			if( (strlength + 1) > dsp->sFailureURL){
				dsp->FailureURL = (char *)os_str_realloc(dsp->FailureURL, strlength+1, dsp->sFailureURL);
				dsp->sFailureURL = strlength + 1;
			}				
			if(dsp->FailureURL == NULL)
				goto error;
			strcpy(dsp->FailureURL, (char*)p->value);
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n FailureURL = %s",dsp->FailureURL);
			}
			if(TCSUPPORT_CROSS_REBOOT_VAL)
				tcapi_set("Cwmp_Entry", "DownloadFailureURL", dsp->FailureURL);
		}
		p = p->next;		
	}
	if(TCSUPPORT_CROSS_REBOOT_VAL)
		tcapi_commit("Cwmp_Entry");
	
	return 0;
error:
	tc_printf("\r\n cwmpGetFileTransferArgument: realloc fail.");
	return -1;
}


int8 cwmpBuildFileTransferResponse(SoapBodyInfoPtr BodyStructPtr, int status)
{
	RPCArgumentPtr curArgPtr = NULL, tempArgPtr = NULL;
	char * CmdKeyPtr = NULL;
	EventStructPtr     eventPtr;
	ParameterValueStructPtr paraValuePtr = NULL;
	int8 flag;
	uint32 time;
	int *value = NULL;

	if(dbg_cwmp >= DBG_CWMP_L1 ){
		tc_printf("\r\n => cwmpBuildFileTransferResponse()");
	}

	if(BodyStructPtr == NULL){
		tc_printf("\r\n File transfer  body struct is NULL!");
		return -1;
	}
      // rpc name
      if(rpc.fileTransferStruct->transferType == Download){
		strcpy(BodyStructPtr->rpcName, "DownloadResponse");
      	}
	else if(rpc.fileTransferStruct->transferType == Upload){
		strcpy(BodyStructPtr->rpcName, "UploadResponse");
	}
	else{
		tc_printf("\r\n File transfer type  is unknow!");
		return -1;
	}

     //Argument.
	if(cwmpInitArgument("Status", 1, IntType, 0, &tempArgPtr) == -1){
		tc_printf("\r\n Initiate DownloadResponse argument CommandKey fails!");
		return -1;
	}
	curArgPtr = tempArgPtr;
	BodyStructPtr->argumentPtr = tempArgPtr;

	if((value = (int*)os_malloc(sizeof(int))) == NULL){
		tc_printf("\r\n malloc  value fails!");
		return -1;
	}

	*value = status;
	tempArgPtr->value = value;
	
//	curArgPtr = tempArgPtr;
//	BodyStructPtr->argumentPtr = tempArgPtr;
	
	//StartTime
	if(cwmpInitArgument("StartTime", 1, DateTimeType, 0,  &tempArgPtr) == -1){
		tc_printf("\r\n Initiate DownloadResponse argument fails!");
		return -1;
	}
	curArgPtr->next = tempArgPtr;

	if( status){
		if((tempArgPtr->value = (char*)os_malloc(64)) == NULL){
			tc_printf("\r\n malloc  value fails!");
			return -1;
		}
		memset(tempArgPtr->value, 0, 64);
		/* set unknown time value*/
		strcpy(tempArgPtr->value,"0001-01-01T00:00:00Z");	
	}
	else{
		if((tempArgPtr->value = (char*)os_malloc(64)) == NULL){
			tc_printf("\r\n malloc  value fails!");
			return -1;
		}
		memset(tempArgPtr->value, 0, 64);
		/* get download start time */
		time = GetFileTransferStartTime();
		if(TCSUPPORT_CWMP_PRECISE_TIME_VAL)
			strcpy(tempArgPtr->value,TimeFormatConvert_improve(time));	
		else
			strcpy(tempArgPtr->value,TimeFormatConvert(time));	
	}
//	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;

	//CompleteTime
	if(cwmpInitArgument("CompleteTime", 1, DateTimeType, 0,  &tempArgPtr) == -1){
		tc_printf("\r\n Initiate DownloadResponse argument fails!");
		return -1;
	}
	curArgPtr->next = tempArgPtr;

	if( status){
		if((tempArgPtr->value = (char*)os_malloc(64)) == NULL){
			tc_printf("\r\n malloc  value fails!");
			return -1;
		}
		memset(tempArgPtr->value, 0, 64);
		/* set unknown time value*/
		strcpy(tempArgPtr->value,"0001-01-01T00:00:00Z");	
	}
	else{
		if((tempArgPtr->value = (char*)os_malloc(64)) == NULL){
			tc_printf("malloc  value fails!\r\n");
			return -1;
		}
		memset(tempArgPtr->value, 0, 64);
		/* get download start time */
		time = GetFileTransferCompleteTime();
		
		if(TCSUPPORT_CWMP_PRECISE_TIME_VAL)		
			strcpy(tempArgPtr->value,TimeFormatConvert_improve(time));	
		else
			strcpy(tempArgPtr->value,TimeFormatConvert(time));	
	}
//	curArgPtr->next = tempArgPtr;
	curArgPtr = tempArgPtr;

	return 0;
}

int8 cwmpPerformFileTransfer(void){
	wcStatus		theWcStatus = eWcRequestIdle;
	void*	theWebRequestPtr = NULL;
	int8 initRequstStatus = 0;
	int op_flag = 1;
	int flag = -1;
	/* if no delay, we choose to download in current session immediately */
	void* theTaskDataPtr = cwmpGetTaskDataPtr();

	if(dbg_cwmp >= DBG_CWMP_L1 ){
		tc_printf("\r\n => cwmpPerformFileTransfer \r\n");
	}

	/* For test by Cisico's ACS */
	if (getCorrectACSPktLengthEnable())
		setCorrectACSPktLengthEnable(0);

	/*
		Build the web client request to deliver the event message.
	*/
    theWebRequestPtr = cwmpGetRequestPtr(theTaskDataPtr);
	if (NULL == theWebRequestPtr) {
		tc_printf("\r\ncwmp.c---[cwmpPerformFileTransfer]---No Web Client Request available \r\n");
	}
	else {		
		rpc.fileTransferStruct->faultStructPtr->faultCode= 0;
		strcpy(rpc.fileTransferStruct->faultStructPtr->faultString, "");

		/*	Fill out a download request. */			
		initRequstStatus = cwmpInitFileTransferRequest(theTaskDataPtr, theWebRequestPtr);
		if(initRequstStatus == -1)
			goto SetFileTransferFlag;	

		/*    Queue the message using WebClient. */
		if(!TCSUPPORT_PON_ROSTELECOM_VAL)
		{
			theWcStatus = cwmpQueueRequest(theTaskDataPtr, theWebRequestPtr);
		}
		if(TCSUPPORT_CT_VAL){
			if((rpc.fileTransferStruct->transferType == Download) && (cwmpGetDownloadFileType() == CWMP_CONFIGURATION_FILE))
				system("sh /usr/script/before_tr069_download.sh ROM");
			else
			{
				if ( TCSUPPORT_CT_E8B_ADSL_VAL && TCSUPPORT_CPU_MT7505_VAL )
				{
					if ( Upload == rpc.fileTransferStruct->transferType )
						 system("echo \"3\" > /proc/sys/vm/drop_caches");
					else
						system("sh /usr/script/before_tr069_download.sh");
				}
				else
				{
	            if(TCSUPPORT_CT_PON_CY_VAL || TCSUPPORT_CT_ADSL_HN_VAL)
	                system("echo \"3\" > /proc/sys/vm/drop_caches");
	            else
	                system("sh /usr/script/before_tr069_download.sh");
				}
			}
		}
		else{
#ifdef TCSUPPORT_FW_UPGRADE_16M
			if((rpc.fileTransferStruct->transferType == Download) && (cwmpGetDownloadFileType()==CWMP_FIRMWARE))
				system("sh /usr/script/before_tr069_download.sh");
#endif
		}
		if(TCSUPPORT_PON_ROSTELECOM_VAL)
		{
			theWcStatus = cwmpQueueRequest(theTaskDataPtr, theWebRequestPtr);
		}
		if(rpc.fileTransferStruct->transferType == Download){
			SetDownloadStart(1);
			SaveCwmpFlags();
			tc_printf("begin download: set downloadStart = 1\n");	
		}
		else if(rpc.fileTransferStruct->transferType == Upload){
			if(TCSUPPORT_CT_CWMP_ZIPROMFILE_VAL){
				flag = compute_checksum(UPLOAD_CONFIG_FILE_PATH, CT_CWMP_UPLOAD_ROMFILE_FILE_NAME);
				if(flag < 0)
				{
					unlink(CT_CWMP_UPLOAD_ROMFILE_FILE_NAME);
					tc_printf("begin upload: create zip romfile failed. \n");	
				}
			}
			SetUploadStart(1);
			set_romfile_upLoad_flag(1);
			SaveCwmpFlags();
			tc_printf("begin Upload: set_romfile_upLoad_flag(1);SetUploadStart(1);\n");	
		}

	SetFileTransferFlag:
//#ifdef TCSUPPORT_CROSS_REBOOT
//                setDownloadStartFlag(0);
//#endif
		if (theWcStatus == eWcRequestBusy) {// why busy?
			//cwmp_download = 1;
			set_file_transfer_flag(1);
			if(TCSUPPORT_CT_MIDWARE_VAL)
				op_flag = (iTr069Enable != 0);
			if(op_flag)				
				SetFileTransferCompleteFlag(TransferComplete_Error9002);//it will be replaced when error occurs or transfer complete
			tc_printf("\r\n => SetFileTransferCompleteFlag");
			if(TCSUPPORT_CWMP_PRECISE_TIME_VAL)
				SetFileTransferStartTime_improve();
			else
				SetFileTransferStartTime(msclock());//need to finish!!	
			
			//SetFileTransferStartTime(3243545654);
			if(dbg_cwmp >= DBG_CWMP_L2 ){			
				tc_printf("\r\n Wc Request is Busy.. \r\n");
			}
		}
		else {
			/*Added for dealing with download error, by hqfeng,2006.10.26*/
			SetFileTransferCompleteFlag(TransferComplete_Error9002|EVENT_TransferComplete);
			rpc.fileTransferStruct->faultStructPtr->faultCode= InternalError;
			strcpy(rpc.fileTransferStruct->faultStructPtr->faultString, "Failed to queue file transfer request to web client!");
			
			cwmpReleaseRequestPtr(theTaskDataPtr, theWebRequestPtr);
			tc_printf("Failed to queue request to web client\n");
			op_flag=1;
			if(TCSUPPORT_CT_MIDWARE_VAL)
				op_flag=iTr069Enable;
			if(op_flag)
				cwmpSignalTimerInform(1);
		}
	}	

	return 0;
}

/*TCSUPPORT_PON_ROSTELECOM begin*/
/*get the download url*/
int8 cwmpGetWgetFileTransferArgument(char* url)
{
	wcStatus theWcStatus = eWcRequestIdle;
	void* theWebRequestPtr = NULL;
	int8 initRequstStatus = 0;
	int op_flag = 1;
	int flag = -1;
	/* if no delay, we choose to download in current session immediately */
	void* theTaskDataPtr = cwmpGetTaskDataPtr();
	if(dbg_cwmp >= DBG_CWMP_L1 ){
		tc_printf("\r\n => cwmpPerformFileTransfer \r\n");
	}

	/* For test by Cisico's ACS */
	if (getCorrectACSPktLengthEnable())
		setCorrectACSPktLengthEnable(0);

	/*
		Build the web client request to deliver the event message.
	*/
    theWebRequestPtr = cwmpGetRequestPtr(theTaskDataPtr);
	if (NULL == theWebRequestPtr) 
    {
		tc_printf("\r\ncwmp.c---[cwmpPerformFileTransfer]---No Web Client Request available \r\n");
        return -1;
    }
	else 
    {       
		rpc.fileTransferStruct->faultStructPtr->faultCode= 0;
		strcpy(rpc.fileTransferStruct->faultStructPtr->faultString, "");

		/*	Fill out a download request. */			
		initRequstStatus = cwmpInitFileTransferRequest(theTaskDataPtr, theWebRequestPtr);
    }
    strcpy(url, rpc.fileTransferStruct->URL);
    return 0;
}
/*TCSUPPORT_PON_ROSTELECOM end*/

extern char * RpFindTokenDelimitedPtr(const char *theStartOfTokenPtr,
                                const char theDelimiter);
void initFileDomainName(char *theURL)
{
	char *		theSourcePtr = NULL;
	Unsigned16	theLength = 0;
	char		theHost[kWcMaxNameLength] = {0};
	char		cmd[128] = {0};

	if ( !theURL )
		return;

	theSourcePtr = RpFindTokenDelimitedPtr(theURL, kAscii_Colon);
	if ( theSourcePtr != (char *) 0)
	{
        /*
            The path is absolute, so skip over the "http://".
        */
        theSourcePtr += 3;

        /*
            Find the end of the host string.
        */
        theURL = theSourcePtr;
        theLength = 0;

        while (*theSourcePtr != ':' && *theSourcePtr != '/' &&
                *theSourcePtr != '\0') {
            theSourcePtr++;
            theLength++;
        }

        if (theLength < kWcMaxNameLength)
		{
            RP_MEMCPY(theHost, theURL, theLength);
            theHost[theLength] = '\0';

			sprintf(cmd, "/bin/echo -n \"%s\" > %s", theHost, TMP_DNS_TR69_FILETRANS);
			system(cmd);
	     }
	}
}

int cwmpBuildTransferCompleteBodyStruct(SoapBodyInfoPtr BodyStructPtr)
{
	RPCArgumentPtr curArgPtr = NULL, tempArgPtr = NULL;
	char * CmdKeyPtr = NULL;
	EventStructPtr     eventPtr;
	ParameterValueStructPtr paraValuePtr = NULL;
	uint32 time;
	FaultDetailPtr faultDetailPtr = NULL;
	int rtnCode = 0;
	int faultCode = 0;
	InformEventIndex  eventIndex;

	if(rpc.fileTransferStruct->transferType == Download){
		eventIndex = M_Download;
      	}
	else if(rpc.fileTransferStruct->transferType == Upload){
		eventIndex = M_Upload;
	}
	else{
		tc_printf("\r\n File transfer type  is unknow!");
		return -1;
	}

	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n=> cwmpBuildTransferCompleteBodyStruct()\n");
	}

	if(BodyStructPtr == NULL){
		tc_printf("TransferComplete  body struct is NULL!\r\n");
		return -1;
	}
      // rpc name
	strcpy(BodyStructPtr->rpcName, "TransferComplete");

     //Argument.
	if(cwmpInitArgument("CommandKey", 1, StringType, 0, &tempArgPtr) == -1){
		tc_printf("Initiate TransferComplete argument CommandKey fails!\r\n");
		return -1;
	}
	BodyStructPtr->argumentPtr = tempArgPtr;
	curArgPtr = tempArgPtr;
	if((tempArgPtr->value = os_malloc(strlen(GetCommandKey(eventIndex))+1)) == NULL){
		tc_printf("malloc  transfer complete command key fails!\r\n");
		return -1;
	}
	strcpy((char*)tempArgPtr->value,GetCommandKey(eventIndex)); //CmdKeyPtr;

	faultCode = rpc.fileTransferStruct->faultStructPtr->faultCode;//added by hqfeng, 2007.01.11
	if(faultCode == 0){
		if((GetFileTransferCompleteFlag()&ErrorCodeMask) != 0){
			faultCode = -1;	
		}
	}
	
//	curArgPtr = tempArgPtr;
//	BodyStructPtr->argumentPtr = tempArgPtr;

#if 1 //FaultStruct xzwang_060731
	if(cwmpInitArgument("FaultStruct", 1, FaultDetailType, 0, &tempArgPtr) == -1){
		tc_printf("Initiate TransferComplete argument fails!\r\n");
		return -1;
	}
	curArgPtr->next = tempArgPtr;
      	rtnCode = cwmpGetFaultDetail(&faultDetailPtr);
	if(rtnCode != 0){
		return rtnCode;
	}
	tempArgPtr->value = faultDetailPtr;
	curArgPtr = tempArgPtr;
	
#endif

	//StartTime
	if(cwmpInitArgument("StartTime", 1, DateTimeType, 0,  &tempArgPtr) == -1){
		tc_printf("Initiate TransferComplete argument fails!\r\n");
		return -1;
	}
	curArgPtr->next = tempArgPtr;

	if((tempArgPtr->value = (char*)os_malloc(64)) == NULL){
		tc_printf("malloc  value fails!\r\n");
		return -1;
	}
	memset(tempArgPtr->value, 0, 64);
	/* get download/upload start time */
	if(faultCode == 0){//no error
		time = GetFileTransferStartTime();
	if(TCSUPPORT_CWMP_PRECISE_TIME_VAL)		
		strcpy(tempArgPtr->value,TimeFormatConvert_improve(time));
	else
		strcpy(tempArgPtr->value,TimeFormatConvert(time));
	}else{//error,set the start time to empty string, hqfeng, 2007.01.11
		strcpy(tempArgPtr->value,"0001-01-01T00:00:00Z");//qingpu_070620. Bug No:FNPD02863 & FNPD02864
	}
	curArgPtr = tempArgPtr;

	//CompleteTime
	if(cwmpInitArgument("CompleteTime", 1, DateTimeType, 0,  &tempArgPtr) == -1){
		tc_printf("Initiate TransferComplete argument fails!\r\n");
		return -1;
	}
	curArgPtr->next = tempArgPtr;

	if((tempArgPtr->value = (char*)os_malloc(64)) == NULL){
		tc_printf("malloc  value fails!\r\n");
		return -1;
	}
	memset(tempArgPtr->value, 0, 64);
	/* get download/upload complete time */
	if(faultCode == 0){//no error
		time = GetFileTransferCompleteTime();
		if(TCSUPPORT_CWMP_PRECISE_TIME_VAL)
			strcpy(tempArgPtr->value,TimeFormatConvert_improve(time));
		else
			strcpy(tempArgPtr->value,TimeFormatConvert(time));
	}else{//error,set the start time to empty string, hqfeng, 2007.01.11
		strcpy(tempArgPtr->value,"0001-01-01T00:00:00Z");//qingpu_070620. Bug No:FNPD02863 & FNPD02864
	}
	curArgPtr = tempArgPtr;

	return 0;
	
}

uint8 DelayFileTransferCompleteFlag = 0;
int /*int8*/ cwmpTransferCompleteResponseResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	/*Auto PVC may stop current TR69 Transaction. we set FileTransfer Flag when Transaction Complete*/
	//SetFileTransferCompleteFlag(EVENT_NoTransferComplete);
	DelayFileTransferCompleteFlag = 1;
	emptyPaket = 1;
	//SaveCwmpFlags();
	downloadInProgress = 0;
	return 0;	
}

int	cwmpGetFaultDetail(FaultDetailPtr *faultDetailPtrP){
	FaultDetailPtr tempFaultPtr = NULL;
	int err_state = 0;
	uint8 transferCompleteFlag = 0;
	char faultString[128] = "";
	
	if((tempFaultPtr = (FaultDetailPtr)os_malloc(sizeof(FaultDetail))) == NULL){
		tc_printf("malloc fault struct fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;
	}	
	memset(tempFaultPtr, 0, sizeof(FaultDetail));

	if((GetFileTransferCompleteFlag() & EVENT_TransferComplete) != 0 )
	
	{
		if( (spSysParam.tcCwmpAttr->downloadErr ) && (spSysParam.tcCwmpAttr->downloadErr < sizeof(downloadErrString)/sizeof(downloadErrString[0]) ) )
		{
			strcpy(faultString,downloadErrString[spSysParam.tcCwmpAttr->downloadErr /*GetDownloadErrType()*/]); 
			SetDownloadErrType(0);
			//SaveCwmpFlags();
		}
		else if( (spSysParam.tcCwmpAttr->uploadErr ) && (spSysParam.tcCwmpAttr->uploadErr < sizeof(uploadErrString)/sizeof(uploadErrString[0]) ) )
		{
			strcpy(faultString,uploadErrString[spSysParam.tcCwmpAttr->uploadErr /*GetUploadErrType()*/]); 
			SetUploadErrType(0);
		}
		else
			strcpy(faultString,"");
		tcdbg_printf("error number is: %d\n", spSysParam.tcCwmpAttr->downloadErr); 
	}
	else
	{
		/*Modified for dealing with download error, hqfeng, 2006.10.26*/
		strcpy(faultString, rpc.fileTransferStruct->faultStructPtr->faultString);		
	}
	
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncwmpRpc.c---[cwmpGetFaultDetail]---faultCode = %ld ",rpc.fileTransferStruct->faultStructPtr->faultCode);
		tc_printf("\r\ncwmpRpc.c---[cwmpGetFaultDetail]---faultString = %s",faultString);
	}

	if((tempFaultPtr->faultString = (char*)os_malloc(strlen(faultString)+1)) == NULL){
		tc_printf("malloc fault  string fails!\r\n");
		err_state = ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		goto Error_Handler;

	}

	/*Modified for dealing with download error, hqfeng, 2006.10.26*/
	//copy the current faultStructPtr.
	tempFaultPtr->faultCode = rpc.fileTransferStruct->faultStructPtr->faultCode;
	transferCompleteFlag = GetFileTransferCompleteFlag();
	/* rpc.fileTransferStruct->faultStructPtr->faultCode could be lost because of  restarting.
	    sam, 2008.10.22 */
	if(tempFaultPtr->faultCode == 0){
		if((transferCompleteFlag&ErrorCodeMask) != 0){//check download error information
			if((transferCompleteFlag&TransferComplete_Error9002) != 0){
				tempFaultPtr->faultCode = InternalError;
			}else if((transferCompleteFlag&TransferComplete_Error9010) != 0){
				tempFaultPtr->faultCode = DownloadFailure;
				SetDownloadErrType(DOWNLOAD_FAIL);
			}else if((transferCompleteFlag&TransferComplete_Error9012) != 0){
				tempFaultPtr->faultCode = FileTransferAuthenFailure;
			}else if((transferCompleteFlag&TransferComplete_Error9013) != 0){
				tempFaultPtr->faultCode = UnsupportedProtocol;
			}else if((transferCompleteFlag&TransferComplete_Error9018) != 0){
				tempFaultPtr->faultCode = IllegalFirmware;
			}else if((transferCompleteFlag&TransferComplete_Error9011) != 0){
				tempFaultPtr->faultCode = UploadFailure;
				SetUploadErrType(UPLOAD_FAIL);
				tc_printf("cwmpGetFaultDetail:UploadFailure\n");
			}
		}
	}
	strcpy(tempFaultPtr->faultString, faultString);
	//reset the faultStructPtr.
	rpc.fileTransferStruct->faultStructPtr->faultCode = 0;
	strcpy(rpc.fileTransferStruct->faultStructPtr->faultString, "");
	
	*faultDetailPtrP = tempFaultPtr;

	return 0;
	
	Error_Handler:

	if(tempFaultPtr != NULL){
		if(tempFaultPtr->faultString != NULL){
			os_free(tempFaultPtr->faultString);
			tempFaultPtr->faultString = NULL;
		}
		os_free(tempFaultPtr);
		tempFaultPtr = NULL;
	}
		
		return err_state;
}

int/*8*/ cwmpFaultCodeResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{	
	RPCArgumentPtr  requestArgumentPtr = NULL;
	FaultDetailPtr  pFaultDetail = NULL;
	int	faultCode = 0;
	
	if(requestBodyStructPtr == NULL)
		return -1;

	requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	if(requestArgumentPtr == NULL)
		return InvalidArgument;

	while(requestArgumentPtr != NULL){
		if( strcmp( requestArgumentPtr->name, "detail") == 0){
			pFaultDetail = (FaultDetailPtr)requestArgumentPtr->value;
			faultCode = pFaultDetail->faultCode;
			break;
		}
		requestArgumentPtr = requestArgumentPtr->next;
	}	

	switch(faultCode)
	{
		case ACS_MethodNotSupport:
		case ACS_RequestDeny:
		case ACS_InternalError:
		case ACS_InvalidArgument:
		case ACS_ResourceExceed:
			if(getBootFlag() == 1){
				setACSFaultCode(faultCode);
				cwmpSetSessionRetryFlag(1);
			}else{
				setACSFaultCode(0);
				emptyPaket = 1;
			}
			break;
		case ACS_RetryRequest:
			setACSFaultCode(faultCode);
			break;
		default:
			setACSFaultCode(0);
			emptyPaket = 1;
			break;
	}
	cwmpDealWithFaultCode(faultCode);
	return 0;
		
}

int cwmpProcessGetRPCMethodsResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	RPCArgumentPtr responseArgumentPtr;
	RPCArgumentPtr nextArgumentPtr;
	RPCArgumentPtr siblingArgumentPtr;
	StringStructPtr curPara;
		
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
	if(strcmp(requestArgumentPtr->name, "MethodList") == 0){
		//print the RPC Methods.
		tc_printf("\r\nThe ACS supports the following RPC Methods:");
		curPara = (StringStructPtr)requestArgumentPtr->parameterList;
		while(curPara != NULL){
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n%s",curPara->value);
			}
			curPara = curPara->next;
		}
	}else
		return InvalidArgument;

	emptyPaket = 1;	//return empty packet.

	return 0;
}


//RebootResponse
int cwmpRebootResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	
	RPCArgumentPtr  requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	AccessType accessType = Reboot;
    
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}
 
	return cwmpProcessRPCResponse(requestArgumentPtr, accessType, NULL, NULL );
}


//FactoryResetResponse
int cwmpFactoryResetResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\ncwmpRpc.c---[cwmpFactoryResetResponse]-----");
	}
	setFactoryResetFlag(1);
	return 0;	
}

//ScheduleInform Response,hqfeng,2007.01.09
int cwmpScheduleInformResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr)
{
	RPCArgumentPtr requestArgumentPtr = NULL;
	unsigned int delaySeconds = 0;
	char *str = NULL;
	int rtnCode = 0;

	if(requestBodyStructPtr == NULL){
		return InvalidArgument;
	}
	
	requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	if(requestArgumentPtr == NULL){
		return InvalidArgument;
	}
	
	while(requestArgumentPtr != NULL){
		if( strcmp( requestArgumentPtr->name, "DelaySeconds") == 0){
			str = (char*)requestArgumentPtr->value;
			if(str == NULL){
				return InvalidArgument;
			}
			delaySeconds = (unsigned int)atoi(str);
			if(delaySeconds == 0){
				return InvalidArgument;
			}
			
		}else if(strcmp( requestArgumentPtr->name, "CommandKey") == 0){
			if(requestArgumentPtr->value != NULL){ 
				rtnCode = SetCommandKey((char*)requestArgumentPtr->value, M_ScheduleInform);
			}else{
				rtnCode = SetCommandKey("", M_ScheduleInform);
			}

			if(rtnCode != 0){
				return InvalidArgument;
			}
		}else{
			return InvalidArgument;
		}
		requestArgumentPtr = requestArgumentPtr->next;
	}	

	cwmpSignalScheduleInform(delaySeconds);
	
	return 0;
}


//InformResponse response.
int cwmpRpcInformResponseResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr){
	RPCArgumentPtr requestArgumentPtr = requestBodyStructPtr->argumentPtr;
	uint8 	downloadComplete;
	uint8 	downloadType;
	int 		ret; //modifed by hqfeng, 2006.01.04
	Unsigned32		theHostAddress;
	Unsigned16		thePort;
	char			thePath[kWcMaxPathLength];
	char                HostName[kWcMaxNameLength];
	char temp[256] = {0};
	time_t startTime = 0, rightnow = 0;
	int		op_flag = 0;
	
	if(requestArgumentPtr == NULL){
		tc_printf("requestArgumentPtr is NULL!\r\n");
		return -1;
	}

	if(strcmp(requestArgumentPtr->name, "MaxEnvelopes") == 0 && requestArgumentPtr->value != NULL){
		uint8 maxEnv = atoi((char*)requestArgumentPtr->value);
		if(cwmpSetMaxEnvelopes(maxEnv) == -1){
			tc_printf("Invalid value of MaxEnvelopes!\r\n");
			return InvalidArgument;
		}
	}
	else{
		tc_printf("Invalid MaxEnvelopes node!\r\n");
	}

	//Added for Session Retry,by hqfeng,2006.07.11
	cwmpSetSessionRetryFlag(0);
	cwmpResetSessionRetryWaitInterval();
	cwmpCalculateSessionRetryCount(0);
	cwmpSetInformCompleteFlag(1);
	
	if(cwmpGetACSFlag == 1){
		cwmpSetSessionRetryFlag(1);
		if(cwmpGetSessionRetryCount() == 0 )
        {
			cwmpCalculateSessionRetryCount(1);
		}
	}
	
	if(getMotiveRSTFlag() == 1)
	{
		setACSAuthSuccess();
	}
	
	//Deal with the Host Address,by hqfeng,2006.06.20
	//WcParseUrl(getCwmpUrl(), HostName, &theHostAddress, &thePort, thePath);
	cwmpGetCurACSInfo(&theHostAddress, &thePort, thePath);
	if(TCSUPPORT_CMCC_VAL){
		processTheHostExt(&theHostAddress, &thePort, thePath, 1);
	}
	else{
		processTheHostAddress(&theHostAddress, 1);
	}
#if 0
	//Added for reset the RebootFlag, by hqfeng,2006.06.22 
	if(getCwmpRebootFlag(NULL)){
		setCwmpRebootFlag(0);	
	}
#endif
	cwmpResetInformFlag();//added by hqfeng,2006.08.01
	//authenticationFlag = 1;
	cwmpSetAuthenFlag(1);
	//memset( rpc.txBuf,0,strlen(rpc.txBuf)+1);	
	
	if(dbg_cwmp >= DBG_CWMP_L2 ){
    		tc_printf("\r\n[cwmpRpcInformResponse] ---Inform Response ");		
       }

	/* send TransferComplete */
	downloadComplete = GetFileTransferCompleteFlag();
	if(TCSUPPORT_CROSS_REBOOT_VAL)
		op_flag = ( !(getDownloadStartFlag()) && ((downloadComplete & EVENT_TransferComplete) != 0) && (getHoldRequestFlag() == 0));
	else
		op_flag = ( ((downloadComplete & EVENT_TransferComplete) != 0) && (getHoldRequestFlag() == 0));
	
	if(op_flag){ // download
	#if 0	
		downloadType = GetFileTransferTypeFlag();
		if( downloadType == 0) { // immediate-download
			/* send downloadResponse */
			if(dbg_cwmp > DBG_CWMP_L2 ){
    				tc_printf("\r\n[cwmpRpcInformResponse] ---send downloadResponse");		
       		}
			cwmpBuildFileTransferResponse(responseBodyStructPtr, 0);
			SetFileTransferCompleteFlag(0); // complete for immediate-download
			return 0;
		}else{
			/* send TransferComplete */
			if(dbg_cwmp > DBG_CWMP_L2 ){
    				tc_printf("\r\n[cwmpRpcInformResponse] ---send TransferComplete");		
       		}
			ret = cwmpBuildTransferCompleteBodyStruct(responseBodyStructPtr);
			SetFileTransferCompleteFlag(0); // complete for delayed-download
			return ret;
		}
	#else
		if(dbg_cwmp > DBG_CWMP_L2 ){
    			tc_printf("\r\n[cwmpRpcInformResponse] ---send TransferComplete");		
       	}
		ret = cwmpBuildTransferCompleteBodyStruct(responseBodyStructPtr);
		return ret;
	#endif
	}
#ifndef CODE_SHRINK  //qingpu.20080110
	else if(getGetRPCMethodsFlag() == 1
		    && gACSGetRPCMethods == 1
		    && getHoldRequestFlag() == 0 )
	{
	
		ret = cwmpBuildGetRPCMethodsBodyStruct(responseBodyStructPtr);
		//setGetRPCMethodsFlag(0);
		gACSGetRPCMethods = 0;
		return ret;
	}
#endif
	else{ // send an empty packet
		emptyPaket = 1;
		if(TCSUPPORT_CROSS_REBOOT_VAL){
			tcapi_get("Cwmp_Entry", "DownloadStartTime", temp);
			startTime = (unsigned int)atoi(temp);
			time(&rightnow);
			if(rightnow < startTime)
			{
				tc_printf("\r\nNTP server not sync, return.");
				return 0;
			}
			if((downloadInProgress == 0) && getDownloadStartFlag() && getDownloadContinueFlag())
			{
				cwmpDownloadContinue(responseBodyStructPtr);
			}
		}
		return 0;
	}

	return 0;
}


int cwmpProcessRPCResponse (RPCArgumentPtr  requestArgumentPtr, uint8 accessType, RPCArgumentPtr responseArgumentPtr, int* status ){
	ParameterType reqArgType, resArgType;
	void* rootResPara = NULL;
	void* resPara = NULL;
	void* tempResPara = NULL;
	uint32  instanceNumber;
	int 	rtnStatus = CWMP_E_SUCCESS;

	if(requestArgumentPtr == NULL){
		tc_printf("process requestArgumentPtr is NULL!\r\n");
		return -1;
	}

	reqArgType = requestArgumentPtr->type;
	if(responseArgumentPtr != NULL)
		resArgType = responseArgumentPtr->type;

	if(accessType == Read){ 	//for GetParameterValues, GetParameterNames, GetParameterAttributes.
		if(responseArgumentPtr == NULL){
			tc_printf("process responsetArgumentPtr is NULL!\r\n");
			return -1;
		}
		rtnStatus = cwmpGetParameterList(requestArgumentPtr, resArgType, responseArgumentPtr);
		if( rtnStatus != CWMP_E_SUCCESS){
			tc_printf("Get parameter list fails!\r\n");
			return rtnStatus;
		}
	}
	else if(accessType == Write){  // for SetParameterValues, SetParameterAttributes.
		rtnStatus = cwmpSetParameterList(requestArgumentPtr, reqArgType);
		if( rtnStatus != CWMP_E_SUCCESS){
			
			if(rtnStatus == CWMP_E_DELAY) 
			{						
					return rtnStatus;
			}

			tc_printf("Set parameter list fails!\r\n");
			return rtnStatus;
		}
	}
	else if(accessType == Add){ // for AddObject, hqfeng,2006.04.25
		rtnStatus = cwmpAddObject(requestArgumentPtr, &instanceNumber);
		if( rtnStatus != CWMP_E_SUCCESS){
			tc_printf("AddObject fails!\r\n");
			return rtnStatus;//Return fauit code.
		}
		*status = rtnStatus;
		return instanceNumber;//return instance number.

	}else if(accessType == Delete){
		rtnStatus = cwmpDeleteObject(requestArgumentPtr);
		if( rtnStatus != CWMP_E_SUCCESS){
			tc_printf("DeleteObject fails!\r\n");
			return rtnStatus;
		}
		*status = rtnStatus;

	}else if(accessType == Reboot){
		rtnStatus = cwmpReboot(requestArgumentPtr);
		if( rtnStatus != CWMP_E_SUCCESS){
			tc_printf("Reboot fails!\r\n");
			return rtnStatus;
		}
	}else{
		tc_printf("the request type is unkown!\r\n");

	}

	return CWMP_E_SUCCESS;
}

int cwmpGetParameterList(RPCArgumentPtr requestArgumentPtr, ParameterType resArgType, RPCArgumentPtr  responseArgumentPtr){
	StringStructPtr requestPara;
	void* rootResPara = NULL;
	void* resPara = NULL;
	void* tempResPara = NULL;
	int 	rtnStatus = CWMP_E_SUCCESS;

	if(strcmp(requestArgumentPtr->name, "ParameterNames") == 0 ){
		requestPara = (StringStructPtr)requestArgumentPtr->parameterList;

		cwmpClearNextLevelFlag();
		// get all device node information.
		while(requestPara != NULL){ 
			tempResPara = NULL;
			if(TCSUPPORT_CWMP_FAST_GET_VAL)
				cwmpFastGet = 1;
			rtnStatus = cwmpGetAllDeviceNodeInfo( requestPara->value, resArgType, &tempResPara, &responseArgumentPtr->number);
			if(TCSUPPORT_CWMP_FAST_GET_VAL)	
				cwmpFastGet = 0;
/*
			if(rtnStatus != CWMP_E_SUCCESS)
				return rtnStatus;
*/			
			if(tempResPara != NULL){
				if (resPara == NULL){
					//rootResPara = tempResPara;
					responseArgumentPtr->parameterList = tempResPara;
					resPara = tempResPara;
				}	
				else{
					switch(resArgType){
						case ParameterValueStructType:
						{
							ParameterValueStructPtr curResPara = (ParameterValueStructPtr)resPara;
							while(curResPara->next != NULL)
								curResPara = curResPara->next;
					
							curResPara->next = (ParameterValueStructPtr)tempResPara;
							resPara = tempResPara;
							break;
						}
						case ParameterAttributeStructType:
						{
							ParameterAttributeStructPtr curResPara = (ParameterAttributeStructPtr)resPara;
							while(curResPara->next != NULL)
								curResPara = curResPara->next;
					
							curResPara->next = (ParameterAttributeStructPtr)tempResPara;
							resPara = tempResPara;
							break;
						}
						default:
							if(dbg_cwmp >= DBG_CWMP_L2 ){
								tc_printf("The argument type is not supported!\r\n");
							}
							break;
					}	
				}
			}
			else{
				if(dbg_cwmp >= DBG_CWMP_L2 ){
					tc_printf("The temp resposne parameter is NULL\r\n");
				}
			}
			/* Set tempResPara to responseArgumentPtr->parameterList first,
			  * and then check the rtnStatus. Because the tempResPara may be
			  *  not NULL, if the rtnStatus is not  CWMP_E_SUCCESS.
			  */
			if(rtnStatus != CWMP_E_SUCCESS)
				return rtnStatus;

			requestPara = requestPara->next;
		}
	}
	else if(strcmp(requestArgumentPtr->name, "ParameterPath") == 0){
		RPCArgumentPtr nextArgument = requestArgumentPtr->next;	

		if(nextArgument != NULL && strcmp(nextArgument->name, "NextLevel") == 0 && nextArgument->value != NULL){
			uint8 nextLevel = 1;
			if(strcmp(nextArgument->value,"0") == 0 || strcmp(nextArgument->value,"False") == 0 ||  strcmp(nextArgument->value,"false") == 0 )
				nextLevel = 0;
			#if 1				
				if(TCSUPPORT_CT_PON_CY_JX_VAL)//tcapi_get ("Cwmp_Entry","fastGet",tmpVaule)==0)
				{
					char tmpVaule[4] = {0};
					int dbgLevel =0;
					#define FIRST_LEVEL (1<<0)
					#define SECONDE_LEVEL (1<<1)

					if(requestArgumentPtr->value != NULL)
					{
						//dbgLevel=atoi(tmpVaule);
						tcdbg_printf("\r\n:requestArgumentPtr->value:%s",requestArgumentPtr->value);
						//tcdbg_printf("\r\n:dbgLevel:%d",dbgLevel);	
						if ((strlen(requestArgumentPtr->value) <= (strlen("InternetGatewayDevice.")+3)) &&
								(strncmp(requestArgumentPtr->value,"InternetGatewayDevice.",strlen("InternetGatewayDevice.")) == 0))
								//nextLevel=1;//for update "InternetGatewayDevice." ,is don't send
								root_name_flag = 2;
						
						if(root_name_flag > 0)//value 2
						{
							if((strlen(requestArgumentPtr->value)<= (strlen("InternetGatewayDevice.WANDevice.")+3)) &&
								(strncmp(requestArgumentPtr->value,"InternetGatewayDevice.WANDevice.",strlen("InternetGatewayDevice.WANDevice.")) == 0)) {
								nextLevel=1;//for update "InternetGatewayDevice." ,is don't send
								if (root_name_flag > 0)
									root_name_flag--;
							}
							else if ((strlen(requestArgumentPtr->value)<= (strlen("InternetGatewayDevice.Services.VoiceService.")+3)) &&
								(strncmp(requestArgumentPtr->value,"InternetGatewayDevice.Services.VoiceService.",strlen("InternetGatewayDevice.Services.VoiceService.")) == 0)) {
								 nextLevel=1;//for update "InternetGatewayDevice." ,is don't send
								 if (root_name_flag > 0)
									root_name_flag--;
							}

						}
					
						tcdbg_printf("\r\n:nextLevel:%d",nextLevel);	
					}
					
				}
			#endif
			
			if(nextLevel){
				cwmpSetNextLevelFlag();
			}
			else{
				cwmpClearNextLevelFlag();
			}
		}
		else{
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("Without valid NextLevel argument!\r\n");
			}
			cwmpClearNextLevelFlag();
		}
		if(TCSUPPORT_CWMP_FAST_GET_VAL)	
			cwmpFastGet = 1;
		rtnStatus = cwmpGetAllDeviceNodeInfo( (char*)requestArgumentPtr->value, resArgType, &tempResPara, &responseArgumentPtr->number);
		if(TCSUPPORT_CWMP_FAST_GET_VAL)	
			cwmpFastGet = 0;
		if(tempResPara != NULL){
//			rootResPara = tempResPara;
			responseArgumentPtr->parameterList = tempResPara;
		}

		if(rtnStatus != CWMP_E_SUCCESS)
			return rtnStatus;
/*		
		if(tempResPara != NULL){
//			rootResPara = tempResPara;
			responseArgumentPtr->parameterList = tempResPara;
		}
*/
	}
	else{
		tc_printf("Invalid request argument\r\n");
		return InvalidArgument;
	}

//	responseArgumentPtr->parameterList = rootResPara;

	return 0;
			
}

int cwmpSetParameterList(RPCArgumentPtr requestArgumentPtr, ParameterType reqArgType){
	int 	rtnStatus = CWMP_E_SUCCESS;
	int 	error = CWMP_E_SUCCESS;
	int 	err_ret = 0;
	int cdStatus = 1;
	char unSetParamName[128]="InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.2.";
	if(requestArgumentPtr == NULL){
		return -1;
	}
	
	if(strcmp(requestArgumentPtr->name, "ParameterList") == 0){
		switch(reqArgType){
			case ParameterValueStructType:
			{
				ParameterValueStructPtr requestPara = (ParameterValueStructPtr)requestArgumentPtr->parameterList;
				while(requestPara != NULL){
					if(TCSUPPORT_CT_PON_C9_VAL){
						if(strncmp(requestPara->name,unSetParamName,strlen(unSetParamName))==0)
							rtnStatus=CWMP_E_SUCCESS;
						else
							rtnStatus = cwmpSetDeviceNodeInfo(requestPara, reqArgType);
					}else{
						rtnStatus = cwmpSetDeviceNodeInfo(requestPara, reqArgType);
					}
					if(rtnStatus == CWMP_E_DELAY) 				
						 error = CWMP_E_DELAY;
					else {
						    if(rtnStatus != CWMP_E_SUCCESS) {
								cdStatus = 1;
								if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
									cdStatus = ( (rtnStatus != WriteSimCardFail) && (rtnStatus != SimCardAuthenticationFail) );
								if((rtnStatus < InvalidParameterName || rtnStatus> SetReadOnlyParameter)  
									&& (rtnStatus != InvalidRequestUsername) 
									&& cdStatus
								){									
									if(faultDetailPtr){
										cwmpReleaseFaultDetailStruct(faultDetailPtr);
										faultDetailPtr = NULL;
									}
								}
								else{
									tc_printf("\r\n error = rtnStatus");
								}
								if(!TCSUPPORT_CWMP_FAULT_RESPONSE_VAL)		
									return rtnStatus;
								else{
									if (err_ret == 0)
										err_ret = rtnStatus;
								}
							}else{
								/* Don't return until set all parameters completetly! */
							}	
					}
					requestPara = requestPara->next;
				}
				
				//return error;

				break;
			}
			case SetParameterAttributesStructType:
			{
				ParameterAttributesSetStructPtr requestPara = (ParameterAttributesSetStructPtr)requestArgumentPtr->parameterList;
				while(requestPara != NULL){
					if(TCSUPPORT_CT_PON_C9_VAL){
						if(strncmp(requestPara->name,unSetParamName,strlen(unSetParamName))==0)
							rtnStatus=CWMP_E_SUCCESS;
						else
							rtnStatus = cwmpSetDeviceNodeInfo(requestPara, reqArgType);
					}else{
						rtnStatus = cwmpSetDeviceNodeInfo(requestPara, reqArgType);
					}
					if( rtnStatus != CWMP_E_SUCCESS){
						tc_printf("set parameter attributes fails!\r\n");
						return rtnStatus;
					}					
					requestPara = requestPara->next;
				}
				break;
			}
			default:
				if(dbg_cwmp >= DBG_CWMP_L2 ){
					tc_printf("Unkown set argument type!\r\n");
				}
				break;
		}
	}
	else{
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("for write operation, need further handle\r\n");
		}
	}

	if(TCSUPPORT_CWMP_FAULT_RESPONSE_VAL){
		if (err_ret != 0)
			return err_ret;
	}
	return error;
}

int cwmpAddObject(RPCArgumentPtr requestArgumentPtr, uint32 *instanceNumber)
{
	char *objectName, tempName[160], instanceStr[20];
	char *parameterKey;
	RPCArgumentPtr nextArgumentPtr;
	DeviceNodePtr deviceNode = NULL;
	int result;
	char faultString[FaultStringLength];
	int rtnStatus = CWMP_E_SUCCESS;
	int rtncode;
	
	if(requestArgumentPtr == NULL){
		return InvalidArgument;
	}
	memset(tempName, 0, 160);
	if(strcmp(requestArgumentPtr->name, "ObjectName") == 0){
		objectName = (char*)(requestArgumentPtr->value);
	}else
		return InvalidArgument;

	nextArgumentPtr = requestArgumentPtr->next;
	if(strcmp(nextArgumentPtr->name, "ParameterKey") == 0){
		parameterKey = (char*)(nextArgumentPtr->value);
	}else
		return InvalidArgument;

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &deviceNode);
	//Added by hqfeng,2006.12.26
	if(deviceNode == NULL){
		return InvalidArgument;
	}
	
	if(deviceNode->addObject == NULL)//added by hqfeng,2006.07.11
		return InvalidArgument;
	if(deviceNode->type != CWMP_OBJECT){
		tc_printf("Can not set value for leaf device node!\r\n");
		return InvalidArgument;
	}
	if(deviceNode->accessType == ReadOnly){
		tc_printf("Can not set value for readonly device node!\r\n");
		sprintf(faultString, "The paramter's accessType is ReadOnly:%s", objectName);//by hqfeng,2006.07.06
		rtnStatus = RequestDeny;
		rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
		if(rtncode != 0)
			return rtncode;
		return rtnStatus;
	}	

	cwmpGetDeviceNodeName(deviceNode, tempName);
	
	cwmpSetSpSaveFlag(0);
	result = deviceNode->addObject(instanceNumber, deviceNode);
	cwmpSetSpSaveFlag(1);
	if(result == -1){
		tc_printf(" Can not find usued InstanceNumber!");
		return InternalError;
	}

	//Clear attributes for new object. xzwang_060818
	sprintf(instanceStr, ".%d", *instanceNumber);
	strcat(tempName, instanceStr);
	cwmpResetAttribute(tempName);
	
	return 0;
}

int cwmpResetAttribute(char *deviceName){
	DeviceNodePtr theDeviceNode = NULL;
	ParameterAttributesSetStruct attribute;


	tc_printf("\r\n%s",deviceName);
	
	cwmpSearchDeviceNodeByName(deviceName, rpc.IGDDeviceRootPtr, &theDeviceNode);

	if(theDeviceNode){
		attribute.name = deviceName;	
		attribute.notificationChange = 1;
		attribute.notification = 0;
		attribute.accessListChange = 0;
		//mark by brian for remving warning 
		//attribute.accessListChange = NULL;
		resetAttributeAfeterAddObject = 1;

		cwmpSetDeviceNodeAttribute(&attribute, theDeviceNode);

		resetAttributeAfeterAddObject = 0;
	}
	else{
		tc_printf("[cwmpResetAttribute]The device node is NULL for reset attributes!\r\n");

	}

	return 0;
	
}

int cwmpDeleteObject(RPCArgumentPtr requestArgumentPtr)
{
	char *objectName;
	char *parameterKey;
	RPCArgumentPtr nextArgumentPtr;
	DeviceNodePtr deviceNode = NULL;
	int result;
	int number;
	char faultString[FaultStringLength];
	int rtnStatus = CWMP_E_SUCCESS;
	int rtncode;
	
	if(requestArgumentPtr==NULL){
		return InvalidArgument;
	}
	
	if(strcmp(requestArgumentPtr->name, "ObjectName") == 0){
		objectName = (char*)(requestArgumentPtr->value);
	}else
		return InvalidArgument;

	nextArgumentPtr = requestArgumentPtr->next;
	if(strcmp(nextArgumentPtr->name, "ParameterKey") == 0){
		parameterKey = (char*)(nextArgumentPtr->value);
	}else
		return InvalidArgument;

	cwmpSearchDeviceNodeByName(objectName, rpc.IGDDeviceRootPtr, &deviceNode);
	//Added by hqfeng,2006.12.26
	if(deviceNode == NULL){
		return InvalidArgument;
	}
	
	if(deviceNode->deleteObject == NULL)//Added by hqfeng,2006.07.11
		return InvalidArgument;
	if(deviceNode->type != CWMP_OBJECT){
		tc_printf("Can not set value for leaf device node!\r\n");
		return InvalidArgument;
	}
	if(deviceNode->accessType == ReadOnly){
		tc_printf("Can not set value for readonly device node!\r\n");
		sprintf(faultString, "The paramter's accessType is ReadOnly:%s", objectName);//by hqfeng,2006.07.06
		rtnStatus = RequestDeny;
		rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
		if(rtncode != 0)
			return rtncode;
		return rtnStatus;
	}	
	
	number = atoi(deviceNode->name);
	if(TCSUPPORT_CT_WANNODE_MAPPING_VAL){//tony add
		if(number < 0)// || number > INSTANCENUMBER)
			return -1;
	}
	else{
		if(number < 0 || number > INSTANCENUMBER)
			return -1;
	}
	cwmpSetSpSaveFlag(0);
	result = deviceNode->deleteObject((uint32)number,deviceNode);
	cwmpSetSpSaveFlag(1);

	if(result == -1){
		tc_printf(" Can not find instance number for deleting!");
		return InvalidArgument;
	}

	return CWMP_E_SUCCESS;
	
}

int cwmpDelaySetRebootFlag(void)
{
	//setCwmpRebootFlag(1); //Modified by hqfeng,2006.06.09
	sysRestart(0);
	return 0;
}

int cwmpReboot(RPCArgumentPtr requestArgumentPtr)
{
	char *CommandKey;
	int rtnCode = 0;
	
	if(requestArgumentPtr== NULL){
		return InvalidArgument;
	}
	
	if(strcmp(requestArgumentPtr->name, "CommandKey") == 0){
		if(requestArgumentPtr->value != NULL){  //xzwang_20061213
		         rtnCode = SetCommandKey((char*)requestArgumentPtr->value, M_Reboot);
		}else{
		         rtnCode = SetCommandKey("", M_Reboot);
		}

		if(rtnCode != 0){
			return InvalidArgument;
		}
	}else{
		return InvalidArgument;
	}

	if ( TCSUPPORT_ECN_SIP_VAL )
	{
	    tcapi_set("InfoVoIP_Common", "WAN_SET_ENABLE", "Yes");
	    tcapi_commit("VoIPBasic");
	    tcdbg_printf("[%s]==reboot===>\n", __func__);
	    sleep(5);
	}

	if (TCSUPPORT_CT_PON_CY_JX_VAL) {
		int delay = 0;
		char delay_buf[10] = {0};

		tcapi_get("Cwmp_Entry", "delayReboot", delay_buf);
		delay = atoi(delay_buf);
		if (delay == 0)
			delay = 3000;

		tcdbg_printf("\r\ncwmpReboot(): delay = %d", delay);
		
		timerStartS(delay, (TimerCallback)cwmpDelaySetRebootFlag, NULL );
	}
	else {
	setCwmpRebootFlag(1); //Modified by hqfeng,2006.06.09
	}

	return 0;
}

int cwmpGetAllDeviceNodeInfo(char* requestParameterName, ParameterType resArgType, void** ResponseParaPtr, int* paraNumberPtr){
	DeviceNodePtr deviceNode = NULL;
	DeviceNodePtr currentDeviceNode = NULL;
	void* tempResPara = NULL;
	void* resPara = NULL;
	void* rootResPara = NULL;
	char faultString[FaultStringLength];
	int rtnStatus = CWMP_E_SUCCESS;
	int rtncode;

    //find device node by its name
	cwmpSearchDeviceNodeByName(requestParameterName, rpc.IGDDeviceRootPtr, &deviceNode);
	if(deviceNode == NULL){
		tc_printf("\r\n Can not find the device node!");
		if(getFaultDetailFlag() == 1){
			sprintf(faultString, "Invalid parameter name:%s", requestParameterName);//by hqfeng,2006.07.06
		}else{
		strcpy(faultString, "Invalid parameter name");
		}
		rtnStatus = InvalidParameterName;
		rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
		if(rtncode != 0)
			return rtncode;
		return rtnStatus;
	}
	
	currentDeviceNode = deviceNode;
	if(TCSUPPORT_CWMP_FAST_GET_VAL){
		if(requestParameterName != NULL){
		if((currentDeviceNode != rpc.IGDDeviceRootPtr))
		{
			cwmpFastGet = 0;
		}
		
		if(/*strncmp(currentDeviceNode->name,"WANDevice",strlen("WANDevice")) == 0*/
			(strlen(requestParameterName)<= (strlen("InternetGatewayDevice.WANDevice")+3)) &&
		(strncmp(requestParameterName,"InternetGatewayDevice.WANDevice",strlen("InternetGatewayDevice.WANDevice")) == 0)
		){
			cwmpFastGet = 1;
		}
		if(
			(strlen(requestParameterName)<= (strlen("InternetGatewayDevice.LANDevice.1.WLANConfiguration")+3)) &&
		(strncmp(requestParameterName,"InternetGatewayDevice.LANDevice.1.WLANConfiguration",strlen("InternetGatewayDevice.LANDevice.1.WLANConfiguration")) == 0)
		){
			cwmpFastGet = 1;
		}

		if(
			(strlen(requestParameterName)<= (strlen("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig")+3)) &&
		(strncmp(requestParameterName,"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig",strlen("InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig")) == 0)
		){
			cwmpFastGet = 1;
		}	

		if(
			(strlen(requestParameterName)<= (strlen("InternetGatewayDevice.LANDevice.1.LANHostConfigManagement")+3)) &&
		(strncmp(requestParameterName,"InternetGatewayDevice.LANDevice.1.LANHostConfigManagement",strlen("InternetGatewayDevice.LANDevice.1.LANHostConfigManagement")) == 0)
		){
			cwmpFastGet = 1;
		}	

		if(
		strncmp(requestParameterName,"InternetGatewayDevice.DeviceInfo",strlen("InternetGatewayDevice.DeviceInfo")) == 0
		){
			cwmpFastGet = 1;
		}
		}
	}
	
	//get device node information
#if 0 //marked by xzwang. 20061201
	if(currentDeviceNode->firstChild == NULL){
		rtnStatus = cwmpGetSingleDeviceNodeInfo(currentDeviceNode, resArgType, &tempResPara);
		if(rtnStatus != CWMP_E_SUCCESS ){
			return rtnStatus;	
		}
		if(tempResPara != NULL){
			rootResPara = tempResPara;
			(*paraNumberPtr)++;
		}
		else{
			if(dbg_cwmp >= DBG_CWMP_L2)
				tc_printf("fetch device information into response parameter fails or be object!\r\n");
		//	return -1;
		}
	}
	else{
#endif		
		if(cwmpGetNextLevelFlag()){  // for get parameter names.
			if(requestParameterName == NULL){
				tempResPara = NULL;
				rtnStatus = cwmpGetSingleDeviceNodeInfo(rpc.IGDDeviceRootPtr, resArgType, &tempResPara);
				if(rtnStatus != CWMP_E_SUCCESS ){
					return rtnStatus;	
				}
				if(tempResPara != NULL){
//					rootResPara = tempResPara;
			             *ResponseParaPtr = tempResPara;
					(*paraNumberPtr)++;
				}
				else{
					if(dbg_cwmp >= DBG_CWMP_L2)
						tc_printf("fetch device information into response parameter fails or be object!\r\n");
				//	return -1;
				}
				goto GetInfoSuccess;
			}
				
			if(currentDeviceNode->type != CWMP_OBJECT )
				return InvalidArgument;

			currentDeviceNode = currentDeviceNode->firstChild;			
			while(currentDeviceNode != NULL){
				tempResPara = NULL;
				rtnStatus = cwmpGetSingleDeviceNodeInfo(currentDeviceNode, resArgType, &tempResPara);
				if(rtnStatus != CWMP_E_SUCCESS ){
					return rtnStatus;	
				}
				if(tempResPara != NULL){
					if (resPara == NULL){
					//	rootResPara = tempResPara;
						*ResponseParaPtr = tempResPara;
						resPara = tempResPara;
					}
					else{
						ParameterInfoStructPtr curResPara = (ParameterInfoStructPtr)resPara;
						curResPara -> next = (ParameterInfoStructPtr)tempResPara;
						resPara = tempResPara;
					}			
					(*paraNumberPtr)++;
				}
				else{
					if(dbg_cwmp >= DBG_CWMP_L2)
						tc_printf("fetch device information into response parameter fails or be object!\r\n");
				//	return -1;
				}

				currentDeviceNode = currentDeviceNode->nextSibling;
			}
		}
		else{
			while(currentDeviceNode != NULL){
				while(currentDeviceNode != NULL){
					tempResPara = NULL;
					rtnStatus = cwmpGetSingleDeviceNodeInfo(currentDeviceNode, resArgType, &tempResPara);
					if(rtnStatus != CWMP_E_SUCCESS ){
						cwmpResetDeviceStack();//added by hqfeng,2007.01.05
						return rtnStatus;	
					}
					if(tempResPara != NULL){
						if (resPara == NULL){
						//	rootResPara = tempResPara;
							*ResponseParaPtr = tempResPara;
							resPara = tempResPara;
						}
						else{
							switch(resArgType){
								case ParameterValueStructType:
								{
									ParameterValueStructPtr curResPara = (ParameterValueStructPtr)resPara;
									curResPara -> next = (ParameterValueStructPtr)tempResPara;
									resPara = tempResPara;
									break;
								}
								case ParameterInfoStructType:
								{	
									ParameterInfoStructPtr curResPara = (ParameterInfoStructPtr)resPara;
									curResPara -> next = (ParameterInfoStructPtr)tempResPara;
									resPara = tempResPara;
									break;
								}	
								case ParameterAttributeStructType:
								{
									ParameterAttributeStructPtr curResPara = (ParameterAttributeStructPtr)resPara;
									curResPara -> next = (ParameterAttributeStructPtr)tempResPara;
									resPara = tempResPara;
								break;
								}
								case String256Type:
								case StringType:
								{
									StringStructPtr curResPara = (StringStructPtr)resPara;
									curResPara -> next = (StringStructPtr)tempResPara;
									resPara = tempResPara;
									break;
								}
								case IntType:
									break;
								case UnsignedIntType:
									break;
								case BooleanType:
									break;
								case DateTimeType:
									break;
								case Base64Type:
									break;
								default:
									tc_printf("Unkown response parameter type\r\n");
									break;
							}
						}			
						(*paraNumberPtr)++;
					}
					else{
						if(dbg_cwmp >= DBG_CWMP_L2)
							tc_printf("fetch device information into response parameter fails or be object!\r\n");
					//	return -1;
					}
					
					rtnStatus = cwmpPushDeviceStack(currentDeviceNode);
					if(rtnStatus != CWMP_E_SUCCESS){
						return rtnStatus;
					}
					currentDeviceNode = currentDeviceNode->firstChild;
				}
				while(currentDeviceNode == NULL && deviceNodeStackHead!= NULL){
					cwmpPopDeviceStack(&currentDeviceNode);
				
					if(deviceNodeStackHead != NULL)
					currentDeviceNode = currentDeviceNode->nextSibling;
					else
						currentDeviceNode = NULL;
				}
			}

		}
#if 0		
	}
#endif

GetInfoSuccess:
//	(*ResponseParaPtr) = rootResPara;
	
	return 0;
}

int cwmpSetDeviceNodeInfo(void* requestParameter,ParameterType reqArgType){
	DeviceNodePtr deviceNode = NULL;
	DeviceNodePtr currentDeviceNode = NULL;
	void* tempResPara = NULL;
	int 	rtnStatus = CWMP_E_SUCCESS;
	int   rtncode;
	char faultString[FaultStringLength];

	if(requestParameter == NULL){
		tc_printf("The request parameter for set value is NULL!\r\n");
		return -1;
	}
	
	switch(reqArgType){
		case ParameterValueStructType:
		{
			ParameterValueStructPtr curReqPara = (ParameterValueStructPtr)requestParameter;
			
			cwmpSearchDeviceNodeByName(curReqPara->name, rpc.IGDDeviceRootPtr, &deviceNode);

			rtnStatus = cwmpCheckDeviceNode(deviceNode, curReqPara);
			if(rtnStatus != CWMP_E_SUCCESS){
				return rtnStatus;
			}

			cwmpSetSpSaveFlag(0);
			rtnStatus = cwmpSetDeviceNodeValue(curReqPara->valueInfoPtr, deviceNode);
			cwmpSetSpSaveFlag(1);
			if( (rtnStatus != CWMP_E_SUCCESS) && (rtnStatus != ResourceExceed)){

				if(rtnStatus == CWMP_E_DELAY) 
				{						
					if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && getCpeFlag() == 1 && getItmsActionStartFlag() == 0 && isCardExist() == 0){
						rtnStatus = -11;	
					}
					else 	
						return rtnStatus;
				}
				
				if(rtnStatus == -10){
					strcpy(faultString, "Can not set 'admin' for ConnectionRequestUsername!");
					rtnStatus = InvalidRequestUsername;

				}else if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && rtnStatus == -11){
					strcpy(faultString, "Sim Card 3DES_ECB check fail!");	
					tcdbg_printf("Sim Card 3DES_ECB check fail!!!\n");
					rtnStatus = SimCardAuthenticationFail;
				}else if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && rtnStatus == -12){
					strcpy(faultString, "Write Sim Card Fail!!!");	
					tcdbg_printf("Write Sim Card Fail!!!\n");
					rtnStatus = WriteSimCardFail;
				}	else{
					strcpy(faultString, "Invalid parameter value");
					rtnStatus = InvalidParameterValue;
				}
				rtncode = cwmpAddSetParameterValuesFaultList(curReqPara->name, rtnStatus, faultString);
				if(rtncode != 0)
					return rtncode;					
				return rtnStatus;
			}
			else if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && getCpeFlag() == 1 && getItmsActionStartFlag() == 0 && isCardExist() == 0){
				strcpy(faultString, "Sim Card 3DES_ECB check fail!");	
				tcdbg_printf("Sim Card 3DES_ECB check fail, card is not exist!!!\n");
				rtnStatus = SimCardAuthenticationFail;
				
				rtncode = cwmpAddSetParameterValuesFaultList(curReqPara->name, rtnStatus, faultString);
				if(rtncode != 0)
					return rtncode;					
				return rtnStatus;
			}

			/*some node maybe free deviceNode,so we get deviceNode again*/
			cwmpSearchDeviceNodeByName(curReqPara->name, rpc.IGDDeviceRootPtr, &deviceNode);
			/*multipulate notify node list with new value.*/
			cwmpMultipulateNotifyListByNewValue(deviceNode);
			break;
		}
		case SetParameterAttributesStructType:
		{
			ParameterAttributesSetStructPtr curReqPara = (ParameterAttributesSetStructPtr)requestParameter;
		
			cwmpSearchDeviceNodeByName(curReqPara->name, rpc.IGDDeviceRootPtr, &deviceNode);

			if(deviceNode == NULL){
				if(getFaultDetailFlag() == 1){
					sprintf(faultString, "Invalid parameter name:%s", curReqPara->name);//by hqfeng,2006.07.06
				}else{
				strcpy(faultString, "Invalid parameter name");
				}
				rtnStatus = InvalidParameterName;
				rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
				if(rtncode != 0)
					return rtncode;
				return rtnStatus;
			}

			cwmpSetSpSaveFlag(0);
			rtnStatus = cwmpSetDeviceNodeAttribute(curReqPara, deviceNode);
			cwmpSetSpSaveFlag(1);
			if(rtnStatus != CWMP_E_SUCCESS){
				return rtnStatus;
			}
	#if 0  //marked by xzwang_060710		
			if(deviceNode->type == CWMP_OBJECT){
//				strcpy(faultString, "Invalid parameter name");
				sprintf(faultString, "Invalid parameter name:%s", curReqPara->name);//by hqfeng,2006.07.06
				rtnStatus = InvalidParameterName;
				rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
				if(rtncode != 0)
					return rtncode;
				return rtnStatus;
			}
			
			if(deviceNode->accessType == ReadOnly){
				sprintf(faultString, "The paramter's accessType is ReadOnly:%s", curReqPara->name);//by hqfeng,2006.07.06
				rtnStatus = RequestDeny;
				rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
				if(rtncode != 0)
					return rtncode;
				return rtnStatus;			
			}
	#endif
			
			break;
		}
		
		default:
			tc_printf("Unkown set argument type!\r\n");
			return -1;
	}

	return rtnStatus;
}

int cwmpSetDeviceNodeValue(ValueInfoPtr valueInfoPtr, DeviceNodePtr deviceNode){
	int 	rtnStatus = CWMP_E_SUCCESS;

	if(valueInfoPtr == NULL || valueInfoPtr->value == NULL){
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\nThe value for setting is NULL: ");
			tc_printf("%s\r\n",deviceNode->name);
		}
		return rtnStatus;
	}
	
	if(deviceNode == NULL){
		return -1;
	}
	
	if(deviceNode->setValueInfo == NULL){
		tc_printf("[cwmpSetDeviceNodeValue] %s setValueInfo function is NULL",  deviceNode->name);
		return -1;
	}
	
	switch(valueInfoPtr->type){
		case Base64Type:
			tc_printf("Need further handle for base64 type!\r\n");
		//	break;
		case StringType:
		case String256Type:
		case DateTimeType:
			rtnStatus = deviceNode->setValueInfo(valueInfoPtr->value,deviceNode);
			if(rtnStatus != CWMP_E_SUCCESS)
				return  rtnStatus;
			break;
		case IntType:
			{
				int* tempValue = NULL;
				int value = 0;

				//Add for checking whether the value is a valid unsigned int type. by hqfeng,2006.02.06
				if(cwmpIsNumeric(valueInfoPtr->value) == -1){
					return InvalidParameterValue;
				}
				value =  atoi(valueInfoPtr->value);
		#if 0	//marked by xzwang_20070226	
				if(value > 32767 || value < -32768){
					return InvalidParameterValue;
				}
		#endif		
				if((tempValue= (int*)os_malloc(sizeof(int))) != NULL){
//					*tempValue = atoi(valueInfoPtr->value);
					*tempValue = value;
					rtnStatus = deviceNode->setValueInfo(tempValue,deviceNode);
				}
				else{
					tc_printf("Malloc temp value for set value fails!\r\n");
					return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				}
				if(tempValue){
				        os_free(tempValue);
					tempValue = NULL;
				}
				break;
			}
		case UnsignedIntType:
			{
				unsigned int* tempValue = NULL;
				unsigned int value = 0;

				//Add for checking whether the value is a valid unsigned int type. by hqfeng,2006.02.06
				if(cwmpIsNumeric(valueInfoPtr->value) != 0){
					return InvalidParameterValue;
				}
				value =  atoi(valueInfoPtr->value);
#if 0	//marked by xzwang_20070226	
				if(value > 65535 || value < 0){
					return InvalidParameterValue;
				}
#endif				
				if((tempValue= (unsigned int*)os_malloc(sizeof(unsigned int))) != NULL){
//					*tempValue = atoi(valueInfoPtr->value);
					*tempValue = value;
					rtnStatus = deviceNode->setValueInfo(tempValue,deviceNode);
				}
				else{
					tc_printf("Malloc temp value for set value fails!\r\n");
					return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				}

				if(tempValue){
				        os_free(tempValue);
					tempValue = NULL;
				}
				break;
			}
		case BooleanType:
			{
				int8* tempValue = NULL;
				if((tempValue= (int8*)os_malloc(sizeof(int8))) != NULL){
					//*tempValue = atoi(valueInfoPtr->value);
					*tempValue = cwmpGetBooleanValue(valueInfoPtr->value);
					if(*tempValue == -1)
						return -1;
					rtnStatus = deviceNode->setValueInfo(tempValue,deviceNode);
				}
				else{
					tc_printf("Malloc temp value for set value fails!\r\n");
					return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				}

				if(tempValue){
				        os_free(tempValue);
					tempValue = NULL;
				}
				break;
			}
		default:
			tc_printf("do not support the value type: %d", valueInfoPtr->type);
			break;
	}

	return rtnStatus;
}

/*______________________________________________________________________________
**	cwmpIsNumeric
**
**	description:
**	This function check whether the string consist of numeric.
**
**	parameters:
**	*valueStr
**
**	return:
**	-1:	This string includes other chars.
**	 0:   This string only consist of numeric.
**	 1:   This string consist of numeric and  the "+" or the "-".
**   
**   Owner: hqfeng,2007.02.06
**____________________________________________________________________________*/
int8 cwmpIsNumeric(const char *valueStr)
{
	char *tempPtr;
	int flag = 0;
	
	if(valueStr == NULL){
		return -1;
	}
	
	tempPtr = (char*)valueStr;
	if(*tempPtr == '\0'){
		return -1;
	}
	
	while(*tempPtr == kAscii_Space){//Ignore the space.
		tempPtr++;
	}
	
	if(*tempPtr == '+' || *tempPtr == '-'){//check whether it has the sign.
		flag = 1;
		tempPtr ++;
	}

	while(*tempPtr != '\0')
	{
		if(*tempPtr > '9' || *tempPtr < '0'){//check whether it only contains the numeric.
			return -1;
		}
		tempPtr++;
	}

	if(flag == 1){
		return 1;
	}else{
		return 0;
	} 
	
}

int8 cwmpGetBooleanValue(char* boolStr){
	char tempStr[20];

	//tempStr = getCapitalStr(boolStr);
	if(strlen(boolStr) > 19)
		return -1;
	
	strcpy(tempStr, boolStr);
	
	if(strcmp(tempStr, "1") == 0 || strcmp(tempStr, "true") == 0 || strcmp(tempStr, "True") == 0)
		return 1;

	if(strcmp(tempStr, "0") == 0 || strcmp(tempStr, "false") == 0 || strcmp(tempStr, "False") == 0)
		return 0;

	return -1;
}

int cwmpSetDeviceNodeAttribute(ParameterAttributesSetStructPtr reqParameter, DeviceNodePtr deviceNode){
	int 	rtnStatus = CWMP_E_SUCCESS;
	int   rtncode;
	char faultString[FaultStringLength];

	if(reqParameter == NULL){
		tc_printf("The requset parameter for set attribute is NULL");
		return -1;
	}
	
	while(deviceNode != NULL){
		while(deviceNode != NULL){
			
			if(deviceNode->type != CWMP_OBJECT){
				//check attribute. xzwang_061012
				//modify by brian ,the judgement ,which is marked, is always no use
				if(/*reqParameter->notification < 0 || */reqParameter->notification > 2 ||
					(reqParameter->notificationChange != 0 && reqParameter->notificationChange != 1)){
					sprintf(faultString, "The setting Attribute for %s is rejected!", reqParameter->name);
					rtnStatus = InvalidParameterValue;
					rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
					if(rtncode != 0)
						return rtncode;
					return rtnStatus;
				}
				
				if(/*(deviceNode->flag == 2 && reqParameter->notification != 0)//flag ==2,can't set to active or passive type.hqfeng,2006.09.30 //marked by xzwang_20061207
				   || */(deviceNode->flag == 0 && reqParameter->notification == 2))//flag == 0, Can't set to active type.hqfeng,2006.09.30
				{
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						tc_printf("\r\n[cwmpSetDeviceNodeInfo] reject to set ative attribute for node:%s\r\n",deviceNode->name);
					}
					
					sprintf(faultString, "The setting Attribute for %s is rejected!", reqParameter->name);//by hqfeng,2006.07.06
					rtnStatus = RejectRequestNotification;
					rtncode = cwmpCreateDetailFaultStruct(rtnStatus, faultString);
					if(rtncode != 0)
						return rtncode;
					return rtnStatus;
				}

				if(deviceNode->setAttribute){
					/*multipulate notify node list with new attributes.*/
					if(reqParameter->notificationChange)
						cwmpMultipulateNotifyListByNewAttr(deviceNode, reqParameter->notification);
						
					rtnStatus = deviceNode->setAttribute(reqParameter->notificationChange, reqParameter->notification, reqParameter->accessListChange, reqParameter->accessList, deviceNode);

				}else{
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						tc_printf("\r\n[cwmpSetDeviceNodeInfo] setAttribute function is NULL for node:%s\r\n",deviceNode->name);
					}
				}
			}
			
			cwmpPushDeviceStack(deviceNode);
			deviceNode = deviceNode->firstChild;
			
		}

		while(deviceNode == NULL && deviceNodeStackHead != NULL){
			cwmpPopDeviceStack(&deviceNode);

			if(deviceNodeStackHead != NULL)
				deviceNode = deviceNode->nextSibling;
			else
				deviceNode = NULL;
		}

	}
	
	return 0;
}

/*search telecom new node*/
 int midSearchDeviceNodeByName(char* requestParameterName, DeviceNodePtr deviceRoot, DeviceNodePtr* curDeviceNode)
{
	int i = 0, number = 0;
	DeviceNodePtr  node = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceRoot==NULL){
			tc_printf("The device root is NULL\r\n");
			return -1;
		}

		if(requestParameterName == NULL){
			tc_printf("The requst parameter name is NULL\r\n");
			return -1;
		}

		number = countNewTeleNode_Open();
		node = deviceRoot;
		for(i=0; i<number; i++){
			if(strcmp(requestParameterName, node->name)==0){
				*curDeviceNode = node;
				return 0;
			}
			node = node->nextSibling;
		}

		*curDeviceNode = node;
	}
	return -1;
}

int cwmpSearchDeviceNodeByName(char* requestParameterName, DeviceNodePtr deviceRoot, DeviceNodePtr* curDeviceNode){
	char   name[PARAMETER_NUM][PARAMETER_NAME_LEN]; //for store the parameter names.
	int   count = 0;
	int   i = 0;
	DeviceNodePtr  node;
	char *parameterName;

	if(deviceRoot == NULL){
		tc_printf("The device root is NULL\r\n");
		return -1;
	}

	if(requestParameterName == NULL){   //xzwang_060628
		tc_printf("The requst parameter name is NULL\r\n");
		*curDeviceNode = deviceRoot;   //default is entire object while empty. xzwang_060720
		return 0;
	}

	if(strcmp(requestParameterName, ".") == 0){   //xzwang_060628
		tc_printf("The requst parameter name is .\r\n");
		*curDeviceNode = deviceRoot;   //default is entire object while parameterName is ".". xzwang_060720
		return 0;
	}
		
	parameterName = requestParameterName;

	while((*parameterName) !=kAscii_Null){
		if(i >=  PARAMETER_NUM)     //Invalid parameter. xzwang_20061113
			return -1;
			
		if(cwmpDivideDeviceNodeName(&parameterName, name[i]) == -1) ////Judge parameter lenth. xzwang_20061024
			return -1;
		i++;
	}

	count = i;

	node = deviceRoot;
	for(i = 0; i < count; i++){
		if(node == NULL)//added by hqfeng,2006.06.06
			break;
		if( strcmp( node->name, name[i]) == 0){
			if( i != count - 1  )
				node = node->firstChild;
		}else{
			node = node->nextSibling;
			while( node != NULL && strcmp( node->name, name[i]) != 0){
				node = node->nextSibling;
			}
			if( node != NULL){
				if( i != count - 1)
					node = node->firstChild;
			}
			else{
				node = NULL;
				break;
			}
		}
		
	}

	*curDeviceNode = node;

	return 0;
}


int cwmpDivideDeviceNodeName( char **parameterName, char *name){
	char  theCharacter;
	char  tempName[PARAMETER_NAME_LEN];
	int i = 0;

	memset( tempName, 0, PARAMETER_NAME_LEN);

	theCharacter = **parameterName;
	while(theCharacter !=kAscii_Null &&  theCharacter != kAscii_Dot){
		tempName[i] = theCharacter;
		(*parameterName)++;
		i++;
		if(i >= PARAMETER_NAME_LEN -1)    //Judge parameter lenth. xzwang_20061024
			return -1;
		theCharacter = **parameterName;
	}
	
	if( theCharacter == kAscii_Null ){
		if( tempName[0] != kAscii_Null){
			tempName[i] = '\0';
			strcpy( name, tempName);
		}else{
			// This is an Object parameter.Maybe we need to do something in the future.
		}
	}else if( theCharacter == kAscii_Dot ){
		tempName[ i ] = '\0';
		strcpy( name, tempName);
		(*parameterName)++;
	}else if( theCharacter == kAscii_Space ){
		// Do nothing, ignore the space.
	}

	return 0;
}

int cwmpGetShortNodeInfo(DeviceNodePtr deviceNode, ParameterType resArgType, void** responseParameter){
	DeviceNodePtr tempDeviceNode = deviceNode;
	int rtnStatus = CWMP_E_SUCCESS;
	char temp_name[256] = "";

	if(TCSUPPORT_CT_MIDWARE_VAL){	
		if(deviceNode == NULL){
			tc_printf("The device Node for get infomation is NULL\r\n");
			return -1;
		}

		switch(resArgType){
			case ParameterValueStructType:
			{
				break;
			}
			case ParameterInfoStructType:
			{
				ParameterInfoStructPtr tempResPara = NULL;
				char tempParaName[256] = "";
				
				if(tempResPara= (ParameterInfoStruct*)os_malloc(sizeof(ParameterInfoStruct))){
					*responseParameter = tempResPara;
					memset(tempResPara, 0, sizeof(ParameterInfoStruct));
					//cwmpGetDeviceNodeName(tempDeviceNode, tempParaName);
					strncpy(tempParaName, tempDeviceNode->name, strlen(tempDeviceNode->name));
					if(tempDeviceNode->type == CWMP_OBJECT) //for object 
						strcat(tempParaName,".");

					if((tempResPara->name = (char*)os_malloc(strlen(tempParaName)+1)) != NULL){
						strcpy(tempResPara->name, tempParaName);
					}
					else{
						tc_printf("[GetSingleDeviceNodeInfo] malloc ParameterInfoStruct's name fails!\r\n");
						cwmpFreeSingleParameterInfoStruct(tempResPara);
						tempResPara = NULL;
						return ResourceExceed;
					}

					tempResPara->writable = tempDeviceNode->accessType;
				}
				else{
					tc_printf("[GetDeviceNodeInfo] malloc ParameterInfoStruct fails!\r\n");
					return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
				}
				break;
			}

			case ParameterAttributeStructType:
			{	
				break;
			}

			case StringType:
			case IntType:
			case UnsignedIntType:
			case BooleanType:
			case DateTimeType:
			case Base64Type:
			default:
				tc_printf("[GetSingleDeviceNodeInfo] need further handle for response type %d!", resArgType);
				break;
		}
	}
	return 0;
}

int cwmpGetSingleDeviceNodeInfo(DeviceNodePtr deviceNode, ParameterType resArgType, void** responseParameter){
	DeviceNodePtr tempDeviceNode = deviceNode;
	int rtnStatus = CWMP_E_SUCCESS;
	char temp_name[256] = "";
	
	if(deviceNode == NULL){
		tc_printf("The device Node for get infomation is NULL\r\n");
		return -1;
	}

	switch(resArgType){
		case ParameterValueStructType:
		{	
			ParameterValueStructPtr tempResPara = NULL;
			char tempParaName[256];

			if(tempDeviceNode->type == CWMP_OBJECT)
				break;

			tempResPara = (ParameterValueStructPtr)os_malloc(sizeof(ParameterValueStruct));
			if(tempResPara == NULL){
				tc_printf("[GetDeviceNodeInfo] malloc parameterValueStruct fails!\r\n");
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			}
			*responseParameter = tempResPara;
			memset(tempResPara, 0, sizeof(ParameterValueStruct));
			cwmpGetDeviceNodeName(tempDeviceNode, tempParaName);

			if((tempResPara->name = (char*)os_malloc(strlen(tempParaName)+1)) != NULL){  //xzwang_060623
				strcpy(tempResPara->name, tempParaName);
			}
			else{
				tc_printf("[GetSingleDeviceNodeInfo] malloc ParameterValueStruct's name fails!\r\n");
				cwmpFreeSingleParameterValueStruct(tempResPara);
				tempResPara = NULL;
				return ResourceExceed;
			}

			if((tempResPara->valueInfoPtr = (ValueInfoPtr)os_malloc(sizeof(ValueInfo))) == NULL){
				tc_printf("[GetDeviceNodeInfo] malloc parameterValueStruct's valueInfo fails!\r\n");
				cwmpFreeSingleParameterValueStruct(tempResPara);
				tempResPara = NULL;
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			}
			memset(tempResPara->valueInfoPtr, 0,sizeof(ValueInfo));
			if(tempDeviceNode->getValueInfo){
				rtnStatus = tempDeviceNode->getValueInfo(&(tempResPara->valueInfoPtr->value), &(tempResPara->valueInfoPtr->type),tempDeviceNode);
				if(rtnStatus != CWMP_E_SUCCESS){
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						tc_printf("\r\n [cwmpGetSingleDeviceNodeInfo]--getValueInfo error!\r\n The Paramter is:");
						cwmpGetDeviceNodeName(tempDeviceNode, temp_name);
						tc_printf("%s\r\n",temp_name);
					}
					cwmpFreeSingleParameterValueStruct(tempResPara);
					tempResPara = NULL;
					return InternalError;//replace InternalError by ResourceExceed,hqfeng,2006.06.21  //RESTORE TO Internal error. xzwang_20061114
				}
			}else{
				cwmpFreeSingleParameterValueStruct(tempResPara);
				tempResPara = NULL;
				tc_printf("[GetDeviceNodeInfo] %s getValueInfo function is NULL!\r\n", tempDeviceNode->name);
				return NoGetValueFunction;
			}

			break;
		}
		
		case ParameterInfoStructType:
		{
			ParameterInfoStructPtr tempResPara = NULL;
			char tempParaName[256] = "";
			
			if(tempResPara= (ParameterInfoStruct*)os_malloc(sizeof(ParameterInfoStruct))){
				*responseParameter = tempResPara;
				memset(tempResPara, 0, sizeof(ParameterInfoStruct));
				cwmpGetDeviceNodeName(tempDeviceNode, tempParaName);

				if(tempDeviceNode->type == CWMP_OBJECT) //for object 
					strcat(tempParaName,".");

				if((tempResPara->name = (char*)os_malloc(strlen(tempParaName)+1)) != NULL){
					strcpy(tempResPara->name, tempParaName);
				}
				else{
					tc_printf("[GetSingleDeviceNodeInfo] malloc ParameterInfoStruct's name fails!\r\n");
					cwmpFreeSingleParameterInfoStruct(tempResPara);
					tempResPara = NULL;
					return ResourceExceed;
				}

				tempResPara->writable = tempDeviceNode->accessType;
			}
			else{
				tc_printf("[GetDeviceNodeInfo] malloc ParameterInfoStruct fails!\r\n");
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			}
		
//			*responseParameter = tempResPara;
			break;
		}

		case ParameterAttributeStructType:
		{	
			ParameterAttributeStructPtr tempResPara = NULL;
			char tempParaName[256] = "";

			if(tempDeviceNode->type == CWMP_OBJECT)
				break;

			tempResPara = (ParameterAttributeStructPtr)os_malloc(sizeof(ParameterAttributeStruct));
			if(tempResPara != NULL){
				*responseParameter = tempResPara;
				memset(tempResPara, 0,sizeof(ParameterAttributeStruct));
				cwmpGetDeviceNodeName(tempDeviceNode, tempParaName);

				if((tempResPara->name = (char*)os_malloc(strlen(tempParaName)+1)) != NULL){  //xzwang_060623
					strcpy(tempResPara->name, tempParaName);
				}
				else{
					tc_printf("[GetSingleDeviceNodeInfo] malloc ParameterAttributeStruct's name fails!\r\n");
					cwmpFreeSingleParameterAttributeStruct(tempResPara);
					tempResPara= NULL;
					return ResourceExceed;
				}
								
				if(tempDeviceNode->getAttribute){
				       rtnStatus = tempDeviceNode->getAttribute(&(tempResPara->notification), &(tempResPara->accessList),tempDeviceNode);
					if(rtnStatus != CWMP_E_SUCCESS){
						cwmpFreeSingleParameterAttributeStruct(tempResPara);
						tempResPara= NULL;
						return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
					}
									
				}else{
					tc_printf("[GetDeviceNodeInfo] %s getAttribute function is NULL!\r\n", tempDeviceNode->name);
					cwmpFreeSingleParameterAttributeStruct(tempResPara);
					tempResPara= NULL;
					return NoGetAttributeFunction;
				}
			}
			else{
				tc_printf("[GetSingleDeviceNodeInfo] malloc parameterAttributeStruct fails!\r\n");
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			}

//			*responseParameter = tempResPara;
			break;
		}

		case StringType:
		case String256Type:
		case IntType:
		case UnsignedIntType:
		case BooleanType:
		case DateTimeType:
		case Base64Type:
		default:
			tc_printf("[GetSingleDeviceNodeInfo] need further handle for response type %d!", resArgType);
			break;
	}
	
	return 0;
}

int cwmpPushDeviceStack(DeviceNodePtr deviceNode){
	DeviceNodeStackPtr newStackNode =NULL;

	if(deviceNode == NULL){
		tc_printf("The device node for push into stack is NULL ");
		return -1;
	}
	if((newStackNode = (DeviceNodeStackPtr)os_malloc(sizeof(DeviceNodeStack))) == NULL){
		tc_printf("malloc device stack node fails");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	memset(newStackNode,0, sizeof(DeviceNodeStack));
	newStackNode->deviceNode = deviceNode;
	
	//add by brian
	pthread_mutex_lock(&value_change_lock);
	newStackNode->next = deviceNodeStackHead;
	deviceNodeStackHead = newStackNode;
	pthread_mutex_unlock(&value_change_lock);

	return 0;
}

int cwmpPopDeviceStack(DeviceNodePtr* deviceNode){
	DeviceNodeStackPtr tempStackNode =deviceNodeStackHead;
	
	if(deviceNodeStackHead == NULL){
		tc_printf("The device stack is NULL, pop stack fails!\r\n");
		return -1;
	}
    //modify by brian
	pthread_mutex_lock(&value_change_lock);
	*deviceNode = deviceNodeStackHead->deviceNode;
	
	deviceNodeStackHead = deviceNodeStackHead->next;
	tempStackNode ->next =NULL;
	pthread_mutex_unlock(&value_change_lock);

	if(tempStackNode != NULL){
	      os_free(tempStackNode);
	      tempStackNode = NULL;
	}
    
	return 0;
}


//modify by brian according to linos(huawei trunk)
int cwmpResetDeviceStack(void)
{
	DeviceNodePtr deviceNode;
	int rtnCode = 0;

	while(rtnCode == 0){
		rtnCode = cwmpPopDeviceStack(&deviceNode);
	}
    
	return 0;
}
int8 cwmpInitFaultStruct(cwmp_rpc_t *rpc)
{
	if((rpc->soapFaultPtr = (FaultStructPtr)os_malloc(sizeof(FaultStruct))) == NULL){
		tc_printf("malloc soap fault structure error!\r\n");
		return -1;
	}
	memset(rpc->soapFaultPtr, 0, sizeof(FaultStruct));
	strcpy(rpc->soapFaultPtr->faultcode, "Client");
	strcpy(rpc->soapFaultPtr->faultstring, "CWMP fault");
	if(faultDetailPtr != NULL){
		rpc->soapFaultPtr->detail = faultDetailPtr;	
		faultDetailPtr->rpcFaultList = rpcFaultListHead;
	}
	
	return 0;

}

int cwmpCreateDetailFaultStruct(int faultCode, char* faultString)
{
	FaultDetailPtr  mFaultDetailPtr;

	/* this function may be call twice, at this situation, will caused memory leak!
	 * In order to avoid that case, it must make sure that faultDetailPtr is NULL
	 * taoliu, 2009.02.16
	 */
	if(faultDetailPtr!=NULL){
		return 0;
	}
	
	if((mFaultDetailPtr = (FaultDetailPtr)os_malloc(sizeof(FaultDetail))) == NULL){
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	memset(mFaultDetailPtr, 0, sizeof(FaultDetail));	
	mFaultDetailPtr->faultCode = faultCode;
	if((mFaultDetailPtr->faultString = (char*)os_malloc(strlen(faultString)+1)) == NULL){
		os_free(mFaultDetailPtr);
		mFaultDetailPtr = NULL;
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}		
	strcpy(mFaultDetailPtr->faultString, faultString);
	mFaultDetailPtr->rpcFaultList = NULL;
	
	faultDetailPtr = mFaultDetailPtr;				
	return 0;
	
}

int cwmpAddSetParameterValuesFaultList(char* parameterName, int faultCode, char* faultString)
{
	RPCFaultPtr nRpcFaultPtr;
	char * rpcErrorType = "SetParameterValuesFault";
	char * deailfaultString = "Invalid arguments";

	if(faultDetailPtr == NULL){
		if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && faultCode == SimCardAuthenticationFail){
			if(cwmpCreateDetailFaultStruct(faultCode, faultString) == ResourceExceed)
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}
		else if( TCSUPPORT_CT_CWMP_WORK_COMPATIBLE_VAL && faultCode == InvalidParameterValue){
			if(cwmpCreateDetailFaultStruct(faultCode, deailfaultString) == ResourceExceed)
				return ResourceExceed;
		}
		else{	
		if(cwmpCreateDetailFaultStruct(InvalidArgument, deailfaultString) == ResourceExceed)
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}
	}
		
	if((nRpcFaultPtr = (RPCFaultPtr)os_malloc(sizeof(RPCFault))) == NULL){
		goto Error_Handle;
	}
	memset(nRpcFaultPtr, 0, sizeof(RPCFault));	
	if((nRpcFaultPtr->rpcErrorType  = (char*)os_malloc(strlen(rpcErrorType)+1)) == NULL){
		goto Error_Handle;
	}
	if((nRpcFaultPtr->parameterName  = (char*)os_malloc(strlen(parameterName)+1)) == NULL){
		goto Error_Handle;
	}
	if((nRpcFaultPtr->faultString  = (char*)os_malloc(strlen(faultString)+1)) == NULL){
		goto Error_Handle;
	}
	strcpy(nRpcFaultPtr->rpcErrorType, rpcErrorType);
	strcpy(nRpcFaultPtr->parameterName, parameterName);
	strcpy(nRpcFaultPtr->faultString, faultString);	
	nRpcFaultPtr->faultCode = faultCode;
	nRpcFaultPtr->next = NULL;

	if(rpcFaultListHead == NULL){
		rpcFaultListHead = rpcFaultListTail = nRpcFaultPtr;
	}else{
		rpcFaultListTail->next = nRpcFaultPtr;
		rpcFaultListTail = nRpcFaultPtr;
	}
		
	return 0;

Error_Handle:

#if 0
	if(nRpcFaultPtr->rpcErrorType){
		os_free(nRpcFaultPtr->rpcErrorType);
		nRpcFaultPtr->rpcErrorType= NULL;
	}
	if(nRpcFaultPtr->parameterName){
		os_free(nRpcFaultPtr->parameterName);	
		nRpcFaultPtr->parameterName = NULL;
	}
	if(nRpcFaultPtr->faultString){
		os_free(nRpcFaultPtr->faultString);		
		nRpcFaultPtr->faultString = NULL;
	}
	if(nRpcFaultPtr){
		os_free(nRpcFaultPtr);	
		nRpcFaultPtr = NULL;
	}
#else
	freeRPCFaultStruct(&nRpcFaultPtr);
#endif
	
	return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	
}

int cwmpBuildFaultStruct(int* errorCode)
{
	FaultCode_String_Map* tempMapPtr = &FaultCode_String_MapTable[0];
	int  err_status = CWMP_E_SUCCESS;
	int tr69ErrCode = CWMP_E_SUCCESS;

	if(TCSUPPORT_CT_UPNP_DM_VAL){
		tr69ErrCode = getTR69ErrCodeByUPnPDM_Open();
		if(tr69ErrCode){
			tcdbg_printf("%s :tr69ErrCode = %d,*errorCode=%d\n",__FUNCTION__,tr69ErrCode,*errorCode);
			*errorCode = tr69ErrCode;
		}
	}

	switch(*errorCode){
		case ACS_Error:
			err_status = ACS_Error;
			break;
		case ExceptError:
			err_status = InternalError;//replace ExceptError by InternalError,hqfeng,2006.06.21
			*errorCode = InternalError;//If we get an ExceptError, return InternalError.
		case InternalError:
		case InvalidArgument:
		case ResourceExceed:
//		case SetReadOnlyParameter://The fault struct has been built in cwmpCheckDeviceNode(), taoliu added on 2008.12.26
//		case RejectRequestNotification:
		case DownloadFailure:
		case UploadFailure:
		case FileTransferAuthenFailure:
		case UnsupportedProtocol:
		case NoGetValueFunction:
		case NoGetAttributeFunction:
		case WriteSimCardFail:
		case SimCardAuthenticationFail:
			while(tempMapPtr->faultCode != 0){
				if(*errorCode == tempMapPtr->faultCode){
					if((tempMapPtr->faultString!= NULL) 
						&& (faultDetailPtr == NULL) ){//maybe faultDetailPtr has already exist. add to void memery leak. taoliu added on 2008.12.26
						err_status = cwmpCreateDetailFaultStruct(*errorCode, tempMapPtr->faultString);
					}
					break;
				}
				tempMapPtr++;
			}					
			break;			
		case RequestDeny:
		case MethodNotSupport:
		case InvalidParameterName:
		case InvalidParameterType:
		case InvalidParameterValue:
		case InvalidRequestUsername:
		case RejectRequestNotification://The fault struct has been built in cwmpSetDeviceNodeAttribute(), hqfeng,2006.09.30
		default:
			break;			

	}
	cwmpDealWithFaultCode(*errorCode);
	return err_status;
}

/*
 * recursive search a node in a tree
*/
/*
int searchNodeInTree(DeviceNodePtr deviceNode, DeviceNodePtr nodeTree)
{
	DeviceNodePtr child = NULL;
	DeviceNodePtr sibling = NULL;
	int ret = -1;
	
	if(deviceNode == NULL)
		return -1;
	if(nodeTree == NULL)
		return -1;

	if(deviceNode == nodeTree) //if find, return 0
		return 0;

	child = nodeTree->firstChild;
	if(child != NULL){
		ret = searchNodeInTree(deviceNode, child);
		if(ret == 0)
			return ret;
	}
	
	sibling = nodeTree->nextSibling;
	if(sibling != NULL){
		ret = searchNodeInTree(deviceNode, sibling);
		if(ret == 0)
			return ret;
	}
	
	return ret;
}
*/

int cwmpGetDeviceNodeName(DeviceNodePtr deviceNode, char* deviceName){
	DeviceNodePtr tempDeviceNode = deviceNode;
	char subNodeName[PARAMETER_NUM][PARAMETER_NAME_LEN];
	int8 i = 0, j = 0;
	
	if(deviceNode == NULL){
		tc_printf("The device node is NULL and get name fails!\r\n");
		return -1;
	}
	if(deviceName == NULL){
		tc_printf("The Pointer for store device node name is NULL!\r\n");
		return -1;
	}

//	if(searchNodeInTree(deviceNode, rpc.IGDDeviceRootPtr) != 0){
//		tc_printf("\r\n ****************find dirty deviceNode, avoid crash!!!!**********");
//		return -1;
//	}
	memset(deviceName, 0, strlen(deviceName)+1);

	while(tempDeviceNode != rpc.IGDDeviceRootPtr && tempDeviceNode->parentNode != NULL){
		strcpy(subNodeName[i], tempDeviceNode->name);
		tempDeviceNode = tempDeviceNode->parentNode;
		i++;
		if ( NULL == tempDeviceNode->parentNode && NULL == rpc.IGDDeviceRootPtr )
			tcdbg_printf("[%s]: rpc.IGDDeviceRootPtr is NULL and current parentNode is NULL \n", __FUNCTION__);
	}
	strcpy(subNodeName[i], tempDeviceNode->name);

	for(j = i; j >= 0; j--){
		strcat(deviceName,subNodeName[j]);
		if(j != 0)
			strcat(deviceName,".");
	}

	return 0;
}

uint8 cwmpClearNextLevelFlag(){
	nextLevelFalg = 0;
	
	return 0;
}
uint8 cwmpSetNextLevelFlag(){
	nextLevelFalg = 1;

	return 0;

}
uint8 cwmpGetNextLevelFlag(){
	return nextLevelFalg;
}


uint8 cwmpSetRpcResponseFlag(cwmp_rpc_t *rpc){
		rpc->responseFlag = 1;

		return 0;
}	

uint8 cwmpGetRpcResponseFlag(cwmp_rpc_t *rpc){
		return rpc->responseFlag;
}
uint8 cwmpResetRpcResponseFlag(cwmp_rpc_t *rpc){
		rpc->responseFlag = 0;

		return 0;
}	

void cwmpSetNotifyFlag(uint8 value){
if (TCSUPPORT_CT_PON_GDV20_VAL)
{
	notifyFlag = value;
#ifdef THREAD_REDUCE	
	/*We will remove Value Change message when notifyFlag is 0*/
	message_handle();
#endif
}

else
{
#ifdef THREAD_REDUCE	
	/*We will remove Value Change message when notifyFlag is 0*/
	message_handle();
#endif
	notifyFlag = value;
}

}
uint8 cwmpGetNotifyFlag(void){
	return notifyFlag;
}

void cwmpSetSpSaveFlag(uint8 value){
	spSaveFlag = value;
}
uint8 cwmpGetSpSaveFlag(void){
	return spSaveFlag;
}


/******************************** Build fault or response Tree **************************************/
/*______________________________________________________________________________
**	cwmpBuildResponseTree
**
**	description:
**	 Build fault or response Tree
**
**	parameters:
**
**	return:
**	0: OK
**
**   
**____________________________________________________________________________*/
int cwmpBuildResponseTree(SoapInfoPtr responseStructPtr, IXML_Document* responseDoc){
	IXML_Node* tempNode = NULL;
	IXML_Node* curNode = NULL, *curAttrNode = NULL;
	IXML_Node* responseHeadNode = NULL, *responseBodyNode = NULL;
	uint8 beFirstNode = 1;
	int err_state = CWMP_E_SUCCESS;

	if(responseStructPtr == NULL){
		tc_printf("The responseStructPtr is NULL!\r\n");
		return -1;
	}
	if(responseDoc == NULL){
		tc_printf("The responseDoc is NULL !\r\n");
		return -1;
	}
	//build envelope node.
	err_state = cwmpBuildEnvelopeNode(responseDoc);
	if(err_state == -1)
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	
	curNode = responseDoc->n.firstChild;
	
	//build head node.
	if(responseStructPtr->headStructPtr != NULL){
		if(cwmpCreatXMLNode(kCwmpHeaderTag, NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat head node fails!\r\n");
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}
		curNode->firstChild = tempNode;
		curNode = tempNode;
		responseHeadNode = curNode;
		beFirstNode = 0;

		if(cwmpBuildResponseHeadNode(responseStructPtr->headStructPtr, responseHeadNode) == -1){
			tc_printf("Build response head node fails!\r\n");
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}
	}
	
	//build body node.
	if((faultDetailPtr != NULL) && (rpc.soapFaultPtr != NULL)){
		rpc.soapFaultPtr->detail = faultDetailPtr;
		if(faultDetailPtr){
			faultDetailPtr->rpcFaultList = rpcFaultListHead;
		}
	}
	if(responseStructPtr->bodyStructPtr || faultDetailPtr){
	
		if(cwmpCreatXMLNode(kCwmpBodyTag, NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat body node fails!\r\n");
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}
		if(beFirstNode)
			curNode->firstChild = tempNode;
		else
			curNode->nextSibling = tempNode;
		curNode = tempNode;
		responseBodyNode = curNode;
		if(faultDetailPtr == NULL){
			if(cwmpBuildResponseBodyNode(responseStructPtr->bodyStructPtr, responseBodyNode) == -1){
				tc_printf("Build response body node fails!\r\n");
				return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
			}
		}else{
			//build fault tree.
	
			if(cwmpBuildFaultBody(rpc.soapFaultPtr, responseBodyNode) == -1){
				tc_printf("Building fault Tree fails!\r\n");
				return -1;
			}
		}
	}
	
	return 0;
}

int8 cwmpBuildEnvelopeNode(IXML_Document* responseDoc){
	IXML_Node* tempNode = NULL;
	IXML_Node* curNode = NULL, *curAttrNode = NULL;
		
	if(responseDoc == NULL){
		tc_printf("The responseDoc is NULL !\r\n");
		return -1;
	}

	//build envelope node.
	if(cwmpCreatXMLNode(kCwmpEnvelopeTag, NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat envelope node fails!\r\n");
		return -1;
	}
	responseDoc->n.firstChild = tempNode;
	curNode = tempNode;

	if(faultDetailPtr != NULL){
		if(cwmpCreatXMLNode(kCwmpXmlns, "http://schemas.xmlsoap.org/soap/envelope/", eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat envelope attribute node 1 fails!\r\n");
			return -1;
		}
		curNode->firstAttr = tempNode;
		curAttrNode = tempNode;
		if(cwmpCreatXMLNode("xmlns:cwmp", "urn:dslforum-org:cwmp-1-0", eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat envelope attribute node 4 fails!\r\n");
			return -1;
		}
		curAttrNode->nextSibling = tempNode;
		curAttrNode = tempNode;
		return 0;
		
	}
	
#if 1  //xzwang_20060716
	if(cwmpCreatXMLNode(kCwmpEncodingStyleAttribute, "http://schemas.xmlsoap.org/soap/encoding/", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 1 fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	curAttrNode = tempNode;

	if(cwmpCreatXMLNode(kCwmpXmlns, "http://schemas.xmlsoap.org/soap/envelope/", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 1 fails!\r\n");
		return -1;
	}
	curAttrNode->nextSibling = tempNode;
	curAttrNode = tempNode;
#else
	if(cwmpCreatXMLNode(kCwmpXmlns, "http://schemas.xmlsoap.org/soap/envelope/", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 1 fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	curAttrNode = tempNode;

#endif
	if(cwmpCreatXMLNode(kCwmpEncXmlns, "http://schemas.xmlsoap.org/soap/encoding/", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 2 fails!\r\n");
		return -1;
	}
	curAttrNode->nextSibling = tempNode;
	curAttrNode = tempNode;
	
	if(cwmpCreatXMLNode("xmlns:xsd", "http://www.w3.org/2001/XMLSchema", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 3 fails!\r\n");
		return -1;
	}
	curAttrNode->nextSibling = tempNode;
	curAttrNode = tempNode;

	if(cwmpCreatXMLNode("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 4 fails!\r\n");
		return -1;
	}
	curAttrNode->nextSibling = tempNode;
	curAttrNode = tempNode;
	
	if(cwmpCreatXMLNode("xmlns:cwmp", "urn:dslforum-org:cwmp-1-0", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat envelope attribute node 4 fails!\r\n");
		return -1;
	}
	curAttrNode->nextSibling = tempNode;
	curAttrNode = tempNode;

	return 0;
}


//build head node
int8 cwmpBuildResponseHeadNode(SoapHeadInfoPtr responseHeadStructPtr, IXML_Node* responseHeadNode){
	IXML_Node* tempNode = NULL;
	IXML_Node* curNode = NULL, *curAttrNode = NULL;

	if(responseHeadStructPtr == NULL){
		tc_printf("The responseHeadStructPtr is NULL!\r\n");
		return -1;
	}
	if(responseHeadNode == NULL){
		tc_printf("The responseHeadNode is NULL !\r\n");
		return -1;
	}

	curNode = responseHeadNode;
	//set id 
	if(strcmp(responseHeadStructPtr->id,"") != 0){
		if(cwmpCreatXMLNode("cwmp:ID", NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat head id node fails!\r\n");
			return -1;
		} 
		curNode->firstChild = tempNode;
		curNode = tempNode;

		if(cwmpCreatXMLNode(kCwmpEnvMustUnderStand, "1", eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat envelope attribute node 4 fails!\r\n");
			return -1;
		}
		curNode->firstAttr = tempNode;
		curAttrNode = tempNode;

		if(cwmpCreatXMLNode(NULL, responseHeadStructPtr->id, eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat head id node fails!\r\n");
			return -1;
		} 
		curNode->firstChild = tempNode;
	}
	//set noMoreRequests.
	if(strcmp(responseHeadStructPtr->noMoreRequest,"") != 0){
		if(cwmpCreatXMLNode("cwmp:NoMoreRequests", "", eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat head id node fails!\r\n");
			return -1;
		} 
		curNode->nextSibling= tempNode;
		curNode = tempNode;
		
		if(cwmpCreatXMLNode(NULL, responseHeadStructPtr->noMoreRequest, eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat head id node fails!\r\n");
			return -1;
		} 
		curNode->firstChild = tempNode;
	}

	return 0;
}


//build body node.
int cwmpBuildResponseBodyNode(SoapBodyInfoPtr responseBodyStructPtr, IXML_Node* responseBodyNode){
	IXML_Node* tempNode = NULL, *attrNode = NULL;
	IXML_Node* curNode = NULL, *curAttrNode = NULL, *oldCurNode = NULL;
	char *tempStr;
	int strLen;
	RPCArgumentPtr curArgument;
	char ArgumentAttributeValue[128];
	void *newParaList;

	if(responseBodyStructPtr == NULL){
		tc_printf("The responseBodyStructPtr is NULL!\r\n");
		return -1;
	}
	if(responseBodyNode == NULL){
		tc_printf("The responseBodyNode is NULL !\r\n");
		return -1;
	}
	
	curNode = responseBodyNode;
	
	//build rpc name node
	strLen = strlen("cwmp:") + strlen(responseBodyStructPtr->rpcName) + 2;
	if((tempStr = (char*)os_malloc(strLen)) != NULL){ 
		strcpy(tempStr, "cwmp:");
		strcat(tempStr, responseBodyStructPtr->rpcName);
	}
	else{
		tc_printf("malloc temp string for node name fails !\r\n");
		return -1;
	}
	if(cwmpCreatXMLNode(tempStr, NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat rpc name node fails!\r\n");
		os_free(tempStr);   //xzwang_060815
		tempStr = NULL;
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;
	if(tempStr != NULL){
	        os_free(tempStr);
		tempStr = NULL;
	}
	
	//build argument node
	curArgument = responseBodyStructPtr->argumentPtr;
	while(curArgument != NULL){
		//argument element;
		if(cwmpCreatXMLNode(curArgument->name, NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat argument node fails!\r\n");
			return -1;
		}
		if(oldCurNode)
			curNode->nextSibling = tempNode;
		else
			curNode->firstChild = tempNode;
	
		curNode = tempNode;
		oldCurNode = curNode; //save the node pointer for next argument.

		if(!curArgument->beArray){
	
			if(cwmpCreatArgumentValueNode(curArgument->value, curArgument->type, &tempNode, &attrNode) == -1){
				tc_printf("Creat argument value node fails!\r\n");
				return -1;
			}
			curNode->firstChild = tempNode;
			curNode->firstAttr = attrNode;
			goto NextArgument;
		}
		
		//argument attribute;
		switch(curArgument->type){
			case StringType:
			case String256Type:
				sprintf(ArgumentAttributeValue,"xsd:string[%d]",curArgument->number);
				break;
			case IntType:
				sprintf(ArgumentAttributeValue,"xsd:int[%d]",curArgument->number);
				break;
			case UnsignedIntType:
				sprintf(ArgumentAttributeValue,"xsd:unsingedInt[%d]",curArgument->number);
				break;
			case BooleanType:
				sprintf(ArgumentAttributeValue,"xsd:boolean[%d]",curArgument->number);
				break;
			case DateTimeType:
				sprintf(ArgumentAttributeValue,"xsd:dateTime[%d]",curArgument->number);
				break;
			case Base64Type:
				sprintf(ArgumentAttributeValue,"xsd:base64[%d]",curArgument->number);
				break;
			case ParameterValueStructType:
				sprintf(ArgumentAttributeValue,"cwmp:ParameterValueStruct[%d]",curArgument->number);
				break;
			case ParameterInfoStructType:
				sprintf(ArgumentAttributeValue,"cwmp:ParameterInfoStruct[%d]",curArgument->number);
				break;
			case SetParameterAttributesStructType:
				sprintf(ArgumentAttributeValue,"cwmp:SetParameterAttributesStruct[%d]",curArgument->number);
				break;
			case ParameterAttributeStructType:
				sprintf(ArgumentAttributeValue,"cwmp:ParameterAttributeStruct[%d]",curArgument->number);
				break;
			case EventStructType:
				sprintf(ArgumentAttributeValue,"cwmp:EventStruct[%d]",curArgument->number);
				break;				
			default:
				tc_printf("Invalid argument type for build attribute node!\r\n");
				return -1;
		}
		if(cwmpCreatXMLNode("xsi:type", kCwmpEncArray,  eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat rpc argument first attribute node fails!\r\n");
			return -1;
		}
		curNode->firstAttr = tempNode;
		curAttrNode = tempNode;
		
		if(cwmpCreatXMLNode(kCwmpEncArrayType, ArgumentAttributeValue,  eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat rpc argument second attribute node fails!\r\n");
			return -1;
		}
		curAttrNode->nextSibling= tempNode;
		//set all parameter node under this argument.
		if(curArgument->number){
			if(cwmpSetBodyParaNode(curArgument->parameterList,curArgument->type, curNode, &newParaList) == -1){
				curArgument->parameterList = newParaList;  //free parameter immediately for reducing memory occupation. xzwang_20061124
				return -1;
		}
			curArgument->parameterList = NULL; //free parameter immediately for reducing memory occupation. xzwang_20061124
		}
	
	NextArgument:	
		curArgument = curArgument->next;
	}

	return 0;
}

int cwmpBuildFaultBody(FaultStructPtr soapFaultPtr, IXML_Node* responseBodyNode){
	IXML_Node* tempNode = NULL;
	IXML_Node* tempNode1 = NULL;
	IXML_Node* curNode = NULL, *curAttrNode = NULL, *oldCurNode = NULL;
//	char *tempStr;
//	int strLen;
	RPCFaultPtr curRPCFault;
	char faultCode[5];
	
	if(soapFaultPtr == NULL){
		tc_printf("The soapFaultPtr is NULL!\r\n");
		return -1;
	}
	if(responseBodyNode == NULL){
		tc_printf("The responseBodyNode is NULL !\r\n");
		return -1;
	}

	curNode = responseBodyNode;
	
	//build soap:Fault node
	if(cwmpCreatXMLNode(kCwmpFaultTag, NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat soap:Fault node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;

	//Build the faultcode and faultstring.
	if(cwmpCreatXMLNode( "faultcode", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat faultcode node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;
	oldCurNode = curNode; //save the node pointer for faultstring.
	if(cwmpCreatXMLNode(NULL, soapFaultPtr->faultcode, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat faultcode text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = oldCurNode;
	if(cwmpCreatXMLNode( "faultstring", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat faultstring node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;	
	oldCurNode = curNode; //save the node pointer for detail
	if(cwmpCreatXMLNode(NULL, soapFaultPtr->faultstring, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat faultstring text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = oldCurNode;

	//Build detail
	if(cwmpCreatXMLNode( "detail", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat detail node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;	

	//Build cwmp:Fault
	if(cwmpCreatXMLNode( "cwmp:Fault", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat faultstring node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;	

	//Build FaultCode and FaultString
	if(cwmpCreatXMLNode( "FaultCode", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat FaultCode node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;	
	oldCurNode = curNode; //save the node pointer for FaultString
	sprintf(faultCode, "%d", soapFaultPtr->detail->faultCode);
	if(cwmpCreatXMLNode(NULL, faultCode, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat FaultCode text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = oldCurNode;
	if(cwmpCreatXMLNode( "FaultString", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat FaultCode node fails!\r\n");
		return -1;
	}
	curNode->nextSibling= tempNode;
	curNode = tempNode;
	oldCurNode = curNode; //save the node pointer for RPCFault
	if(cwmpCreatXMLNode(NULL, soapFaultPtr->detail->faultString, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat FaultString text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = oldCurNode;	

	//Build RPCFault
	curRPCFault = soapFaultPtr->detail->rpcFaultList;
	while(curRPCFault != NULL){
		if(cwmpCreatXMLNode( curRPCFault->rpcErrorType, NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat rpcErrorType node fails!\r\n");
			return -1;
		}
		curNode->nextSibling = tempNode;
		curNode = tempNode;
		oldCurNode = curNode; //save the node pointer for next RPCFault
		//Build ParameterName
		if(cwmpCreatXMLNode( "ParameterName", NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat ParameterName node fails!\r\n");
			return -1;
		}		
		curNode->firstChild= tempNode;
		curNode = tempNode;
		tempNode1 = curNode; //save the node pointer for FaultCode
		if(cwmpCreatXMLNode(NULL, curRPCFault->parameterName, eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat ParameterName text node fails!\r\n");
			return -1;
		}
		curNode->firstChild = tempNode;
		curNode = tempNode1;
		//Build FaultCode
		if(cwmpCreatXMLNode( "FaultCode", NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat FaultCode node fails!\r\n");
			return -1;
		}		
		curNode->nextSibling = tempNode;
		curNode = tempNode;
		tempNode1 = curNode; //save the node pointer for FaultString		
		sprintf(faultCode, "%d",curRPCFault->faultCode);
		if(cwmpCreatXMLNode(NULL, faultCode, eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat ParameterName text node fails!\r\n");
			return -1;
		}
		curNode->firstChild = tempNode;
		curNode = tempNode1;
		//Build FaultString
		if(cwmpCreatXMLNode( "FaultString", NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat FaultString node fails!\r\n");
			return -1;
		}		
		curNode->nextSibling = tempNode;
		curNode = tempNode;
		if(cwmpCreatXMLNode(NULL, curRPCFault->faultString, eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat FaultString text node fails!\r\n");
			return -1;
		}
		curNode->firstChild = tempNode;	
		
		//For next RPCFault
		curNode = oldCurNode;
		curRPCFault = curRPCFault->next;
	}	
		
	return 0;
}


int cwmpCreatArgumentValueNode(void* argValue, ParameterType argType, IXML_Node** argValueNode, IXML_Node** argAttrNode){
	IXML_Node* tempNode = NULL, *attrNode = NULL;
	IXML_Node* curNode = NULL, *rootNode = NULL;

	if(argValue == NULL){
		tc_printf("The argument value is NULL!\r\n" );
		return 0;
	}
	
	switch(argType){
		case StringType:
		case String256Type:
			if(cwmpCreatXMLNode(NULL, (char*)argValue, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
		#ifdef EXPLICTDEFINETYPE
			if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &attrNode) == -1){
				tc_printf("Creat string argument attribute node fails!\r\n");
				goto Error_Handler;
			}
		#endif	
			break;
		case DateTimeType:
			if(cwmpCreatXMLNode(NULL, (char*)argValue, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
		#ifdef EXPLICTDEFINETYPE
			if(cwmpCreatXMLNode("xsi:type", "xsd:dateTime", eATTRIBUTE_NODE, &attrNode) == -1){
				tc_printf("Creat dateTime argument attribute node fails!\r\n");
				goto Error_Handler;
			}
		#endif	
			break;
		case Base64Type:	
			if(cwmpCreatXMLNode(NULL, (char*)argValue, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
		#ifdef EXPLICTDEFINETYPE
			if(cwmpCreatXMLNode("xsi:type", "xsd:base64", eATTRIBUTE_NODE, &attrNode) == -1){
				tc_printf("Creat base64 argument attribute node fails!\r\n");
				goto Error_Handler;
			}
		#endif		
			break;
		case IntType:
		{
			int* tempValue = (int*)argValue;
			char tempStr[32]; 

			cwmpItoa(*tempValue, tempStr, 10);
			if(cwmpCreatXMLNode(NULL, tempStr, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
		#ifdef EXPLICTDEFINETYPE
			if(cwmpCreatXMLNode("xsi:type", "xsd:int", eATTRIBUTE_NODE, &attrNode) == -1){
				tc_printf("Creat int argument attribute node fails!\r\n");
				goto Error_Handler;
			}
		#endif		
			break;
		}
		case UnsignedIntType:
		{
			unsigned int* tempValue = (unsigned int*)argValue;
			char tempStr[32]; 

			cwmpItoa((int)*tempValue, tempStr, 10);
			if(cwmpCreatXMLNode(NULL, tempStr, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");

				goto Error_Handler;
			}
			rootNode = tempNode;
		#ifdef EXPLICTDEFINETYPE
			if(cwmpCreatXMLNode("xsi:type", "xsd:unsignedInt", eATTRIBUTE_NODE, &attrNode) == -1){
				tc_printf("Creat unsignedInt argument attribute node fails!\r\n");
				goto Error_Handler;
			}
		#endif			
			break;
		}	
		case BooleanType:
		{

			unsigned int* tempValue = (unsigned int*)argValue;
			char tempStr[32]; 

			cwmpItoa((int)*tempValue, tempStr, 10);
			if(cwmpCreatXMLNode(NULL, tempStr, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
		#ifdef EXPLICTDEFINETYPE
			if(cwmpCreatXMLNode("xsi:type", "xsd:boolean", eATTRIBUTE_NODE, &attrNode) == -1){
				tc_printf("Creat boolean argument attribute node fails!\r\n");
				goto Error_Handler;
			}
		#endif		
			break;
		}
		case DeviceIdStructType:
		{
			DeviceIdStructPtr tempDeviceId = (DeviceIdStructPtr)argValue;
			
			if(cwmpCreatXMLNode("Manufacturer", NULL, eELEMENT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
			curNode = tempNode;
	#ifdef EXPLICTDEFINETYPE
			//Manufacturer attribute node
			if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
				tc_printf("Creat Manufacturer attribute node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstAttr = tempNode;
	#endif	
			if(cwmpCreatXMLNode(NULL, tempDeviceId->manufacture, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				return -1;
			}
			curNode->firstChild = tempNode;

			if(cwmpCreatXMLNode("OUI", NULL, eELEMENT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->nextSibling= tempNode;
			curNode = tempNode;
	#ifdef EXPLICTDEFINETYPE
			//OUI attribute node
			if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
				tc_printf("Creat OUI attribute node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstAttr = tempNode;
	#endif				
			if(cwmpCreatXMLNode(NULL, tempDeviceId->OUI, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstChild = tempNode;

			if(cwmpCreatXMLNode("ProductClass", NULL, eELEMENT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->nextSibling= tempNode;
			curNode = tempNode;
	#ifdef EXPLICTDEFINETYPE
			//ProductClass attribute node
			if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
				tc_printf("Creat ProductClass attribute node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstAttr = tempNode;
	#endif				
		
			if(cwmpCreatXMLNode(NULL, tempDeviceId->productClass, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstChild = tempNode;
			//serialNumber
			if(cwmpCreatXMLNode("SerialNumber", NULL, eELEMENT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->nextSibling= tempNode;
			curNode = tempNode;
	#ifdef EXPLICTDEFINETYPE
			//serialNumber attribute node
			if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
				tc_printf("Creat SerialNumber attribute node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstAttr = tempNode;
	#endif					
			if(cwmpCreatXMLNode(NULL, tempDeviceId->serialNumber, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstChild = tempNode;

			break;
		}
		case FaultDetailType:
		{
			FaultDetailPtr tempFault = (FaultDetailPtr)argValue;
			char tempStr[32]; 

			cwmpItoa(tempFault->faultCode, tempStr, 10);
			//FaultCode
			if(cwmpCreatXMLNode("FaultCode", NULL, eELEMENT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			rootNode = tempNode;
			curNode = tempNode;
	
			if(cwmpCreatXMLNode(NULL, tempStr, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstChild = tempNode;
			//FaultString
			if(cwmpCreatXMLNode("FaultString", NULL, eELEMENT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->nextSibling= tempNode;
			curNode = tempNode;
			
			if(cwmpCreatXMLNode(NULL, tempFault->faultString, eTEXT_NODE, &tempNode) == -1){
				tc_printf("Creat xml node fails!\r\n");
				goto Error_Handler;
			}
			curNode->firstChild = tempNode;
			
			break;
		}
		default:
			tc_printf("Error argument type!\r\n");
			goto Error_Handler;
			
			break;
	}

	*argValueNode = rootNode;
	*argAttrNode = attrNode;
	
	return 0;

Error_Handler:

	if(rootNode != NULL){
		ixmlNode_free_new(rootNode);
		rootNode = NULL;
	}
	if(attrNode != NULL){
		ixmlNode_free_new(attrNode);
		attrNode = NULL;
	}
	
	return -1;
}
//build parameter list.
int8 cwmpSetBodyParaNode(void* parameterList, ParameterType argType, IXML_Node* bodyParaNode, void** newParaListP){
	IXML_Node* curNode = bodyParaNode;
	IXML_Node*	tempNode = NULL, *oldCurNode = NULL;
	void* curPara = parameterList, *nextPara = NULL;
	char* typeString;
	mbuf_t* tempMbuf = NULL;

	if(parameterList == NULL){
		*newParaListP = curPara;
		tc_printf("The response parameter list is NULL!\r\n");
		return -1;
	}
	if(bodyParaNode == NULL){
		*newParaListP = curPara;
		tc_printf("The response parameter Node is NULL !\r\n");
		return -1;
	}
	while(curPara){
	//	oldCurNode = curNode; 
		switch(argType){
			case StringType:
			case String256Type:
			{
				StringStructPtr tempCurPara = (StringStructPtr)curPara;
	
				typeString = "string";
				if(cwmpCreateBasicParameterNode(tempCurPara, typeString,curNode,&oldCurNode) == -1){
					*newParaListP = curPara;
					tc_printf("Create string parameter node fails!\r\n");
					return -1;
				}
				
				//curPara = tempCurPara->next;
				break;
			}
			case IntType:	
			case UnsignedIntType:	
			case BooleanType:	
			case Base64Type:
			case DateTimeType:
				 tc_printf("Invalid parameter type!\r\n");
				 break;
			case ParameterValueStructType:
			{
				ParameterValueStructPtr tempCurPara = (ParameterValueStructPtr)curPara;

				if(cwmpCreateParaValueNode(tempCurPara, curNode, &oldCurNode) == -1){
					*newParaListP = curPara;
					tc_printf("Create parameter value node fails!\r\n");
					return -1;
				}
				
				//curPara = tempCurPara->next;
				break;
			}
			case ParameterInfoStructType:
			{
				ParameterInfoStructPtr tempCurPara = (ParameterInfoStructPtr)curPara;

				if(cwmpCreateParaInfoNode(tempCurPara, curNode, &oldCurNode) == -1){
					*newParaListP = curPara;
					tc_printf("Create parameter info node fails!\r\n");
					return -1;
				}
				
				//curPara = tempCurPara->next;
				break;
			}
			case ParameterAttributeStructType:
			{
				ParameterAttributeStructPtr tempCurPara = (ParameterAttributeStructPtr)curPara;
				
				if(cwmpCreateParaAttributeNode(tempCurPara, curNode, &oldCurNode) == -1){
					*newParaListP = curPara;
					tc_printf("Create parameter attribute node fails!\r\n");
					return -1;
				}
				
				//curPara = tempCurPara->next;
				break;
			}
			case EventStructType:
			{
				EventStructPtr tempCurPara = (EventStructPtr)curPara;
				
				if(cwmpCreateEventNode(tempCurPara, curNode, &oldCurNode) == -1){
					*newParaListP = curPara;
					tc_printf("Create parameter attribute node fails!\r\n");
					return -1;
				}

				//curPara = tempCurPara->next;
				break;
			}
			default:
				tc_printf("Invalid parameter type for response!\r\n");
				break;
		}	
		//free parameter immediately for reducing memory occupation. xzwang_20061124
		cwmpFreeParameter(curPara, argType, &nextPara);
		curPara = nextPara;
	
#ifndef CWMP_FREE_PARA_XML_NODE_IMMEDIATELY
		if(curPara)
			curNode = oldCurNode;
#else
		if(argType == ParameterValueStructType || argType == ParameterInfoStructType 
                   || argType == ParameterAttributeStructType){
			cwmpFrameTreeToBuf((IXML_Document *)oldCurNode, &tempMbuf);
			//cwmpDumpMbufData();
	
			cwmpReleaseSoapDoc_new((IXML_Document *)oldCurNode);
	
			if(tempMbuf == NULL){
				tc_printf("Frame parameter node into mbuf fails!\r\n");
				*newParaListP = curPara;
				return -1;
			}
		}
		else{
		if(curPara)
			curNode = oldCurNode;
		}
#endif
	}

#ifdef CWMP_FREE_PARA_XML_NODE_IMMEDIATELY
	if(argType == ParameterValueStructType || argType == ParameterInfoStructType 
           || argType == ParameterAttributeStructType){
		if(cwmpCreatXMLNode(NULL, "ParaListIsInMbuf", eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat parameterValueStruct value text node fails!\r\n");
			return -1;
		}
		curNode->firstChild = tempNode;
		
		cwmpSetParaMbufList();
	}		
#endif
	return 0;
}


int8 cwmpCreateBasicParameterNode(void* parameter, char* typeString, IXML_Node* paraNode, IXML_Node** oldParaNode){
	IXML_Node* tempNode = NULL;
	IXML_Node* curNode = paraNode, *curAttrNode = NULL;
	char tempStr[256];
	
	//element node.
	if(cwmpCreatXMLNode(typeString, NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat rpc name node fails!\r\n");
		return -1;
	}
	if(*oldParaNode)
		curNode->nextSibling = tempNode;
	else
		curNode->firstChild = tempNode;

	curNode = tempNode;
	*oldParaNode = curNode;

	//attribute node.
	memset(tempStr, 0, 256);
	strcpy(tempStr, "xsd:");
	strcat(tempStr, typeString);
	if(cwmpCreatXMLNode("xsi:type", tempStr, eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat rpc name node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	
	//text node
	memset(tempStr, 0, 256);
	if(strcmp(typeString, "string") == 0){
		StringStructPtr tempCurPara = (StringStructPtr)parameter;
		strcpy(tempStr,tempCurPara->value);
	}
	else if(strcmp(typeString, "int") == 0){
		IntStructPtr tempCurPara = (IntStructPtr)parameter;
		cwmpItoa(tempCurPara->value, tempStr,10);
	}
	else if(strcmp(typeString, "unsignedInt") == 0){
		UnsignedIntStructPtr tempCurPara = (UnsignedIntStructPtr)parameter;
		cwmpItoa((int)tempCurPara->value, tempStr,10);
	}
	else if(strcmp(typeString, "boolean") == 0){
		BooleanStructPtr tempCurPara = (BooleanStructPtr)parameter;
		cwmpItoa((int)tempCurPara->value, tempStr,10);
	}
	else if(strcmp(typeString, "dateTime") == 0){
		DateTimeStructPtr tempCurPara = (DateTimeStructPtr)parameter;
		tc_printf("Need further handle for datetime!\r\n");
		//here need furter handle
		//cwmpItoa(tempCurPara->value, tempStr,10);
	}
	else if(strcmp(typeString, "base64") == 0){
		Base64StructPtr tempCurPara = (Base64StructPtr)parameter;
		tc_printf("Need further handle for base64!\r\n");
		//here need furter handle
		//cwmpItoa(tempCurPara->value, tempStr,10);	
	}
	else{
		tc_printf("Unkown parameter type!\r\n");
		return -1;
	}
	
	if(cwmpCreatXMLNode(NULL, tempStr, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat rpc name node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;

	return 0;

}	

int8 cwmpCreateParaValueNode(ParameterValueStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP){
	IXML_Node*	tempNode = NULL;
	char* tempStr = NULL;
	
	if(cwmpCreatXMLNode("ParameterValueStruct", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct node fails!\r\n");
		return -1;
	}

#ifndef CWMP_FREE_PARA_XML_NODE_IMMEDIATELY
	if(*oldCurNodeP)
		curNode->nextSibling = tempNode;
	else
		curNode->firstChild = tempNode;
#endif	
	curNode = tempNode;
	*oldCurNodeP = curNode;   //save the node pointer for next parameter

#ifdef EXPLICTDEFINETYPE
	//attribute	
	if(cwmpCreatXMLNode("xsi:type", "cwmp:ParameterValueStruct",  eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat rpc  parameterValueStruct attribute node fails!\r\n");
			return -1;
	}
	curNode->firstAttr = tempNode;
#endif	
	
	//Name node
	if(cwmpCreatXMLNode("Name", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct name node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;
	
#ifdef EXPLICTDEFINETYPE
	//Name attribute node
	if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct name attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
#endif	

	//Name text node
	if(cwmpCreatXMLNode(NULL, curPara->name, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct name text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	
	//Value node.
	if(cwmpCreatXMLNode("Value", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct value node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;

	//if value is NULL or value type is NoType, donot build value text node and return directly. xzwang_060720
	if(curPara->valueInfoPtr->value == NULL ||curPara->valueInfoPtr->type == NoType){
		tc_printf("Without value for the parameter!\r\n");
		return 0;
	}

	//Value attribute node.
	switch(curPara->valueInfoPtr->type){
		case StringType:
		case String256Type:
			tempStr = "xsd:string";
			break;
		case IntType:	
			tempStr = "xsd:int";
			break;
		case UnsignedIntType:	
			tempStr = "xsd:unsignedInt";
			break;
		case BooleanType:	
			tempStr = "xsd:boolean";
			break;
		case DateTimeType:
			tempStr = "xsd:dateTime";
			break;
		case Base64Type:
			tempStr = "xsd:base64";
		default:
			tc_printf("Unknown parameter value type!\r\n");
			break;

	}		
	if(cwmpCreatXMLNode("xsi:type", tempStr, eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct value attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	//Value text node.
	if(cwmpCreatXMLNode(NULL, curPara->valueInfoPtr->value, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct value text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;

	return 0;

}

int8 cwmpCreateParaInfoNode(ParameterInfoStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP){
	IXML_Node*	tempNode = NULL;
	char* tempStr = NULL;
	char tempStr2[4];
				
	if(cwmpCreatXMLNode("ParameterInfoStruct", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterInfoStruct node fails!\r\n");
		return -1;
	}
#ifndef CWMP_FREE_PARA_XML_NODE_IMMEDIATELY
	if(*oldCurNodeP)
		curNode->nextSibling = tempNode;
	else
		curNode->firstChild = tempNode;
#endif
	
	curNode = tempNode;
	*oldCurNodeP= curNode;   //save the node pointer for next parameter
	//Name node
	if(cwmpCreatXMLNode("Name", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterInfoStruct name node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;
	//Name text node
	if(cwmpCreatXMLNode(NULL, curPara->name, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterInfoStruct name text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	
	//Writable node.
	if(cwmpCreatXMLNode("Writable", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterInfoStruct Writable node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;
	//Writable attribute node.
	tempStr = "xsd:boolean";
	if(cwmpCreatXMLNode("xsi:type", tempStr, eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterInfoStruct Writable attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	//Value text node.
	cwmpItoa(curPara->writable,tempStr2,10);
	if(cwmpCreatXMLNode(NULL, tempStr2, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterInfoStruct Writable text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;

	return 0;
}

int8 cwmpCreateParaAttributeNode(ParameterAttributeStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP){
	IXML_Node*	tempNode = NULL;
	char* tempStr = NULL;
	char tempStr2[32];
	uint8 aclNum = 0;
	AccessEntry* tempAcl = NULL;
	IXML_Node* tempCurNode = NULL;

	if(cwmpCreatXMLNode("ParameterAttributeStruct", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct node fails!\r\n");
		return -1;
	}
#ifndef CWMP_FREE_PARA_XML_NODE_IMMEDIATELY
	if(*oldCurNodeP)
		curNode->nextSibling = tempNode;
	else
		curNode->firstChild = tempNode;
#endif	
	curNode = tempNode;
	*oldCurNodeP = curNode;   //save the node pointer for next parameter
	//Name node
	if(cwmpCreatXMLNode("Name", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct name node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;
	//Name text node
	if(cwmpCreatXMLNode(NULL, curPara->name, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct name text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	
	//Notifcation node.
	if(cwmpCreatXMLNode("Notification", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct Notification node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;
	//Notification attribute node.
	tempStr = "xsd:int";
	if(cwmpCreatXMLNode("xsi:type", tempStr, eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct Notification attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	//Notification text node.
	cwmpItoa(curPara->notification,tempStr2,10);
	if(cwmpCreatXMLNode(NULL, tempStr2, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct Notification text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;

	//AccessList node.
	if(cwmpCreatXMLNode("AccessList", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct AccessList node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;
	//AccessList attribute node.
	tempAcl = curPara->accessList; 
	while(tempAcl){
		aclNum++;
		tempAcl = tempAcl->next;
	}
	sprintf(tempStr2,"xsd:string[%d]", aclNum);
	if(cwmpCreatXMLNode(kCwmpEncArrayType, tempStr2, eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat ParameterAttributeStruct AccessList attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
	//AccessList text node.
	tempAcl = curPara->accessList; 
	while(tempAcl){
		if(cwmpCreatXMLNode("string", NULL, eELEMENT_NODE, &tempNode) == -1){
			tc_printf("Creat ParameterAttributeStruct AccessList text node fails!\r\n");
			return -1;
		}
		if(tempCurNode)
			curNode->nextSibling= tempNode;
		else
			curNode->firstChild = tempNode;
		
		curNode = tempNode;
		tempCurNode = curNode;

		if(cwmpCreatXMLNode(NULL, tempAcl->entryName, eTEXT_NODE, &tempNode) == -1){
			tc_printf("Creat ParameterAttributeStruct AccessList text node fails!\r\n");
			return -1;
		}
		curNode->firstChild = tempNode;
		
		tempAcl = tempAcl->next;
	}

	return 0;
}

int8 cwmpCreateEventNode(EventStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP){
	IXML_Node*	tempNode = NULL;
				
	if(cwmpCreatXMLNode("EventStruct", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct node fails!\r\n");
		return -1;
	}
	if(*oldCurNodeP)
		curNode->nextSibling = tempNode;
	else
		curNode->firstChild = tempNode;
	
	curNode = tempNode;
	*oldCurNodeP= curNode;   //save the node pointer for next parameter

#ifdef EXPLICTDEFINETYPE
	//attribute	
	if(cwmpCreatXMLNode("xsi:type", "cwmp:EventStruct",  eATTRIBUTE_NODE, &tempNode) == -1){
			tc_printf("Creat rpc  EventStruct attribute node fails!\r\n");
			return -1;
	}
	curNode->firstAttr = tempNode;
#endif	
	//EventCode node
	if(cwmpCreatXMLNode("EventCode", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct name node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	curNode = tempNode;

#ifdef EXPLICTDEFINETYPE
	//EventCode attribute node
	if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat Event code attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
#endif	
	//EventCode  text node
	if(cwmpCreatXMLNode(NULL, curPara->eventCode, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct name text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;
	
	//CommandKey node.
	if(cwmpCreatXMLNode("CommandKey", NULL, eELEMENT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct value node fails!\r\n");
		return -1;
	}
	curNode->nextSibling = tempNode;
	curNode = tempNode;

#ifdef EXPLICTDEFINETYPE
	//CommandKey attribute node
	if(cwmpCreatXMLNode("xsi:type", "xsd:string", eATTRIBUTE_NODE, &tempNode) == -1){
		tc_printf("Creat CommandKey attribute node fails!\r\n");
		return -1;
	}
	curNode->firstAttr = tempNode;
#endif	
	//CommandKey text node.
	if(cwmpCreatXMLNode(NULL, curPara->commandKey, eTEXT_NODE, &tempNode) == -1){
		tc_printf("Creat parameterValueStruct value text node fails!\r\n");
		return -1;
	}
	curNode->firstChild = tempNode;

	return 0;
}

int8 cwmpCreatXMLNode(char* nodeName, char* nodeValue, IXML_NODE_TYPE nodeType, IXML_Node** envNode){
	IXML_Node* tempNode = NULL;

	if((tempNode = (IXML_Node*)os_malloc(sizeof(IXML_Node))) == NULL){
		tc_printf("malloc envelope node fails!\r\n");
		return -1;
	}

	memset(tempNode, 0, sizeof(IXML_Node));
	
	if(nodeName){
		if((tempNode->nodeName = (char*)os_malloc(strlen(nodeName)+1)) == NULL){
			tc_printf("malloc envelope name node fails!\r\n");
//			return -1;
			goto Error_Handle;
		}
		strcpy(tempNode->nodeName, nodeName);
	}
	if(nodeValue){
		if((tempNode->nodeValue = (char*)os_malloc(strlen(nodeValue)+1)) == NULL){
			tc_printf("malloc envelope value node fails!\r\n");
//			return -1;
			goto Error_Handle;
		}
		strcpy(tempNode->nodeValue, nodeValue);
	}
	tempNode->nodeType = nodeType;

	*envNode = tempNode;
	
	return 0;
	
Error_Handle:

	if(tempNode->nodeName != NULL){
		os_free(tempNode->nodeName);
		tempNode->nodeName = NULL;
	}
	if(tempNode != NULL){
		os_free(tempNode);
		tempNode = NULL;
	}
	
	return -1;
}



/************************************ build txBuf.***************************************/

#if CWMP_USING_MBUF_FOR_TXBUF
int cwmpFrameTreeToBuf(IXML_Document* responseDoc, mbuf_t **txBuf){
	
	if(responseDoc == NULL){
		tc_printf("[cwmpFrameTreeToBuf]The responseDoc is NULL!\r\n");
		return -1;
	}
	if((*txBuf = ixmlPrintNode_to_mbuf((IXML_Node *)responseDoc)) == NULL){
		tc_printf("[cwmpFrameTreeToBuf] XML print node fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}
	return 0;
}


#else

int cwmpFrameTreeToBuf(IXML_Document* responseDoc, char **txBuf){
	
	if(responseDoc == NULL){
		tc_printf("[cwmpFrameTreeToBuf]The responseDoc is NULL!\r\n");
		return -1;
	}

	if((*txBuf = ixmlPrintNode((IXML_Node *)responseDoc)) == NULL){
		tc_printf("[cwmpFrameTreeToBuf] XML print node fails!\r\n");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	return 0;
}
#endif	


/************************************ Release doc and struct *****************************/

int8 cwmpReleaseSoapDoc(IXML_Document* soapDocPtr){
    
	if(soapDocPtr)
		ixmlDocument_free(soapDocPtr);

	return 0;
}


int8 cwmpReleaseSoapDoc_new(IXML_Document* soapDocPtr){

	if(soapDocPtr)
		ixmlDocument_free_new(soapDocPtr);

	return 0;
}


int8 cwmpReleaseSoapStruct(SoapInfoPtr soapStructPtr){
	SoapBodyInfoPtr bodyStructPtr = NULL;
	RPCArgument *curArgPtr = NULL, *nextArgPtr = NULL;
	
	if(soapStructPtr){
		cwmpReleaseSoapHeadStruct(&soapStructPtr->headStructPtr);
		cwmpReleaseSoapBodyStruct(&soapStructPtr->bodyStructPtr);
		os_free(soapStructPtr);
	}

	return 0;
}

int8 cwmpReleaseSoapHeadStruct(SoapHeadInfoPtr  *headStructPtrP){
	SoapHeadInfoPtr  headStructPtr = *headStructPtrP;

	if(headStructPtr != NULL){
		os_free(headStructPtr);
		headStructPtr = NULL;
	}

	*headStructPtrP = NULL;	

}

int8 cwmpReleaseSoapBodyStruct(SoapBodyInfoPtr *bodyStructPtrP){
	SoapBodyInfoPtr  bodyStructPtr = *bodyStructPtrP;
	RPCArgument *curArgPtr = NULL, *nextArgPtr = NULL;

	if(bodyStructPtr != NULL){
		curArgPtr = bodyStructPtr->argumentPtr;
		
		while(curArgPtr != NULL){
			nextArgPtr = curArgPtr->next;
			cwmpFreeArgment(&curArgPtr);
			curArgPtr = nextArgPtr;
		}

		bodyStructPtr->argumentPtr = NULL;

		os_free(bodyStructPtr);
		bodyStructPtr = NULL;
	}

	*bodyStructPtrP = NULL;
}

#if 1
int8 cwmpFreeArgment(RPCArgument **argument){
	RPCArgument *curArgPtr = NULL;

	curArgPtr = *argument;
	if(curArgPtr == NULL)
		return 0;

	if(curArgPtr->name != NULL){
		os_free(curArgPtr->name);
		curArgPtr->name = NULL;
	}
	
	if(curArgPtr->value != NULL){
		//free fault detail struct. xzwang_060731
		if(curArgPtr->type == FaultDetailType){
			FaultDetailPtr tempFaultPtr = (FaultDetailPtr) curArgPtr->value;
			cwmpReleaseFaultDetailStruct(tempFaultPtr);
			curArgPtr->value = NULL;
		}
		else{
			os_free(curArgPtr->value);
			curArgPtr->value= NULL;
		}
	}
	
	if(curArgPtr->parameterList != NULL){
		cwmpFreeParameterList(curArgPtr->parameterList, curArgPtr->type);
		curArgPtr->parameterList = NULL;
	}

	os_free(curArgPtr);
	*argument = NULL;

	return 0;
}
#endif

int8 cwmpFreeParameterList(void* parameterList, ParameterType argType){
	void* curPara = parameterList, *nextPara =NULL;

	if(parameterList == NULL){
		tc_printf("The parameter list for free is NULL!\r\n");
		return 0;
	}

	while(curPara){
		cwmpFreeParameter(curPara, argType, &nextPara);
		curPara = nextPara;
	}

	return 0;

}


int8 cwmpFreeSingleParameterValueStruct(ParameterValueStructPtr valueStruct)
{
	if(valueStruct == NULL){
		return 0;
	}
	if(valueStruct->name != NULL){
		os_free(valueStruct->name);
		valueStruct->name = NULL;
	}
	if(valueStruct->valueInfoPtr != NULL){
		if(valueStruct->valueInfoPtr->value != NULL){
			os_free(valueStruct->valueInfoPtr->value);
			valueStruct->valueInfoPtr->value = NULL;
		}
		os_free(valueStruct->valueInfoPtr);
		valueStruct->valueInfoPtr = NULL;
	}
	if(valueStruct != NULL){
		os_free(valueStruct);
		valueStruct = NULL;
	}

	return 0;

}

int8 cwmpFreeSingleParameterInfoStruct(ParameterInfoStructPtr infoSruct)
{
	if(infoSruct == NULL){
		return 0;
	}
	if(infoSruct->name != NULL){
		os_free(infoSruct->name);
		infoSruct->name = NULL;
	}
	if(infoSruct != NULL){
		os_free(infoSruct);
		infoSruct = NULL;
	}

	return 0;
}

int8 cwmpFreeSingleParameterAttributeStruct(ParameterAttributeStructPtr attributeStruct)
{
	AccessEntryPtr curAclEntry, nextAclEntry;

	if(attributeStruct == NULL){
		return 0;
	}
	
	if(attributeStruct->name)
		os_free(attributeStruct->name);
	curAclEntry = attributeStruct->accessList;
	while(curAclEntry != NULL){
		nextAclEntry = curAclEntry->next;
		os_free(curAclEntry);
		curAclEntry = nextAclEntry;
	}
	if(attributeStruct){
	        os_free(attributeStruct);
		 attributeStruct = NULL;
	}

	return 0;
}

int8 cwmpFreeSingleParameterAttributesSetStruct(ParameterAttributesSetStructPtr attributeSetStruct)
{
	AccessEntryPtr curAclEntry, nextAclEntry;
				
	if(attributeSetStruct == NULL){
		return 0;
	}
	
	if(attributeSetStruct->name)
		os_free(attributeSetStruct->name);
	curAclEntry = attributeSetStruct->accessList;
	while(curAclEntry){
		nextAclEntry = curAclEntry->next;
		os_free(curAclEntry);
		curAclEntry = nextAclEntry;
	}
	if(attributeSetStruct){
	        os_free(attributeSetStruct);
		attributeSetStruct = NULL;
	}

	return 0;
}

int8 cwmpFreeParameter(void* curPara, ParameterType argType, void** nextPara){

	if(curPara == NULL){
               tc_printf("\r\n[cwmpFreeParameter] curPara is NULL!");
               return 0;
        }


	switch(argType){
		case StringType:
		case String256Type:
		{
			StringStructPtr tempCurPara =(StringStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
			if(tempCurPara->value){
				os_free(tempCurPara->value);
				tempCurPara->value = NULL;
			}
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}
		case IntType:
		{
			IntStructPtr tempCurPara =(IntStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}
		case UnsignedIntType:
		{
			UnsignedIntStructPtr tempCurPara =(UnsignedIntStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}	
		case BooleanType:
		{
			BooleanStructPtr tempCurPara =(BooleanStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}
		case DateTimeType:
		{
			DateTimeStructPtr tempCurPara =(DateTimeStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}
		case Base64Type:
		{
			Base64StructPtr tempCurPara =(Base64StructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}
		case ParameterValueStructType:
		{
			ParameterValueStructPtr tempCurPara =(ParameterValueStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
#if 0
			if(tempCurPara->name)
				os_free(tempCurPara->name);
			if(tempCurPara->valueInfoPtr){
				if(tempCurPara->valueInfoPtr->value){
					os_free(tempCurPara->valueInfoPtr->value);
					tempCurPara->valueInfoPtr->value = NULL;
				}
				os_free(tempCurPara->valueInfoPtr);
				tempCurPara->valueInfoPtr = NULL;
			}
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
#else
			cwmpFreeSingleParameterValueStruct(tempCurPara);
			tempCurPara = NULL;
#endif
			break;
		}
		case ParameterInfoStructType:
		{
			ParameterInfoStructPtr tempCurPara =( ParameterInfoStructPtr)curPara;
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
#if 0
			if(tempCurPara->name){
				os_free(tempCurPara->name);
				tempCurPara->name = NULL;
			}
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
#else
			cwmpFreeSingleParameterInfoStruct(tempCurPara);
			tempCurPara = NULL;
#endif
			break;
		}
		case SetParameterAttributesStructType:
		{
			ParameterAttributesSetStructPtr tempCurPara =(ParameterAttributesSetStructPtr)curPara;
			AccessEntryPtr curAclEntry, nextAclEntry;
				
			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
#if 0
			if(tempCurPara->name)
				os_free(tempCurPara->name);
			curAclEntry = tempCurPara->accessList;
			while(curAclEntry){
				nextAclEntry = curAclEntry->next;
				os_free(curAclEntry);
				curAclEntry = nextAclEntry;
			}
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
#else
			cwmpFreeSingleParameterAttributesSetStruct(tempCurPara);
			tempCurPara = NULL;
#endif
			break;
		}
		case ParameterAttributeStructType:
		{
			ParameterAttributeStructPtr tempCurPara =(ParameterAttributeStructPtr) curPara;
			AccessEntryPtr curAclEntry, nextAclEntry;

			//cwmpFreeParameterList(tempCurPara->next, argType);
			*nextPara = tempCurPara->next;
#if 0
			if(tempCurPara->name)
				os_free(tempCurPara->name);
			curAclEntry = tempCurPara->accessList;
			while(curAclEntry){
				nextAclEntry = curAclEntry->next;
				os_free(curAclEntry);
				curAclEntry = nextAclEntry;
			}
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
#else
			cwmpFreeSingleParameterAttributeStruct(tempCurPara);
			tempCurPara = NULL;
#endif
			break;
		}
		case EventStructType:
		{
			EventStructPtr tempCurPara =(EventStructPtr) curPara;

			*nextPara = tempCurPara->next;
			if(tempCurPara){
			        os_free(tempCurPara);
				tempCurPara = NULL;
			}
			break;
		}
		default:
			tc_printf("Error argument type!\r\n");
			break;
		}

	return 0;
}

static int8 cwmpReleaseFaultDetailStruct(FaultDetailPtr faultDetailtPtr){
	FaultDetailPtr detail;
	RPCFaultPtr rpcFaultList, rpcFaultList_temp;

	detail = faultDetailtPtr;
	if(detail == NULL)
		return 0;
	
	if(detail->faultString != NULL){
		 os_free(detail->faultString);
		detail->faultString = NULL;
	}	
	rpcFaultList = detail->rpcFaultList;
		
	while(rpcFaultList != NULL ){
			
		rpcFaultList_temp = rpcFaultList;
		rpcFaultList = rpcFaultList->next; 

#if 0
		if(rpcFaultList_temp->rpcErrorType != NULL){
			os_free(rpcFaultList_temp->rpcErrorType);
			rpcFaultList_temp->rpcErrorType = NULL;
		}
		if(rpcFaultList_temp->parameterName != NULL){
			os_free(rpcFaultList_temp->parameterName);
			rpcFaultList_temp->parameterName = NULL;
		}
		if(rpcFaultList_temp->faultString != NULL){
			os_free(rpcFaultList_temp->faultString);
			rpcFaultList_temp->faultString = NULL;
		}
		os_free(rpcFaultList_temp);	
		rpcFaultList_temp = NULL;
#else
		freeRPCFaultStruct(&rpcFaultList_temp);
#endif

	}

	os_free(detail);
	detail = NULL;
	return 0;

}

int8 cwmpReleaseFaultStruct(FaultStructPtr soapFaultPtr){
	FaultDetailPtr detail = NULL;

	if(soapFaultPtr == NULL){
		tc_printf("The soapFaultPtr for free is NULL!\r\n");
		return 0;
	}
	detail = soapFaultPtr->detail;
	if(detail != NULL){
		cwmpReleaseFaultDetailStruct(detail);
	}

	rpcFaultListHead = NULL;
	rpcFaultListTail = NULL;
	faultDetailPtr = NULL;
		
	os_free(soapFaultPtr);

	return 0;
}

void	cwmpFreeSoapMemory(cwmp_rpc_t *rpc)
{
	if(rpc->requestDocPtr){
		cwmpReleaseSoapDoc(rpc->requestDocPtr);  
		rpc->requestDocPtr = NULL;
	}
	
	if(rpc->requestStructPtr){
		cwmpReleaseSoapStruct(rpc->requestStructPtr);
		rpc->requestStructPtr = NULL;
	}
	
	if(rpc->responseStructPtr){
		cwmpReleaseSoapStruct(rpc->responseStructPtr);
		rpc->responseStructPtr = NULL;
	}
	
	if(rpc->soapFaultPtr){
		cwmpReleaseFaultStruct(rpc->soapFaultPtr);
		rpc->soapFaultPtr = NULL;
	}
	
	if(rpc->responseDocPtr){
		cwmpReleaseSoapDoc_new(rpc->responseDocPtr);
		rpc->responseDocPtr = NULL;
	}
}


//The function can only used for decimal.
void cwmpItoa( int num, char* str, int type){
	int tempNum;
	int i = 0, j = 0, count = 0;
	int curDig[32];
	
	if(num == 0){
		str[0] = 48;
		str[1] = 0;
		return;
	}

	if(num < 0){
		str[i] = '-';
		num = - num;
		i++;
	}

	tempNum = num;
	while(tempNum){
		curDig[count] = tempNum%type;
		tempNum = tempNum / type;
		count++;
       }

	for(j = 0; j<count; j++){
		str[i] = curDig[count-1-j] + 48;
		i++;
	}
	str[i] = '\0';
}

/***********************for value change mechanism *************************************/

void cwmpAddNotifyNodeList(NotifyDeviceNodeInfoPtr notifyNodeP, NotifyType notify){
	NotifyDeviceNodeInfoPtr NotifyNodeListHeadP = NULL;
	NotifyDeviceNodeInfoPtr NotifyNodeListTailP  = NULL;

	if(notifyNodeP == NULL){
		tc_printf("\r\nAdded notify node is NULL!");
		return;
	}
	if(TCSUPPORT_CT_MIDWARE_VAL)
		notify &= 0x07;
	if(notify == Active){
		NotifyNodeListHeadP = activeNotifyNodeListHeadP;
		NotifyNodeListTailP = activeNotifyNodeListTailP;
	}
	else if(notify == Passive){
		NotifyNodeListHeadP = passiveNotifyNodeListHeadP;
		NotifyNodeListTailP = passiveNotifyNodeListTailP;
	}
	else{
		tc_printf("\r\nInvalid notification for add notify list!");
		return;
	}
	
	if(NotifyNodeListHeadP == NULL)
		NotifyNodeListHeadP = notifyNodeP;
	else{
		NotifyNodeListTailP->next = notifyNodeP;
		notifyNodeP->prev = NotifyNodeListTailP;
	}

	NotifyNodeListTailP = notifyNodeP;

	if(notify == Active){
		activeNotifyNodeListHeadP = NotifyNodeListHeadP;
		activeNotifyNodeListTailP = NotifyNodeListTailP;
	}
	else if(notify == Passive){
		passiveNotifyNodeListHeadP = NotifyNodeListHeadP ;
		passiveNotifyNodeListTailP = NotifyNodeListTailP;
	}
	
}

void cwmpDeleteNotifyNodeList(DeviceNode* deviceNode, NotifyType notify){
	NotifyDeviceNodeInfoPtr NotifyNodeListHead = NULL;
	NotifyDeviceNodeInfoPtr NotifyNodeListTail = NULL;
	NotifyDeviceNodeInfo* curNotifyNode = NULL;
	NotifyDeviceNodeInfo* tempNotifyNode1= NULL, *tempNotifyNode2 = NULL;
	char deviceName[256] = "";

	if(deviceNode == NULL){
		tc_printf("\r\nDelete device node is NULL!");
		return;
	}
	if(TCSUPPORT_CT_MIDWARE_VAL)
		notify &= 0x07;
	if(notify == Active){
		NotifyNodeListHead = activeNotifyNodeListHeadP;
		NotifyNodeListTail = activeNotifyNodeListTailP;
	}
	else if(notify == Passive){
		NotifyNodeListHead = passiveNotifyNodeListHeadP;
		NotifyNodeListTail = passiveNotifyNodeListTailP;
	}
	else{
		tc_printf("\r\nInvalid notification for delete notify list!");
		return;
	}

	curNotifyNode = NotifyNodeListHead;

	cwmpGetDeviceNodeName(deviceNode, deviceName);
	
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n[cwmpDeleteNotifyNodeList] !");
	}
	while(curNotifyNode){

		if(strcmp(curNotifyNode->deviceNodeName, deviceName) == 0){
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\nDelete Notify node:%s\r\n",deviceName);
			}
			cwmpFreeNotifyNode(&curNotifyNode, &NotifyNodeListHead, &NotifyNodeListTail);
			goto RelinkList;
		}
		curNotifyNode = curNotifyNode->next;
	}
		
RelinkList:
	if(notify == Active){
		activeNotifyNodeListHeadP = NotifyNodeListHead;
		activeNotifyNodeListTailP = NotifyNodeListTail;
	}
	else if(notify == Passive){
		passiveNotifyNodeListHeadP = NotifyNodeListHead;
		passiveNotifyNodeListTailP = NotifyNodeListTail;
	}
}

int cwmpFreeNotifyNode(NotifyDeviceNodeInfoPtr *NotifyNodeP, NotifyDeviceNodeInfoPtr *NotifyListHeadP, NotifyDeviceNodeInfoPtr *NotifyListTailP){
	NotifyDeviceNodeInfo* curNotifyNode= NULL;
	NotifyDeviceNodeInfo* tempNotifyNode1 = NULL, *tempNotifyNode2 = NULL;
	NotifyDeviceNodeInfo* tempNotifyListHead = NULL, *tempNotifyListTail = NULL;

	curNotifyNode = *NotifyNodeP;
	tempNotifyListHead = *NotifyListHeadP;
	tempNotifyListTail = *NotifyListTailP;

	if(curNotifyNode){
		tempNotifyNode1 = curNotifyNode->prev;
		tempNotifyNode2 = curNotifyNode->next;

		if(tempNotifyNode1 )
			tempNotifyNode1->next = tempNotifyNode2;
		else{
			tempNotifyListHead = tempNotifyNode2;
		}
				
		if(tempNotifyNode2 )
			tempNotifyNode2->prev = tempNotifyNode1;
		else
			tempNotifyListTail = tempNotifyNode1;

	
		if(curNotifyNode->acsSetValue){
			os_free(curNotifyNode->acsSetValue);
			curNotifyNode->acsSetValue = NULL;
		}

		if(curNotifyNode->deviceNodeName){
			os_free(curNotifyNode->deviceNodeName);
			curNotifyNode->deviceNodeName = NULL;
		}
		
		os_free(curNotifyNode);
		curNotifyNode = NULL;
	}

	*NotifyNodeP = NULL;
	*NotifyListHeadP = tempNotifyListHead;
	*NotifyListTailP = tempNotifyListTail;
	
	return 0;
}

void cwmpModifyNotifyNodeList(DeviceNode* deviceNode, NotifyType notify){
	NotifyDeviceNodeInfo* curNotifyNode = NULL;
	char deviceName[256] = "";

	if(deviceNode == NULL){
		tc_printf("\r\n[cwmpModifyNotifyNodeList]device node is NULL!");
		return;
	}
	if(TCSUPPORT_CT_MIDWARE_VAL)
		notify &= 0x07;
	if(notify == Active){
		curNotifyNode = activeNotifyNodeListHeadP;
	}
	else if(notify == Passive){
		curNotifyNode = passiveNotifyNodeListHeadP;
	}
	else{
		tc_printf("\r\nInvalid notification for delete notify list!");
		return;
	}

	cwmpGetDeviceNodeName(deviceNode, deviceName);

	while(curNotifyNode){
	#if 0
		if(curNotifyNode->deviceNode  == deviceNode){
	#else
		if(strcmp(curNotifyNode->deviceNodeName, deviceName) == 0){
	#endif	
			char* curValue = NULL;
			ParameterType paraType;

			if(deviceNode->getValueInfo){
				deviceNode->getValueInfo(&curValue, &paraType, deviceNode);
				if(curNotifyNode->acsSetValue)
                      			os_free(curNotifyNode->acsSetValue);
				curNotifyNode->acsSetValue = curValue;
			}
			else{
				tc_printf("\r\nwithout get value func, so modify notify node list fails!");
			}
			
			return;
		}
		
		curNotifyNode = curNotifyNode->next;
	}
	
}

int8 cwmpCreateNotifyNode(DeviceNodePtr deviceNode, NotifyDeviceNodeInfoPtr* notifyNodeP){
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;
	DeviceNodePtr currentDeviceNode = deviceNode;
	ParameterType paraType;

	if(deviceNode == NULL){
		tc_printf("\r\nDevice Node is NULL for create Notify Node!\r\n");
		return -1;
	}

	if((tempNotifyNodeP = (NotifyDeviceNodeInfoPtr)os_malloc(sizeof(NotifyDeviceNodeInfo))) != NULL){
		memset(tempNotifyNodeP, 0, sizeof(NotifyDeviceNodeInfo));
		if(currentDeviceNode->getValueInfo){
			int8 ret;
			ret = currentDeviceNode->getValueInfo(&tempNotifyNodeP->acsSetValue, &paraType, currentDeviceNode);
			if(ret != 0){
				tc_printf("\r\n GetValueInfo fails!");
				os_free(tempNotifyNodeP);
				return -1;									
			}
		}	
		else{
			tc_printf("\r\n GetValueInfo func is NULL!");
			os_free(tempNotifyNodeP);
			return -1;
		}

		if(cwmpCreateDeviceNodeName(currentDeviceNode, &(tempNotifyNodeP->deviceNodeName)) == -1){
			tc_printf("\r\nCreate Device Node Name Fails!\r\n");
			if(tempNotifyNodeP->acsSetValue)
				os_free(tempNotifyNodeP->acsSetValue);
			os_free(tempNotifyNodeP);
			tempNotifyNodeP = NULL;
			return -1; //xzwang_060815
		}	
	#if 0	
		tempNotifyNodeP->deviceNode = currentDeviceNode;
	#endif
	}
	else{
		tc_printf("\r\n malloc NotifyDeviceNodeInfo fails!");
		return -1;
	}

	*notifyNodeP = tempNotifyNodeP;

	return 0;
}	

int8 cwmpInitNotifyNodeList(void){
	DeviceNodePtr currentDeviceNode = rpc.IGDDeviceRootPtr;
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;
	uint8 notification = 0;
	AccessEntry* acl = NULL;

	while(currentDeviceNode != NULL){

		while(currentDeviceNode != NULL){
			if(currentDeviceNode->type != CWMP_OBJECT){
				if(currentDeviceNode->getAttribute){
					currentDeviceNode->getAttribute(&notification, &acl, currentDeviceNode);
					if(acl){
					        os_free(acl);
						acl = NULL;
					}
				
					if(notification){
						if(!cwmpBeForcedNotifyNode(currentDeviceNode)){
							if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
								cwmpAddNotifyNodeList(tempNotifyNodeP, notification);
								if(dbg_cwmp >= DBG_CWMP_L2 ){
									tc_printf("\r\n  Notify node:%s\r\n",currentDeviceNode->name);
								}
							}
						}
					}
				}
				else{
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						tc_printf("\r\n  without attribute func for device node:%s\r\n",currentDeviceNode->name);
					}
				}

				if(cwmpBeForcedNotifyNode(currentDeviceNode)){
					if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
						cwmpAddNotifyNodeList(tempNotifyNodeP, Active);
						if(dbg_cwmp >= DBG_CWMP_L2 ){
							tc_printf("\r\n  Forced notify node:%s\r\n",currentDeviceNode->name);
						}
					}
				}
			}

			cwmpPushDeviceStack(currentDeviceNode);
			currentDeviceNode = currentDeviceNode->firstChild;
			
		}

		while(currentDeviceNode == NULL && deviceNodeStackHead != NULL){
			cwmpPopDeviceStack(&currentDeviceNode);
			if(deviceNodeStackHead != NULL)
				currentDeviceNode = currentDeviceNode->nextSibling;
			else
				currentDeviceNode = NULL;
		}
	}

	return 0;
}

int8 cwmpClearNotifyNodeList( NotifyType notify){
	NotifyDeviceNodeInfo *curNotifyNode = NULL, *nextNotifyNode = NULL;
	//char deviceName[256];
	
	if(TCSUPPORT_CT_MIDWARE_VAL)
		notify &= 0x07;
	if(notify == Active){
		curNotifyNode = activeNotifyNodeListHeadP;
	}
	else if(notify == Passive){
		curNotifyNode = passiveNotifyNodeListHeadP;
	}
	else{
		tc_printf("\r\nInvalid notification for delete notify list!");
		return 0;
	}

	while(curNotifyNode){
		nextNotifyNode = curNotifyNode->next;
		
		if(curNotifyNode->acsSetValue){
			os_free(curNotifyNode->acsSetValue);
			curNotifyNode->acsSetValue = NULL;
		}

		if(curNotifyNode->deviceNodeName){
			os_free(curNotifyNode->deviceNodeName);
			curNotifyNode->deviceNodeName = NULL;
		}
		
		os_free(curNotifyNode);
		
		curNotifyNode = nextNotifyNode;
	}

	if(notify == Active){
		activeNotifyNodeListHeadP = NULL;
		activeNotifyNodeListTailP = NULL;
	}
	else if(notify == Passive){
		 passiveNotifyNodeListHeadP = NULL;
		 passiveNotifyNodeListTailP = NULL;
	}
	
	return 0;
}	


int8 cwmpMultipulateNotifyListByNewAttr(DeviceNodePtr deviceNode, uint8 newNotification){
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;
	uint8 notification = 0;
	AccessEntry* acl = NULL; 

	if(deviceNode == NULL){
		tc_printf("\r\n[cwmpMultipulateNotifyListByNewAttr]device node is NULL!");
		return  -1;
	}

	if(cwmpBeForcedNotifyNode(deviceNode)){
		tc_printf("\r\nReject to change forced node in notify list.");
		return 0;
	}
	
	if(deviceNode->getAttribute == NULL){
		tc_printf("\r\nwithout get attribute  func for node:%s\r\n",deviceNode->name);
		return 0;
	}

	//always delete node from notification list first and then add it into new list. xzwang_060907
	cwmpDeleteNotifyNodeList(deviceNode, Active);
	cwmpDeleteNotifyNodeList(deviceNode, Passive);

	if(newNotification != 0){
		if(cwmpCreateNotifyNode(deviceNode, &tempNotifyNodeP) == 0)
			cwmpAddNotifyNodeList(tempNotifyNodeP, newNotification);
	}
		
	return 0;
}

int8 cwmpMultipulateNotifyListByNewValue(DeviceNodePtr deviceNode){
	uint8 notification = 0;
	AccessEntry* acl = NULL;

	if(deviceNode == NULL){
		tc_printf("\r\n[cwmpMultipulateNotifyListByNewValue]device node is NULL!");
		return -1;
	}
	
	if(cwmpBeForcedNotifyNode(deviceNode)){
		cwmpModifyNotifyNodeList(deviceNode, Active);
		return 0;
	}

	if(deviceNode->getAttribute == NULL){
		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("\r\nwithout get attribute  func for node:%s\r\n",deviceNode->name);
		}
		return 0;
	}
	
	deviceNode->getAttribute(&notification, &acl, deviceNode);
	if(acl != NULL){
	        os_free(acl);
		acl = NULL;
	}

	if(notification != 0){
		cwmpModifyNotifyNodeList(deviceNode, notification);
	}

	return 0;
}

//modify by brian for removing warning
int/*8*/ cwmpPushValueChangeStack(DeviceNodePtr deviceNode){
	DeviceNodeStackPtr newStackNode =NULL;

	if(deviceNode == NULL){
		tc_printf("The device node for push into stack is NULL ");
		return -1;
	}
	if((newStackNode = (DeviceNodeStackPtr)os_malloc(sizeof(DeviceNodeStack))) == NULL){
		tc_printf("malloc device stack node fails");
		return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
	}

	memset(newStackNode,0, sizeof(DeviceNodeStack));
	newStackNode->deviceNode = deviceNode;
	
	newStackNode->next = valueChangeStackHeadP;
	valueChangeStackHeadP= newStackNode;

	return 0;
}

int8 cwmpPopValueChangeStack(DeviceNodePtr* deviceNode){
	DeviceNodeStackPtr tempStackNode =valueChangeStackHeadP;
	
	if(valueChangeStackHeadP == NULL){
	//	printf("The device stack is NULL, pop stack fails!\r\n");
		return -1;
	}
    
	*deviceNode = valueChangeStackHeadP->deviceNode;
	
	valueChangeStackHeadP = valueChangeStackHeadP->next;
	tempStackNode ->next =NULL;
	
        if(tempStackNode){
	        os_free(tempStackNode);
		tempStackNode = NULL;
	}
    
	return 0;
}

uint8 getITMSregFlag(void)
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

int8 cwmpSearchChangedValue(NotifyType notify){
	NotifyDeviceNodeInfo* curNotifyNode = NULL, *tempNotifyNode = NULL;
	NotifyDeviceNodeInfo* NotifyNodeHead = NULL, *NotifyNodeTail = NULL; 
	DeviceNodePtr currentDeviceNode = NULL;
	char* curParaValue = NULL;
	ParameterType paraType;
	int ret =0;

	if(notify == Active){
		NotifyNodeHead = activeNotifyNodeListHeadP;	
		NotifyNodeTail = activeNotifyNodeListTailP;	
	}
	else if(notify == Passive){
		NotifyNodeHead = passiveNotifyNodeListHeadP;	
		NotifyNodeTail = passiveNotifyNodeListTailP;	
	}

	curNotifyNode = NotifyNodeHead;
	
	while(curNotifyNode){
	#if 0	
		currentDeviceNode = curNotifyNode->deviceNode;
	#else
		cwmpSearchDeviceNodeByName(curNotifyNode->deviceNodeName, rpc.IGDDeviceRootPtr, &currentDeviceNode);

		if(currentDeviceNode == NULL){
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\nThe node in notify list does not exist!");
			}
			tempNotifyNode = curNotifyNode->next;

			cwmpFreeNotifyNode(&curNotifyNode, &NotifyNodeHead, &NotifyNodeTail);
			
			curNotifyNode = tempNotifyNode;
			continue;
		}
	#endif	
	
		if(currentDeviceNode->getValueInfo){
			currentDeviceNode->getValueInfo(&curParaValue, &paraType, currentDeviceNode);
		}

		if(curParaValue){
			if(strcmp(curParaValue, curNotifyNode->acsSetValue) !=0){
				if(dbg_cwmp >= DBG_CWMP_L2 ){
					tc_printf("\r\n Find Value changed node:%s\r\n",currentDeviceNode->name);
				}
				if (TCSUPPORT_CT_PON_C9_VAL)
				{
					if ((1 == getBootFlag()) || (1 == getITMSregFlag()))
					{
						ret = 1;
					}
				}
				else
				{
					ret = 1;
				}
				if(!cwmpBeForcedNotifyNode(currentDeviceNode)){
					cwmpPushValueChangeStack(currentDeviceNode);
				}		
				cwmpModifyNotifyNodeList(currentDeviceNode, notify);	
			}
			os_free(curParaValue);
			curParaValue = NULL;
		}

		curNotifyNode = curNotifyNode->next;
	}

	if(notify == Active){
		activeNotifyNodeListHeadP = NotifyNodeHead;	
		activeNotifyNodeListTailP = NotifyNodeTail;	
	}
	else if(notify == Passive){
		passiveNotifyNodeListHeadP = NotifyNodeHead;	
		passiveNotifyNodeListTailP = NotifyNodeTail;	
	}


	return ret;
}

int8 cwmpBeForcedNotifyNode(DeviceNodePtr deviceNode){
	int8 i = 0;
	char deviceNodeName[160] = "";
	char* forcedName = forcedNotifyNode[0];

	DeviceNodePtr currentDeviceNode = NULL;
	char *conRequestURLPtr = NULL;
	
	if(deviceNode == NULL){
		tc_printf("\r\n[cwmpBeForcedNotifyNode] device node is NULL!");
		return 0;
	}
		
	cwmpGetDeviceNodeName(deviceNode, deviceNodeName);
	
	while(forcedName){
		if(strcmp(deviceNodeName, forcedName) == 0)
			return 1;
		i++;
		forcedName = forcedNotifyNode[i];
	}

	if(strcmp(deviceNodeName, getWANExternalName(getWANIndex())) == 0)
	{
#if 0
		//update connection request URL
		conRequestURLPtr = fetchConRequestURL_detail();
		cwmpSearchDeviceNodeByName("InternetGatewayDevice.ManagementServer.ConnectionRequestURL", rpc.IGDDeviceRootPtr, &currentDeviceNode);
		if(currentDeviceNode && (strcmp(conRequestURLPtr, "0.0.0.0") != 0)){
			//cwmpModifyNotifyNodeList(currentDeviceNode, Active);
			setActiveNotifyFlag(1);
		}
#endif
		return 1;
	}

	return 0;
}

int8 cwmpBuildValueChangeParameters(void){
	DeviceNodePtr  deviceNode = NULL;
	int8 ret= 0;
	int8 state = 0;

	cwmpPopValueChangeStack(&deviceNode);
	
	while(deviceNode){
		char paraName[256] = "";

		cwmpGetDeviceNodeName(deviceNode, paraName);

		if(deviceNode->getValueInfo){
			char* paraValue = NULL;
			ParameterType paraType;
			
			deviceNode->getValueInfo(&paraValue, &paraType, deviceNode);
			if(paraValue){
				if(dbg_cwmp >= DBG_CWMP_L2 ){
					tc_printf("\r\n Build inform parameter for Value changed node:%s\r\n",deviceNode->name);
				}

				state =cwmpAddInformParameter(paraName, paraValue, paraType);
					
				os_free(paraValue);
				paraValue = NULL;

				if (state!=0)
				{
					tc_printf("\r\n cwmpBuildValueChangeParameters() fails!");
					return -1;
				}

				ret = 1;
			}
			else{
				if(dbg_cwmp >= DBG_CWMP_L2 ){
					tc_printf("\r\n  Can not get parameter value of node:%s\r\n",deviceNode->name);
				}
			}
		}
		else{
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\n  Without get value func of node:%s\r\n",deviceNode->name);
			}
		}
		
		deviceNode = NULL;
		cwmpPopValueChangeStack(&deviceNode);
	}	

	return ret;

}

extern char* getOperationStatus_Open(int index);
extern int getUPnPDownloadInstance_Open(void);
int8 cwmpBuildUPnPDMDownloadParameters(void){
	int index = 0;	
	char tmpBuf[MAX_ATTRIBUTE_LEN] = {0};
	int8 state = 0;	
	char operationStatus[256] = {'\0'};

	if(TCSUPPORT_CT_UPNP_DM_VAL){
		index = getUPnPDownloadInstance_Open();
		sprintf(tmpBuf, "InternetGatewayDevice.X_CT-COM_ProxyDevice.DeviceList.%d.ActionList.BMS.X_CT-COM_GetDownloadStatus.OperationStatus", index);
		strcpy(operationStatus,getOperationStatus_Open(index));
		state = cwmpAddInformParameter(tmpBuf, operationStatus, StringType);
		if (state!=0)
			 return -1;
	}
	return 0;
}

int8 cwmpBuildCardmanageParameters(void){
	char tmpBuf[64] = {0};
	int8 state = 0;	

	memset(tmpBuf,0,sizeof(tmpBuf));
	tcapi_get(SIM_CARD_NODE_NAME, "CardSN", tmpBuf);
	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_Cardmanage.CardNo", tmpBuf, StringType);
	if (state!=0)
		 return -1;

	memset(tmpBuf,0,sizeof(tmpBuf));
	tcapi_get(SIM_CARD_NODE_NAME, "Status", tmpBuf);
	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_Cardmanage.status", tmpBuf, UnsignedIntType);
	if (state!=0)
		 return -1;

	
	memset(tmpBuf,0,sizeof(tmpBuf));
	tcapi_get(SIM_CARD_NODE_NAME, "cardExist", tmpBuf);
	state = cwmpAddInformParameter("InternetGatewayDevice.DeviceInfo.X_CT-COM_Cardmanage.CardStatus", tmpBuf, BooleanType);
	if (state!=0)
		 return -1;

	return 0;
}


int midwareGetDeviceNodeName(DeviceNodePtr deviceNode, char* deviceName, DeviceNodePtr deviceRoot){
	DeviceNodePtr tempDeviceNode = deviceNode;
	char subNodeName[PARAMETER_NUM][PARAMETER_NAME_LEN];
	int8 i = 0, j = 0;

	if(TCSUPPORT_CT_MIDWARE_VAL){	
		if(deviceNode == NULL){
			tc_printf("The device node is NULL and get name fails!\r\n");
			return -1;
		}
		if(deviceName == NULL){
			tc_printf("The Pointer for store device node name is NULL!\r\n");
			return -1;
		}
		memset(deviceName, 0, strlen(deviceName)+1);
		while(tempDeviceNode != deviceRoot){
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
	}
	return 0;
}

/***********************for midware value change mechanism *************************************/
int/*8*/ midwarePushValueChangeStack(DeviceNodePtr deviceNode, NotifyType notify){
	DeviceNodeStackPtr newStackNode =NULL;
	DeviceNodeStackPtr *valueChangeStackHeadP = NULL;	

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceNode == NULL){
			tc_printf("The device node for push into stack is NULL ");
			return -1;
		}
		if((newStackNode = (DeviceNodeStackPtr)os_malloc(sizeof(DeviceNodeStack))) == NULL){
			tc_printf("malloc device stack node fails");
			return ResourceExceed;//replace InternalError by ResourceExceed,hqfeng,2006.06.21
		}

		if(notify == MWActive){
			valueChangeStackHeadP = &MWvalueChangeStackHeadP;
		}
		else if(notify == TRActive){
			valueChangeStackHeadP = &TRvalueChangeStackHeadP;
		}
		else if(notify == ITMSActive){
			valueChangeStackHeadP = &ITMSvalueChangeStackHeadP;
		}
			
		memset(newStackNode,0, sizeof(DeviceNodeStack));
		newStackNode->deviceNode = deviceNode;
		
		//newStackNode->next = MWvalueChangeStackHeadP;
		//MWvalueChangeStackHeadP= newStackNode;
		newStackNode->next = *valueChangeStackHeadP;
		*valueChangeStackHeadP = newStackNode;
	}

	return 0;
}

void midwareModifyNotifyNodeList(DeviceNode* deviceNode, NotifyType notify){
	NotifyDeviceNodeInfo* curNotifyNode = NULL;
	char deviceName[256] = "";

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceNode == NULL){
			tc_printf("\r\n[cwmpModifyNotifyNodeList]device node is NULL!");
			return;
		}
		if(notify == MWActive){
			curNotifyNode = MWActiveNotifyNodeListHeadP;
			midwareGetDeviceNodeName(deviceNode, deviceName,rpc.MidwareRootPtr);
		}
		else if(notify == TRActive){
			curNotifyNode = TRActiveNotifyNodeListHeadP;
			cwmpGetDeviceNodeName(deviceNode, deviceName);
		}
		else if(notify == ITMSActive){
			curNotifyNode = ITMSActiveNotifyNodeListHeadP;
			cwmpGetDeviceNodeName(deviceNode, deviceName);
		}
		else{
			tc_printf("midwareModifyNotifyNodeList:notify err!\n");
			return;
			}

		while(curNotifyNode){
		#if 0
			if(curNotifyNode->deviceNode  == deviceNode){
		#else
			if(strcmp(curNotifyNode->deviceNodeName, deviceName) == 0){
		#endif	
				char* curValue = NULL;
				ParameterType paraType;

				if(deviceNode->getValueInfo){
					deviceNode->getValueInfo(&curValue, &paraType, deviceNode);
					if(curNotifyNode->acsSetValue)
	                      			os_free(curNotifyNode->acsSetValue);
					curNotifyNode->acsSetValue = curValue;
				}
				else{
					tc_printf("\r\nwithout get value func, so modify notify node list fails!");
				}
				
				return;
			}		
			curNotifyNode = curNotifyNode->next;
		}
	}
}

int8 midwarePopValueChangeStack(DeviceNodePtr* deviceNode, NotifyType notify){
	#if 0
	DeviceNodeStackPtr tempStackNode =MWvalueChangeStackHeadP;
	
	if(MWvalueChangeStackHeadP == NULL){
	//	printf("The device stack is NULL, pop stack fails!\r\n");
		return -1;
	}
    
	*deviceNode = MWvalueChangeStackHeadP->deviceNode;
	
	MWvalueChangeStackHeadP = MWvalueChangeStackHeadP->next;
	#endif
	DeviceNodeStackPtr *valueChangeStackHeadP = NULL;	
	DeviceNodeStackPtr tempStackNode = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){	
		if(notify == MWActive){
			valueChangeStackHeadP = &MWvalueChangeStackHeadP;
		}
		else if(notify == TRActive){
			valueChangeStackHeadP = &TRvalueChangeStackHeadP;
		}
		else if(notify == ITMSActive){
			valueChangeStackHeadP = &ITMSvalueChangeStackHeadP;
		}
		tempStackNode = *valueChangeStackHeadP;
		
		if(tempStackNode == NULL){
		//	printf("The device stack is NULL, pop stack fails!\r\n");
			return -1;
		}
		
		*deviceNode = tempStackNode->deviceNode;
		
		*valueChangeStackHeadP = tempStackNode->next;
		tempStackNode ->next = NULL;
	        if(tempStackNode){
		        os_free(tempStackNode);
			tempStackNode = NULL;
		}
	}   
	return 0;
}

int8 midwareSearchChangedValue(NotifyType notify){
	NotifyDeviceNodeInfo* curNotifyNode  = NULL, *tempNotifyNode = NULL;
	NotifyDeviceNodeInfo* NotifyNodeHead = NULL, *NotifyNodeTail = NULL; 
	DeviceNodePtr currentDeviceNode = NULL;
	DeviceNodePtr deviceRoot = NULL;
	char* curParaValue = NULL;
	ParameterType paraType;
	int ret =0;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(notify == MWActive){
			NotifyNodeHead = MWActiveNotifyNodeListHeadP;
			NotifyNodeTail = MWActiveNotifyNodeListTailP; 
			deviceRoot = rpc.MidwareRootPtr;
		}
		else if(notify == TRActive){
			NotifyNodeHead = TRActiveNotifyNodeListHeadP;
			NotifyNodeTail = TRActiveNotifyNodeListTailP;
			deviceRoot = rpc.IGDDeviceRootPtr;
		}
		else if(notify == ITMSActive){
			NotifyNodeHead = ITMSActiveNotifyNodeListHeadP;
			NotifyNodeTail = ITMSActiveNotifyNodeListTailP;
			deviceRoot = rpc.IGDDeviceRootPtr;
		}

		curNotifyNode = NotifyNodeHead;	
		while(curNotifyNode){
				#if 0
			currentDeviceNode = curNotifyNode->deviceNode;
		#else
			cwmpSearchDeviceNodeByName(curNotifyNode->deviceNodeName, deviceRoot, &currentDeviceNode);

			if(currentDeviceNode == NULL){
				if(dbg_cwmp >= DBG_CWMP_L2 ){
					tc_printf("\r\nThe node in notify list does not exist!");
				}
				tempNotifyNode = curNotifyNode->next;
				cwmpFreeNotifyNode(&curNotifyNode, &NotifyNodeHead, &NotifyNodeTail);			
				curNotifyNode = tempNotifyNode;
				continue;
					}	
				#endif
			//tc_printf("cwmpSearchChangedValue()search node:%s,%s!\n",curNotifyNode->deviceNodeName,currentDeviceNode->name);
			if(currentDeviceNode->getValueInfo){
				currentDeviceNode->getValueInfo(&curParaValue, &paraType, currentDeviceNode);
			}

			if(curParaValue){
				//tc_printf("cwmpSearchChangedValue()curParaValue=%s,acsSetValue=%s!\n",curParaValue,curNotifyNode->acsSetValue);
				if(strcmp(curParaValue, curNotifyNode->acsSetValue) !=0){
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						//tc_printf("\r\n Find Value changed node:%s\r\n",currentDeviceNode->name);
						tc_printf("midwareSearchChangedValue():node=%s,curParaValue=%s,acsSetValue=%s!\n",currentDeviceNode->name,curParaValue,curNotifyNode->acsSetValue);
					}
					ret = 1;
					midwarePushValueChangeStack(currentDeviceNode,notify);  //add to value change list
					midwareModifyNotifyNodeList(currentDeviceNode,notify);  //update notify node list value
				}
				os_free(curParaValue);
				curParaValue = NULL;
			}

			curNotifyNode = curNotifyNode->next;
		}

		if(notify == MWActive){
		MWActiveNotifyNodeListHeadP = NotifyNodeHead;	
		MWActiveNotifyNodeListTailP = NotifyNodeTail;	
		}
		else if(notify == TRActive){
			TRActiveNotifyNodeListHeadP = NotifyNodeHead;	
			TRActiveNotifyNodeListTailP = NotifyNodeTail;
		}
		else if(notify == ITMSActive){
			ITMSActiveNotifyNodeListHeadP = NotifyNodeHead;	
			ITMSActiveNotifyNodeListTailP = NotifyNodeTail;
		}
	}	
	return ret;
}


void midwareAddNotifyNodeList(NotifyDeviceNodeInfoPtr notifyNodeP, NotifyType notify){
	NotifyDeviceNodeInfoPtr NotifyNodeListHeadP = NULL;
	NotifyDeviceNodeInfoPtr NotifyNodeListTailP = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(notifyNodeP == NULL){
			tc_printf("\r\nAdded notify node is NULL!");
			return;
		}
		if(notify == MWActive){
			NotifyNodeListHeadP = MWActiveNotifyNodeListHeadP;
			NotifyNodeListTailP = MWActiveNotifyNodeListTailP;
		}
		else if(notify == TRActive){
			NotifyNodeListHeadP = TRActiveNotifyNodeListHeadP;
			NotifyNodeListTailP = TRActiveNotifyNodeListTailP;
		}
		else if(notify == ITMSActive){
			NotifyNodeListHeadP = ITMSActiveNotifyNodeListHeadP;
			NotifyNodeListTailP = ITMSActiveNotifyNodeListTailP;
		}
		else{
			tc_printf("midwareAddNotifyNodeList:notify err!\n");
			return;
		}
		
		if(NotifyNodeListHeadP == NULL)
			NotifyNodeListHeadP = notifyNodeP;
		else{
			NotifyNodeListTailP->next = notifyNodeP;
			notifyNodeP->prev = NotifyNodeListTailP;
		}

		NotifyNodeListTailP = notifyNodeP;

		if(dbg_cwmp >= DBG_CWMP_L2 ){
			tc_printf("midwareAddNotifyNodeList,notify=%d:%s!\n",notify,notifyNodeP->deviceNodeName);
		}	
		if(notify == MWActive){
			MWActiveNotifyNodeListHeadP = NotifyNodeListHeadP;
			MWActiveNotifyNodeListTailP =NotifyNodeListTailP;
		}
		else if(notify == TRActive){
			TRActiveNotifyNodeListHeadP = NotifyNodeListHeadP;
			TRActiveNotifyNodeListTailP = NotifyNodeListTailP;
		}
		else if(notify == ITMSActive){
			ITMSActiveNotifyNodeListHeadP = NotifyNodeListHeadP;
			ITMSActiveNotifyNodeListTailP = NotifyNodeListTailP;
		}
	}
}

void midwareDeleteNotifyNodeList(DeviceNode* deviceNode, NotifyType notify){
	NotifyDeviceNodeInfoPtr NotifyNodeListHeadP = NULL;
	NotifyDeviceNodeInfoPtr NotifyNodeListTailP = NULL;
	NotifyDeviceNodeInfo* curNotifyNode = NULL;
	NotifyDeviceNodeInfo* tempNotifyNode1= NULL, *tempNotifyNode2 = NULL;
	char deviceName[256] = "";

	if(!TCSUPPORT_CT_MIDWARE_VAL){
		return;
	}
	if(deviceNode == NULL){
		tc_printf("\r\nDelete device node is NULL!");
		return;
	}
	
	if(notify == MWActive){
		NotifyNodeListHeadP = MWActiveNotifyNodeListHeadP;
		NotifyNodeListTailP = MWActiveNotifyNodeListTailP;
		midwareGetDeviceNodeName(deviceNode, deviceName,rpc.MidwareRootPtr);
	}
	else if(notify == TRActive){
		NotifyNodeListHeadP = TRActiveNotifyNodeListHeadP;
		NotifyNodeListTailP = TRActiveNotifyNodeListTailP;
		cwmpGetDeviceNodeName(deviceNode, deviceName);
	}
	else if(notify == ITMSActive){
		NotifyNodeListHeadP = ITMSActiveNotifyNodeListHeadP;
		NotifyNodeListTailP = ITMSActiveNotifyNodeListTailP;
		cwmpGetDeviceNodeName(deviceNode, deviceName);
	}
	else{
		tc_printf("midwareDeleteNotifyNodeList:notify err!\n");
		return;
	}

	curNotifyNode = NotifyNodeListHeadP;
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("midwareDeleteNotifyNodeList:%s, notify:%d!\n",deviceName,notify);
	}
	while(curNotifyNode){
		if(strcmp(curNotifyNode->deviceNodeName, deviceName) == 0){
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\nDelete Notify node:%s\r\n",deviceName);
			}
			cwmpFreeNotifyNode(&curNotifyNode, &NotifyNodeListHeadP, &NotifyNodeListTailP);
			goto RelinkList;
		}
		curNotifyNode = curNotifyNode->next;
	}	
RelinkList:
	if(notify == MWActive){
		MWActiveNotifyNodeListHeadP = NotifyNodeListHeadP;
		MWActiveNotifyNodeListTailP = NotifyNodeListTailP;
	}
	else if(notify == TRActive){
		TRActiveNotifyNodeListHeadP = NotifyNodeListHeadP;
		TRActiveNotifyNodeListTailP = NotifyNodeListTailP;
	}
	else if(notify == ITMSActive){
		ITMSActiveNotifyNodeListHeadP = NotifyNodeListHeadP;
		ITMSActiveNotifyNodeListTailP = NotifyNodeListTailP;
	}
}

int8 midwareClearNotifyNodeList(NotifyType notify){
	NotifyDeviceNodeInfo *curNotifyNode = NULL, *nextNotifyNode = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(notify == MWActive){
			curNotifyNode = MWActiveNotifyNodeListHeadP;
		}
		else if(notify == TRActive){
			curNotifyNode = TRActiveNotifyNodeListHeadP;
		}
		else if(notify == ITMSActive){
			curNotifyNode = ITMSActiveNotifyNodeListHeadP;
		}
		else{
			tc_printf("\r\nInvalid notification for delete notify list!");
			return 0;
		}

		while(curNotifyNode){
			nextNotifyNode = curNotifyNode->next;
			
			if(curNotifyNode->acsSetValue){
				os_free(curNotifyNode->acsSetValue);
				curNotifyNode->acsSetValue = NULL;
			}

			if(curNotifyNode->deviceNodeName){
				os_free(curNotifyNode->deviceNodeName);
				curNotifyNode->deviceNodeName = NULL;
			}
			
			os_free(curNotifyNode);
			
			curNotifyNode = nextNotifyNode;
		}

		if(notify == MWActive){
			MWActiveNotifyNodeListHeadP = NULL;
			MWActiveNotifyNodeListTailP = NULL;
		}
		else if(notify == TRActive){
			TRActiveNotifyNodeListHeadP = NULL;
			TRActiveNotifyNodeListTailP = NULL;
		}
		else if(notify == ITMSActive){
			ITMSActiveNotifyNodeListHeadP = NULL;
			ITMSActiveNotifyNodeListTailP = NULL;
		}
	}	
	return 0;
}

int8 midwareBeForcedNotifyNode(DeviceNodePtr deviceNode){
	int8 i = 0;
	char deviceNodeName[160] = "";
	char* forcedName = MWforcedNotifyNode[0];
	DeviceNodePtr currentDeviceNode = NULL;
	char *conRequestURLPtr = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){	
		if(deviceNode == NULL){
			tc_printf("\r\n[midwareBeForcedNotifyNode] device node is NULL!");
			return 0;
		}
			
		midwareGetDeviceNodeName(deviceNode, deviceNodeName, rpc.MidwareRootPtr);
		tc_printf("midwareBeForcedNotifyNode:%s!\n",deviceNodeName);
		while(forcedName){
			if(strcmp(deviceNodeName, forcedName) == 0)
				return 1;
			i++;
			forcedName = MWforcedNotifyNode[i];
		}
	}
	return 0;
}

int midwareCreateDeviceNodeName(DeviceNodePtr deviceNode, char** deviceNodeNameP){
	char dName[256] = "";
	char* deviceName = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceNode == NULL){
			tc_printf("\r\nDevice Node is NULL for Create Node Name!\r\n");
			return -1;
		}
		
		midwareGetDeviceNodeName(deviceNode, dName, rpc.MidwareRootPtr); 
		if((deviceName = os_malloc(strlen(dName)+1)) != NULL){
			strcpy(deviceName, dName);
		}
		else{
			tc_printf("[midwareCreateDeviceNodeName]Malloc device Name fails!\r\n");
			return -1;
		}

		*deviceNodeNameP = deviceName;
	}	
	return 0;
}

int8 midwareCreateNotifyNode(DeviceNodePtr deviceNode, NotifyDeviceNodeInfoPtr* notifyNodeP){
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;
	DeviceNodePtr currentDeviceNode = deviceNode;
	ParameterType paraType;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceNode == NULL){
			tc_printf("\r\nDevice Node is NULL for create Notify Node!\r\n");
			return -1;
		}

		if((tempNotifyNodeP = (NotifyDeviceNodeInfoPtr)os_malloc(sizeof(NotifyDeviceNodeInfo))) != NULL){
			memset(tempNotifyNodeP, 0, sizeof(NotifyDeviceNodeInfo));
			if(currentDeviceNode->getValueInfo){
				int8 ret;
				ret = currentDeviceNode->getValueInfo(&tempNotifyNodeP->acsSetValue, &paraType, currentDeviceNode);
				if(ret != 0){
					tc_printf("\r\n GetValueInfo fails!");
					os_free(tempNotifyNodeP);
					return -1;									
				}
			}	
			else{
				tc_printf("\r\n GetValueInfo func is NULL!");
				os_free(tempNotifyNodeP);
				return -1;
			}

			if(midwareCreateDeviceNodeName(currentDeviceNode, &(tempNotifyNodeP->deviceNodeName)) == -1){
				tc_printf("\r\nCreate Device Node Name Fails!\r\n");
				if(tempNotifyNodeP->acsSetValue)
					os_free(tempNotifyNodeP->acsSetValue);
				os_free(tempNotifyNodeP);
				tempNotifyNodeP = NULL;
				return -1; //xzwang_060815
			}	
			tc_printf("midwareCreateNotifyNode:%s:%s!\n",tempNotifyNodeP->deviceNodeName,tempNotifyNodeP->acsSetValue);
		#if 0	
			tempNotifyNodeP->deviceNode = currentDeviceNode;
		#endif
		}
		else{
			tc_printf("\r\n malloc NotifyDeviceNodeInfo fails!");
			return -1;
		}

		*notifyNodeP = tempNotifyNodeP;
	}
	return 0;
}	

int8 midwareMultipulateNotifyListByNewAttr(DeviceNodePtr deviceNode, uint8 newNotification, NotifyType notify){
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceNode == NULL){
			tc_printf("\r\n[cwmpMultipulateNotifyListByNewAttr]device node is NULL!");
			return  -1;
		}

		if((notify == MWActive) && midwareBeForcedNotifyNode(deviceNode)){
			tc_printf("BeForcedNotifyNode Reject to change forced node in notify list!\n");
			return 0;
		}
		
		if(deviceNode->getAttribute == NULL){
			tc_printf("without get attribute func for node:%s!\n",deviceNode->name);
			return 0;
		}
		//always delete node from notification list first and then add it into new list. xzwang_060907
		midwareDeleteNotifyNodeList(deviceNode, notify);
		if( (newNotification &0x08) && (notify == MWActive)){
			if(midwareCreateNotifyNode(deviceNode, &tempNotifyNodeP) == 0){
				midwareAddNotifyNodeList(tempNotifyNodeP, notify);
			}
		}
		if(((newNotification & 0x02) && (notify == ITMSActive)) || ((newNotification & 0x08) && (notify == TRActive))){
			if(cwmpCreateNotifyNode(deviceNode, &tempNotifyNodeP) == 0){
				midwareAddNotifyNodeList(tempNotifyNodeP, notify);
			}
		}
	}
	return 0;
}

int8 midwareMultipulateNotifyListByNewValue(DeviceNodePtr deviceNode){
	uint8 notification = 0;
	AccessEntry* acl = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){
		if(deviceNode == NULL){
			tc_printf("\r\n[midwareMultipulateNotifyListByNewValue]device node is NULL!");
			return -1;
		}
#if 0	
		if(cwmpBeForcedNotifyNode(deviceNode)){
			cwmpModifyNotifyNodeList(deviceNode, Active);
			return 0;
		}
#endif
		if(deviceNode->getAttribute == NULL){
			if(dbg_cwmp >= DBG_CWMP_L2 ){
				tc_printf("\r\nwithout get attribute  func for node:%s\r\n",deviceNode->name);
			}
			return 0;
		}
		
		deviceNode->getAttribute(&notification, &acl, deviceNode);
		if(acl != NULL){
		        os_free(acl);
			acl = NULL;
		}

		if(notification != 0){
			if(notification & 0x08)
			midwareModifyNotifyNodeList(deviceNode, TRActive);
			if(notification & 0x07)
			midwareModifyNotifyNodeList(deviceNode, ITMSActive);
		}
	}
	return 0;
}

int8 midwareInitNotifyNodeList(void){
	DeviceNodePtr currentDeviceNode = rpc.MidwareRootPtr;
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;
	uint8 notification = 0;
	AccessEntry* acl = NULL;

	if(TCSUPPORT_CT_MIDWARE_VAL){	
		tc_printf("midwareInitNotifyNodeList!\n");
		while(currentDeviceNode != NULL){

			while(currentDeviceNode != NULL){
				if(currentDeviceNode->type != CWMP_OBJECT){
				#if 0
					if(currentDeviceNode->getAttribute){
						currentDeviceNode->getAttribute(&notification, &acl, currentDeviceNode);
						if(acl){
						        os_free(acl);
							acl = NULL;
						}
					
						if(notification){
							if(!midwareBeForcedNotifyNode(currentDeviceNode)){
								if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
									cwmpAddNotifyNodeList(tempNotifyNodeP, notification);
									if(dbg_cwmp >= DBG_CWMP_L2 ){
										tc_printf("\r\n  Notify node:%s\r\n",currentDeviceNode->name);
									}
								}
							}
						}
					}
					else{
						if(dbg_cwmp >= DBG_CWMP_L2 ){
							tc_printf("\r\n  without attribute func for device node:%s\r\n",currentDeviceNode->name);
						}
					}
				#endif
					if(midwareBeForcedNotifyNode(currentDeviceNode)){
						if(midwareCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
							midwareAddNotifyNodeList(tempNotifyNodeP,MWActive);
							if(dbg_cwmp >= DBG_CWMP_L2 ){
								tc_printf("midwareInitNotifyNodeList Forced notify node:%s\r\n",currentDeviceNode->name);
							}
						}
					}
				}

				cwmpPushDeviceStack(currentDeviceNode);
				currentDeviceNode = currentDeviceNode->firstChild;
				
			}

			while(currentDeviceNode == NULL && deviceNodeStackHead != NULL){
				cwmpPopDeviceStack(&currentDeviceNode);
				if(deviceNodeStackHead != NULL)
					currentDeviceNode = currentDeviceNode->nextSibling;
				else
					currentDeviceNode = NULL;
			}
		}
		/* add tr069 node for informMW*/
		
		currentDeviceNode = rpc.IGDDeviceRootPtr;	
		while(currentDeviceNode != NULL){

			while(currentDeviceNode != NULL){
				if(currentDeviceNode->type != CWMP_OBJECT){
					if(currentDeviceNode->getAttribute){
						currentDeviceNode->getAttribute(&notification, &acl, currentDeviceNode);
						if(acl){
						        os_free(acl);
							acl = NULL;
						}

						if(notification & 0x08){
						#if 0	
							if(!cwmpBeForcedNotifyNode(currentDeviceNode)){
								if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
									cwmpAddNotifyNodeList(tempNotifyNodeP, notification);
									if(dbg_cwmp >= DBG_CWMP_L2 ){
										tc_printf("\r\n  Notify node:%s\r\n",currentDeviceNode->name);
									}
								}
							}
						#endif
							if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
								midwareAddNotifyNodeList(tempNotifyNodeP,TRActive);
								if(dbg_cwmp >= DBG_CWMP_L2 ){
									tc_printf("MW Notify node:%s,notify:%d\r\n",currentDeviceNode->name,notification);
								}
							}
						}
						if((iTr069Enable == 0) &&(notification & 0x07)){
						#if 1	
							//if(!cwmpBeForcedNotifyNode(currentDeviceNode)){
								if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
									midwareAddNotifyNodeList(tempNotifyNodeP,ITMSActive);
									if(dbg_cwmp >= DBG_CWMP_L2 ){
										tc_printf("ITMS Notify node:%s,notify:%d!\r\n",currentDeviceNode->name,notification);
									}
								}
							//}
						#endif
						}
					}
					else{
						if(dbg_cwmp >= DBG_CWMP_L2 ){
							tc_printf("\r\n  without attribute func for device node:%s\r\n",currentDeviceNode->name);
						}
					}
				#if 0
					if(cwmpBeForcedNotifyNode(currentDeviceNode)){
						if(cwmpCreateNotifyNode(currentDeviceNode, &tempNotifyNodeP) == 0){
							cwmpAddNotifyNodeList(tempNotifyNodeP, Active);
							if(dbg_cwmp >= DBG_CWMP_L2 ){
								tc_printf("\r\n  Forced notify node:%s\r\n",currentDeviceNode->name);
							}
						}
					}
				#endif
				}

				cwmpPushDeviceStack(currentDeviceNode);
				currentDeviceNode = currentDeviceNode->firstChild;
				
			}

			while(currentDeviceNode == NULL && deviceNodeStackHead != NULL){
				cwmpPopDeviceStack(&currentDeviceNode);
				if(deviceNodeStackHead != NULL)
					currentDeviceNode = currentDeviceNode->nextSibling;
				else
					currentDeviceNode = NULL;
			}
		}
	}	
	return 0;
}

#define Alarm_Node "AlarmLog_Entry%d"
#define Cwmp_AlarmInfo "InternetGatewayDevice.X_CU_Function.Log.AlarmInfo.%d.%s"
int checkCurrIndex(void) {
	char tmpBuf[16] = {0};
	int currIndex = 0;

	if(TCSUPPORT_C1_CUC_VAL && TCSUPPORT_SYSLOG_VAL){
		if(tcapi_get("AlarmLog_Common", "CurrALogIndex", tmpBuf) != 0 )
			return -1;
		if(strlen(tmpBuf) != 0)
			currIndex = atoi(tmpBuf);
	}
	return currIndex;	
}

int8 cwmpAddAlarmParameters(void){
	char nodeBuf[128] = {0};
	char tmpBuf[128]={0};
	char buf[32] ={0};
	int8 state = 0;
	int currAlogIndex = 0;

	if(TCSUPPORT_C1_CUC_VAL && TCSUPPORT_SYSLOG_VAL){
	   	currAlogIndex = checkCurrIndex();
		if(currAlogIndex == -1){
			return -1;
		}
		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));
		if(tcapi_get(buf,"AlarmID", nodeBuf) != 0 ){
               		tc_printf("\r\n==>cwmpAddAlarmParameters AlarmID fails!"); 		 
			strcpy(nodeBuf,"");
		}
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "AlarmID");			
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, UnsignedIntType );
		if (state!=0) {
				return -1;
			}

		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));		
		if(tcapi_get(buf,"AlarmCode", nodeBuf) != 0 ){
                	tc_printf("\r\n==>cwmpAddAlarmParameters AlarmCode fails!"); 
			strcpy(nodeBuf,"");
		}		
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "AlarmCode");		
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, UnsignedIntType );
			if (state!=0) {
				return -1;

                        }

		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));		
		if(tcapi_get(buf,"AlarmStatus", nodeBuf) != 0 ){
                	tc_printf("\r\n==>cwmpAddAlarmParameters AlarmStatus fails!");
			strcpy(nodeBuf,"");
		}				
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "AlarmStatus");			
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, UnsignedIntType );
			if (state!=0) {
				return -1;	

                         }

		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));		
		if(tcapi_get(buf,"PerceivedSeverity", nodeBuf) != 0 ){
            		tc_printf("\r\n==>cwmpAddAlarmParameters PerceivedSeverity fails!");
			strcpy(nodeBuf,"");
		}						
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "PerceivedSeverity");		
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, UnsignedIntType );
			if (state!=0) {
				return -1;

			}		

		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));		
		if(tcapi_get(buf,"AlarmRaisedTime", nodeBuf) != 0 ){
           		tc_printf("\r\n==>cwmpAddAlarmParameters AlarmRaisedTime fails!");
			strcpy(nodeBuf,"");
		}					
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "AlarmRaisedTime");
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, DateTimeType );
			if (state!=0) {
				return -1;

			}	

		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));		
		if(tcapi_get(buf,"AlarmClearedTime", nodeBuf) != 0 ){
             		tc_printf("\r\n==>cwmpAddAlarmParameters AlarmClearedTime fails!");
			strcpy(nodeBuf,"");
		}					
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "AlarmClearedTime");
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, DateTimeType );		
		if (state!=0) {
				return -1;

			}

		memset(tmpBuf, 0, sizeof(tmpBuf));
		memset(buf,0,sizeof(buf));
		sprintf(buf,Alarm_Node,currAlogIndex);
		memset(nodeBuf, 0, sizeof(nodeBuf));		
		if(tcapi_get(buf,"AlarmDetail", nodeBuf) != 0 ){
                	tc_printf("\r\n==>cwmpAddAlarmParameters AlarmDetail fails!");
			strcpy(nodeBuf,"");
		}				
		sprintf(tmpBuf, Cwmp_AlarmInfo, currAlogIndex, "AlarmDetail");
		state =cwmpAddInformParameter(tmpBuf, nodeBuf, StringType );
		if (state!=0) {
			return -1;
		}
	}
	return 0;
}

int cwmpCreateDeviceNodeName(DeviceNodePtr deviceNode, char** deviceNodeNameP){
	char dName[256] = "";
	char* deviceName = NULL;

	if(deviceNode == NULL){
		tc_printf("\r\nDevice Node is NULL for Create Node Name!\r\n");
		return -1;
	}
	
	cwmpGetDeviceNodeName(deviceNode, dName);

	if((deviceName = os_malloc(strlen(dName)+1)) != NULL){
		strcpy(deviceName, dName);
	}
	else{
		tc_printf("[cwmpCreateDeviceNodeName]Malloc device Name fails!\r\n");
		return -1;
	}

	*deviceNodeNameP = deviceName;
	
	return 0;
}


#if CWMP_USING_MBUF_FOR_TXBUF
/*********************** Using  Mbuf For Storing TxBuf Message *************************************/

mbuf_t  *cwmpMbufListHead = NULL, *cwmpMbufListTail = NULL, *cwmpCurrentMbuf = NULL;
uint32  mbufDataExportIndex = 0;
uint8    cwmpAllocMbufFail = 0;
uint8   cwmpCopyFirstMbufFlag = 1;
mbuf_t  *cwmpParaMbufListHead = NULL, *cwmpParaMbufListTail = NULL; 

mbuf_t *cwmpGetNextMbuf(mbuf_t* curMbuf){
	//modify by brian for remving warning
	if(curMbuf != (mbuf_t*)NULL)
		return (mbuf_t*)curMbuf->qlink.qnext;
	else
		return NULL;
}

void  cwmpAddMbufList(mbuf_t *mbuf){
	//modify by brian for remving warning
	if(cwmpMbufListHead == (mbuf_t*)NULL)
		cwmpMbufListHead = mbuf;
	else
		cwmpMbufListTail->qlink.qnext = (qlink_t*)mbuf;
	
	cwmpMbufListTail = mbuf;
}

void cwmpResetMbufList(void){
	mbuf_t *curMbuf = cwmpMbufListHead;
	mbuf_t *nextMbuf = NULL;

	//i_state = dirps();
	pthread_mutex_lock(&value_change_lock);
	//modify by brian for remving warning
	while(curMbuf != (mbuf_t*)NULL){
		nextMbuf = (mbuf_t*)curMbuf->qlink.qnext;
		free_p(curMbuf);
		curMbuf = nextMbuf;
	}

	curMbuf = cwmpParaMbufListHead;
	while(curMbuf != (mbuf_t*)NULL){
		nextMbuf = (mbuf_t*)curMbuf->qlink.qnext;
		free_p(curMbuf);
		curMbuf = nextMbuf;
	}

	cwmpMbufListHead = cwmpMbufListTail = NULL;
	cwmpParaMbufListHead = cwmpParaMbufListTail = NULL;
	cwmpCurrentMbuf = NULL;
	rpc.txBuf = NULL;
	pthread_mutex_unlock(&value_change_lock);
}

int32 cwmpCountMbufListDataLength(void){
	mbuf_t *curMbuf = cwmpMbufListHead;
	uint32 len = 0;
	//modify by brian for remving warning
	while(curMbuf != (mbuf_t*)NULL){
		len += curMbuf->cnt;
		curMbuf = (mbuf_t*)curMbuf->qlink.qnext;
	}

	return len;
}

int8 cwmpCopyStringToMbuf(char* Msg){
	uint32 len = 0, copyLen = 0, leftLen = 0;
	mbuf_t * newMbuf = NULL;
	
	if(Msg == NULL){
		tc_printf("\r\nThe string for copying to mbuf data is NULL!");
		return -1;
	}	
	
	len = strlen(Msg);
	if(len == 0)
		return 0;

	pthread_mutex_lock(&value_change_lock);
	if(cwmpMbufListHead == (mbuf_t*)NULL){
		if((newMbuf= alloc_cache_mbuf2k()) == (mbuf_t*)NULL){
			pthread_mutex_unlock(&value_change_lock);
			tc_printf("\r\nAllocate mbuf2k from cache fails!");
			goto ALLOC_MBUF_FAIL;
		}
		cwmpAddMbufList(newMbuf);
	}
	
	if( (cwmpMbufListTail->size - cwmpMbufListTail->cnt) >= len){
		memcpy(cwmpMbufListTail->data + cwmpMbufListTail->cnt, Msg, len);
		cwmpMbufListTail->cnt += len;
		if(cwmpMbufListTail->cnt ==  cwmpMbufListTail->size){
			if((newMbuf= alloc_cache_mbuf2k()) == (mbuf_t*)NULL){
				pthread_mutex_unlock(&value_change_lock);
				tc_printf("\r\nAllocate mbuf2k from cache fails!");
				goto ALLOC_MBUF_FAIL;
			}
			cwmpAddMbufList(newMbuf);
		}	
	}
	else{
		memcpy(cwmpMbufListTail->data + cwmpMbufListTail->cnt, Msg, cwmpMbufListTail->size - cwmpMbufListTail->cnt);
		leftLen = len - (cwmpMbufListTail->size - cwmpMbufListTail->cnt);
		cwmpMbufListTail->cnt = cwmpMbufListTail->size;

		while(leftLen > 0){
			if((newMbuf= alloc_cache_mbuf2k()) == (mbuf_t*)NULL){
				pthread_mutex_unlock(&value_change_lock);
				tc_printf("\r\nAllocate mbuf2k from cache fails!");
				goto ALLOC_MBUF_FAIL;
			}
			cwmpAddMbufList(newMbuf);

			if(leftLen < cwmpMbufListTail->size){
				memcpy(cwmpMbufListTail->data, Msg + (len-leftLen), leftLen);
				cwmpMbufListTail->cnt += leftLen;
				leftLen = 0;
			}
			else{
				memcpy(cwmpMbufListTail->data, Msg + (len-leftLen), cwmpMbufListTail->size);
				cwmpMbufListTail->cnt = cwmpMbufListTail->size;
				leftLen -= cwmpMbufListTail->size;
			}
		}
	}
	pthread_mutex_unlock(&value_change_lock);
	return 0;

	ALLOC_MBUF_FAIL:
		cwmpAllocMbufFail = 1;
		return -1;
}


int8 cwmpCopyMbufToString(char* sBuf, uint32 sBufSize){
	uint32 leftSize = 0, bufcnt = 0;

	if(sBuf == NULL){
		tc_printf("\r\nThe string for storing mbuf data is NULL!");
		return -1;
	}	
	
	#ifdef LINOS_ENV
	if(strlen(sBuf) > (sBufSize -1)){
		tc_printf("\r\nThe string for storing mbuf data is too short!");
		return -1;
	}
	#endif	
	
	memset(sBuf, 0, sBufSize);
	pthread_mutex_lock(&value_change_lock);
	if(cwmpCurrentMbuf == NULL){
		if(cwmpCopyFirstMbufFlag == 1){
			cwmpCopyFirstMbufFlag = 0;
			cwmpCurrentMbuf = cwmpMbufListHead;
			mbufDataExportIndex = 0;
			if(cwmpCurrentMbuf == NULL){
				if(dbg_cwmp >= DBG_CWMP_L2){
					tc_printf("\r\nMbuf list head is NULL!");
				}		
				pthread_mutex_unlock(&value_change_lock);
				return -1;
			}	
		}
		else{
			pthread_mutex_unlock(&value_change_lock);
			tc_printf("\r\nCurrent mbuf is NULL!");
			return -1;
		}	
	}

	if(sBufSize <= ( cwmpCurrentMbuf->cnt - mbufDataExportIndex)){
		memcpy(sBuf, cwmpCurrentMbuf->data + mbufDataExportIndex, sBufSize);
		mbufDataExportIndex += sBufSize;
		if(mbufDataExportIndex == cwmpCurrentMbuf->cnt){
			cwmpCurrentMbuf = cwmpGetNextMbuf(cwmpCurrentMbuf);
			mbufDataExportIndex = 0;
		}
	}
	else{
		memcpy(sBuf, cwmpCurrentMbuf->data + mbufDataExportIndex, cwmpCurrentMbuf->cnt - mbufDataExportIndex);
		bufcnt = cwmpCurrentMbuf->cnt - mbufDataExportIndex;
		leftSize = sBufSize - bufcnt;
		while(leftSize > 0){
			cwmpCurrentMbuf = cwmpGetNextMbuf(cwmpCurrentMbuf);
			if(cwmpCurrentMbuf == NULL){
				mbufDataExportIndex = 0;
				pthread_mutex_unlock(&value_change_lock);			
				return 0;
			}
			if(leftSize <= cwmpCurrentMbuf->cnt){
				memcpy(sBuf + bufcnt, cwmpCurrentMbuf->data, leftSize);
				mbufDataExportIndex = leftSize;
				leftSize = 0;
				if(mbufDataExportIndex == cwmpCurrentMbuf->cnt){
					cwmpCurrentMbuf = cwmpGetNextMbuf(cwmpCurrentMbuf);
					mbufDataExportIndex = 0;
				}
			}
			else{
				memcpy(sBuf + bufcnt, cwmpCurrentMbuf->data, cwmpCurrentMbuf->cnt);
				mbufDataExportIndex = 0;
				leftSize -=cwmpCurrentMbuf->cnt;
			}
			
		}
	}
	pthread_mutex_unlock(&value_change_lock);
	return 0;
	
}

mbuf_t *cwmpGetMbufListHead(void){
	return cwmpMbufListHead;
}

uint8 cwmpGetAllocMbufFlag(void){
	return cwmpAllocMbufFail;
}

void cwmpSetAllocMbufFlag(uint8 value){
	cwmpAllocMbufFail = value;
}

void cwmpSetFirstMbufFlag(uint8 value){
	cwmpCopyFirstMbufFlag = value;
}

void cwmpSetParaMbufList(void){
	pthread_mutex_lock(&value_change_lock);
	cwmpParaMbufListHead = cwmpMbufListHead;
	cwmpParaMbufListTail = cwmpMbufListTail;
	
	cwmpMbufListHead = NULL;
	cwmpMbufListTail = NULL;
	pthread_mutex_unlock(&value_change_lock);
}

void cwmpLinkParaMbufList(void){

	pthread_mutex_lock(&value_change_lock);
	if(cwmpParaMbufListHead == NULL || cwmpParaMbufListTail == NULL){
		cwmpParaMbufListHead = NULL;
		cwmpParaMbufListTail = NULL;
			pthread_mutex_unlock(&value_change_lock);
		return;
	}

	if(cwmpMbufListHead == NULL){
		cwmpMbufListHead = cwmpParaMbufListHead;
		cwmpMbufListTail = cwmpParaMbufListTail;
	}
	else{
		cwmpMbufListTail->qlink.qnext = (qlink_t*)cwmpParaMbufListHead;
		cwmpMbufListTail = cwmpParaMbufListTail;
	}

	cwmpParaMbufListHead = NULL;
	cwmpParaMbufListTail = NULL;
	pthread_mutex_unlock(&value_change_lock);
}
#endif



#if CWMP_REDUCE_CODE
/*********************** Old code *************************/ 

void cwmpRpcNodeTreeRecursive(IXML_Node *nodeptr)
{
	IXML_Node *sibling = NULL;

	if( nodeptr != NULL ) {
		cwmpRpcNodeDump(nodeptr);
#if 1 //xzwang_060324	
		 if( nodeptr->firstAttr != NULL ) {
            	cwmpRpcNodeTreeRecursive(nodeptr->firstAttr);
		  }
#endif	 
		cwmpRpcNodeTreeRecursive(ixmlNode_getFirstChild(nodeptr));
		cwmpRpcNodeTreeRecursive(ixmlNode_getNextSibling(nodeptr));
	}
}
#endif

#if CWMP_REDUCE_CODE
static int cwmpRpcNodeDump(IXML_Node *tmpNode)
{

	const char *str;
	
	printf("\n\nNode****");
	str = ixmlNode_getNodeName(tmpNode);
	printf("\nname = %s",str);
	str = ixmlNode_getPrefix(tmpNode);
	printf("\nprefix = %s",str);
	str = ixmlNode_getNodeValue(tmpNode);
	printf("\nvalue = %s",str);
	str = ixmlNode_getNamespaceURI(tmpNode);
	printf("\nnamespace = %s",str);
	str = ixmlNode_getLocalName(tmpNode);
	printf("\nlocal_name = %s",str);
	printf("\ntype = %d",ixmlNode_getNodeType(tmpNode));

	return 0;
}
#endif

uint8 getCwmpRebootFlag(uint8* rebootFlag)
{
	uint8 RebootFlag_flash;
	RebootFlag_flash = spSysParam.tcCwmpAttr->vRebootFlag;
	if(rebootFlag != NULL)
		*rebootFlag = cwmpRebootFlag;
	return RebootFlag_flash;
}	
	
uint8 setCwmpRebootFlag(uint8 flag)
{	
	spSysParam.tcCwmpAttr->vRebootFlag = flag;
	cwmpRebootFlag = flag;
	//SaveCwmpFlags();
	//spSave();
	return 0;
}	

int getACSFaultCode(void)
{
	return gACSFaultCode;
}

void setACSFaultCode(int value)
{
	gACSFaultCode = value;
	return;
}

int8 cwmpGetFaultParaList(void){
	DeviceNodePtr currentDeviceNode = rpc.IGDDeviceRootPtr;
	NotifyDeviceNodeInfoPtr tempNotifyNodeP = NULL;
	char* value = NULL;
	ParameterType valueType;
	uint8 notification = 0;
	AccessEntry* acl = NULL;
	char paraName[256] = "";

	if(currentDeviceNode == NULL){
		cwmpInitParameterTree();
		currentDeviceNode = rpc.IGDDeviceRootPtr;
	}

	while(currentDeviceNode != NULL){

		while(currentDeviceNode != NULL){
			if(currentDeviceNode->type != CWMP_OBJECT){

				if(currentDeviceNode->getValueInfo){
					if(currentDeviceNode->getValueInfo(&value, &valueType, currentDeviceNode) != 0)
					{
						if(dbg_cwmp >= DBG_CWMP_L2 ){
							cwmpGetDeviceNodeName(currentDeviceNode, paraName);
							tc_printf("Get value with fault for device node:%s\r\n",paraName);
						}
					}


					if(value){
						os_free(value);
						value = NULL;
					}
				}
				else{
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						cwmpGetDeviceNodeName(currentDeviceNode, paraName);
						tc_printf("Without get value func for device node:%s\r\n",paraName);
					}
				}

				if(currentDeviceNode->getAttribute){
					if(currentDeviceNode->getAttribute(&notification, &acl, currentDeviceNode) != 0){
						if(dbg_cwmp >= DBG_CWMP_L2 ){
							cwmpGetDeviceNodeName(currentDeviceNode, paraName);
							tc_printf("Get attribute with fault for device node:%s\r\n",paraName);
						}
					}

					if(acl){
						os_free(acl);
						acl = NULL;
					}
				}
				else{
					if(dbg_cwmp >= DBG_CWMP_L2 ){
						cwmpGetDeviceNodeName(currentDeviceNode, paraName);
						tc_printf("Without get attribute func for device node:%s\r\n",paraName);
					}
				}
				
			}
			
			cwmpPushDeviceStack(currentDeviceNode);
			currentDeviceNode = currentDeviceNode->firstChild;
			
		}

		while(currentDeviceNode == NULL && deviceNodeStackHead != NULL){
			cwmpPopDeviceStack(&currentDeviceNode);
			if(deviceNodeStackHead != NULL)
				currentDeviceNode = currentDeviceNode->nextSibling;
			else
				currentDeviceNode = NULL;
		}
	}

	return 0;
}


char * TimeFormatConvert(uint32 time)
{
	uint16	years,months,days,hrs,mins,secs;
	int	minus;
	int32 uptime;
	
	uptime=time;
	secs = (uint16) (uptime % 60);
	uptime /= 60;
	mins = (uint16) (uptime % 60);
	uptime /= 60;
	hrs = (uint16) (uptime % 24);
	uptime /= 24;
	days = (uint16) uptime;
	//memset(chartime, 0, sizeof(chartime));
	sprintf(chartime,"0001-01-%02uT%02u:%02u:%02u",days+1,hrs,mins,secs);
	printf("\r\nTimeFormatConvert: now uptime is [%d] time is [%s] \n", uptime, chartime);  
	return chartime;
}

/*______________________________________________________________________________
**  TimeFormatConvert_improve
**
**  descriptions: 
		to conver the time into special formate
**  execution sequence:
**  commands process:
**  parameters:
**  local:
**  global:
**  return:
**  called by:
		cwmpBuildFileTransferResponse
		cwmpBuildTransferCompleteBodyStruct
**  call:
**____________________________________________________________________________*/
char * TimeFormatConvert_improve(uint32 time)
{
	uint16	years,months,days,hrs,mins,secs;
	int	minus;
	struct tm *p;

	if(TCSUPPORT_CWMP_PRECISE_TIME_VAL){
		p=gmtime(&time);
		sprintf(chartime,"%04u-%02u-%02uT%02u:%02u:%02u",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
		//tcdbg_printf("\r\nTimeFormatConvert:  time is [%s] \n",  chartime);  
	}
	return chartime;
}

/* Added by hqfeng, 2006.08.09*/
int  cwmpCheckDeviceNode(DeviceNodePtr deviceNode, ParameterValueStructPtr reqPara)
{
	char faultString[FaultStringLength];
	int 	rtnStatus = CWMP_E_SUCCESS;
	int   rtncode;
	int	op_flag = 1;

	if(reqPara == NULL){
		return -1;
	}

	if(deviceNode == NULL){
		strcpy(faultString, "Invalid parameter name");
		rtnStatus = InvalidParameterName;
		if(TCSUPPORT_CT_MIDWARE_VAL)
			op_flag = iTr069Enable;
		if(op_flag){
			rtncode = cwmpAddSetParameterValuesFaultList(reqPara->name, rtnStatus, faultString);
			if(rtncode != 0)
				return rtncode;
		}
		return rtnStatus;
	}
	if(deviceNode->type == CWMP_OBJECT){
		strcpy(faultString, "Invalid parameter name");
		rtnStatus = InvalidParameterName;
		if(TCSUPPORT_CT_MIDWARE_VAL)
			op_flag = iTr069Enable;
		if(op_flag){
			rtncode = cwmpAddSetParameterValuesFaultList(reqPara->name, rtnStatus, faultString);
			if(rtncode != 0)
				return rtncode;
		}
		return rtnStatus;
	}
	if(deviceNode->accessType == ReadOnly){
		strcpy(faultString, "Attempt to set a non-writable parameter");
		rtnStatus = SetReadOnlyParameter;
		if(TCSUPPORT_CT_MIDWARE_VAL)
			op_flag = iTr069Enable;
		if(op_flag){
			rtncode = cwmpAddSetParameterValuesFaultList(reqPara->name, rtnStatus, faultString);
			if(rtncode != 0)
				return rtncode;
		}
		return rtnStatus;
	}
	
	/* 
		if the value type is not taken out in the packet from some ACSs,
                we will set CI_CMD: sys cwmp donovaluetype 1 to add the value type 
                with the default value type in a work around way so that we can 
                pass these specical ACSs.
	*/
	if(deviceNode->type != reqPara->valueInfoPtr->type)
	{
		if (getNoValueTypeFlag())
		{
			reqPara->valueInfoPtr->type = deviceNode->type;
		}
		else
		{
			tc_printf("\r\n deviceNode->type != reqPara->valueInfoPtr->type");
		}	
	}

	if(deviceNode->type != reqPara->valueInfoPtr->type){
		strcpy(faultString, "Invalid parameter type");
		rtnStatus = InvalidParameterType;
		rtncode = cwmpAddSetParameterValuesFaultList(reqPara->name, rtnStatus, faultString);
		if(rtncode != 0)
			return rtncode;
		return rtnStatus;
	}

	return rtnStatus;
}

/* Added by hqfeng, 2006.08.09*/
static void  freeRPCFaultStruct( RPCFault **rpcFaultPtr)
{
	RPCFaultPtr pRpcFaultPtr;

	pRpcFaultPtr = *rpcFaultPtr;
	if(pRpcFaultPtr == NULL){
		return;
	}

	if(pRpcFaultPtr->rpcErrorType){
		os_free(pRpcFaultPtr->rpcErrorType);
		pRpcFaultPtr->rpcErrorType= NULL;
	}
	if(pRpcFaultPtr->parameterName){
		os_free(pRpcFaultPtr->parameterName);	
		pRpcFaultPtr->parameterName = NULL;
	}
	if(pRpcFaultPtr->faultString){
		os_free(pRpcFaultPtr->faultString);		
		pRpcFaultPtr->faultString = NULL;
	}
	if(pRpcFaultPtr){
		os_free(pRpcFaultPtr);	
		pRpcFaultPtr = NULL;
	}

	*rpcFaultPtr = NULL;

	return;
	
}

/* Added by hqfeng, 2006.08.09*/
static int dealACSFaultCode(int acsFaultCode, cwmp_rpc_t **rpcPtr)
{
	cwmp_rpc_t  *pRpc;

	pRpc = *rpcPtr;
	if( (acsFaultCode != 0) && (acsFaultCode != ACS_RetryRequest)){
        	cwmpReleaseSoapStruct(pRpc->responseStructPtr);
		pRpc->responseStructPtr = NULL;
		if(!cwmpGetInformCompleteFlag()){
			cwmpResetRpcResponseFlag(pRpc);
			cwmpSetInformCompleteFlag(1);
		}
		if(pRpc->txBuf != NULL){
#if CWMP_USING_MBUF_FOR_TXBUF
			cwmpResetMbufList();
#else		
			if(!getPreAllocTxBufFlag())
				os_free(pRpc->txBuf);//do not free it if it is pre-allocated. xzwang_060831
#endif
			pRpc->txBuf = NULL;
		}
		return 1;
	}else if(acsFaultCode == ACS_RetryRequest){
        	cwmpReleaseSoapStruct(pRpc->responseStructPtr);
		pRpc->responseStructPtr = NULL;
		setACSFaultCode(0);
		return 2;
	}else{
		if(pRpc->txBuf != NULL){
#if CWMP_USING_MBUF_FOR_TXBUF
			cwmpResetMbufList();
#else			
			if(!getPreAllocTxBufFlag())
				os_free(pRpc->txBuf);//do not free it if it is pre-allocated. xzwang_060831
#endif
			pRpc->txBuf = NULL;
		}		
	}

	return 0;
}

int cwmpDownloadContinue(SoapBodyInfoPtr  responseBodyStructPtr){
	int strlength;
	char temp[256];
	uint32 DelaySecond;
	FileTransferStructPtr dsp = NULL;
	int rtnCode = 0;
	time_t startTime, rightnow;

	if(!TCSUPPORT_CROSS_REBOOT_VAL)	
		return 0;
	
	dsp = rpc.fileTransferStruct;
	if( dsp == NULL){
		tc_printf("\r\n FileTransferStruct == NULL!");
		return -1;
	}
	
	dsp->transferType = Download;
	
	tcapi_get("Cwmp_Entry", "Cmdkey", temp);
	strlength = strlen(temp);
	if( (strlength + 1) > dsp->sCmdKey ){ 
		dsp->CmdKey = (char *)os_str_realloc(dsp->CmdKey, strlength+1, dsp->sCmdKey);		
		dsp->sCmdKey = strlength + 1;
	}
	if( dsp->CmdKey == NULL)
		goto error;
	strcpy(dsp->CmdKey, temp);
	rtnCode = SetCommandKey(dsp->CmdKey, M_Download);
	if(rtnCode != 0){
		return InvalidArgument;
	}		
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n cmdkey = %s",dsp->CmdKey);
	}

	tcapi_get("Cwmp_Entry", "DownloadFileType", temp);
	strlength = strlen(temp);	
	if( (strlength + 1) > dsp->sFileType){
		dsp->FileType = (char *)os_str_realloc(dsp->FileType, strlength+1, dsp->sFileType);
		dsp->sFileType = strlength + 1;
	}
	if(dsp->FileType== NULL)
		goto error;
	strcpy(dsp->FileType, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
	tc_printf("\r\n FileType = %s",dsp->FileType);
	}

	tcapi_get("Cwmp_Entry", "DownloadURL", temp);
	strlength = strlen(temp) ;	
	if( (strlength + 1) > dsp->sURL){
		dsp->URL = (char *)os_str_realloc(dsp->URL, strlength+1, dsp->sURL);
		dsp->sURL = strlength + 1;
	}
	if(dsp->URL  == NULL)
		goto error;
	strcpy(dsp->URL, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
	tc_printf("\r\n URL = %s",dsp->URL);
	}
	
	tcapi_get("Cwmp_Entry", "DownloadUserName", temp);
	strlength = strlen(temp);	
	if( (strlength + 1) > dsp->sUsername){
		dsp->Username= (char *)os_str_realloc(dsp->Username, strlength+1, dsp->sUsername);
		dsp->sUsername = strlength + 1;
	}
	if(dsp->Username== NULL)
		goto error;
	strcpy(dsp->Username, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n username = %s",dsp->Username);
	}

	tcapi_get("Cwmp_Entry", "DownloadPassword", temp);
	strlength = strlen(temp);	
	if( (strlength + 1) > dsp->sPassword){
		dsp->Password= (char *)os_str_realloc(dsp->Password, strlength+1, dsp->sPassword);
		dsp->sPassword = strlength + 1;
	}
	if(dsp->Password== NULL)
		goto error;
	strcpy(dsp->Password, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n passwd = %s",dsp->Password);
	}

	tcapi_get("Cwmp_Entry", "DownloadFileSize", temp);
	dsp->FileSize = (unsigned int)atoi(temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n FileSize = %ld", dsp->FileSize);	
	}

	tcapi_get("Cwmp_Entry", "DownloadTargetFileName", temp);
	strlength = strlen(temp);	
	if( (strlength + 1) > dsp->sTargetFileName){
		dsp->TargetFileName = (char *)os_str_realloc(dsp->TargetFileName, strlength+1, dsp->sTargetFileName);
		dsp->sTargetFileName = strlength + 1;
	}				
	if(dsp->TargetFileName == NULL)
		goto error;
	strcpy(dsp->TargetFileName, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n TargetFileName = %s",dsp->TargetFileName);
	}

	tcapi_get("Cwmp_Entry", "DownloadDelaySeconds", temp);
	dsp->DelaySeconds = (unsigned int)atoi(temp);
	DelaySecond = dsp->DelaySeconds;
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n DelaySeconds = %ld", dsp->DelaySeconds);
	}

	
	tcapi_get("Cwmp_Entry", "DownloadSuccessURL", temp);
	strlength = strlen(temp);	
	if( (strlength + 1) > dsp->sSuccessURL){
		dsp->SuccessURL = (char *)os_str_realloc(dsp->SuccessURL, strlength+1, dsp->sSuccessURL);
		dsp->sSuccessURL = strlength + 1;
	}				
	if(dsp->SuccessURL == NULL)
		goto error;
	strcpy(dsp->SuccessURL, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n SuccessURL = %s",dsp->SuccessURL);
	}
	
	tcapi_get("Cwmp_Entry", "DownloadFailureURL", temp);
	strlength = strlen(temp);	
	if( (strlength + 1) > dsp->sFailureURL){
		dsp->FailureURL = (char *)os_str_realloc(dsp->FailureURL, strlength+1, dsp->sFailureURL);
		dsp->sFailureURL = strlength + 1;
	}				
	if(dsp->FailureURL == NULL)
		goto error;
	strcpy(dsp->FailureURL, temp);
	if(dbg_cwmp >= DBG_CWMP_L2 ){
		tc_printf("\r\n FailureURL = %s",dsp->FailureURL);
	}

	setDownloadContinueFlag(0);	
	tcapi_get("Cwmp_Entry", "DownloadStartTime", temp);
	startTime = (unsigned int)atoi(temp);
	time(&rightnow);
    if((startTime + DelaySecond) >= rightnow && (startTime < rightnow))
	{
		DelaySecond = startTime + DelaySecond - rightnow;
		SetFileTransferTypeFlag(1);
	}
	else if(((startTime + DelaySecond) < rightnow) && ((startTime + DelaySecond + 3600) >= rightnow))
	{
		DelaySecond = 1;
		SetFileTransferTypeFlag(1);
	}
	else 
	{
		SetFileTransferCompleteFlag(TransferComplete_Error9010|EVENT_TransferComplete);
		rpc.fileTransferStruct->faultStructPtr->faultCode= DownloadFailure;
		strcpy(rpc.fileTransferStruct->faultStructPtr->faultString, "Failed to queue file transfer request to web client!");	
		set_file_transfer_flag(0);
		setDownloadStartFlag(0);
		SaveCwmpFlags();
		cwmpSignalTimerInform(1);
	
		return 0;
	}

	timerStartS( DelaySecond*1000, (TimerCallback)cwmpPerformFileTransfer, NULL );

	return 0;
error:
	tc_printf("\r\n cwmpDownloadContinue: realloc fail.");
	return -1;	
}

