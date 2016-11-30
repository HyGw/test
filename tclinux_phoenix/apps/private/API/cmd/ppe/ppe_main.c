/***************************************************************
Copyright Statement:

This software/firmware and related documentation (��EcoNet Software��) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (��EcoNet��) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (��ECONET SOFTWARE��) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ��AS IS�� 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER��S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER��S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <asm/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include "hwnat_ioctl.h"

#include "libapi_lib_utility.h"
#include "libapi_lib_ppe.h"
#include "linux/libcompileoption.h"


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
#define CMD_METHOD_ADD          "add"
#define CMD_METHOD_DEL          "del"
#define CMD_METHOD_SHOW         "show"
#define CMD_METHOD_ENABLE       "enable"
#define CMD_METHOD_DISABLE      "disable"
#define CMD_METHOD_CLEAR        "clear"
#define CMD_METHOD_SET          "set"

/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define ARG_VAL_LENGTH  16


#define SPORT_STRING        "sport"
#define MODE_STRING         "mode"
#define MISS_STRING         "miss"
#define VIDS_STRING         "vids"
#define VID_RANGE_STRING    "vid_range"
#define VID0_STRING         "vid0"
#define VID1_STRING         "vid1"
#define VID2_STRING         "vid2"
#define VID3_STRING         "vid3"
#define VID4_STRING         "vid4"

#define GROUP_STRING        "group"
#define RATE_STRING         "rate"
#define SCALE_STRING        "scale"
#define VLAN_STRING         "vlan"
#define DIR_STRING          "dir"
#define QUEUE_STRING        "queue"
#define ALL_STRING          "all"

#define SCALE_PRI_STRING    "scale_pir"
#define SCALE_CRI_STRING    "scale_cir"
#define RATE_PRI_STRING     "rate_pir"
#define RATE_CRI_STRING     "rate_cir"

#define VALID_STRING        "valid"
#define INVALID_STRING      "invalid"
#define LOCKED_STRING       "locked"
#define LINECNT_STRING      "lineCnt"

#define BITSHIFT_STRING     "bitShift"
#define TLS_STRING          "tls"
#define L2B_STRING          "l2b"

#define ENABLE_STRING       "enable"
#define DISABLE_STRING      "disable"
#define DMAC_STRING         "dmac"
#define PBITS_STRING        "pbits"
#define ETYPE_DSCP_STRING   "etype_dscp"
#define SMAC_STRING         "smac"



#define METER_PKT_RATE_MAX  (METER_PKT_RATE_BASE*METER_SCALE_1000)


/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/
struct argVal;
typedef int (*argCheckFunc)(char*, struct argVal*, int); 

struct argVal {
    char *arg;
    char val[ARG_VAL_LENGTH];
    argCheckFunc chkFunc;
    unsigned char hasVal;   /* 0: val is still NULL_STRING, 1: val is assigned a value string */
    unsigned int data;
};

enum ppeCmdEnum {
    PPE_CMD_TLS = 0,
    PPE_CMD_METER,
    PPE_CMD_TRTCM,
    PPE_CMD_ACCOUNT,
    PPE_CMD_CACHE_SET,
};

enum {
    TLS_ARG_SPORT = 0,
    TLS_ARG_MODE,
    TLS_ARG_MISS,
    TLS_ARG_VIDS,
    TLS_ARG_VID_RANGE,
    TLS_ARG_VID0,
    TLS_ARG_VID1,
    TLS_ARG_VID2,
    TLS_ARG_VID3,
    TLS_ARG_VID4,
    TLS_ARG_MAX
};

enum {
    METER_ARG_GROUP = 0,
    METER_ARG_MODE,
    METER_ARG_SCALE,
    METER_ARG_RATE,
    METER_ARG_VLAN,
    METER_ARG_QUEUE,
    METER_ARG_DIR,
    METER_ARG_MAX
};

enum {
    TRTCM_ARG_GROUP = 0,
    TRTCM_ARG_MODE,
    TRTCM_ARG_SCALE_PRI,
    TRTCM_ARG_RATE_PRI,
    TRTCM_ARG_SCALE_CRI,
    TRTCM_ARG_RATE_CRI,
    TRTCM_ARG_VLAN,
    TRTCM_ARG_DIR,
    TRTCM_ARG_MAX
};

enum {
    ACCOUNT_ARG_GROUP = 0,
    ACCOUNT_ARG_VLAN,
    ACCOUNT_ARG_DIR,
    ACCOUNT_ARG_MAX
};

enum {
    CACHE_ARG_MODE = 0,
    CACHE_ARG_MAX
};


/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
static int do_tls(int argc, char *argv[], void *p);
static int do_tls_add(int argc, char *argv[], void *p);
static int do_tls_del(int argc, char *argv[], void *p);
static int do_tls_show(int argc, char *argv[], void *p);
static int do_meter(int argc, char *argv[], void *p);
static int do_meter_add(int argc, char *argv[], void *p);
static int do_meter_del(int argc, char *argv[], void *p);
static int do_meter_show(int argc, char *argv[], void *p);
static int do_trtcm(int argc, char *argv[], void *p);
static int do_trtcm_add(int argc, char *argv[], void *p);
static int do_trtcm_del(int argc, char *argv[], void *p);
static int do_trtcm_show(int argc, char *argv[], void *p);
static int do_account(int argc, char *argv[], void *p);
static int do_account_add(int argc, char *argv[], void *p);
static int do_account_del(int argc, char *argv[], void *p);
static int do_account_show(int argc, char *argv[], void *p);
static int do_cache(int argc, char *argv[], void *p);
static int do_cache_enable(int argc, char *argv[], void *p);
static int do_cache_disable(int argc, char *argv[], void *p);
static int do_cache_set(int argc, char *argv[], void *p);
static int do_cache_set_mode(int argc, char *argv[], void *p);
static int do_cache_clear(int argc, char *argv[], void *p);
static int do_cache_show(int argc, char *argv[], void *p);
static int do_hash(int argc, char *argv[], void *p);
static int do_hash_set(int argc, char *argv[], void *p);
static int do_hash_set_mode(int argc, char *argv[], void *p);
static int do_hash_set_bitShift(int argc, char *argv[], void *p);
static int do_hash_set_tls(int argc, char *argv[], void *p);
static int do_hash_set_l2b(int argc, char *argv[], void *p);


static int isTlsSport(char *sportP, struct argVal argVals[], int idx);
static int isTlsMode(char *modeP, struct argVal argVals[], int idx);
static int isTlsMissAction(char *missP, struct argVal argVals[], int idx);
static int isValidVid(char *vidP, struct argVal argVals[], int idx);

static int isMeterGroupId(char *gidP, struct argVal argVals[], int idx);
static int isMeterMode(char *modeP, struct argVal argVals[], int idx);
static int isMeterScale(char *scaleP, struct argVal argVals[], int idx);
static int isMeterRate(char *rateP, struct argVal argVals[], int idx);
static int is2byteVlan(char *vlanP, struct argVal argVals[], int idx);
static int isMeterDir(char *dirP, struct argVal argVals[], int idx);
static int isMeterQueueId(char *queueId, struct argVal argVals[], int idx);

static int isTrtcmGroupId(char *gidP, struct argVal argVals[], int idx);
static int isPirRate(char *rateP, struct argVal argVals[], int idx);
static int isCirRate(char *rateP, struct argVal argVals[], int idx);


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/
struct ppe_tls_cfg gPpeTlsCfg;
struct ppe_meter_cfg gPpeMeterCfg;
struct ppe_account_cfg gPpeAccountCfg;
struct ppe_cache_cfg gPpeCacheCfg;
struct ppe_hash_cfg gPpeHashCfg;


/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static cmds_t ppeCmds[] = {
    {"tls",     do_tls,     0x10, 0, NULL},
    {"meter",   do_meter,   0x10, 0, NULL},
    {"trtcm",   do_trtcm,   0x10, 0, NULL},
    {"account", do_account, 0x10, 0, NULL},
    {"cache",   do_cache,   0x10, 0, NULL},
    {"hash",    do_hash,    0x10, 0, NULL},
    {NULL,      NULL,       0x10, 0, NULL}
};

