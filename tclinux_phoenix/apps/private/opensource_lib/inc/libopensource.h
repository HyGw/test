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
#ifndef _OPENSOURCE_LIB_H
#define _OPENSOURCE_LIB_H

void
init_test_data(int index);
int get_default_iface(char *ifname );
unsigned int get_IMG_BOOT_FLAG_OFFSET();
unsigned int get_IMG_COMMIT_FLAG_OFFSET();

#endif
