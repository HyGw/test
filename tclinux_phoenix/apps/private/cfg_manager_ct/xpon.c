/*
 ***************************************************************************
 * MediaTeK Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2012, MTK.
 *
 * All rights reserved.	MediaTeK's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of MediaTeK Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of MediaTeK, Inc. is obtained.
 ***************************************************************************

	Module Name:
	xpon.c

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
	jq.zhu		2012/6/28	Create
        Andy.yi         2012/7/6        modify:add the gpon node
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "cfg_manager.h"
#include "utility.h"
#include "cc.h"
#include "xpon.h"
#include "web.h"

#ifdef TCSUPPORT_EPON_DUAL_IMAGE
#include "../../public/mtd/tc_partition.h"
#endif

#ifdef TCSUPPORT_WAN_EPON
#include "../lib/libepon.h"
#endif

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_NX)
static char loid_old[25] = {0};
#endif

#if defined(TCSUPPORT_CT_PON)
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
extern void resetOLT_TimerBySave();
#endif
#endif

#ifdef TCSUPPORT_PMMGR
#include "pmmgr_api.h"
extern void pmmgrPrintf(uint8 debugLevel, char *fmt,...);

int pmmgrPMCmdGetAttrFunc(mxml_node_t *top, char *buffer)
{
	u_short classId = 0;
	u_short portId = 0;
	u_char dataFlag = 0;
	u_char attrIndex = 0;
	u_char attrOffset = 0;
	u_char attrLength = 0;
	u_short times = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0};
	int tempData = 0;
	u_char pm[128] = {0};

	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PM);
	strcpy(nodeName[1], ENTRY);


	memset(tempBuffer, 0, sizeof(tempBuffer));
	if(getAttrValue(top, nodeName, PMMGR_CLASS_ID, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	classId = tempData;
	
	memset(tempBuffer, 0, sizeof(tempBuffer));	
	if(getAttrValue(top, nodeName, PMMGR_PORT_ID, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	portId = tempData;
	
	memset(tempBuffer, 0, sizeof(tempBuffer));
	if(getAttrValue(top, nodeName, PMMGR_DATA_FLAG, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	dataFlag = tempData;

	memset(tempBuffer, 0, sizeof(tempBuffer));
	if(getAttrValue(top, nodeName, PMMGR_ATTR_INDEX, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	attrIndex = tempData;
	
#ifdef PMMGR_DEBUG
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdGetAttrFunc: classId = %x\n", classId);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdGetAttrFunc: portId = %x\n", portId);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdGetAttrFunc: dataFlag = %x\n", dataFlag);	
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdGetAttrFunc: attrIndex = %x\n", attrIndex);	
#endif

	if (dataFlag == PMMGR_STATISTICS_TIME)
	{/* get pmgr statistics time: interval end time = (statistics times *256 /900) */
		times = pmmgrGetStaticticsTimes();
		sprintf(buffer, "%x", times);
		return 0;
	}	
	else if(dataFlag == PMMGR_CURRENT_DATA)
	{/* get pm me instance current data */
		if(pmmgrGetPmCurrentData(classId, portId, pm) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdGetAttrFunc: pmmgrGetPmCurrentData fail\n") ;
			return -1 ;
		}
	}
	else if(dataFlag == PMMGR_HISTORY_DATA)
	{/* get pm me instance history data */
		if(pmmgrGetPmHistoryData(classId, portId, pm) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdGetAttrFunc: pmmgrGetPmHistoryData fail !\n") ;
			return -1 ;
		}
	}
	else if(dataFlag == PMMGR_THRESHOLD)
	{/* get pm me instance threshold */
		if(pmmgrGetPmThreshold(classId, portId, pm) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "Get the test command failed\n\n") ;
			return -1 ;
		}
	}
	else
	{/* unkown dataFlag */
		pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdGetAttrFunc: unknown dataFlag!\n");
		return -1;
	}


	if (buffer == NULL)
		return -1;

	if ((attrOffset = pmmgrGetPmMeAttriOffsetByClassId(classId, attrIndex)) == 0xFF)
			return -1;

	if ((attrLength = pmmgrGetPmMeAttriLengthByClassId(classId, attrIndex)) == 0xFF)
			return -1;

#ifdef PMMGR_DEBUG
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrPmAttrGetHandle: attrOffset = %x\n", attrOffset);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrPmAttrGetHandle: attrLength = %x\n", attrLength);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrPmAttrGetHandle: pm = %x %x %x %x\n", pm[attrOffset], pm[attrOffset+1], pm[attrOffset+2], pm[attrOffset+3]);
#endif
	/*********************************
	 pm me  attribute data handle
	*********************************/		
	if (attrLength == 1)
		sprintf(buffer, "%x", *(pm + attrOffset));
	else if (attrLength == 2)
		sprintf(buffer, "%x", get16(pm + attrOffset));
	else if (attrLength == 4)
		sprintf(buffer, "%x", get32(pm + attrOffset));
	else if (attrLength == 8)
		//sprintf(buffer, "%llx", (u_int64_t)get64(pm + attrOffset));  :get64 will return 0, so use two get32 instead.
		sprintf(buffer, "%08x%08x", get32(pm + attrOffset),get32(pm + attrOffset+4));
	else
		return -1;
#ifdef PMMGR_DEBUG	
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrPmAttrGetHandle: buffer = %s\n", buffer);
#endif	
	return 0;
}

int pmmgrPMCmdExcuteFunc(mxml_node_t *top)
{
	u_short classId = 0;
	u_short portId = 0;
	u_char dataFlag = 0;
	u_char attrIndex = 0;
	u_char attrOffset = 0;
	u_char attrLength = 0;
	u_int attrMask = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0};
	u_char pm[128] = {0};
	int tempData = 0;
	long long temp64Data = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PM);
	strcpy(nodeName[1], ENTRY);

	memset(tempBuffer, 0, sizeof(tempBuffer));
	if(getAttrValue(top, nodeName, PMMGR_CLASS_ID, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	classId = tempData;
	
	memset(tempBuffer, 0, sizeof(tempBuffer));	
	if(getAttrValue(top, nodeName, PMMGR_PORT_ID, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	portId = tempData;
	
	memset(tempBuffer, 0, sizeof(tempBuffer));
	if(getAttrValue(top, nodeName, PMMGR_DATA_FLAG, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%x", &tempData);
	dataFlag = tempData;
	
#ifdef PMMGR_DEBUG
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: classId = %x\n", classId);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: portId = %x\n", portId);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: dataFlag = %x\n", dataFlag);
#endif
	if(dataFlag == PMMGR_CREATE)
	{/* create pm me instance to pmgr */
		if(pmmgrCreatePmMeInstance(classId, portId) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdExcuteFunc: pmmgrCreatePmMeInstance fail!\n") ;
			return FAIL ;
		}
	}	
	else if(dataFlag == PMMGR_DELETE)
	{/* delete pm me instance from pmgr */
		if(pmmgrDelPmMeInstance(classId, portId) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdExcuteFunc: pmmgrDelPmMeInstance fail!\n") ;
			return FAIL ;
		}
	}	
	else if(dataFlag == PMMGR_CURRENT_DATA)
	{/* clear pm me instance the current data to 0 */
		if(pmmgrSetPmCurrentData(classId, portId, pm) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdExcuteFunc: pmmgrSetPmCurrentData fail!\n") ;
			return FAIL ;
		}
	}
	else if(dataFlag == PMMGR_SYNCHRONIZE_TIME)
	{/* ONU-G synchronize time */
		return pmmgrSynchronizeTime();
	}
	else if(dataFlag == PMMGR_INIT)
	{/* MIB reset */
		pmmgrDestroy();
		return pmmgrInit(GPON_MODE);
	}
	else if(dataFlag == PMMGR_THRESHOLD)
	{/* set pm me instance attr threshold */

		/*********************************
		 pm me data set threshold handle
		*********************************/	
		memset(tempBuffer, 0, sizeof(tempBuffer));
		if(getAttrValue(top, nodeName, PMMGR_ATTR_INDEX, tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE,"tempBuffer = %s\n", tempBuffer);
		sscanf(tempBuffer, "%x", &tempData);
		attrIndex = tempData;

		
		if(getAttrValue(top, nodeName, PMMGR_ATTR_DATA, tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;	
		sscanf(tempBuffer, "%x", &tempData);
		sscanf(tempBuffer, "%llx", &temp64Data);
		

		if ((attrOffset = pmmgrGetPmMeAttriOffsetByClassId(classId, attrIndex)) == 0xFF)
			return FAIL;

		if ((attrLength = pmmgrGetPmMeAttriLengthByClassId(classId, attrIndex)) == 0xFF)
			return FAIL;

		#ifdef PMMGR_DEBUG
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: attrOffset = %x\n", attrOffset);
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: attrLength = %x\n", attrLength);
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: attrIndex = %x\n", attrIndex);
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: attrData = %x\n", tempData);		
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: attr64Data = %llx\n", temp64Data);	
		#endif

		if (attrLength == 1)
			*(pm + attrOffset) = (u_char)tempData;
		else if (attrLength == 2)
			put16((pm + attrOffset), (u_short)tempData);
		else if (attrLength == 4)
			put32((pm + attrOffset), tempData);
		else if (attrLength == 8)
			put64((pm + attrOffset), temp64Data);
		else
			return FAIL;
		
		attrMask = 1 << (attrIndex- 1);
		#ifdef PMMGR_DEBUG
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmmgrPMCmdExcuteFunc: pm = %x %x %x %x\n", pm[attrOffset], pm[attrOffset+1], pm[attrOffset+2], pm[attrOffset+3]);		
		#endif
			
		if(pmmgrSetPmThreshold(classId, portId, pm, attrMask) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdExcuteFunc: pmmgrSetPmThreshold fail!\n") ;
			return FAIL ;
		}
	}
	else
	{
		pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgrPMCmdExcuteFunc:unknown dataFlag\n") ;
		return FAIL;
	}
	
	return SUCCESS;
}

#ifdef TCSUPPORT_EPON_OAM_CTC
extern mxml_node_t *get_web_node(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int pmgrEPONPMCmdGetAttrFunc(mxml_node_t *top, mxml_node_t *curNode)
{
	char pm_keywd[][25]=
	{
		{"ds_DropEvents"},{"us_DropEvents"},
		{"ds_Octets"},{"us_Octets"},
		{"ds_Frames"},{"us_Frames"},
		{"ds_Broadcast"}, {"us_Broadcast"},
		{"ds_Multicast"}, {"us_Multicast"},
		{"ds_CRCError"}, {"us_CRCError"},
		{"ds_Undersize"}, {"us_Undersize"},
		{"ds_Oversize"}, {"us_Oversize"},
		{"ds_Fragments"}, {"us_Fragments"},
		{"ds_Jabbers"}, {"us_Jabbers"},
		{"ds_64Octets"}, {"us_64Octets"},
		{"ds_65to127"}, {"us_65to127"},
		{"ds_128to255"}, {"us_128to255"},
		{"ds_256to511"}, {"us_256to511"},
		{"ds_512to1023"}, {"us_512to1023"},
		{"ds_1024to1518"}, {"us_1024to1518"},
		{"ds_Discards"}, {"us_Discards"},
		{"ds_Errors"}, {"us_Errors"},
		{"statusChangeTimes"},
		{""}
	};
	u_short portId = 0;
	u_char dataFlag = 0;
	int times = 0, status = 0, i;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0};
	char *pBuf = NULL;
	int tempData = 0;
	PMEponCounters_t pm = {0};
	u_short alarmId = 0;
	u_int set=0, clear = 0;
	long long *pd = NULL;
	
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PM);
	strcpy(nodeName[1], EPON);

	if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_PORT_ID)) == NULL)
		return FAIL;
	sscanf(pBuf, "%x", &tempData);
	portId = tempData;
	
	if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_DATA_FLAG)) == NULL)
		return FAIL;
	sscanf(pBuf, "%x", &tempData);
	dataFlag = tempData;

#ifdef PMMGR_DEBUG
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrEPONPMCmdGetAttrFunc: portId = %x dataFlag = %x\n", portId, dataFlag);
#endif

	if (dataFlag == PMMGR_EPON_NONE){
		return 0;
	}
	else if (dataFlag == PMMGR_EPON_CYCLE_TIME)
	{/* get epon pm cycle time */
		times = getEponPMCycleTime(portId);
		if (times == FAIL)
			return FAIL;
		sprintf(tempBuffer, "%u", times);
		mxmlElementSetAttr(curNode, PMMGR_ATTR_CYCLE, tempBuffer);
		return 0;
	}	
	else if(dataFlag == PMMGR_EPON_ENABLE)
	{/* get epon pm enable status */
		status = getEponPMEnable(portId);
		if(status == FAIL) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdGetAttrFunc: getEponPMEnable fail\n") ;
			return FAIL ;
		}
		else if (status == 1)
			strcpy(tempBuffer, "Yes");
		else /* if (status == 0) */
			strcpy(tempBuffer, "No");
		mxmlElementSetAttr(curNode, PMMGR_ATTR_STATUS, tempBuffer);		
	}
	else if(dataFlag == PMMGR_EPON_CURRENT_DATA)
	{/* get epon pm current data */
		if(getEponPMCurrentData(portId, &pm) == FAIL) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdGetAttrFunc: getEponPMCurrentData fail !\n") ;
			return -1 ;
		}

		pd = (long long*)(&pm);
		for (i = 0; i < sizeof(PMEponCounters_t)/sizeof(long long); i++, pd++)
		{
			sprintf(tempBuffer, "%llu", *pd);
			mxmlElementSetAttr(curNode, pm_keywd[i], tempBuffer);
		}
	}
	else if(dataFlag == PMMGR_EPON_HISTORY_DATA)
	{/* get epon pm history data */
		if(getEponPMHistoryData(portId, &pm) == FAIL) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdGetAttrFunc: getEponPMHistoryData fail !\n") ;
			return -1 ;
		}
		
		pd = (long long*)(&pm);
		for (i = 0; i < sizeof(PMEponCounters_t)/sizeof(long long); i++, pd++)
		{
			sprintf(tempBuffer, "%llu", *pd);
			mxmlElementSetAttr(curNode, pm_keywd[i], tempBuffer);
		}
	}
	else if(dataFlag == PMMGR_EPON_THRESHOLD)
	{/* get epon pm alarm's threshold */
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ALARM_ID)) == NULL)
			return FAIL;	
		sscanf(pBuf, "%x", &tempData);
		alarmId = tempData;
		
		if(getEponPortAlarmThreshold(portId, alarmId, &set, &clear) == FAIL) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdGetAttrFunc: getEponPortAlarmThreshold fail !\n") ;
			return FAIL ;
		}
		sprintf(tempBuffer, "%u", set);
		mxmlElementSetAttr(curNode, PMMGR_ATTR_SET, tempBuffer);	
		sprintf(tempBuffer, "%u", clear);
		mxmlElementSetAttr(curNode, PMMGR_ATTR_CLEAR, tempBuffer);	
	}
	else if(dataFlag == PMMGR_EPON_STATUS)
	{/* get epon pm alarm's status: enable/disable */
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ALARM_ID)) == NULL)
			return FAIL;	
		sscanf(pBuf, "%x", &tempData);
		alarmId = tempData;

		status = getEponPortAlarmStatus(portId, alarmId);
		if( status == FAIL) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdGetAttrFunc: getEponPortAlarmThreshold fail !\n") ;
			return FAIL ;
		}
		else if (status == 1)
			strcpy(tempBuffer, "Yes");
		else /*if (status == 0) */
			strcpy(tempBuffer, "No");
		mxmlElementSetAttr(curNode, PMMGR_ATTR_STATUS, tempBuffer);
	}
	else
	{/* unkown dataFlag */
		pmmgrPrintf(PMMGR_DEBUG_WARN, "pmgrEPONPMCmdGetAttrFunc: unknown dataFlag!\n");
		return FAIL;
	}

	/* to reset the dataFlag = PMMGR_EPON_NONE to skip read from system twice or more. */
	sprintf(tempBuffer, "%d", PMMGR_EPON_NONE);
	mxmlElementSetAttr(curNode, PMMGR_DATA_FLAG, tempBuffer);
	
	return 0;
}


int pmgrEPONPMCmdExcuteFunc(mxml_node_t *top)
{
	u_short portId = 0;
	u_char dataFlag = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0};
	char *pBuf = NULL;
	int tempData = 0;

	u_short cycleTime = 0;
	u_char status = 0;
	PMEponCounters_t pm = {0};
	u_short alarmId = 0;
	u_int set = 0, clear = 0;
	
	mxml_node_t *curNode = NULL;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], PM);
	strcpy(nodeName[1], EPON);

	curNode = get_web_node(top, nodeName);
	if (curNode == NULL){
		pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdExcuteFunc: Error no node: PM_EPON!\n");
		return FAIL;
	}
	
	if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_PORT_ID)) == NULL)
		return FAIL;	
	sscanf(pBuf, "%u", &tempData);
	portId = tempData;
	
	if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_DATA_FLAG)) == NULL)
		return FAIL;	
	sscanf(pBuf, "%u", &tempData);
	dataFlag = tempData;
	
#ifdef PMMGR_DEBUG
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrEPONPMCmdExcuteFunc: portId = %x\n", portId);
	pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrEPONPMCmdExcuteFunc: dataFlag = %x\n", dataFlag);	
#endif
	if (dataFlag == PMMGR_EPON_NONE){
		return 0;
	}
	else if(dataFlag == PMMGR_EPON_CYCLE_TIME)
	{/* set epon pm cycle time */
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ATTR_CYCLE)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "cycleTime = %s\n", pBuf);
		sscanf(pBuf, "%u", &tempData);
		cycleTime = tempData;
		
		if(setEponPMCycleTime(portId, cycleTime) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdExcuteFunc: setEponPMCycleTime fail!\n") ;
			return FAIL ;
		}
	}	
	else if(dataFlag == PMMGR_EPON_ENABLE)
	{/* set epon pm enable/disable */
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ATTR_STATUS)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE,"status = %s\n", pBuf);
		#if 0
		sscanf(pBuf, "%u", &tempData);
		status = tempData;
		#else
		if (strncmp(pBuf, "Yes", 3) == 0){
			status = ENABLE;
		}else
			status = DISABLE;
		#endif
		
		if(setEponPMEnable(portId, status) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdExcuteFunc: setEponPMEnable fail!\n") ;
			return FAIL ;
		}
	}	
	else if(dataFlag == PMMGR_EPON_CURRENT_DATA)
	{/* clear epon pm the current data to 0 */
		if(setEponPMCurrentData(portId, &pm) != 0) 
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdExcuteFunc: setEponPMCurrentData fail!\n") ;
			return FAIL ;
		}
	}
	else if(dataFlag == PMMGR_EPON_THRESHOLD)
	{/* set epon alarm's threshold */
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ALARM_ID)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "alarmId = %s\n", pBuf);
		sscanf(pBuf, "%u", &tempData);
		alarmId = tempData;

		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ATTR_SET)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "set = %s\n", pBuf);
		sscanf(pBuf, "%u", &set);
		
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ATTR_CLEAR)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "clear = %s\n", pBuf);
		sscanf(pBuf, "%u", &clear);

		if ((setEponPortAlarmThreshold(portId, alarmId, set, clear)) != 0)
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdExcuteFunc: setEponPortAlarmThreshold fail!\n") ;
			return FAIL ;
		}

		#ifdef PMMGR_DEBUG
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "pmgrEPONPMCmdExcuteFunc: portId=%u alarmId=%u set=%u clear=%u\n", portId, alarmId, set, clear);
		#endif
	}
	else if (dataFlag == PMMGR_EPON_STATUS)
	{	
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ALARM_ID)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "alarmId = %s\n", pBuf);
		sscanf(pBuf, "%u", &tempData);
		alarmId = tempData;
		if ((pBuf = mxmlElementGetAttr(curNode, PMMGR_ATTR_STATUS)) == NULL)
			return FAIL;	
		pmmgrPrintf(PMMGR_DEBUG_TRACE, "status = %s\n", pBuf);
