/********   **********************************************************************/
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
#include "libmtdpartition.h"
#include "libtcapi.h"
#include  "../../public/mtd/tc_partition.h"

int getProlineCwmparaRaSize(){
#ifdef TCSUPPORT_PRODUCTIONLINE
	return PROLINE_CWMPPARA_RA_SIZE;
#else
	return 0;
#endif
}

int getProlineCwmparaRaOffset(){
#ifdef TCSUPPORT_PRODUCTIONLINE
	return PROLINE_CWMPPARA_RA_OFFSET;
#else
	return 0;
#endif
}

unsigned long getBackupRomfileRaSize(){
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT) && !defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
	return BACKUPROMFILE_RA_SIZE;
#else
#if defined(TCSUPPORT_BACKUPROMFILE)
	return BACKUPROMFILE_RA_SIZE;
#endif
	return 0x10000;
#endif
#else
	return 0;
#endif
}

unsigned long getBackupRomfileRaOffset(){
#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT) && !defined(TCSUPPORT_CT_CWMP_ZIPROMFILE)
	return BACKUPROMFILE_RA_OFFSET;
#else
#if defined(TCSUPPORT_BACKUPROMFILE)
	return BACKUPROMFILE_RA_OFFSET;
#endif
	return 0x0;
#endif
#else
	return 0;
#endif

}

unsigned long getDefaultRomfileRaSize(){
#if defined(TCSUPPORT_CT)
#if !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	return DEFAULTROMFILE_RA_SIZE;
#else
	return 0x10000;
#endif
#else
	return 0;
#endif
}

unsigned long getDefaultRomfileRaOffset(){
#if defined(TCSUPPORT_CT)
#if !(defined(TCSUPPORT_CT_E8B_ADSL) && defined(TCSUPPORT_CPU_MT7505))
	return DEFAULTROMFILE_RA_OFFSET;
#else
	return 0x10000;
#endif
#else
	return 0;
#endif
}
