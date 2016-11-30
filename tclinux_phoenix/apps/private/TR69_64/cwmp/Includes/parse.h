#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdio.h>

#define MAX_PACKET_LEN 2048

#define MW_PKT_DONE			1
#define MW_PKT_ONE_DONE		2
#define MW_PKT_NUMS			3  //get numbers packet
#define MW_PKT_TWO			4  //packet length > 2048
#define MW_PKT_INIT			0

//opcode define
#define REGISTER 61
#define REGISTER_OK 62
#define PARAMETER_SET 63
#define PARAMETER_SET_RET 64
#define PARA_INFORM_MW 65
#define PARA_INFORM_MW_RET 66
#define PARAMETER_GET 67
#define PARAMETER_GET_RET 68
#define PARA_ATTRIBUTE_SET 69
#define PARA_ATTRIBUTE_SET_RET 70
#define PARA_ATTRIBUTE_GET 71
#define PARA_ATTRIBUTE_GET_RET 72
#define PARA_INFORM_ITMS 73
#define PARA_INFORM_ITMS_RET 74
#define DOWNLOAD 75
#define DOWNLOAD_RET 76
#define REBOOT 77
#define OPERATION_DONE 78
#define UPLOAD 79
#define UPLOAD_RET 80
#define ADD_OBJECT 81
#define ADD_OBJECT_RET 82
#define DELETE_OBJECT 83
#define DELETE_OBJECT_RET 84
#define GET_PARA_NAMES 85
#define GET_PARA_NAMES_RET 86
#define SET_DEFAULT 87
#define SET_DEFAULT_RET 88
#define MW_EXIT 89
#define MW_EXIT_RET 90

//type define
#define MOD_NAME 0
#define PARA_NAMES 4
#define PARA_VALUES 5
#define RET_CODE 7
#define PARA_ATTRIBUTES 8
#define TWM 9
#define OBJECT 12
#define INSTANCE 13
#define PARA_LIST 15
#define OPERATION 16

//retcode define
//success
#define SUCCESS_200 200 //success
#define SUCCESS_202 202 //partial success
#define SUCCESS_203 203 //success after reboot
//fail
#define FAIL_400 400 //fail
#define FAIL_402 402 //login error
#define FAIL_405 405 //connect server error
#define FAIL_407 407 //no file in server
#define FAIL_412 412 //no parameter
#define FAIL_420 420 //packet more than 2048
#define FAIL_424 424 //parameter value is invalid or illegal
#define FAIL_425 425 //try to write parameter which is not writable
#define FAIL_426 426 //not configurable
#define FAIL_427 427 //not support

typedef struct midware_cmd
{
	uint8 type;
	uint16 length;
	char *value;
	struct midware_cmd *next;
}midware_cmd;

typedef struct midware_packet
{
	uint8 opcode;
	uint8 number;
	struct midware_cmd* mcmd;
	struct midware_packet *next;
}midware_packet, *midware_Ptr;

 typedef struct
 {
 	char *buf;
 	int length;
 }buf_t;
 
uint16 get16(uint8 *cp);
midware_packet * new_mwPkt(uint8 opcode, uint8 number);
midware_cmd * new_mwCmd(uint8 type, uint16 length, char *value);

void free_mwCmd(midware_cmd ** mc);

midware_packet *buf2midwarePkt(const char *buf, int length);
int get_cmd(midware_packet *mp, const char *buf, int length);
void print_mwPkt(midware_packet * mp);

uint8 *put16(uint8 *x, uint16 cp);
int get_pktLen(midware_packet *mp);
int get_minNum(int a, int b);
void get_cmdBuf(char *buf, midware_cmd *mc, int length);
buf_t * midwarePkt2buf(midware_packet *mp);

extern buf_t *new_buf_t(int length);
extern char *new_value(uint16 length);
extern void free_mwPkt(midware_packet ** mp);
extern void free_buf_t(buf_t ** bufT);
extern midware_packet *parse_packet(const char *buf, int length);
extern void respSend(int length, midware_packet *mp);
#if 0 
extern int midware2tr69node(char *inmid,char *out);
#endif
midware_packet *respOperation(uint8 op);
midware_packet * respDownloadUpload(int ret, uint8 flag);

#endif
