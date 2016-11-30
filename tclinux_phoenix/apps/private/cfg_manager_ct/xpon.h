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
	xpon.h

	Abstract:

	Revision History:
	Who			When			What
	--------	----------		----------------------------------------------
	Name		Date			Modification logs
	jq.zhu		2012/6/28	Create
	andy.Yi		2012/7/16     add the pmgr node
*/

#ifndef _XPON_H_
#define _XPON_H_

#include "cfg_manager.h"
#ifdef TCSUPPORT_PONMGR
#include "api/mgr_api.h"
#endif

#ifdef TCSUPPORT_PMMGR
#define PM                                				"PM"
#define ENTRY 							"Entry"
#define EPON_MODE_ENABLE   				2
#define GPON_MODE_ENABLE  				1
#define DISABLE 							0
#define PMMGR_HISTORY_DATA			0        							/* data flag get PM me instance history data*/
#define PMMGR_CURRENT_DATA			1	 							/* data flag get PM me instance current data*/
#define PMMGR_THRESHOLD                  		2       							/* data flag get /set pm me instance threshold*/
#define PMMGR_CREATE					3								/* data flag create pm me instance*/
#define PMMGR_DELETE					4								/* data flag delete pm me instance*/
#define PMMGR_SYNCHRONIZE_TIME		5								/* data flag synchronize time*/
#define PMMGR_STATISTICS_TIME			6								/* data flag get statistics time*/
#define PMMGR_INIT						7     								/* re-init pmgr*/
#define PMMGR_PORT_ID					"portId"	
#define PMMGR_CLASS_ID					"classId"
#define PMMGR_DATA_FLAG				"dataFlag"
#define PMMGR_ATTR_INDEX				"attrIndex"      					/* attr index in pm struct */
#define PMMGR_ATTR_DATA				"attrData"       					/* attr data when get, or threshold data when set*/

#ifdef TCSUPPORT_EPON_OAM_CTC
#define PMMGR_EPON_NONE				0								/* data flag for get/set none*/
#define PMMGR_EPON_CYCLE_TIME			(PMMGR_EPON_NONE+1)			/* data flag for get/set cycle time*/
#define PMMGR_EPON_ENABLE				(PMMGR_EPON_CYCLE_TIME+1)		/* data flag for get/set all pm enable or disable*/
#define PMMGR_EPON_CURRENT_DATA		(PMMGR_EPON_ENABLE+1)			/* data flag for get/set current data*/
#define PMMGR_EPON_HISTORY_DATA		(PMMGR_EPON_CURRENT_DATA+1)	/* data flag for get history data*/
#define PMMGR_EPON_THRESHOLD			(PMMGR_EPON_HISTORY_DATA+1)	/* data flag for get/set alarm's threshold*/
#define PMMGR_EPON_STATUS				(PMMGR_EPON_THRESHOLD+1)		/* data flag for get/set alarm's status*/
#define PMMGR_EPON_INIT				(PMMGR_EPON_STATUS+1)			/* data flag for reinit pmmgr */

#define EPON						"EPON"
#define PMMGR_ALARM_ID			"alarmId"
#define PMMGR_ATTR_CYCLE		"cycleTime" 								/* cycle*/
#define PMMGR_ATTR_STATUS		"status"  									/* enable/disable*/
#define PMMGR_ATTR_SET			"set"										/* the alarm threshold set*/
#define PMMGR_ATTR_CLEAR		"clear"									/* the alarm threshold clear*/

#endif

int 
pmmgr_init(void);
int
pmmgr_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
pmmgr_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
pmmgr_boot(mxml_node_t *top);
#endif

#ifdef TCSUPPORT_WAN_GPON

#define GPON 				"GPON"
#define COMMON 				"Common"
#define ONU					"ONU"
#define ANI					"ANI"
#define TCONT				"TCONT"
#define GEMPORT				"GEMPort"
#define OLT					"OLT"
#define TRTCM				"Trtcm"
#define TRTCM_CHANNEL		"channel"
#define TRTCM_CIR			"cir"
#define TRTCM_PIR			"pir"
#define TRTCM_CBS			"cbs"
#define TRTCM_PBS			"pbs"

#define CURRENTATTRIBUTE 	"CurrentAttribute"
#define MAINIMAGE			"SoftImage0"
#define SLAVEIMAGE    		"SoftImage1"
#define CAPABILITY			"Capability"
#define LOID					"LOIDAuth"

int
gpon_init(void);
int
gpon_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
gpon_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
gpon_boot(mxml_node_t *top);
#endif

#ifdef TCSUPPORT_OMCI
#define INIT_GPON_IFC					"initGponIfc"
#define INIT_GPON_IFC_ENABLE			1
#define INIT_GPON_IFC_DISABLE			0