#if 0
		sscanf(tempBuffer, "%u", &tempData);
		status = tempData;
#else
		if (strncmp(pBuf, "Yes", 3) == 0){
			status = ENABLE;
		}else
			status = DISABLE;
#endif
		
		if ((setEponPortAlarmStatus(portId, alarmId, status)) != 0)
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmgrEPONPMCmdExcuteFunc: setEponPortAlarmStatus fail!\n") ;
			return FAIL ;
		}	
	}
	else if(dataFlag == PMMGR_EPON_INIT)
	{/* MIB reset */
		pmmgrDestroy();
		return pmmgrInit(EPON_MODE);
	}
	else
	{
		pmmgrPrintf(PMMGR_DEBUG_WARN, "pmgrEPONPMCmdExcuteFunc:unknown dataFlag\n") ;
		return FAIL;
	}
	
	/* to reset the dataFlag = PMMGR_EPON_NONE to skip read from system twice or more. */
	sprintf(tempBuffer, "%d", PMMGR_EPON_NONE);
	/* setAttrValue(top,nodeName,PMMGR_DATA_FLAG, tempBuffer); */
	mxmlElementSetAttr(curNode, PMMGR_DATA_FLAG, tempBuffer);

	return SUCCESS;
}


char *
pm_adv_table[MAX_NODE_NAME]=
{
    EPON,
};
#endif

int
pmmgr_init(void){
	int ret=0;
	cfg_node_t node={
			.name="PM",
#ifdef TCSUPPORT_EPON_OAM_CTC
			.type=SINGLE_TYPE | UPDATE_TYPE|ADV_FLAG,
			.adv_table=pm_adv_table,
#else			
			.type=SINGLE_TYPE | UPDATE_TYPE,
			.adv_table=NULL,
#endif	
			.merge_table=NULL,
			.cfg_read=pmmgr_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=pmmgr_execute,
			.cfg_boot=pmmgr_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}
int
pmmgr_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	int i=0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;
	int ret = -2;

	char buffer[25] = {0};


/* find the attr, if not find, will new the node*/
	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, name[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			pmmgrPrintf(PMMGR_DEBUG_WARN, "pmmgr_read: can't find the node = %s, will add the node\n", name[i]);
			curNode = mxmlNewElement(parent,  name[i] );
		}
	}
	if(curNode==NULL)
	{

		return FAIL;
	}

	if (attr == NULL)
	{

		return FAIL;
	}

	if (strcmp(name[1], ENTRY) == 0){
		ret = pmmgrPMCmdGetAttrFunc(top, buffer);

		if (ret == 0)
		{
			pmmgrPrintf(PMMGR_DEBUG_WARN, "gpon_read: the attr = %s, read the data from pmmgr success\n", attr);
			mxmlElementSetAttr(curNode, attr, buffer);
		}
		else
		{
			pmmgrPrintf(PMMGR_DEBUG_ERROR, "pmmgr_read: the attr = %s, read the data fail \n", attr);
			return FAIL;
		}
	}
#ifdef TCSUPPORT_EPON_OAM_CTC	
	else if (strcmp(name[1], EPON) == 0){
		//pmmgrPrintf(PMMGR_DEBUG_TRACE, "CTC: Get EPON PM Node: %s *-_-*\n", name[1]);
		ret = pmgrEPONPMCmdGetAttrFunc( top,curNode);
		if (ret == FAIL)
			return FAIL;
	}
#endif

	return SUCCESS;
}

int
pmmgr_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	if (strcmp(name[1], ENTRY)== 0){
		return pmmgrPMCmdExcuteFunc(top);
	}
#ifdef TCSUPPORT_EPON_OAM_CTC	
	else if (strcmp(name[1], EPON) == 0){
		return pmgrEPONPMCmdExcuteFunc(top);
	}else{
		return (pmmgrPMCmdExcuteFunc(top) &&
				pmgrEPONPMCmdExcuteFunc(top));
		
	}
#endif
	return 0;
}

int
pmmgr_boot(mxml_node_t *top)
{
	return pmmgrInit(GPON_MODE);
}
#endif

#ifdef TCSUPPORT_WAN_GPON
char *
gpon_adv_table[MAX_NODE_NAME]=
{
    COMMON,
    ONU,
    OLT,
    ANI,
    TCONT,
    GEMPORT,
    TRTCM,
    MAINIMAGE,
    SLAVEIMAGE,
    CAPABILITY,
    LOID,
};

int
gpon_init(void){
	int ret=0;
	cfg_node_t node={
			.name="GPON",
			.type=UPDATE_TYPE |ADV_FLAG,
			.adv_table=gpon_adv_table,
			.merge_table=NULL,
			.cfg_read=gpon_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=gpon_execute,
			.cfg_boot=gpon_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int gpon_boot(mxml_node_t *top){
#if 0//def TCSUPPORT_PRODUCTIONLINE
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	proline_Para para;
	int notUsed = 0;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],GPON);
	strcpy(nodeName[1],ONU);
	tcgetProLinePara(&para,0);
	if(PRODUCTLINEPARAMAGIC == para.magic){
		notUsed = 1;
	}

	para.gponSn[GPONSNLEN-1] = '\0';
	para.gponPasswd[GPONPASSWDLEN-1] = '\0';

	if (notUsed && (para.flag & PL_GPON_SN_FLAG)){
		if (setAttrValue(top, nodeName,"SerialNumber", para.gponSn) != TCAPI_PROCESS_OK){
			tcdbg_printf("Error: gpon_boot set SerialNumber fail!\n");
			return FAIL;
		}
	}
	if (notUsed && (para.flag & PL_GPON_PASSWD_FLAG)){
		if (setAttrValue(top, nodeName,"Password", para.gponPasswd) != TCAPI_PROCESS_OK){
			tcdbg_printf("Error: gpon_boot set Password fail!\n");
			return FAIL;
		}
	}
#endif
#endif	
	return 0;
}

#ifdef TCSUPPORT_PONMGR
#if 0
int gponOnuCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	int i ;
	MACRO_CMD_STRUCT(GponSystemInfo) gponInfo ;

	if (strcmp(attr, "SerialNumber") == 0)
	{
	if(PonApi_GponSystemInfo_Get(&gponInfo) != EXEC_OK) {
		printf("Exec. Failed: Get the 'GponSystemInfo' command failed\n") ;
		return FAIL;
	}
		for(i=0 ; i<4 ; i++) 
		{
			sprintf(buffer+i, "%c", gponInfo.Serial[i]);
		}
		for(i=4 ; i<8 ; i++) 
		{
			sprintf(buffer+(2*i -4), "%.2x", gponInfo.Serial[i]);
		}
	}
	else
		return -2;

	return 0;
}
#endif

int gponOnuCmdSetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
#if 0
	MACRO_CMD_STRUCT(PonTestCmd) setEntry, getEntry ;
	MGR_Mask mask = 0 ;

	
	if(strcmp(attr, "SecurityMode") == 0)
	{
		setEntry.value1 = *buffer;
		setEntry.value2 = 20 ;
		setEntry.value3 = 30 ;
		mask = (MASK_SetValue1|MASK_SetValue2|MASK_SetValue3) ;
		if(PonApi_PonTestCmd_Set(mask, &setEntry) != EXEC_OK) {
			tcdbg_printf("Set the test command failed\n\n") ;
			return -1 ;
		}
	}
	else
		return -2;

	/*********************************
	 example for get command
	*********************************/	
	if(PonApi_PonTestCmd_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("Get the test command failed\n\n") ;
		return -1 ;
	}
	
	tcdbg_printf("===>Get Value 1: %d\n", getEntry.value1) ;
	tcdbg_printf("===>Get Value 2: %d\n", getEntry.value2) ;
	tcdbg_printf("===>Get Value 3: %d\n", getEntry.value3) ;

	/****************************************
	 example for get value from database
	****************************************/
	tcdbg_printf("===>Get Value 1: %d\n", gpSysDbAddr->pon.ponTestCmd.value1) ;
	tcdbg_printf("===>Get Value 2: %d\n", gpSysDbAddr->pon.ponTestCmd.value2) ;
	tcdbg_printf("===>Get Value 3: %d\n", gpSysDbAddr->pon.ponTestCmd.value3) ;
#endif
	return 0;


}

int gponAniCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	MACRO_CMD_STRUCT(PhyTransParameters) phyTransParams;

	if(PonApi_PhyTransParameters_Get(&phyTransParams) != EXEC_OK) {
		tcdbg_printf("Get the phyTransParams command failed\n\n") ;
		return -1 ;
	}

	if (strcmp(attr, "PhyTransParameters") == 0)
	{/*ani test*/
		sprintf(buffer, "Temperature:%x,Voltage:%x,TxCurrent:%x,TxPower:%x,RxPower:%x",
			phyTransParams.Temperature, phyTransParams.Voltage, phyTransParams.TxCurrent, phyTransParams.TxPower, phyTransParams.RxPower);
	}
	else
		return -2;
	
	return 0;
}


int gponAniCmdSetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{	
#ifdef TCSUPPORT_PMMGR
		gponAlarmConfig_t alarmConfig;
		memset (&alarmConfig, 0, sizeof(alarmConfig));
#endif

	if(strcmp(attr, "GEMBlockLength") == 0)
	{
		MACRO_CMD_STRUCT(GponDbaConfig) gponDbaConfig ;
		unsigned int value ;
			
		value = strtoul(buffer, NULL, 10) ;
		if(value > 65535) {
			tcdbg_printf("Input Error: block size error.\n") ;
			return -1 ;
		}
		gponDbaConfig.BlockSize = value ;
			
		if(PonApi_GponDbaConfig_Set((MASK_GponBlockSize), &gponDbaConfig) != EXEC_OK) {
			tcdbg_printf("Exec. Failed: Set the 'GponDbaConfig' command failed\n\n") ;
			return -1 ;
		}
	}
#ifdef TCSUPPORT_PMMGR
	else if(strcmp(attr, "SFThd") == 0)
	{
		alarmConfig.sfThreshold= strtoul(buffer, NULL, 10) ;
		pmmgrSetAlarmConfig(MASK_sfThreshold, alarmConfig);
	}
	else if(strcmp(attr, "SDThd") == 0)
	{
		alarmConfig.sdThreshold= strtoul(buffer, NULL, 10) ;
		pmmgrSetAlarmConfig(MASK_sdThreshold, alarmConfig); 	
	}
	else if(strcmp(attr, "LowerRxOpticalThd") == 0)
	{
		alarmConfig.lowerOpticalThreshold= strtoul(buffer, NULL, 10) ;
		pmmgrSetAlarmConfig(MASK_lowerOpticalThreshold, alarmConfig);	
	}
	else if(strcmp(attr, "UpperRxOpticalThd") == 0)
	{
		alarmConfig.upperOpticalThreshold= strtoul(buffer, NULL, 10) ;
		pmmgrSetAlarmConfig(MASK_upperOpticalThreshold, alarmConfig);	
	}
	else if(strcmp(attr, "LowerTxOpticalThd") == 0)
	{
		alarmConfig.lowerTxPowerThreshold= strtoul(buffer, NULL, 10) ;
		pmmgrSetAlarmConfig(MASK_lowerTxPowerThreshold, alarmConfig);		
	}
	else if(strcmp(attr, "UpperTxOpticalThd") == 0)
	{
		alarmConfig.upperTxPowerThreshold= strtoul(buffer, NULL, 10) ;
		pmmgrSetAlarmConfig(MASK_upperTxPowerThreshold, alarmConfig);		
	}
#endif
	
	
	return 0;
}

#if 0
int PonTCONTCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	char attriName[MAX_NODE_NAME] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	MACRO_CMD_STRUCT(PonTestCmd) getEntry;
	

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], GPON);
	strcpy(nodeName[1], TCONT);

	if(getAttrValue(top, nodeName, "Number", attriName) != TCAPI_PROCESS_OK)
		return FAIL;	
	

	/*********************************
	 example for get command
	*********************************/	
	if(PonApi_PonTestCmd_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("Get the test command failed\n\n") ;
		return -1 ;
	}

	if (strcmp(attr, "AllocID") == 0)
	{
		put32(buffer, getEntry.value1);
		put32(buffer, getEntry.value2);
		put32(buffer, getEntry.value3);
		put32(buffer, 0xaabbccdd);
		put32(buffer, 0xaabbccdd);
		put32(buffer, 0xaabbccdd);
	}
	else
		return -2;


	return 0;
	
}

int PonTCONTCmdSetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	MACRO_CMD_STRUCT(PonTestCmd) setEntry, getEntry ;
	MGR_Mask mask = 0 ;

	char number[MAX_NODE_NAME] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], GPON);
	strcpy(nodeName[1], TCONT);

	if(getAttrValue(top, nodeName, "Number", number) != TCAPI_PROCESS_OK)
		return FAIL;	
	
	if(strcmp(attr, "AllocID") == 0)
	{
		setEntry.value1 = 10 ;
		setEntry.value2 = 20 ;
		setEntry.value3 = 30 ;
		setEntry.value1 = get32(buffer) ;
		setEntry.value2 = get32(buffer+4) ;
		setEntry.value3 = get32(buffer+8) ;
		mask = (MASK_SetValue1|MASK_SetValue2|MASK_SetValue3) ;
		if(PonApi_PonTestCmd_Set(mask, &setEntry) != EXEC_OK) {
			tcdbg_printf("Set the test command failed\n\n") ;
			return -1 ;
		}	
	}
	else if(strcmp(attr, "Policy") == 0)
	{
		setEntry.value1 = *number ;
		setEntry.value2 = 20 ;
		setEntry.value3 = 30 ;
		setEntry.value1 = *number ;
		setEntry.value2 = get32(buffer) ;
		setEntry.value3 = get32(buffer+8) ;
	}

	/*********************************
	 example for get command
	*********************************/	
	if(PonApi_PonTestCmd_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("Get the test command failed\n\n") ;
		return -1 ;
	}
	
	tcdbg_printf("===>Get Value 1: %d\n", getEntry.value1) ;
	tcdbg_printf("===>Get Value 2: %d\n", getEntry.value2) ;
	tcdbg_printf("===>Get Value 3: %d\n", getEntry.value3) ;

	/****************************************
	 example for get value from database
	****************************************/
	tcdbg_printf("===>Get Value 1: %d\n", gpSysDbAddr->pon.ponTestCmd.value1) ;
	tcdbg_printf("===>Get Value 2: %d\n", gpSysDbAddr->pon.ponTestCmd.value2) ;
	tcdbg_printf("===>Get Value 3: %d\n", gpSysDbAddr->pon.ponTestCmd.value3) ;
	return 0;
}
#endif

int gponGemPortCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	MACRO_CMD_STRUCT(GponGemPortConfig) gponGemInfo ;
	int gemportId = 0;
	char tempBuffer[MAXSIZE] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i ;
	int totalCnt = 0;

	if (strcmp(attr, "EncryptionState") == 0)
	{
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], GPON);
		strcpy(nodeName[1], GEMPORT);

		if(getAttrValue(top, nodeName, "PortId", tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	
		gemportId = atoi(tempBuffer);
		memset(&gponGemInfo, 0, sizeof(MACRO_CMD_STRUCT(GponGemPortConfig))) ;
		if(PonApi_GponGemPortConfig_Get(&gponGemInfo) != EXEC_OK) 
		{
			tcdbg_printf("Exec. Failed: Get the 'GponGemPortConfig' command failed\n") ;
			return FAIL;
		}
		
		tcdbg_printf("GPON GEM Port information, Entries:%d:\n", gponGemInfo.EntryNum) ;
		for(i=0 ; i<gponGemInfo.EntryNum ; i++) 
		{
			if (gponGemInfo.GemPort[i].GemPortId == gemportId)
			{
				sprintf(buffer, "%d", gponGemInfo.GemPort[i].Encryption);
			}
		}
	}
	else if ( 0 == strcmp(attr, "EncryptionStateALL") )
	{
		memset(&gponGemInfo, 0, sizeof(MACRO_CMD_STRUCT(GponGemPortConfig)));
		if( EXEC_OK != PonApi_GponGemPortConfig_Get(&gponGemInfo) ) 
		{
			tcdbg_printf("Exec. Failed: Get the 'GponGemPortConfig' command failed\n") ;
			return FAIL;
		}

		memset(buffer, 0, MAXLEN_TCAPI_MSG);
		for(i=0 ; i < gponGemInfo.EntryNum ; i++) 
		{
			if ( ENUM_GPON_GEM_TYPE_UNICAST != gponGemInfo.GemPort[i].GemType )
				continue;

			memset(tempBuffer, 0, sizeof(tempBuffer));
			totalCnt += snprintf(tempBuffer, sizeof(tempBuffer)-1, "%d:%d#",
				gponGemInfo.GemPort[i].GemPortId, gponGemInfo.GemPort[i].Encryption);
			if ( totalCnt >= MAXLEN_TCAPI_MSG )
				break;
			strcat(buffer, tempBuffer);
		}
	}
	else
		return -2;

	return 0;
}
int gponGemPortCmdSetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	return 0;
}
int gponOltCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	MACRO_CMD_STRUCT(GponTodConfig) gponTodCfg ;

	if (strcmp(attr, "ToD") == 0)
	{
		if(PonApi_GponTodConfig_Get(&gponTodCfg) != EXEC_OK) 
		{
			tcdbg_printf("Exec. Failed: Get the 'GponTodConfig' command failed\n") ;
			return FAIL;
		}

		sprintf(buffer, "%d,%d", gponTodCfg.Second, gponTodCfg.Nanosecond);
	}
	else
		return -2;
	
	return 0;
}
int gponOltCmdSetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
	MACRO_CMD_STRUCT(GponTodConfig) gponTodCfg ;

	if(strcmp(attr, "ToD") == 0)
	{
		sscanf(buffer, "%d,%d,%d", &gponTodCfg.SFCounter, &gponTodCfg.Second, &gponTodCfg.Nanosecond);
		if(PonApi_GponTodConfig_Set(0, &gponTodCfg) != EXEC_OK) 
		{
			tcdbg_printf("Exec. Failed: Set the 'GponTodConfig' command failed\n\n") ;
			return FAIL;
		}	
		tcdbg_printf("Exec. Successful\n") ;
	}	
	return 0;
}