static cmds_t tlsCmds[] = {
    {CMD_METHOD_ADD,    do_tls_add, 0x10, 0, NULL},
    {CMD_METHOD_DEL,    do_tls_del, 0x10, 0, NULL},
    {CMD_METHOD_SHOW,   do_tls_show,0x10, 0, NULL},
    {NULL,              NULL,       0x10, 0, NULL}
};

static cmds_t meterCmds[] = {
    {CMD_METHOD_ADD,    do_meter_add,   0x10, 0, NULL},
    {CMD_METHOD_DEL,    do_meter_del,   0x10, 0, NULL},
    {CMD_METHOD_SHOW,   do_meter_show,  0x10, 0, NULL},
    {NULL,              NULL,           0x10, 0, NULL}
};

static cmds_t trtcmCmds[] = {
    {CMD_METHOD_ADD,    do_trtcm_add,   0x10, 0, NULL},
    {CMD_METHOD_DEL,    do_trtcm_del,   0x10, 0, NULL},
    {CMD_METHOD_SHOW,   do_trtcm_show,  0x10, 0, NULL},
    {NULL,              NULL,           0x10, 0, NULL}
};

static cmds_t accountCmds[] = {
    {CMD_METHOD_ADD,    do_account_add,     0x10, 0, NULL},
    {CMD_METHOD_DEL,    do_account_del,     0x10, 0, NULL},
    {CMD_METHOD_SHOW,   do_account_show,    0x10, 0, NULL},
    {NULL,              NULL,               0x10, 0, NULL}
};

static cmds_t cacheCmds[] = {
    {CMD_METHOD_ENABLE,     do_cache_enable,    0x10, 0, NULL},
    {CMD_METHOD_DISABLE,    do_cache_disable,   0x10, 0, NULL},
    {CMD_METHOD_SET,        do_cache_set,       0x10, 0, NULL},
    {CMD_METHOD_CLEAR,      do_cache_clear,     0x10, 0, NULL},
    {CMD_METHOD_SHOW,       do_cache_show,      0x10, 0, NULL},
    {NULL,                  NULL,               0x10, 0, NULL}
};

static cmds_t cacheSetCmds[] = {
    {MODE_STRING,   do_cache_set_mode,  0x10, 0, NULL},
    {NULL,          NULL,               0x10, 0, NULL}
};

static cmds_t hashCmds[] = {
    {CMD_METHOD_SET,        do_hash_set,    0x10, 0, NULL},
    {NULL,                  NULL,           0x10, 0, NULL}
};

static cmds_t hashSetCmds[] = {
    {MODE_STRING,       do_hash_set_mode,       0x10, 0, NULL},
    {BITSHIFT_STRING,   do_hash_set_bitShift,   0x10, 0, NULL},
    {TLS_STRING,        do_hash_set_tls,        0x10, 0, NULL},
    {L2B_STRING,        do_hash_set_l2b,        0x10, 0, NULL},
    {NULL,                  NULL,               0x10, 0, NULL}
};


static struct cmdVal tlsModeArr[TLS_MODE_MAX] =
{
    {TLS_MODE_CMD_UNI, TLS_MODE_SPORT},
    {TLS_MODE_CMD_UUTAG, TLS_MODE_UNTAG},
    {TLS_MODE_CMD_PRI_TAG, TLS_MODE_PRI_TAG},
    {TLS_MODE_CMD_CTAG, TLS_MODE_CTAG},
    {TLS_MODE_CMD_STAG_SIN, TLS_MODE_STAG_SIN},
    {TLS_MODE_CMD_DBL_TAG, TLS_MODE_DBL_TAG},
    {TLS_MODE_CMD_CTAG_NON, TLS_MODE_CTAG_NON},
    {TLS_MODE_CMD_STAG_SPE, TLS_MODE_STAG_SPE}
};

static struct cmdVal tlsSportArr[TLS_SPORT_CMD_MAX] =
{
    {TLS_SPORT_CMD_LAN0, TLS_SP_GSW_P0},
    {TLS_SPORT_CMD_LAN1, TLS_SP_GSW_P1},
    {TLS_SPORT_CMD_LAN2, TLS_SP_GSW_P2},
    {TLS_SPORT_CMD_LAN3, TLS_SP_GSW_P3},
    {TLS_SPORT_CMD_WAN,  TLS_SP_GDMA2}
};

static struct cmdVal cacheStrVals[] =
{
    {VALID_STRING,      PPE_CACHE_TYPE_VALID},
    {INVALID_STRING,    PPE_CACHE_TYPE_INVALID},
    {LOCKED_STRING,     PPE_CACHE_TYPE_LOCKED},
    {ALL_STRING,        PPE_CACHE_TYPE_ALL},
    {LINECNT_STRING,    PPE_CACHE_TYPE_LINECNT},
    {NULL,              0}
};

static struct argVal tlsArgVals[] =
{
    {SPORT_STRING,      NULL_STRING,    isTlsSport,     0,  0},
    {MODE_STRING,       NULL_STRING,    isTlsMode,      0,  0},
    {MISS_STRING,       NULL_STRING,    isTlsMissAction,0,  0},
    {VIDS_STRING,       NULL_STRING,    NULL,           0,  0},
    {VID_RANGE_STRING,  NULL_STRING,    NULL,           0,  0},
    {VID0_STRING,       NULL_STRING,    isValidVid,     0,  0},
    {VID1_STRING,       NULL_STRING,    isValidVid,     0,  0},
    {VID2_STRING,       NULL_STRING,    isValidVid,     0,  0},
    {VID3_STRING,       NULL_STRING,    isValidVid,     0,  0},
    {VID4_STRING,       NULL_STRING,    isValidVid,     0,  0},
    {NULL,              NULL_STRING,    NULL,           0,  0}
};

static struct argVal meterArgVals[] =
{
    {GROUP_STRING,      NULL_STRING,    isMeterGroupId, 0,  0},
    {MODE_STRING,       NULL_STRING,    isMeterMode,    0,  0},
    {SCALE_STRING,      NULL_STRING,    isMeterScale,   0,  0},
    {RATE_STRING,       NULL_STRING,    isMeterRate,    0,  0},
    {VLAN_STRING,       NULL_STRING,    is2byteVlan,    0,  0},
    {QUEUE_STRING,      NULL_STRING,    isMeterQueueId, 0,  0},
    {DIR_STRING,        NULL_STRING,    isMeterDir,     0,  0},
    {NULL,              NULL_STRING,    NULL,           0,  0}
};

static struct argVal trtcmArgVals[] =
{
    {GROUP_STRING,      NULL_STRING,    isTrtcmGroupId, 0,  0},
    {MODE_STRING,       NULL_STRING,    isMeterMode,    0,  0},
    {SCALE_PRI_STRING,  NULL_STRING,    isMeterScale,   0,  0},
    {RATE_PRI_STRING,   NULL_STRING,    isPirRate,      0,  0},
    {SCALE_CRI_STRING,  NULL_STRING,    isMeterScale,   0,  0},
    {RATE_CRI_STRING,   NULL_STRING,    isCirRate,      0,  0},
    {VLAN_STRING,       NULL_STRING,    is2byteVlan,    0,  0},
    {DIR_STRING,        NULL_STRING,    isMeterDir,     0,  0},
    {NULL,              NULL_STRING,    NULL,           0,  0}
};

static struct argVal accountArgVals[] =
{
    {GROUP_STRING,      NULL_STRING,    isMeterGroupId, 0,  0},
    {VLAN_STRING,       NULL_STRING,    is2byteVlan,    0,  0},
    {DIR_STRING,        NULL_STRING,    isMeterDir,     0,  0},
    {NULL,              NULL_STRING,    NULL,           0,  0}
};



/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
static int isNumber(char *vidP)
{
    int i;

    for (i = 0; i < strlen(vidP); i++)
        if (!isdigit(vidP[i]))
            return 0;

    return 1;
}

