/***************************************************************
Copyright Statement:

This software/firmware and related documentation (EcoNet Software) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (EcoNet) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (ECONET SOFTWARE) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN AS IS 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVERS SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVERS SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "libapi_lib_ppe.h"

#include "linux/libcompileoption.h"

#include "modules/ra_hwnat/hwnat_ioctl.h"

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
************************************************************************/

/************************************************************************
*                  M A C R O S
************************************************************************/

/************************************************************************
*                  D A T A	 T Y P E S
************************************************************************/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
************************************************************************/

/************************************************************************
*                  P U B L I C   D A T A
************************************************************************/

/************************************************************************
*                  P R I V A T E   D A T A
************************************************************************/

/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
************************************************************************/

/******************************************************************************
 Description:	It's used to do ioctl to /dev/hwnat0 device.
 
 Input Args:	command: ioctl command.
                opt: pointer to a ppe cmd related structure.
				
 Ret Value:		success: HWNAT_SUCCESS
                fail: HWNAT_FAIL
******************************************************************************/
int do_ppe_ioctl(void *opt, unsigned long command)
{
    int fd, retVal = 1;
    struct ppe_tls_cfg *tlsCfg = opt;
    struct ppe_meter_cfg *meterCfg = opt;
    struct ppe_account_cfg *accountCfg = opt;
    struct ppe_cache_cfg *cacheCfg = opt;
    struct ppe_hash_cfg *hashCfg = opt;
    

    fd = open("/dev/"HW_NAT_DEVNAME, O_RDONLY);
    if (fd < 0)
    {
    	printf("Open %s pseudo device failed\n","/dev/"HW_NAT_DEVNAME);
    	return HWNAT_FAIL;
    }

    if(ioctl(fd, command, opt)<0) {
    	printf("PPE_API: ioctl error\n");
    	close(fd);
    	return HWNAT_FAIL;
    }

    close(fd);

    if (command == PPE_COMMAND_TLS)
        retVal = tlsCfg->retVal;
    else if (command == PPE_COMMAND_METER || command == PPE_COMMAND_TRTCM)
        retVal = meterCfg->retVal;
    else if (command == PPE_COMMAND_ACCOUNT)
        retVal = accountCfg->retVal;
    else if (command == PPE_COMMAND_CACHE)
        retVal = cacheCfg->retVal;
    else if (command == PPE_COMMAND_HASH)
        retVal = hashCfg->retVal;
    else 
        retVal = 1; /* fail */
        

    if (command >= PPE_COMMAND_TLS) {
        if (retVal) { /* old hw_nat cmd has no retrun value */
        	printf("PPE_API: return value error\n");
        	return HWNAT_FAIL;
        }
        else {
            printf("\nppe cmd done!\n");   
        }
    }
    
    return HWNAT_SUCCESS;
}

int HwNatDumpEntry(unsigned int entry_num)
{
    struct hwnat_args opt;

    opt.entry_num=entry_num;

    return do_ppe_ioctl(&opt, HW_NAT_DUMP_ENTRY);
}

int HwNatBindEntry(unsigned int entry_num)
{
    struct hwnat_args opt;

    opt.entry_num=entry_num;

    return do_ppe_ioctl(&opt, HW_NAT_BIND_ENTRY);
}

int HwNatUnBindEntry(unsigned int entry_num)
{
    struct hwnat_args opt;

    opt.entry_num=entry_num;

    return do_ppe_ioctl(&opt, HW_NAT_UNBIND_ENTRY);
}

int HwNatInvalidEntry(unsigned int entry_num)
{
    struct hwnat_args opt;

    opt.entry_num=entry_num;

    return do_ppe_ioctl(&opt, HW_NAT_INVALID_ENTRY);
}

int HwNatCacheClear(unsigned int entry_num)
{
   struct hwnat_args opt;

    opt.entry_num=entry_num;

    return do_ppe_ioctl(&opt, HW_NAT_CLEAR_CACHE);
}
int HwNatCacheDumpData(unsigned char cache_type)
{
    struct hwnat_args opt;

    opt.cache_type = cache_type;

    return do_ppe_ioctl(&opt, HW_NAT_DUMP_CACHE_DATA);
}
int HwNatCacheDumpEntry(unsigned int entry_num)
{
    struct hwnat_args opt;

    opt.entry_num=entry_num & 0xffff;

    return do_ppe_ioctl(&opt, HW_NAT_DUMP_CACHE_ENTRY);
}

