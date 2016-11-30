#ifndef _EXEC_H
#define _EXEC_H

#define EXEC_SOCK_PATH  "/tmp/exec_sock"

/*EXEC return message*/
#define EXEC_PROCESS_OK	0
#define EXEC_CONN_ERR	-3

#define MAXLEN_EXEC_CMD	512

typedef struct
exec_msg{
	/*	0:sucess,         */
	int	retval;
	char op_cmd;
	char exec_cmd[MAXLEN_EXEC_CMD];
} exec_msg_t;

int send2Exec(char cmd, char *buffer);

#endif