static int isTlsSport(char *sportP, struct argVal argVals[], int idx)
{
    int i;

    for (i = 0; i < TLS_SPORT_CMD_MAX; i++) {
        if (strncmp(sportP, tlsSportArr[i].cmd, strlen(tlsSportArr[i].cmd)) == 0) {
            argVals[idx].data = tlsSportArr[i].val;
            return 1;
        }
    }

    return 0;
}

static int isTlsMode(char *modeP, struct argVal argVals[], int idx)
{
    int i;

    for (i = 0; i < TLS_MODE_CMD_MAX; i++) {
        if (strncmp(modeP, tlsModeArr[i].cmd, strlen(tlsModeArr[i].cmd)) == 0) {
            argVals[idx].data = tlsModeArr[i].val;
            return 1;
        }
    }

    return 0;
}

static int isTlsMissAction(char *missP, struct argVal argVals[], int idx)
{
    if (strncmp(missP, TLS_MA_CMD_DROP, strlen(TLS_MA_CMD_DROP)) == 0) {
        argVals[idx].data = TLS_MISS_DROP;
        return 1;
    }
    else if (strncmp(missP, TLS_MA_CMD_NONTLS, strlen(TLS_MA_CMD_NONTLS)) == 0) {
        argVals[idx].data = TLS_MISS_NON_TLS;
        return 1;
    }
    else
        return 0;
}

static int isValidVid(char *vidP, struct argVal argVals[], int idx)
{
    unsigned int vid;

    if (!isNumber(vidP))
        return 0;

    vid = atoi(vidP);

    if (0 <= vid && vid < VID_MAX)
        return 1;
    else
        return 0;
}

static int isTlsModeNeedVid(char *modeP)
{
    int i;

    for (i = 0; i < TLS_MODE_MAX; i++)
        if (strncmp(modeP, tlsModeArr[i].cmd, strlen(tlsModeArr[i].cmd)) == 0)
            break;

    if (i >= TLS_MODE_CTAG)
        return 1;
    else
        return 0;
}

static int isTlsArgsHaveVids(void)
{
    int i;

    for (i = TLS_ARG_VIDS; i <= TLS_ARG_VID4; i++)
    {
        if (tlsArgVals[i].hasVal)
            return 1;
    }

    return 0;
}

static int isTlsArgsVidsCheckOK(void)
{
    if (tlsArgVals[TLS_ARG_VID_RANGE].hasVal && tlsArgVals[TLS_ARG_VIDS].hasVal) {
        printf("\nError: \"%s\" and \"%s\" should not both exist\n",
                        tlsArgVals[TLS_ARG_VIDS].arg, tlsArgVals[TLS_ARG_VID_RANGE].arg);
    }
    else if (tlsArgVals[TLS_ARG_VID_RANGE].hasVal && !tlsArgVals[TLS_ARG_VIDS].hasVal)
    {
        if (tlsArgVals[TLS_ARG_VID0].hasVal && tlsArgVals[TLS_ARG_VID1].hasVal && 
            !tlsArgVals[TLS_ARG_VID2].hasVal && !tlsArgVals[TLS_ARG_VID3].hasVal &&
            !tlsArgVals[TLS_ARG_VID4].hasVal)
        {
            return 1;
        }
        else
            printf("\nError: \"%s\" should have %s and %s only\n",tlsArgVals[TLS_ARG_VID_RANGE].arg,
                        tlsArgVals[TLS_ARG_VID0].arg, tlsArgVals[TLS_ARG_VID1].arg);
    }
    else if (!tlsArgVals[TLS_ARG_VID_RANGE].hasVal && tlsArgVals[TLS_ARG_VIDS].hasVal)
    {
        if (tlsArgVals[TLS_ARG_VID0].hasVal)
            return 1;
        else
            printf("\nError: \"%s\" should have value\n", tlsArgVals[TLS_ARG_VID0].arg);
    }
    else
        printf("\nError: \"%s\" and \"%s\" should not both non-exist\n",
                        tlsArgVals[TLS_ARG_VIDS].arg, tlsArgVals[TLS_ARG_VID_RANGE].arg);

    return 0;
}

static int isGroupId(char *gidP, struct argVal argVals[], int idx, int max)
{
    int i, gid;

    if (strncmp(gidP, ALL_STRING, strlen(ALL_STRING)) == 0)
    {
        argVals[idx].data = max;
        return 1;
    }

    for (i = 0; i < strlen(gidP); i++)
        if (!isdigit(gidP[i]))
            return 0;

    gid = atoi(gidP);

    if (0 <= gid && gid < max) {
        argVals[idx].data = gid;
        return 1;
    }
    else
        return 0;
}

static int isMeterGroupId(char *gidP, struct argVal argVals[], int idx)
{
    return isGroupId(gidP, argVals, idx, METER_GROUP_MAX);
}

static int isTrtcmGroupId(char *gidP, struct argVal argVals[], int idx)
{
    return isGroupId(gidP, argVals, idx, TRTCM_GROUP_MAX);
}

static int isMeterMode(char *modeP, struct argVal argVals[], int idx)
{
    if (strncmp(modeP, BYTE_STRING, strlen(BYTE_STRING)) == 0) {
        argVals[idx].data = METER_MODE_BYTE;
        return 1;
    }
    else if (strncmp(modeP, PACKET_STRING, strlen(PACKET_STRING)) == 0) {
        argVals[idx].data = METER_MODE_PACKET;
        return 1;
    }
    else
        return 0;
}

static int isMeterScale(char *scaleP, struct argVal argVals[], int idx)
{
    if (!argVals[METER_ARG_MODE].hasVal || 
        strncmp(argVals[METER_ARG_MODE].val, PACKET_STRING, strlen(PACKET_STRING)) == 0)
    {
        printf("\nERROR: packet mode doesn't need arg \"%s\"\n", SCALE_STRING);
        return 0;
    }

    if (strncmp(scaleP, K_STRING, strlen(K_STRING)) == 0) {
        argVals[idx].data = METER_SCALE_KBPS;
        return 1;
    }
    else if (strncmp(scaleP, M_STRING, strlen(M_STRING)) == 0) {
        argVals[idx].data = METER_SCALE_MBPS;
        return 1;
    }
    else if (strncmp(scaleP, G_STRING, strlen(G_STRING)) == 0) {
        argVals[idx].data = METER_SCALE_GBPS;
        return 1;
    }
    else
        return 0;
}

static int isCorrectRate(char *rateP, struct argVal argVals[], int idx, int scaleEnum)
{
    unsigned int rate, val;

    if (!isNumber(rateP)) {
        printf("\nERROR: arg \"%s\"'s value should a number\n", RATE_STRING);
        return 0;
    }
    else
        rate = atoi(rateP);

    if (!argVals[METER_ARG_MODE].hasVal)
    {
        printf("\nERROR: arg \"%s\" needs arg \"%s\"\n", RATE_STRING, MODE_STRING);
        return 0;
    }

    if (strncmp(argVals[METER_ARG_MODE].val, BYTE_STRING, strlen(BYTE_STRING)) == 0)
    {
        if (!argVals[scaleEnum].hasVal)
        {
            printf("\nERROR: arg \"%s\" needs arg \"%s\" in %s mode\n", RATE_STRING, SCALE_STRING, BYTE_STRING);
            return 0;
        }

        if (strncmp(argVals[scaleEnum].val, K_STRING, strlen(K_STRING)) == 0)
            val = rate * METER_SCALE_K;
        else if (strncmp(argVals[scaleEnum].val, M_STRING, strlen(M_STRING)) == 0)
            val = rate * METER_SCALE_M;
        else /* Gbps */
            val = rate * METER_SCALE_G;

        if (!(val == 0 || (16*METER_SCALE_K <= val && val <= 2*METER_SCALE_G)))
        {
            printf("\nERROR: arg \"%s\"'s value:%d is wrong for %s mode\n", RATE_STRING, rate, BYTE_STRING);
            return 0;
        }
    }
    else /* packet mode */
    {
        if (!(0 <= rate && rate <= METER_PKT_RATE_MAX))
        {
            printf("\nERROR: arg \"%s\"'s value:%d is wrong for %s mode\n", RATE_STRING, rate, PACKET_STRING);
            return 0;
        }
    }

    argVals[idx].data = rate;
    return 1;

}

