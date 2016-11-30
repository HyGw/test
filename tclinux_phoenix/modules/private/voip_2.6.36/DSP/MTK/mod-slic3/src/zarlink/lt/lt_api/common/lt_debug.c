/** \file lt_debug.c
 * lt_api.c
 *
 *  This file contains the implementation of LT_DEBUG MACRO.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7089 $
 * $LastChangedDate: 2010-05-05 23:25:12 -0500 (Wed, 05 May 2010) $
 */

#include "lt_api.h"
#include "lt_api_internal.h"
#ifdef LT_DEBUG_H
  #include "vp_api.h"
  #ifdef LT_DEBUG
    uint32 gLtDbgFlag = LT_DBG_DFLT;
  #endif
#endif
