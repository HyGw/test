#include "Global_res.h"
#include "cp_wandev_wandslInterCfg_stats.h"

static NodeFunc ReceiveBlock_st=
{
	NULL,
	NULL,
	getReceiveBlock_stValue,
	NULL, 
	getReceiveBlock_stAttribute, 
	setReceiveBlock_stAttribute, 
	NULL,
};

static NodeFunc TransmitBlock_st=
{
	NULL,
	NULL,
	getTransmitBlock_stValue,
	NULL, 
	getTransmitBlock_stAttribute, 
	setTransmitBlock_stAttribute, 
	NULL,
};

static NodeFunc CellDelin_st=
{
	NULL,
	NULL,
	getCellDelin_stValue,
	NULL, 
	getCellDelin_stAttribute, 
	setCellDelin_stAttribute, 
	NULL,
};

static NodeFunc LinkRetrain_st=
{
	NULL,
	NULL,
	getLinkRetrain_stValue,
	NULL, 
	getLinkRetrain_stAttribute, 
	setLinkRetrain_stAttribute, 
	NULL,
};

static NodeFunc InitErrors_st=
{
	NULL,
	NULL,
	getInitErrors_stValue,
	NULL, 
	getInitErrors_stAttribute, 
	setInitErrors_stAttribute, 
	NULL,
};

static NodeFunc InitTimeouts_st=
{
	NULL,
	NULL,
	getInitTimeouts_stValue,
	NULL, 
	getInitTimeouts_stAttribute, 
	setInitTimeouts_stAttribute, 
	NULL,
};

static NodeFunc LossOfFraming_st=
{
	NULL,
	NULL,
	getLossOfFraming_stValue,
	NULL, 
	getLossOfFraming_stAttribute, 
	setLossOfFraming_stAttribute, 
	NULL,
};

static NodeFunc ErroredSecs_st=
{
	NULL,
	NULL,
	getErroredSecs_stValue,
	NULL, 
	getErroredSecs_stAttribute, 
	setErroredSecs_stAttribute, 
	NULL,
};

static NodeFunc SeveErrSecs_st=
{
	NULL,
	NULL,
	getSeveErrSecs_stValue,
	NULL, 
	getSeveErrSecs_stAttribute, 
	setSeveErrSecs_stAttribute, 
	NULL,
};

static NodeFunc FecErrors_st=
{
	NULL,
	NULL,
	getFecErrors_stValue,
	NULL, 
	getFecErrors_stAttribute, 
	setFecErrors_stAttribute, 
	NULL,
};

static NodeFunc ATUCFECErrors_st=
{
	NULL,
	NULL,
	getATUCFECErrors_stValue,
	NULL, 
	getATUCFECErrors_stAttribute, 
	setATUCFECErrors_stAttribute, 
	NULL,
};

static NodeFunc HECErrors_st=
{
	NULL,
	NULL,
	getHECErrors_stValue,
	NULL, 
	getHECErrors_stAttribute, 
	setHECErrors_stAttribute, 
	NULL,
};

static NodeFunc CRCErrors_st=
{
	NULL,
	NULL,
	getCRCErrors_stValue,
	NULL, 
	getCRCErrors_stAttribute, 
	setCRCErrors_stAttribute, 
	NULL,
};

static NodeFunc ATUCCRCErrors_st=
{
	NULL,
	NULL,
	getATUCCRCErrors_stValue,
	NULL, 
	getATUCCRCErrors_stAttribute, 
	setATUCCRCErrors_stAttribute, 
	NULL,
};

static NodeFunc ReceiveBlock_total=
{
	NULL,
	NULL,
	getReceiveBlock_totalValue,
	NULL, 
	getReceiveBlock_totalAttribute, 
	setReceiveBlock_totalAttribute, 
	NULL,
};

static NodeFunc TransmitBlock_total=
{
	NULL,
	NULL,
	getTransmitBlock_totalValue,
	NULL, 
	getTransmitBlock_totalAttribute, 
	setTransmitBlock_totalAttribute, 
	NULL,
};

static NodeFunc CellDelin_total=
{
	NULL,
	NULL,
	getCellDelin_totalValue,
	NULL, 
	getCellDelin_totalAttribute, 
	setCellDelin_totalAttribute, 
	NULL,
};

static NodeFunc LinkRetrain_total=
{
	NULL,
	NULL,
	getLinkRetrain_totalValue,
	NULL, 
	getLinkRetrain_totalAttribute, 
	setLinkRetrain_totalAttribute, 
	NULL,
};