static int isMeterRate(char *rateP, struct argVal argVals[], int idx)
{
    return isCorrectRate(rateP, argVals, idx, METER_ARG_SCALE);
}

static int isPirRate(char *rateP, struct argVal argVals[], int idx)
{
    return isCorrectRate(rateP, argVals, idx, TRTCM_ARG_SCALE_PRI);
}

static int isCirRate(char *rateP, struct argVal argVals[], int idx)
{
    return isCorrectRate(rateP, argVals, idx, TRTCM_ARG_SCALE_CRI);
}

static int is2byteVlan(char *vlanP, struct argVal argVals[], int idx)
{
    unsigned int vlan;

    vlan = strtoll(vlanP, NULL, 16);

    if (0 <= vlan && vlan <= 0xffff) {
        argVals[idx].data = vlan;
        return 1;
    }
    else
        return 0;
}

static int isMeterQueueId(char *queueId, struct argVal argVals[], int idx)
{
    int i, qId;

    qId = atoi(queueId);

    printf("\nisMeterQueueId qId = %d \n", qId);

    if (0 <= qId && qId <= METER_QUEUE_MAX) {
        argVals[idx].data = qId;
        return 1;
    }
    else
        return 0;

}


static int isMeterDir(char *dirP, struct argVal argVals[], int idx)
{
    if (strncmp(dirP, UP_STRING, strlen(UP_STRING)) == 0) {
        argVals[idx].data = METER_DIR_UP;
        return 1;
    }
    else if (strncmp(dirP, DOWN_STRING, strlen(DOWN_STRING)) == 0) {
        argVals[idx].data = METER_DIR_DOWN;
        return 1;
    }
    else
        return 0;
}


static int isMustMaveNotFound(int idx, char *method, enum ppeCmdEnum cmd)
{
    if (
        (cmd == PPE_CMD_TLS && 
        (((strncmp(method, CMD_METHOD_ADD, strlen(CMD_METHOD_ADD)) == 0) && idx <= TLS_ARG_MISS) ||
         ((strncmp(method, CMD_METHOD_DEL, strlen(CMD_METHOD_DEL)) == 0) && idx == TLS_ARG_SPORT))) ||
        (cmd == PPE_CMD_ACCOUNT && 
         (strncmp(method, CMD_METHOD_ADD, strlen(CMD_METHOD_ADD)) == 0)) ||
        (cmd == PPE_CMD_CACHE_SET && 
         (strncmp(method, CMD_METHOD_SET, strlen(CMD_METHOD_SET)) == 0))
    ){
        return 1;
    }
    else
        return 0;
}

        
static int argsParsing(int argc, char *argv[], struct argVal argVals[], enum ppeCmdEnum cmd)
{
    int i = 0, j;


    while (argVals[i].arg)
    {
        /* initialize */
        strcpy(argVals[i].val, NULL_STRING);
        argVals[i].hasVal = 0;
        
        for (j = 0; j < argc; j++)
        {
            if (strncmp(argVals[i].arg, argv[j], strlen(argVals[i].arg)) == 0)
            {
                if (argVals[i].chkFunc) /* for those args that come with value */
                {
                    if (j < (argc-1)) {
                        if (argVals[i].chkFunc(argv[j+1], argVals, i)) {
                            strcpy(argVals[i].val, argv[j+1]);
                            argVals[i].hasVal = 1;
                        }
                        else {
                            printf("\nError: Wrong value \"%s\" for arg \"%s\"\n", argv[j+1], argVals[i].arg);
                            return -1;
                        }
                    }
                    else {
                        printf("\nError: No value for arg \"%s\"\n", argVals[i].arg);
                        return -1;
                    }
                } 
                else {
                    strcpy(argVals[i].val, YES_STRING);
                    argVals[i].hasVal = 1;
                }
                
                break;
            }
        }

        if (j == argc && isMustMaveNotFound(i, argv[0], cmd))
        {
            printf("\nError: can't find arg \"%s\"\n", argVals[i].arg);
            return -1;
        }
        i++;
    }

    return 0;
}

static int isCheckTlsAddArgsFailed(void)
{
    struct argVal *argVals = tlsArgVals;

    if (!isTlsModeNeedVid(argVals[TLS_ARG_MODE].val) && isTlsArgsHaveVids()) {
        printf("\nError: TLS mode \"%s\" doesn't need vids\n", argVals[TLS_ARG_MODE].val);
        return -1;
    }
    else if (isTlsModeNeedVid(argVals[TLS_ARG_MODE].val) && !isTlsArgsHaveVids()) {
        printf("\nError: TLS mode \"%s\" should have vids\n", argVals[TLS_ARG_MODE].val);
        return -1;
    }
    else if (isTlsModeNeedVid(argVals[TLS_ARG_MODE].val) && !isTlsArgsVidsCheckOK()) {
        return -1;
    }  
    else
        return 0; /* check OK */
}

static int isCheckMeterAddArgsFailed(struct ppe_meter_cfg *meterCfg)
{
    struct argVal *argVals = meterArgVals;

    if (argVals[METER_ARG_GROUP].hasVal==1 && argVals[METER_ARG_MODE].hasVal==1 &&
        argVals[METER_ARG_RATE].hasVal==1 &&
        argVals[METER_ARG_QUEUE].hasVal==1 && argVals[METER_ARG_DIR].hasVal==1) {
        meterCfg->addFlowOnly = 0;
        return 0;
    }
    else if (argVals[METER_ARG_GROUP].hasVal==1 && argVals[METER_ARG_MODE].hasVal==1 &&
        argVals[METER_ARG_RATE].hasVal==1 &&
        argVals[METER_ARG_VLAN].hasVal==1 && argVals[METER_ARG_DIR].hasVal==1) {
        meterCfg->addFlowOnly = 0;
        return 0;
    }
    else if (argVals[METER_ARG_GROUP].hasVal==1 && argVals[METER_ARG_MODE].hasVal==0 &&
        argVals[METER_ARG_SCALE].hasVal==0 && argVals[METER_ARG_RATE].hasVal==0 &&
        argVals[METER_ARG_VLAN].hasVal==1 && argVals[METER_ARG_DIR].hasVal==1) {
        meterCfg->addFlowOnly = 1;
        return 0;
    }
    else {
        printf("\nError: isCheckMeterAddArgsFailed\n");
        return -1; /* check fail */
    }      
}

static int isCheckMeterDelShowArgsFailed(void)
{   
    struct argVal *argVals = meterArgVals;

    if (argVals[METER_ARG_GROUP].hasVal==1 && argVals[METER_ARG_MODE].hasVal==0 &&
        argVals[METER_ARG_SCALE].hasVal==0 && argVals[METER_ARG_RATE].hasVal==0 &&
        argVals[METER_ARG_VLAN].hasVal==0 && argVals[METER_ARG_DIR].hasVal==0)
        return 0;
    else if (argVals[METER_ARG_GROUP].hasVal==0 && argVals[METER_ARG_MODE].hasVal==0 &&
        argVals[METER_ARG_SCALE].hasVal==0 && argVals[METER_ARG_RATE].hasVal==0 &&
        argVals[METER_ARG_VLAN].hasVal==1 && argVals[METER_ARG_DIR].hasVal==1)
        return 0;
    else {
        printf("\nError: only need arg \"%s\" or args \"%s and %s\"\n",
                    GROUP_STRING, VLAN_STRING, DIR_STRING);
        return -1; /* check fail */
    }      
}

