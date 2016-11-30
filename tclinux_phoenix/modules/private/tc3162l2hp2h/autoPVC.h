/*
 * AutoPVC.h
 *
 *  Created on: Mar 5, 2009
 *      Author: krammer
 */

#ifndef AUTOPVC_H_
#define AUTOPVC_H_

#define	VC_HUNT_FLAG_MANG_START	0x01

#define VC_HUNT_FLAG_HUNT_SUCC 1

#define PROBE_OAM				0x01
#define PROBE_PPPOE				0x02
#define PROBE_ARP				0x04
#define PROBE_DHCP				0x08

#define MAX_PVC_NUM 8
#define MAX_VC_POOL_NUM 32
#define MaxVPI 255
#define MaxVCI 65535
#define SERVICE_NUMBER 3

#define HUNT_PVC 0
#define HUNT_VCPOOL 1

typedef struct vc_pool_s{
	uint8 vpi;
	uint16 vci;
	uint8 encap;
	uint8 management;
}vc_pool_t;


void
autopvcInit(void);
void
setTxRaw(atmTxCcDescr_t * atmTxCcDescrp);
int
vcPoolProbeVcPktRx(uint8 vpi_in,uint16 vci_in);
void RawCcCellRecv(uint8 * cellp);
void
sarAAL5RxComplete(struct sk_buff *bp,uint8	vpi,uint16	vci);
void
autopvcExit(void);

#endif /* AUTOPVC_H_ */
