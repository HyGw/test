/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#include <stdio.h>
#include <unistd.h>

#include "ether.h"
#include "libmii.h"
#include "ethercmd.h"

#ifdef ETHCMD_DBG
extern u8 ethcmdDbg;
#endif
static etherChannVlanCap_t etherVlanCap;

u8 use_extern_switch=0;

/*___________________________________________________________________
**      function name: detect switch type
**      descriptions:
**         Detect if there is external switch
**
**      parameters:
**        None
**  
**      global:
**          
**             
**      return:
**          
**	    
**      call:
**           None
**      
**      revision:
**      1. Jason 2015/09/28
**_______________________________________________________________
*/
int detect_switch(struct swconfig *swcfg)
{
    u32 switch_chip_id = 0;
    /*  external switch detected, use external  */
    switch_chip_id = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 1, 0x7ffC);
    if((switch_chip_id & 0xFFFF0000) == 0x75300000){
		return 1;
    }

    /*  external switch detected, use internal  */
    switch_chip_id = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, 0x7ffC);
    if((switch_chip_id & 0xFFFF0000) == 0x75300000){
		return 0;
    }
    return 0;
}

/*___________________________________________________________________
**      function name: getEtherVlanCap
**      descriptions:
**         Get the address of switch capability.
**
**      parameters:
**        None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          The address of etherVlanCap.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/9
**_______________________________________________________________
*/
etherChannVlanCap_t* getEtherVlanCap(void){
	return &etherVlanCap;
}/*end getEtherVlanCap*/

/*TC2206 Switch IC Functions*/
/*___________________________________________________________________
**      function name: macTC2206VlanActive
**      descriptions:
**         TC2206 switch IC to active tagged base vlan function.
**
**      parameters:
**         type: 0:Deactived
**			 1:Actived 
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macTC2206VlanActive(u8 type){
	int i;
	u32 value=0; 
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	if(type==ACTIVED){
		/*Enable VLAN function*/
		if (swcfg->flag & STAG_TO_VTAG) {
		// port 0 to 4, forward broadcast/multicast/unicast only to cpu
			for(i = 2; i < 7; i++){
				value = mdio_read(swcfg->skfd, swcfg->ifr, 20, i);
				value &= ~((1<<15)|(1<<14)|(1<<13)|(1<<12));
				mdio_write(swcfg->skfd, swcfg->ifr, 20, i, value);
			}
			//forward every arp packet to cpu port
			value = mdio_read(swcfg->skfd, swcfg->ifr, 21, 7);
			value &= ~(0x3);
			value |= 0x2;
			mdio_write(swcfg->skfd, swcfg->ifr, 21, 7, value);
			mdio_write(swcfg->skfd, swcfg->ifr, 20, 14, 0x1);
			mdio_write(swcfg->skfd, swcfg->ifr, 20, 15, 0x555f);
		}
		else {	
			/*Enable VLAN function, Tag base VLAN , Default portmap to cpu port, 
			and vlan header tagged to cpu port*/
			mdio_write(swcfg->skfd, swcfg->ifr, 20, 14, 0x1);
			mdio_write(swcfg->skfd, swcfg->ifr, 20, 15, 0x8008);
		}
		
		/*Disable  vlan_in_filter*/
		for(i=2; i<8; i++){
			value=mdio_read(swcfg->skfd, swcfg->ifr,20, i);
			value&= ~(1<<11);
			mdio_write(swcfg->skfd, swcfg->ifr, 20, i, value);
		}

		/*set default pvid=1*/
		for(i=0; i<6; i++){
			if (swcfg->flag & STAG_TO_VTAG) {
				/*force insert pvid*/
				mdio_write(swcfg->skfd, swcfg->ifr, 20, 17+i, 0x4001);

			}
			else {
				mdio_write(swcfg->skfd, swcfg->ifr, 20, 17+i, 0x01);
			}
		}
		if (swcfg->flag & STAG_TO_VTAG) {
			mdio_write(swcfg->skfd, swcfg->ifr, 24, 1, 0x555);
		}
		else {
			/*set  port5:egress tagged, port0~port4:egress untagged*/
			mdio_write(swcfg->skfd, swcfg->ifr, 24, 1, 0x955);
		}
		
		/*set  default vid=1*/
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 2, 0x01);

		/*set  vaild vlan entry, vpri=0, vid_mem=port0~port5, vlan_tbl_idx=0*/
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 0, 0x8FD0);
		
		if (swcfg->flag & STAG_TO_VTAG) {
			system("echo 1 >/proc/tc3162/vport_enable");
		}
	}
	else{
		if (swcfg->flag & STAG_TO_VTAG) {
			// forward broadcast to all ports
			for(i=2; i<7; i++){
				value=mdio_read(swcfg->skfd, swcfg->ifr,20, i);
				value |= ((1<<15)|(1<<14)|(1<<13)|(1<<12));
				mdio_write(swcfg->skfd, swcfg->ifr, 20, i, value);
			}        
			//reset arp packet setting 
			value=mdio_read(swcfg->skfd, swcfg->ifr,21, 7);
			value &= ~(0x3);
			value |= 0x1;
			mdio_write(swcfg->skfd, swcfg->ifr, 21, 7, value);
		}
	
		/*Disable VLAN function, port based*/
		mdio_write(swcfg->skfd, swcfg->ifr,20, 14, 0x0);
		
		for(i=0; i<MAX_VLAN_RULE; i++){
			if(i<6){
				/*set default pvid=1*/
				mdio_write(swcfg->skfd, swcfg->ifr, 20, 17+i, 0x01);
				/*set port0~port5:egress unchange*/
				mdio_write(swcfg->skfd, swcfg->ifr, 24, 1, 0x0);
				/*set  default vid=1*/
				mdio_write(swcfg->skfd, swcfg->ifr, 24, 2, i+1);
				/*set default lookup table*/
				mdio_write(swcfg->skfd, swcfg->ifr, 24, 0, 0x8FD0+i);
			}
			else{
				/*Disable other vlan entry*/
				mdio_write(swcfg->skfd, swcfg->ifr, 24, 0, (i|0x10));
			}
		}
		
		system("echo 0 >/proc/tc3162/vport_enable");
	}

	
	return 0;
}/*end macTC2206VlanActive*/

