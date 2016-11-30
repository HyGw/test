/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdarg.h>
#include "../cmd_ci/atmcmdd.h"

void send2atmcmdd(atmcmd_msg_t *msg)
{
	int sockfd=0, len=0;
	struct sockaddr_un remote;

	msg->retval = ATMCMD_FAIL;

	if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return;
	}

	remote.sun_family = AF_UNIX;
	strcpy(remote.sun_path, ATMCMD_SOCK_PATH);
	len = strlen(remote.sun_path) + sizeof(remote.sun_family);
	if (connect(sockfd, (struct sockaddr *)&remote, len) == -1) {
		perror("connect");
		close(sockfd);
		return;
	}

	if (send(sockfd, msg, sizeof(atmcmd_msg_t), 0) == -1){
		perror("send");
		close(sockfd);
		return;
	}

	memset(msg, 0, sizeof(atmcmd_msg_t));

	if ((recv(sockfd, msg, sizeof(atmcmd_msg_t), 0)) <= 0 ) {
		/*Failure to receive information from cfg manage*/
		perror("recv");
		close(sockfd);
		return;
	}
	close(sockfd);
}/*end send2atmcmdd */

int atmcmdc_create_pvc(int itfnum, int vpi, int vci, int encap, int payload,
		int qos_type, int pcr, int scr, int mbs, int sndbuf)
{
	atmcmd_msg_t msg;

	memset(&msg, 0, sizeof(msg));

	/*Fill information*/
	msg.op_cmd = ATMCMD_CREATE_PVC;
	msg.itfnum = itfnum;
	msg.vpi = vpi;
	msg.vci = vci;
	msg.encap = encap;
	msg.payload = payload;
	msg.qos_type = qos_type;
	msg.pcr = pcr;
	msg.scr = scr;
	msg.mbs = mbs;
	msg.sndbuf = sndbuf;

	send2atmcmdd(&msg);
	return msg.retval;
}

int atmcmdc_close_pvc(int vpi, int vci)
{
	atmcmd_msg_t msg;

	memset(&msg, 0, sizeof(msg));

	/*Fill information*/
	msg.op_cmd = ATMCMD_CLOSE_PVC;
	msg.vpi = vpi;
	msg.vci = vci;

	send2atmcmdd(&msg);
	return msg.retval;
}

