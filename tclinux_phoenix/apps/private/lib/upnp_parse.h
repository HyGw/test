#ifndef PARSE_H
#define PARSE_H 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
 
#define UBUF_ALL_STR	1
#define UBUFSIZE 		1024
#define UBUFSIZEADD 	1024
#ifdef UBUF_ALL_STR
#define UBUFLENGTHPOS	2
#define UBUFPARAPOS		6  //opcode(1B)+length(2B)
#define UBUFVALUEPOS	13 //opcode(1B)+length(2B) + dev=01&
#else
#define UBUFLENGTHPOS	1
#define UBUFPARAPOS		3
#define UBUFVALUEPOS	10 //opcode(1B)+length(2B) + dev=01&
#endif

typedef enum opcode_en {
	OP_ACT_START = 0,
	OP_BMS_REBOOT,
	OP_BMS_BASELINERESET,
	OP_BMS_GETDEVICESTATUS,
	OP_BMS_DOWNLOAD,
	OP_BMS_GETDOWNLOADSTATUS,
	OP_CMS_GETVALULE, //6
	OP_CMS_SETVALULE, //7
	OP_CMS_GETATTRIBUTES,
	OP_CMS_SETATTRIBUTES,
	OP_CMS_GETINSTANCES, //a 
	OP_CMS_CREATEINSTANCES,
	OP_CMS_DELETEINSTANCES,
	OP_CMS_GETNAMES,
	OP_ACT_RES_START, //e
	OP_BMS_REBOOT_RES,
	OP_BMS_BASELINERESET_RES,
	OP_BMS_GETDEVICESTATUS_RES,
	OP_BMS_DOWNLOAD_RES,
	OP_BMS_GETDOWNLOADSTATUS_RES,
	OP_CMS_GETVALULE_RES,
	OP_CMS_SETVALULE_RES,
	OP_CMS_GETATTRIBUTES_RES,
	OP_CMS_SETATTRIBUTES_RES,
	OP_CMS_GETINSTANCES_RES,
	OP_CMS_CREATEINSTANCES_RES,
	OP_CMS_DELETEINSTANCES_RES,
	OP_CMS_GETNAMES_RES,
	OP_ADD_DEVICE,
	OP_DEL_DEVICE,
	OP_OPRATION_DONE
}enOpcode;

typedef struct u_buffer
{
	unsigned int length;
	unsigned int size;
	char *buf;
}u_buf;

typedef struct para_buf
{
	unsigned int length;
	char *para;
	struct para_buf *next;
}u_para;

typedef struct
{
	unsigned char opcode;
	unsigned char num;
	unsigned int length;
	struct para_buf* paralist;
}u_packet;

extern int get_devnum(u_buf* ubuffer);
extern int get_opcode(u_buf* ubuffer);
extern int get_length(u_buf* ubuffer);
extern int parse_u_buf(char *buffer, int *pdevnum, int *popcode, unsigned int *plength);
extern u_buf* add_to_ubuf(u_buf** upbuffer, unsigned char opcode, unsigned char devnum, char *paraname, char *paravalue);
extern void free_ubuf(u_buf** upbuffer);
extern void print_ubuf(u_buf* ubuffer);
extern u_packet* new_u_packet(unsigned int length, unsigned int num);
extern u_para* new_u_para(unsigned int length, char *value);
extern void free_u_packet(u_packet** upak);
extern char* print_u_packet(u_packet *upak);
extern u_packet* parse_ubuf(char* ubuf);

#endif /* PARSE_H */

