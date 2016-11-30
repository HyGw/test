#ifndef _CP_DET_EXP_H
#define _CP_DET_EXP_H

/* ATTENTION! before you modify this file */
/* This header file "cp_det_exp.h is located at /voip_module/DSP/MTK/mod-fxs3/src/tone/cp_det/cp_det_exp.h */
#ifndef OSBN_VOIP
#include <stdio.h>
#endif
#include "comm_def.h"
#ifndef OSBN_VOIP
#include "det_def.h"
#endif

typedef enum{
   CP_DET_8K,
   TP_DET_16K
}CP_DET_SR;

typedef enum{
    VTSP_TEMPL_CONTROL_UTD_MASK = 0,
    VTSP_TEMPL_CONTROL_UTD_TONE = 1
}VTSP_TemplateControlUtd;

typedef enum{
    VTSP_TEMPL_UTD_MASK_MOD = 0x0100,
    VTSP_TEMPL_UTD_MASK_CNG = 0x0080,
    VTSP_TEMPL_UTD_MASK_RING = 0x0040,
    VTSP_TEMPL_UTD_MASK_BUSY = 0x0020,
    VTSP_TEMPL_UTD_MASK_REOR = 0x0010,
    VTSP_TEMPL_UTD_MASK_NU = 0x0008,
    VTSP_TEMPL_UTD_MASK_SIT = 0x0004,
    VTSP_TEMPL_UTD_MASK_DIAL = 0x0002,
    VTSP_TEMPL_UTD_MASK_UNK = 0x0001,
}VTSP_TemplateUtdMask;

typedef enum{
    VTSP_TEMPL_UTD_TONE_TYPE_MOD = 1,
    VTSP_TEMPL_UTD_TONE_TYPE_CNG = 2,
    VTSP_TEMPL_UTD_TONE_TYPE_RING = 3,
    VTSP_TEMPL_UTD_TONE_TYPE_BUSY = 4,
    VTSP_TEMPL_UTD_TONE_TYPE_REOR = 5,
    VTSP_TEMPL_UTD_TONE_TYPE_NU = 6,
    VTSP_TEMPL_UTD_TONE_TYPE_SIT = 7,
    VTSP_TEMPL_UTD_TONE_TYPE_DIAL = 8,
    VTSP_TEMPL_UTD_TONE_TYPE_UNK = 9
}VTSP_TemplateUtdToneType;

typedef enum{
    VTSP_TEMPL_UTD_TONE_SINGLE = 0,
    VTSP_TEMPL_UTD_TONE_DUAL = 1,
    VTSP_TEMPL_UTD_TONE_MOD = 2,
}VTSP_TemplateUtdToneControlDual;

typedef enum{
    VTSP_TEMPL_UTD_SIT_US = 10,
    VTSP_TEMPL_UTD_SIT_EURO = 11
}VTSP_TemplateUtdToneControlSIT;

typedef struct{
    VTSP_TemplateControlUtd control;
    VTSP_TemplateUtdMask mask;
    VTSP_TemplateUtdToneType type;
    union{
        struct{
            VTSP_TemplateUtdToneControlDual control;
            int32 cadences;
            int32 freq1;
            int32 freqDev1;
            int32 freq2;
            int32 freqDev2;
            int32 minMake1;
            int32 maxMake1;
            int32 minBreak1;
            int32 maxBreak1;
            int32 minMake2;
            int32 maxMake2;
            int32 minBreak2;
            int32 maxBreak2;
            int32 minMake3;
            int32 maxMake3;
            int32 minBreak3;
            int32 maxBreak3;
            int32 power;
        }dual;
        struct{
            VTSP_TemplateUtdToneControlSIT control;
            int32 freq1;
            int32 freqDev1;
            int32 freq2;
            int32 freqDev2;
            int32 freq3;
            int32 freqDev3;
            int32 freq4;
            int32 freqDev4;
            int32 freq5;
            int32 freqDev5;
            int32 shortMin;
            int32 shortMax;
            int32 longMin;
            int32 longMax;
            int32 power;
        }sit;
    };
}VTSP_UtdTemplate;

typedef enum{
    STATE_MAKE1 = 0x01,
    STATE_BREAK1 = 0x02,
    STATE_MAKE2 = 0x04,
    STATE_BREAK2 = 0x08,
    STATE_MAKE3 = 0x10,
    STATE_BREAK3 = 0x20,
    STATE_MAKE_CONTINUOUS = 0x40
}dualState;

typedef enum{
    STATE_SEG1 = 0x01,
    STATE_SEG2 = 0x02,
    STATE_SEG3 = 0x03
}sitState;

typedef enum{
    SHORTSEG = 0x01,
    LONGSEG = 0x02,
    STARTSEG = 0x03
}sitSegType;

typedef struct{
    union{
        struct{
            int32 maketimes;
            int32 breaktimes;
            int32 acccadences;
            int32 earlycount;
            int32 groups;
            int32 start;
            dualState state;
        }dual;
        struct{
            int32 maketimes;
            int32 dup_size;
            int32 groups;
            sitSegType segtype[3];
            sitState state;
        }sit;
    };
}utdTemplateStatus;

typedef struct{
    VTSP_UtdTemplate *pUtdTemplate;
    utdTemplateStatus *putdTemplateStatus;
    int32 utdTemplateSize;
    int32 detected;
    int32 conStatus;//start = 1, end = 2
}CP_DET_Obj;

typedef CP_DET_Obj *CP_DET_Handle;

void tp_Det_Init(CP_DET_Handle cp_det_hdl, 
             VTSP_UtdTemplate *pUtdTemplate, 
             utdTemplateStatus *putdTemplateStatus, 
             int utdTemplateSize, 
             CP_DET_SR samplingrate,
             int precision);

void cp_Detect(CP_DET_Handle cp_det_hdl, int freq1, int freq2, int freq_interval, int amplitude);

#endif
