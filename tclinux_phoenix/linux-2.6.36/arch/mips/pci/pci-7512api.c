

/************************************************************************
*               I N C L U D E S
*************************************************************************
*/
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/interrupt.h> 
#include <asm/tc3162/tc3162.h>
#include <linux/proc_fs.h>
#include <linux/io.h>
#include <ecnt_hook/ecnt_hook_pcie.h>


/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/



/************************************************************************
*                  M A C R O S
*************************************************************************
*/
#if defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
	
#if defined(TCSUPPORT_CPU_MT7505) || defined(TCSUPPORT_CPU_EN7512) || defined(TCSUPPORT_CPU_EN7521)
#define isRC0_LINKUP		((regRead32(0xbfb80050) & 0x2) ? 1 : 0)  
#define isRC1_LINKUP		((regRead32(0xbfb80050) & 0x4) ? 1 : 0)
#else
#define isRC0_LINKUP		((regRead32(0xbfb82050) & 0x1) ? 1 : 0)
#define isRC1_LINKUP		((regRead32(0xbfb83050) & 0x1) ? 1 : 0)
#endif

#define PCIE_MAJOR 225
#define PCIE_CNT_NUMBER 6

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/
typedef int (*pcie_api_op_t)(struct ecnt_pcie_data * data);
enum {
	PCIE_DEV_RC0 = 0,
	PCIE_DEV_RC1,
	PCIE_DEV_EP0,
	PCIE_DEV_EP1,
};

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/

int pcie_write_config_word(unsigned char type, unsigned char bus, unsigned char devnum, unsigned int regnum, unsigned long int value);
unsigned long  pcie_read_config_word(unsigned char type, unsigned char bus, unsigned char devnum, unsigned int regnum);
int pcie_api_get_confreg(struct ecnt_pcie_data *data);
int pcie_api_set_confreg(struct ecnt_pcie_data *data);
int pcie_api_get_aspm(struct ecnt_pcie_data *data);
int pcie_api_set_aspm(struct ecnt_pcie_data *data);
int pcie_api_get_speed(struct ecnt_pcie_data *data);
int pcie_api_set_speed(struct ecnt_pcie_data *data);
int pcie_api_get_count(struct ecnt_pcie_data *data);
int pcie_api_get_linkstate(struct ecnt_pcie_data *data);
ecnt_ret_val ecnt_pcie_api_hook(struct ecnt_data *in_data);
static long pcie_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/

struct ecnt_hook_ops ecnt_pcie_api_op = {
	.name = "pcie_api_hook",
	.is_execute = 1,
	.hookfn = ecnt_pcie_api_hook,
	.maintype = ECNT_PCIE,
	.subtype = ECNT_PCIE_API,
	.priority = 1
};

struct file_operations pcie_fops = {
	.owner 			= THIS_MODULE,
	.unlocked_ioctl	= pcie_ioctl,
};


/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
static spinlock_t pcie_api_lock = SPIN_LOCK_UNLOCKED;

static unsigned int pcie_err_reg[2][PCIE_CNT_NUMBER] = 
{
	{0xbfb80054,0xbfb801d8,0xbfb801dc,0xbfb801e0,0xbfb801e4,0xbfb801e8},
	{0xbfb80058,0xbfb801ec,0xbfb801f0,0xbfb801f4,0xbfb801f8,0xbfb801fc}
};

static pcie_api_op_t pcie_operation[] = {
	pcie_api_get_confreg,
	pcie_api_set_confreg,
	pcie_api_get_aspm,
	pcie_api_set_aspm,
	pcie_api_get_speed,
	pcie_api_set_speed,
	pcie_api_get_count,
	pcie_api_get_linkstate
};

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/



int pcie_write_config_word_hw(unsigned char bus, unsigned char dev,unsigned char func, unsigned int reg, unsigned long int value)
{
	return pcie_write_config_word(0,bus,dev,reg,value);
}

unsigned int pcie_read_config_word_hw(unsigned char bus,unsigned char dev,unsigned char func ,unsigned int reg)
{
	return pcie_read_config_word(0,bus,dev,reg);
}

static int get_cap_pos(char bus,char dev,char func, char id)
{
	unsigned int val,pos;

	val = pcie_read_config_word_hw(bus,dev,func,0x34);
	pos = val&0xff;
	while(pos && pos != 0xff)
	{
		val = pcie_read_config_word_hw(bus,dev,func,pos);
		if ( (val&0xff) == id)
			return pos;
		pos = (val >> 0x08) & 0xff;
	}
	return 0;
}


static int pcie_get_confreg(int idx,int offset)
{
	unsigned char bus,dev,func;
	unsigned  int val;
	
	if (offset >= 0x1000 || offset < 0)
		return -1;

	if (isRC0_LINKUP == 0 && idx == PCIE_DEV_EP0)
		return -1;
	
	if (isRC1_LINKUP == 0 && idx == PCIE_DEV_EP1)
		return -1;	
	
	if (idx == PCIE_DEV_RC0){
		bus = 0; dev = 0; func = 0;
	}else if (idx == PCIE_DEV_RC1){
		bus = 0; dev = 1; func = 0;
	}else if (idx == PCIE_DEV_EP0){
		bus = 1; dev = 0; func = 0;
	}else if (idx == PCIE_DEV_EP1){
		bus = 2;dev = 0; func = 0;
	}else{
		return -1;
	}
	
	offset &= 0xffc;
	
	val = pcie_read_config_word_hw(bus,dev,func,offset);
	
	return val;
}



