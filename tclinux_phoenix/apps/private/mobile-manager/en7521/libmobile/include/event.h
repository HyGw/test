#ifndef EVENT_H_
#define EVENT_H_

#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <sys/epoll.h>

#include "list.h"
#include "debug.h"

typedef enum event_cmd {
	EVENT_EPOLL_ADD,	// Add an epoll handler (auto-removed on close())
	EVENT_EPOLL_DEL,	// Remove an epoll handler
	EVENT_EPOLL_MOD,	// Modify an epoll handler
	EVENT_SIGNAL_ADD,	// Add a signal handler (multiple per signal possible)
	EVENT_SIGNAL_DEL,	// Remove a signal handler
	EVENT_TIMER_ADD,	// Add a timer
	EVENT_TIMER_MOD,	// Modifies a timer
	EVENT_TIMER_DEL,	// Removes a timer
}event_cmd_e;

typedef enum event_trigger {
	EVENT_NOW,			// Run event handlers immediately
	EVENT_QUEUE,		// Run event handlers on next iteration
}event_trigger_e;

/* event for EVENT_EPOLL_ADD, EVENT_EPOLL_DEL, EVENT_EPOLL_MOD */
typedef struct event_epoll {
	/* mandatory input */
	int fd;
	int queueindex;
	uint32_t events;
	void (*handler)(struct event_epoll*, uint32_t revents);
	/* optional input */
	void *context;
}event_epoll_t;

/* event for EVENT_SIGNAL_ADD, EVENT_SIGNAL_DEL */
typedef struct event_signal {
	/* mandatory input */
	void (*handler)(struct event_signal*, const siginfo_t *siginfo);
	int signal;
	/* optional input */
	void *context;
	/* internal */
	struct list_head _head;
}event_signal_t;

/* event for EVENT_TIMER_ADD, EVENT_TIMER_MOD, EVENT_TIMER_DEL */
typedef struct event_timer {
	/* mandatory input */
	int value;		// msec: current timer value (one-shot / first trigger/minus not start yet)
	unsigned interval;	// msec: following timer values (interval) (0 = off)
	void (*handler)(struct event_timer*, int64_t now);
	/* optional input */
	void *context;
	#if EVTIMER_DBG
	char *name;
	#endif
	/* internal */
	int64_t _next; 		//next timeout 
	struct list_head _head;
}event_timer_t;

#define MAX_EVENTS 32
typedef struct event_ctrl {
	int fd_epoll;
	int epoll_evlen;
	int event_exit;
	int event_wait;
	struct list_head list_signal;
	struct list_head list_timer;
	struct epoll_event epoll_ev[MAX_EVENTS];
	sigset_t sig_default;
}event_ctrl_t;


// Initialise event system
int event_init(event_ctrl_t *pev_ctl);
// Deinitialise event system
void event_deinit(event_ctrl_t *pev_ctl);
// Register / unregister struct event_*
int event_ctl(event_ctrl_t *pev_ctl, event_cmd_e cmd, void *event);
// Trigger a custom signal / emulate a system signal
int event_trigger(siginfo_t *siginfo, event_trigger_e how);
// Main loop
int event_run(event_ctrl_t *pev_ctl);
// Stop the main loop after the current iteration and set return code
int event_stop(event_ctrl_t *pev_ctl) ;
// Set close-on-exec flag on fd and return it
int event_cloexec(int fd);
// Set non-blocking flag on fd and return it
int event_nonblock(int fd);
// Get the current time of the monotonic (not realtime) clock in milliseconds
int64_t event_time();


#endif /* EVENT_H_ */
