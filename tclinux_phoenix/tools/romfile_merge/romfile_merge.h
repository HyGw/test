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
#ifndef _ROMFILE_MERGE_H
#define _ROMFILE_MERGE_H
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include "mxml.h"
#define DEF_ROMFILE_ERR_MSG "Default romfile format is wrong.Please to check again!!!\n"
#define OPEN_FAIL_MSG "Open fail: %s %s\n"
#define RENAME_FAIL_MSG "Rename fail: %s\n"
#define VERIFY_ERR_MSG "verify_romfile func: None %s node\n"
#define CREATE_ROMFILE_ERR_MSG "Fail to create the romfile!!!\n"

mxml_node_t* load_romfile(char *file);
int getOptionFromConfigFile(char* opt,char* configFile);
int parseOption(char *nodeName, char *fileOpt, char *fileConfig);
mxml_node_t* merge_romfile_tree (mxml_node_t *tree_main, mxml_node_t *tree_custom,char *fileOpt, char *fileConfig);

#endif