int gponTrtcmCmdSetAttrFunc(mxml_node_t *top, char *attr, char *tmpBuffer)
{
	MACRO_CMD_STRUCT(PWanTrtcmConfig) pwanTrtcmCfg ;
	MACRO_CMD_STRUCT(PWanCongestConfig) pwanCongestCfg ;
	char buffer[32] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	u_char channel = 0;
	u_int cir = 0;
	u_int pir = 0;
	u_int cbs = 0;
	u_int pbs = 0;
	u_int scale = 0;


	memset(&pwanTrtcmCfg, 0, sizeof(pwanTrtcmCfg));
	if(PonApi_PWanTrtcmConfig_Get(&pwanTrtcmCfg) != EXEC_OK) {
		tcdbg_printf("Exec. Failed: Get the 'PWanTrtcmConfig' command failed\n") ;
		return FAIL;
	}
	
	scale = 1<<(pwanTrtcmCfg.TrtcmScale);

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], GPON);
	strcpy(nodeName[1], TRTCM);

	memset(buffer, 0, sizeof(buffer));
	if(getAttrValue(top, nodeName, TRTCM_CHANNEL, buffer) != TCAPI_PROCESS_OK)
		return FAIL;		

	channel = strtoul(buffer, NULL, 0) ;
	if(channel >= CONST_SHAPING_NUMBER) {
		tcdbg_printf("Input Error: TSID error.\n") ;
		return FAIL;
	}

	memset(&pwanTrtcmCfg, 0, sizeof(pwanTrtcmCfg));
	pwanTrtcmCfg.Trtcm[0].TSIdx = channel ;

	memset(buffer, 0, sizeof(buffer));
	if(getAttrValue(top, nodeName, TRTCM_CIR, buffer) != TCAPI_PROCESS_OK)
		return FAIL;		
	cir = strtoul(buffer, NULL, 0) ;
	cir = cir /8000;
	if(cir > 16000) {
		tcdbg_printf("Input Error: CIR unit error.\n") ;
		return FAIL;
	}
	pwanTrtcmCfg.Trtcm[0].CIRValue = cir ;

	memset(buffer, 0, sizeof(buffer));
	if(getAttrValue(top, nodeName, TRTCM_PIR, buffer) != TCAPI_PROCESS_OK)
		return FAIL;		

	pir = strtoul(buffer,NULL, 0) ;
	pir = pir /8000;
	if(pir>16000 || pir < pwanTrtcmCfg.Trtcm[0].CIRValue) {
		tcdbg_printf("Input Error: PIR unit error.\n") ;
		return FAIL;
        }
	
	pwanTrtcmCfg.Trtcm[0].PIRValue = pir ;

	memset(buffer, 0, sizeof(buffer));
	if(getAttrValue(top, nodeName, TRTCM_CBS, buffer) != TCAPI_PROCESS_OK)
		return FAIL;		
	
	cbs = strtoul(buffer,NULL, 0) ;

	cbs = cbs /scale;
	if(cbs > 65535) {
		tcdbg_printf("Input Error: CBS value error.\n") ;
		return FAIL;
	}
	pwanTrtcmCfg.Trtcm[0].CBSUnit = cbs ;

	memset(buffer, 0, sizeof(buffer));
	if(getAttrValue(top, nodeName, TRTCM_PBS, buffer) != TCAPI_PROCESS_OK)
		return FAIL;		
	
	pbs = strtoul(buffer, NULL, 0) ;
	pbs = pbs /scale;
	if(pbs  > 65535) {
		tcdbg_printf("Input Error: PBS value error.\n") ;
		return FAIL;
	}
	pwanTrtcmCfg.Trtcm[0].PBSUnit = pbs ;
	
	if(PonApi_PWanTrtcmConfig_Set((MASK_TrtcmParameters), &pwanTrtcmCfg) != EXEC_OK) {
		tcdbg_printf("Exec. Failed: Set the 'PWanTrtcmConfig' command failed\n\n") ;
		return FAIL;
	}	

	pwanCongestCfg.TrtcmMode = ENUM_ENABLE ;
	if(PonApi_PWanCongestConfig_Set((MASK_CongestTrtcmMode), &pwanCongestCfg) != EXEC_OK) {
		tcdbg_printf("Exec. Failed: Set the 'PWanCongestConfig' command failed\n\n") ;
		return FAIL;
	}

	return SUCCESS;
}
#endif

int
gpon_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	int i=0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;
	int ret = -2;

	char buffer[MAXLEN_TCAPI_MSG] = {0};

/* find the attr, if not find, will new the node*/
	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, name[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			tcdbg_printf("gpon_read: can't find the node = %s, will add the node\n", name[i]);
			curNode = mxmlNewElement(parent,  name[i] );;
		}
	}
	if(curNode==NULL){

		return FAIL;
	}

#ifdef TCSUPPORT_PONMGR
/* ponmng get api*/
#ifdef OMCI_DEBUG
	tcdbg_printf("gpon_read: attr = %s\n", attr);
	tcdbg_printf("gpon_read: name[0] = %s\n", name[0]);
	tcdbg_printf("gpon_read: name[1] = %s\n", name[1]);	
#endif

	if (attr != NULL)
	{
#if 0
		if(strcmp(name[1], ONU) == 0)
		{
			ret = gponOnuCmdGetAttrFunc(top, attr, buffer);
		}
		else 
#endif
		if(strcmp(name[1], ANI) == 0)
		{
			ret = gponAniCmdGetAttrFunc(top, attr, buffer);
		}
#if 0
		else if(strcmp(name[1], TCONT) == 0)
		{
			ret = PonTCONTCmdGetAttrFunc(top, attr, buffer);
		}	
#endif
		else if(strcmp(name[1], GEMPORT) == 0)
		{
			ret = gponGemPortCmdGetAttrFunc(top, attr, buffer);
		}
		else if(strcmp(name[1], OLT) == 0)
		{
			ret = gponOltCmdGetAttrFunc(top, attr, buffer);
		}		
		else		
		{
		#ifdef OMCI_DEBUG
			tcdbg_printf("gpon_read: not from OMCI\n");
		#endif
		}
	}
#endif

	if (ret == 0)
	{
	#ifdef OMCI_DEBUG
		tcdbg_printf("gpon_read: the attr = %s, read the data from ponmng success\n", attr);
	#endif
		mxmlElementSetAttr(curNode, attr, buffer);
	}
	else if (ret == -1)
	{
	#ifdef OMCI_DEBUG
		tcdbg_printf("gpon_read: the attr = %s, read the data from ponmng fail\n", attr);
	#endif
		return -1;
	}
	else
	{
//		tcdbg_printf("gpon_read: the attr = %s, read the data from romfile\n", attr);
		;
	}

	return 0;
}
int setXponStart(int mode)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;
	
	if(PonApi_SysLinkConfig_Get(&sysLinkCfg) != EXEC_OK) {
		printf("Exec. Failed: Get the 'SysLinkConfig' command failed\n") ;
		return FAIL;
	}

	if (sysLinkCfg.LinkStatus == ENUM_SYSTEM_LINK_GPON)
	{
		/*start new xPON mode*/
		sysLinkCfg.LinkStart = XPON_NODE_ENABLE;
		if(PonApi_SysLinkConfig_Set((MASK_SysLinkStart), &sysLinkCfg) != EXEC_OK) {
			tcdbg_printf("setXponMode->Exec. Failed: Set the 'SysLinkConfig' command START failed\n") ;
			return -1;
		}
	}

#endif
	return 0;
}

char lastPWD[32] = {0};
int
gpon_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char attriName[128] = {0};
	char buffer[32] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	omciBaselinePayload_t cmdMsg;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], GPON);
	strcpy(nodeName[1], COMMON);

#ifdef TCSUPPORT_PONMGR
#ifdef OMCI_DEBUG
	tcdbg_printf("gpon_execute: attriName = %s\n", attriName);
	tcdbg_printf("gpon_execute: name[0] = %s\n", name[0]);
	tcdbg_printf("gpon_execute: name[1] = %s\n", name[1]);	
#endif
	if ((strlen(name[1]) != 0))
	{
		 if(strcmp(name[1], TRTCM) == 0)
		{
			gponTrtcmCmdSetAttrFunc(top, NULL, NULL);
			return SUCCESS;
		}
		 else if(strcmp(name[1], LOID) == 0)
		 {
#if defined(TCSUPPORT_CT_PON)
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
			resetOLT_TimerBySave();
#endif
#endif
		 	//mib reset
			cmdMsg.devId = 0x0a;
			sendOmciCmdMsg(OMCI_IPC_CMD_TYPE_MIB_RESET,  (omciBaselinePayload_t*)&cmdMsg);
			//gpon restart
		 	setXponStart(XMCS_IF_WAN_DETECT_MODE_GPON);
			return SUCCESS;
		 }
#if defined(TCSUPPORT_CMCC)
		  else if(strcmp(name[1], ONU) == 0)
		  { 	 
 
			 char snBuffer[25] = {0};
			 char passBuffer[25] = {0};
			 unsigned int value = 0;
			 
		 //  tcdbg_printf("gpon_execute: name[0] = %s\n", name[0]);
		 //  tcdbg_printf("gpon_execute: name[1] = %s\n", name[1]);  
		 
			 if ((getAttrValue(top, name, "SerialNumber", snBuffer) == TCAPI_PROCESS_OK) && (getAttrValue(top, name, "Password", passBuffer) == TCAPI_PROCESS_OK))
			 {	 
 
			 //  tcdbg_printf("gpon_execute: snBuffer = %s\n", snBuffer);
			 //  tcdbg_printf("gpon_execute: passBuffer = %s\n", passBuffer);	 
			 
				 MACRO_CMD_STRUCT(GponSnPasswd) gponSnPasswd ;
				 memset(&gponSnPasswd, 0, sizeof(MACRO_CMD_STRUCT(GponSnPasswd))) ;
				 memcpy(gponSnPasswd.Serial, snBuffer, 4) ;
				 
				 value = strtoul(snBuffer+4, NULL, 16) ;
				 gponSnPasswd.Serial[4] = (unsigned char)(value>>24) ;
				 gponSnPasswd.Serial[5] = (unsigned char)(value>>16) ;
				 gponSnPasswd.Serial[6] = (unsigned char)(value>>8) ;
				 gponSnPasswd.Serial[7] = (unsigned char)(value) ;
  			 if(0!=saveGponPasswd(passBuffer,& gponSnPasswd))
  			 {
  				 tcdbg_printf("save Gpon Passwd error\n");
  			 }			 
			 if(PonApi_GponSnPasswd_Set(0, &gponSnPasswd) != EXEC_OK) 
			 {
				 tcdbg_printf("Exec. Failed: Set the 'GponSnPasswd' command failed\n\n") ;
				 return -1;
			 }
 
			 //  tcdbg_printf("Exec. gpon restart\n\n") ;
				 
				 setXponStart(XMCS_IF_WAN_DETECT_MODE_GPON);
				 return SUCCESS;
			 }		  
		 }	 
#endif
		else
		{
			if(getAttrValue(top, nodeName, CURRENTATTRIBUTE, attriName) != TCAPI_PROCESS_OK)
				return FAIL;	
		if (strlen(attriName) != 0)
	        {
		if(getAttrValue(top, name,  attriName, buffer) != TCAPI_PROCESS_OK)
			return FAIL;
		if(strcmp(name[1], ONU) == 0)
		{
			gponOnuCmdSetAttrFunc(top, attriName, buffer);
		}
		else if(strcmp(name[1], ANI) == 0)
		{
			gponAniCmdSetAttrFunc(top, attriName, buffer);
		}
#if 0
		else if(strcmp(name[1], TCONT) == 0)
		{
			PonTCONTCmdSetAttrFunc(top, attriName, buffer);
		}
		else if(strcmp(name[1], GEMPORT) == 0)
		{
			gponGemPortCmdSetAttrFunc(top, attriName, buffer);
		}
#endif
		else if(strcmp(name[1], OLT) == 0)
		{
			gponOltCmdSetAttrFunc(top, attriName, buffer);
		}
		else
		{/*name[1] = "", tcapi_commit("GPON")*/
			#ifdef OMCI_DEBUG
			tcdbg_printf("gpon_execute: not from OMCI\n");
			#endif
			}
		}
	}
	}
#endif
#ifdef TCSUPPORT_OMCI
	avcMsgHandle(top, name);
#endif

#if !defined(TCSUPPORT_CMCC)
	SNModified(top);
#endif
	return SUCCESS;	
}

#endif

#ifdef TCSUPPORT_OMCI 
/* 
    SET : set up channel qos type to default value X 
    GET : find channel qos type and return
*/
char ponQosTypeActionByIndex(int action, int index, char *buffer)
{	
    char *tempStr_p = buffer;	
    int count = 0;	

    while(*tempStr_p != '\0')	
    {		    
        if (*tempStr_p != GPON_GEMPORT_LIST_SEP)	    
        {	        
            if (index == count)	        
            {	   
                if (PON_QOS_TYPE_GET == action)
                    return *tempStr_p;	            	            
                else
                {
                    /* Recover to default X */
                    *tempStr_p = PON_QOS_TYPE_DEFAULT_VALUE;
                    break;
                }
            }	        
            count++;	    
        }	    	    
        tempStr_p++;	
    }	    	
    return PON_QOS_TYPE_DEFAULT_VALUE;
}
int ponQosCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(PWanChannelQoS) getEntry;
	int i=0;
	char tmpBuffer[64] = {0};
    char tmpQosType[64] = {0};
    char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};	
    int qostype;
    char qos_mode[2] = {0};

    strcpy(nodeName[0],OMCI);	
    strcpy(nodeName[1],ENTRY); 	
    if(getAttrValue(top, nodeName, PON_QOS_TYPE, tmpQosType) != TCAPI_PROCESS_OK)	
    {		
        tcdbg_printf("[%s][LN%d]getAttrValue-> PON_QOS_TYPE error\n",__FUNCTION__,__LINE__) ;	
    }

	/*********************************
	 example for get command
	*********************************/	
	if(PonApi_PWanChannelQoS_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("PonApi_PWanChannelQoS_Get failed\n\n") ;
		return -1 ;
	}

	if (strcmp(attr, ATTR_QOS_WEIGHT_TYPE) == 0)
	{/* length =1 */
	
		sprintf(buffer, "%d", (u_char)getEntry.WeightType);
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponQosCmdGetAttrFunc==>getEntry.WeightType: %d\n", getEntry.WeightType) ;
#endif
		return 0;
	}
	
	if(strcmp(attr, ATTR_QOS_WEIGHT_SCALE) == 0)
	{/*length = 1*/
		sprintf(buffer, "%d", (u_char)getEntry.WeightScale);	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponQosCmdGetAttrFunc==>getEntry.WeightScale: %d\n", getEntry.WeightScale) ;
#endif
		return 0;		
	}
	
	for(i=0; i<CONST_CHANNEL_NUMBER; i++)
	{
		memset(tmpBuffer, 0, sizeof(tmpBuffer));
		sprintf(tmpBuffer, "%s%d", ATTR_QOS_VALUE, i);
		if(strcmp(attr, tmpBuffer) == 0)
		{
		    qos_mode[0] = PON_QOS_TYPE_DEFAULT_VALUE;
		    qos_mode[0] = ponQosTypeActionByIndex(PON_QOS_TYPE_GET, i, tmpQosType);		
		    if (PON_QOS_TYPE_DEFAULT_VALUE != qos_mode[0])	
		    {
    			sprintf(buffer, "%d_%s_%d_%d_%d_%d_%d_%d_%d_%d_", getEntry.Channel[i].Channel, qos_mode,
    				getEntry.Channel[i].Queue[0].Weight, getEntry.Channel[i].Queue[1].Weight, getEntry.Channel[i].Queue[2].Weight,
    				getEntry.Channel[i].Queue[3].Weight, getEntry.Channel[i].Queue[4].Weight, getEntry.Channel[i].Queue[5].Weight,
    				getEntry.Channel[i].Queue[6].Weight, getEntry.Channel[i].Queue[7].Weight);

    		    if (atoi(qos_mode) == getEntry.Channel[i].QosType)
    		    {
    		        /* Because ponmgr database have sync with OMCI, we need to recover */
    		        ponQosTypeActionByIndex(PON_QOS_TYPE_SET, i, tmpQosType);
                    if (setAttrValue(top, nodeName, PON_QOS_TYPE, tmpQosType) != TCAPI_PROCESS_OK)
						tcdbg_printf("Exec: setAttrValue-> PON_QOS_TYPE fail!\n");
				}    		            
		    }
		    else	
		    {
    			sprintf(buffer, "%d_%d_%d_%d_%d_%d_%d_%d_%d_%d_", getEntry.Channel[i].Channel, getEntry.Channel[i].QosType,
    				getEntry.Channel[i].Queue[0].Weight, getEntry.Channel[i].Queue[1].Weight, getEntry.Channel[i].Queue[2].Weight,
    				getEntry.Channel[i].Queue[3].Weight, getEntry.Channel[i].Queue[4].Weight, getEntry.Channel[i].Queue[5].Weight,
    				getEntry.Channel[i].Queue[6].Weight, getEntry.Channel[i].Queue[7].Weight);		    
			}
#if 0 //def OMCI_DEBUG								
			tcdbg_printf("ponQosCmdGetAttrFunc==>buffer = %s\n", buffer) ;
#endif
			return 0;
		}
	}
#endif	
	return -2;
}

/*_____________________________________________________________________________
**      function name: getQoSType
**      descriptions:
**            to caculate qostype by queue weight.
**
**      parameters:
**            paramList:  the qos weight list
**
**      global:
**             None
**
**      return:
**           qostype:
**				0:wrr
**              1:sp(Q7>Q6>...>Q1>Q0)
**				2:sp+wrr(Q7>Wrr:Q6...Q0)
**				3:sp+wrr(Q7>Q6>Wrr:Q5...Q0)
**				4:sp+wrr(Q7>Q6>Q5>Wrr:Q4...Q0)
**				5:sp+wrr(Q7>Q6>Q5>Q4>Wrr:Q3...Q0)
**				6:sp+wrr(Q7>Q6>Q5>Q4>Q3>Wrr:Q2...Q0)
**				7:sp+wrr(Q7>Q6>Q5>Q4>Q3>Q2>Wrr:Q1...Q0)			
**				
**				
**      call:
**
**      revision:
**      1.lisa 2015/5/19
**____________________________________________________________________________
*/

int getQoSType(u_char paramList[GPON_MAX_ATTR_NUM]){
	int i = 0;
	int spNum = 1;
	int weightNum = 0;
	
	for(i=CONST_QUEUE_NUMBER-1 ; i>0 ; i--) {
		if((weightNum == 0) && (paramList[i] == 0)){
				spNum++;
		}else{
			weightNum++;
		}
	}

	if(weightNum == CONST_QUEUE_NUMBER - 1){
		/*all weight is not 0, set as WRR*/
		return 0;
	}

	if(spNum == CONST_QUEUE_NUMBER){
		/*all weight is 0, set as SP*/
		return 1;
	}

	return spNum;
}

