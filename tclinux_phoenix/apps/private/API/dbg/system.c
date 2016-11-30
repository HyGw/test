
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
*************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <libapi_lib_utility.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/


/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#define _SUCCESS	(0)
#define _FAIL	(-1)

/************************************************************************
*                  D A T A	 T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L	 D A T A   D E C L A R A T I O N S
*************************************************************************
*/



/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/
int DoInterfaceCmd(int argc, char **argv, void *p);  
static int DoPowerDW(int argc, char **argv, void *p);
static int DoPowerUP(int argc, char **argv, void *p);

static int DoPDWFE(int argc, char **argv, void *p);
static int DoPDWGE(int argc, char **argv, void *p);
static int DoPUPFE(int argc, char **argv, void *p);
static int DoPUPGE(int argc, char **argv, void *p);
static int DoPCIEP0(int argc, char **argv, void *p);
static int DoPCIEP1(int argc, char **argv, void *p);


#ifdef TCSUPPORT_USBHOST
static int DoPDWUSBP0(int argc, char **argv, void *p);
static int DoPDWUSBP1(int argc, char **argv, void *p);
#endif

#ifdef TCSUPPORT_VOIP
static int DoPDWSLIC(int argc, char **argv, void *p);
#endif

#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM)
static int DoPDWxDSL(int argc, char **argv, void *p);
#endif

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
static int DoPDWxPON(int argc, char **argv, void *p);
#endif



/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/



/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/
static cmds_t InterfaceDbgCmds[] = {
    {"power_down",         DoPowerDW,        0x10,   0,  NULL},
    {"power_up",           DoPowerUP,        0x10,   0,  NULL},
    {NULL,            NULL,            0x10,   0,  NULL},
};

static cmds_t PowerDWCmds[] = {
    {"fswitch",        DoPDWFE,             0x10,   1,  NULL},
    {"gswitch",        DoPDWGE,             0x10,   0,  NULL},
    {"pcie0",         DoPCIEP0,             0x10,   0,  NULL},
    {"pcie1",         DoPCIEP1,             0x10,   0,  NULL},
#ifdef TCSUPPORT_USBHOST
    {"usbp0",           DoPDWUSBP0,          0x10,   0,  NULL},
    {"usbp1",           DoPDWUSBP1,          0x10,   0,  NULL},
#endif
#ifdef TCSUPPORT_VOIP
    {"slic",           DoPDWSLIC,          0x10,   0,  NULL},
#endif
#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM)
    {"xdsl",           DoPDWxDSL,          0x10,   0,  NULL},
#endif

#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
    {"xpon",           DoPDWxPON,          0x10,   0,  NULL},
#endif
    {NULL,            NULL,                 0x10,   0,  NULL},
    
    
};

static cmds_t PowerUPCmds[] = {
    {"fswitch",        DoPUPFE,             0x10,   1,  NULL},
    {"gswitch",        DoPUPGE,             0x10,   0,  NULL},
    {NULL,            NULL,                0x10,   0,  NULL},
};


int DoInterfaceCmd(int argc, char **argv, void *p)  
{
    return subcmd(InterfaceDbgCmds, argc, argv, p);   
}


static int DoPowerDW(int argc, char **argv, void *p)
{
    return subcmd(PowerDWCmds, argc, argv, p);
}

static int DoPowerUP(int argc, char **argv, void *p)
{
    return subcmd(PowerUPCmds, argc, argv, p);
}


/******************************************************************************
 Descriptor:	It's used to power down switch(FE).
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPDWFE(int argc, char **argv, void *p)
{
        ushort _level = (atoi(argv[1]) & 0xf);
        ushort swport[4] = {8, 9, 10, 11}; /*lan 1,2,3,4*/
        char cmdbuf[30];
        
       if(_level < 1 || _level > 4)
       {
               printf("input error: input value :1~4, input error: %d\n", atoi(argv[1])) ;
               return _FAIL;
       }
       _level--;
