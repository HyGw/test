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
 #ifndef _TCETHERCMD_H_
#define _TCETHERCMD_H_
 int doEther(int argc, char *argv[], void *p);
 int doIp(int argc, char *argv[], void *p);
 int doSet(int argc, char *argv[], void *p);
 int doDriver(int argc, char *argv[], void *p);
 int doAddr(int argc, char *argv[], void *p);
 int doLan(int argc, char *argv[], void *p);
 int doIpaddr(int argc, char *argv[], void *p);
 int doCnt(int argc, char *argv[], void *p);
 int doDisp(int argc, char *argv[], void *p);
 int doClear(int argc, char *argv[], void *p);
void tcethercmd(void);
#endif