/*___________________________________________________________________
**      function name: macTC2206VlanSetPvid
**      descriptions:
**         TC2206 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macTC2206VlanSetPvid(u8 port, u16 pvid){
	u32 value=0;
	int regIndex=0;
	u8 vlanIndex=0;
	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();
	
	/*Parameters is error*/
	if ((port > 3) && (port != 31)){
		return -1;
	}

	/* cpu port=5 */
	if (port == 31) {
		regIndex = 22;
	}
	else{
		regIndex = 17 + port;
	}
	/* always pvid option */
	pvid |= 0x4000;
	
	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("SetPVID: port:%d pvid:%d\r\n", port, pvid);
	#endif
	mdio_write(swcfg->skfd, swcfg->ifr, 20, regIndex, pvid);
	return 0;
}/*end macTC2206VlanSetPvid*/
/*___________________________________________________________________
**      function name: macTC2206VlanSetPvid
**      descriptions:
**         TC2206 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macTC2206VlanGetPvid(u8 port){
	u32 value=0;
	int regIndex=0;
	u8 vlanIndex=0;
	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();
	
	/*Parameters is error*/
	if ((port > 3) && (port != 31)){
		return -1;
	}

	/* cpu port=5 */
	if (port == 31) {
		regIndex = 22;
	}
	else{
		regIndex = 17 + port;
	}
	return mdio_read(swcfg->skfd, swcfg->ifr, 20, regIndex);
}/*end macTC2206VlanGetPvid*/
/*___________________________________________________________________
**      function name: macTC2206VlanSetVid
**      descriptions:
**         TC2206 switch IC to set vid per vlan rule.
**
**      parameters:
**         index: Index of VLAN Table rule
**         active: 0:Deactive, 1:actived
**         vid: vlan id 
**         portMap: membership
**         tagPortMap: tagged or untagger membership
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int
macTC2206VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap
#if defined(EXTEND_SETVID_PARAM)
, u8 ivl_en, u16 fid, u16 stag
#endif
) {
	u32 value=0;
	int regIndex=0;
	u8 nTPM=0;/*newTaggedPortMap*/
	u8 nPM=0;/*newPortMap*/
#if !defined(EXTEND_SETVID_PARAM)
	u16 fid=0;
#endif
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if (index >= MAX_VLAN_RULE){
		return -1;
	}
	
	nTPM=tagPortMap&0xf;
	nPM=portMap&0xf;
	
	value=((((nTPM&0x8)>>3)+1)<<6)|((((nTPM&0x4)>>2)+1)<<4)
		|((((nTPM&0x2)>>1)+1)<<2)|((nTPM&0x1)+1);	
	/*CPU Port Tagged*/
	value|=(1<<11);
	mdio_write(swcfg->skfd, swcfg->ifr, 24, 1, value);
	/*Add differnet fid to differnet vlan group*/
	fid=(index<<12);
	mdio_write(swcfg->skfd, swcfg->ifr, 24, 2, (vid|fid));
	
	/*Vaild VLAN Entry or not*/
	value = active ? (1<<15) : 0;
	/*port Map(CPU Port),VLAN Entry Index*/
	//value |=((1<<11)|(nPM<<6)|index);
	/*port Map(CPU Port),Write Enable,VLAN Entry Index*/
	value |=((1<<11)|(nPM<<6)|(1<<4)|index);
	mdio_write(swcfg->skfd, swcfg->ifr, 24, 0, value);
	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("SetVid: index:%d active:%d vid:%d portMap:%x tagPortMap:%x\r\n", \
			index, active, vid, portMap , tagPortMap);
	#endif
	return 0;
}/*end macTC2206VlanSetVid*/
/*___________________________________________________________________
**      function name: macTC2206VlanGetVtbl
**      descriptions:
**         TC2206 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/9
**_______________________________________________________________
*/
int 
macTC2206VlanGetVtbl(u16 index){	// MT7530 need 16bit index (4K)
	u16 portMap=0,tagPortMap=0, vid=0, value=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	index&=0xf;
	mdio_write(swcfg->skfd, swcfg->ifr, 24, 0, index);
	value=mdio_read(swcfg->skfd, swcfg->ifr, 24, 0);
	/*The port Map is at 6~11 bits*/
	portMap=((value&0xfc0)>>6);
	/*The vlan tag port Map is at 0~11 bits*/
	tagPortMap=mdio_read(swcfg->skfd, swcfg->ifr, 24, 1)&0xfff;
	/*The vid vlaue is at 0~11 bits*/
	vid=mdio_read(swcfg->skfd, swcfg->ifr, 24, 2);
	fprintf(stderr,"%02d.%03s vid:%02d fid:%02d portMap:0x%02x tagMap:0x%02x\r\n" \
		, index,(value&0x8000)?"on":"off", (vid&0xfff), ((vid&0xf000)>>12),portMap, tagPortMap);
	return 0;
}/*end macTC2206VlanGetVtbl*/
/*___________________________________________________________________
**      function name: macTC2206VlanDisp
**      descriptions:
**         Display the vlan setting  at tc2206 switch ic.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/9
**_______________________________________________________________
*/
int 
macTC2206VlanDisp(void){
	u16 value=0;
	int i=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	value=mdio_read(swcfg->skfd, swcfg->ifr, 20, 14);
	fprintf(stderr, "VLAN function is %s\n", (value&ACTIVED)? ETHCMD_ENABLE:ETHCMD_DISABLE);
	fprintf(stderr,"PVID e0:%02d e1:%02d e2:%02d e3:%02d\n",macTC2206VlanGetPvid(0) \
		,macTC2206VlanGetPvid(1), macTC2206VlanGetPvid(2), macTC2206VlanGetPvid(3));
	
	for(i=0; i<MAX_VLAN_RULE; i++){
		macTC2206VlanGetVtbl(i);
	}
	return 0;
}/*end macTC2206VlanDisp*/
/*___________________________________________________________________
**      function name: macTC2206VlanCap
**      descriptions:
**         Set the TC2206 switch IC's VLAN capability.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/8
**_______________________________________________________________
*/
int 
macTC2206VlanCap(void){
	etherVlanCap.max_port_no=SWIC_PORTS_NUM;
	etherVlanCap.max_vid=MAX_VID_VALUE;
	etherVlanCap.flags=ETHER_8021Q_VLAN;
	return 0;
}/*end macTC2206VlanCap*/

#if defined(SQA_VERIFY)
int 
macRT62806VlanCap(void){
	etherVlanCap.max_port_no=6;
	etherVlanCap.max_vid=4095;
	return 0;
}/*end macTC2206VlanCap*/
#endif