#if API_CMD_EMI_DBG
        printf("enter %s.\n", __func__);
#endif
        

        sprintf(cmdbuf, "tce forcePWD %d", swport[_level] ); 
        system(cmdbuf);
        printf("Exec. Successful\n") ;

        return _SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to power UP switch(FE).
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPUPFE(int argc, char **argv, void *p)
{
        ushort _level = (atoi(argv[1]) & 0xf);
        ushort swport[4] = {8, 9, 10, 11}; /*lan 1,2,3,4*/
        char cmdbuf[30];
        
       if(_level < 1 || _level > 4)
       {
               printf("input error: input value :1~4, input error: %d\n", atoi(argv[1])) ;
               return _FAIL;
       }
       _level--;
#if API_CMD_EMI_DBG
        printf("enter %s.\n", __func__);
#endif
        

        sprintf(cmdbuf, "tce forceReset %d", swport[_level] ); 
        system(cmdbuf);
        printf("Exec. Successful\n") ;

        return _SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to power UP switch(GE).
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPUPGE(int argc, char **argv, void *p)
{
       
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                system("tce forceReset 12");
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to power down switch(GE).
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPDWGE(int argc, char **argv, void *p)
{
       
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                system("tce forcePWD 12");
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to power down pcie port0(port). only gen 1
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPCIEP0(int argc, char **argv, void *p)
{
       
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                system("ifconfig ra0 down");
                system("rmmod mt7603eap");
                system("sys memwl bfaf2000 7f7f0010"); /*PCIe port0*/
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}
/******************************************************************************
 Descriptor:	It's used to power down pcie port1 (port). gen1 + gen2
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPCIEP1(int argc, char **argv, void *p)
{
       
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                /*system("ifconfig ra0 down");*/
                system("ifconfig rai0 down");
                /*system("rmmod mt7603eap");*/
                system("rmmod mt7662e_ap");
                system("sys memwl bfac030c 80000000"); /*PCIe port1*/
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}

#ifdef TCSUPPORT_USBHOST
/******************************************************************************
 Descriptor:	It's used to power down USB2 in Port0.
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPDWUSBP0(int argc, char **argv, void *p)
{
       
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                system("sys memwl bfa81068 2000000");
                system("sys memwl bfa8106c 0");
                system("sys memwl bfa81020 1a");
                system("sys memwl bfa81068 2000008");
                system("sys memwl bfa81068 2040008");
                system("sys memwl bfa81068 2040048");
                system("sys memwl bfa81068 20400c8");
                system("sys memwl bfa81068 20400d8");
                system("sys memwl bfa81068 20400dc");
                system("sys memwl bfa81068 20400dc");
                system("sys memwl bfa81068 21400dc");
                system("sys memwl bfa81068 23400dc");
                system("sys memwl bfa81068 23c00dc");
                system("sys memwl bfa81068 23e00dc");
                system("sys memwl bfa81068 2be00dc");
                system("sys memwl bfa8101c c024150");
                system("sys memwl bfa81018 4680008");
                system("sys memwl bfa81068 2be00d4");


                system("sys modifybit bfb000c4 25 0"); /* bit[25] set 0 [d000603]*/
                system("sys modifybit bfb000ec 6 1");    /*scu p1 phy reset [6] set 1 [40]*/
                
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}

/******************************************************************************
 Descriptor:	It's used to power down usb3 + usb2 in Port1.
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
static int DoPDWUSBP1(int argc, char **argv, void *p)
{
       
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                system("sys memwl bfa8030c 80000000");
                system("sys memwl bfa80868 2000000");	
                system("sys memwl bfa8086c 0	");
                system("sys memwl bfa80820 1a");		
                system("sys memwl bfa80868 2000008");	
                system("sys memwl bfa80868 2040008");
                system("sys memwl bfa80868 2040048");	
                system("sys memwl bfa80868 20400c8");	
                system("sys memwl bfa80868 20400d8");	
                system("sys memwl bfa80868 20400dc");	
                system("sys memwl bfa80868 20400dc");	
                system("sys memwl bfa80868 21400dc");	
                system("sys memwl bfa80868 23400dc");	
                system("sys memwl bfa80868 23c00dc");	
                system("sys memwl bfa80868 23e00dc");	
                system("sys memwl bfa80868 2be00dc");	
                system("sys memwl bfa8081c c024150");	
                system("sys memwl bfa80818 4680008");	
                system("sys memwl bfa80868 2be00d4");

                
               
                system("sys modifybit bfb000c4 27 0"); /* bit[27] set 0 [7000603]*/
                system("sys modifybit bfb000ec 7 1");    /*scu p1 phy reset [7] set 1 [80]*/
                
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}
#endif

