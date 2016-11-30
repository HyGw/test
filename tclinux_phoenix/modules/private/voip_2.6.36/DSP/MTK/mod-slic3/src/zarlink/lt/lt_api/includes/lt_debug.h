/** \file lt_debug.h
 * lt_debug.h
 *
 * This file contains the configuration and compile time settings for
 * the Line Test API debug statements.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7401 $
 * $LastChangedDate: 2010-06-23 15:15:43 -0500 (Wed, 23 Jun 2010) $
 */

#ifndef LT_DEBUG_H
#define LT_DEBUG_H
#include "lt_api.h"

#define printf printk
#ifdef LT_DEBUG
  #include <stdio.h>
  /* LT-API Debug flags: try to keep flags all same length */
  #define LT_DBG_ERRR    0x00000001 /* Any error condition */
  #define LT_DBG_WARN    0x00000002 /* Any warning condition */
  #define LT_DBG_INFO    0x00000004 /* Un-categorized information */
  #define LT_DBG_FUNC    0x00000008 /* Any function */
  #define LT_DBG_ALL     0x0000000F /* Include all debug */

  #define LT_DBG_DFLT   LT_DBG_ALL

  #define LT_DOUT(flag, strings)    \
    if ((flag) & (gLtDbgFlag))       \
    {                               \
        printf(" [%s]", #flag);     \
        printf strings;            \
        printf("\n");               \
    }
#else
 #define LT_DOUT(flag, strings)
#endif


EXTERN uint32 gLtDbgFlag; /* Decleration for the default flag variable */

#endif