/*___________________________________________________________________
**      function name: macTC2206Igmpsnoop
**      descriptions:
**         Enable/Disable/Query the TC2206 switch hardware igmpsnoop function.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_read
**           mdio_write

**      
**      revision:
**      1. Here 2009/11/8
**_______________________________________________________________
*/
int 
macTC2206Igmpsnoop(u8 type){
	int retval=0;
	u16 value=0;
	int i=0;
	
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if(type==ACTIVED){
		/*enable hardware igmpsnoop function*/
		value=mdio_read(swcfg->skfd, swcfg->ifr, 21, 5) |TC2206_IGMPSNOOP;
		mdio_write(swcfg->skfd, swcfg->ifr, 21, 5, value);
		/*Disable port0~port4 IP mulitcast default portmap*/
		for(i=2; i<8; i++){
			value=mdio_read(swcfg->skfd, swcfg->ifr, 20, i);
			value&=~(1<<15);
			mdio_write(swcfg->skfd, swcfg->ifr, 20, i, value);
		}
		/*workaroud the UPNP multicast packet will be drop by TC2206 switch*/
		/*PortMap=e0~e4*/
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 6, 0xaaa);
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 7, (0xeffffffa&0xffff));
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 8, 0x37f);
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 10, 0xc000);
		mdio_write(swcfg->skfd, swcfg->ifr, 24, 5, 0x40);
		
		/*
		**Reserve IP multicast Address for routing protocols.
		**Set RIP mask register to reserve 224.0.0.0~224.0.0.255
		**/
		mdio_write(swcfg->skfd, swcfg->ifr, 23, 30, 0xff00);
	}
	else if(type==DEACTIVED){
		/* multicast snooping register */
		value=mdio_read(swcfg->skfd, swcfg->ifr, 21, 5);
		/*disable igmp snooping function*/
		value &= ~TC2206_IGMPSNOOP;
		mdio_write(swcfg->skfd, swcfg->ifr, 21, 5, value);
		
		/*Enable Default IP Multicast Port Map*/
		for(i=2; i<8; i++){
			value= mdio_read(swcfg->skfd, swcfg->ifr, 20, i) |(1<<15);
			mdio_write(swcfg->skfd, swcfg->ifr, 20, i, value);
		}
		/*
		**Reserve IP multicast Address for routing protocols.
		**Set RIP mask register to reserve 224.0.0.0~224.0.0.255
		**/
		mdio_write(swcfg->skfd, swcfg->ifr, 23, 30, 0xff00);
	}
	else if(type==QUERY){
		value=mdio_read(swcfg->skfd, swcfg->ifr, 21, 5);
		return ((value&0x1)?ACTIVED:DEACTIVED);
	}
	else{
		retval=ETHCMD_ERR;
	}
	return retval;
}/*end macTC2206VlanCap*/

/*___________________________________________________________________
**      function name: macTC2206IgmpFastLeave
**      descriptions:
**         Enable/Disable/Query the TC2206 switch hardware igmpsnoop function.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_read
**           mdio_write

**      
**      revision:
**      1. Here 2009/11/28
**_______________________________________________________________
*/
int 
macTC2206IgmpFastLeave(u8 type){
	int retval=0;
	u16 value=0;
	
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	if(type==ACTIVED){
		/*enable hardware igmpsnoop function*/
		value=mdio_read(swcfg->skfd, swcfg->ifr, 21, 5) |TC2206_IGMPFASTLEAVE;
		mdio_write(swcfg->skfd, swcfg->ifr, 21, 5, value);
	}
	else if(type==DEACTIVED){
		/* multicast snooping register */
		value=mdio_read(swcfg->skfd, swcfg->ifr, 21, 5);
		/*Disable igmp fastleave function*/
		value &= ~TC2206_IGMPFASTLEAVE;
		mdio_write(swcfg->skfd, swcfg->ifr, 21, 5, value);
	}
	else{
		retval=ETHCMD_ERR;
	}
	return retval;
}/*end macTC2206IgmpFastLeave*/

/*RT63365 Switch IC Functions*/
/*___________________________________________________________________
**      function name: macRT63365VlanActive
**      descriptions:
**         RT63365 switch IC to active tagged base vlan function.
**
**      parameters:
**         type: 0:Deactived
**			 1:Actived 
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macRT63365VlanActive(u8 type){
	int i;
	u32 value; 
	u32 reg; 
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	int wan_port = -1;
	
	if(type==ACTIVED){
		if (swcfg->flag & STAG_TO_VTAG) {
			//port 0 to 4, forward broadcast/multicast/unicast only to cpu if lookup miss.
			reg = 0x0010;
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x404040e0);
					
			//forward every arp packet to cpu port
			reg = 0x0020;
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			value &= ~((1<<18) | (1<<17) | (1<<16) | (1<<2) | (1<<1) | (1<<0));
			value |=  ((1<<18) | (1<<17) | (1<<2) | (1<<1));
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		}

		/*Enable VLAN function*/
		/* security mode */
		for (i = 0; i <= 5; i++) {
			reg = 0x2004 + i*0x100;	
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0xff0003);
		}

		/* transparent mode */
		for (i = 0; i <= 5; i++) {
			reg = 0x2010 + i*0x100;	
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x810000c0);
		}

		/* set cpu port as user port */
		reg = 0x2610;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		/* keep special tag */
		value &= 0x20;
		value |= 0x81000000;
#if defined(TCSUPPORT_SOC_ETHER_WAN)
        value |= 0x800;     /* enable PT*/
#endif
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		/* port6, Egress VLAN Tag Attribution=tagged */
		reg = 0x2604;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x20ff0003);

		if (swcfg->flag & STAG_TO_VTAG){
			wan_port = get_wan_port();
			//set wan port to PVID 5.
			if(wan_port != -1){
				reg = 0x2014 + wan_port*0x100;
				gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x10005);	
			}
		}else{	
		/* set PVID */
		for (i = 0; i <= 6; i++) {
			reg = 0x2014 + i*0x100;	
			value = 0x10000 + (i+1);
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		}
	}
#if defined(MT7530_SUPPORT) || defined(TCSUPPORT_MT7510_FE)
		system("echo 1 >/proc/tc3162/vport_enable");
