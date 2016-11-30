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
#ifndef _ATMCMDD_H
#define _ATMCMDD_H

#define ATMCMD_SOCK_PATH 	"/tmp/atmcmd_sock"

#define ATMCMD_MAX_CLIENT	5

/*ATMCMD OP_CMD */
#define ATMCMD_CREATE_PVC	0
#define ATMCMD_CLOSE_PVC	1

/*ATMCMD return message*/
#define ATMCMD_OK			0
#define ATMCMD_FAIL			-1

typedef struct atmcmd_msg {
	unsigned int	op_cmd; 
	/*
		0:sucess, -1:fail
	*/
	int	retval;

  	int itfnum;
  	int vpi;
  	int vci;
  	int encap;
  	int sndbuf;
  	int payload;
  	int qos_type; 
  	int pcr; 
  	int scr; 
  	int mbs; 
} atmcmd_msg_t;

#endif
