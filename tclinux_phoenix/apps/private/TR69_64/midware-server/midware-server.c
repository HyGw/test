/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../../lib/libtcapi.h"

#define BACKLOG 10
//#define E8_PATH "/tmp/interface2sock"
#define E8_PATH "/var/ct/tmp/interface2sock"

#define file_path "/tmp/sockbuf"
#define N 2048

typedef unsigned char uint8;
typedef unsigned int uint16;

typedef struct midware_cmd
{
	uint8 type;
	uint16 length;
	char *value;
	struct midware_cmd *next;
}midware_cmd;

typedef struct
{
	uint8 opcode;
	uint8 number;
	struct midware_cmd* mcmd;
}midware_packet;

typedef struct
{
	char *buf;
	int length;
}buf_t;

int cli_sock;

char temp_buf[N];
buf_t inbuf;
buf_t outbuf;

void cmd_printf(char *buf);


void tc3162_console_dbg(char * ptr)
{
	FILE *proc_file;
    proc_file = fopen("/proc/tc3162/dbg_msg", "w");
	if (!proc_file) {
		printf("open /proc/tc3162/dbg_msg fail\n");
		return;
	}
	fprintf(proc_file, "%s", (char *)ptr);
	fclose(proc_file);	
}

#define DBGONCECNT 64
void tc_printf(char *fmt,...)
{
	int len = 0;
	int index = 0;
	char msg[2560];
	char *ptr = NULL;
	char tmp = 0;
		
	va_list args;

	va_start(args, fmt);
	vsprintf(msg, fmt, args);

	len = strlen(msg);
	ptr = msg;
	index = 0;
	while(len > 0)
	{
		if(len <= DBGONCECNT)
		{
			tc3162_console_dbg((char *)(ptr+index));
			//fprintf(proc_file, "%s", (char *)(ptr+index));
			len = 0;
		}
		else
		{
			tmp = ptr[index+DBGONCECNT];
			ptr[index+DBGONCECNT] = 0;
			tc3162_console_dbg((char *)(ptr+index));
			//fprintf(proc_file, "%s", (char *)(ptr+index));
			ptr[index+DBGONCECNT] = tmp;
			len -= DBGONCECNT;
			index += DBGONCECNT;
		}			
	
	}
	va_end(args);
}

uint8 *put16(uint8 *x, uint16 cp)
{
	*x++ = cp >> 8;
	*x++ = cp;

	return x;
}

int get_pktLen(midware_packet *mp)
{
	int count = 0;
	midware_cmd *mc = NULL;
	
	count += 2;
	//printf("count00:%d\n",count);
	if(mp->mcmd == NULL)
	{
		return count;
	}

	mc = mp->mcmd;
	while(mc !=NULL)
	{
		//printf("count00:%d\n",count);
		//count += sizeof(mc->type)+sizeof(mc->length);
		count += 3;
		//printf("count11:%d\n",count);
		count +=mc->length;
		//printf("count22:%d\n",count);
		mc = mc->next;
	}

	return count;
}

midware_packet * new_mwPkt(uint8 opcode, uint8 number)
{
	midware_packet *mp=NULL;
	mp = (midware_packet *)malloc(sizeof(midware_packet));
	memset(mp,0,sizeof(midware_packet));
	if(NULL==mp)
	{
		printf("error:allocate memory for midware_packet fail!\n");
		return NULL;
	}

	mp->opcode = opcode;
	mp->number = number;
	mp->mcmd = NULL;
	return mp;
}

midware_cmd * new_mwCmd(uint8 type, uint16 length, char *value)
{
	midware_cmd *mc=NULL;
	if(value == NULL)
		return mc;
	
	mc = (midware_cmd *)malloc(sizeof(midware_cmd));
	memset(mc,0,sizeof(midware_cmd));
	if(NULL==mc)
	{
		printf("error:allocate memory for midware_cmd fail!\n");
		return NULL;
	}
	
	mc->type = type;
	mc->length = length;
	mc->value = value;
	mc->next = NULL;

	return mc;
}

char * new_value(uint16 length)
{
	char * value = NULL;
	value = (char *)malloc(length);
	memset(value,0,length);
	if(NULL==value)
	{
		printf("error:allocate memory for value fail!\n");
		return NULL;
	}
	return value;
}

buf_t* new_buf_t(int length)
{
	buf_t* bufT = NULL;
	bufT = (buf_t *)malloc(sizeof(buf_t));
	memset(bufT,0,sizeof(buf_t));
	if(NULL==bufT)
	{
		printf("error:allocate memory for buf_t fail!\n");
		return NULL;
	}
	bufT->buf = NULL;
	bufT->length = length;
	return bufT;
}