#endif				
	}
	else{
		/*Disable VLAN function*/
		if (swcfg->flag & STAG_TO_VTAG) {
			//port 0 to 4, forward broadcast/multicast/unicast to every port.
			reg = 0x0010;
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0xffffffe0);

			//forward every arp packet to cpu port
			reg = 0x0020;
			value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
			value &= ~((1<<18) | (1<<17) | (1<<16) | (1<<2) | (1<<1) | (1<<0));
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		}	
			
		/* port matrix mode */
		for (i = 0; i <= 5; i++) {
			reg = 0x2004 + i*0x100;	
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0xff0000);
		}

		/* transparent mode */
		for (i = 0; i <= 5; i++) {
			reg = 0x2010 + i*0x100;	
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x810000c0);
		}

		/* set cpu port as user port */
		reg = 0x2610;
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		/* keep special tag */
		value &= 0x20;
		if (value & 0x20)
			value |= 0x81000000;
		else
			value |= 0x810000c0;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

		/* port6 */
		reg = 0x2604;
		gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x00ff0000);

		if (swcfg->flag & STAG_TO_VTAG) {
			wan_port = get_wan_port();
			//set wan port to PVID 2.
			if(wan_port != -1){
				reg = 0x2014 + wan_port*0x100;
				gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x10001);	
			}
		}else{
		/* set PVID as 1 */
		for (i = 0; i <= 6; i++) {
			reg = 0x2014 + i*0x100;	
			value = 0x10000 + (1);
			gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
		}

		}
#if defined(MT7530_SUPPORT) || defined(TCSUPPORT_MT7510_FE)
		system("echo 0 >/proc/tc3162/vport_enable");
#endif			
	}
	
	/* switch clear */
	reg = 0x80;
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x8002);
	usleep(5000);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	
	return 0;
}/*end macRT63365VlanActive*/

/*___________________________________________________________________
**      function name: macRT63365VlanSetPvid
**      descriptions:
**         RT63365 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macRT63365VlanSetPvid(u8 port, u16 pvid){
	u32 value;
	u32 reg;
	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();
	
	/*Parameters is error*/
	if (port > 6) {
		return -1;
	}

	reg = 0x2014 + (port * 0x100);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	value &= ~0xfff;
	value |= pvid;
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
	
	/* switch clear */
	reg = 0x80;
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x8002);
	usleep(5000);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("SetPVID: port:%d pvid:%d\r\n", port, pvid);
	#endif
	return 0;
}/*end macRT63365VlanSetPvid*/
/*___________________________________________________________________
**      function name: macRT63365VlanSetPvid
**      descriptions:
**         RT63365 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macRT63365VlanGetPvid(u8 port){
	u32 value;
	u32 reg;
	struct swconfig *swcfg=NULL;

	swcfg=getSwcfg();
	
	/*Parameters is error*/
	if (port > 6) {
		return -1;
	}

	reg = 0x2014 + (port * 0x100);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	return (value & 0xfff);
}/*end macRT63365VlanGetPvid*/
/*___________________________________________________________________
**      function name: macRT63365VlanSetVid
**      descriptions:
**         RT63365 switch IC to set vid per vlan rule.
**
**      parameters:
**         index: Index of VLAN Table rule
**         active: 0:Deactive, 1:actived
**         vid: vlan id 
**         portMap: membership
**         tagPortMap: tagged or untagger membership
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macRT63365VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap
#if defined(EXTEND_SETVID_PARAM) || defined(SQA_VERIFY)
, u8 ivl_en, u8 fid, u16 stag
#endif
) {
	u32 value=0;
	u32 value2=0;
	int reg=0;

#if !defined(EXTEND_SETVID_PARAM) && !defined(SQA_VERIFY)
	u8 ivl_en=1;
	u16 fid=0;
	u8 stag=0;
#endif
#if !defined(SQA_VERIFY)
	ivl_en=1;
#endif

	int i;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

#if defined(SQA_VERIFY)
	printf("index: %x, active: %x, vid: %x, portMap: %x, tagPortMap: %x, ivl_en: %x, fid: %x, stag: %x\n",
			index, active, vid, portMap, tagPortMap, ivl_en, fid, stag);
#endif			

	if (index >= MAX_VLAN_RULE){
		return -1;
	}
	
	/* vlan id */
	reg = 0x100 + 4 * (index/2);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	if (index % 2 == 0) {
		value &= 0xfff000;
		value |= vid;
	} else {
		value &= 0xfff;
		value |= (vid << 12);
	}
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

#if !defined(SQA_VERIFY)
	portMap |= (1<<6);
#endif
	value = (portMap<<16);
	value |= (stag<<4);
	value |= (ivl_en<<30);
	value |= (fid<<1);
	value |= (active ? 1 : 0);

	for (i = 0; i < SWIC_PORTS_NUM; i++) {
		if (tagPortMap & (1<<i)) {
			value2 |= 0x2<<(i*2);
		}
	}

	if (value2) {
		value |= (1<<28);		// eg_tag
	}

	reg = 0x98; // VAWD2
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value2);

	reg = 0x94; // VAWD1
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

	reg = 0x90; // VTCR
	value = (0x80001000 + index);
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

	reg = 0x90; // VTCR
	while (1) {
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		if ((value & 0x80000000) == 0) { //table busy
			break;
		}
	}

	/* switch clear */
	reg = 0x80;
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x8002);
	usleep(5000);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("SetVid: index:%d active:%d vid:%d portMap:%x tagPortMap:%x\r\n", \
			index, active, vid, portMap , tagPortMap);
	#endif
	return 0;
}/*end macRT63365VlanSetVid*/
/*___________________________________________________________________
**      function name: macRT63365VlanGetVtbl
**      descriptions:
**         RT63365 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/9
**_______________________________________________________________
*/
int
macRT63365VlanGetVtbl(u16 index){	// MT7530 need 16bit index (4K)
	u16 vid=0;
	u32 reg, value, vawd1, vawd2;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	/* vlan id */
	reg = 0x100 + 4 * (index/2);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	if (index % 2 == 0) {
		vid = value & 0xfff;
	} else {
		vid = (value & 0xfff000)>>12;
	}

	reg = 0x90; // VTCR
	value = (0x80000000 + index);
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

	reg = 0x90; // VTCR
	while (1) {
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		if ((value & 0x80000000) == 0) { //table busy
			break;
		}
	}

	reg = 0x94; // VAWD1
	vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

	reg = 0x98; // VAWD2
	vawd2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

#if defined(SQA_VERIFY)
	fprintf(stderr,"%02d.%03s vid:%02d fid:%02d portMap:0x%02x tagMap:0x%04x stag:0x%02x ivl_en:0x%02x\r\n",
	index,(vawd1&0x1)?"on":"off",vid,((vawd1&0xe)>>1),(vawd1&0xff0000)>>16,vawd2,(vawd1&0xfff0)>>0x4,(vawd1>>30)&0x1);
#else
	fprintf(stderr,"%02d.%03s vid:%02d fid:%02d portMap:0x%02x tagMap:0x%02x\r\n",
		index,(vawd1&0x1)?"on":"off", vid, ((vawd1&0xe)>>1),(vawd1&0xff0000)>>16, vawd2);
#endif
	return 0;
}/*end macRT63365VlanGetVtbl*/
/*___________________________________________________________________
**      function name: macRT63365VlanDisp
**      descriptions:
**         Display the vlan setting  at tc2206 switch ic.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/9
**_______________________________________________________________
*/
int 
macRT63365VlanDisp(void){
	int i=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	u32 reg, value;

	reg = 0x2604; 
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	value &= 0x30000000;

	fprintf(stderr, "VLAN function is %s\n", value ? ETHCMD_ENABLE:ETHCMD_DISABLE);
	fprintf(stderr,"PVID e0:%02d e1:%02d e2:%02d e3:%02d e4:%02d e5:%02d\n",
		macRT63365VlanGetPvid(0), macRT63365VlanGetPvid(1), macRT63365VlanGetPvid(2), 
		macRT63365VlanGetPvid(3), macRT63365VlanGetPvid(4), macRT63365VlanGetPvid(5));
	
	for(i=0; i<MAX_VLAN_RULE; i++){
		macRT63365VlanGetVtbl(i);
	}
	return 0;
}/*end macRT63365VlanDisp*/
/*___________________________________________________________________
**      function name: macRT63365VlanCap
**      descriptions:
**         Set the RT63365 switch IC's VLAN capability.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/8
**_______________________________________________________________
*/
int 
macRT63365VlanCap(void){
	etherVlanCap.max_port_no=SWIC_PORTS_NUM;
	etherVlanCap.max_vid=MAX_VID_VALUE;
	etherVlanCap.flags=ETHER_8021Q_VLAN;
	return 0;
}/*end macRT63365VlanCap*/
/*___________________________________________________________________
**      function name: macRT63365Igmpsnoop
**      descriptions:
**         Enable/Disable/Query the RT63365 switch hardware igmpsnoop function.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_read
**           mdio_write

**      
**      revision:
**      1. Here 2009/11/8
**_______________________________________________________________
*/
int 
macRT63365Igmpsnoop(u8 type){
	int retval=0;
	u16 value=0;
	int i=0;
	
	/* TBD */
	return retval;
}/*end macRT63365VlanCap*/