#define GPON_IFC						"interface"
#define GPON_GEMPORT_LIST				"gemportlist"
#define GPON_HANDLE_PON_IFC				"handleifc" //0:default, 1:create, 2:del, 3:modify, 4:unassign gemport
#define GPON_GEMPORT_LIST_SEP			'_'
#define GPON_MAX_ATTR_NUM				32
#define GPON_HANDLE_PON_IFC_DEFAULT		"0"
#define GPON_HANDLE_PON_IFC_CREATE		1
#define GPON_HANDLE_PON_IFC_DEL			2
#define GPON_HANDLE_PON_IFC_MODIFY		3
#define GPON_HANDLE_PON_IFC_UNASSIGN	4 //unassign gemport in this inteface
#define GPON_PRE_CREATE_IFC_NUM			8

#define MAX_GPON_IFC_GEM_PORT_NUM		8
#define MAX_GPON_OMCI_NODE_VAL_LEN		48
#define	OMCI_MAC_BRIDGE_PORT_PON_PORT_BASIC 			13
#define MAX_PON_IFC_NUM					32

#define OMCI_ENTRY_READY				"Ready"
#define OMCI_ENTRY_READY_DOWN			"down"
#define OMCI_ENTRY_READY_START			"start"
#define OMCI_ENTRY_READY_WORK			"work"

#define GPON_QOS_CFG 				"QOSCfg"
#define GPON_CONGEST_CFG 			"CongestCfg"
#define GPON_TRTCM_CFG 				"TrtcmCfg"

#define PON_QOS						"QOS" 
#define PON_CONGEST					"Congest"
#define PON_TRTCM_PARAMS			"TrtcmParam" 

#define PON_QOS_MASK				"QOSMask" 
#define PON_CONGEST_MASK			"CongestMask"
#define PON_TRTCM_PARAMS_MASK		"TrtcmMask" 

#define PON_QOS_TYPE				"QOSType"
#define PON_QOS_TYPE_DEFAULT_VALUE	'X'
#define PON_QOS_TYPE_SET        	1
#define PON_QOS_TYPE_GET        	0

#define ATTR_QOS_WEIGHT_TYPE		"WeightType"
#define ATTR_QOS_WEIGHT_SCALE		"WeightScale"
#define ATTR_QOS_VALUE				"QOSValue"

#define ATTR_CONGEST_TRTCM_MODE		"CongestTrtcmMode"
#define ATTR_CONGEST_DEI_DROP_MODE	"CongestDeiDropMode"
#define ATTR_CONGEST_THRED_MODE		"CongestThredMode"
#define ATTR_CONGEST_SCALE			"CongestScale"
#define ATTR_CONGEST_DROP_P			"CongestDropProbability"
#define ATTR_CONGEST_THRED_VALUE	"CongestThredValue"

#define ATTR_TRTCM_SCALE			"TrtcmScale"
#define ATTR_TRTCM_VALUE			"TrtcmValue"

#define GPON_HANDLE_DEF			0 //default
#define GPON_HANDLE_SET			1 

#define GPON_HANDLE_DEF_STR		"0"

#define MASK_DEF			0 //default

#define MASK_QOS_WEIGHT_PARAM 		(1<<0) 
#define MASK_QOS_CONFIG				(1<<1) 

#define MASK_TRTCM_SCALE 		(1<<0) 
#define MASK_TRTCM_PARAMETERS			(1<<1) 

#define MASK_CONGEST_TRTCM_MODE 		(1<<0) 
#define MASK_CONGEST_DEI_DROP_MODE		(1<<1) 
#define MASK_CONGEST_THRED_MODE 		(1<<2) 
#define MASK_CONGEST_SCALE				(1<<3) 
#define MASK_CONGEST_DROP_PROBILITY		(1<<4) 
#define MASK_CONGEST_THRED		(1<<5) 


#define CONFIGURATION_DOCUMENT_PATH 		"/tmp/omci/cfgDocument.xml"
#define DEFAULT_XML_PATH 					"/tmp/omci/default.xml"
#define XPON_PS_PATH 						"/tmp/var/log/xponps.txt"
#define XPON_OMCI_THREAD_CMD 				"/userfs/bin/omci"
#define XPON_OAM_THREAD_CMD 				"/userfs/bin/epon_oam"


int generalStatusPortalHandler(mxml_node_t *top);
int sendAvc2Omci(int avcId, int devIndex);
#define OMCI 				"OMCI"
//#define OMCI_DEBUG
#undef OMCI_DEBUG

int
omci_init(void);

int
omci_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
omci_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#endif

#ifdef TCSUPPORT_WAN_EPON
#define EPON 	"EPON"
#define ONU        "ONU"
#define PON		"PON"
#define LOIDAUTH        "LOIDAuth"
#define POWERSAVING "PowerSaving"
#define SERVICESLA	 "ServiceSLA"

int epon_init(void);
int epon_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int epon_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int epon_boot(mxml_node_t *top);

int eponLoidPswCmdExcuteFunc(mxml_node_t *top);
int eponPonCmdGetAttrFunc(mxml_node_t *top, mxml_node_t *curNode, char *attr);
int eponPonCmdExcuteFunc(mxml_node_t *top);

