#include "Global_res.h"
#include "cp_lanconfigsecurity.h"

static NodeFunc ConfigPassword=
{
	NULL,
	NULL,
	getConfigPasswordValue,
	setConfigPasswordValue, 
	getConfigPasswordAttribute, 
	setConfigPasswordAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.LanConfigSecurity.
	Parent Node Table Location: cp_topnodetable.c
*/
CwmpParameterStruct LanConfigSecurity[] = 
{
	{"ConfigPassword", StrW, NoLimite, &ConfigPassword, NULL},	
	{NULL, -1, -1, NULL, NULL}
};


