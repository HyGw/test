/******************************************************************************
Filename:    pmegaco_support.h
Description: define the head file  by dare-tech communication with megaco
author:      frank
create date: 2013/9/5 20:12:10
modified date:
*******************************************************************************
                Copyright (c) 2013 dare-tech
*******************************************************************************/
#ifndef PMEGACO_SUPPORT_H
#define PMEGACO_SUPPORT_H

#if defined(__cplusplus)
extern "C" {
#endif


int pmegaco_cfg_InitMsg();

int pmegaco_cfg_Notice2megaco(void);

int voip_GetVoiceWanAddr(char *pStrVoiceWanAddr);

#if defined(__cplusplus)
}
#endif


#endif