static NodeFunc InitErrors_total=
{
	NULL,
	NULL,
	getInitErrors_totalValue,
	NULL, 
	getInitErrors_totalAttribute, 
	setInitErrors_totalAttribute, 
	NULL,
};

static NodeFunc InitTimeouts_total=
{
	NULL,
	NULL,
	getInitTimeouts_totalValue,
	NULL, 
	getInitTimeouts_totalAttribute, 
	setInitTimeouts_totalAttribute, 
	NULL,
};

static NodeFunc LossOfFraming_total=
{
	NULL,
	NULL,
	getLossOfFraming_totalValue,
	NULL, 
	getLossOfFraming_totalAttribute, 
	setLossOfFraming_totalAttribute, 
	NULL,
};

static NodeFunc ErroredSecs_total=
{
	NULL,
	NULL,
	getErroredSecs_totalValue,
	NULL, 
	getErroredSecs_totalAttribute, 
	setErroredSecs_totalAttribute, 
	NULL,
};

static NodeFunc SeveErrSecs_total=
{
	NULL,
	NULL,
	getSeveErrSecs_totalValue,
	NULL, 
	getSeveErrSecs_totalAttribute, 
	setSeveErrSecs_totalAttribute, 
	NULL,
};

static NodeFunc FecErrors_total=
{
	NULL,
	NULL,
	getFecErrors_totalValue,
	NULL, 
	getFecErrors_totalAttribute, 
	setFecErrors_totalAttribute, 
	NULL,
};

static NodeFunc ATUCFECErrors_total=
{
	NULL,
	NULL,
	getATUCFECErrors_totalValue,
	NULL, 
	getATUCFECErrors_totalAttribute, 
	setATUCFECErrors_totalAttribute, 
	NULL,
};

static NodeFunc HECErrors_total=
{
	NULL,
	NULL,
	getHECErrors_totalValue,
	NULL, 
	getHECErrors_totalAttribute, 
	setHECErrors_totalAttribute, 
	NULL,
};

static NodeFunc CRCErrors_total=
{
	NULL,
	NULL,
	getCRCErrors_totalValue,
	NULL, 
	getCRCErrors_totalAttribute, 
	setCRCErrors_totalAttribute, 
	NULL,
};

static NodeFunc ATUCCRCErrors_total=
{
	NULL,
	NULL,
	getATUCCRCErrors_totalValue,
	NULL, 
	getATUCCRCErrors_totalAttribute, 
	setATUCCRCErrors_totalAttribute, 
	NULL,
};

static NodeFunc ReceiveBlock_lst=
{
	NULL,
	NULL,
	getReceiveBlock_lstValue,
	NULL, 
	getReceiveBlock_lstAttribute, 
	setReceiveBlock_lstAttribute, 
	NULL,
};

static NodeFunc TransmitBlock_lst=
{
	NULL,
	NULL,
	getTransmitBlock_lstValue,
	NULL, 
	getTransmitBlock_lstAttribute, 
	setTransmitBlock_lstAttribute, 
	NULL,
};

static NodeFunc CellDelin_lst=
{
	NULL,
	NULL,
	getCellDelin_lstValue,
	NULL, 
	getCellDelin_lstAttribute, 
	setCellDelin_lstAttribute, 
	NULL,
};

static NodeFunc LinkRetrain_lst=
{
	NULL,
	NULL,
	getLinkRetrain_lstValue,
	NULL, 
	getLinkRetrain_lstAttribute, 
	setLinkRetrain_lstAttribute, 
	NULL,
};

static NodeFunc InitErrors_lst=
{
	NULL,
	NULL,
	getInitErrors_lstValue,
	NULL, 
	getInitErrors_lstAttribute, 
	setInitErrors_lstAttribute, 
	NULL,
};

static NodeFunc InitTimeouts_lst=
{
	NULL,
	NULL,
	getInitTimeouts_lstValue,
	NULL, 
	getInitTimeouts_lstAttribute, 
	setInitTimeouts_lstAttribute, 
	NULL,
};

static NodeFunc LossOfFraming_lst=
{
	NULL,
	NULL,
	getLossOfFraming_lstValue,
	NULL, 
	getLossOfFraming_lstAttribute, 
	setLossOfFraming_lstAttribute, 
	NULL,
};