/*___________________________________________________________________
**      function name: macRT63365IgmpFastLeave
**      descriptions:
**         Enable/Disable/Query the RT63365 switch hardware igmpsnoop function.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_read
**           mdio_write

**      
**      revision:
**      1. Here 2009/11/28
**_______________________________________________________________
*/
int 
macRT63365IgmpFastLeave(u8 type){
	int retval=0;
	u16 value=0;

	/* TBD */
	return retval;
}/*end macRT63365IgmpFastLeave*/



/*mt7530 Switch IC Functions*/
/*___________________________________________________________________
**      function name: macRT7530VlanActive
**      descriptions:
**         mt7530 switch IC to active tagged base vlan function.
**
**      parameters:
**         type: 0:Deactived
**			 1:Actived
**
**      global:
**           None
**
**      return:
**          0: Success
**          -1: FAIL
**
**      call:
**           getSwcfg
**           mdio_write
**           mdio_read
**
**      revision:
**      1. Reid 2013/2/19
**_______________________________________________________________
*/
	
#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
	extern u32 mt751020_submodel_id;	// 0 means uninitialized/invalid
	extern u32 np_hw_id;
	extern u32 efuse_extreme_mode;
#endif

int
macRT7530VlanActive(u8 type)
{
	int i,ext_switch;
	u32 value;
	u32 reg;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	int wan_port = get_wan_port();

#if 0
	reg = 0x2610;
	if (gsw_reg_read2(swcfg->skfd, swcfg->ifr, 1, reg) == 0)//inner switch only
		flag = 0;
	else
		flag = 1;
#endif
#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
		ext_switch = 1;
	else
		ext_switch = 0;
#elif defined(DEFAULT_USE_EXT_SWIC)
	ext_switch = 1;
#else
	ext_switch = 0;
#endif

	if(type==ACTIVED)
	{
		//port 0 to 4, forward broadcast/multicast/unicast only to cpu if lookup miss.
		reg = 0x0010;
		if (wan_port >= 0 && wan_port < 6){
			//set port 6 as mirror port,used for etherwan
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x404040ee);
		}else
			// without etherwan, no need to set mirror port
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x404040e0);
		
		//forward every arp packet to cpu port
		reg = 0x0020;
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
		value &= ~((1<<18) | (1<<17) | (1<<16) | (1<<2) | (1<<1) | (1<<0));
		value |=  ((1<<18) | (1<<17) | (1<<2) | (1<<1));
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		
		// vlan mismatch forword to cpu port
		for (i = 0; i <= 6; i++) 
		{
			reg = 0x2004 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xff0003);
		}
		
		if (wan_port >=0 && wan_port < 6){
			//use mirror port + port matrix to make etherwan packet only sent to CPU port
			reg = 0x2004 + wan_port * 0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xf00100);
		
			//disable etherwan port mac learning
			reg = 0x200c + wan_port * 0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xfff10);
		}
		
		for (i = 0; i <= 5; i++)
		{
			reg = 0x2010 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008100); // EG_TAG consistent
		}
		reg = 0x2610;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008020);// user port & enable stag
		
		for (i = 0; i <= 6; i++) 
		{	// set vlan group
			reg = 0x2014 + i*0x100;
			value = 0x10000 + (i+1);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		}
		
		if (ext_switch == 1)
		{
			//config inner switch
			for (i = 0; i <= 6; i++)
			{
				reg = 0x2010 + i*0x100;
				//set all port to transparent mode
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0x810081c0);
			}
			for (i = 0; i <= 6; i++)
			{
				//disable mac learning
				reg = 0x200c + i*0x100;
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0xfff10);
			}

			//disable inner switch pkt len check
			reg = 0x000c;
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, reg);
			value &= ~(1 << 4);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, value);
		}
		
#if defined(MT7530_SUPPORT) || defined(TCSUPPORT_MT7510_FE)
		system("echo 1 >/proc/tc3162/vport_enable");
