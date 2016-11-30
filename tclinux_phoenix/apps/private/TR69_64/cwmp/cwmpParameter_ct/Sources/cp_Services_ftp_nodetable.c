#include "Global_res.h"
#include "cp_Services_ftp.h"
#include "cp_DeviceInfo.h"

static NodeFunc FTPSvrEnable=
{
	NULL,
	NULL,
	getFtpEnableValue, 
	setFtpEnableValue, 
	getFtpEnableAttribute, 
	setFtpEnableAttribute, 
	NULL,
};

static NodeFunc FTPSvrUserName=
{
	NULL,
	NULL,
	getFtpUserNameValue, 
	setFtpUserNameValue, 
	getFtpUserNameAttribute, 
	setFtpUserNameAttribute, 
	NULL,
};

static NodeFunc FTPSvrPassword=
{
	NULL,
	NULL,
	getFtpPasswordValue, 
	setFtpPasswordValue, 
	getFtpPasswordAttribute, 
	setFtpPasswordAttribute, 
	NULL,
};

static NodeFunc FTPSvrPort=
{
	NULL,
	NULL,
	getFtpPortValue, 
	setFtpPortValue, 
	getFtpPortAttribute, 
	setFtpPortAttribute, 
	NULL,
};

static NodeFunc FTPSvrUserRight=
{
	NULL,
	NULL,
	getX_CU_FTPSvrUserRightValue, 
	setX_CU_FTPSvrUserRightValue, 
	getX_CU_FTPSvrUserRightAttribute, 
	setX_CU_FTPSvrUserRightAttribute, 
	NULL,
};

static NodeFunc FTPSvrPath=
{
	NULL,
	NULL,
	getX_CU_FTPSvrPathValue, 
	setX_CU_FTPSvrPathValue, 
	getX_CU_FTPSvrPathAttribute, 
	setX_CU_FTPSvrPathAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.Services.X_CU_FTPService.FTPServer.{i}.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_FTPServer[] = 
{
	{"Enable", BooleanW, NoLimite, &FTPSvrEnable,NULL},
	{"FtpUserName", StrW, NoLimite, &FTPSvrUserName,NULL},
	{"FtpPassword", StrW, NoLimite, &FTPSvrPassword,NULL},
	{"FtpPort", IntW, NoLimite, &FTPSvrPort, NULL},
	{"FtpUserRight", StrW, NoLimite, &FTPSvrUserRight, NULL},
	{"FtpPath", StrW, NoLimite, &FTPSvrPath, NULL},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.X_CU_FTPService.FTPServer
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_FTPSvrIndex[] = 
{
	{"1", ObjectR, NoLimite, NULL, X_CU_FTPServer},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.Services.X_CU_FTPService.
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_FTPService[] = 
{
	{"FTPServer", ObjectR, NoLimite, NULL, X_CU_FTPSvrIndex},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc FTPCltEnable=
{
	NULL,
	NULL,
	getX_CU_FTPCltEnableValue, 
	setX_CU_FTPCltEnableValue, 
	getX_CU_FTPCltEnableAttribute, 
	setX_CU_FTPCltEnableAttribute, 
	NULL,
};

static NodeFunc FTPCltFtpServerURL=
{
	NULL,
	NULL,
	getX_CU_FTPCltFtpServerURLValue, 
	setX_CU_FTPCltFtpServerURLValue, 
	getX_CU_FTPCltFtpServerURLAttribute, 
	setX_CU_FTPCltFtpServerURLAttribute, 
	NULL,
};

static NodeFunc FTPCltSvrPort=
{
	NULL,
	NULL,
	getX_CU_FTPCltSvrPortValue, 
	setX_CU_FTPCltSvrPortValue, 
	getX_CU_FTPCltSvrPortAttribute, 
	setX_CU_FTPCltSvrPortAttribute, 
	NULL,
};

static NodeFunc FTPCltSvrUserName=
{
	NULL,
	NULL,
	getX_CU_FTPCltSvrUserNameValue, 
	setX_CU_FTPCltSvrUserNameValue, 
	getX_CU_FTPCltSvrUserNameAttribute, 
	setX_CU_FTPCltSvrUserNameAttribute, 
	NULL,
};

static NodeFunc FTPCltSvrPassword=
{
	NULL,
	NULL,
	getX_CU_FTPCltSvrPasswordValue, 
	setX_CU_FTPCltSvrPasswordValue, 
	getX_CU_FTPCltSvrPasswordAttribute, 
	setX_CU_FTPCltSvrPasswordAttribute, 
	NULL,
};

static NodeFunc FTPCltStatus=
{
	NULL,
	NULL,
	getX_CU_FTPCltStatusValue, 
	NULL,
	getX_CU_FTPCltStatusAttribute, 
	setX_CU_FTPCltStatusAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.Services.X_CU_FTPFTPClient.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_FTPClient[] = 
{
	{"Enable", BooleanW, NoLimite, &FTPCltEnable,NULL},
	{"FtpServerURL", StrW, NoLimite, &FTPCltFtpServerURL,NULL},
	{"FtpServerPort", IntW, NoLimite, &FTPCltSvrPort, NULL},
	{"Username", StrW, NoLimite, &FTPCltSvrUserName,NULL},
	{"Password", StrW, NoLimite, &FTPCltSvrPassword,NULL},
	{"Status", StrR, NoLimite, &FTPCltStatus, NULL},
	{NULL, -1, -1, NULL, NULL}
};

static NodeFunc X_CU_FTPClientIndex_Funcs=
{
	NULL,
	DeviceNodeDelFTPClientObject,
	NULL,
	NULL,
	NULL, 
	NULL, 
	NULL
};

/*
	Parent Node: InternetGatewayDevice.Services.X_CU_FTPFTPClient
	Parent Node Table Location: cp_Services_nodetable.c
*/
CwmpParameterStruct X_CU_FTPCltIndex[] = 
{
	{NULL, ObjectW, NoLimite, &X_CU_FTPClientIndex_Funcs, X_CU_FTPClient},
	{NULL, -1, -1, NULL, NULL}
};

/*
	Parent Node: InternetGatewayDevice.Services.X_CU_FTPFTPClient
	Parent Node Table Location: cp_Services_nodetable.c
*/
NodeFunc X_CU_FTPCltFun=
{
	FTPClientNodeAddObject,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cwmpInitFTPClientTable,
};
