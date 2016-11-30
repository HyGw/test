#ifndef SDK_QS_UTILS_H
#define SDK_QS_UTILS_H

#include "vp_api.h"

#define QS_DEBUG printk

#ifdef __KERNEL__      //dyma_130306
#include <linux/slab.h> 
#define PRINT					printk
#define MALLOC(size)				kmalloc(size, GFP_ATOMIC)
#define FREE(ptr)					kfree(ptr)
#else
#define PRINT					printf
#define MALLOC(size)				malloc(size)
#define FREE(ptr)					free(ptr)
#endif /*__KERNEL__*/

extern char*
MapStatus(
    VpStatusType status);

extern void
UtilPrintEvent(
    VpEventType *pEvent);

#endif /* SDK_QS_UTILS_H */
