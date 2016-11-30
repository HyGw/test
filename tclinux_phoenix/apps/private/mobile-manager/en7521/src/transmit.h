#ifndef TRANSMIT_H_
#define TRANSMIT_H_
#include <stddef.h>
#include <stdint.h>

#include "event.h"
#include "jsonpkt.h"

#define SOCK_CREATE_ERR		-1
#define SOCK_CREATE_OK		0
#define SOCK_MAXTRY_ERR 	1

#define PACKET_NORMAL		0
#define PACKET_HB			1

typedef void (trans_cb)(trans_pkt_t *pkt, void *context, int sock, int len);
typedef int (send_cb)(void *context);
typedef int (trans_restart)(int index, void *context, int prd);

typedef struct resend_ctrl_data{
	uint16_t tries_max;
	uint16_t responsePrd;		//wait period for respone	
	uint16_t resendPrd;		//resend period if no response 	
	uint16_t sendPrd;    		//normal send pkt period	
}resend_ctrl_t;

typedef struct trans_req {
	trans_pkt_t *packet;
	send_cb *sendcb;
	void *ctx;
	int fd;
	int32_t timeout;
	uint16_t tries_num;
	resend_ctrl_t *send_ctrl;
}trans_req_t;

#define TRANS_SOCKET_NUM		2  //for normal and heartbeat
#define TRANS_QUEUE_NUM		2
#define TRANS_STATE_NUM_DISTR		2
#define TRANS_STATE_NUM_OPERATE	5
#define TRANS_STATE_NUM_PLUGIN		4

#define TRANS_STATE_BOOT	0
#define TRANS_STATE_REG		1
#define TRANS_STATE_HB		2
#define TRANS_STATE_REQ		3
#define TRANS_STATE_STOP	4  /* add for stop for reuqest plugin or distrbuit when key err*/

typedef struct trans_cluster {
	event_ctrl_t *pev_ctrl;
	event_epoll_t socket[TRANS_SOCKET_NUM]; //for normal and heartbeat
	trans_req_t queue[TRANS_QUEUE_NUM];
	event_timer_t timer;
	event_timer_t call_evtimer;
	struct addrinfo *serveraddr;	
	char	serverport[8];
	int	socktype;	
	int	current;
	int	count;
	int	states;	
	int	queuwnum;
	int	retries;
	void *savepkt;
	trans_cb *cb;
	trans_restart *restart;	
	resend_ctrl_t	*ctrl_data;	
	struct trans_cluster* next;
}trans_cluster_t;

typedef struct private_data{
	uint8_t tries_max;
	uint16_t sendPrd;    	//normal send pkt period
	uint16_t responsePrd;
	uint16_t resendPrd;	
}private_data_t;

typedef enum trans_restart_cmd{
	RESTART_NEW = 0,
	RESTART_TRY_MAX,
	RESTART_HBTRY_MAX, //notice:HBTRY must follow with TRY
	RESTART_HBNEW,
	RESTART_HBRENEW
}restart_cmd_e;

void trans_receiver(event_epoll_t *ev, uint32_t events);
int trans_reconfigure(trans_cluster_t *cl, int index, const char *ifname, const char *port);
int trans_request(trans_cluster_t *cl, int fd, int pkt_index, trans_pkt_t *pkt);
int trans_request_ctrl(trans_cluster_t *cl, int pkt_index, trans_pkt_t *pkt, send_cb *cb, void *ctx);
void update_hb_timer(trans_cluster_t *cl);

#endif /* TRANSMIT_H_ */

