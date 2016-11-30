#include "Global_res.h"
#include "cp_Services.h"
#if defined(TCSUPPORT_TRUE_LANDING_PAGE) || defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
#include "cp_DeviceInfo.h"
#endif

#if VOIP
static NodeFunc VoiceProfileNmberOfEntries=
{
	NULL,
	NULL,
	getVoiceProfileNmberOfEntriesValue, 
	NULL, 
	getVoiceProfileNmberOfEntriesAttribute, 
	setVoiceProfileNmberOfEntriesAttribute, 
	NULL
};

static NodeFunc VoiceProfileFuncs=
{
	cwmpVoiceProfileAddObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitVoiceProfileSubTree
};

static NodeFunc VoiceProfile_D =
{
	NULL,
	cwmpVoiceProfileDelObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct VoiceProfileIndex[] = 
{
	{NULL, ObjectW, NoLimite, &VoiceProfile_D, VoiceProfile},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc PhyInterfaceFuncs=
{
	cwmpPhyInterfaceAddObject,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitPhyInterfaceSubTree
};

static NodeFunc PhyInterface_D =
{
	NULL,
	cwmpPhyInterfaceDelObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.PhyInterface.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct PhyInterfaceIndex[] = 
{
	{NULL, ObjectW, NoLimite, &PhyInterface_D, PhyInterface},
	{NULL, -1, -1, NULL, NULL}
};


/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct VoiceService[] = 
{
	{"VoiceProfileNumberOfEntries", UnsignIntR, NoLimite, &VoiceProfileNmberOfEntries, NULL},
	{"Capabilities", ObjectR, NoLimite, NULL, Capabilities},
	{"VoiceProfile", ObjectW, NoLimite, &VoiceProfileFuncs, NULL},
	{"PhyInterface", ObjectW, NoLimite, &PhyInterfaceFuncs,NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct VoiceServiceIndex[] = 
{
	{NULL, ObjectR, NoLimite, NULL, VoiceService},
	{NULL, -1, -1, NULL, NULL}
};


static NodeFunc VoiceServiceFuncs=
{
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL, 
	NULL, 
	cwmpInitVoiceServiceSubTree
};
#endif

#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
static NodeFunc Mode_Services=
{
	NULL,
	NULL,
	getModeServicesValue, 
	setModeServicesValue, 
	getModeServicesAttribute, 
	setModeServicesAttribute, 
	NULL,
};

static NodeFunc TotalTerminalNumber_Services=
{
	NULL,
	NULL,
	getTotalTerminalNumberServicesValue, 
	setTotalTerminalNumberServicesValue, 
	getTotalTerminalNumberServicesAttribute, 
	setTotalTerminalNumberServicesAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CMCC)
static NodeFunc TermialType_Services=
{
	NULL,
	NULL,
	getTerminalTypeServicesValue, 
	setTerminalTypeServicesValue, 
	getTerminalTypeServicesAttribute, 
	setTerminalTypeServicesAttribute, 
	NULL,
};
#endif

#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
static NodeFunc STBRestrictEnable_Services=
{
	NULL,
	NULL,
	getSTBRestrictEnableServicesValue, 
	setSTBRestrictEnableServicesValue, 
	getSTBRestrictEnableServicesAttribute, 
	setSTBRestrictEnableServicesAttribute, 
	NULL,
};
static NodeFunc STBNumber_Services=
{
	NULL,
	NULL,
	getSTBNumberServicesValue, 
	setSTBNumberServicesValue, 
	getSTBNumberServicesAttribute, 
	setSTBNumberServicesAttribute, 
	NULL,
};
static NodeFunc CameraRestrictEnable_Services=
{
	NULL,
	NULL,
	getCameraRestrictEnableServicesValue, 
	setCameraRestrictEnableServicesValue, 
	getCameraRestrictEnableServicesAttribute, 
	setCameraRestrictEnableServicesAttribute, 
	NULL,
};
static NodeFunc CameraNumber_Services=
{
	NULL,
	NULL,
	getCameraNumberServicesValue, 
	setCameraNumberServicesValue, 
	getCameraNumberServicesAttribute, 
	setCameraNumberServicesAttribute, 
	NULL,
};
static NodeFunc ComputerRestrictEnable_Services=
{
	NULL,
	NULL,
	getComputerRestrictEnableServicesValue, 
	setComputerRestrictEnableServicesValue, 
	getComputerRestrictEnableServicesAttribute, 
	setComputerRestrictEnableServicesAttribute, 
	NULL,
};
static NodeFunc ComputerNumber_Services=
{
	NULL,
	NULL,
	getComputerNumberServicesValue, 
	setComputerNumberServicesValue, 
	getComputerNumberServicesAttribute, 
	setComputerNumberServicesAttribute, 
	NULL,
};
static NodeFunc PhoneRestrictEnable_Services=
{
	NULL,
	NULL,
	getPhoneRestrictEnableServicesValue, 
	setPhoneRestrictEnableServicesValue, 
	getPhoneRestrictEnableServicesAttribute, 
	setPhoneRestrictEnableServicesAttribute, 
	NULL,
};
static NodeFunc PhoneNumber_Services=
{
	NULL,
	NULL,
	getPhoneNumberServicesValue, 
	setPhoneNumberServicesValue, 
	getPhoneNumberServicesAttribute, 
	setPhoneNumberServicesAttribute, 
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.DeviceInfo.X_CT-COM_MWBAND.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CTCOM_MWBAND[] =
{
	{"Mode", IntW, NoLimite, &Mode_Services, NULL},
	{"TotalTerminalNumber", IntW, NoLimite, &TotalTerminalNumber_Services, NULL},		
#if defined(TCSUPPORT_CMCC)
	{"TerminalType", UnsignIntW, NoLimite, &TermialType_Services, NULL},
#endif
#if defined(TCSUPPORT_CT_CWMP_E8BPARAMETER)
	{"STBRestrictEnable", BooleanW, NoLimite, &STBRestrictEnable_Services, NULL},
	{"STBNumber", IntW, NoLimite, &STBNumber_Services, NULL},
	{"CameraRestrictEnable", BooleanW, NoLimite, &CameraRestrictEnable_Services, NULL},
	{"CameraNumber", IntW, NoLimite, &CameraNumber_Services, NULL},
	{"ComputerRestrictEnable", BooleanW, NoLimite, &ComputerRestrictEnable_Services, NULL},
	{"ComputerNumber", IntW, NoLimite, &ComputerNumber_Services, NULL},
	{"PhoneRestrictEnable", BooleanW, NoLimite, &PhoneRestrictEnable_Services, NULL},
	{"PhoneNumber", IntW, NoLimite, &PhoneNumber_Services, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};
#endif
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_IPTV_NODE)
static NodeFunc IGMPEnable=
{
	NULL,
	NULL,
	getIGMPEnableValue, 
	setIGMPEnableValue, 
	getIGMPEnableAttribute, 
	setIGMPEnableAttribute, 
	NULL
};

static NodeFunc ProxyEnable=
{
	NULL,
	NULL,
	getProxyEnableValue, 
	setProxyEnableValue, 
	getProxyEnableAttribute, 
	setProxyEnableAttribute, 
	NULL
};

static NodeFunc SnoopingEnable=
{
	NULL,
	NULL,
	getSnoopingEnableValue, 
	setSnoopingEnableValue, 
	getSnoopingEnableAttribute, 
	setSnoopingEnableAttribute, 
	NULL
};

static NodeFunc STBNumber=
{
	NULL,
	NULL,
	getSTBNumberValue, 
	NULL, 
	getSTBNumberAttribute, 
	setSTBNumberAttribute, 
	NULL
};

#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
static NodeFunc STBMAC=
{
	NULL,
	NULL,
	getSTBMACValue, 
	NULL, 
	getSTBMACAttribute, 
	setSTBMACAttribute, 
	NULL
};
#endif

CwmpParameterStruct IPTV[] = 
{
	{"IGMPEnable", BooleanW, NoLimite, &IGMPEnable, NULL},
	{"ProxyEnable", BooleanW, NoLimite, &ProxyEnable, NULL},
	{"SnoopingEnable", BooleanW, NoLimite, &SnoopingEnable, NULL},
	{"STBNumber", UnsignIntR, NoLimite, &STBNumber, NULL},
#if defined(TCSUPPORT_CT_PON_JS) || defined(TCSUPPORT_CT_STBMAC_REPORT)
	{"STBMAC", StrR, NoLimite, &STBMAC, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};
#endif
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
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

static NodeFunc UserNumberOfEntries=
{
	NULL,
	NULL,
	getUserNumberOfEntriesValue, 
	NULL, 
	getUserNumberOfEntriesAttribute, 
	setUserNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc FTPServerNumberOfEntries=
{
	NULL,
	NULL,
	getFTPServerNumberOfEntriesValue, 
	NULL, 
	getFTPServerNumberOfEntriesAttribute, 
	setFTPServerNumberOfEntriesAttribute, 
	NULL,
};

static NodeFunc FTPClientNumberOfEntries=
{
	NULL,
	NULL,
	getFTPClientNumberOfEntriesValue, 
	NULL, 
	getFTPClientNumberOfEntriesAttribute, 
	setFTPClientNumberOfEntriesAttribute, 
	NULL,
};
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE) || defined(TCSUPPORT_CUC_CWMP_PARAMETER)
static NodeFunc PortalEnable_Service=
{
	NULL,
	NULL,
	getMaintenancePortalEnableValue, 
	setMaintenancePortalEnableValue, 
	getMaintenancePortalEnableAttribute, 
	setMaintenancePortalEnableAttribute, 
	NULL,
};
CwmpParameterStruct PORTAL[] = 
{
	{"Enable", BooleanW, NoLimite, &PortalEnable_Service,NULL},
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{"Computer", StrW, NoLimite, &PortalComputer,NULL},
	{"IPTV", StrW, NoLimite, &PortalIPTV,NULL},
	{"Phone", StrW, NoLimite, &PortalPhone,NULL},	
#endif
	{NULL, -1, -1, NULL, NULL}
};
CwmpParameterStruct User[] = 
{
	{"Portal", ObjectR, NoLimite, NULL, PORTAL},
	{NULL, -1, -1, NULL, NULL}
};
CwmpParameterStruct UserIndex[] = 
{
	{"1", ObjectR, NoLimite, NULL, User},
	{NULL, -1, -1, NULL, NULL}
};
#endif
/*
	Parent Node: InternetGatewayDevice.Services.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct Services[] = 
{
#if VOIP
	{"VoiceService", ObjectR, NoLimite, &VoiceServiceFuncs, NULL},
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if defined(TCSUPPORT_CT_ACCESSLIMIT)
	{X_CT_COM"MWBAND", ObjectR, NoLimite,  NULL,X_CTCOM_MWBAND},
#endif
#endif
#if !defined(TCSUPPORT_C1_CUC)
#if !defined(TCSUPPORT_CUC_CWMP_PARAMETER)
#if defined(TCSUPPORT_CT_IPTV_NODE)
	{X_CT_COM"IPTV", ObjectR, NoLimite,NULL, IPTV},
#endif
#endif
#endif
#if defined(TCSUPPORT_CUC_CWMP_PARAMETER)
	{X_CU"UserNumberOfEntries", UnsignIntR, NoLimite, &UserNumberOfEntries, NULL},
	{X_CU"FTPServerNumberOfEntries", UnsignIntR, NoLimite, &FTPServerNumberOfEntries, NULL},
	{X_CU"FTPClientNumberOfEntries", UnsignIntR, NoLimite, &FTPClientNumberOfEntries, NULL},
	{X_CU"User", ObjectW, NoLimite,  &X_CU_UserFun, NULL},
	{X_CU"FTPService", ObjectR, NoLimite,  NULL, X_CU_FTPService},
	{X_CU"FTPFTPClient", ObjectW, NoLimite,  &X_CU_FTPCltFun, NULL},
	{"User", ObjectW, NoLimite,  &X_CU_UserFun, NULL},
#endif
#if defined(TCSUPPORT_TRUE_LANDING_PAGE)
	{"User",ObjectR,NoLimite,NULL,UserIndex},
#endif
	{NULL, -1, -1, NULL, NULL}
};