/******************************************************************************
 Descriptor:	It's used to power down slic.
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
#ifdef TCSUPPORT_VOIP
static int DoPDWSLIC(int argc, char **argv, void *p)
{
           
#if API_CMD_EMI_DBG
                 printf("enter %s.\n", __func__);
#endif         
                 system("tcapi set VoIPAdvanced_Common LanBindEnable 1");
                 system("killall sipclient");   
                 system("rmmod pcmDump.ko");
                 system("rmmod ovdsp.ko");
                 system("rmmod foip.ko");
                 system("rmmod acodec_x.ko");
                 system("rmmod ortp.ko");
                 system("rmmod ksocket.ko");
                 system("rmmod fxs3.ko");
                 system("rmmod slic3.ko");
                 system("rmmod spi.ko");
                 system("rmmod lec.ko");
                 system("rmmod pcm2.ko");
                 system("rmmod pcm1.ko");
                 system("rmmod DSPCore.ko");
                 system("rmmod sys_mod.ko");

                 
                  /* pcm reset [2020811]*/
                 system("sys modifybit bfb00834 0 1");    /*scu pcm reset [0] set 1*/
                 system("sys modifybit bfb00834 4 1");    /*scu pcm reset [4] set 1*/ 
                 system("sys modifybit bfb00834 11 1");    /*scu pcm reset [11] set 1*/
                 system("sys modifybit bfb00834 17 1");    /*scu pcm reset [17] set 1*/
                 system("sys modifybit bfb00834 25 1");    /*scu pcm reset [25] set 1*/

                 
                 printf("Exec. Successful\n") ;

                 return _SUCCESS;
}
#endif

/******************************************************************************
 Descriptor:	It's used to power down xdsl.
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM)
static int DoPDWxDSL(int argc, char **argv, void *p)
{
               
#if API_CMD_EMI_DBG
                     printf("enter %s.\n", __func__);
#endif         
                     system("sys memwl bfb00084 1");
                     system("sys memwl bfa20120 100020");
                     printf("Exec. Successful\n") ;
                     return _SUCCESS;
}
#endif

/******************************************************************************
 Descriptor:	It's used to power down xpon (only xpon mac disable clock without xpon phy)
 
 Input Args:	see PowerDWCmds.
 Ret Value:	success: _SUCCESS
                       fail: _FAIL
******************************************************************************/
#if defined(TCSUPPORT_WAN_GPON) || defined(TCSUPPORT_WAN_EPON)
static int DoPDWxPON(int argc, char **argv, void *p)
{
        
                   
#if API_CMD_EMI_DBG
                printf("enter %s.\n", __func__);
#endif         
                system("ifconfig pon down");
                system("killall -9 cfg_manager");
                system("echo gpon startup 0 > /proc/gpon/debug");
                
                system("sys modifybit bfb00834 31 1");    /*scu xpon mac reset[31] set 1 [8000000]*/
                /*system("sys  memwl bfb00830  1");*/ /*xpon phy reset*/
                system("sys  memwl bfa200e4  ffef7eff");
                system("sys  memwl bfb000e0  ff7ff");
                printf("Exec. Successful\n") ;
                return _SUCCESS;
}
#endif