int ponQOSCmdSetAttrFunc(u_short mask, char *buffer)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(PWanChannelQoS) setEntry ;
	int len =0;
	char tmp_buffer[GPON_MAX_ATTR_NUM] = {0};
	char *tempStr_p = NULL;
	u_char paramNum = 0;
	u_char paramList[GPON_MAX_ATTR_NUM] = {0};
	int i = 0;
	
	tempStr_p = buffer;

	while(*tempStr_p != '\0'){		
		if(*tempStr_p == GPON_GEMPORT_LIST_SEP){	
			tmp_buffer[len] = '\0';
			paramList[paramNum] = atoi(tmp_buffer);
			paramNum++;
			memset(tmp_buffer,0,GPON_MAX_ATTR_NUM);
			len = 0;
			tempStr_p++;
			continue;			
		}
		tmp_buffer[len++] = *tempStr_p;
		tempStr_p++;
	}
	paramNum = 0;
	if(mask & MASK_QOS_WEIGHT_PARAM)
	{
		setEntry.WeightType = paramList[paramNum++] ;
		setEntry.WeightScale = paramList[paramNum++] ;
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponQOSCmdSetAttrFunc==>setEntry.WeightType: %d\n", setEntry.WeightType) ;
		tcdbg_printf("ponQOSCmdSetAttrFunc==>setEntry.WeightScale: %d\n", setEntry.WeightScale) ;
#endif
	}
	
	if(mask & MASK_QOS_CONFIG)
	{
		setEntry.Channel[0].Channel = paramList[paramNum++];
		setEntry.Channel[0].QosType = getQoSType(&paramList[++paramNum]);
		
		
		for(i=0 ; i<CONST_QUEUE_NUMBER ; i++) {
			setEntry.Channel[0].Queue[i].Weight = paramList[paramNum++] ;
#if 0 //def OMCI_DEBUG								
			tcdbg_printf("ponQOSCmdSetAttrFunc==>setEntry.Channel[0].Queue[%d].Weight: %d\n", i, setEntry.Channel[0].Queue[i].Weight) ;
#endif			
		}
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponQOSCmdSetAttrFunc==>setEntry.Channel[0].Channel: %d\n", setEntry.Channel[0].Channel) ;
		tcdbg_printf("ponQOSCmdSetAttrFunc==>setEntry.Channel[0].QosType: %d\n", setEntry.Channel[0].QosType) ;
#endif	
	}
	
	if(PonApi_PWanChannelQoS_Set(mask, &setEntry) != EXEC_OK) {
		tcdbg_printf("PonApi_PWanChannelQoS_Set failed\n\n") ;
		return -1 ;
	}	
#endif	
	return 0;
}

int ponCongestCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(PWanCongestConfig) getEntry;
	int i=0;
	char tmpBuffer[64] = {0};

	/*********************************
	 example for get command
	*********************************/	
	if(PonApi_PWanCongestConfig_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("ponCongestCmdGetAttrFunc failed\n\n") ;
		return -1 ;
	}
	
	if (strcmp(attr, ATTR_CONGEST_TRTCM_MODE) == 0)
	{/* length =1 */
	
		sprintf(buffer, "%d", (u_char)getEntry.TrtcmMode );
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.TrtcmMode: %d\n", getEntry.TrtcmMode) ;
#endif
		return 0;
	}
	
	if(strcmp(attr, ATTR_CONGEST_DEI_DROP_MODE) == 0)
	{/*length = 1*/
		sprintf(buffer, "%d", (u_char)getEntry.DeiDropMode);	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.DeiDropMode: %d\n", getEntry.DeiDropMode) ;
#endif
		return 0;		
	}

	if(strcmp(attr, ATTR_CONGEST_THRED_MODE) == 0)
	{/*length = 1*/
		sprintf(buffer, "%d", (u_char)getEntry.ThresholdMode);	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.ThresholdMode: %d\n", getEntry.ThresholdMode) ;
#endif
		return 0;		
	}	
	
	if(strcmp(attr, ATTR_CONGEST_SCALE) == 0)
	{/*length = 1*/
		sprintf(buffer, "%d_%d_", (u_char)getEntry.MaxScale, (u_char)getEntry.MinScale);	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.MaxScale: %d\n", getEntry.MaxScale) ;
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.MinScale: %d\n", getEntry.MinScale) ;
#endif
		return 0;
	}

	if(strcmp(attr, ATTR_CONGEST_DROP_P) == 0)
	{/*length = 1*/
		sprintf(buffer, "%d_%d_", (u_char)getEntry.GreenDropProbability, (u_char)getEntry.YellowDropProbability);	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.GreenDropProbability: %d\n", getEntry.GreenDropProbability) ;
		tcdbg_printf("ponCongestCmdGetAttrFunc==>getEntry.YellowDropProbability: %d\n", getEntry.YellowDropProbability) ;
#endif
		return 0;
	}
	
	for(i=0; i<CONST_QUEUE_NUMBER; i++)
	{
		memset(tmpBuffer, 0, sizeof(tmpBuffer));
		sprintf(tmpBuffer, "%s%d", ATTR_CONGEST_THRED_VALUE, i);
		if(strcmp(attr, tmpBuffer) == 0)
		{/* length =1 */
			sprintf(buffer, "%d_%d_%d_%d_%d_", getEntry.Thrshld[i].QueueIdx,
				getEntry.Thrshld[i].GreenMaxThreshold, getEntry.Thrshld[i].GreenMinThreshold,
				getEntry.Thrshld[i].YellowMaxThreshold, getEntry.Thrshld[i].YellowMinThreshold);
#if 0 //def OMCI_DEBUG								
			tcdbg_printf("ponCongestCmdGetAttrFunc==>buffer = %s\n", buffer) ;
#endif
			return 0;
		}
	}
#endif	
	return -2;
}

int ponCongestCmdSetAttrFunc(u_short mask, char *buffer)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(PWanCongestConfig) setEntry ;
	int len =0;
	char tmp_buffer[GPON_MAX_ATTR_NUM] = {0};
	char *tempStr_p = NULL;
	u_char paramNum = 0;
	u_char paramList[GPON_MAX_ATTR_NUM] = {0};
	
	tempStr_p = buffer;

	while(*tempStr_p != '\0'){		
		if(*tempStr_p == GPON_GEMPORT_LIST_SEP){	
			tmp_buffer[len] = '\0';
			paramList[paramNum] = atoi(tmp_buffer);
			paramNum++;
			memset(tmp_buffer,0,GPON_MAX_ATTR_NUM);
			len = 0;
			tempStr_p++;
			continue;			
		}
		tmp_buffer[len++] = *tempStr_p;
		tempStr_p++;
	}
	paramNum = 0;
	
	if(mask & MASK_CONGEST_TRTCM_MODE)
	{
		setEntry.TrtcmMode = paramList[paramNum++] ;
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.TrtcmMode: %d\n", setEntry.TrtcmMode) ;
#endif
	}
	if(mask & MASK_CONGEST_DEI_DROP_MODE)
	{
		setEntry.DeiDropMode = paramList[paramNum++];
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.DeiDropMode: %d\n", setEntry.DeiDropMode) ;
#endif
	}
	if(mask & MASK_CONGEST_THRED_MODE)
	{
		setEntry.ThresholdMode = paramList[paramNum++];	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.ThresholdMode: %d\n", setEntry.ThresholdMode) ;
#endif
	}
	if(mask & MASK_CONGEST_SCALE)
	{
		setEntry.MaxScale = paramList[paramNum++];
		setEntry.MinScale = paramList[paramNum++];		
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.MaxScale: %d\n", setEntry.MaxScale) ;	
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.MinScale: %d\n", setEntry.MinScale) ;	
#endif		
	}
	if(mask & MASK_CONGEST_DROP_PROBILITY)
	{
		setEntry.GreenDropProbability = paramList[paramNum++];
		setEntry.YellowDropProbability = paramList[paramNum++];	
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.GreenDropProbability: %d\n", setEntry.GreenDropProbability) ;	
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.YellowDropProbability: %d\n", setEntry.YellowDropProbability) ;		
#endif		
	}
	if(mask & MASK_CONGEST_THRED)
	{
		setEntry.Thrshld[0].QueueIdx = paramList[paramNum++] ;
		setEntry.Thrshld[0].GreenMaxThreshold = paramList[paramNum++] ;
		setEntry.Thrshld[0].GreenMinThreshold = paramList[paramNum++] ;
		setEntry.Thrshld[0].YellowMaxThreshold = paramList[paramNum++] ;
		setEntry.Thrshld[0].YellowMinThreshold = paramList[paramNum++] ;
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.Thrshld[0].QueueIdx: %d\n", setEntry.Thrshld[0].QueueIdx) ;
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.Thrshld[0].GreenMaxThreshold: %d\n", setEntry.Thrshld[0].GreenMaxThreshold) ;		
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.Thrshld[0].GreenMinThreshold: %d\n", setEntry.Thrshld[0].GreenMinThreshold) ;
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.Thrshld[0].YellowMaxThreshold: %d\n", setEntry.Thrshld[0].YellowMaxThreshold) ;		
		tcdbg_printf("ponCongestCmdSetAttrFunc==>setEntry.Thrshld[0].YellowMinThreshold: %d\n", setEntry.Thrshld[0].YellowMinThreshold) ;		
#endif
}	
	
	if(PonApi_PWanCongestConfig_Set(mask, &setEntry) != EXEC_OK) {
		tcdbg_printf("ponCongestCmdSetAttrFunc failed\n\n") ;
		return -1 ;
	}	
#endif
	return 0;
}

int ponTrtcmCfgCmdGetAttrFunc(mxml_node_t *top, char *attr, char *buffer)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(PWanTrtcmConfig) getEntry;
	int i=0;
	char tmpBuffer[64] = {0};

	/*********************************
	 example for get command
	*********************************/	
	if(PonApi_PWanTrtcmConfig_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("ponTrtcmCfgCmdGetAttrFunc failed\n\n") ;
		return -1 ;
	}
	
	if (strcmp(attr, ATTR_TRTCM_SCALE) == 0)
	{/* length =1 */
	
		sprintf(buffer, "%d", (u_char)getEntry.TrtcmScale);
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponTrtcmCfgCmdGetAttrFunc==>getEntry.TrtcmScale: %d\n", getEntry.TrtcmScale) ;
#endif
		return 0;
	}
		
	for(i=0; i<CONST_SHAPING_NUMBER; i++)
	{
		memset(tmpBuffer, 0, sizeof(tmpBuffer));
		sprintf(tmpBuffer, "%s%d", ATTR_TRTCM_VALUE, i);
		if(strcmp(attr, tmpBuffer) == 0)
		{/* length =1 */
			sprintf(buffer, "%d_%d_%d_%d_%d_", getEntry.Trtcm[i].TSIdx, getEntry.Trtcm[i].CIRValue, 
				getEntry.Trtcm[i].CBSUnit, getEntry.Trtcm[i].PIRValue, getEntry.Trtcm[i].PBSUnit);
#if 0 //def OMCI_DEBUG								
			tcdbg_printf("ponTrtcmCfgCmdGetAttrFunc==>buffer = %s\n", buffer) ;
#endif
			return 0;
		}
	}
#endif	
	return -2;
}


int ponTrtcmCfgCmdSetAttrFunc(u_short mask, char *buffer)	
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(PWanTrtcmConfig) setEntry ;
	
	int len =0;
	char tmp_buffer[GPON_MAX_ATTR_NUM] = {0};
	char *tempStr_p = NULL;
	u_char paramNum = 0;
	u_short paramList[GPON_MAX_ATTR_NUM] = {0};
	
	tempStr_p = buffer;

	while(*tempStr_p != '\0'){		
		if(*tempStr_p == GPON_GEMPORT_LIST_SEP){	
			tmp_buffer[len] = '\0';
			paramList[paramNum] = atoi(tmp_buffer);
			paramNum++;
			memset(tmp_buffer,0,GPON_MAX_ATTR_NUM);
			len = 0;
			tempStr_p++;
			continue;
		}
		tmp_buffer[len++] = *tempStr_p;
		tempStr_p++;
	}
	paramNum = 0;
	if(mask & MASK_TRTCM_SCALE)
	{
		setEntry.TrtcmScale = paramList[paramNum++] ;
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc==>setEntry.TrtcmScale: %d\n", setEntry.TrtcmScale) ;
#endif
	}

	if(mask & MASK_TRTCM_PARAMETERS)
	{
		setEntry.Trtcm[0].TSIdx = paramList[paramNum++] ;
		setEntry.Trtcm[0].CIRValue = paramList[paramNum++] ;
		setEntry.Trtcm[0].CBSUnit = paramList[paramNum++] ;
		setEntry.Trtcm[0].PIRValue = paramList[paramNum++] ;
		setEntry.Trtcm[0].PBSUnit = paramList[paramNum++] ;
#if 0 //def OMCI_DEBUG								
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc==>setEntry.Trtcm[0].TSIdx: %d\n", setEntry.Trtcm[0].TSIdx) ;
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc==>setEntry.Trtcm[0].CIRValue: %d\n", setEntry.Trtcm[0].CIRValue) ;		
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc==>setEntry.Trtcm[0].CBSUnit: %d\n", setEntry.Trtcm[0].CBSUnit) ;
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc==>setEntry.Trtcm[0].PIRValue: %d\n", setEntry.Trtcm[0].PIRValue) ;		
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc==>setEntry.Trtcm[0].PBSUnit: %d\n", setEntry.Trtcm[0].PBSUnit) ;
#endif		
	}	
	
	if(PonApi_PWanTrtcmConfig_Set(mask, &setEntry) != EXEC_OK) {
		tcdbg_printf("ponTrtcmCfgCmdSetAttrFunc failed\n\n") ;
		return -1 ;
	}	
#endif	
	return 0;
}

/*send avc msg to omci*/
int sendAvc2Omci(int avcId, int devIndex)
{
	notify_msg_t msg;
	memset(&msg, 0, sizeof(msg));

	msg.notifyType = OMCI_NOTIFY_TYPE_AVC;
	msg.avcId = avcId;
	msg.deviceId = devIndex;
	
	return sendOmciNotifyMsg(&msg);
}

int createConfigurationDocument(mxml_node_t *top)
{
	FILE *fp;
	FILE *fp1;
	int retval = 0;
	mxml_node_t *tree = NULL;
	mxml_node_t *curNode = NULL;


	/*read a default xml*/
	fp1 = fopen(DEFAULT_XML_PATH, "r");

	if(fp1 == NULL){
		return FAIL;
	}
	else{
		tree = mxmlLoadFile(NULL, fp1,  MXML_TEXT_CALLBACK);
		if (tree == NULL)
		{
			fclose(fp1);
			return -1;
		}
		fclose(fp1);
	}

	/*Create a Configuration Document xml*/
	fp = fopen(CONFIGURATION_DOCUMENT_PATH, "w");

	if(fp == NULL || top == NULL){
		return FAIL;
	}
	else{
		curNode = mxmlFindElement(top, top, "Cwmp", NULL, NULL, MXML_DESCEND);
		if (curNode != NULL)
		{
			mxmlAdd(tree, MXML_ADD_AFTER, tree, curNode);
			retval = mxmlSaveFile(tree, fp,  whitespace_cb);
		}
		fclose(fp);
	}


	return retval;
}
int generalStatusPortalHandler(mxml_node_t *top)
{
	int changeFlag = 1;/* */

	if (changeFlag == 1)
	{
		createConfigurationDocument(top);
		
		sendAvc2Omci(OMCI_AVC_ID_CONFIGURATION_DOCUMENT, 0);
	}
	return 0;
}


int avcMsgHandle(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buffer[32] = {0};
	static unsigned char oldOMCCVersion = 0;
	unsigned char  newOMCCVersion = 0;
	static int flag = 0;


	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "GPON");
	strcpy(nodeName[1], "ONU");
	if(getAttrValue(top, nodeName, "OMCCVersion", buffer) != TCAPI_PROCESS_OK)
		return FAIL;	

	newOMCCVersion = (unsigned char) strtoul(buffer, (char **)NULL, 16);
	if (flag == 0)
	{
		oldOMCCVersion = newOMCCVersion;
		flag = 1;
	}

	if (newOMCCVersion != oldOMCCVersion)
	{
		if (sendAvc2Omci(OMCI_AVC_ID_ONU2_G_OMCC_VERSION, 0) != 0)
			tcdbg_printf("avcMsgHandle: sendAvc2Omci fail!\n");
		oldOMCCVersion = newOMCCVersion;
	}

	return 0;
	
}

extern mxml_node_t **gTreePtr;