static int isCheckTrtcmAddArgsFailed(struct ppe_meter_cfg *meterCfg)
{
    struct argVal *argVals = trtcmArgVals;

    if (argVals[TRTCM_ARG_GROUP].hasVal==1 && argVals[TRTCM_ARG_MODE].hasVal==1 &&
        argVals[TRTCM_ARG_RATE_PRI].hasVal==1 &&
        argVals[TRTCM_ARG_RATE_CRI].hasVal==1 &&
        argVals[TRTCM_ARG_VLAN].hasVal==1 && argVals[TRTCM_ARG_DIR].hasVal==1) {
        meterCfg->addFlowOnly = 0;
        return 0;
    }
    else if (argVals[TRTCM_ARG_GROUP].hasVal==1 && argVals[TRTCM_ARG_MODE].hasVal==0 &&
        argVals[TRTCM_ARG_SCALE_PRI].hasVal==0 && argVals[TRTCM_ARG_SCALE_PRI].hasVal==0 &&
        argVals[TRTCM_ARG_SCALE_CRI].hasVal==0 && argVals[TRTCM_ARG_RATE_CRI].hasVal==0 &&
        argVals[TRTCM_ARG_VLAN].hasVal==1 && argVals[TRTCM_ARG_DIR].hasVal==1) {
        meterCfg->addFlowOnly = 1;
        return 0;
    }
    else {
        printf("\nError: isCheckTrtcmAddArgsFailed\n");
        return -1; /* check fail */
    }      
}

static int isCheckTrtcmDelShowArgsFailed(void)
{
    struct argVal *argVals = trtcmArgVals;

    if (argVals[TRTCM_ARG_GROUP].hasVal==1 && argVals[TRTCM_ARG_MODE].hasVal==0 &&
        argVals[TRTCM_ARG_SCALE_PRI].hasVal==0 && argVals[TRTCM_ARG_RATE_PRI].hasVal==0 &&
        argVals[TRTCM_ARG_SCALE_CRI].hasVal==0 && argVals[TRTCM_ARG_RATE_CRI].hasVal==0 &&
        argVals[TRTCM_ARG_VLAN].hasVal==0 && argVals[TRTCM_ARG_DIR].hasVal==0)
        return 0;
    else if (argVals[TRTCM_ARG_GROUP].hasVal==0 && argVals[TRTCM_ARG_MODE].hasVal==0 &&
        argVals[TRTCM_ARG_SCALE_PRI].hasVal==0 && argVals[TRTCM_ARG_SCALE_PRI].hasVal==0 &&
        argVals[TRTCM_ARG_SCALE_CRI].hasVal==0 && argVals[TRTCM_ARG_RATE_CRI].hasVal==0 &&
        argVals[TRTCM_ARG_VLAN].hasVal==1 && argVals[TRTCM_ARG_DIR].hasVal==1)
        return 0;
    else {
        printf("\nError: only need arg \"%s\" or args \"%s and %s\"\n",
                    GROUP_STRING, VLAN_STRING, DIR_STRING);
        return -1; /* check fail */
    }      
}

static int isCheckAccountDelShowArgsFailed(void)
{
    struct argVal *argVals = accountArgVals;    

    if (argVals[ACCOUNT_ARG_GROUP].hasVal==1 &&
        argVals[ACCOUNT_ARG_VLAN].hasVal==0 && argVals[ACCOUNT_ARG_DIR].hasVal==0)
        return 0;
    else if (argVals[ACCOUNT_ARG_GROUP].hasVal==0 &&
        argVals[ACCOUNT_ARG_VLAN].hasVal==1 && argVals[ACCOUNT_ARG_DIR].hasVal==1)
        return 0;
    else {
        printf("\nError: only need arg \"%s\" or args \"%s and %s\"\n",
                    GROUP_STRING, VLAN_STRING, DIR_STRING);
        return -1; /* check fail */
    }      
}


static int do_tls(int argc, char *argv[], void *p)
{
    return subcmd(tlsCmds, argc, argv, NULL);
}

static int do_tls_add(int argc, char *argv[], void *p)
{
    struct ppe_tls_cfg *tlsCfg = &gPpeTlsCfg;
    struct argVal *argVals = tlsArgVals;
    unsigned int i, idx = 0;

    if (argsParsing(argc, argv, argVals, PPE_CMD_TLS))
        goto tls_add_args_check_fail;

    if (isCheckTlsAddArgsFailed())
        goto tls_add_args_check_fail;

    memset(tlsCfg, 0, sizeof(struct ppe_tls_cfg));


    tlsCfg->method = PPE_METHOD_ADD;
    tlsCfg->uni = argVals[TLS_ARG_SPORT].data;
    tlsCfg->mode = argVals[TLS_ARG_MODE].data;
    tlsCfg->ma = argVals[TLS_ARG_MISS].data;
    
    if (tlsCfg->mode == TLS_MODE_CTAG_NON)
        tlsCfg->cm = TLS_CONCATENATE_AND;
    else
        tlsCfg->cm = TLS_CONCATENATE_OR;


    if (argVals[TLS_ARG_VID_RANGE].hasVal)
    {
        tlsCfg->tagEn = 1;
        tlsCfg->rng = 1;
        tlsCfg->tags = argc - 10;
        tlsCfg->vids[0] = atoi(argVals[TLS_ARG_VID0].val);
        tlsCfg->vids[1] = atoi(argVals[TLS_ARG_VID1].val);                
        tlsCfg->tags = 2;
    }
    else if (argVals[TLS_ARG_VIDS].hasVal)
    {
        tlsCfg->tagEn = 1;

        for (i = TLS_ARG_VID0; i <= TLS_ARG_VID4; i++) {
            if (argVals[i].hasVal)
            {
                /* user should fill vid0 ~ vid4 by sequence. 
                 * if vid1 is NULL, vid2 ~ vid4 will be treated as NULL also */
                tlsCfg->vids[idx++] = atoi(argVals[i].val);
                tlsCfg->tags++;
            }
            else
                break;
        }
    }


    tlsCfg->en = 1;

    if (do_ppe_ioctl(tlsCfg, PPE_COMMAND_TLS))
        return -1;

    return 0;


tls_add_args_check_fail:
printf("\nUsage:\n" 
        "ppe tls add\n\t"
        "sport (%s/%s/%s/%s/%s)\n\t"
        "mode (%s/%s/%s/%s/%s/%s/%s/%s)\n\t"
        "miss (%s/%s)\n\t"
        "vids/vid_range\n\t"
        "vid0 (0~4095) vid1 (0~4095) vid2 (0~4095) vid3 (0~4095) vid4 (0~4095)\n\n",
        TLS_SPORT_CMD_LAN0, TLS_SPORT_CMD_LAN1, TLS_SPORT_CMD_LAN2, TLS_SPORT_CMD_LAN3, TLS_SPORT_CMD_WAN,
        TLS_MODE_CMD_UNI, TLS_MODE_CMD_UUTAG, TLS_MODE_CMD_PRI_TAG, TLS_MODE_CMD_CTAG, TLS_MODE_CMD_STAG_SIN, TLS_MODE_CMD_DBL_TAG, TLS_MODE_CMD_CTAG_NON, TLS_MODE_CMD_STAG_SPE,
        TLS_MA_CMD_DROP, TLS_MA_CMD_NONTLS);
    
    return -1;
}

static int do_tls_del(int argc, char *argv[], void *p)
{
    struct ppe_tls_cfg *tlsCfg = &gPpeTlsCfg;
    struct argVal *argVals = tlsArgVals;
    

    if (argsParsing(argc, argv, argVals, PPE_CMD_TLS))
        goto tls_del_args_check_fail;

    memset(tlsCfg, 0, sizeof(struct ppe_tls_cfg));

    tlsCfg->method = PPE_METHOD_DEL;
    tlsCfg->uni = argVals[TLS_ARG_SPORT].data;
    tlsCfg->en = 0;

    if (do_ppe_ioctl(tlsCfg, PPE_COMMAND_TLS))
        return -1;

    return 0;

tls_del_args_check_fail:
    printf("\nUsage:\n" 
            "ppe tls del\n\t"
            "sport (%s/%s/%s/%s/%s)\n\n", 
            TLS_SPORT_CMD_LAN0, TLS_SPORT_CMD_LAN1, TLS_SPORT_CMD_LAN2, TLS_SPORT_CMD_LAN3, TLS_SPORT_CMD_WAN);
    return -1; 
}

static int do_tls_show(int argc, char *argv[], void *p)
{
    struct ppe_tls_cfg *tlsCfg = &gPpeTlsCfg;

    tlsCfg->method = PPE_METHOD_SHOW;

    if (do_ppe_ioctl(tlsCfg, PPE_COMMAND_TLS))
        return -1;

    return 0;
}


