#ifndef _SIMCARD_LIB_H
#define _SIMCARD_LIB_H 

#define SIMCARD_IOC_MAGIC 			'S'
#define SIMCARD_MAJOR 				168

#define SIM_OP_SUCCESS		(0)
#define SIM_IOCTL_ERR 		(-1)
#define SIM_VERIFYCARD_ERR	(41)
#define SIM_VERIFYCARD_OK	(81)

#define SIM_VERIFYCT_ERR	(43)
#define SIM_VERIFYCT_OK		(83)

#define SIM_VERIFYCPE_ERR 	(42)
#define SIM_VERIFYCPE_OK 	(82)

#define VERIYDATALEN				24
#define VERIYCARDDATALEN			24
#define VERIYCPEDATALEN				16
#define VERIYCTDATALEN				256
#define UPDATEDATALEN				27

#define SIM_ROMRST_ALL			0x00
#define SIM_ROMRST_LONG			0x01
#define SIM_ROMRST_SHORT		0x02

#define SIMCARD_LED_INTERFACE	1
#define SIMCARD_LED_CONFIG		2

#define SIMCARD_INTERFACE_UNUSED 	0
#define SIMCARD_INTERFACE_CONNECT 	1
#define SIMCARD_INTERFACE_ERROR		2
#define SIMCARD_INTERFACE_AUTH_FAIL	3

#define SIMCARD_CONF_FAIL		0
#define SIMCARD_CONF_RUNNING	1
#define SIMCARD_CONF_SUCCESS	2


typedef		unsigned char	uint8_t;
typedef		signed char		int8_t;
typedef		unsigned short	uint16_t;
typedef		signed short	int16_t;
typedef		unsigned int	uint32_t;
typedef		signed int		int32_t;

typedef struct _verify_data_para
{
	uint8_t cpe[VERIYCPEDATALEN];
	uint8_t card[VERIYCARDDATALEN];
} verify_data_para;

typedef struct _verify_ct_para
{
	uint8_t data[VERIYCTDATALEN];
} verify_ct_para;

typedef struct _update_data_para
{
	uint8_t data[UPDATEDATALEN];
} update_data_para, *pupdate_data_para;

typedef struct _led_data_para
{
	uint8_t type;
	uint8_t status;
} ledconf_para;


typedef struct _rwfile_data_para
{
	uint8_t id;
	int32_t len;
} rwfile_data_para;

#define SIMCARD_IOC_DEBUGFLAG	_IOW(SIMCARD_IOC_MAGIC,		0, 	unsigned int)
#define SIMCARD_IOC_VERIFYCT	_IOW(SIMCARD_IOC_MAGIC,		1, 	verify_ct_para)
#define SIMCARD_IOC_VERIFY		_IOW(SIMCARD_IOC_MAGIC,		2, 	verify_data_para)
#define SIMCARD_IOC_VERIFYCPE	_IOW(SIMCARD_IOC_MAGIC,		3, 	verify_data_para)
#define SIMCARD_IOC_RDFILE		_IOWR(SIMCARD_IOC_MAGIC,	4, 	rwfile_data_para)
#define SIMCARD_IOC_WRFILE		_IOW(SIMCARD_IOC_MAGIC,		5, 	rwfile_data_para)
#define SIMCARD_IOC_UPDATE		_IOR(SIMCARD_IOC_MAGIC,		6, 	update_data_para)
#define SIMCARD_IOC_RESET		_IOW(SIMCARD_IOC_MAGIC,		7, 	unsigned int)
#define SIMCARD_IOC_STATUS		_IOR(SIMCARD_IOC_MAGIC,		8, 	unsigned int)
#define SIMCARD_IOC_ROMRST		_IOW(SIMCARD_IOC_MAGIC,		9, 	unsigned int)
#define SIMCARD_IOC_LEDCONF		_IOW(SIMCARD_IOC_MAGIC,		10,	ledconf_para)

#define SIMCARD_IOC_MAX_NR 16

//extern int ioctl_simcard(int cmd, void *data, int datealen);
extern int simcard_verify(uint8_t *cdata, int cdatalen);
extern int simcard_verifyct(uint8_t *cdata);
extern int simcard_fileread(uint8_t no, int32_t *plen);
extern int simcard_filewrite(uint8_t no, uint16_t len);
extern int simcard_update(update_data_para  *pupdate);
extern int simcard_romrst(int type);
extern int simcard_reset(void);
extern int simcard_status(void);
extern int simcard_ledconf(int type, int status);

#endif /* _SIMCARD_LIB_H */

