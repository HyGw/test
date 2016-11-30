#include "Global_res.h"
#include "cp_landev_loopbackdetect.h"

static NodeFunc LoopbackEnable=
{
	NULL,
	NULL,
	getLoopbackEnableValue, 
	setLoopbackEnableValue, 
	getLoopbackEnableAttribute, 
	setLoopbackEnableAttribute, 
	NULL,
};

static NodeFunc LoopExistPeriod=
{
	NULL,
	NULL,
	getLoopExistPeriodValue, 
	setLoopExistPeriodValue, 
	getLoopExistPeriodAttribute, 
	setLoopExistPeriodAttribute, 
	NULL,
};

static NodeFunc LoopCancelPeriod=
{
	NULL,
	NULL,
	getLoopCancelPeriodValue, 
	setLoopCancelPeriodValue, 
	getLoopCancelPeriodAttribute, 
	setLoopCancelPeriodAttribute, 
	NULL,
};

static NodeFunc VlanTag=
{
	NULL,
	NULL,
	getVlanTagValue, 
	setVlanTagValue, 
	getVlanTagAttribute, 
	setVlanTagAttribute, 
	NULL,
};

static NodeFunc EthernetType=
{
	NULL,
	NULL,
	getEthernetTypeValue, 
	setEthernetTypeValue, 
	getEthernetTypeAttribute, 
	setEthernetTypeAttribute, 
	NULL,
};

CwmpParameterStruct LANLoopbackDetection[] =
{
	{"LoopbackEnable", BooleanW, NoLimite, &LoopbackEnable, NULL},
	{"LoopExistPeriod", UnsignIntW, NoLimite, &LoopExistPeriod, NULL},
	{"LoopCancelPeriod", UnsignIntW, NoLimite, &LoopCancelPeriod, NULL},
	{"VlanTag", StrW, NoLimite, &VlanTag, NULL},
	{"EthernetType", StrW, NoLimite, &EthernetType, NULL},
	{NULL, -1, -1, NULL, NULL}
};

