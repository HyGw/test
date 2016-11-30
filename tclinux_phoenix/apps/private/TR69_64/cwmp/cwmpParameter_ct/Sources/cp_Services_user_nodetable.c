#include "Global_res.h"
#include "cp_Services_user.h"
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#include "cp_DeviceInfo.h"
#endif


static NodeFunc PortalComputer=
{
	NULL,
	NULL,
	getMaintenancePortalComputerValue, 
	setMaintenancePortalComputerValue, 
	getMaintenancePortalComputerAttribute, 
	setMaintenancePortalComputerAttribute, 
	NULL,
};

static NodeFunc PortalIPTV=
{
	NULL,
	NULL,
	getMaintenancePortalSTBValue, 
	setMaintenancePortalSTBValue, 
	getMaintenancePortalSTBAttribute, 
	setMaintenancePortalSTBAttribute, 
	NULL,
};

static NodeFunc PortalPhone=
{
	NULL,
	NULL,
	getMaintenancePortalPhoneValue, 
	setMaintenancePortalPhoneValue, 
	getMaintenancePortalPhoneAttribute, 
	setMaintenancePortalPhoneAttribute, 
	NULL,
};


static NodeFunc userEnable=
{
	NULL,
	NULL,
	getX_CU_UserEnableValue, 
	setX_CU_UserEnableValue, 
	getX_CU_UserEnableAttribute, 
	setX_CU_UserEnableAttribute, 
	NULL,
};

static NodeFunc userName=
{
	NULL,
	NULL,
	getX_CU_UserNameValue, 
	setX_CU_UserNameValue, 
	getX_CU_UserNameAttribute, 
	setX_CU_UserNameAttribute, 
	NULL,
};

static NodeFunc userDescription=
{
	NULL,
	NULL,
	getX_CU_UserDespValue, 
	setX_CU_UserDespValue, 
	getX_CU_UserDespAttribute, 
	setX_CU_UserDespAttribute, 
	NULL,
};

static NodeFunc userNumberOfSubuser=
{
	NULL,
	NULL,
	getX_CU_UserNum0fSubUserValue, 
	setX_CU_UserNum0fSubUserValue, 
	getX_CU_UserNum0fSubUserAttribute, 
	setX_CU_UserNum0fSubUserAttribute, 
	NULL,
};

static NodeFunc userMACFilterNumberOfEntries=
{
	NULL,
	NULL,
	getX_CU_UserNum0fMACFilterValue, 
	NULL,
	getX_CU_UserNum0fMACFilterAttribute, 
	setX_CU_UserNum0fMACFilterAttribute, 
	NULL,
};

static NodeFunc userIPFilterNumberOfEntries=
{
	NULL,
	NULL,
	getX_CU_UserNum0fIPFilterValue, 
	NULL,
	getX_CU_UserNum0fIPFilterAttribute, 
	setX_CU_UserNum0fIPFilterAttribute, 
	NULL,
};

static NodeFunc userWhiteListNumberofEntries=
{
	NULL,
	NULL,
	getX_CU_UserNum0fWhiteListValue, 
	NULL,
	getX_CU_UserNum0fWhiteListAttribute, 
	setX_CU_UserNum0fWhiteListAttribute, 
	NULL,
};

static NodeFunc userBlackListNumberofEntries=
{
	NULL,
	NULL,
	getX_CU_UserNum0fBlackListValue, 
	NULL,
	getX_CU_UserNum0fBlackListAttribute, 
	setX_CU_UserNum0fBlackListAttribute, 
	NULL,
};

static NodeFunc userURLListFilterMode=
{
	NULL,
	NULL,
	getX_CU_UserURLListFilterModeValue, 
	setX_CU_UserURLListFilterModeValue, 
	getX_CU_UserURLListFilterModeAttribute, 
	setX_CU_UserURLListFilterModeAttribute, 
	NULL,
};

static NodeFunc timeEnable=
{
	NULL,
	NULL,
	getX_CU_TimeEnableValue, 
	setX_CU_TimeEnableValue, 
	getX_CU_TimeEnableAttribute, 
	setX_CU_TimeEnableAttribute, 
	NULL,
};