static int pcie_set_confreg(int idx,int offset,unsigned int val)
{
	unsigned char bus,dev,func;
	
	if (offset >= 0x1000 || offset < 0)
		return -1;

	if (isRC0_LINKUP == 0 && idx == PCIE_DEV_EP0)
		return -1;
	
	if (isRC1_LINKUP == 0 && idx == PCIE_DEV_EP1)
		return -1;	
	
	if (idx == PCIE_DEV_RC0){
		bus = 0; dev = 0; func = 0;
	}else if (idx == PCIE_DEV_RC1){
		bus = 0; dev = 1; func = 0;
	}else if (idx == PCIE_DEV_EP0){
		bus = 1; dev = 0; func = 0;
	}else if (idx == PCIE_DEV_EP1){
		bus = 2;dev = 0; func = 0;
	}else{
		return -1;
	}

	
	offset &= 0xffc;

	pcie_write_config_word_hw(bus,dev,func,offset,val);

	return 0;
}



static int pcie_set_aspm_ext(char bus,char dev,char func,unsigned int val)
{
	unsigned int pos = 0,value = 0;

	pos = get_cap_pos(bus,dev,func,0x10);
	
	if (pos < 0x40)
		return -1;

	if (val){
		value = pcie_read_config_word_hw(bus,dev,func,pos+12);
		value &= 0x0c00;
		value = value >> 10;
		if ((val > value) || ((val & value) == 0))
			return -1;
	}

	value = pcie_read_config_word_hw(bus,dev,func,pos+16);
	
	value &= 0xfffffffc;
	
	value |= val;
	
	pcie_write_config_word_hw(bus,dev,func,pos+16,value);
	
	return 0;
}

static int pcie_set_aspm(int idx,int sw)
{
	sw &= 0x3;

	if (idx == PCIE_DEV_RC0)
		return pcie_set_aspm_ext(0,0,0,sw);

	if (idx == PCIE_DEV_RC1)
		return pcie_set_aspm_ext(0,1,0,sw);

	if (idx == PCIE_DEV_EP0 && isRC0_LINKUP)
		return pcie_set_aspm_ext(1,0,0,sw);
	
	if (idx == PCIE_DEV_EP1 && isRC1_LINKUP)
		return pcie_set_aspm_ext(2,0,0,sw);

	return -1;
}

static int pcie_get_aspm_ext(char bus,char dev,char func)
{
	unsigned int pos = 0,value = 0;

	pos = get_cap_pos(bus,dev,func,0x10);
	
	if (pos < 0x40)
		return -1;

	value = pcie_read_config_word_hw(bus,dev,func,pos+16);
	
	value &= 0x3;
	
	return value;
}

static int pcie_get_aspm(int idx)
{
	if (idx == PCIE_DEV_RC0)
		return pcie_get_aspm_ext(0,0,0);

	if (idx == PCIE_DEV_RC1)
		return pcie_get_aspm_ext(0,1,0);

	if (idx == PCIE_DEV_EP0 && isRC0_LINKUP)
		return pcie_get_aspm_ext(1,0,0);
	
	if (idx == PCIE_DEV_EP1 && isRC1_LINKUP)
		return pcie_get_aspm_ext(2,0,0);

	return -1;
}


static int pcie_set_speed_ext(char bus,char dev,char func,unsigned int val)
{
	unsigned int pos = 0,value = 0;

	pos = get_cap_pos(bus,dev,func,0x10);
	if (pos < 0x40)
		return -1;

	value = pcie_read_config_word_hw(bus,dev,func,pos+0x30);
	value &= (~0x0f);
	value |= val ;
	pcie_write_config_word_hw(bus,dev,func,pos+0x30,value);
	return 0;
}

static int pcie_set_speed(int idx,unsigned int mode)
{
	unsigned int pos = 0,val = 0,dev ,bus;

	if (idx != PCIE_DEV_EP0 && idx != PCIE_DEV_EP1)
		return -1;
	
	if (isRC0_LINKUP == 0 && idx == PCIE_DEV_EP0)
		return -1;
	
	if (isRC1_LINKUP == 0 && idx == PCIE_DEV_EP1)
		return -1;

	if (idx == PCIE_DEV_EP0)
	{
		dev = 0;
		bus = 1;
	}
	else
	{
		dev = 1;
		bus = 2;
	}

	mode &= 0x3;
	
	pos = get_cap_pos(0,dev,0,0x10);
	if (pos < 0x40)
		return -1;
	
	val = pcie_read_config_word_hw(0,dev,0,pos+0x0c);
	if ((val&0x0f) < mode)
		return -1;

	pos = get_cap_pos(bus,0,0,0x10);
	if (pos < 0x40)
		return -1;
	
	val = pcie_read_config_word_hw(bus,0,0,pos+0x0c);
	if ((val&0x0f) < mode)
		return -1;

	pcie_set_speed_ext(0,dev,0,mode);
	pcie_set_speed_ext(bus,0,0,mode);
	
	val = pcie_read_config_word_hw(0,dev,0,pos+0x10);
	val |=  (1 << 5); 
	pcie_write_config_word_hw(0,dev,0,pos+0x10,val);
	mdelay(100);
	
	return 0;
}