/*******************************************************************************************
**function name
	handleGponIfc
**description:
	create interface and bind or unbind interface with gemport.
 **retrun :
 	-1:	fail
 	0:	success
**parameter:
	type: create, del, modify, unassign
	ifcNum: interface num.
	validGemportNum: valid max gemport num
	gemPortlist_p: get gemport list
********************************************************************************************/
int handleGponIfc(u_char type, u_char ifcNum, u_char validGemportNum, u_short * gemPortlist_p){
	int ret = -1;
	int i = 0;
	char cmd[128] = {0};
	int realIfcIndex = 0;
#if defined(TCSUPPORT_OMCI)
	char attrName[MAX_NODE_NAME] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t * top = *gTreePtr;
	int bindSuccFlag = 0;
#endif
#ifdef TCSUPPORT_PONMGR
//	MACRO_CMD_STRUCT(GponInterfaceConfig) gponIfCfg ;	
	MACRO_CMD_STRUCT(GponGemPortConfig) gponGemPort ;	

	if(gemPortlist_p == NULL){
	#ifdef OMCI_DEBUG
		tcdbg_printf("handleGponIfc->gemPortlist_p == NULL fail \n");
	#endif
		goto end;
	}
	
	if(ifcNum<0 || ifcNum>=MAX_PON_IFC_NUM) {
	#ifdef OMCI_DEBUG
		tcdbg_printf("handleGponIfc-> Error: MAC bridge port index error.\n") ;
	#endif
		goto end;
	}

	if((type != GPON_HANDLE_PON_IFC_CREATE)
		&& (type != GPON_HANDLE_PON_IFC_DEL)
		&& (validGemportNum == 0)){			
	#ifdef OMCI_DEBUG
		tcdbg_printf("handleGponIfc-> type = %d,success[%d]\n", type, __LINE__) ;
	#endif
		ret = 0;
		goto end;		
	}
	

#if defined(TCSUPPORT_OMCI)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], XPON_NODE);
	strcpy(nodeName[1], PONCOMMON);	
#endif
	
	realIfcIndex = ifcNum+OMCI_MAC_BRIDGE_PORT_PON_PORT_BASIC;
	switch(type){
		case GPON_HANDLE_PON_IFC_CREATE:
#if 0
//			if(ifcNum >= GPON_PRE_CREATE_IFC_NUM){
				/*create interface*/
				gponIfCfg.IfIndex = ifcNum ;
				if(PonApi_GponInterfaceConfig_Create(&gponIfCfg) != 0) {
		#ifdef OMCI_DEBUG
					tcdbg_printf("handleGponIfc->Exec. Failed: Create the 'GponInterfaceConfig' command failed\n\n") ;
					tcdbg_printf("handleGponIfc->Exec. Failed: type=%d,ifcNum=%d,\n",type,ifcNum);
		#endif
					goto end;
				}
				
				sprintf(cmd,"/sbin/ifconfig  nas%d up",realIfcIndex);
				system(cmd);
			
				sprintf(cmd,"/usr/bin/brctl  addif br0 nas%d ",realIfcIndex);
				system(cmd);
//			}
#endif

			/*bind gemport to interface*/
			for(i=0; i<validGemportNum; i++){				
				gponGemPort.GemPort[0].GemPortId = gemPortlist_p[i] ;
				gponGemPort.GemPort[0].IfIndex = ifcNum ;
				gponGemPort.EntryNum = 1 ;
				if(PonApi_GponGemPortConfig_Set(MASK_AssignMacBridge, &gponGemPort) != 0) {
	#ifdef OMCI_DEBUG
					tcdbg_printf("handleGponIfc-Exec. Failed: Set assign the 'GponGemPortConfig' command failed\n\n") ;
					tcdbg_printf("handleGponIfc->Exec. Failed: type=%d, ifcNum=%d,\n",type,ifcNum);
					tcdbg_printf("handleGponIfc->Exec. Failed: type=%d, ifcNum=%d, gemPortList[%d]=%d,\n",type, ifcNum, i, gemPortlist_p[i]);
	#endif
					goto end;
				}
				
#if defined(TCSUPPORT_OMCI)	
				if(bindSuccFlag == 0){
					strcpy(attrName,PONCOMMON_ATTR_GPON_BIND_IFC);
					if (setAttrValue(top, nodeName,attrName, GPON_BIND_IFC_SUCC) != TCAPI_PROCESS_OK){
							tcdbg_printf("handleGponIfc->Exec: setAttrValue-> PONCOMMON_ATTR_GPON_BIND_IFC fail!\n");
					}else{
						bindSuccFlag = 1;
					}
					
				}
#endif
			}	
			break;
		case GPON_HANDLE_PON_IFC_DEL:
		case GPON_HANDLE_PON_IFC_UNASSIGN:
			/*unbind gemport from interface*/
			for(i=0; i<validGemportNum; i++){
				gponGemPort.GemPort[0].GemPortId = gemPortlist_p[i] ;	
				gponGemPort.EntryNum = 1 ;
				if(PonApi_GponGemPortConfig_Set(MASK_UnassignMacBridge, &gponGemPort) != 0){
	#ifdef OMCI_DEBUG
					tcdbg_printf("handleGponIfc-Exec. Failed: Set unassign the 'GponGemPortConfig' command failed\n\n");
					tcdbg_printf("handleGponIfc->Exec. Failed: type=%d,gemPortList[%d]=%d,\n",type, i, gemPortlist_p[i]);
	#endif
					goto end;
				}
			}
#if 0
			/*delete interface*/
			if(type == GPON_HANDLE_PON_IFC_DEL){
//				if(ifcNum >= GPON_PRE_CREATE_IFC_NUM){
					sprintf(cmd,"/usr/bin/brctl  delif br0 nas%d",realIfcIndex);
					system(cmd);
			
					gponIfCfg.IfIndex = ifcNum;
					if(PonApi_GponInterfaceConfig_Delete(&gponIfCfg) != 0) {
				#ifdef OMCI_DEBUG
						tcdbg_printf("handleGponIfc->Exec. Failed: Delete the 'GponInterfaceConfig' command failed\n\n") ;
						tcdbg_printf("handleGponIfc->Exec. Failed: type=%d,ifcNum=%d,\n",type, ifcNum);
				#endif
						goto end;
					}
//				}
			}
#endif
			break;
		case GPON_HANDLE_PON_IFC_MODIFY:
			for(i=0; i<validGemportNum; i++){
				gponGemPort.GemPort[0].IfIndex = ifcNum ;
				gponGemPort.GemPort[0].GemPortId = gemPortlist_p[i] ;		
				gponGemPort.EntryNum = 1 ;
				if(PonApi_GponGemPortConfig_Set(MASK_UnassignMacBridge, &gponGemPort) != 0){
					#ifdef OMCI_DEBUG
					tcdbg_printf("handleGponIfc-Exec. Failed: MASK_UnassignMacBridge the 'GponGemPortConfig' command failed\n\n") ;
					tcdbg_printf("handleGponIfc->Exec. Failed: type=%d,ifcNum=%d, gemPortList[%d]=%d,\n",type, ifcNum,i, gemPortlist_p[i]);
					#endif
				}
				if(PonApi_GponGemPortConfig_Set(MASK_AssignMacBridge, &gponGemPort) != 0) {
	#ifdef OMCI_DEBUG
					tcdbg_printf("handleGponIfc-Exec. Failed: Set the assign 'GponGemPortConfig' command failed\n\n") ;
					tcdbg_printf("handleGponIfc->Exec. Failed: type=%d,ifcNum=%d, gemPortList[%d]=%d,\n",type, ifcNum,i, gemPortlist_p[i]);
	#endif
					goto end;
				}	
#if defined(TCSUPPORT_OMCI)
				if(bindSuccFlag == 0){
					strcpy(attrName,PONCOMMON_ATTR_GPON_BIND_IFC);
					if (setAttrValue(top, nodeName, attrName, GPON_BIND_IFC_SUCC) != TCAPI_PROCESS_OK){
							tcdbg_printf("handleGponIfc->Exec: setAttrValue-> PONCOMMON_ATTR_GPON_BIND_IFC fail!\n");
					}else{
						bindSuccFlag = 1;
					}
					
				}
#endif
				
			}
			break;
		default:			
			break;
	}
#endif
	ret = 0;
end:
	return ret;
}

int
omci_init(void){
//	tcdbg_printf("%s ",__func__);
	int ret=0;
	cfg_node_t node={
			.name="OMCI",
			.type=SINGLE_TYPE | UPDATE_TYPE,// |ADV_FLAG,
			.adv_table=NULL,//omci_adv_table,
			.merge_table=NULL,
			.cfg_read=omci_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=omci_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
omci_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	int i=0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;
	int ret = -2;

	char buffer[25] = {0};

/* find the attr, if not find, will new the node*/
	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, name[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
#if 0//def OMCI_DEBUG			
			tcdbg_printf("omci_read: can't find the node = %s, will add the node\n", name[i]);
#endif
			curNode = mxmlNewElement(parent,  name[i] );;
		}
	}
	if(curNode==NULL){

		return FAIL;
	}

	if (attr == NULL)
	{

		return FAIL;
	}
	
	if ((strcmp(attr, ATTR_QOS_WEIGHT_TYPE) == 0)
		|| (strcmp(attr, ATTR_QOS_WEIGHT_SCALE) == 0)
		|| (strncmp(attr, ATTR_QOS_VALUE, strlen(ATTR_QOS_VALUE)) == 0))
	{
		ret = ponQosCmdGetAttrFunc(top, attr, buffer);
	}

	if((strcmp(attr, ATTR_CONGEST_TRTCM_MODE) == 0)
		|| (strcmp(attr, ATTR_CONGEST_DEI_DROP_MODE) == 0)
		|| (strcmp(attr, ATTR_CONGEST_THRED_MODE) == 0)
		|| (strcmp(attr, ATTR_CONGEST_SCALE) == 0)
		|| (strcmp(attr, ATTR_CONGEST_DROP_P) == 0)
		|| (strncmp(attr, ATTR_CONGEST_THRED_VALUE, strlen(ATTR_CONGEST_THRED_VALUE)) == 0))
	{
		ret = ponCongestCmdGetAttrFunc(top, attr, buffer);
	}

	if ((strcmp(attr, ATTR_TRTCM_SCALE) == 0)
		|| (strncmp(attr, ATTR_TRTCM_VALUE, strlen(ATTR_TRTCM_VALUE)) == 0))
	{
		ret = ponTrtcmCfgCmdGetAttrFunc(top, attr, buffer);
	}	
	
	if (ret == 0)
	{
		mxmlElementSetAttr(curNode, attr, buffer);
	}
	else if (ret == -1)
	{
		tcdbg_printf("omci_read: the attr = %s, read the data fail\n", attr);
	}

	return ret;
}

int
omci_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char value[MAX_NODE_NAME] = {0};
	char buffer[GPON_MAX_ATTR_NUM] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char debugLevel;
	omciBaselinePayload_t cmdMsg;
#ifdef TCSUPPORT_OMCI_LAN_DEBUG	
	char lanDebugFlag = 0;
#endif
	char attrName[GPON_MAX_ATTR_NUM] = {0};
	char attrValue[MAX_GPON_OMCI_NODE_VAL_LEN] = {0};
	char * tempStr_p = NULL;
	u_char len = 0;
	int ret = -1;
	u_short mask = MASK_DEF;
	u_char ifcHandleType = 0; 
	u_char ifcNum = 0;
	u_char validGemportNum = 0;
	u_short gemPortList[MAX_GPON_IFC_GEM_PORT_NUM] = {0};
//	u_char initIfcFlag = INIT_GPON_IFC_DISABLE;

#if 0//def OMCI_DEBUG
	tcdbg_printf("Enter omci_execute: Active = \n");
#endif
	memset(&cmdMsg , 0, sizeof(omciBaselinePayload_t));

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "OMCI");
	strcpy(nodeName[1], "Entry");
	
	/*handle create pon interface*/
	/*get handle interface type*/
	strcpy(attrName,GPON_HANDLE_PON_IFC);
	if(getAttrValue(top, nodeName, attrName, attrValue) == TCAPI_PROCESS_OK){
		ifcHandleType = atoi(attrValue);		
		if((ifcHandleType == GPON_HANDLE_PON_IFC_CREATE)
			|| (ifcHandleType == GPON_HANDLE_PON_IFC_DEL)
			|| (ifcHandleType == GPON_HANDLE_PON_IFC_MODIFY)
			|| (ifcHandleType == GPON_HANDLE_PON_IFC_UNASSIGN)){
			setAttrValue(top, nodeName, attrName, GPON_HANDLE_PON_IFC_DEFAULT);

			/*get handle interface number from romfile*/
			strcpy(attrName,GPON_IFC);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK){
				ret = FAIL;				
#ifdef OMCI_DEBUG
				tcdbg_printf("omci_execute: getAttrValue->GPON_IFC fail!\n");
#endif
				goto end;
			}
			ifcNum = atoi(attrValue);

			/*get handle pon interface gemport list from romfile*/
			strcpy(attrName,GPON_GEMPORT_LIST);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK){
		#ifdef OMCI_DEBUG
				tcdbg_printf("omci_execute: getAttrValue->gemportlist fail!\n");
		#endif
				validGemportNum = 0; 
			}else{
				memset(buffer,0,GPON_MAX_ATTR_NUM);
				len = 0;
				tempStr_p = attrValue;
				while(*tempStr_p != '\0'){		
					if(*tempStr_p == GPON_GEMPORT_LIST_SEP){	
						buffer[len] = '\0';
						gemPortList[validGemportNum] = atoi(buffer);
						validGemportNum++;
						memset(buffer,0,GPON_MAX_ATTR_NUM);
						len = 0;
						tempStr_p++;
						continue;
					}
					buffer[len++] = *tempStr_p;
					tempStr_p++;
				}
			}
			if(handleGponIfc(ifcHandleType, ifcNum, validGemportNum, gemPortList) !=  0){
				#ifdef OMCI_DEBUG
				tcdbg_printf("omci_execute: getAttrValue->handleGponIfc exec fail!\n");
				#endif
			}
			ret = SUCCESS;
			goto end;
		}

	}

	if(getAttrValue(top, nodeName, "Active", value) != TCAPI_PROCESS_OK)
		return FAIL;	
#if 0//def OMCI_DEBUG
		tcdbg_printf("omci_execute: Active = %s\n", value);
#endif
	if(strcmp(value, "Yes") != 0){
		return 0;
	}

	memset(value , 0, MAX_NODE_NAME);
	if(getAttrValue(top, nodeName, "DebugLevel", value) != TCAPI_PROCESS_OK)
		return FAIL;
#if 0//def OMCI_DEBUG
	tcdbg_printf("omci_execute: DebugLevel = %s\n", value);
#endif
	debugLevel = atoi(value);
	
	
	//cmdMsg.msgType = MT_OMCI_MSG_TYPE_DEBUG_LEVEL;
	cmdMsg.devId = 0x0a;
	cmdMsg.msgContent[0] = debugLevel;
	
	
#ifdef TCSUPPORT_OMCI_LAN_DEBUG	
	memset(value , 0, MAX_NODE_NAME);
	if(getAttrValue(top, nodeName, "LanDebug", value) != TCAPI_PROCESS_OK)
		return FAIL;
#if 0//def OMCI_DEBUG
	tcdbg_printf("omci_execute: LanDebug = %s\n", value);
#endif
	lanDebugFlag = atoi(value);
	cmdMsg.msgContent[1] = lanDebugFlag;
#endif	
	
	sendOmciCmdMsg(OMCI_IPC_CMD_TYPE_DEBUG_LEVEL,  (omciBaselinePayload_t*)&cmdMsg);
	
	memset(value , 0, MAX_NODE_NAME);
	if(getAttrValue(top, nodeName, "PMDebugLevel", value) != TCAPI_PROCESS_OK)
		return FAIL;
#if 0//def OMCI_DEBUG
	tcdbg_printf("omci_execute: PMDebugLevel = %s\n", value);
#endif	

#ifdef TCSUPPORT_PMMGR
	debugLevel = atoi(value);
	pmmgrSetDebugLevel(debugLevel);
#endif /* TCSUPPORT_PMMGR */
	strcpy(attrName,GPON_TRTCM_CFG);
	if(getAttrValue(top, nodeName, attrName, attrValue) == TCAPI_PROCESS_OK){
		ifcHandleType = atoi(attrValue);
		if(ifcHandleType == GPON_HANDLE_SET){
			setAttrValue(top, nodeName, attrName, GPON_HANDLE_DEF_STR);
			
			strcpy(attrName,PON_TRTCM_PARAMS_MASK);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK)
				return FAIL;
			mask = atoi(attrValue);

			strcpy(attrName,PON_TRTCM_PARAMS);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK)
				return FAIL;
					
			ponTrtcmCfgCmdSetAttrFunc(mask, attrValue);
		
		}
	}

	strcpy(attrName,GPON_QOS_CFG);
	if(getAttrValue(top, nodeName, attrName, attrValue) == TCAPI_PROCESS_OK){
		ifcHandleType = atoi(attrValue);
		if(ifcHandleType == GPON_HANDLE_SET){
			setAttrValue(top, nodeName, attrName, GPON_HANDLE_DEF_STR);
			
			strcpy(attrName,PON_QOS_MASK);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK)
				return FAIL;
			mask = atoi(attrValue);

			strcpy(attrName,PON_QOS);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK)
				return FAIL;
					
			ponQOSCmdSetAttrFunc(mask, attrValue);
		
		}
	}

	strcpy(attrName,GPON_CONGEST_CFG);
	if(getAttrValue(top, nodeName, attrName, attrValue) == TCAPI_PROCESS_OK){
		ifcHandleType = atoi(attrValue);
		if(ifcHandleType == GPON_HANDLE_SET){
			setAttrValue(top, nodeName, attrName, GPON_HANDLE_DEF_STR);
			
			strcpy(attrName,PON_CONGEST_MASK);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK)
				return FAIL;
			mask = atoi(attrValue);

			strcpy(attrName,PON_CONGEST);
			if(getAttrValue(top, nodeName, attrName, attrValue) != TCAPI_PROCESS_OK)
				return FAIL;
					
			ponCongestCmdSetAttrFunc(mask, attrValue);
		
		}
	}	
	ret = SUCCESS;
end:

	return 	ret;
}
#endif

#ifdef TCSUPPORT_WAN_EPON
char loid_pre[25] = {0};
char psw_pre[13] = {0};

char *
epon_adv_table[MAX_NODE_NAME]=
{
	ONU,
	PON,
	POWERSAVING,
	LOIDAUTH,
#ifdef TCSUPPORT_EPON_DUAL_IMAGE	
	SOFTIMAGE,
#endif
	SERVICESLA,	
};

int
epon_init(void){
	int ret=0;
	cfg_node_t node={
			.name="EPON",
			.type=UPDATE_TYPE|ADV_FLAG,
			.adv_table=epon_adv_table,
			.merge_table=NULL,
			.cfg_read=epon_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=epon_execute,
			.cfg_boot=epon_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int epon_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	int i=0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;
	int ret = -2;

	/* find the attr, if not find, will new the node*/
	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, name[i], NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			curNode = mxmlNewElement(parent,  name[i] );;
		}
	}
	if((curNode==NULL) || (attr==NULL)){
		return FAIL;
	}

	if (strcmp(name[1], PON) == 0){
		ret = eponPonCmdGetAttrFunc(top, curNode, attr);
		if (ret == FAIL)
			return FAIL;
	}else if (strcmp(name[1], ONU) == 0){
		if (strcmp(attr, "Type") == 0){ // get onu type
			MACRO_CMD_STRUCT(SysOnuType) onuType;
			if (EXEC_OK == PonApi_SysOnuType_Get(&onuType)){
				if (onuType.OnuType == ENUM_SYSTEM_ONU_TYPE_HGU)
					mxmlElementSetAttr(curNode, "Type", "HGU");
				else 
					mxmlElementSetAttr(curNode, "Type", "SFU"); // default is
			}else{
				tcdbg_printf("%s: Warning: PonApi_SysOnuType_Get error!\n", __FUNCTION__);
				return FAIL;
			}
		}
	}

	return ret;	
}

int epon_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
    int ret, pon_stat = 0;
	char xponModeBuf[16] = {0};	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_NX)
	char buf[30] = {0};
	cwmp_msg_t message;
	long type = 1;	//tr69 must be 1
	int flag = IPC_NOWAIT;//0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], EPON);
	strcpy(nodeName[1], LOIDAUTH);
	if (getAttrValue(top, nodeName, "LOID0", buf) != TCAPI_PROCESS_OK){
		tcdbg_printf("Error: epon_execute get loid failed!\n");
		return FAIL;
	}

	if (strcmp(loid_old, buf) != 0){
		strncpy(loid_old, buf, 24);
		loid_old[25] = '\0';

		if (loid_old[0] != '\0'){
			// send msg to cwmp
			message.cwmptype = 15;
			ret = sendmegq(type,&message,flag);
			if(ret < 0)
			{
				tcdbg_printf("\r\nsend message error!!");
			}
			else
			{
				tcdbg_printf("\r\nsend message ok!");
			}
		}
	}
#endif

#ifdef TCSUPPORT_EPON_DUAL_IMAGE	
	if (strcmp(name[1], SOFTIMAGE)== 0){
		return softImageCmdExcuteFunc(top);
	}else
#endif // TCSUPPORT_EPON_DUAL_IMAGE

	if (strcmp(name[1], SERVICESLA) == 0){
		return eponServiceSlaCmdExcFunc(top);
	}else
	if (strcmp(name[1], PON) == 0){
		return eponPonCmdExcuteFunc(top);
	}
	
	if (strcmp(name[1], LOIDAUTH) == 0){
#if defined(TCSUPPORT_CT_PON)
#if !defined(TCSUPPORT_CUC) && !defined(TCSUPPORT_CMCC)
		resetOLT_TimerBySave();
#endif
#endif

#if defined(TCSUPPORT_CT_PON)
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], XPON_NODE);
		strcpy(nodeName[1], PONCOMMON);
		getAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_MODE, xponModeBuf);
		if (!strcmp(xponModeBuf, "EPON")) {
			pon_stat = getPonState2();

			/* if it's up, set it down */
			if (pon_stat == 1) {
				setXponModeSwitch(XMCS_IF_WAN_DETECT_MODE_EPON, 0);
				epon_sys_set_link_state_cfg(0); /* remove llid */
			}

#if defined(TCSUPPORT_CT_PON_CY)
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], EPON);
			strcpy(nodeName[1], LOIDAUTH);
			if (setAttrValue(top, nodeName,"AuthStatus", "0") != TCAPI_PROCESS_OK)
			{
				tcdbg_printf("Error: epon_boot set AuthStatus 0 fail!\n");
				return FAIL;
			}