#define MASK_TRTCM_SCALE 		(1<<0) 
#define MASK_TRTCM_PARAMETERS			(1<<1) 
#define MASK_CONGEST_TRTCM_MODE 		(1<<0) 
#define MASK_CONGEST_DEI_DROP_MODE		(1<<1) 
#define MASK_CONGEST_THRED_MODE 		(1<<2) 
#define MASK_CONGEST_SCALE				(1<<3) 
#define MASK_CONGEST_DROP_PROBILITY		(1<<4) 
#define MASK_CONGEST_THRED		(1<<5) 
int eponServiceSlaCmdExcFunc(mxml_node_t *top);

#ifdef TCSUPPORT_EPON_DUAL_IMAGE
#define SOFTIMAGE	 "SoftImage"
#define SI_MAIN 		 "Main"
#define SI_ACTIVE	 "Active"

int softImageCmdExcuteFunc(mxml_node_t *top);
void softImageSetBootFlag(int flag);
#endif // TCSUPPORT_EPON_DUAL_IMAGE

#ifdef TCSUPPORT_EPON_OAM
#define OAM       "Oam"
#define CTC 		"CTC"
#define CUC		"CUC"
#define OAM_ENTRY				"Entry"
#define OAM_ENTRY_READY			"Ready"
#define OAM_ENTRY_READY_WORK	"work"
#define LINK_UP 1
#define LINK_DOWN 0
#define OAM_LINK_STAT_PATH "/tmp/oam_stat"

int oam_init(void);
int oam_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif // TCSUPPORT_EPON_OAM

#endif // TCSUPPORT_WAN_EPON
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
#define MAX_EPON_LLID_NUM				8
typedef enum {
	XPON_NODE_DISABLE = 0,
	XPON_NODE_ENABLE 
} xPONNodeEnable_t ;

typedef enum {
	XPON_LINK_OFF = 0,
	XPON_LINK_GPON,
	XPON_LINK_EPON
} xPONLinkStatus_t ;
typedef enum {
	XPON_GPON_OMCI_WORK =	(1<<0),
	XPON_EPON_OAM_WORK 	=	(1<<1)
} xPONStartFlag_t ;

#define XPON_NODE 							"XPON"

#define PONCOMMON 							"Common" //node
#define PONCOMMON_ATTR_XPON_PHY_STATUS 	 	"phyStatus"
#define PONCOMMON_XPON_PHY_DOWN				"down"
#define PONCOMMON_XPON_PHY_GPON_DOWN		"gpon_phy_down"
#define PONCOMMON_XPON_PHY_GPON_UP			"gpon_phy_up"
#define PONCOMMON_XPON_PHY_EPON_DOWN		"epon_phy_down"
#define PONCOMMON_XPON_PHY_EPON_UP			"epon_phy_up"

#define PONCOMMON_ATTR_GPON_BIND_IFC  		"GponBindIfc"
#define GPON_BIND_IFC_SUCC 					"Yes"
#define GPON_BIND_IFC_FAIL  				"No"

#define PONCOMMON_ATTR_START_XPON  			"startXpon"
#define START_XPON_ATTR_PROC  				"Yes"
#define NO_START_XPON_ATTR_PROC				"No"

#define PONCOMMON_ATTR_DATA_TRAFFIC_STA  	"trafficStatus"
#define PONCOMMON_ATTR_DATA_UP_TIME  		"upTime"

#define	PONCOMMON_LOS_STATUS				"losStatus"

#define ONU_WAN_DATA_LINK_UP 				"up"
#define ONU_WAN_DATA_LINK_DOWN 				"down"

#define PONCOMMON_ATTR_XPON_MODE            "xponMode"

#define PON_LINK_CFG					"LinkCfg" //node
#define PON_LINK_CFG_ATTR_XPON_MODE  	"Mode"
#define PONCOMMON_ATTR_XPON_LINK_STA  	"LinkSta"
#ifdef TCSUPPORT_PON_IP_HOST
#define PON_IPHOST			"IPHost" //node
#define PON_IPHOST_COMMIT_WAN_ATTR	"commitWan"
#define PON_IPHOST_WANIF_ATTR		"WanIf"
#endif

int xpon_node_init(void);
int xpon_node_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int xpon_node_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int xpon_node_boot(mxml_node_t *top);

int getXponMode(void);
int setXponMode(int xPONMode);
int epon_sys_set_link_state_cfg(u_char state);
int setXponModeSwitch(int xPONMode, int start);
int getXponLinkSta(mxml_node_t *top, char *attr, char *buffer);
int getTrafficSta(mxml_node_t *top, char *attr, char *buffer);
int getLosSta(char *buffer);
int getPsThreadInfo(char * str);
int xPONStart(mxml_node_t *top);

int checkOnuType(mxml_node_t *top);
int SNModified(mxml_node_t *top);

#endif

#endif