static int pcie_get_speed(int idx)
{
	unsigned int pos = 0,val = 0,dev = 0;

	if (idx != PCIE_DEV_EP0 && idx != PCIE_DEV_EP1)
		return -1;
	
	if (isRC0_LINKUP == 0 && idx == PCIE_DEV_EP0)
		return -1;
	
	if (isRC1_LINKUP == 0 && idx == PCIE_DEV_EP1)
		return -1;

	
	if (idx == PCIE_DEV_EP0)
		dev = 0;
	else
		dev = 1;

	pos = get_cap_pos(0,dev,0,0x10);
	if (pos < 0x40)
		return -1;
	
	val = pcie_read_config_word_hw(0,dev,0,pos+0x10);
	val = (val >> 16) & 0x0f ;

	return val;
}


static int pcie_get_count(int idx, struct ecnt_pcie_count_data* pcnt)
{
	int i;

	if (idx != PCIE_DEV_RC0 && idx != PCIE_DEV_RC1)
		return -1;
	
	for(i=0; i < PCIE_CNT_NUMBER; i++){
		pcnt->err[i] = regRead32(pcie_err_reg[idx][i]);
	}

	return 0;
}

static int pcie_get_linkstate(int idx)
{
	if (idx == PCIE_DEV_RC0)
		return isRC0_LINKUP;
	
	if (idx == PCIE_DEV_RC1)
		return isRC1_LINKUP;

	return -1;
}

int pcie_api_get_confreg(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	int off = data->conf.off;
	data->retValue = pcie_get_confreg(idx,off);
	return 0;
}

int pcie_api_set_confreg(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	unsigned int off = data->conf.off;
	unsigned int val = data->conf.val;
	data->retValue = pcie_set_confreg(idx,off,val);
	return 0;
}

int pcie_api_get_aspm(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	data->retValue = pcie_get_aspm(idx);
	return 0;
}

int pcie_api_set_aspm(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	unsigned int val = data->conf.val;
	data->retValue = pcie_set_aspm(idx,val);
	return 0;
}

int pcie_api_get_speed(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	data->retValue = pcie_get_speed(idx);
	return 0;
}

int pcie_api_set_speed(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	unsigned int val = data->conf.val;
	data->retValue = pcie_set_speed(idx,val);
	return 0;
}

int pcie_api_get_count(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	data->retValue = pcie_get_count(idx, &data->cnt);
	return 0;
}

 int pcie_api_get_linkstate(struct ecnt_pcie_data *data)
{
	int idx = data->idx;
	data->retValue = pcie_get_linkstate(idx);
	return 0;
}

ecnt_ret_val ecnt_pcie_api_hook(struct ecnt_data *in_data)
{
	struct ecnt_pcie_data *data = (struct ecnt_pcie_data *)in_data ;	
	
	if(data->function_id >= PCIE_FUNCTION_MAX_NUM) {
		printk("pcie data->function_id is %d, exceed max number: %d", data->function_id, PCIE_FUNCTION_MAX_NUM);
 		return ECNT_HOOK_ERROR;
	}

	spin_lock(&pcie_api_lock);
	pcie_operation[data->function_id](data) ;
	spin_unlock(&pcie_api_lock);
	
	return ECNT_CONTINUE;
}

static long pcie_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) 
{
	int ret = 0 ;
	struct ecnt_pcie_data data;
	struct ecnt_pcie_data* puser = (struct ecnt_pcie_data*)arg;

	if (cmd >= PCIE_FUNCTION_MAX_NUM)
		return -1;
	
	memset(&data,0,sizeof(struct ecnt_pcie_data));
	copy_from_user(&data, puser ,sizeof(struct ecnt_pcie_data));
	spin_lock(&pcie_api_lock);	
	ret = pcie_operation[data.function_id](&data);
	spin_unlock(&pcie_api_lock);
	copy_to_user(puser,&data,sizeof(struct ecnt_pcie_data));
	
	return ret;
}

int pcie_api_init(void)
{
	int ret;
	
	if(ecnt_register_hook(&ecnt_pcie_api_op)){
		printk("pcie ecnt_dev_fe_api_op register fail\n");
		return 0;
	}

	ret = register_chrdev(PCIE_MAJOR, "/dev/pcie", &pcie_fops);
	
	if (ret < 0) {
		printk(KERN_WARNING "pcie: can't get major %d\n", PCIE_MAJOR);
		return ret;
	}

	return 0;
}

#else

int pcie_api_init(void)
{
	return 0;
}

#endif