#endif

			/* set xpon mode auto */
			if (pon_stat == 0) { /* if pon state is down, restart epon */
				tcdbg_printf("\r\npon state is down, set it up");
				setXponModeSwitch(XMCS_IF_WAN_DETECT_MODE_EPON, 0);
				setXponMode(XMCS_IF_WAN_DETECT_MODE_EPON);
			}

			return SUCCESS;
		}
		else {
		return eponLoidPswCmdExcuteFunc(top);
	}
#endif
		
	}
	
	return 0;
}

int epon_boot(mxml_node_t *top){

	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[30] = {0};
#ifdef TCSUPPORT_EPON_DUAL_IMAGE	
	int active = 0, main_image = 0;

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],EPON);
	strcpy(nodeName[1],SOFTIMAGE);
	if (getAttrValue(top, nodeName, SI_ACTIVE, buf) != TCAPI_PROCESS_OK){
		tcdbg_printf("Error: epon_boot Get SoftImage Active fail!\n");
		return FAIL;
	}
	sscanf(buf, "%d", &active);
	if (active == 1){
		if (setAttrValue(top, nodeName,SI_ACTIVE, "0") != TCAPI_PROCESS_OK){
			tcdbg_printf("Error: epon_boot set SoftImage Active fail!\n");
			return FAIL;
		}
		
		if (getAttrValue(top, nodeName, SI_MAIN, buf) != TCAPI_PROCESS_OK){
			tcdbg_printf("Error: epon_boot Get SoftImage Active fail!\n");
			return FAIL;
		}
		sscanf(buf, "%d", &main_image);
#if defined(TCSUPPORT_CT_UPG_PINGPONG)
		softImageSetBootFlag(main_image); // reset the boot flag to main image.
#endif
	}
#endif // TCSUPPORT_EPON_DUAL_IMAGE
		
	{ // for LOID or password change init
		strcpy(nodeName[1], LOIDAUTH);
		if (getAttrValue(top, nodeName, "LOID0", buf) != TCAPI_PROCESS_OK){
			tcdbg_printf("Error: epon_boot get loid failed!\n");
			return FAIL;
		}
		strncpy(loid_pre, buf, 24);
		loid_pre[25] = '\0';

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_PON_CZ_NX)
		strncpy(loid_old, buf, 24);
		loid_old[25] = '\0';
#endif

		if (getAttrValue(top, nodeName, "Password0", buf) != TCAPI_PROCESS_OK){
			tcdbg_printf("Error: epon_boot get password failed!\n");
			return FAIL;
		}
		strncpy(psw_pre, buf, 12);
		psw_pre[13] = '\0';
	}

	return SUCCESS;
}

int eponLoidPswCmdExcuteFunc(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[30] = {0};
	int tempData = 0;
	int chg_flag = 0;
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;
#endif

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], EPON);
	strcpy(nodeName[1], LOIDAUTH);
	
	memset(tempBuffer, 0, sizeof(tempBuffer));	
	if(getAttrValue(top, nodeName, "LOID0", tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;
	if (strcmp(loid_pre, tempBuffer) != 0){
		chg_flag = 1;
		strncpy(loid_pre, tempBuffer, 24);
		loid_pre[25] = '\0';
	}
	
	if(getAttrValue(top, nodeName, "Password0", tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;
	if (strcmp(psw_pre, tempBuffer) != 0){
		chg_flag = 1;
		strncpy(psw_pre, tempBuffer, 12);
		psw_pre[13] = '\0';
	}

#ifdef TCSUPPORT_PONMGR
	if(PonApi_SysLinkConfig_Get(&sysLinkCfg) != EXEC_OK) {
		return FAIL;
	}

	if ((chg_flag==1) && (sysLinkCfg.LinkStatus == ENUM_SYSTEM_LINK_EPON)){ // must be epon link
		system("epon mpcp wanreset");
		tcdbg_printf("EPON changed loid or password!!!\n");
	}
#endif

#if defined(TCSUPPORT_CT_PON_CY)
	if (setAttrValue(top, nodeName,"AuthStatus", "0") != TCAPI_PROCESS_OK)
	{
		tcdbg_printf("Error: epon_boot set AuthStatus 0 fail!\n");
		return FAIL;
	}
#endif

	return SUCCESS;
	
}

int eponPonCmdGetAttrFunc(mxml_node_t *top, mxml_node_t *curNode, char *attr)
{
	// read phy api
	char buf[20] = {0};
	MACRO_CMD_STRUCT(PhyTransParameters) getEntry;
	
	if(PonApi_PhyTransParameters_Get(&getEntry) != EXEC_OK) {
		tcdbg_printf("PonApi_PhyTransParamInfo_Get failed\n\n") ;
		return -1 ;
	}
	
	sprintf(buf, "%d", getEntry.Temperature);
	mxmlElementSetAttr(curNode, "Temperature", buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%d", getEntry.Voltage);
	mxmlElementSetAttr(curNode, "SupplyVoltage", buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%d", getEntry.TxCurrent);
	mxmlElementSetAttr(curNode, "TxBiasCurrent", buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%d", getEntry.TxPower);
	mxmlElementSetAttr(curNode, "RxPower", buf);

	memset(buf, 0 , 20);
	sprintf(buf, "%d", getEntry.RxPower);
	mxmlElementSetAttr(curNode, "TxPower", buf);

	return SUCCESS;
}

int eponPonCmdExcuteFunc(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0};
	int tempData = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], EPON);
	strcpy(nodeName[1], PON);

	memset(tempBuffer, 0, sizeof(tempBuffer));	
	/*if(getAttrValue(top, nodeName, "LLIDUseMask", tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;
	sscanf(tempBuffer, "%u", &tempData);
	if (tempData != 0 ){
		eponCmdSetLlidMask(tempData);
	}*/

	{
		unsigned int state = 0, time = 50, mpcpTime;

		memset(tempBuffer, 0, sizeof(tempBuffer));	
		if(getAttrValue(top, nodeName, "Holdover", tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;
		if (0 == strcmp(tempBuffer, "Yes")){
			state = 1;
		}
		
		memset(tempBuffer, 0, sizeof(tempBuffer));	
		if(getAttrValue(top, nodeName, "HoldoverTime", tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;
		sscanf(tempBuffer, "%u", &tempData);
		time = tempData;
		
		if (FAIL == eponApiSetOnuHoldoverCfg(state, time)){
			return FAIL;
		}
		
		memset(tempBuffer, 0, sizeof(tempBuffer));	
		if(getAttrValue(top, nodeName, "TLosMAC", tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;
		sscanf(tempBuffer, "%u", &tempData);
		mpcpTime = tempData;
		
		if (FAIL == eponApiSetOnuMpcpTime(mpcpTime)){
			return FAIL;
		}
	}
	
	return SUCCESS;
}

int eponServiceSlaCmdExcFunc(mxml_node_t *top)
{
	MACRO_CMD_STRUCT(PWanTrtcmConfig) setTrTcmParamEntry ;
	MACRO_CMD_STRUCT(PWanCongestConfig) setTrTcmModeEntry ;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0}, attrName[MAX_NODE_NAME] = {0};
	int tempData = 0;
	int highPriBoudary,bestScheduleScheme, serviceNum, i;
	int priQ, cir, pir;
	unsigned short mask = 0;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], EPON);
	strcpy(nodeName[1], SERVICESLA);

	memset(tempBuffer, 0, sizeof(tempBuffer));	
	if(getAttrValue(top, nodeName, "Enable", tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%u", &tempData);
	if (tempData == 0 ){ // disable ServiceSLA
		setTrTcmModeEntry.TrtcmMode = 0;
		mask = MASK_CONGEST_TRTCM_MODE;
		
		if(PonApi_PWanCongestConfig_Set(mask, &setTrTcmModeEntry) != EXEC_OK) {
			tcdbg_printf("%s set TrTcm disable failed\n", __FUNCTION__);
			return FAIL ;
		}		
	}
	else if (tempData == 1){ // enable ServiceSLA
		if (getAttrValue(top, nodeName, "BestScheduleScheme", tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;
		sscanf(tempBuffer, "%u", &tempData);
		bestScheduleScheme = tempData;
		
		if (getAttrValue(top, nodeName, "HighPriBoundary", tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;
		sscanf(tempBuffer, "%u", &tempData);
		highPriBoudary = tempData;

		if (getAttrValue(top, nodeName, "Num", tempBuffer) != TCAPI_PROCESS_OK)
			return FAIL;
		sscanf(tempBuffer, "%u", &tempData);
		serviceNum = tempData;

		mask = MASK_TRTCM_PARAMETERS;
		for (i = 0; i < serviceNum; i++){
			sprintf(attrName, "PriQ%d", i);
			if (getAttrValue(top, nodeName, attrName, tempBuffer) != TCAPI_PROCESS_OK)
				return FAIL;
			sscanf(tempBuffer, "%u", &tempData);
			priQ = tempData;
			
			sprintf(attrName, "FixedBW%d", i);
			if (getAttrValue(top, nodeName, attrName, tempBuffer) != TCAPI_PROCESS_OK)
				return FAIL;
			sscanf(tempBuffer, "%u", &tempData);
			cir = tempData;

			sprintf(attrName, "GrantBW%d", i);
			if (getAttrValue(top, nodeName, attrName, tempBuffer) != TCAPI_PROCESS_OK)
				return FAIL;
			sscanf(tempBuffer, "%u", &tempData);
			cir += tempData;
			
			sprintf(attrName, "BestBW%d", i);
			if (getAttrValue(top, nodeName, attrName, tempBuffer) != TCAPI_PROCESS_OK)
				return FAIL;
			sscanf(tempBuffer, "%u", &tempData);
			pir = tempData+cir;

			setTrTcmParamEntry.Trtcm[0].TSIdx = priQ+1;
			setTrTcmParamEntry.Trtcm[0].CIRValue = cir/64 ; //(HW unit is 64Kbps)
			setTrTcmParamEntry.Trtcm[0].CBSUnit = 200*cir /128; // (HW unit is 128Byte def)
			setTrTcmParamEntry.Trtcm[0].PIRValue = pir/64 ;  //(HW unit is 64Kbps)
			setTrTcmParamEntry.Trtcm[0].PBSUnit =  400*pir / 128;
			
			if(PonApi_PWanTrtcmConfig_Set(mask, &setTrTcmParamEntry) != EXEC_OK) {
				tcdbg_printf("%s TrTcm params failed\n", __FUNCTION__) ;
				return FAIL ;
			}
		}


		setTrTcmModeEntry.TrtcmMode = 1;
		mask = MASK_CONGEST_TRTCM_MODE;
		
		if(PonApi_PWanCongestConfig_Set(mask, &setTrTcmModeEntry) != EXEC_OK) {
			tcdbg_printf("%s set TrTcm enable failed\n", __FUNCTION__);
			return FAIL ;
		}
	}
	return SUCCESS;
}

#ifdef TCSUPPORT_EPON_DUAL_IMAGE
//#define SOFT_IMAGE_DEBUG
int softImageCmdExcuteFunc(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempBuffer[MAX_NODE_NAME] = {0};
	int tempData = 0;

	int main_image = 0, active = 0;
	char image_real_to_boot = 0;
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], EPON);
	strcpy(nodeName[1], SOFTIMAGE);

	memset(tempBuffer, 0, sizeof(tempBuffer));	
	if(getAttrValue(top, nodeName, SI_MAIN, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%u", &tempData);
	main_image = tempData;
	
	memset(tempBuffer, 0, sizeof(tempBuffer));
	if(getAttrValue(top, nodeName, SI_ACTIVE, tempBuffer) != TCAPI_PROCESS_OK)
		return FAIL;	
	sscanf(tempBuffer, "%u", &tempData);
	active = tempData;
#ifdef SOFT_IMAGE_DEBUG
	tcdbg_printf("softImageCmdExcuteFunc: main = %d, active = %d\n", main_image, active);
#endif
	//2 need to test the valid0 & valid1 ??
	if ((main_image == 0 && active == 1) ||
	    (main_image == 1 && active == 0)){
		image_real_to_boot = 1;
	}
	// write to flash reserved
	softImageSetBootFlag(image_real_to_boot);
	if (active == 1){
		// to save the active node 
		tcapi_msg_t msg;
		memset(&msg, 0, sizeof(tcapi_msg_t));
		tcapi_save_req(top, &msg);
		if(msg.retval != TCAPI_PROCESS_OK){
			return -1;
		}
		
		system("reboot");
	}
	return 0;
}

void softImageSetBootFlag(int flag){
#ifndef TCSUPPORT_RESERVEAREA_EXTEND
#define IMG_BOOT_FLAG_SIZE 	1
#if defined(TCSUPPORT_CT)
#define IMG_BOOT_FLAG_OFFSET  	(CERM1_RA_OFFSET - IMG_BOOT_FLAG_SIZE)
#else
#define IMG_BOOT_FLAG_OFFSET  	(MRD_RA_OFFSET - IMG_BOOT_FLAG_SIZE)
#endif
#define IMG_BOOT_FLAG_FILE 	"/tmp/image_boot_flag"
#endif

	char cmds[256] = {0};
	
	sprintf(cmds, "echo %d > %s", flag, IMG_BOOT_FLAG_FILE);
#ifdef SOFT_IMAGE_DEBUG
	tcdbg_printf(cmds); // for dbg
#endif
	system(cmds);

	sprintf(cmds, TC_FLASH_WRITE_CMD, IMG_BOOT_FLAG_FILE, (unsigned long)IMG_BOOT_FLAG_SIZE, (unsigned long)IMG_BOOT_FLAG_OFFSET, RESERVEAREA_NAME);
#ifdef SOFT_IMAGE_DEBUG
	tcdbg_printf("\n>>>>: ");
	tcdbg_printf(cmds); // for dbg
#endif
	system(cmds);

}
#endif // TCSUPPORT_EPON_DUAL_IMAGE	



#ifdef TCSUPPORT_EPON_OAM

char *
oam_adv_table[MAX_NODE_NAME]=
{
	CTC,
	CUC
};


int
oam_init(void){
	int ret=0;
	cfg_node_t node={
			.name="Oam",
			.type=ADV_FLAG,
			.adv_table=oam_adv_table,
			.merge_table=NULL,
			.cfg_read=NULL,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=oam_execute,
			.cfg_boot=NULL,
	};
	ret=register_cfg_node(&node);
	return ret;
}


int
oam_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char value[MAX_NODE_NAME] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	unsigned char debugLevel;
	unsigned int cmdSeq = 0;
	
	memset(nodeName, 0, sizeof(nodeName));
#ifdef TCSUPPORT_PONMGR	
	if (strcmp(name[1], ENTRY) == 0){
		int llid, rxMode, txMode;
		MACRO_CMD_STRUCT(EponRxConfig) rxCfg;
		MACRO_CMD_STRUCT(EponTxConfig) txCfg;
		
		strcpy(nodeName[0], OAM);
		strcpy(nodeName[1], ENTRY);
		if (getAttrValue(top, nodeName, "llid", value) != TCAPI_PROCESS_OK)
			return FAIL;
		llid = atoi(value);
		if (getAttrValue(top, nodeName, "RxMode", value) != TCAPI_PROCESS_OK)
			return FAIL;
		rxMode = atoi(value);
		if (getAttrValue(top, nodeName, "TxMode", value) != TCAPI_PROCESS_OK)
			return FAIL;
		rxMode = atoi(value);

		rxCfg.LlidIdx = llid;
		rxCfg.RxMode = rxMode;
		if (EXEC_OK != PonApi_EponRxConfig_Set(MASK_ALL, &rxCfg)){			
			tcdbg_printf("%s: Warning: PonApi_EponRxConfig_Set error!\n", __FUNCTION__);
			return FAIL;
		}
		
		txCfg.LlidIdx = llid;
		txCfg.TxMode = txMode;
		if (EXEC_OK != PonApi_EponTxConfig_Set(MASK_ALL, &txCfg)){
			tcdbg_printf("%s: Warning: PonApi_EponTxConfig_Set error!\n", __FUNCTION__);
			return FAIL;
		}
	}
#endif // TCSUPPORT_PONMGR
#ifdef TCSUPPORT_EPON_OAM_CTC
	else if (strcmp(name[1], CTC) == 0){		
		strcpy(nodeName[0], OAM);
		strcpy(nodeName[1], CTC);
		if(getAttrValue(top, nodeName, "Active", value) != TCAPI_PROCESS_OK)
			return FAIL;	

		if(strcmp(value, "Yes") != 0){
			return 0;
		}

		memset(value , 0, MAX_NODE_NAME);
		if(getAttrValue(top, nodeName, "DebugLevel", value) != TCAPI_PROCESS_OK)
			return FAIL;

		debugLevel = atoi(value);

		sendEponOamCmdMsg(EPONOAM_CMD_TYPE_CTC_DBG_LVL, (void *)&debugLevel, sizeof(debugLevel), &cmdSeq);
	}
#endif // TCSUPPORT_EPON_OAM_CTC
#ifdef TCSUPPORT_EPON_OAM_CUC
	else if (strcmp(name[1], CUC) == 0){
		if (FAIL == eponOamCmdCucExec(top, name))
			return FAIL;
	}
#endif // TCSUPPORT_EPON_OAM_CUC
	return SUCCESS;	
}

#ifdef TCSUPPORT_EPON_OAM_CUC
#define ETH_PORT_MAX_NUM 4
int eponOamCmdCucExec(mxml_node_t *top, char name[][MAX_NODE_NAME]){
	char value[MAX_NODE_NAME] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	char *pBuf = NULL;
	u_int tmpData = 0, i;
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], OAM);
	strcpy(nodeName[1], CUC);

	curNode = get_web_node(top, nodeName);
	if (curNode == NULL){
		tcdbg_printf("%s: Get Oam_CUC Failed!\n", __FUNCTION__);
		return FAIL;
	}
	
	// IF CfgCnt == 0, no need to set param to system.
	if ((pBuf = mxmlElementGetAttr(curNode, "CfgCnt")) == NULL)
		return FAIL;	
	sscanf(pBuf, "%u", &tmpData);
	if (tmpData == 0)
	return SUCCESS;	

	for (i = 0; i < ETH_PORT_MAX_NUM; i++){
		sprintf(nodeName[2], "Port%d", i);
		curNode = get_web_node(top, nodeName);
		if (curNode == NULL){
			tcdbg_printf("%s: Get Oam_CUC_Port%d Failed!\n", __FUNCTION__, i);
			return FAIL;
		}
		if (FAIL == eponOamCmdCucPortExec(curNode, i))
			return FAIL;
	}
	return SUCCESS;
}

int eponOamCmdCucPortExec(mxml_node_t *curNode, int port){
	char *pBuf = NULL;
	u_int state = 0, cir, cbs, ebs, pir;

	if (port >= ETH_PORT_MAX_NUM)
		return FAIL;
	// flow control & eth pause
	if ((pBuf = mxmlElementGetAttr(curNode, "FlowControl")) == NULL) 
		return FAIL;
	sscanf(pBuf, "%u", &state);
	macMT7530SetPortPause(port, state);

	// us policing param
	if ((pBuf = mxmlElementGetAttr(curNode, "USPolicingEnable")) == NULL) 
		return FAIL;
	sscanf(pBuf, "%u", &state);
	macMT7530SetPortPolicingEnable(port, state);

	if (state != 0){
		if ((pBuf = mxmlElementGetAttr(curNode, "USPolicingCIR")) == NULL) 
			return FAIL;
		sscanf(pBuf, "%u", &cir);
		if ((pBuf = mxmlElementGetAttr(curNode, "USPolicingCBS")) == NULL) 
			return FAIL;
		sscanf(pBuf, "%u", &cbs);
		if ((pBuf = mxmlElementGetAttr(curNode, "USPolicingEBS")) == NULL) 
			return FAIL;
		sscanf(pBuf, "%u", &ebs);
		macMT7530SetPortPolicing(port, cir, cbs, ebs);
	}

	// ds ratelimiting param
	if ((pBuf = mxmlElementGetAttr(curNode, "DSRateLimitEnable")) == NULL) 
		return FAIL;
	sscanf(pBuf, "%u", &state);
	macMT7530SetPortRatelimitingEnable(port, state);

	if (state != 0){
		if ((pBuf = mxmlElementGetAttr(curNode, "DSRateLimitCIR")) == NULL) 
			return FAIL;
		sscanf(pBuf, "%u", &cir);
		if ((pBuf = mxmlElementGetAttr(curNode, "DSRateLimitPIR")) == NULL) 
			return FAIL;
		sscanf(pBuf, "%u", &pir);
		macMT7530SetPortRatelimiting(port, cir, pir);
}

	// Ether port phy admin control
	if ((pBuf = mxmlElementGetAttr(curNode, "PhyAdminControl")) == NULL) 
		return FAIL;
	sscanf(pBuf, "%u", &state);
	macMT7530SetPortActive(port, state);

	// ether port auto neg admin control
	if ((pBuf = mxmlElementGetAttr(curNode, "AutoNegAdminControl")) == NULL) 
		return FAIL;
	sscanf(pBuf, "%u", &state);
	macMT7530PortAutoNegActive(port, state);
	
	return SUCCESS;
}
#endif // TCSUPPORT_EPON_OAM_CUC

#endif // TCSUPPORT_EPON_OAM

#endif // TCSUPPORT_WAN_EPON

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
char *
xpon_adv_table[MAX_NODE_NAME]=
{
	PONCOMMON,
	PON_LINK_CFG,
#ifdef TCSUPPORT_PON_IP_HOST
	PON_IPHOST
#endif
};

int xpon_node_init(void){
	int ret=0;
	cfg_node_t node={
			.name="XPON",
			.type=UPDATE_TYPE |ADV_FLAG,
			.adv_table=xpon_adv_table,
			.merge_table=NULL,
			.cfg_read=xpon_node_read,
			.cfg_write=NULL,
			.cfg_verify=NULL,
			.cfg_execute=xpon_node_execute,
			.cfg_boot=xpon_node_boot,
	};
	ret=register_cfg_node(&node);
	return ret;
}

int
xpon_node_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr)
{
	int i=0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;
	int ret = -2;
	char buffer[40] = {0};
	unsigned long duration = 0;

	/* find the attr, if not find, will new the node*/
	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, name[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			tcdbg_printf("xpon_node_read: can't find the node = %s, will add the node\n", name[i]);
			curNode = mxmlNewElement(parent,  name[i] );;
		}
	}
	if(curNode==NULL){
		return FAIL;
	}

	/* ponmng get api*/
	if (attr != NULL){
		if(strcmp(name[1], PON_LINK_CFG) == 0){
			ret = getXponLinkSta(top, attr, buffer);
		}else if(strcmp(name[1], PONCOMMON) == 0){ 
			if(strcmp(attr,PONCOMMON_ATTR_DATA_TRAFFIC_STA) == 0){
				ret = getTrafficSta(top,attr, buffer);
			}else if(strcmp(attr,PONCOMMON_ATTR_DATA_UP_TIME) == 0){
				ret = getGponOnlineDuration(&duration);
				sprintf( buffer, "%ld", duration);
			}else if(strcmp(attr,"webtrafficStatus") == 0){
				if (getPonState2() == 1) {
					strcpy(buffer, "up");
				}
				else {
					strcpy(buffer, "down");
				}
				ret = 0;
			}else if(strcmp(attr,PONCOMMON_LOS_STATUS) == 0) {
				ret = getLosSta(buffer);
			}
		}else{
//			tcdbg_printf("*LHS DBG* xpon_read: not from ponmgr\n");
		}
	}

	if (ret == 0){
		mxmlElementSetAttr(curNode, attr, buffer);
	}else if (ret == -1){
		return -1;
	}else{
		//nothing
	}

	return 0;
}

int
xpon_node_execute(mxml_node_t *top, char name[][MAX_NODE_NAME])
{
	char attriName[MAX_NODE_NAME] = {0};
	char buffer[32] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int xPONMode = -1;
	int findThread = -1;
	
#ifdef TCSUPPORT_PON_IP_HOST
	if((strcmp(name[1],PONCOMMON)==0) && (strcmp(name[2],"xponigmp")==0))
	{
		setPonWanIf(top);
		return SUCCESS;
	}
#endif
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], XPON_NODE);
	strcpy(nodeName[1], PON_LINK_CFG);

	/*get xpon mode */
	if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, buffer) == TCAPI_PROCESS_OK){
		xPONMode = atoi(buffer);
		/*start omci thread*/
		if((xPONMode==XMCS_IF_WAN_DETECT_MODE_AUTO) || (xPONMode==XMCS_IF_WAN_DETECT_MODE_GPON)){
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer,"%s", XPON_OMCI_THREAD_CMD);
			findThread = getPsThreadInfo(buffer);
			if(findThread == 0){
				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer,"%s &", XPON_OMCI_THREAD_CMD);
				system(buffer);
			}
		}
		/*start epon oam thread*/
		if((xPONMode==XMCS_IF_WAN_DETECT_MODE_AUTO) || (xPONMode==XMCS_IF_WAN_DETECT_MODE_EPON)){
			memset(buffer, 0, sizeof(buffer));
			sprintf(buffer,"%s", XPON_OAM_THREAD_CMD); 		
			
			findThread = getPsThreadInfo(XPON_OAM_THREAD_CMD);
			if(findThread == 0){
				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer,"%s &", XPON_OAM_THREAD_CMD);
				system(buffer);
			}
		}
#if 0
		/*call xPON driver API to change xPON mode*/
		if((findThread != -1) && (xPONMode!= -1)){
			setXponMode(xPONMode);
		}
#endif
		
	}

	/*get startXpon attribute and start xPON*/
	strcpy(nodeName[1], PONCOMMON);
	if(getAttrValue(top, nodeName, PONCOMMON_ATTR_START_XPON, buffer) == TCAPI_PROCESS_OK){
		if(strcmp(buffer,START_XPON_ATTR_PROC) == 0){
			/*set startXpon attribute to default value*/
			setAttrValue(top, nodeName, PONCOMMON_ATTR_START_XPON, NO_START_XPON_ATTR_PROC);
			/*start xPON */
			xPONStart(top);
		}
	}

	return SUCCESS;	
}
int xpon_node_boot(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buf[20] = {0};
	int active = 0, main_image = 0;
	char xPONModeAttr[8] = {0};
	int xPONMode = -1;

#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
	proline_Para para;
#endif

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],XPON_NODE);
	strcpy(nodeName[1],PON_LINK_CFG);
	/*if romfile doesn't have the xpon mode attribute, give it a default value*/
	if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, buf)  != TCAPI_PROCESS_OK){		
		sprintf(buf,"%d",XMCS_IF_WAN_DETECT_MODE_GPON);
		setAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, buf);
	}
	
	/*set xpon link status attribute to default value*/
	if(getAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_LINK_STA, buf)  != TCAPI_PROCESS_OK){		
		sprintf(buf,"%d",XPON_LINK_OFF);
		setAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_LINK_STA, buf);
	}

	strcpy(nodeName[1],PONCOMMON);
	/*set phy status attribute to default value*/
	strcpy(buf,PONCOMMON_XPON_PHY_DOWN);
	setAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_PHY_STATUS, buf);
	
	/*set GPON bind interface attribute to default value*/
	strcpy(buf,GPON_BIND_IFC_FAIL);
	setAttrValue(top, nodeName, PONCOMMON_ATTR_GPON_BIND_IFC, buf);