#endif				
	}
	else
	{
		/* Disable VLAN Function */
		// port 0 to 4, forword broadcast/multicast/unicast to every port.
		reg = 0x0010;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xffffffe0);
		// forward every arp packet
		reg = 0x0020;
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
		value &= ~((1<<18) | (1<<17) | (1<<16) | (1<<2) | (1<<1) | (1<<0));
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		
		// port matrix mode 
		for (i = 0; i <= 6; i++) 
		{
			reg = 0x2004 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xff0000);
		}
		/*for (i = 0; i <= 5; i++)
		{
			reg = 0x2010 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x810081c0);
		}*/
		// set EG_TAG = CONSISTENT
		for (i = 0; i <= 5; i++)
		{
			reg = 0x2010 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008100);
		}
		// set CPU Port EG_TAG=CONSISTENT & enable SP_TAG
		reg = 0x2610;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008120);

		// set PVID as 1
		for (i = 0; i <= 6; i++) 
		{
			reg = 0x2014 + i*0x100;
			value = 0x10000 + (1);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		}
			
		if (ext_switch == 1)
		{
			for (i = 0; i <= 6; i++)
			{
				reg = 0x2010 + i*0x100;
				//set all port to transparent mode
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0x810081c0);
			}
			for (i = 0; i <= 6; i++)
			{
				//disable mac learning
				reg = 0x200c + i*0x100;
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0xfff10);
			}
		}


#if defined(MT7530_SUPPORT) || defined(TCSUPPORT_MT7510_FE)
		system("echo 0 >/proc/tc3162/vport_enable");
#endif			
	}

	/* switch clear */
	reg = 0x80;
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x8002);
	usleep(5000);
	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
	if(ext_switch == 1)
	{
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0x8002);
		usleep(5000);
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, reg);
	}

	return 0;
}




/*___________________________________________________________________
**      function name: macMT7530VlanSetVid
**      descriptions:
**         MT7530 switch IC to set vid per vlan rule.
**
**      parameters:
**         index: Index of VLAN Table rule
**         active: 0:Deactive, 1:actived
**         vid: vlan id
**         portMap: membership
**         tagPortMap: tagged or untagger membership
**
**      global:
**           None
**
**      return:
**          0: Success
**          -1: FAIL
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1. Frank 2012/04
**_______________________________________________________________
*/
int
macMT7530VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap
#if defined(EXTEND_SETVID_PARAM) || defined(SQA_VERIFY)
, u8 ivl_en, u8 fid, u16 stag
#endif
) {
	u32 value=0;
	u32 value2=0;
	int reg=0;

#if !defined(EXTEND_SETVID_PARAM) && !defined(SQA_VERIFY)
	u8 ivl_en=1;
	u16 fid=0;
	u8 stag=0;
#endif
#if !defined(SQA_VERIFY)
	ivl_en=1;
#endif

	int i;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

#if defined(SQA_VERIFY)
	printf("index: %x, active: %x, vid: %x, portMap: %x, tagPortMap: %x, ivl_en: %x, fid: %x, stag: %x\n",
			index, active, vid, portMap, tagPortMap, ivl_en, fid, stag);
#endif

#if 0	// these register no more necessary for MT7530
	if (index >= MAX_VLAN_RULE){
		return -1;
	}

	/* vlan id */
	reg = 0x100 + 4 * (index/2);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	if (index % 2 == 0) {
		value &= 0xfff000;
		value |= vid;
	} else {
		value &= 0xfff;
		value |= (vid << 12);
	}
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);
#endif

#if !defined(SQA_VERIFY)
	portMap |= (1<<6);
#endif
	value = (portMap<<16);
	value |= (stag<<4);
	value |= (ivl_en<<30);
	value |= (fid<<1);
	value |= (active ? 1 : 0);

	// total 7 ports
#if defined(SQA_VERIFY)
	for (i = 0; i < 7; i++)
#else
	for (i = 0; i < 6; i++)	//SWIC_PORTS_NUM
#endif
	{
		if (tagPortMap & (1<<i)) {
			value2 |= 0x2<<(i*2);
		}
	}

	if (value2) {
		value |= (1<<28);		// eg_tag
	}

	reg = 0x98; // VAWD2
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value2);

	reg = 0x94; // VAWD1
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

	reg = 0x90; // VTCR
	value = (0x80001000 + vid);
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

	reg = 0x90; // VTCR
	while (1) {
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		if ((value & 0x80000000) == 0) { //table busy
			break;
		}
	}

	/* switch clear */
	reg = 0x80;
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, 0x8002);
	usleep(5000);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

	#ifdef ETHCMD_DBG
	if(ethcmdDbg)
		printf("SetVid: index:%d active:%d vid:%d portMap:%x tagPortMap:%x\r\n", \
			index, active, vid, portMap , tagPortMap);
	#endif
	return 0;
}/*end macMT7530VlanSetVid*/

/*___________________________________________________________________
**      function name: macMT7530VlanGetVtbl
**      descriptions:
**         MT7530 switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**
**      global:
**           None
**
**      return:
**          0: Success
**          -1: FAIL
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1. Frank 2012/04
**_______________________________________________________________
*/
int
macMT7530VlanGetVtbl(u16 index){
	u16 vid=0;
	u32 reg, value, vawd1, vawd2;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

#if 0
	/* vlan id */
	reg = 0x100 + 4 * (index/2);
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	if (index % 2 == 0) {
		vid = value & 0xfff;
	} else {
		vid = (value & 0xfff000)>>12;
	}
#endif
	reg = 0x90; // VTCR
	value = (0x80000000 + index);
	gsw_reg_write(swcfg->skfd, swcfg->ifr, reg, value);

	reg = 0x90; // VTCR
	while (1) {
		value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
		if ((value & 0x80000000) == 0) { //table busy
			break;
		}
	}

	reg = 0x94; // VAWD1
	vawd1 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

	reg = 0x98; // VAWD2
	vawd2 = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);

	if (vawd1&0x1){
		fprintf(stderr,"%02d.%03s vid:%02d fid:%02d portMap:0x%02x tagMap:0x%04x stag:0x%02x ivl_en:0x%02x\r\n",
		index,(vawd1&0x1)?"on":"off",index,((vawd1&0xe)>>1),(vawd1&0xff0000)>>16,vawd2,(vawd1&0xfff0)>>0x4,(vawd1>>30)&0x1);
	}
	return 0;
}/*end macMT7530VlanGetVtbl*/

/*___________________________________________________________________
**      function name: macMT7530VlanDisp
**      descriptions:
**         Display the vlan setting at MT7530 switch ic.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**
**      global:
**           None
**
**      return:
**          0: Success
**          -1: FAIL
**
**      call:
**           getSwcfg
**           mdio_write
**
**      revision:
**      1. Frank 2012/04
**_______________________________________________________________
*/
int
macMT7530VlanDisp(void){
	int i=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	u32 reg, value;

	reg = 0x2604;
	value = gsw_reg_read(swcfg->skfd, swcfg->ifr, reg);
	value &= 0x30000000;

	fprintf(stderr, "VLAN function is %s\n", value ? ETHCMD_ENABLE:ETHCMD_DISABLE);
	fprintf(stderr,"PVID e0:%02d e1:%02d e2:%02d e3:%02d e4:%02d e5:%02d e6:%02d\n",
		macRT63365VlanGetPvid(0), macRT63365VlanGetPvid(1), macRT63365VlanGetPvid(2),
		macRT63365VlanGetPvid(3), macRT63365VlanGetPvid(4), macRT63365VlanGetPvid(5), macRT63365VlanGetPvid(6));

	for(i=0; i<4096; i++){
		macMT7530VlanGetVtbl(i);
	}
	return 0;
}/*end macMT7530VlanDisp*/

