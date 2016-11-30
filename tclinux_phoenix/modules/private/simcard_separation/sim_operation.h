#include <linux/version.h>
#define KERNEL_2_6_36 	(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))

//for debug 
enum{
	SIM_DBG_NONE=0,
	SIM_DBG_ERR,
	SIM_DBG_WARN,
	SIM_DBG_DEBUG,
	SIM_DBG_TRACE
};

#define SIMCARD_IOC_MAGIC 			'S'
#define SIMCARD_MAJOR 				168

#define SIMDATAFILEPTH 				"/var/sim/%02x"
#define VERIYDATALEN				24
#define VERIYCARDDATALEN			24
#define VERIYCPEDATALEN				16
#define VERIYCTDATALEN				256
#define UPDATEDATALEN				27

#define SIMCARD_RWMAX_LEN		0xFF

#define SIMCMD_VERIFY_CT		0xE0
#define SIMCMD_FILE_UPDATE		0xE1
#define SIMCMD_VERIFY_CARD		0xE2
#define SIMCMD_VERIFY_CPE		0xE3
#define SIMCMD_ROMRST_CARD		0xE4

#define SIM_ROMRST_ALL			0x00
#define SIM_ROMRST_LONG			0x01
#define SIM_ROMRST_SHORT		0x02

#define SIM_OP_SUCCESS		(0)
#define SIM_OP_ERR			(40)

#define SIM_IOCTL_ERR 		(-1)

#define SIM_VERIFYCARD_ERR	(41)
#define SIM_VERIFYCARD_OK	(81)

#define SIM_VERIFYCPE_ERR 	(42)
#define SIM_VERIFYCPE_OK 	(82)

#define SIM_VERIFYCT_ERR	(43)
#define SIM_VERIFYCT_OK		(83)


#define SIM_ROMRST_ERR 		(44)

#define SIMCARD_UPDATE_OK 			2
#define SIMCARD_RDFILE_OK 			3

#define T0END_TIMOUT_ERR 	0x10

#define SIM_PARITY_ERR 		0x1001
#define SIM_EXCUTE_OK		0x9000
#define SIM_EXCUTE_POK		0x9001
#define SIM_EXCUTE_NULL		0x6000

#define SIM_PARITY_ERR		0x1001

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
} update_data_para, *pupdata_data_para;

typedef struct _rwfile_data_para
{
	uint8_t id;
	int32_t len;
} rwfile_data_para;

typedef struct _led_data_para
{
	uint8_t type;
	uint8_t status;
} ledconf_para;

typedef struct _simcmd{
	int		cmd;
	int		romrst_type;
	verify_data_para	verifydata;
	rwfile_data_para	rwfiledata;
	verify_ct_para 		verifyct;
	uint8_t	updatedata[UPDATEDATALEN];
	int		result;
}simcmd_t;

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

void simdcard_init(void);
void simdcard_exit(void);

#if KERNEL_2_6_36
int simcard_ioctl(struct file *filp,unsigned int cmd, unsigned long arg);
#else
int simcard_ioctl(struct inode *inode, struct file *filp,unsigned int cmd, unsigned long arg);
#endif