#if 0// defined(TCSUPPORT_CT_PON)
	/* set hasMap to 0 */
	strcpy(buf, "0");
	setAttrValue(top, nodeName, "hasMap", buf);
#endif

#if defined(TCSUPPORT_PRODUCTIONLINE) && defined(TCSUPPORT_RESERVEAREA_EXTEND)
	memset(&para, 0, sizeof(para));
	tcgetProLinePara(&para,0);

	if((PRODUCTLINEPARAMAGIC == para.magic) && (para.flag & PL_CP_XPONMODE_FLAG))
	{
		if(0 != strcmp("GPON", para.xponmode) && 0 != strcmp("EPON", para.xponmode))
		{
			setAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_MODE, DEFAULT_XPONMODE);
		}
		else
		{
			setAttrValue(top, nodeName, PONCOMMON_ATTR_XPON_MODE, para.xponmode);
		}
	}
#endif

	/*set  xpon data traffic status attribute to default value*/
	strcpy(buf,ONU_WAN_DATA_LINK_DOWN);		
	setAttrValue(top, nodeName, PONCOMMON_ATTR_DATA_TRAFFIC_STA, buf);
	
	/*set  xpon data traffic status attribute to default value*/
	strcpy(buf,"down");		
	setAttrValue(top, nodeName, "losStatus", buf);
	
	/*set  xpon startXpon attribute to default value*/
	setAttrValue(top, nodeName, PONCOMMON_ATTR_START_XPON, NO_START_XPON_ATTR_PROC);
	
	/*set  xpon data online duration attribute to default value*/
	strcpy(buf,"0");		
	setAttrValue(top, nodeName, PONCOMMON_ATTR_DATA_UP_TIME, buf);	

#if defined(TCSUPPORT_CT_OMCI_CFG_SHRINK)
#ifdef TCSUPPORT_PONMGR
		/* move the ponapi_init from main() temporary by kevin, because the ponapi_init()  
		   must be called after wan module (GPON/EPON) ready */
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],XPON_NODE);
		strcpy(nodeName[1],PON_LINK_CFG); 
		if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, xPONModeAttr) == TCAPI_PROCESS_OK){
			xPONMode = atoi(xPONModeAttr);
		}
		if(ponapi_init(xPONMode) != EXEC_OK) {
			tcdbg_printf("PONAPI initial error\n") ;
		}
#endif
	
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
		strcpy(nodeName[0],XPON_NODE);
		strcpy(nodeName[1],PON_LINK_CFG); 
		if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, xPONModeAttr) == TCAPI_PROCESS_OK){
			xPONMode = atoi(xPONModeAttr);
#ifdef TCSUPPORT_EPON_OAM
			if((xPONMode == XMCS_IF_WAN_DETECT_MODE_AUTO) || (xPONMode == XMCS_IF_WAN_DETECT_MODE_EPON)){
				system("/userfs/bin/epon_oam &");
			}
#endif		
#ifdef TCSUPPORT_OMCI
			if((xPONMode == XMCS_IF_WAN_DETECT_MODE_AUTO) || (xPONMode == XMCS_IF_WAN_DETECT_MODE_GPON)){
				/* move /userfs/bin/omci & from rcS to here: 
				must need after ponapi_init() */
#if defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CT_PON_C5)
				system("taskset 0x8 /userfs/bin/omci &");
				system("renice -10 `pidof omci`");
#else
				system("/userfs/bin/omci &");
#endif
			}
#endif
#if 0
			if(xPONMode != -1){
				setXponMode(xPONMode);
			}
#endif
		}else{
			tcdbg_printf("getAttrValue-> PON_LINK_CFG_ATTR_XPON_MODE error\n") ;
		}
	/*check the onu type 
	if it is SFU, brctl addif br0 pon.
	*/
		checkOnuType(top);	
#endif
#endif
	
	
	return SUCCESS;
}

/*******************************************************************************************
**function name
	getXponMode
**description:
	get xPON detected mode
 **retrun :
 	-1:	failure
 	other: success(0:auto, 1:gpon, 2:epon)
**parameter:
 	 
********************************************************************************************/
int getXponMode(void){
#ifndef TCSUPPORT_PONMGR
	return XMCS_IF_WAN_DETECT_MODE_GPON;
#else
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;

	if(PonApi_SysLinkConfig_Get(&sysLinkCfg) != EXEC_OK) {
//		tcdbg_printf("getXponMode->Exec. Failed: Get the 'SysLinkConfig' command failed\n") ;
		return -1;
	}
	return sysLinkCfg.WanDetectMode;
#endif
}

/*******************************************************************************************
**function name
	setXponMode
**description:
	set xPON detected mode
 **retrun :
 	-1:	failure
 	0: success
**parameter:
 	xPONMode: (0:auto, 1:gpon, 2:epon)
********************************************************************************************/
int setXponMode(int xPONMode){
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;
	int oldxPONMode = 0;

	if((xPONMode != XMCS_IF_WAN_DETECT_MODE_AUTO)
		&& (xPONMode != XMCS_IF_WAN_DETECT_MODE_GPON)
		&& (xPONMode != XMCS_IF_WAN_DETECT_MODE_EPON)){
		tcdbg_printf("setXponMode->parameter failed \n") ;
		return -1;
	}

	/*change EPON to GPON*/	
	sysLinkCfg.WanDetectMode = xPONMode ;	
	if(PonApi_SysLinkConfig_Set((MASK_SysWanDetectMode), &sysLinkCfg) != EXEC_OK) {
		tcdbg_printf("setXponMode->Exec. Failed: Set the 'SysLinkConfig' command failed\n\n") ;
		return -1;
	}

	/*start new xPON mode*/
	sysLinkCfg.LinkStart = XPON_NODE_ENABLE;
	if(PonApi_SysLinkConfig_Set((MASK_SysLinkStart), &sysLinkCfg) != EXEC_OK) {
		tcdbg_printf("setXponMode->Exec. Failed: Set the 'SysLinkConfig' command START failed\n") ;
		return -1;
	}

#endif
	return 0;
}

/*
 * Set LLID's Link State:
 * 	state: 0=down, 1=up
 * Return: FAIL(-1) or SUCCESS(0)
 * */
int epon_sys_set_link_state_cfg(u_char state){
	int sta[EPON_LLID_MAX_NUM] = {0};
	char buf[128] = {0};
	FILE *fp = NULL;
	static int protect = 1;
	int i = 0;

	if (protect != 1)
		return FAIL;
	protect = 0;
	
	fp = fopen(OAM_LINK_STAT_PATH, "w+");
	if (fp ==  -1){
		return FAIL;
	}
	
	fscanf(fp, "%d %d %d %d %d %d %d %d", &sta[0], &sta[1], &sta[2], 
		&sta[3], &sta[4], &sta[5], &sta[6], &sta[7]);

	for (i = 0; i < 8; i++)
		sta[i] = state;
	
	fprintf(fp, "%d %d %d %d %d %d %d %d", sta[0], sta[1], sta[2], 
		sta[3], sta[4], sta[5], sta[6], sta[7]);
	fclose(fp);

#if defined(TCSUPPORT_XPON_LED)
	if (state == LINK_UP){
		xpon_led("2");
	}
#endif		

	protect = 1;
	return SUCCESS;
}


int setXponModeSwitch(int xPONMode, int start){
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;
	int oldxPONMode = 0;

	if((xPONMode != XMCS_IF_WAN_DETECT_MODE_AUTO)
		&& (xPONMode != XMCS_IF_WAN_DETECT_MODE_GPON)
		&& (xPONMode != XMCS_IF_WAN_DETECT_MODE_EPON)){
		tcdbg_printf("setXponMode->parameter failed \n") ;
		return -1;
	}
	
	/*change EPON to GPON*/	
	sysLinkCfg.WanDetectMode = xPONMode ;	
	if (start == ENABLE) {
		if(PonApi_SysLinkConfig_Set((MASK_SysWanDetectMode), &sysLinkCfg) != EXEC_OK) {
			tcdbg_printf("setXponMode->Exec. Failed: Set the 'SysLinkConfig' command failed\n\n") ;
			return -1;
		}
	}

	/*start new xPON mode*/
	sysLinkCfg.LinkStart = start;
	if(PonApi_SysLinkConfig_Set((MASK_SysLinkStart), &sysLinkCfg) != EXEC_OK) {
		tcdbg_printf("setXponMode->Exec. Failed: Set the 'SysLinkConfig' command START failed\n") ;
		return -1;
	}
#endif
	return 0;
}


/*******************************************************************************************
**function name
	getXponLinkSta
**description:
	get xPON mode and link status
 **retrun :
 	-2:	failure
 	0: success
**parameter:
********************************************************************************************/
int getXponLinkSta(mxml_node_t *top, char *attr, char *buffer){
#ifndef TCSUPPORT_PONMGR			
	if(top == NULL || attr == NULL || buffer == NULL){
		return -1;
	}
	sprintf(buffer, "%d", XPON_LINK_OFF);
#else
	int i ;	
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;
	int mode = 0;

	if(top == NULL || attr == NULL || buffer == NULL){
		return -1;
	}

	if(PonApi_SysLinkConfig_Get(&sysLinkCfg) != EXEC_OK) {
//		tcdbg_printf("*LHS DBG->xPON.c->getXponLinkSta->Exec. Failed: Get the 'SysLinkConfig' command failed\n") ;
		return FAIL;
	}
	
	if(strcmp(attr, PONCOMMON_ATTR_XPON_LINK_STA) == 0){
		sprintf(buffer, "%d", sysLinkCfg.LinkStatus);
	}else{
		return -2;
	}
	
//	tcdbg_printf("xPON.c->getXponLinkSta->Exec. ok, sysLinkCfg.WanDetectMode=%d",sysLinkCfg.WanDetectMode);
//	tcdbg_printf("xPON.c->getXponLinkSta->Exec. ok, sysLinkCfg.LinkStatus=%d",sysLinkCfg.LinkStatus);

#endif
	return 0;
}

