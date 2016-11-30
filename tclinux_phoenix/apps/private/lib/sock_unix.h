#ifndef SOCK_UNIX_H
#define SOCK_UNIX_H 

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include "upnp_parse.h"
 
#define UPNP_MQ_PATH "/tmp/upnp_msgq"
#define UPNP_MQ_TYPE 88

#define UPNP_PATH "/tmp/upnp"
#define CMD_MAX_LENGTH 2048

#define SOCK_UNIX_TIMEOUT 5

typedef struct upnpmsg{
	char UpnpType;
	char MsgText[7];
} upnp_msg_t;

typedef struct upnpmsgq{
	long MsgType;
	upnp_msg_t text;
} upnp_msgq_t;


extern int create_socket_client(void);
extern int send_u_buf(int sockfd, u_buf* ubuffer);
extern int send_u_bufwithRes(int sockfd, u_buf* ubuffer , char **resBuf);
extern int send_message(long type, int msgFlag, u_buf* ubuffer);

extern int init_upnp_msgq(void);
extern int CTupnpReadmsgQ(upnp_msg_t *msg);
extern int CTupnpSendmsgQ(upnp_msg_t text);
#endif /* SOCK_UNIX_H */