static NodeFunc ErroredSecs_lst=
{
	NULL,
	NULL,
	getErroredSecs_lstValue,
	NULL, 
	getErroredSecs_lstAttribute, 
	setErroredSecs_lstAttribute, 
	NULL,
};

static NodeFunc SeveErrSecs_lst=
{
	NULL,
	NULL,
	getSeveErrSecs_lstValue,
	NULL, 
	getSeveErrSecs_lstAttribute, 
	setSeveErrSecs_lstAttribute, 
	NULL,
};

static NodeFunc FecErrors_lst=
{
	NULL,
	NULL,
	getFecErrors_lstValue,
	NULL, 
	getFecErrors_lstAttribute, 
	setFecErrors_lstAttribute, 
	NULL,
};

static NodeFunc ATUCFECErrors_lst=
{
	NULL,
	NULL,
	getATUCFECErrors_lstValue,
	NULL, 
	getATUCFECErrors_lstAttribute, 
	setATUCFECErrors_lstAttribute, 
	NULL,
};

static NodeFunc HECErrors_lst=
{
	NULL,
	NULL,
	getHECErrors_lstValue,
	NULL, 
	getHECErrors_lstAttribute, 
	setHECErrors_lstAttribute, 
	NULL,
};

static NodeFunc CRCErrors_lst=
{
	NULL,
	NULL,
	getCRCErrors_lstValue,
	NULL, 
	getCRCErrors_lstAttribute, 
	setCRCErrors_lstAttribute, 
	NULL,
};

static NodeFunc ATUCCRCErrors_lst=
{
	NULL,
	NULL,
	getATUCCRCErrors_lstValue,
	NULL, 
	getATUCCRCErrors_lstAttribute, 
	setATUCCRCErrors_lstAttribute, 
	NULL,
};

static NodeFunc ReceiveBlock_cd=
{
	NULL,
	NULL,
	getReceiveBlock_cdValue,
	NULL, 
	getReceiveBlock_cdAttribute, 
	setReceiveBlock_cdAttribute, 
	NULL,
};

static NodeFunc TransmitBlock_cd=
{
	NULL,
	NULL,
	getTransmitBlock_cdValue,
	NULL, 
	getTransmitBlock_cdAttribute, 
	setTransmitBlock_cdAttribute, 
	NULL,
};

static NodeFunc CellDelin_cd=
{
	NULL,
	NULL,
	getCellDelin_cdValue,
	NULL, 
	getCellDelin_cdAttribute, 
	setCellDelin_cdAttribute, 
	NULL,
};

static NodeFunc LinkRetrain_cd=
{
	NULL,
	NULL,
	getLinkRetrain_cdValue,
	NULL, 
	getLinkRetrain_cdAttribute, 
	setLinkRetrain_cdAttribute, 
	NULL,
};

static NodeFunc InitErrors_cd=
{
	NULL,
	NULL,
	getInitErrors_cdValue,
	NULL, 
	getInitErrors_cdAttribute, 
	setInitErrors_cdAttribute, 
	NULL,
};

static NodeFunc InitTimeouts_cd=
{
	NULL,
	NULL,
	getInitTimeouts_cdValue,
	NULL, 
	getInitTimeouts_cdAttribute, 
	setInitTimeouts_cdAttribute, 
	NULL,
};

static NodeFunc LossOfFraming_cd=
{
	NULL,
	NULL,
	getLossOfFraming_cdValue,
	NULL, 
	getLossOfFraming_cdAttribute, 
	setLossOfFraming_cdAttribute, 
	NULL,
};

static NodeFunc ErroredSecs_cd=
{
	NULL,
	NULL,
	getErroredSecs_cdValue,
	NULL, 
	getErroredSecs_cdAttribute, 
	setErroredSecs_cdAttribute, 
	NULL,
};

static NodeFunc SeveErrSecs_cd=
{
	NULL,
	NULL,
	getSeveErrSecs_cdValue,
	NULL, 
	getSeveErrSecs_cdAttribute, 
	setSeveErrSecs_cdAttribute, 
	NULL,
};

static NodeFunc FecErrors_cd=
{
	NULL,
	NULL,
	getFecErrors_cdValue,
	NULL, 
	getFecErrors_cdAttribute, 
	setFecErrors_cdAttribute, 
	NULL,
};

static NodeFunc ATUCFECErrors_cd=
{
	NULL,
	NULL,
	getATUCFECErrors_cdValue,
	NULL, 
	getATUCFECErrors_cdAttribute, 
	setATUCFECErrors_cdAttribute, 
	NULL,
};