#ifdef TCSUPPORT_WAN_GPON
/*******************************************************************************************
**function name
	getGponOnuSta
**description:
	get GPON onu state
 **retrun :
 	onu state(O1 ~ O7)
**parameter:
********************************************************************************************/
int getGponOnuSta(void){
#ifndef TCSUPPORT_PONMGR			
	return GPON_STATE_O1;
#else
	MACRO_CMD_STRUCT(GponSystemInfo) GponOnuInfo ;

	if(PonApi_GponSystemInfo_Get(&GponOnuInfo) != EXEC_OK) {
		return FAIL;
	}
	
	return GponOnuInfo.ActState;
#endif
}
#endif
/*****************************************************************************************
****function name
	getGponTrafficSta
****description
	get Gpon real upstream traffic states
****return:
	0: traffic Up
	1: traffic Down
****************************************************************************************/
#ifdef TCSUPPORT_WAN_GPON
int getGponTrafficSta(void)
{
	int ret = 0;
	int fp = -1;
	unsigned int flag = 1;
	fp = open("/dev/pon", O_RDONLY);
	if (fp == -1)
	{
		printf("gpon IOCTL file open failed!\n");
		return -1;
	}

	ret = ioctl(fp, GPON_GET_TRAFFIC_STATE, &flag);
	if (ret != 0)
	{
		printf("gpon IOCTL error!\n");
	}	
	close(fp);
	return flag;

}
#endif

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
int getXponUpstreamBlock(void)
{
	int ret = 0;
	int fp = -1;
	unsigned int flag = 1;
	fp = open("/dev/pon", O_RDONLY);
	if (fp == -1)
	{
		printf("gpon IOCTL file open failed!\n");
		return -1;
	}
	
	ret = ioctl(fp, XPON_GET_UPSTREAM_BLOCK_STATE, &flag);
	if (ret != 0)
	{
		printf("gpon IOCTL error!\n");
	}	
	close(fp);
	return flag;
}
#endif
/*****************************************************************************************
****function name
	getEponTrafficSta
****description
	get Epon upstream traffic states
****return:
	0: traffic Up
	1: traffic Down
	-1:error
****************************************************************************************/
#ifdef TCSUPPORT_WAN_EPON
inline int getEponTrafficSta(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char attrVal[40] = {0};
	int sta[MAX_EPON_LLID_NUM] = {0};
	FILE *fp = NULL;
	int i=0;

	strcpy(nodeName[0], "XPON");
	strcpy(nodeName[1], "Common");
	if(getAttrValue(top, nodeName, "phyStatus", attrVal) != TCAPI_PROCESS_OK)
	{
		return -1;
	}
	if (strcmp(attrVal, PONCOMMON_XPON_PHY_EPON_UP) != 0)
	{
		return 1;
	}
	fp = fopen(OAM_LINK_STAT_PATH, "r");
	if (fp == NULL)
	{
		return -1;
	}
	fscanf(fp, "%d %d %d %d %d %d %d %d", &sta[0], &sta[1], &sta[2], 
	&sta[3], &sta[4], &sta[5], &sta[6], &sta[7]);
	for (i=0; i<MAX_EPON_LLID_NUM; ++i)
	{
		if (sta[i] == LINK_UP 
#ifndef TCSUPPORT_CPU_EN7521
			&& (getXponUpstreamBlock() == 0)
#endif
			)
		{
			fclose(fp);
			return 0;		
		}
	}
	fclose(fp);			
	return 1;
}
#endif

/*******************************************************************************************
**function name
	getTrafficSta
**description:
	get xpon(gpon or epon) traffic status (up or down)
 **retrun :
 	-1:	failure
 	0: success
**parameter:
********************************************************************************************/
int getTrafficSta(mxml_node_t *top, char *attr, char *buffer){
	char attrVal[40] = {0};
	char attrName[MAX_NODE_NAME] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];	
	int xPonLinkSta = XPON_LINK_OFF;
	int i = 0;
	mxml_node_t *parent = NULL;
	mxml_node_t *curNode = top;

	if(top == NULL || attr == NULL || buffer == NULL){
		return -1;
	}
	if(strcmp(attr, PONCOMMON_ATTR_DATA_TRAFFIC_STA)){		
		tcdbg_printf("getTrafficSta->attr=%s, fail \n",attr);
		return -1;
	}

	/*set default value to attribute*/
	strcpy(buffer, ONU_WAN_DATA_LINK_DOWN);
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], XPON_NODE);

	/*get xpon link status*/
	strcpy(nodeName[1], PON_LINK_CFG);
	strcpy(attrName, PONCOMMON_ATTR_XPON_LINK_STA);
	memset(attrVal, 0, sizeof(attrVal));
	if(getXponLinkSta(top, attrName, attrVal) != 0){
//		tcdbg_printf("getTrafficSta->getXponLinkSta, linkstat fail\n");
		return -1;
	}
	/*updata real link status to tcapi*/
	/* find the attr, if not find, will new the node*/
	for(i=0; i < MAX_ARG_NUM; i++){
		parent=curNode;
		if(strlen(nodeName[i])==0){
			break;
		}
		curNode=mxmlFindElement(parent, parent, nodeName[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			tcdbg_printf("xpon_node_read: can't find the node = %s, will add the node\n", nodeName[i]);
			curNode = mxmlNewElement(parent,  nodeName[i] );;
		}
	}
	if(curNode==NULL){
		return -1;
	}	
	mxmlElementSetAttr(curNode, attrName, attrVal);
	
	xPonLinkSta = atoi(attrVal);	
	memset(attrVal, 0, sizeof(attrVal));	
	switch(xPonLinkSta){
		case XPON_LINK_GPON:
			/*get gpon traffic status*/
			if(getGponTrafficSta() != 0 )
				break;       
            strcpy(buffer,ONU_WAN_DATA_LINK_UP);
			break;
		case XPON_LINK_EPON:
			/*get epon traffic status*/
			if(getEponTrafficSta(top) == 0)
			{
					strcpy(buffer,ONU_WAN_DATA_LINK_UP);
			}
			break;
		case XPON_LINK_OFF:
		default:			
			break;			
	}

	return 0;
}

/*******************************************************************************************
**description:
	get gpon online duration
 **return :
 	-1:	failure
 	0: success
**parameter:
	duration time
********************************************************************************************/
int getGponOnlineDuration(unsigned long* durationPtr)
{
	int ret = 0;
	int fp = -1;
	
	fp = open("/dev/pon", O_RDONLY);
	if (fp == -1)
	{
		printf("gpon IOCTL file open failed!\n");
		return -1;
	}

	ret = ioctl(fp, GPON_GET_ONLINE_DURATION, durationPtr);
	if (ret != 0)
	{
		printf("gpon IOCTL error!\n");
	}
	close(fp);
	return ret;

}

/*******************************************************************************************
**function name
	getLosSta
**description:
	get xpon(gpon or epon) los status (up or down)
	if phy los, reutrn 0, if phy not not return 1
 **retrun :
 	-1:	failure
 	0: success
**parameter:
********************************************************************************************/

int getLosSta(char *buffer)
{
	int fd, losStatus, ret;
	char string[4] = {0};
	fd = open(LOS_STATUS_PATH,O_RDONLY|O_NONBLOCK);

	sprintf(buffer, "down");
	
	if (fd != -1) {
		ret = read(fd, string, sizeof(string));
		close(fd);

		if(ret) {
			losStatus = atoi(string);
			if(1 == losStatus) {
				strcpy(buffer, "up");
			}
		}
	}
	else {
		fprintf(stderr,"no file %s\n",LOS_STATUS_PATH);
		
	}

	return 0;
}


/*******************************************************************************************
**function name
	getPsThreadInfo
**description:
	find string from ps results
 **retrun :
	-1: failure
	0: no find
	1: find
**parameter:
	str: find string
********************************************************************************************/
int getPsThreadInfo(char * str){
	FILE *fp= NULL;
	char tempStr[80] = {0};

	if(str == NULL){
		tcdbg_printf("getPsThreadInfo->getPsThreadInfo->str == NULL \n") ;
		return -1;
	}
	if(strlen(str)== 0){
		return 0; //no find
	}
	
	sprintf(tempStr,"/bin/ps >%s",XPON_PS_PATH);
	system(tempStr);

	fp = fopen(XPON_PS_PATH, "r");
	if(fp == NULL){
		tcdbg_printf("getPsThreadInfo->fopen fail == NULL \n") ;
		return -1;
	}
	
	memset(tempStr, 0, sizeof(tempStr));	  
	while(fgets(tempStr, 80, fp)){
		if(strstr(tempStr,str)){
			fclose(fp);
			return 1; // find
		}
	}

	fclose(fp);
	return 0; //no find
}

int xPONStart(mxml_node_t *top){
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char attrVal[MAX_NODE_NAME] = {0};
	int xPONMode = -1;
	uint8 startFlag = 0;

	if(top == NULL){
		return -1;
	}
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], XPON_NODE);
	strcpy(nodeName[1], PON_LINK_CFG);
	/*get xpon mode*/
	if(getAttrValue(top, nodeName, PON_LINK_CFG_ATTR_XPON_MODE, attrVal) == TCAPI_PROCESS_OK){
		xPONMode = atoi(attrVal);
#ifdef 	TCSUPPORT_OMCI	
		/*get omci status*/			
		strcpy(nodeName[0], "OMCI");
		strcpy(nodeName[1], "Entry");
		if(getAttrValue(top, nodeName, OMCI_ENTRY_READY, attrVal) == TCAPI_PROCESS_OK){
			if(strcmp(attrVal,OMCI_ENTRY_READY_WORK)== 0 ){
				startFlag |= XPON_GPON_OMCI_WORK;
			}
		}
#endif
#ifdef 	TCSUPPORT_EPON_OAM
		/*get epon oam status*/
		strcpy(nodeName[0], "Info");
		strcpy(nodeName[1], "Oam");
		if(getAttrValue(top, nodeName, OAM_ENTRY_READY, attrVal) == TCAPI_PROCESS_OK){
			if(strcmp(attrVal,OAM_ENTRY_READY_WORK) == 0){
				startFlag |= XPON_EPON_OAM_WORK;
			}
		}
#endif
		/*start xPON*/
		switch(xPONMode){		
			case XMCS_IF_WAN_DETECT_MODE_AUTO:
				if((startFlag & (XPON_GPON_OMCI_WORK|XPON_EPON_OAM_WORK)) ==  (XPON_GPON_OMCI_WORK|XPON_EPON_OAM_WORK)){
					setXponMode(xPONMode);
				}
				break;
			case XMCS_IF_WAN_DETECT_MODE_GPON:
				if((startFlag & XPON_GPON_OMCI_WORK) == XPON_GPON_OMCI_WORK){
					setXponMode(xPONMode);
				}
				break;
			case XMCS_IF_WAN_DETECT_MODE_EPON:
				if((startFlag & XPON_EPON_OAM_WORK) == XPON_EPON_OAM_WORK){
					setXponMode(xPONMode);
				}
				break;
			default:
				return -1;
		}
		
	}

	return 0;

}

#ifdef TCSUPPORT_PON_IP_HOST
void setPonWanIf(mxml_node_t *top){
	int pvc_index;
	int entry_index;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char wanMode[8] = {0};
	char temp[32] = {0};
	char cmd[32] = {0};
	char ifname[8] = {0};
	
	int flag = 0; //0x01 voip iface; 0x10 bridge iface
	
	/*clear WanIf*/
	memset(temp, 0, sizeof(temp));
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],"XPON");
	sprintf(nodeName[1], "IPHost");	
	if(getAttrValue(top, nodeName, "WanIf", temp) == TCAPI_PROCESS_OK){
		setAttrValue(top, nodeName, "WanIf", "" );
	}
	memset(temp, 0, sizeof(temp));

	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0],WAN);
			sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
			if(getAttrValue(top, nodeName, "WanMode", wanMode) != TCAPI_PROCESS_OK )
				continue;

			if(!(flag & 0x01) && strcmp(wanMode, "Route") == 0){
				if(getAttrValue(top, nodeName, "ServiceList", temp) != TCAPI_PROCESS_OK )
					continue;
				
				if(strcmp(temp, "VOICE") == 0){	//wan only support voice
					sprintf(ifname, DEFAULT_NASNAME_FORMAT, pvc_index, entry_index);
					sprintf(cmd, "echo voip %s > /proc/tc3162/pon_wanIf", ifname);
					system(cmd);

					memset(nodeName, 0, sizeof(nodeName));
					strcpy(nodeName[0],"XPON");
					sprintf(nodeName[1], "IPHost");
					setAttrValue(top, nodeName, "WanIf", ifname);

					flag |= 0x01;
					if(flag & 0x10)
						break;
				}			
			}else if(!(flag & 0x10) && strcmp(wanMode, "Bridge") == 0){
				if(getAttrValue(top, nodeName, "Active", temp) != TCAPI_PROCESS_OK )
					continue;
                
				if(strcmp(temp, "Yes") == 0){	//bridge wan for traffic except voip
					sprintf(ifname, DEFAULT_NASNAME_FORMAT, pvc_index, entry_index);
					sprintf(cmd, "echo bridge %s > /proc/tc3162/pon_bridge_wanIf", ifname);
					system(cmd);
					
					flag |= 0x10;
					if(flag & 0x01)
						break;
				}
			}

		}
	}
}
#endif

char lastSN[32] = {0};
int SNModified(mxml_node_t *top){	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char buffer[32] = {0};
	MACRO_CMD_STRUCT(SysLinkConfig) sysLinkCfg ;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], GPON);
	strcpy(nodeName[1], ONU);
#if defined(TCSUPPORT_CMCC)	
	if ((getAttrValue(top, nodeName, "Password", buffer) == TCAPI_PROCESS_OK) && strcmp(buffer,lastPWD) != 0)
#else
	if ((getAttrValue(top, nodeName, "SerialNumber", buffer) == TCAPI_PROCESS_OK) && strcmp(buffer,lastSN) != 0)
#endif
	{
    #ifdef TCSUPPORT_OMCI
        system("killall -9 omci");
#if defined(TCSUPPORT_CT_PON_CY) || defined(TCSUPPORT_CT_PON_C5)
        system("taskset 0x8 /userfs/bin/omci &");
        system("renice -10 `pidof omci`");
#else
        system("/userfs/bin/omci &");
#endif
    #endif
		tcdbg_printf("\r\n[%s]SerialNumber=%s,lastSN=%s\n",__FUNCTION__,buffer,lastSN);
#if defined(TCSUPPORT_CMCC)	
		tcdbg_printf("\r\n[%s]Password=%s,lastPWD=%s\n",__FUNCTION__,buffer,lastPWD);
#endif
		checkOnuType(top);

		sysLinkCfg.LinkStart = ENUM_DISABLE ;
		if(PonApi_SysLinkConfig_Set((MASK_SysLinkStart), &sysLinkCfg) != EXEC_OK) {
			tcdbg_printf("Exec. Failed: Set the 'SysLinkConfig' command failed\n\n") ;
			return -1;
		}

		sysLinkCfg.LinkStart =  ENUM_ENABLE;
		if(PonApi_SysLinkConfig_Set((MASK_SysLinkStart), &sysLinkCfg) != EXEC_OK) {
			tcdbg_printf("Exec. Failed: Set the 'SysLinkConfig' command failed\n\n") ;
			return -1;
		}
	}
	return 0;
}

int checkOnuType(mxml_node_t *top)
{
#ifdef TCSUPPORT_PONMGR
	MACRO_CMD_STRUCT(GponSnPasswd) gponSnPasswd ;
	MACRO_CMD_STRUCT(SysOnuType) sysOnuType ;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i = 0;
	char buffer[25] = {0};
	char passBuffer[25] = {0};
	unsigned int value = 0;
	char onuType[25] = "2";
	char sntemp[32] = {0};

	if(top == NULL){
		return -1;
	}
	
	if(PonApi_SysOnuType_Get(&sysOnuType) != EXEC_OK) {
		tcdbg_printf("Exec. Failed: Get the 'SysOnuType' command failed\n") ;
		return -1;
	}
		
	if(sysOnuType.OnuType != ENUM_SYSTEM_ONU_TYPE_HGU)
	{
		strcpy(onuType, "1");
		system("echo 8192 8192 8192 8192 > /proc/tc3162/eth_ports");
		
#ifdef TCSUPPORT_PON_IP_HOST
		setPonWanIf(top);
		system("echo enable 0 > /proc/tc3162/wan_mvlan");
#endif
	}
	else
	{
		system("echo 8192 8192 8192 8192 > /proc/tc3162/eth_ports");
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], GPON);
	strcpy(nodeName[1], ONU);
	setAttrValue(top, nodeName, "OnuType", onuType);
	if ((getAttrValue(top, nodeName, "SerialNumber", buffer) == TCAPI_PROCESS_OK) && (getAttrValue(top, nodeName, "Password", passBuffer) == TCAPI_PROCESS_OK))
	{
		strcpy(lastSN,buffer);
#if defined(TCSUPPORT_CMCC)
		strcpy(lastPWD,passBuffer);
#endif
		memset(&gponSnPasswd, 0, sizeof(MACRO_CMD_STRUCT(GponSnPasswd))) ;
		if(strlen(buffer) == 16){
			memcpy(sntemp, buffer, 8) ;
			value = strtoul(sntemp, NULL, 16) ;
			gponSnPasswd.Serial[0] = (unsigned char)(value>>24) ;
			gponSnPasswd.Serial[1] = (unsigned char)(value>>16) ;
			gponSnPasswd.Serial[2] = (unsigned char)(value>>8) ;
			gponSnPasswd.Serial[3] = (unsigned char)(value) ;

			
			value = strtoul(buffer+8, NULL, 16) ;
			gponSnPasswd.Serial[4] = (unsigned char)(value>>24) ;
			gponSnPasswd.Serial[5] = (unsigned char)(value>>16) ;
			gponSnPasswd.Serial[6] = (unsigned char)(value>>8) ;
			gponSnPasswd.Serial[7] = (unsigned char)(value) ;
			
		}else{
			memcpy(gponSnPasswd.Serial, buffer, 4) ;
			
			value = strtoul(buffer+4, NULL, 16) ;
			gponSnPasswd.Serial[4] = (unsigned char)(value>>24) ;
			gponSnPasswd.Serial[5] = (unsigned char)(value>>16) ;
			gponSnPasswd.Serial[6] = (unsigned char)(value>>8) ;
			gponSnPasswd.Serial[7] = (unsigned char)(value) ;
		}

#ifdef TCSUPPORT_WAN_GPON
		if(0!=saveGponPasswd(passBuffer,& gponSnPasswd))
		{
			tcdbg_printf("save Gpon Passwd error\n");
		}
#endif

		
		if(getAttrValue(top, nodeName, "EmergencyState", buffer)!=TCAPI_PROCESS_OK) {
            gponSnPasswd.EmergencyState=0;
        }else {
            i=atoi(buffer);
            if(i==0 || i==1) {
                gponSnPasswd.EmergencyState=i;
            }else{
				gponSnPasswd.EmergencyState=0;
			}
        }	

		if(PonApi_GponSnPasswd_Set(0, &gponSnPasswd) != EXEC_OK) {
			tcdbg_printf("Exec. Failed: Set the 'GponSnPasswd' command failed\n\n") ;
			return -1;
		}
	}

#endif //TCSUPPORT_PONMGR
	return 0;
}
#endif // defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#ifdef TCSUPPORT_WAN_GPON
int  saveGponPasswd(char *passBuffer,MACRO_CMD_STRUCT(GponSnPasswd) * gponSnPasswd )
{
	int i,len,db = 0;
	
	if(strlen(passBuffer) > CONST_GPON_HEX_PASSWD_LENS)
	{
		tcdbg_printf("Input Error: Password length must < %d characters in %s\n", CONST_GPON_HEX_PASSWD_LENS,__func__) ;
		return -1;
	}
	if(strncmp("0x",passBuffer,2)==0 || strncmp("0X",passBuffer,2)==0)
	{	
		gponSnPasswd->PasswdLength = strlen(passBuffer)-2;
		gponSnPasswd->hexFlag = 1;
		i = 0;

		for(len = 0; len<gponSnPasswd->PasswdLength; len +=2)
		{
			sscanf(passBuffer+len+2, "%2X",&db);
			gponSnPasswd->Password[i++]=(unsigned char) db;
		}
	}
	else
	{
		if(strlen(passBuffer) > CONST_GPON_PASSWD_LENS) 
		{
			printf("Input Error: Password length must < %d characters in %s\n", CONST_GPON_PASSWD_LENS,__func__) ;
			return -1;
		}
			gponSnPasswd->hexFlag=0;
			strcpy((char *)gponSnPasswd->Password, passBuffer) ;
	}
	return 0;
}
#endif
