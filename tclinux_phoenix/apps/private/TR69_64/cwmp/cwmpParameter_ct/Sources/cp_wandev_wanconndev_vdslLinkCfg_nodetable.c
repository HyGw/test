#include "Global_res.h"
#include "cp_wandev_wanconndev_vdslLinkCfg.h"


static NodeFunc Enable=
{
	NULL,
	NULL,
	getWANVdslLinkCfg1EnableValue,
	setWANVdslLinkCfg1EnableValue,
	getWANVdslLinkCfg1EnableAttribute,
	setWANVdslLinkCfg1EnableAttribute,
	NULL
};

static NodeFunc Mode=
{
	NULL,
	NULL,
	getWANVdslLinkCfg1ModeValue,
	setWANVdslLinkCfg1ModeValue,
	getWANVdslLinkCfg1ModeAttribute,
	setWANVdslLinkCfg1ModeAttribute,
	NULL
};

static NodeFunc VLANIDMark=
{
	NULL,
	NULL,
	getWANVdslLinkCfg1VLANIDMarkValue,
	setWANVdslLinkCfg1VLANIDMarkValue,
	getWANVdslLinkCfg1VLANIDMarkAttribute,
	setWANVdslLinkCfg1VLANIDMarkAttribute,
	NULL
};

static NodeFunc V8021PMark=
{
	NULL,
	NULL,
	getWANVdslLinkCfg18021PMarkValue,
	setWANVdslLinkCfg18021PMarkValue,
	getWANVdslLinkCfg18021PMarkAttribute,
	setWANVdslLinkCfg18021PMarkAttribute,
	NULL
};

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.X_CT-COM_WANVdslLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
CwmpParameterStruct WANVdslLinkConfig[] =
{
	{"Enable", BooleanW, NoLimite, &Enable, NULL},
	{"Mode", UnsignIntW, NoLimite, &Mode, NULL},
	{"VLANIDMark", UnsignIntW, NoLimite, &VLANIDMark, NULL},
	{"802-1pMark", UnsignIntW, NoLimite, &V8021PMark, NULL},
	{NULL, -1, -1, NULL, NULL}
};