static NodeFunc HECErrors_cd=
{
	NULL,
	NULL,
	getHECErrors_cdValue,
	NULL, 
	getHECErrors_cdAttribute, 
	setHECErrors_cdAttribute, 
	NULL,
};

static NodeFunc CRCErrors_cd=
{
	NULL,
	NULL,
	getCRCErrors_cdValue,
	NULL, 
	getCRCErrors_cdAttribute, 
	setCRCErrors_cdAttribute, 
	NULL,
};

static NodeFunc ATUCCRCErrors_cd=
{
	NULL,
	NULL,
	getATUCCRCErrors_cdValue,
	NULL, 
	getATUCCRCErrors_cdAttribute, 
	setATUCCRCErrors_cdAttribute, 
	NULL,
};

static NodeFunc ReceiveBlock_qh=
{
	NULL,
	NULL,
	getReceiveBlock_qhValue,
	NULL, 
	getReceiveBlock_qhAttribute, 
	setReceiveBlock_qhAttribute, 
	NULL,
};

static NodeFunc TransmitBlock_qh=
{
	NULL,
	NULL,
	getTransmitBlock_qhValue,
	NULL, 
	getTransmitBlock_qhAttribute, 
	setTransmitBlock_qhAttribute, 
	NULL,
};

static NodeFunc CellDelin_qh=
{
	NULL,
	NULL,
	getCellDelin_qhValue,
	NULL, 
	getCellDelin_qhAttribute, 
	setCellDelin_qhAttribute, 
	NULL,
};

static NodeFunc LinkRetrain_qh=
{
	NULL,
	NULL,
	getLinkRetrain_qhValue,
	NULL, 
	getLinkRetrain_qhAttribute, 
	setLinkRetrain_qhAttribute, 
	NULL,
};

static NodeFunc InitErrors_qh=
{
	NULL,
	NULL,
	getInitErrors_qhValue,
	NULL, 
	getInitErrors_qhAttribute, 
	setInitErrors_qhAttribute, 
	NULL,
};

static NodeFunc InitTimeouts_qh=
{
	NULL,
	NULL,
	getInitTimeouts_qhValue,
	NULL, 
	getInitTimeouts_qhAttribute, 
	setInitTimeouts_qhAttribute, 
	NULL,
};

static NodeFunc LossOfFraming_qh=
{
	NULL,
	NULL,
	getLossOfFraming_qhValue,
	NULL, 
	getLossOfFraming_qhAttribute, 
	setLossOfFraming_qhAttribute, 
	NULL,
};

static NodeFunc ErroredSecs_qh=
{
	NULL,
	NULL,
	getErroredSecs_qhValue,
	NULL, 
	getErroredSecs_qhAttribute, 
	setErroredSecs_qhAttribute, 
	NULL,
};

static NodeFunc SeveErrSecs_qh=
{
	NULL,
	NULL,
	getSeveErrSecs_qhValue,
	NULL, 
	getSeveErrSecs_qhAttribute, 
	setSeveErrSecs_qhAttribute, 
	NULL,
};

static NodeFunc FecErrors_qh=
{
	NULL,
	NULL,
	getFecErrors_qhValue,
	NULL, 
	getFecErrors_qhAttribute, 
	setFecErrors_qhAttribute, 
	NULL,
};

static NodeFunc ATUCFECErrors_qh=
{
	NULL,
	NULL,
	getATUCFECErrors_qhValue,
	NULL, 
	getATUCFECErrors_qhAttribute, 
	setATUCFECErrors_qhAttribute, 
	NULL,
};

static NodeFunc HECErrors_qh=
{
	NULL,
	NULL,
	getHECErrors_qhValue,
	NULL, 
	getHECErrors_qhAttribute, 
	setHECErrors_qhAttribute, 
	NULL,
};

static NodeFunc CRCErrors_qh=
{
	NULL,
	NULL,
	getCRCErrors_qhValue,
	NULL, 
	getCRCErrors_qhAttribute, 
	setCRCErrors_qhAttribute, 
	NULL,
};

static NodeFunc ATUCCRCErrors_qh=
{
	NULL,
	NULL,
	getATUCCRCErrors_qhValue,
	NULL, 
	getATUCCRCErrors_qhAttribute, 
	setATUCCRCErrors_qhAttribute, 
	NULL,
};

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.Showtime.
	Parent Node Table Location: cp_wandev_wandslInterCfg_stats_nodetable.c