/*___________________________________________________________________
**      function name: macMT7530VlanCap
**      descriptions:
**         Set the MT7530 switch IC's VLAN capability.
**
**      parameters:
**         None
**
**      global:
**           etherVlanCap
**
**      return:
**          0: Success
**          -1: FAIL
**
**      call:
**           None
**
**      revision:
**      1. Frank 2012/04
**_______________________________________________________________
*/
int
macMT7530VlanCap(void){
#if defined(SQA_VERIFY)
	etherVlanCap.max_port_no=7;
#else
	etherVlanCap.max_port_no=6;	//SWIC_PORTS_NUM
#endif
	etherVlanCap.max_vid=MAX_VID_VALUE;
	return 0;
}/*end macMT7530VlanCap*/

/*IP175C Switch IC Functions*/
/*___________________________________________________________________
**      function name: macIP175CVlanActive
**      descriptions:
**         IP175C switch IC to active tagged base vlan function.
**
**      parameters:
**         type: 0:Deactived
**			 1:Actived 
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macIP175CVlanActive(u8 type){
	int i;
	u16 value=0;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	
	if(type==ACTIVED){
		/* router control register 1: TAG_VLAN_EN | ROUTER ENABLE*/
		mdio_write(swcfg->skfd, swcfg->ifr, 30, 9, ((0x1001) | (1<<7) |(1<<3)));
		
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 23, 0x7c2);
	
		/* set default vlan lookup table */
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 19, 0x9f9f);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 20, 0x9f9f);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 21, 0x9f00);
	
		/* set default pvid */
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 28, 0x1);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 30, 0x1);
	
		/* set tag vlan output mask */
		for (i = 1; i<= 8; i++)
			mdio_write(swcfg->skfd, swcfg->ifr, 30, i, 0x0);
		/*Disable port#0~port#4 mac learning, bit8~bit12*/
		value=mdio_read(swcfg->skfd, swcfg->ifr, 30, 16) & 0xE0FF;
		mdio_write(swcfg->skfd, swcfg->ifr, 30, 16, value);
	}
	else{
		/* router control register 1: disable vlan tag */
		mdio_write(swcfg->skfd, swcfg->ifr, 30, 9, 0x1001);
		
		/* tag register 11: Port5 disable add tag */
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 23, 0x0);
	
		/* set default vlan lookup table */
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 19, 0x9f9f);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 20, 0x9f9f);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 21, 0x9f00);
		
		/* set default pvid */
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 24, 0xFFF);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 25, 0xFFF);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 26, 0xFFF);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 27, 0xFFF);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 28, 0x1);
		mdio_write(swcfg->skfd, swcfg->ifr, 29, 30, 0x1);
		
		/* set tag vlan output mask */
		for (i = 1; i<= 8; i++)
			mdio_write(swcfg->skfd, swcfg->ifr, 30, i, 0x3f3f);
		/*Enable port#0~port#4 mac learning, bit8~bit12*/
		value=mdio_read(swcfg->skfd, swcfg->ifr, 30, 16) |0x1f00;
		mdio_write(swcfg->skfd, swcfg->ifr, 30, 16, value);
	}	
	/* soft reset */
	mdio_write(swcfg->skfd, swcfg->ifr, 30, 0, 0x175C);
	sleep(1);
	return 0;
}/*end macIP175CVlanActive*/
/*___________________________________________________________________
**      function name: macIP175CVlanSetPvid
**      descriptions:
**         IP175C switch IC to set pvid per ethernet port id.
**
**      parameters:
**         port: port number :0~3
**         pvid: PVID
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
macIP175CVlanSetPvid(u8 port, u16 pvid){
	int regIndex;
	struct swconfig *swcfg=NULL;
	
	swcfg=getSwcfg();
	
	regIndex = 24 + port;
	mdio_write(swcfg->skfd, swcfg->ifr, 29, regIndex, pvid);
	return 0;
}/*end macIP175CVlanSetPvid*/
/*___________________________________________________________________
**      function name: macIP175CVlanSetVid
**      descriptions:
**         IP175C switch IC to set vid per vlan rule.
**
**      parameters:
**         index: Index of VLAN Table rule
**         active: 0:Deactive, 1:actived
**         vid: vlan id 
**         portMap: membership
**         tagPortMap: tagged or untagger membership
**  
**      global:
**           None
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           getSwcfg
**           mdio_write
**           mdio_read
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int
macIP175CVlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap
#if defined(EXTEND_SETVID_PARAM)
, u8 ivl_en, u8 fid, u16 stag
#endif
) {
	u32 value=0;
	int regIndex;
	u8 higherPart;
	struct swconfig *swcfg=NULL;

	if (index > 15)
		return -1;
	if (active) {
		/* IP175C only supports 4 bit VID */
		if (vid > 15)
			return -1;
		if (vid != index){
			return -1;
		}
	} else {
		vid = index;
	}

	swcfg=getSwcfg();
	
	/* tag vlan mask 0~15: phy=30 mii=1~8 */
	regIndex = (vid >> 1) + 1;
	higherPart = vid & 0x1;
	
	value=mdio_read(swcfg->skfd, swcfg->ifr, 30, regIndex);
	if(active){
		if (higherPart) 
			value = ((value & 0x00ff) |((portMap & 0xf) | (1 << 5) ) << 8);
		else
			value = ((value & 0xff00) |(portMap & 0xf) | (1 << 5)) ;
	} else {
		if (higherPart) 
			value = ((value & 0x00ff) | ((1 << 5) << 8));
		else
			value = ((value & 0xff00) | ((1 << 5)));
	}
	
	mdio_write(swcfg->skfd, swcfg->ifr, 30, regIndex, value);
	return 0;
}/*end macIP175CVlanSetVid*/
/*___________________________________________________________________
**      function name: macIP175CVlanCap
**      descriptions:
**         Set the IP175C switch IC's VLAN capability.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/8
**_______________________________________________________________
*/
int 
macIP175CVlanCap(void){
	etherVlanCap.max_port_no=SWIC_PORTS_NUM;
	etherVlanCap.max_vid=15;
	etherVlanCap.flags=ETHER_VLAN_SAME_VID_IDX;
	return 0;
}/*end macIP175CVlanCap*/

