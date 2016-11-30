/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _CT_SGW_API_H_
#define _CT_SGW_API_H_


/**/
typedef enum {
	SF_EXECENV = 0,
	SF_DEPLOYMENTUNIT,
	SF_EXECUTEUNIT
} sfmodules_type_t;

typedef enum {
	EXENV_ENABLE = 0,
	EXENV_STATUS,
	EXENV_RESET,
	EXENV_NAME,
	EXENV_VENDOR,
	EXENV_VERSION,
	EXENV_PARENTEXECENV
} exenv_type_t;

typedef enum {
	DE_UUID = 0, 
	DE_DUID,
	DE_NAME,
	DE_STATUS,
	DE_RESOLVED,
	DE_URL,
	DE_DESCRIPTION,
	DE_VENDOR,
	DE_VERSION,
	DE_EXECUTIONUNITLIST,
	DE_EXECUTIONENVREF
} deunit_type_t;

typedef enum {
	EXU_EUID = 0,
	EXU_NAME,
	EXU_STATUS,
	EXU_REQUESTEDSTATE,
	EXU_EXECUTIONFAULTCODE,
	EXU_EXECUTIONFAULTMESSAGE,
	EXU_AUTOSTART,
	EXU_VENDOR,
	EXU_VERSION,
	EXU_DESCRIPTION,
	EXU_EXECUTIONENVREF 
} exunit_type_t;

int getSoftModulesPara(int type, char *retbuf);
int getExecEnv(int type, char *retbuf);
int setExecEnvEnable(int setbuf);
int setExecEnv(int type, int setbuf);
int getDeploymentUnit(int type, int num, char *retbuf);
int setDeploymentUnit(int type, int num, char *setbuf);

int getExecutionUnit(int type, int num, char *retbuf);
int setExecutionUnit (int type, int num, char *setbuf);


#endif