static int do_meter(int argc, char *argv[], void *p)
{
    return subcmd(meterCmds, argc, argv, NULL);
}

static int do_meter_add(int argc, char *argv[], void *p)
{
    struct ppe_meter_cfg *meterCfg = &gPpeMeterCfg;
    struct argVal *argVals = meterArgVals;
	unsigned int meterGroup=0;
	unsigned int rate = 0;

	if(argc != 3)
	{
    if (argsParsing(argc, argv, argVals, PPE_CMD_METER))
        goto meter_add_args_check_fail;

    memset(meterCfg, 0, sizeof(struct ppe_meter_cfg));

    if (isCheckMeterAddArgsFailed(meterCfg))
        goto meter_add_args_check_fail;


    meterCfg->method = PPE_METHOD_ADD;
    meterCfg->group = argVals[METER_ARG_GROUP].data;
    meterCfg->mode = argVals[METER_ARG_MODE].data;
    meterCfg->scale = argVals[METER_ARG_SCALE].data;
    meterCfg->rate = argVals[METER_ARG_RATE].data;
    meterCfg->vlan = argVals[METER_ARG_VLAN].data;
    meterCfg->queue = argVals[METER_ARG_QUEUE].data;
    meterCfg->dir = argVals[METER_ARG_DIR].data;
    meterCfg->enable = 1;
    

    if (do_ppe_ioctl(meterCfg, PPE_COMMAND_METER))
        return -1;
	}
	else
	{
		meterGroup = strtoul(argv[1],NULL,10);
		rate = strtoul(argv[2],NULL,10);	
		
		if(ppemgr_lib_set_meter_ratelimit(meterGroup, rate))
			return -1;
	}
   

    return 0;


meter_add_args_check_fail:
printf("\nUsage:\n" 
        "ppe meter add\n\t"
        "group (0~62)\n\t"
        "mode (byte/packet)\n\t"
        "[scale (K/M/G)] for byte mode only. K/M/G means 1000/1024K/1024M\n\t"
        "rate (16K~2G) bps for byte mode / (1~2047000) fps for packet mode\n\t"
        "vlan (0~ffff)\n\t"
        "queue (0~8)\n\t"
        "dir (up/down)\n\n");
    
    return -1;
}

static int do_meter_del(int argc, char *argv[], void *p)
{
    struct ppe_meter_cfg *meterCfg = &gPpeMeterCfg;
    struct argVal *argVals = meterArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_METER))
        goto meter_del_args_check_fail;

    if (isCheckMeterDelShowArgsFailed())
         goto meter_del_args_check_fail;


    memset(meterCfg, 0, sizeof(struct ppe_meter_cfg));


    meterCfg->method = PPE_METHOD_DEL;
    if (argVals[METER_ARG_GROUP].hasVal) {
        meterCfg->group = argVals[METER_ARG_GROUP].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_GROUP;
    }
    else {
        meterCfg->vlan = argVals[METER_ARG_VLAN].data;
        meterCfg->dir = argVals[METER_ARG_DIR].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_FLOW;
    }


    if (do_ppe_ioctl(meterCfg, PPE_COMMAND_METER))
        return -1;
   

    return 0;


meter_del_args_check_fail:
printf("\nUsage:\n" 
        "ppe meter del\n\t"
        "group (0~62/all)\n"
        "or\n"
        "ppe meter del\n\t"
        "vlan (0~ffff) dir (up/down)\n\n");

    return -1;
}

static int do_meter_show(int argc, char *argv[], void *p)
{
    struct ppe_meter_cfg *meterCfg = &gPpeMeterCfg;
    struct argVal *argVals = meterArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_METER))
        goto meter_show_args_check_fail;

    if (isCheckMeterDelShowArgsFailed())
         goto meter_show_args_check_fail;


    memset(meterCfg, 0, sizeof(struct ppe_meter_cfg));


    meterCfg->method = PPE_METHOD_SHOW;
    if (argVals[METER_ARG_GROUP].hasVal) {
        meterCfg->group = argVals[METER_ARG_GROUP].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_GROUP;
    }
    else {
        meterCfg->vlan = argVals[METER_ARG_VLAN].data;
        meterCfg->dir = argVals[METER_ARG_DIR].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_FLOW;
    }


    if (do_ppe_ioctl(meterCfg, PPE_COMMAND_METER))
        return -1;
   

    return 0;


meter_show_args_check_fail:
printf("\nUsage:\n" 
        "ppe meter show\n\t"
        "group (0~62/all)\n"
        "or\n"
        "ppe meter show\n\t"
        "vlan (0~ffff) dir (up/down)\n\n");
    
    return -1;
}

static int do_trtcm_add(int argc, char *argv[], void *p)
{
    struct ppe_meter_cfg *meterCfg = &gPpeMeterCfg;
    struct argVal *argVals = trtcmArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_TRTCM))
        goto trtcm_add_args_check_fail;

    memset(meterCfg, 0, sizeof(struct ppe_meter_cfg));

    if (isCheckTrtcmAddArgsFailed(meterCfg))
        goto trtcm_add_args_check_fail;



    meterCfg->method = PPE_METHOD_ADD;
    meterCfg->trtcm = 1;
    meterCfg->group = argVals[TRTCM_ARG_GROUP].data;
    meterCfg->mode = argVals[TRTCM_ARG_MODE].data;
    meterCfg->scale= argVals[TRTCM_ARG_SCALE_PRI].data;
    meterCfg->scale2 = argVals[TRTCM_ARG_SCALE_CRI].data;
    meterCfg->rate = argVals[TRTCM_ARG_RATE_PRI].data;
    meterCfg->rate2 = argVals[TRTCM_ARG_RATE_CRI].data;
    meterCfg->vlan = argVals[TRTCM_ARG_VLAN].data;
    meterCfg->dir = argVals[TRTCM_ARG_DIR].data;
    meterCfg->enable = 1;


    if (do_ppe_ioctl(meterCfg, PPE_COMMAND_TRTCM))
        return -1;
   

    return 0;


trtcm_add_args_check_fail:
printf("\nUsage:\n" 
        "ppe trtcm add\n\t"
        "group (0~30)\n\t"
        "mode (byte/packet)\n\t"
        "[scale_pir (K/M/G)] for byte mode only. K/M/G means 1000/1024K/1024M\n\t"
        "rate_pir (16K~2G) bps for byte mode / (1~2047000) fps for packet mode\n\t"
        "[scale_cir (K/M/G)] for byte mode only. K/M/G means 1000/1024K/1024M\n\t"
        "rate_cir (16K~2G) bps for byte mode / (1~2047000) fps for packet mode\n\t"
        "vlan (0~ffff)\n\t"
        "dir (up/down)\n\n");
    
    return -1;
}

static int do_trtcm_del(int argc, char *argv[], void *p)
{
    struct ppe_meter_cfg *meterCfg = &gPpeMeterCfg;
    struct argVal *argVals = trtcmArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_TRTCM))
        goto trtcm_del_args_check_fail;

    if (isCheckTrtcmDelShowArgsFailed())
         goto trtcm_del_args_check_fail;


    memset(meterCfg, 0, sizeof(struct ppe_meter_cfg));


    meterCfg->method = PPE_METHOD_DEL;
    if (argVals[TRTCM_ARG_GROUP].hasVal) {
        meterCfg->group = argVals[TRTCM_ARG_GROUP].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_GROUP;
    }
    else {
        meterCfg->vlan = argVals[TRTCM_ARG_VLAN].data;
        meterCfg->dir = argVals[TRTCM_ARG_DIR].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_FLOW;
    }


    if (do_ppe_ioctl(meterCfg, PPE_COMMAND_TRTCM))
        return -1;
   

    return 0;


trtcm_del_args_check_fail:
printf("\nUsage:\n" 
        "ppe trtcm del\n\t"
        "group (0~30/all)\n"
        "or\n"
        "ppe meter del\n\t"
        "vlan (0~ffff) dir (up/down)\n\n");

    return -1;
}