int HwNatGetAGCnt(struct hwnat_ac_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_GET_AC_CNT);

}

int HwNatSetBindThreshold(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_BIND_THRESHOLD);
}

int HwNatSetMaxEntryRateLimit(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_MAX_ENTRY_LMT);
}

int HwNatSetKaInterval(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_KA_INTERVAL);
}

int HwNatSetUnbindLifeTime(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_UB_LIFETIME);
}

int HwNatSetBindLifeTime(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_BIND_LIFETIME);
}

int HwNatSetBindDir(unsigned int dir)
{
    struct hwnat_args opt;

    opt.bind_dir=dir;

    return do_ppe_ioctl(&opt, HW_NAT_BIND_DIRECTION);
}

int HwNatSetMulticastEn(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_MULTICAST_EN);
}

int HwNatREGDUMP(struct hwnat_config_args *opt)
{
     return do_ppe_ioctl(opt, HW_NAT_REG_DUMP);
}

int HwNatSetIPChk(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_SET_IP_CHK);
}

int HwNatAddEntry(struct hwnat_tuple *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_ADD_ENTRY);
}

int HwNatGetAllEntries(struct hwnat_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_GET_ALL_ENTRIES);
}

int HwNatDebug(unsigned int debug)
{
    struct hwnat_args opt;

    opt.debug=debug;

    return do_ppe_ioctl(&opt, HW_NAT_DEBUG);
}

int HwNatTblClean(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_TAB_CLEAN);
}

int HwNatRemoveFoe(int entry_idx){
    struct hwnat_args opt;
    int fd;

    opt.entry_num=entry_idx;

    return do_ppe_ioctl(&opt, HW_NAT_FOE_REMOVE);
}

int HwNatSnoopEN(struct hwnat_config_args *opt){
    return do_ppe_ioctl(opt, HW_NAT_SNOOPING_EN);
}

int HwNatL2br_ethtypeEN(struct hwnat_config_args *opt){
    return do_ppe_ioctl(opt, HW_NAT_L2BR_ETHTYPE_EN);
}

int HwNatCleanLanWan(int cmd,int entry_idx){
    struct hwnat_args opt;

    opt.entry_num=entry_idx;

    return do_ppe_ioctl(&opt, cmd);
}

int HwNatSetDSOffloadEn(struct hwnat_config_args *opt)
{
    return do_ppe_ioctl(opt, HW_NAT_DS_OFFLOAD);
}

/******************************************************************************
function name:	ppemgr_lib_set_meter_ratelimit
 Descriptor:		It's used to cfg meter group ratelimit
 Input Args:		meterGroup:  meter group id
 				rate: ratelimit kbps

 Ret Value:		0: success
 				1: fail
 other:
 
******************************************************************************/
int ppemgr_lib_set_meter_ratelimit(unsigned int meterGrop, unsigned int rate)
{
    struct ppe_meter_cfg meterCfg;
	memset(&meterCfg, 0, sizeof(struct ppe_meter_cfg));

	if(meterGrop >= METER_GROUP_MAX)
		return -1;
	
	meterCfg.group = meterGrop;
	meterCfg.rate = rate;//kbps
	meterCfg.mode = METER_MODE_BYTE;
	meterCfg.vlan = 0;//just  config register
	meterCfg.scale = METER_SCALE_KBPS;//Kbps
	meterCfg.enable = 1;
	meterCfg.addFlowOnly = 2;// only set the meter register
	meterCfg.method = PPE_METHOD_ADD;

//	printf("\r\nppemgr_lib_set_meter_ratelimit meterCfg.group = %d, meterCfg.rate = %d kbps", meterCfg.group, meterCfg.rate);
    return do_ppe_ioctl((void *)(&meterCfg), PPE_COMMAND_METER);

}

