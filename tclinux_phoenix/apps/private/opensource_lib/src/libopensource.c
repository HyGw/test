/******************************************************************************/
/*
 * Copyright (C) 1994-2014 EcoNet Technologies, Corp.
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
#include <stdlib.h>
#include "libopensource.h"
#include "libtcapi.h"
#if defined(TCSUPPORT_CT)
#include "../cfg_manager_ct/cfg_manager.h"
#else
#include "../cfg_manager/cfg_manager.h"
#endif
#if defined(TCSUPPORT_CT_PON) && (TCSUPPORT_RESERVEAREA_BLOCK==7)
#include "../../public/mtd/tc_partition.h"
#endif

void
init_test_data(int index){

	return;
}

static int isMultiSerPVC(int pvc){
	if((pvc >= 8) && (pvc <=10)){
		return 1;
	}
	return 0;
}

/*return 0:not found default interface with ip ; 1:found*/
int get_default_iface(char *ifname ) //check_internet_connect
{
	int nIspNode = -1;
	char nodeName[32] = {0};
	char wanIPValue[16] = {0};
	char ispValue[8] = {0};
	char defaultRoute[8] = {0};
	//char ifname[16] = {0};
	//char def_isp[32];
	int isp;
	int is_ipv4addr_exist = 0, is_ipv6addr_exist = 0;
    int serv, service_num;
	int iface_found=0;

	if(ifname == NULL)
		return 0;
	
	/*find default router pvc*/
	for(nIspNode=0; nIspNode<PVC_NUM; nIspNode++){    
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
        if(isMultiSerPVC(nIspNode)){
            service_num = MAX_SERVICE_NUM;
        }else{
            service_num = 1;
        }
        
    for(serv=0; serv<service_num; serv++){
        if(isMultiSerPVC(nIspNode)) 
            sprintf(nodeName,"WanExt_PVC%de%d", nIspNode, serv);
        else
#endif
		sprintf(nodeName,"Wan_PVC%d", nIspNode);
		tcapi_get(nodeName, "ISP", ispValue);
		tcapi_get(nodeName, "DEFAULTROUTE", defaultRoute);
	
		if(atoi(ispValue)==WAN_ENCAP_BRIDGE)
			continue;

		if(strcmp(defaultRoute, "Yes") == 0){
			
		
			isp = atoi(ispValue);
			switch(isp){
#if !defined(TCSUPPORT_C1_NEW_GUI) 
                            case WAN_ENCAP_PPP:
#endif
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
            if(isMultiSerPVC(nIspNode))
                sprintf(ifname, "ppp%d%d", nIspNode, serv);
            else   
#endif
				sprintf(ifname, "ppp%d", nIspNode);
				break;
				default:
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
            if(isMultiSerPVC(nIspNode))
                sprintf(ifname, "nas%d_%d", nIspNode, serv);
            else
#endif
				sprintf(ifname, "nas%d", nIspNode);
				break;
			}
//#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
            iface_found=1;
//#endif
			break;
		}
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
    }
	if(iface_found)
		break;
#endif
	}
    
	return iface_found;
}

#if !(defined(TCSUPPORT_CT_PON) && (TCSUPPORT_RESERVEAREA_BLOCK==7))
#define IMG_BOOT_FLAG_SIZE 	1
#if defined(TCSUPPORT_CT)
#define IMG_BOOT_FLAG_OFFSET  	(CERM1_RA_OFFSET - IMG_BOOT_FLAG_SIZE)
#else
#ifdef TCSUPPORT_NAND_FLASH
#define IMG_BOOT_FLAG_OFFSET  	(MRD_RA_OFFSET - IMG_BOOT_FLAG_SIZE)
#else
#if (TCSUPPORT_RESERVEAREA_BLOCK==4)
#define IMG_BOOT_FLAG_OFFSET  	(DEFAULTROMFILE_RA_OFFSET)
#else
#define IMG_BOOT_FLAG_OFFSET  	(MRD_RA_OFFSET - IMG_BOOT_FLAG_SIZE)
#endif
#endif
#endif
#endif

unsigned int get_IMG_BOOT_FLAG_OFFSET(){
	return IMG_BOOT_FLAG_OFFSET;
}

unsigned int get_IMG_COMMIT_FLAG_OFFSET(){
	return (IMG_BOOT_FLAG_OFFSET + 1);
}

