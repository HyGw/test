#ifndef DEBUG_H_
#define DEBUG_H_

/* for timer name debug */
#define EVTIMER_DBG  1

#define MAX_BUF (15 * 1024)

#define WITH_TEST_SERVER 1

typedef enum {
	MOBILE_NONE = 0,
	MOBILE_ERR,
	MOBILE_INFO,
	MOBILE_DEBUG,
	MOBILE_ALL
} DebugLevel;

extern DebugLevel MobileDbg;
extern void mobile_printf(DebugLevel dbg, const char *fmt, ...);

#endif /*DEBUG_H_*/