void free_mwCmd(midware_cmd** mc)
{
	midware_cmd* tmp = *mc;
	midware_cmd * pre = NULL;
	if(NULL==tmp)
		return;
	 
	while(NULL!=tmp)
	{
		if(NULL!=tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		pre = tmp;
		tmp = tmp->next;
		pre->next = NULL;
		free(pre);
	}
	*mc = NULL;
}

void free_mwPkt(midware_packet** mp)
{
	midware_packet* tmp = *mp;
	if(NULL==tmp)
		return;
	
	if(NULL==tmp->mcmd)
	{
		free(tmp);
		tmp = NULL;
		return;
	}
	
	free_mwCmd(&tmp->mcmd);
	free(tmp);
	*mp = NULL;
}

void free_buf_t(buf_t** bufT)
{
	buf_t *tmp = *bufT;
	if(NULL!=tmp->buf)
	{
		free(tmp->buf);
		tmp->buf = NULL;
	}
	free(tmp);
	tmp = NULL;
	*bufT = NULL;
}

int get_minNum(int a, int b)
{
	return a>b?b:a;
}

void get_cmdBuf(char *buf, midware_cmd *mc, int length)
{
	int pos=0;
	uint8 *temp=NULL;
	//printf("pos0:%d\n",pos);
	while(mc!=NULL)
	{
		buf[pos] = mc->type;
		
		pos++;
		temp = buf+pos;
		temp = put16(temp, mc->length);
		//printf("pos1:%d\n",pos);
		pos += 2;
		//printf("pos2:%d\n",pos);
		length = length-3;
		strncpy(buf+pos, mc->value,mc->length);
			
		pos += mc->length;
		//printf("pos3:%d\n",pos);
		length = length-mc->length;
		mc = mc->next;
	}
	//printf("\n");
}

buf_t * midwarePkt2buf(midware_packet *mp)
{
	buf_t *bufT = NULL;
	int pos=0;
	int len = get_pktLen(mp)-sizeof(mp->opcode)-sizeof(mp->number);

	bufT = new_buf_t(len);

	if(0==len)
		return bufT;
	
	if(NULL==bufT)
	{
		printf("error:allocate memory for buf_t fail!\n");
		return NULL;
	}
	
	bufT->buf = new_value(len+1);
	
	if(NULL==bufT->buf)
	{
		printf("error:allocate memory for buf_t's buf fail!\n");
		return NULL;
	}
	
	if(mp->mcmd!=NULL)
		get_cmdBuf((bufT->buf), mp->mcmd, len);
	
	return bufT;
}

void print_mwPkt(midware_packet *mp)
{
	midware_cmd *mc = NULL;
	if(NULL == mp){
		printf("NULL!\n");
		return;
	}

	if(0 == mp->number){
		tc_printf("%d %d!\n",mp->opcode,mp->number);
	}
	
	if(mp->number >= 1){
		tc_printf("%d %d ",mp->opcode,mp->number);
		mc = mp->mcmd;
		while(mc != NULL){
			tc_printf("%d %d %s ",mc->type,mc->length,mc->value);
			mc = mc->next;
		}
		tc_printf("\n");
	}
}


static void send_buf(int sig)
{
	FILE *fp;
	char cmd[5]={0};
	midware_packet *in =NULL;
	midware_cmd *temp  =NULL;
	int num;
	char *pt=NULL;
	buf_t *buffer=NULL;
	buf_t *buffer2=NULL;
	
	char *value=NULL;
	char *value2=NULL;
	uint8 opcode;
	uint8 number;
	uint8 type;
	uint16 length;
	
	int i=0;
	
	if((fp=fopen(file_path,"r")) ==NULL)
	{
		printf("open file error");
		return;
	}
	fgets(inbuf.buf,N,fp);
	inbuf.length = strlen(inbuf.buf);
	//printf("inbuf.length:%d\n",inbuf.length);
	inbuf.length -= 6;
	
	//printf("inpuf.buf:%s\ninpuf.length=%d\n",inbuf.buf, inbuf.length);
	fclose(fp);
	
	pt=inbuf.buf;
	
	memset(cmd, 0, 5);
	strncpy(cmd, pt, 2);
	pt += 2;
	sscanf(cmd, "%x", &num);
	opcode = num;
	//printf("opcode:%d\n",opcode);

	memset(cmd, 0, 5);
	strncpy(cmd, pt, 2);
	pt += 2;
	sscanf(cmd, "%x", &num);
	number = num;
	//printf("number:%d\n",number);
	
	in = new_mwPkt(opcode,number);
	for(i=0;i<number;i++){
		memset(cmd, 0, 5);
		strncpy(cmd, pt, 2);
		pt += 2;
		sscanf(cmd, "%x", &num);
		type = num;
		//printf("type:%d\n",type);
	
		memset(cmd, 0, 5);
		strncpy(cmd, pt, 4);
		pt += 4;
		sscanf(cmd, "%x", &num);
		length = num;
		//printf("length:%d\n",length);
	
		value = new_value(length+1);
		strncpy(value,pt,length);
		pt += length;
	
		if(in->mcmd == NULL){
			in->mcmd = new_mwCmd(type,length,value);
			temp = in->mcmd;
		}
		else{
			temp->next = new_mwCmd(type,length,value);
			temp = temp->next;
		}
		//printf("value:%s\n",value);
	}
	
	buffer=midwarePkt2buf(in);
	
	value2 = new_value(buffer->length+3);
	value2[0] = (char)(in->opcode);
	value2[1] = (char)(in->number);
	memcpy(value2+2,buffer->buf,buffer->length);
	buffer2 = new_buf_t(buffer->length+3);
	buffer2->buf = value2;
	buffer2->length = buffer->length+2;
	
	if(send(cli_sock,buffer2->buf, buffer2->length,0) == -1){
		printf("send error\n");
	}
	else{
		print_mwPkt(in);		
	}
	
	if(buffer != NULL)
		free_buf_t(&buffer);
	if(in != NULL)
		free_mwPkt(&in);
	if(buffer2 != NULL)
		free_buf_t(&buffer2);

}



int main()
{
	int serv_sock,addr_len,len;
	struct sockaddr_un local;
	struct sockaddr_un remote;
	int flags,ret;
	int recvlen=0;
	void send_buf(int);
	int i=0,j=0,k=0;
	
	inbuf.buf = temp_buf;
	inbuf.length = N;
	outbuf.buf = temp_buf;
	outbuf.length = 0;
	memset(outbuf.buf,0,N);
	addr_len = sizeof(remote);
	if((serv_sock=socket(AF_UNIX,SOCK_STREAM,0)) == -1){
		tc_printf("midware-server:socket error\n");
		close(serv_sock);
		exit(0);
	}
	bzero(&local,sizeof(local));
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, E8_PATH);
	len = strlen(local.sun_path)+sizeof(local.sun_family);
	unlink(local.sun_path);
	flags = fcntl(serv_sock,F_GETFL,0);
	fcntl(serv_sock,F_SETFL,flags|O_NONBLOCK);

	if((ret=bind(serv_sock,(struct sockaddr *) &local,len)) == -1){
		tc_printf("midware-server:bind error\n");
		close(serv_sock);
		exit(0);
	}
	if(listen(serv_sock,BACKLOG) == -1){
		tc_printf("midware-server:listen error\n");
		close(serv_sock);
		exit(0);
	}
	
	while(1){
		if((cli_sock = accept(serv_sock, (struct sockaddr *) &remote, &addr_len)) == -1){
			tc_printf("midware-server:accept error\n");
			sleep(10);
			close(cli_sock);
			continue;
			}
		else {
			tc_printf("cli_sock=%d\nmidware server: accepted!!!!!\n",cli_sock);
			break;
		}
	}

	while(1){
		signal(SIGUSR1,send_buf);
		memset(outbuf.buf,0,N);
		outbuf.length = 0;
		recvlen=recv(cli_sock,outbuf.buf,N,0);
		tc_printf("midware-server:recvlen=%d\n",recvlen);
		if( recvlen== -1)
		{
			tc_printf("midware-server:recv error\n");
			continue;
		}
		else
		{
			outbuf.length=recvlen;
			if(outbuf.length != 0)
			{
				/*
				tcdbg_printf("midware server recv: ");
				for(i=0;i<5;i++)
					tcdbg_printf("%d ",outbuf.buf[i]);
				for(i=5;i<13;i++)
					tcdbg_printf("%c",outbuf.buf[i]);
				for(i=13;i<16;i++)
					tcdbg_printf("%d ",outbuf.buf[i]);
				for(i=16;i<outbuf.length;i++)
					tcdbg_printf("%c",outbuf.buf[i]);
				tcdbg_printf("\n");
				*/
				//tcdbg_printf("midware server recv: ");
				//cmd_printf(outbuf.buf);
			}
		//	else
			//	printf("outbuf.length==0!\n");
		}
		
		sleep(1);
	 }
	close(cli_sock);
}

void cmd_printf(char *buf){
	int i = 0,Num = 0;
	uint16 type,length;
	char Obuf[1024];
	
	//tcdbg_printf("Opcode=%d,%x,",*buf,*buf);
	buf++;
	Num = *buf++;
	for(i=0; i<Num; i++){
		type = *buf++;
		length = *buf++;
		length = (length<<8) + *buf++;
		strncpy(Obuf,buf,length);
		Obuf[length] = '\0';
		//tcdbg_printf("type=%d,length=%d,",type,length);
		//tc_printf("value=%s!\n",Obuf);
		buf += length;		
	}	
}