static NodeFunc timeRules=
{
	NULL,
	NULL,
	getX_CU_TimeRulesValue, 
	setX_CU_TimeRulesValue, 
	getX_CU_TimeRulesAttribute, 
	setX_CU_TimeRulesAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.Services.User.{i}.Time.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct timeObj[] = 
{
	{"Enable", BooleanW, NoLimite, &timeEnable,NULL},
	{"Rules", StrW, NoLimite, &timeRules,NULL},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc MAC=
{
	NULL,
	NULL,
	getX_CU_FilterMacValue, 
	setX_CU_FilterMacValue, 
	getX_CU_FilterMacAttribute, 
	setX_CU_FilterMacAttribute, 
	NULL,
};


CwmpParameterStruct MacFilter[] = 
{
	{"MAC", StrW, NoLimite, &MAC,NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc MacFilterIndex_Funcs=
{
	NULL,
	DeviceNodeDelMacFilterObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

/*Acs is adding Macfilter object*/
CwmpParameterStruct MacFilterIndex[] = 
{
	{NULL,ObjectW, NoLimite, &MacFilterIndex_Funcs, MacFilter},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.User.{i}.MACFilter.
	Parent Node Table Location: cp_Services_nodetable.c
*/
static NodeFunc MacFilterFun=
{
	MacFilterNodeAddObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitMacFilterTable,
};

static NodeFunc Ip=
{
	NULL,
	NULL,
	getX_CU_FilterIPValue, 
	setX_CU_FilterIPValue, 
	getX_CU_FilterIPAttribute, 
	setX_CU_FilterIPAttribute, 
	NULL,
};


CwmpParameterStruct IpFilter[] = 
{
	{"IP", StrW, NoLimite, &Ip, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc IpFilterIndex_Funcs=
{
	NULL,
	DeviceNodeDelIpFilterObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

CwmpParameterStruct IpFilterIndex[] = 
{
	{NULL,ObjectW, NoLimite, &IpFilterIndex_Funcs, IpFilter},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc IpFilterFun=
{
	IpFilterNodeAddObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitIpFilterTable,
};

static NodeFunc WhiteURL=
{
	NULL,
	NULL,
	getX_CU_WhiteURLValue, 
	setX_CU_WhiteURLValue, 
	getX_CU_WhiteURLAttribute, 
	setX_CU_WhiteURLAttribute, 
	NULL,
};

CwmpParameterStruct WhiteList[] = 
{
	{"Url", StrW, NoLimite, &WhiteURL, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc WhiteListIndex_Funcs=
{
	NULL,
	DeviceNodeDelWhiteListObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

CwmpParameterStruct WhiteListIndex[] = 
{
	{NULL,ObjectW, NoLimite, &WhiteListIndex_Funcs, WhiteList},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc WhiteListFun=
{
	WhiteListNodeAddObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitWhiteListTable,
};

static NodeFunc BlackURL=
{
	NULL,
	NULL,
	getX_CU_BlackURLValue, 
	setX_CU_BlackURLValue, 
	getX_CU_BlackURLAttribute, 
	setX_CU_BlackURLAttribute, 
	NULL,
};

CwmpParameterStruct BlackList[] = 
{
	{"Url", StrW, NoLimite, &BlackURL, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc BlackListIndex_Funcs=
{
	NULL,
	DeviceNodeDelBlackListObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

CwmpParameterStruct BlackListIndex[] = 
{
	{NULL,ObjectW, NoLimite, &BlackListIndex_Funcs, BlackList},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc BlackListFun=
{
	BlackListNodeAddObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitBlackListTable,
};

static NodeFunc UsrPortalEnable=
{
	NULL,
	NULL,
	getX_CU_UsrPortalEnableValue, 
	setX_CU_UsrPortalEnableValue, 
	getX_CU_UsrPortalEnableAttribute, 
	setX_CU_UsrPortalEnableAttribute, 
	NULL,
};

static NodeFunc UsrPortalComputer=
{
	NULL,
	NULL,
	getX_CU_UsrPortalComputerValue, 
	setX_CU_UsrPortalComputerValue, 
	getX_CU_UsrPortalComputerAttribute, 
	setX_CU_UsrPortalComputerAttribute, 
	NULL,
};

static NodeFunc UsrPortalIPTV=
{
	NULL,
	NULL,
	getX_CU_UsrPortalIPTVValue, 
	setX_CU_UsrPortalIPTVValue, 
	getX_CU_UsrPortalIPTVAttribute, 
	setX_CU_UsrPortalIPTVAttribute, 
	NULL,
};

static NodeFunc UsrPortalPhone=
{
	NULL,
	NULL,
	getX_CU_UsrPortalPhoneValue, 
	setX_CU_UsrPortalPhoneValue, 
	getX_CU_UsrPortalPhoneAttribute, 
	setX_CU_UsrPortalPhoneAttribute, 
	NULL,
};

CwmpParameterStruct usrPORTAL[] = 
{
	{"Enable", BooleanW, NoLimite, &UsrPortalEnable,NULL},
	{"Computer", StrW, NoLimite, &UsrPortalComputer,NULL},
	{"IPTV", StrW, NoLimite, &UsrPortalIPTV,NULL},
	{"Phone", StrW, NoLimite, &UsrPortalPhone,NULL},	
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.Services.X_CU_User.{i}.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_User[] = 
{
	{"Enable", BooleanW, NoLimite, &userEnable,NULL},
	{"Name", StrW, NoLimite, &userName,NULL},
	{"Description", StrW, NoLimite, &userDescription,NULL},
	{"NumberOfSubuser", UnsignIntW, NoLimite, &userNumberOfSubuser, NULL},
	{"MACFilterNumberOfEntries", UnsignIntR, NoLimite, &userMACFilterNumberOfEntries, NULL},
	{"IPFilterNumberOfEntries", UnsignIntR, NoLimite, &userIPFilterNumberOfEntries, NULL},
	{"WhiteListNumberofEntries", UnsignIntR, NoLimite, &userWhiteListNumberofEntries, NULL},
	{"BlackListNumberofEntries", UnsignIntR, NoLimite, &userBlackListNumberofEntries, NULL},
	{"URLListFilterMode", UnsignIntW, NoLimite, &userURLListFilterMode, NULL},
	{"Time", ObjectR, NoLimite, NULL, timeObj},
	{"MACFilter", ObjectW, NoLimite, &MacFilterFun, NULL},
	{"IPFilter", ObjectW, NoLimite, &IpFilterFun, NULL},
	{"WhiteList", ObjectW, NoLimite, &WhiteListFun, NULL},
	{"BlackList", ObjectW, NoLimite, &BlackListFun, NULL},	
	{"Portal", ObjectR, NoLimite, NULL, usrPORTAL},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc X_CU_UserIndex_Funcs=
{
	NULL,
	DeviceNodeDelXCUUserObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.X_CU_User
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_UserIndex[] = 
{
	{NULL, ObjectW, NoLimite, &X_CU_UserIndex_Funcs, X_CU_User},
	{NULL, -1, -1, NULL, NULL}
};


CwmpParameterStruct usr[] = 
{
	{"Time", ObjectR, NoLimite, NULL, timeObj},
	{"MACFilter", ObjectW, NoLimite, &MacFilterFun, NULL},
	{"IPFilter", ObjectW, NoLimite, &IpFilterFun, NULL},
	{"WhiteList", ObjectW, NoLimite, &WhiteListFun, NULL},
	{"BlackList", ObjectW, NoLimite, &BlackListFun, NULL},	
	{"Portal", ObjectR, NoLimite, NULL, usrPORTAL},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.Services.User
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct usrIndex[] = 
{
	{NULL, ObjectW, NoLimite, &X_CU_UserIndex_Funcs, usr},
	{NULL, -1, -1, NULL, NULL}
};

NodeFunc X_CU_UserFun=
{
	XCUUserNodeAddObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitXCUUserTable,
};