static int do_trtcm_show(int argc, char *argv[], void *p)
{
    struct ppe_meter_cfg *meterCfg = &gPpeMeterCfg;
    struct argVal *argVals = trtcmArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_TRTCM))
        goto trtcm_show_args_check_fail;

    if (isCheckTrtcmDelShowArgsFailed())
         goto trtcm_show_args_check_fail;


    memset(meterCfg, 0, sizeof(struct ppe_meter_cfg));


    meterCfg->method = PPE_METHOD_SHOW;
    if (argVals[TRTCM_ARG_GROUP].hasVal) {
        meterCfg->group = argVals[TRTCM_ARG_GROUP].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_GROUP;
    }
    else {
        meterCfg->vlan = argVals[TRTCM_ARG_VLAN].data;
        meterCfg->dir = argVals[TRTCM_ARG_DIR].data;
        meterCfg->grpOrFlow = METER_HANDLE_BY_FLOW;
    }

    if (do_ppe_ioctl(meterCfg, PPE_COMMAND_TRTCM))
        return -1;
   

    return 0;


trtcm_show_args_check_fail:
printf("\nUsage:\n" 
        "ppe trtcm show\n\t"
        "group (0~30/all)\n"
        "or\n"
        "ppe trtcm show\n\t"
        "vlan (0~ffff) dir (up/down)\n\n");
    
    return -1;
}


static int do_trtcm(int argc, char *argv[], void *p)
{
    return subcmd(trtcmCmds, argc, argv, NULL);
}

static int do_account_add(int argc, char *argv[], void *p)
{
    struct ppe_account_cfg *accountCfg = &gPpeAccountCfg;
    struct argVal *argVals = accountArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_ACCOUNT))
        goto account_add_args_check_fail;

     memset(accountCfg, 0, sizeof(struct ppe_account_cfg));


    accountCfg->method = PPE_METHOD_ADD;
    accountCfg->group = argVals[ACCOUNT_ARG_GROUP].data;
    accountCfg->vlan = argVals[ACCOUNT_ARG_VLAN].data;
    accountCfg->dir = argVals[ACCOUNT_ARG_DIR].data;


    if (do_ppe_ioctl(accountCfg, PPE_COMMAND_ACCOUNT))
        return -1;
   

    return 0;


account_add_args_check_fail:
printf("\nUsage:\n" 
        "ppe account add\n\t"
        "group (0~62)\n\t"
        "vlan (0~ffff)\n\t"
        "dir (up/down)\n\n");
    
    return -1;
}

static int do_account_del(int argc, char *argv[], void *p)
{
    struct ppe_account_cfg *accountCfg = &gPpeAccountCfg;
    struct argVal *argVals = accountArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_ACCOUNT))
        goto account_del_args_check_fail;

    if (isCheckAccountDelShowArgsFailed())
         goto account_del_args_check_fail;


    memset(accountCfg, 0, sizeof(struct ppe_account_cfg));


    accountCfg->method = PPE_METHOD_DEL;
    if (argVals[ACCOUNT_ARG_GROUP].hasVal) {
        accountCfg->group = argVals[ACCOUNT_ARG_GROUP].data;
        accountCfg->grpOrFlow = METER_HANDLE_BY_GROUP;
    }
    else {
        accountCfg->vlan = argVals[ACCOUNT_ARG_VLAN].data;
        accountCfg->dir = argVals[ACCOUNT_ARG_DIR].data;
        accountCfg->grpOrFlow = METER_HANDLE_BY_FLOW;
    }


    if (do_ppe_ioctl(accountCfg, PPE_COMMAND_ACCOUNT))
        return -1;
   

    return 0;


account_del_args_check_fail:
printf("\nUsage:\n" 
        "ppe account del\n\t"
        "group (0~62/all)\n"
        "or\n"
        "ppe account del\n\t"
        "vlan (0~ffff) dir (up/down)\n\n");

    return -1;
}

static int do_account_show(int argc, char *argv[], void *p)
{
    struct ppe_account_cfg *accountCfg = &gPpeAccountCfg;
    struct argVal *argVals = accountArgVals;


    if (argsParsing(argc, argv, argVals, PPE_CMD_ACCOUNT))
        goto account_show_args_check_fail;

    if (isCheckAccountDelShowArgsFailed())
         goto account_show_args_check_fail;


    memset(accountCfg, 0, sizeof(struct ppe_account_cfg));


    accountCfg->method = PPE_METHOD_SHOW;
    if (argVals[ACCOUNT_ARG_GROUP].hasVal) {
        accountCfg->group = argVals[ACCOUNT_ARG_GROUP].data;
        accountCfg->grpOrFlow = METER_HANDLE_BY_GROUP;
    }
    else {
        accountCfg->vlan = argVals[ACCOUNT_ARG_VLAN].data;
        accountCfg->dir = argVals[ACCOUNT_ARG_DIR].data;
        accountCfg->grpOrFlow = METER_HANDLE_BY_FLOW;
    }


    if (do_ppe_ioctl(accountCfg, PPE_COMMAND_ACCOUNT))
        return -1;
   

    return 0;


account_show_args_check_fail:
printf("\nUsage:\n" 
        "ppe account show\n\t"
        "group (0~62/all)\n"
        "or\n"
        "ppe account show\n\t"
        "vlan (0~ffff) dir (up/down)\n\n");
    
    return -1;
}


static int do_account(int argc, char *argv[], void *p)
{
    return subcmd(accountCmds, argc, argv, NULL);
}

static int do_cache_enable(int argc, char *argv[], void *p)
{
    struct ppe_cache_cfg *cacheCfg = &gPpeCacheCfg;

    memset(cacheCfg, 0, sizeof(struct ppe_cache_cfg));

    cacheCfg->method = CACHE_METHOD_ENABLE;

    if (do_ppe_ioctl(cacheCfg, PPE_COMMAND_CACHE))
        return -1;

    return 0;
}

static int do_cache_disable(int argc, char *argv[], void *p)
{
    struct ppe_cache_cfg *cacheCfg = &gPpeCacheCfg;

    memset(cacheCfg, 0, sizeof(struct ppe_cache_cfg));

    cacheCfg->method = CACHE_METHOD_DISABLE;

    if (do_ppe_ioctl(cacheCfg, PPE_COMMAND_CACHE))
        return -1;

    return 0;
}

static int do_cache_set_mode(int argc, char *argv[], void *p)
{
    struct ppe_cache_cfg *cacheCfg = &gPpeCacheCfg;
    int mode;

    if (argc != 2)
        goto cache_set_args_check_fail;

    memset(cacheCfg, 0, sizeof(struct ppe_cache_cfg));

    if (isNumber(argv[1])) {
        mode = atoi(argv[1]);
        if (0 <= mode && mode < CACHE_MODE_MAX) {
            cacheCfg->method = CACHE_METHOD_SET_MODE;
            cacheCfg->mode = mode;
        }
        else {
            printf("\nERROR: wrong cache mode:%d\n", mode);
            goto cache_set_args_check_fail;
        }
    }
    else {
        printf("\nERROR: hash cache:\"%s\" is not a number\n", argv[1]);
        goto cache_set_args_check_fail;
    }

    if (do_ppe_ioctl(cacheCfg, PPE_COMMAND_CACHE))
        return -1;

    return 0;


cache_set_args_check_fail:
    printf("\nUsage:\n" 
            "ppe cache set mode (0~3)\n\n");
    
    return -1;
}


static int do_cache_set(int argc, char *argv[], void *p)
{
    return subcmd(cacheSetCmds, argc, argv, NULL);
}


static int do_cache_clear(int argc, char *argv[], void *p)
{
    struct ppe_cache_cfg *cacheCfg = &gPpeCacheCfg;

    if (argc != 2)
        goto cache_clear_args_check_fail;

    memset(cacheCfg, 0, sizeof(struct ppe_cache_cfg));

    cacheCfg->method = CACHE_METHOD_CLEAR;

    if(strncmp(argv[1], ALL_STRING, strlen(ALL_STRING)) == 0) {
        cacheCfg->foeIdx = 0;
    }
    else if (isNumber(argv[1])) {
        cacheCfg->foeIdx = atoi(argv[1]);
    }
    else {
        goto cache_clear_args_check_fail;
    }

    
    if (do_ppe_ioctl(cacheCfg, PPE_COMMAND_CACHE))
        return -1;

    return 0;

cache_clear_args_check_fail:
    printf("\nUsage:\n" 
            "ppe cache clear (foeIdx/all)\n\n");
    
    return -1;
}

