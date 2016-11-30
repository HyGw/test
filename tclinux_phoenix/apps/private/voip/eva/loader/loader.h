/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/

/*
 *	File:	loader.h
 *	 
 *	Author: Peter.Jiang
 *	 		
 *	Last Revision Date: 2015.5.9
 *	 
 *	Description:
 *	VoIP drivers loader Constant and function prototypes. 
 *
 */

#ifndef _VOIP_LOADER_H_
#define _VOIP_LOADER_H_

#define INSMOD "/sbin/insmod"
#define RMMOD  "/sbin/rmmod"


#define PROC_SLIC3_CHECK "/proc/spi/check"
#define LOADER_CHECK_FILE "/var/voipslictype"


#define M_SYS_MOD "/lib/modules/sys_mod.ko"
#define M_DSPCORE "/lib/modules/DSPCore.ko"
#define M_PCM1 "/lib/modules/pcm1.ko"
#define M_PCM2 "/lib/modules/pcm2.ko"
#define M_PCM "/lib/modules/pcm.ko"
#define M_LEC_LOADER "/lib/modules/lec.ko"
#define M_SPI "/lib/modules/spi.ko"
#define M_SLIC3 "/lib/modules/slic3.ko"
#define M_FXS3 "/lib/modules/fxs3.ko"
#define M_KSOCKET "/lib/modules/ksocket.ko"
#define M_ORTP "/lib/modules/ortp.ko"
#define M_ACODEC_X "/lib/modules/acodec_x.ko"
#define M_FOIP "/lib/modules/foip.ko"
#define M_OVDSP "/lib/modules/ovdsp.ko"
#define M_PCMDUMP "/lib/modules/pcmDump.ko"

#define M_PCM_SILICON "/lib/modules/pcm_silicon.ko"
#define M_SLIC3_SILICON "/lib/modules/slic3_silicon.ko"
#define M_FXS3_SILICON "/lib/modules/fxs3_silicon.ko"

#define SLIC_TYPE_ZL88601 "zl88601"
#define SLIC_TYPE_LE89156 "le89156"
#define SLIC_TYPE_LE89316 "le89316"
#define SLIC_TYPE_LE89116 "le89116"
#define SLIC_TYPE_LE9641  "le9641"
#define SLIC_TYPE_LE9642  "le9642"
#define SLIC_TYPE_LE9662  "le9662"

#define SLIC_TYPE_SI32176 "si32176"
#define SLIC_TYPE_SI32260 "si32260"
#define SLIC_TYPE_SI32266 "si32266"
#define SLIC_TYPE_SI32183 "si32183"
#define SLIC_TYPE_SI32185 "si32185"
#define SLIC_TYPE_SI32285 "si32285"
#define SLIC_TYPE_SI32287 "si32287"




#define ABNORMAL_CHECK(_c_) if ((_c_)!=0) {printf ("system failed at %d(%d:%s)\n", __LINE__, (_c_),strerror(errno)); }

#define OUT
#define IN
#define INOUT

#define SLIC_TYPE_LEN_MAX 32

#define LOADER_SUCCESS 0
#define LOADER_FAILED  -1

typedef enum 
{
    SLIC_CLASS_ZARLINK,
    SLIC_CLASS_SILICON,
}slic_class_e;

typedef enum 
{
    SPI_TYPE,
    ZSI_TYPE,
    ISI_TYPE,
}interface_class_e;

typedef enum 
{
    LOADER_CHECK_NO,
    LOADER_CHECK_YES,
}loader_check_e;

typedef enum 
{
    SLIC_CHECK_FAILED = 0,
    SLIC_CHECK_SUCCESS = 1,
}slic_check_result_e;


int get_slic_type (OUT char *slic_type, OUT char *slic_type_1, OUT int *pslicNum, OUT int *pinterface_type, OUT int *pslic_class);
int load_zl88601 (IN int slicNum);
int load_le89156 (IN int slicNum);
int load_le89316 (IN int slicNum);
int load_le9641 (IN int slicNum);
int load_le89116 (IN int slicNum);
int load_le9642 (IN int slicNum);
int load_le9662 (IN int slicNum);
int load_le89116_le89316 (IN int slicNum);



int load_voip(IN char *slic_type, IN char* slic_type_1, IN int slicNum);
int check_slic_type(IN char *slic_type);


int load_si32176 (IN int slicNum);
int load_si32260 (IN int slicNum);
int load_si32266 (IN int slicNum);
int load_si32183 (IN int slicNum);
int load_si32185 (IN int slicNum);
int load_si32285 (IN int slicNum);
int load_si32287 (IN int slicNum);



int check_silicon(IN char *slic_type);

#endif /* End of _VOIP_EVCOM_H_ */

