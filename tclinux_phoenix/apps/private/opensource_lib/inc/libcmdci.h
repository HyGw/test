/******************************************************************************/
/*
 * Copyright (C) 1994-2014 Econet Technologies, Corp.
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
#ifndef _CMDCI_LIB_H
#define _CMDCI_LIB_H

#define PORTBIND_IOC_MAGIC 'g'
#define PORTBIND_MAJOR 200
#define PORTBIND_IOC_ENABLE _IOW(PORTBIND_IOC_MAGIC,0,unsigned long)
#define PORTBIND_IOC_DISABLE _IOR(PORTBIND_IOC_MAGIC,1,unsigned long)
#define PORTBIND_IOC_ADD_INTERFACE _IOR(PORTBIND_IOC_MAGIC,2,unsigned long)
#define PORTBIND_IOC_DEL_INTERFACE _IOR(PORTBIND_IOC_MAGIC,3,unsigned long)
#define PORTBIND_IOC_ADD_GROUP _IOR(PORTBIND_IOC_MAGIC,4,unsigned long)
#define PORTBIND_IOC_DEL_GROUP _IOR(PORTBIND_IOC_MAGIC,5,unsigned long)
#define PORTBIND_IOC_GET_FREE_GROUPID _IOR(PORTBIND_IOC_MAGIC,6,unsigned long)
#define PORTBIND_IOC_SHOW_GROUP _IOR(PORTBIND_IOC_MAGIC,7,unsigned long)
#define PORTBIND_IOC_LANBIND_CHECK _IOR(PORTBIND_IOC_MAGIC,8,unsigned long)
#define PORTBIND_IOC_MAX_NR 20

#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_PORT_BIND)
#define 	MAX_GROUP_NUM			64
#else
#define 	MAX_GROUP_NUM			16
#endif
#define 	MAX_IFNAME_LST_NUM		(MAX_GROUP_NUM * 6)
#define		MAX_IFTYPE_LST_NUM		(MAX_GROUP_NUM * 6)
#else
#ifdef TCSUPPORT_E8B
#define 	MAX_GROUP_NUM			64
#else
#define 	MAX_GROUP_NUM			16
#endif
#define 	MAX_IFNAME_LST_NUM		128
#define		MAX_IFTYPE_LST_NUM		64
#endif

/* for receive ioctl info from user space */
struct portbindcmdif
{
	int groupid;
	char ifNameLst[MAX_IFNAME_LST_NUM];
	char ifTypeLst[MAX_IFTYPE_LST_NUM];
	int ifNum;
};

int doaddGroup(int argc, char *argv[], void *p);
int dodelGroup(int argc, char *argv[], void *p);
int doshowGroup(int argc, char *argv[], void *p);
int ioctl_pmap(int cmd, void *data, int datealen);

#endif
