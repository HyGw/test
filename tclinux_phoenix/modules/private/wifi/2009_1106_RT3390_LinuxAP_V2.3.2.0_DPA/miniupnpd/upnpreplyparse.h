/* $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/modules/private/wifi/2009_1106_RT3390_LinuxAP_V2.3.2.0_DPA/miniupnpd/upnpreplyparse.h#1 $ */
/* MiniUPnP project
 * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
 * (c) 2006 Thomas Bernard 
 * This software is subject to the conditions detailed
 * in the LICENCE file provided within the distribution */

#ifndef __UPNPREPLYPARSE_H__
#define __UPNPREPLYPARSE_H__

#if defined(NO_SYS_QUEUE_H) || defined(WIN32)
#include "bsdqueue.h"
#else
#include <sys/queue.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct NameValue {
    LIST_ENTRY(NameValue) entries;
    char name[64];
    char value[64];
};

struct NameValueParserData {
    LIST_HEAD(listhead, NameValue) head;
    char curelt[64];
};

/* ParseNameValue() */
void
ParseNameValue(const char * buffer, int bufsize,
               struct NameValueParserData * data);

/* ClearNameValueList() */
void
ClearNameValueList(struct NameValueParserData * pdata);

/* GetValueFromNameValueList() */
char *
GetValueFromNameValueList(struct NameValueParserData * pdata,
                          const char * Name);

/* GetValueFromNameValueListIgnoreNS() */
char *
GetValueFromNameValueListIgnoreNS(struct NameValueParserData * pdata,
                                  const char * Name);

/* DisplayNameValueList() */
#ifdef DEBUG
void
DisplayNameValueList(char * buffer, int bufsize);
#endif

#ifdef __cplusplus
}
#endif

#endif