/*Single Port*/
/*___________________________________________________________________
**      function name: macIP175CVlanCap
**      descriptions:
**         Set the single port VLAN capability.
**
**      parameters:
**         None
**  
**      global:
**           etherVlanCap
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/8
**_______________________________________________________________
*/
int 
macDefVlanCap(void){
	etherVlanCap.max_port_no=SINGLE_PORT;
	etherVlanCap.max_vid=MAX_VID_VALUE;
	etherVlanCap.flags=ETHER_8021Q_VLAN;
	return 0;
}/*end macDefVlanCap*/


static void ether_wan_config(int wan_port, int ext_switch)
{
    u32 value,reg;
    struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	//use port matrix + ACL to make etherwan packet only sent to CPU port
	reg = 0x2004 + wan_port * 0x100;
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x00f00400);

	//disable etherwan port mac learning
	reg = 0x200c + wan_port * 0x100;
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xfff10);


    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x94, 0x0);    
    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x98, 0x80000 | ((1<<wan_port)<<8));
    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x90, 0x80005000);

    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x94, 0x1);    
    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x98, 0x0);
    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x90, 0x80009000);

    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x94, 0x4080);    
    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x98, 0x0);
    gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, 0x90, 0x8000B000);
}

/*mt7530 Switch IC Functions*/
/*___________________________________________________________________
**      function name: macEN7512VlanActive
**      descriptions:
**         mt7530 switch IC to active tagged base vlan function.
**
**      parameters:
**         type: 0:Deactived
**			 1:Actived
**
**      global:
**           None
**
**      return:
**          0: Success
**          -1: FAIL
**
**      call:
**           getSwcfg
**           mdio_write
**           mdio_read
**
**      revision:
**      1. Jason 2015/07/28
**_______________________________________________________________
*/


int
macEN7512VlanActive(u8 type)
{
	int i,ext_switch;
	u32 value;
	u32 reg;
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();

	int wan_port = get_wan_port();

#if 0
	reg = 0x2610;
	if (gsw_reg_read2(swcfg->skfd, swcfg->ifr, 1, reg) == 0)//inner switch only
		flag = 0;
	else
		flag = 1;
#endif

#if defined(JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL)
	if (_DefaultUseExtMT7530)
		ext_switch = 1;
	else
		ext_switch = 0;
#elif defined(DEFAULT_USE_EXT_SWIC)
	ext_switch = 1;
#else
	ext_switch = 0;
#endif

#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	//move use_extern_switch init to mt7530_init_api
  //  use_extern_switch = detect_switch(swcfg);
    ext_switch = use_extern_switch;
#endif

	if(type==ACTIVED)
	{
		//port 0 to 4, forward broadcast/multicast/unicast only to cpu if lookup miss.
		reg = 0x0010;
        gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x404040e0);
		
		//forward every arp packet to cpu port
		reg = 0x0020;
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
		value &= ~((1<<18) | (1<<17) | (1<<16) | (1<<2) | (1<<1) | (1<<0));
		value |=  ((1<<18) | (1<<17) | (1<<2) | (1<<1));
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		
		// vlan mismatch forword to cpu port
		for (i = 0; i <= 6; i++) 
		{
			reg = 0x2004 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xff0003);
		}
		
		if (wan_port >=0 && wan_port < 6){
			// user acl for ether wan
			ether_wan_config(wan_port, ext_switch);
		}
		
		for (i = 0; i <= 5; i++)
		{
			reg = 0x2010 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008100); // EG_TAG consistent
		}
		reg = 0x2510;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008020);// user port & enable stag
		reg = 0x2610;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008020);// user port & enable stag
		
		for (i = 0; i <= 6; i++) 
		{	// set vlan group
			reg = 0x2014 + i*0x100;
			value = 0x10000 + (i+1);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		}
		
		if (ext_switch == 1)
		{
			//config inner switch
			for (i = 0; i <= 6; i++)
			{
				reg = 0x2010 + i*0x100;
				//set all port to transparent mode
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0x810081c0);
			}
			for (i = 0; i <= 6; i++)
			{
				//disable mac learning
				reg = 0x200c + i*0x100;
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0xfff10);
			}

			//disable inner switch pkt len check
			reg = 0x000c;
			value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, reg);
			value &= ~(1 << 4);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, value);
		}
		system("echo 1 >/proc/tc3162/vport_enable");
			
	}
	else
	{
		/* Disable VLAN Function */
		// port 0 to 4, forword broadcast/multicast/unicast to every port.
		reg = 0x0010;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xffffffe0);
		// forward every arp packet
		reg = 0x0020;
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
		value &= ~((1<<18) | (1<<17) | (1<<16) | (1<<2) | (1<<1) | (1<<0));
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		
		// port matrix mode 
		for (i = 0; i <= 6; i++) 
		{
			reg = 0x2004 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0xff0000);
		}
		/*for (i = 0; i <= 5; i++)
		{
			reg = 0x2010 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x810081c0);
		}*/
		// set EG_TAG = CONSISTENT
		for (i = 0; i <= 5; i++)
		{
			reg = 0x2010 + i*0x100;
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008100);
		}
		// set CPU Port EG_TAG=CONSISTENT & enable SP_TAG
		reg = 0x2610;
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x81008120);

		// set PVID as 1
		for (i = 0; i <= 6; i++) 
		{
			reg = 0x2014 + i*0x100;
			value = 0x10000 + (1);
			gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, value);
		}
			
		if (ext_switch == 1)
		{
			for (i = 0; i <= 6; i++)
			{
				reg = 0x2010 + i*0x100;
				//set all port to transparent mode
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0x810081c0);
			}
			for (i = 0; i <= 6; i++)
			{
				//disable mac learning
				reg = 0x200c + i*0x100;
				gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0xfff10);
			}
		}
		system("echo 0 >/proc/tc3162/vport_enable");		
	}

	/* switch clear */
	reg = 0x80;
	gsw_reg_write2(swcfg->skfd, swcfg->ifr, ext_switch, reg, 0x8002);
	usleep(5000);
	value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, ext_switch, reg);
	if(ext_switch == 1)
	{
		gsw_reg_write2(swcfg->skfd, swcfg->ifr, 0, reg, 0x8002);
		usleep(5000);
		value = gsw_reg_read2(swcfg->skfd, swcfg->ifr, 0, reg);
	}

	return 0;
}

