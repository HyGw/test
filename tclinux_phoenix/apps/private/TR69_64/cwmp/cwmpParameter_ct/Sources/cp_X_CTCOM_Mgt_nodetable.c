#include "Global_res.h"
#include "cp_X_CTCOM_Mgt.h"

static NodeFunc MgtURL=
{
	NULL,
	NULL,
	getMgtURLValue, 
	setMgtURLValue, 
	getMgtURLAttribute, 
	setMgtURLAttribute, 
	NULL,
};

static NodeFunc Port=
{
	NULL,
	NULL,
	getMgtPortValue, 
	setMgtPortValue, 
	getMgtPortAttribute, 
	setMgtPortAttribute, 
	NULL,
};

static NodeFunc Heartbeat=
{
	NULL,
	NULL,
	getMgtHeartbeatValue, 
	setMgtHeartbeatValue, 
	getMgtHeartbeatAttribute, 
	setMgtHeartbeatAttribute, 
	NULL,
};

static NodeFunc Ability=
{
	NULL,
	NULL,
	getMgtAbilityValue, 
	setMgtAbilityValue, 
	getMgtAbilityAttribute, 
	setMgtAbilityAttribute, 
	NULL,
};

static NodeFunc LocatePort=
{
	NULL,
	NULL,
	getMgtLocatePortValue, 
	setMgtLocatePortValue, 
	getMgtLocatePortAttribute, 
	setMgtLocatePortAttribute, 
	NULL,
};

static NodeFunc Version=
{
	NULL,
	NULL,
	getMgtVersionValue, 
	NULL, 
	getMgtVersionAttribute, 
	setMgtVersionAttribute, 
	NULL,
};

#if defined(TCSUPPORT_CT_JOYME)
static NodeFunc SSN=
{
	NULL,
	NULL,
	getMgtSSNValue, 
	NULL, 
	getMgtSSNAttribute, 
	setMgtSSNAttribute, 
	NULL,
};
#endif

/*
	Parent Node: InternetGatewayDevice.X_CT-COM_Mgt.
	Parent Node Table Location: cp_topnodetable.c 
*/
CwmpParameterStruct X_CTCOM_Mgt[] = 
{
	{"MgtURL", 					StrW, 				NoLimite, 		&MgtURL, 			NULL},
	{"Port", 					UnsignIntW, 		NoLimite, 		&Port,				NULL},
	{"Heartbeat", 				UnsignIntW, 		NoLimite, 		&Heartbeat,			NULL},
	{"Ability", 				UnsignIntW, 		NoLimite, 		&Ability,			NULL},
	{"LocatePort", 				UnsignIntW, 		NoLimite, 		&LocatePort,		NULL},
	{"Version", 				StrR, 				NoLimite, 		&Version,			NULL},
#if defined(TCSUPPORT_CT_JOYME)
	{"SSN", StrR, NoLimite, &SSN, NULL},
#endif
	{NULL, -1, -1, NULL, NULL}
};