*/
CwmpParameterStruct Showtime[] = 
{
	{"ReceiveBlock", UnsignIntR, NoLimite, &ReceiveBlock_st, NULL},
	{"TransmitBlock", UnsignIntR, NoLimite, &TransmitBlock_st, NULL},
	{"CellDelin", UnsignIntR, NoLimite, &CellDelin_st, NULL},
	{"LinkRetrain", UnsignIntR, NoLimite, &LinkRetrain_st, NULL},
	{"InitErrors", UnsignIntR, NoLimite, &InitErrors_st, NULL},
	{"InitTimeouts", UnsignIntR, NoLimite, &InitTimeouts_st, NULL},
	{"LossOfFraming", UnsignIntR, NoLimite, &LossOfFraming_st, NULL},
	{"ErroredSecs", UnsignIntR, NoLimite, &ErroredSecs_st, NULL},
	{"SeverelyErroredSecs", UnsignIntR, NoLimite, &SeveErrSecs_st, NULL},
	{"FecErrors", UnsignIntR, NoLimite, &FecErrors_st, NULL},
	{"ATUCFECErrors", UnsignIntR, NoLimite, &ATUCFECErrors_st, NULL},
	{"HECErrors", UnsignIntR, NoLimite, &HECErrors_st, NULL},
	{"CRCErrors", UnsignIntR, NoLimite, &CRCErrors_st, NULL},
	{"ATUCCRCErrors", UnsignIntR, NoLimite, &ATUCCRCErrors_st, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.Total.
	Parent Node Table Location: cp_wandev_wandslInterCfg_stats_nodetable.c
*/
CwmpParameterStruct Total[] = 
{
	{"ReceiveBlock", UnsignIntR, NoLimite, &ReceiveBlock_total, NULL},
	{"TransmitBlock", UnsignIntR, NoLimite, &TransmitBlock_total, NULL},
	{"CellDelin", UnsignIntR, NoLimite, &CellDelin_total, NULL},
	{"LinkRetrain", UnsignIntR, NoLimite, &LinkRetrain_total, NULL},
	{"InitErrors", UnsignIntR, NoLimite, &InitErrors_total, NULL},
	{"InitTimeouts", UnsignIntR, NoLimite, &InitTimeouts_total, NULL},
	{"LossOfFraming", UnsignIntR, NoLimite, &LossOfFraming_total, NULL},
	{"ErroredSecs", UnsignIntR, NoLimite, &ErroredSecs_total, NULL},
	{"SeverelyErroredSecs", UnsignIntR, NoLimite, &SeveErrSecs_total, NULL},
	{"FecErrors", UnsignIntR, NoLimite, &FecErrors_total, NULL},
	{"ATUCFECErrors", UnsignIntR, NoLimite, &ATUCFECErrors_total, NULL},
	{"HECErrors", UnsignIntR, NoLimite, &HECErrors_total, NULL},
	{"CRCErrors", UnsignIntR, NoLimite, &CRCErrors_total, NULL},
	{"ATUCCRCErrors", UnsignIntR, NoLimite, &ATUCCRCErrors_total, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.LastShowtime.
	Parent Node Table Location: cp_wandev_wandslInterCfg_stats_nodetable.c
*/
CwmpParameterStruct LastShowtime[] = 
{
	{"ReceiveBlock", UnsignIntR, NoLimite, &ReceiveBlock_lst, NULL},
	{"TransmitBlock", UnsignIntR, NoLimite, &TransmitBlock_lst, NULL},
	{"CellDelin", UnsignIntR, NoLimite, &CellDelin_lst, NULL},
	{"LinkRetrain", UnsignIntR, NoLimite, &LinkRetrain_lst, NULL},
	{"InitErrors", UnsignIntR, NoLimite, &InitErrors_lst, NULL},
	{"InitTimeouts", UnsignIntR, NoLimite, &InitTimeouts_lst, NULL},
	{"LossOfFraming", UnsignIntR, NoLimite, &LossOfFraming_lst, NULL},
	{"ErroredSecs", UnsignIntR, NoLimite, &ErroredSecs_lst, NULL},
	{"SeverelyErroredSecs", UnsignIntR, NoLimite, &SeveErrSecs_lst, NULL},
	{"FecErrors", UnsignIntR, NoLimite, &FecErrors_lst, NULL},
	{"ATUCFECErrors", UnsignIntR, NoLimite, &ATUCFECErrors_lst, NULL},
	{"HECErrors", UnsignIntR, NoLimite, &HECErrors_lst, NULL},
	{"CRCErrors", UnsignIntR, NoLimite, &CRCErrors_lst, NULL},
	{"ATUCCRCErrors", UnsignIntR, NoLimite, &ATUCCRCErrors_lst, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.CurrentDay.
	Parent Node Table Location: cp_wandev_wandslInterCfg_stats_nodetable.c
*/
CwmpParameterStruct CurrentDay[] = 
{
	{"ReceiveBlock", UnsignIntR, NoLimite, &ReceiveBlock_cd, NULL},
	{"TransmitBlock", UnsignIntR, NoLimite, &TransmitBlock_cd, NULL},
	{"CellDelin", UnsignIntR, NoLimite, &CellDelin_cd, NULL},
	{"LinkRetrain", UnsignIntR, NoLimite, &LinkRetrain_cd, NULL},
	{"InitErrors", UnsignIntR, NoLimite, &InitErrors_cd, NULL},
	{"InitTimeouts", UnsignIntR, NoLimite, &InitTimeouts_cd, NULL},
	{"LossOfFraming", UnsignIntR, NoLimite, &LossOfFraming_cd, NULL},
	{"ErroredSecs", UnsignIntR, NoLimite, &ErroredSecs_cd, NULL},
	{"SeverelyErroredSecs", UnsignIntR, NoLimite, &SeveErrSecs_cd, NULL},
	{"FecErrors", UnsignIntR, NoLimite, &FecErrors_cd, NULL},
	{"ATUCFECErrors", UnsignIntR, NoLimite, &ATUCFECErrors_cd, NULL},
	{"HECErrors", UnsignIntR, NoLimite, &HECErrors_cd, NULL},
	{"CRCErrors", UnsignIntR, NoLimite, &CRCErrors_cd, NULL},
	{"ATUCCRCErrors", UnsignIntR, NoLimite, &ATUCCRCErrors_cd, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.Stats.QuarterHour.
	Parent Node Table Location: cp_wandev_wandslInterCfg_stats_nodetable.c
*/
CwmpParameterStruct QuarterHour[] = 
{
	{"ReceiveBlock", UnsignIntR, NoLimite, &ReceiveBlock_qh, NULL},
	{"TransmitBlock", UnsignIntR, NoLimite, &TransmitBlock_qh, NULL},
	{"CellDelin", UnsignIntR, NoLimite, &CellDelin_qh, NULL},
	{"LinkRetrain", UnsignIntR, NoLimite, &LinkRetrain_qh, NULL},
	{"InitErrors", UnsignIntR, NoLimite, &InitErrors_qh, NULL},
	{"InitTimeouts", UnsignIntR, NoLimite, &InitTimeouts_qh, NULL},
	{"LossOfFraming", UnsignIntR, NoLimite, &LossOfFraming_qh, NULL},
	{"ErroredSecs", UnsignIntR, NoLimite, &ErroredSecs_qh, NULL},
	{"SeverelyErroredSecs", UnsignIntR, NoLimite, &SeveErrSecs_qh, NULL},
	{"FecErrors", UnsignIntR, NoLimite, &FecErrors_qh, NULL},
	{"ATUCFECErrors", UnsignIntR, NoLimite, &ATUCFECErrors_qh, NULL},
	{"HECErrors", UnsignIntR, NoLimite, &HECErrors_qh, NULL},
	{"CRCErrors", UnsignIntR, NoLimite, &CRCErrors_qh, NULL},
	{"ATUCCRCErrors", UnsignIntR, NoLimite, &ATUCCRCErrors_qh, NULL},
	{NULL, -1, -1, NULL, NULL}
};
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLCommonInterfaceConfig.
	Parent Node Table Location: cp_wandslInterCfg_nodetable.c
*/
CwmpParameterStruct WandslInterCfg_Stats[] = 
{
	{"Showtime", ObjectR, NoLimite, NULL, Showtime},
	{"Total", ObjectR, NoLimite, NULL, Total},
	{"LastShowtime", ObjectR, NoLimite, NULL, LastShowtime},
	{"CurrentDay", ObjectR, NoLimite, NULL, CurrentDay},
	{"QuarterHour", ObjectR, NoLimite, NULL, QuarterHour},
	{NULL, -1, -1, NULL, NULL}
};

