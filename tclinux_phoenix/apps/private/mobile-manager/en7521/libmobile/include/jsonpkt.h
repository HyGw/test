#ifndef JSON_PKT_H_
#define JSON_PKT_H_
#include <stddef.h>
#include <stdint.h>
#include "json.h"

#define PKT_DISTRI_LEN  40
//working states for gateway
#define ST_DISTR_INIT		(0)
#define ST_DISTR_BOOT		(10)
#define ST_DISTR_REG		(11)

#define ST_OPERATE_BOOT		(20)
#define ST_OPERATE_REG		(21)
#define ST_OPERATE_RUNNING	(22)

#define ST_PLUGIN_BOOT		(30)
#define ST_PLUGIN_REG		(31)
#define ST_PLUGIN_RUNNING	(32)


//result from server
#define RES_OK_NEW		(3)
#define RES_OK_VERIFY		(2)
#define RES_OK_DevRND		(1)
#define RES_OK_GENERAL		(0)
#define RES_ERR_GENERAL		(-1)
#define RES_ERR_VERIFY		(-2)
#define RES_REGISTER		(-3)
#define RES_TIMEOUT			(-4)
#define RES_RECONECT		(-5)
#define RES_ERR_OTHER		(-10)
#define RES_NO_RESULT		(-11)

//define myself result
#define RES_OK_PLUGIN		(10)
#define RES_OK_DISTRIBUT		(11)


typedef enum jspkt_index{
	JSPKT_DIS_BOOT = 0,
	JSPKT_DIS_REG,
	JSPKT_OPT_BOOT,
	JSPKT_OPT_REG,
	JSPKT_OPT_HB0,
	JSPKT_OPT_HB1,
	JSPKT_PLUG_BOOT,
	JSPKT_PLUG_REG,
	JSPKT_PLUG_HB,
	JSPKT_PLUGREQ_RES
}jspkt_index_e;

#define JS_INFO_REGSITER	0x03
#define JS_INFO_RESULT	0x06
#define JS_INFO_MAC		0x0B
#define JS_INFO_CHECKSN	0x0F
#define JS_INFO_USERID	0x10
#define JS_INFO_DEVRND	0x11
#define JS_INFO_PKTID	0x15
#define JS_INFO_CHCODE	0x16
#define JS_INFO_SADDER	0x17
#define JS_INFO_SPORT	0x18
#define JS_INFO_KEY		0x19
#define JS_INFO_PLUGREQ	0x1A


typedef struct trans_pkt {
	uint32_t offset;
	uint32_t length;
	uint8_t payload[];
} trans_pkt_t;

typedef struct pkt_buf {
	uint32_t length;
	uint8_t payload[];
} pkt_buf_t;

#define JSPKT_NULL		0
#define JSPKT_STR		1
#define JSPKT_VAUE		2
#define JSPKT_FUNC		3
#define JSPKT_INPUT		4
#define JSPKT_GET		5

typedef int (get_jsoninfo)(int index, char *buffer);

typedef struct jsoninfo{
	int type;
	const char *name;
	const char *value;
	const char *node;
	const char *entry;
	get_jsoninfo *func;
}jsoninfo_t;

typedef struct jsonpkt{
	int num;
	const char *index;	
}jsonpkt_t;

//extern const char *SN;
extern char gSN[];

#if	(WITH_TEST_SERVER) /* only for test server  */
trans_pkt_t *FinishPktBuffer(rcstring *rcbuf, int needlength);
rcstring * NewPktBuffer(int length);
json_err_e AppendJsonPkt(rcstring *rcbuf, int index, const char *input);
#endif

int prehandle_pcaket(json_t **jsonpkt, const char *text);
int get_jsonpacket_result(json_t *jsonpkt);
char * get_json_value(json_t *jsonpkt, const char * labeltext);
char* make_checkSN(char *challengecode);
char* JS_Update_RND16(char *newstr);
char* JS_New_RND16(char *newstr);
char* JS_Calc_MD5(char *input0, char *input1, char *output);
char* js_get_DevRND(int index, char *buffer);
char* js_get_LOID(int index, char *buffer);

json_err_e CreateJsonPkt(trans_pkt_t **pPkt, jspkt_index_e index, int needlength, char *input);
json_err_e json_tree_to_packet (json_t * root, trans_pkt_t **text, int length);
json_err_e NewResultPkt(trans_pkt_t **pPkt, char *result, char *DevRND);
json_err_e NewPluginRegPkt(trans_pkt_t **pPkt, uint8_t *challengecode );
json_err_e NewOperateRegPkt(trans_pkt_t **pPkt, uint8_t *challengecode );

#endif /* JSON_PKT_H_ */