static int do_cache_show(int argc, char *argv[], void *p)
{
    struct ppe_cache_cfg *cacheCfg = &gPpeCacheCfg;
    int i = 0;

    if (argc != 2)
        goto cache_show_args_check_fail;

    memset(cacheCfg, 0, sizeof(struct ppe_cache_cfg));

    cacheCfg->method = CACHE_METHOD_SHOW;

    while (cacheStrVals[i].cmd) {
        if(strncmp(argv[1], cacheStrVals[i].cmd, strlen(cacheStrVals[i].cmd)) == 0) {
            cacheCfg->showType = cacheStrVals[i].val;
            break;
        }
        i++;
    }

    if (i == PPE_CACHE_TYPE_NUM) {
        printf("\nERROR: wrong cache type: \"%s\"\n", argv[1]);
        goto cache_show_args_check_fail;
    }
    
    if (do_ppe_ioctl(cacheCfg, PPE_COMMAND_CACHE))
        return -1;

    return 0;

cache_show_args_check_fail:
    printf("\nUsage:\n" 
            "ppe cache show (valid/invalid/locked/all/lineCnt)\n\n");
    
    return -1;
}


static int do_cache(int argc, char *argv[], void *p)
{
    return subcmd(cacheCmds, argc, argv, NULL);
}

static int do_hash_set_mode(int argc, char *argv[], void *p)
{
    struct ppe_hash_cfg *hashCfg = &gPpeHashCfg;
    int mode;

    if (argc != 2)
        goto hash_set_mode_args_check_fail;

    memset(hashCfg, 0, sizeof(struct ppe_hash_cfg));

    if (isNumber(argv[1])) {
        mode = atoi(argv[1]);
        if (0 <= mode && mode < HASH_MODE_MAX) {
            hashCfg->method = HASH_METHOD_SET_MODE;
            hashCfg->mode = mode;
        }
        else {
            printf("\nERROR: wrong hash mode:%d\n", mode);
            goto hash_set_mode_args_check_fail;
        }
    }
    else {
        printf("\nERROR: hash mode:\"%s\" is not a number\n", argv[1]);
        goto hash_set_mode_args_check_fail;
    }

    if (do_ppe_ioctl(hashCfg, PPE_COMMAND_HASH))
        return -1;

    return 0;


hash_set_mode_args_check_fail:
    printf("\nUsage:\n" 
            "ppe hash set mode (0~3)\n\n");
    
    return -1;
}

static int do_hash_set_bitShift(int argc, char *argv[], void *p)
{
    struct ppe_hash_cfg *hashCfg = &gPpeHashCfg;
    int shift;

    if (argc != 2)
        goto hash_set_bitShift_args_check_fail;

    memset(hashCfg, 0, sizeof(struct ppe_hash_cfg));

    if (isNumber(argv[1])) {
        shift = atoi(argv[1]);
        if (0 <= shift && shift < HASH_BITSHIFT_MAX) {
            hashCfg->method = HASH_METHOD_SET_BITSHIFT;
            hashCfg->bitShift = shift;
        }
        else {
            printf("\nERROR: wrong hash bit shift:%d\n", shift);
            goto hash_set_bitShift_args_check_fail;
        }
    }
    else {
        printf("\nERROR: hash bitShift:\"%s\" is not a number\n", argv[1]);
        goto hash_set_bitShift_args_check_fail;
    }

    if (do_ppe_ioctl(hashCfg, PPE_COMMAND_HASH))
        return -1;

    return 0;


hash_set_bitShift_args_check_fail:
    printf("\nUsage:\n" 
            "ppe hash set bitShift (0~15)\n\n");
    
    return -1;
}

static int do_hash_set_tls(int argc, char *argv[], void *p)
{
    struct ppe_hash_cfg *hashCfg = &gPpeHashCfg;

    if (argc != 3)
        goto hash_set_tls_args_check_fail;

    memset(hashCfg, 0, sizeof(struct ppe_hash_cfg));

    hashCfg->method = HASH_METHOD_SET_TLS;

    if (strncmp(argv[1], ENABLE_STRING, strlen(ENABLE_STRING)) == 0) {
        if (strncmp(argv[2], DMAC_STRING, strlen(DMAC_STRING)) == 0)
            hashCfg->tlsDmacDis = 2;
        else if (strncmp(argv[2], PBITS_STRING, strlen(PBITS_STRING)) == 0)
            hashCfg->tlsPbitsDis = 2;
        else if (strncmp(argv[2], ETYPE_DSCP_STRING, strlen(ETYPE_DSCP_STRING)) == 0)
            hashCfg->tlsEtypeDis = 2;
        else {
            printf("\nERROR: wrong hash tls type:%s\n", argv[2]);
            goto hash_set_tls_args_check_fail;
        }
    }
    else if (strncmp(argv[1], DISABLE_STRING, strlen(DISABLE_STRING)) == 0) {
        if (strncmp(argv[2], DMAC_STRING, strlen(DMAC_STRING)) == 0)
            hashCfg->tlsDmacDis = 3;
        else if (strncmp(argv[2], PBITS_STRING, strlen(PBITS_STRING)) == 0)
            hashCfg->tlsPbitsDis = 3;
        else if (strncmp(argv[2], ETYPE_DSCP_STRING, strlen(ETYPE_DSCP_STRING)) == 0)
            hashCfg->tlsEtypeDis = 3;
        else {
            printf("\nERROR: wrong hash tls type:%s\n", argv[2]);
            goto hash_set_tls_args_check_fail;
        }
    }
    else {
        goto hash_set_tls_args_check_fail;
    }


    if (do_ppe_ioctl(hashCfg, PPE_COMMAND_HASH))
        return -1;

    return 0;


hash_set_tls_args_check_fail:
    printf("\nUsage:\n" 
            "ppe hash set tls enable/disable (dmac/pbits/etype_dscp)\n\n");
    
    return -1;
}

static int do_hash_set_l2b(int argc, char *argv[], void *p)
{
    struct ppe_hash_cfg *hashCfg = &gPpeHashCfg;

    if (argc != 3)
        goto hash_set_l2b_args_check_fail;

    memset(hashCfg, 0, sizeof(struct ppe_hash_cfg));

    hashCfg->method = HASH_METHOD_SET_L2B;

    if (strncmp(argv[1], SMAC_STRING, strlen(SMAC_STRING)) == 0) {
        if (strncmp(argv[2], ENABLE_STRING, strlen(ENABLE_STRING)) == 0)
            hashCfg->l2bSmacEn = 1;
        else if (strncmp(argv[2], DISABLE_STRING, strlen(DISABLE_STRING)) == 0)
            hashCfg->l2bSmacEn = 0;
        else {
            printf("\nERROR: wrong hash l2b smac \"%s\"\n", argv[2]);
            goto hash_set_l2b_args_check_fail;
        }
    }
    else {
        printf("\nERROR: wrong hash l2b \"%s\"\n", argv[1]);
        goto hash_set_l2b_args_check_fail;
    }


    if (do_ppe_ioctl(hashCfg, PPE_COMMAND_HASH))
        return -1;

    return 0;


hash_set_l2b_args_check_fail:
    printf("\nUsage:\n" 
            "ppe hash set l2b smac (enable/disable)\n\n");
    
    return -1;
}


static int do_hash_set(int argc, char *argv[], void *p)
{
    return subcmd(hashSetCmds, argc, argv, NULL);
}


static int do_hash(int argc, char *argv[], void *p)
{
    return subcmd(hashCmds, argc, argv, NULL);
}


int main(int argc, char **argv) 
{
    init_compileoption_val();

	return subcmd(ppeCmds, argc, argv, NULL);
}


