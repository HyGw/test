#include <net/sock.h>
#include <net/arp.h>
#include <linux/ip.h>
#include <linux/version.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include <linux/skbuff.h>
#include <linux/completion.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/unistd.h>
#include <asm/uaccess.h>
#include <asm/types.h>
#include <asm/unaligned.h>	
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/ledcetrl.h>
#include <linux/libcompileoption.h>

#include "sim_operation.h"
#include "drv_comm.h"
#include "sim_hw_mtk.h"

extern void (*Simcard_Led_hook)(unsigned int type ,unsigned int status);

int sim_driver_dbg = SIM_DBG_ERR;//SIM_DBG_TRACE  SIM_DBG_ERR SIM_DBG_NONE
#define SIMLVL(x) if ((sim_driver_dbg) >= (x))
#define SIMCARDBUG(x,msg,args...) do { SIMLVL(x) printk(msg, ## args); } while (0)
#define SIMCARDBUG_N(x, p, N, msg,args...) do { SIMLVL(x) {printk(msg, ## args);int ik; for(ik=0; ik<(N); ik++){if(p) printk(" %02x",(unsigned char)(*((p)+ik)));} printk("!\n");}} while (0)
 
struct task_struct *simtask = NULL;
struct completion 	simComp;
wait_queue_head_t simwait;
spinlock_t simlock;
simcmd_t simcmd;
int sim_on_flag = -1;
int sim_hw_st = -1;

update_data_para romrstdata;
const uint8_t rstfilenums[] = {/*para key donot reset */
		0x02,0x03,0x04,0x05,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,
		0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x29,0x2A,0x2C,0x2D,0x2E,0x36,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,
		0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x7C,0x7D,0x7E,0x7F,0x80,0x81,0x82,0x83,0x86,0x87,0x8E,0x90,0x94,0x96,0x97,
		0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE};

void setRomRSTFileIndex(int type){
	int i = 0;
	uint8_t id = 0;
	
	memset(&romrstdata, 0xFF, sizeof(romrstdata));
	if(type != SIM_ROMRST_ALL){
		for(i = 0; i< sizeof(rstfilenums); i++){
			id = rstfilenums[i] - 2;
			romrstdata.data[id>>3] &= ~(0x80 >> (id & 0x07)); /* 1 file will be reset*/
		}
	}	
	
	SIMCARDBUG_N(SIM_DBG_ERR,romrstdata.data,UPDATEDATALEN,"romrstdata:");
}

int ReadBinary2File(uint8_t fileid){
	char fpath[20] = {0}; 
	uint8_t cmd[20] = {0}, respbuf[256] = {0};
	uint8_t i = 0, j = 0, k = 0;
	uint16_t len=0, offset = 0, sw = 0, sw0 = 0;
	static int readonce = 0;
	int ret = SIM_OP_ERR;
	struct file *fp = NULL;

	if(readonce == 0){
		cmd[0] = 0x7F;
		cmd[1] = 0x80;
		sw = SelectFile(cmd,respbuf);
		readonce = 1;
	}

	cmd[0] = 0x6F;
	cmd[1] = fileid;
	sw = SelectFile(cmd,respbuf);
	simcmd.rwfiledata.len = -1;
	if((sw & 0xff00) == 0x9f00){
		sw0 = GetResponse_CT(sw&0x00ff, respbuf);
		if(SIM_EXCUTE_OK == sw0){
			len = (respbuf[2] << 8) + respbuf[3];
			simcmd.rwfiledata.len = len;
		}
	}
	if(simcmd.rwfiledata.len < 0){
		SIMCARDBUG(SIM_DBG_ERR,"%s:file:%02x,sw:%04x sw-response:%04x!\n",__FUNCTION__, fileid, sw, sw0);
		goto ERR;
	}
	
	snprintf(fpath, sizeof(fpath), SIMDATAFILEPTH, fileid);
	fp = filp_open(fpath, O_WRONLY | O_CREAT, 0);
	if (IS_ERR(fp)) {
		SIMCARDBUG(SIM_DBG_ERR,"open file id=%d err!\n",fileid);
		goto ERR;
	}
	if(!(fp->f_op) || !(fp->f_op->write)){
		SIMCARDBUG(SIM_DBG_ERR,"write file id=%d ops err!\n",fileid);
		goto FILECLOSE;;
	}	
	
	
	i = len / SIMCARD_RWMAX_LEN;
	j = len % SIMCARD_RWMAX_LEN;
	for(k = 0; k < i; k++){
		respbuf[255] = 0;
		sw = ReadBinary_CT(offset>>8,offset&0xff,SIMCARD_RWMAX_LEN,respbuf);
		if(SIM_EXCUTE_OK != sw && SIM_EXCUTE_POK != sw){
			SIMCARDBUG(SIM_DBG_ERR,"%s:file:%02x,offset:%04x err:%04x!\n",__FUNCTION__, fileid, offset, sw);
			goto FILECLOSE;
		}
		fp->f_op->write(fp,	respbuf, SIMCARD_RWMAX_LEN, &fp->f_pos);	
		SIMCARDBUG(SIM_DBG_DEBUG,"read:%s!\n",respbuf);
		offset += SIMCARD_RWMAX_LEN;
	}
	if( j ){
		respbuf[j] = '\0';
		sw = ReadBinary_CT(offset>>8,offset&0xff,j,respbuf);
		if(SIM_EXCUTE_OK != sw ){
			SIMCARDBUG(SIM_DBG_ERR,"%s:file:%02x,offset:%04x err:%04x!\n",__FUNCTION__, fileid, offset, sw);
			goto FILECLOSE;
		}
		fp->f_op->write(fp,	respbuf, j, &fp->f_pos);
		SIMCARDBUG(SIM_DBG_DEBUG,"read:%s!\n",respbuf);
	}
	
	ret = SIM_OP_SUCCESS;
	
FILECLOSE:	
	filp_close(fp, NULL);
	
ERR:
	return ret;
}

void Simcard_Wait_T0END(int number, int period){
	int i = 0;
	for(i = 0; i < number; i++){
		/*
		if(SIM_Reg(SIM_SW1) != 0x60){
			SIMCARDBUG(SIM_DBG_ERR,"%s: write %02x, sw=%02x!\n", __FUNCTION__, k, SIM_Reg(SIM_SW1));
			break;
		}
		*/
		if(SIM_Reg(SIM_STS) & SIM_STS_T0END){
			SIMCARDBUG(SIM_DBG_DEBUG,"%s: wait T0END %02x, sw=%02x!\n", __FUNCTION__, i, SIM_Reg(SIM_SW1));
			break;
		}
		msleep(period);
		SIMCARDBUG(SIM_DBG_DEBUG,"%s: write %02x, sw=%02x!\n", __FUNCTION__, i, SIM_Reg(SIM_SW1));
	}
}

int WriteBinaryfromFile(uint8_t fileid, uint16_t len){
	char fpath[20] = {0};
	uint8_t buffer[256]={0}, respbuf[256]={0}, cmd[20]={0};
	uint8_t i = 0, j = 0, k = 0;
	uint16_t offset = 0, sw = 0;
	static int readonce = 0;
	struct file *fp = NULL;
	int ret = SIM_OP_ERR, readlen = 0;

	if(readonce == 0){
		cmd[0] = 0x7F;
		cmd[1] = 0x80;
		sw = SelectFile(cmd,respbuf);
		readonce = 1;
	}

	snprintf(fpath, sizeof(fpath), SIMDATAFILEPTH, fileid);
	fp = filp_open(fpath, O_RDONLY, 0);
	if (IS_ERR(fp)) {
		SIMCARDBUG(SIM_DBG_ERR,"open file id=%d err!\n",fileid);
		goto ERR;
	}
	if(!(fp->f_op) || !(fp->f_op->read)){
		SIMCARDBUG(SIM_DBG_ERR,"read file id=%d ops err!\n",fileid);
		goto FILECLOSE;;
	}
	cmd[0] = 0x6F;
	cmd[1] = fileid;
	sw = SelectFile(cmd,respbuf);
	if((sw & 0xff00) == 0x9f00){
		//sw = GetResponse_CT(sw&0x00ff, respbuf);
	}
	
	i = len / SIMCARD_RWMAX_LEN;
	j = len % SIMCARD_RWMAX_LEN;
	SIMCARDBUG(SIM_DBG_DEBUG,"WriteBinary len=%04x,%02x,%02x!\n",len,i,j);
	#if 0
	while(1){
		buffer[255] = '\0';
		readlen = fp->f_op->read(fp, buffer, 0xff, &fp->f_pos);
		if(readlen > 0 && readlen <= 0xff){
			sw = WriteBinary_CT(buffer, readlen, offset>>8, offset & 0xff, respbuf);
			SIMCARDBUG(SIM_DBG_DEBUG, "write:sw=%04x,len=%02x,%s!\n", sw, readlen, buffer);
			if(buffer[readlen] == '>'){
				if(SIM_EXCUTE_OK != sw){
					SIMCARDBUG(SIM_DBG_ERR,"%s:file:%02x,offset:%04x len:%02x err:%04x!\n", __FUNCTION__, fileid, offset, readlen, sw);
					//goto FILECLOSE;
				}
				break;
			}
			offset += 0xff;
		}
	}
	#else
	for(k = 0; k < i; k++){
		buffer[255] = '\0';
		readlen = fp->f_op->read(fp, buffer, SIMCARD_RWMAX_LEN, &fp->f_pos); 	
		sw = WriteBinary_CT(buffer, SIMCARD_RWMAX_LEN, offset>>8, offset&0xff, respbuf);
		if(SIM_EXCUTE_OK != sw && SIM_EXCUTE_POK != sw){
			SIMCARDBUG(SIM_DBG_ERR,"%s:file:%02x,offset:%04x len:%02x err:%04x!\n", __FUNCTION__, fileid, offset, readlen, sw);
			goto FILECLOSE;
		}
		SIMCARDBUG(SIM_DBG_DEBUG, "write:sw=%04x,%s!\n", sw, buffer);
		offset += SIMCARD_RWMAX_LEN;
	}
	if( j ){	
		buffer[j] = '\0';
		readlen = fp->f_op->read(fp, buffer, j, &fp->f_pos);
		sw = WriteBinary_CT(buffer, j, offset>>8, offset&0xff, respbuf);
		if((sw & 0xff00) == SIM_EXCUTE_NULL){/* if return 0x60, simcard cmd  not complete*/
			Simcard_Wait_T0END(100,100);
		}
		sw = SIM_Reg(SIM_SW2)| (SIM_Reg(SIM_SW1) << 8);
		if(SIM_EXCUTE_OK != sw ){
			SIMCARDBUG(SIM_DBG_ERR,"%s:file:%02x,offset:%04x len:%02x err:%04x!\n", __FUNCTION__, fileid, offset, readlen, sw);
			goto FILECLOSE;
		}
		SIMCARDBUG(SIM_DBG_DEBUG, "write:sw= %04x,%s!\n", sw, buffer);
	}
	//usleep(500000);
	#endif

	ret = SIM_OP_SUCCESS;
	
FILECLOSE:	
	filp_close(fp, NULL);
	
ERR:
	return ret;

}

static int check_simcard_on(void){
	char cmd[20] = {0};
	char respbuf[100] = {0};
	uint16_t sw = 0;
	cmd[0] = 0x7F;
	cmd[1] = 0x80;
	sw = SelectFile(cmd,respbuf);
	SIMCARDBUG(SIM_DBG_TRACE, "simcard_check: sw=%04x!\n",sw);
	if((sw & 0xFF00) == 0x9F00){		
		sw = GetResponse_CT(sw&0xFF, respbuf);
		SIMCARDBUG_N(SIM_DBG_TRACE,respbuf,sw&0xFF,"res:sw=%04x",sw);
		if(0x9000 != sw ) return -1;
	}else{
		return -1;
	}
	return 0;
}

static void simcard_check(void){
	static int need_reset = 0;
	int sim_on = 0;
	
	/*for reset*/
	if(need_reset){
		if(0 == Sim_Reset()){
			need_reset = 0;
			sim_on_flag = 0;
			
			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && Simcard_Led_hook) Simcard_Led_hook(SIMCARD_LED_INTERFACE, SIMCARD_INTERFACE_CONNECT);

		}
		else{
			sim_on_flag = -1;
			if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && Simcard_Led_hook) Simcard_Led_hook(SIMCARD_LED_INTERFACE, SIMCARD_INTERFACE_ERROR);
		}
		return;
	}
	sim_on = check_simcard_on();
	if( sim_on_flag != sim_on ){
		if ( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL )
		{
			if(Simcard_Led_hook){
				if(sim_on == 0 ) {
					Simcard_Led_hook(SIMCARD_LED_INTERFACE, SIMCARD_INTERFACE_CONNECT);
				}
				else{
					Simcard_Led_hook(SIMCARD_LED_INTERFACE, SIMCARD_INTERFACE_UNUSED);
				}
			}
		}
		sim_on_flag = sim_on;
		//if(sim_on_flag) need_reset = 1;
	}
	return;
}
/*
*/
static int simcardopsThread(void* data){
	int channel = *(int *)data;
	char cmd[20] = {0};
	char respbuf[100] = {0};
	uint16_t sw = 0, sw0 = 0;
	int status = 0, sim_on = -1;
	int i = 0;
	
	
	SIMCARDBUG(SIM_DBG_NONE,"simcardopsThread: start!\n");
	if(sim_hw_st == 0){
		spin_lock(&simlock);
		simcard_check();
		spin_unlock(&simlock);
	}
	while(!kthread_should_stop()){
		interruptible_sleep_on_timeout(&simwait, HZ);
	
		if(simcmd.cmd == 0 ){
			if(sim_hw_st == 0 ){
				spin_lock(&simlock);
				simcard_check();
				spin_unlock(&simlock);
			}
			continue;
		}
		//wait_event_interruptible(simwait, 1);
		switch(simcmd.cmd){
			case SIMCARD_IOC_VERIFY:				
				//sw = VerifyCard_CT(simcmd.verifydata.cpe, respbuf);		
				sw = SimCMD_CT(SIMCMD_VERIFY_CARD,simcmd.verifydata.cpe,VERIYCPEDATALEN,respbuf);
				
				if( 0x9F18 == sw ){
					sw = GetResponse_CT(0x18,respbuf);
					memcpy(simcmd.verifydata.card,respbuf,VERIYCARDDATALEN);
					simcmd.result = SIM_VERIFYCARD_OK;
				}
				else{
					SIMCARDBUG(SIM_DBG_ERR,"verify card err sw:%04x!\n", sw);
					simcmd.result = SIM_VERIFYCARD_ERR;
				}
				break;
			case SIMCARD_IOC_VERIFYCPE:
				//sw = VerifyCPE_CT(simcmd.verifydata.cpe, respbuf);	
				sw = SimCMD_CT(SIMCMD_VERIFY_CPE,simcmd.verifydata.cpe,0x08,respbuf);
				if( SIM_EXCUTE_OK == sw ){
					simcmd.result = SIM_VERIFYCPE_OK;
				}
				else{
					simcmd.result = SIM_VERIFYCPE_ERR;
					SIMCARDBUG(SIM_DBG_ERR,"verify card err sw:%04x!\n", sw);
					if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && Simcard_Led_hook)
						Simcard_Led_hook(SIMCARD_LED_INTERFACE, SIMCARD_INTERFACE_AUTH_FAIL);
				}
				break;
			case SIMCARD_IOC_VERIFYCT:
				sw = SimCMD_CT(SIMCMD_VERIFY_CT,simcmd.verifyct.data, strlen(simcmd.verifyct.data),respbuf);
				if( SIM_EXCUTE_OK == sw ){
					simcmd.result = SIM_VERIFYCT_OK;
				}
				else{
					simcmd.result = SIM_VERIFYCT_ERR;
				}
				SIMCARDBUG(SIM_DBG_ERR,"verifyct sw:%04x!\n", sw);
				break;
			case SIMCARD_IOC_RDFILE:				
				simcmd.result = ReadBinary2File(simcmd.rwfiledata.id);
				break;
			case SIMCARD_IOC_WRFILE:
				simcmd.result = WriteBinaryfromFile(simcmd.rwfiledata.id, simcmd.rwfiledata.len);
				break;
			case SIMCARD_IOC_UPDATE:
				simcmd.result = SIM_OP_ERR;
				sw = GetFileUpdate(respbuf);
				memset(simcmd.updatedata, 0, UPDATEDATALEN);
				if(0x9f1b == sw){
					sw0 = GetResponse_CT(0x1B,respbuf);
					if(SIM_EXCUTE_OK == sw0){
						memcpy(simcmd.updatedata,respbuf,UPDATEDATALEN);
						simcmd.result = SIM_OP_SUCCESS;
					}
				}
				SIMCARDBUG(SIM_DBG_DEBUG,"update sw:%04x response sw:%04x!\n", sw, sw0);
				break;	
			case SIMCARD_IOC_ROMRST:				
				setRomRSTFileIndex(simcmd.romrst_type);
				//sw = ResetCard_CT(romrstdata.data,UPDATEDATALEN,respbuf);
				sw = SimCMD_CT(SIMCMD_ROMRST_CARD,romrstdata.data,UPDATEDATALEN,respbuf);
				if((sw & 0xff00) == SIM_EXCUTE_NULL){ /* if return 0x60, simcard cmd  not complete*/
					Simcard_Wait_T0END(200,1000);
				}
				//SIM_ObtainSW(sw);
				sw = SIM_Reg(SIM_SW2)| (SIM_Reg(SIM_SW1) << 8);
				if(SIM_EXCUTE_OK == sw){
					simcmd.result = SIM_OP_SUCCESS;
				}
				else simcmd.result = SIM_OP_ERR;
				SIMCARDBUG(SIM_DBG_DEBUG,"rom reset sw:%04x!\n", sw);
				break;	
			case SIMCARD_IOC_RESET:				
				if(0 == sim_hw_st && 0 == Sim_Reset()){
					simcmd.result = SIM_OP_SUCCESS;
				}
				else simcmd.result = SIM_OP_ERR;	
				break;	
			}
		simcmd.cmd = 0;
		SIMCARDBUG(SIM_DBG_DEBUG,"simcardopThread: complete!\n");
				
		complete(&simComp);
	}

	SIMCARDBUG(SIM_DBG_NONE,"simcardopThread: done!\n");
	return 0;
} 

static struct file_operations simcard_fops = {
	.owner =		THIS_MODULE,
	.write =		NULL,
	.read =			NULL,
#if KERNEL_2_6_36
	.unlocked_ioctl = simcard_ioctl,
#else
	.ioctl =		simcard_ioctl,
#endif
	.open =			NULL,
	.release =		NULL,
};

void simdcard_init(){
	int ret = 0;
	
	ret = register_chrdev(SIMCARD_MAJOR, "simcard", &simcard_fops);
	if (ret < 0){
		SIMCARDBUG(SIM_DBG_ERR,"simdcard_init:error!\n");
	}
	
	if(!MT6306_HW_Check(3)){
		Sim_Reset();
		sim_hw_st = 0;
		simtask = kthread_run(simcardopsThread, NULL, "simcard_ops");
		if(IS_ERR(simtask)){
			SIMCARDBUG(SIM_DBG_ERR,"Create simcardopThread:error!\n");
		}

		init_completion(&simComp);
		init_waitqueue_head(&simwait);
		spin_lock_init(&simlock);
	}
	
}

void simdcard_exit(){
	unregister_chrdev(SIMCARD_MAJOR, "simcard");
	if(simtask){
		kthread_stop(simtask);
		simtask = NULL;
	}
}

extern sim_dbg_level;
#if KERNEL_2_6_36
int simcard_ioctl(struct file *filp,unsigned int cmd, unsigned long arg)
#else
int simcard_ioctl(struct inode *inode, struct file *filp,unsigned int cmd, unsigned long arg)
#endif
{
	unsigned int dbgflag;
	ledconf_para ledconf;
	memset(&simcmd,0,sizeof(simcmd_t));
	simcmd.result = SIM_OP_SUCCESS;
	if(_IOC_TYPE(cmd) != SIMCARD_IOC_MAGIC) 
		return -ENOTTY;
	if(_IOC_NR(cmd) > SIMCARD_IOC_MAX_NR)
		return -ENOTTY;

	if(sim_hw_st){
		simcmd.result = SIM_OP_ERR;
		if(put_user(simcmd.result,  (int __user *)arg)){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		return simcmd.result;
	}

	switch (cmd){
	case SIMCARD_IOC_DEBUGFLAG:	
		if (get_user(dbgflag, (unsigned long __user *)arg))
			return -EFAULT;
		
		if (dbgflag >= 0){
			sim_driver_dbg = dbgflag >> 8;
			sim_dbg_level  = dbgflag & 0xff;
		}
		else{
			sim_driver_dbg = 0;
			sim_dbg_level  = 0;
		}
		SIMCARDBUG(SIM_DBG_NONE,"simdcard set debug flag:%d,simdriver:%d,sim:%d!\n",dbgflag,sim_driver_dbg,sim_dbg_level);
		break;
	case SIMCARD_IOC_VERIFY:
	case SIMCARD_IOC_VERIFYCPE:	
		spin_lock(&simlock);
		if (copy_from_user(&simcmd.verifydata, (char *)arg, sizeof(verify_data_para))){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		simcmd.cmd = cmd;	
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		
		if((simcmd.result == SIM_VERIFYCARD_OK)){	
			if(copy_to_user((char *)arg, &simcmd.verifydata, sizeof(verify_data_para))){
				spin_unlock(&simlock);
				return -EFAULT;
			}
		}		
		spin_unlock(&simlock);
		break;
	case SIMCARD_IOC_VERIFYCT:	
		spin_lock(&simlock);
		if (copy_from_user(&simcmd.verifyct, (char *)arg, sizeof(verify_ct_para))){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		SIMCARDBUG(SIM_DBG_ERR, "verifyct:%s!\n",simcmd.verifyct.data);
		simcmd.cmd = cmd;	
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		spin_unlock(&simlock);
		break;	
	case SIMCARD_IOC_RDFILE:
		spin_lock(&simlock);
		if (copy_from_user(&simcmd.rwfiledata, (char *)arg, sizeof(rwfile_data_para))){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		SIMCARDBUG(SIM_DBG_DEBUG, "read file id:%d!\n",simcmd.rwfiledata.id);
		simcmd.cmd = cmd;
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		//if((simcmd.result == SIM_OP_SUCCESS)){
			if(copy_to_user((char *)arg, &simcmd.rwfiledata, sizeof(rwfile_data_para))){
				spin_unlock(&simlock);
				return -EFAULT;
			}
		//}
		spin_unlock(&simlock);
		break;
	case SIMCARD_IOC_WRFILE:
		spin_lock(&simlock);
		if (copy_from_user(&simcmd.rwfiledata, (char *)arg, sizeof(rwfile_data_para))){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		SIMCARDBUG(SIM_DBG_DEBUG, "write file id:%d,len:%d!\n",simcmd.rwfiledata.id,simcmd.rwfiledata.len);
		simcmd.cmd = cmd;
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		spin_unlock(&simlock);
		break;
	case SIMCARD_IOC_LEDCONF:
		if (copy_from_user(&ledconf, (char *)arg, sizeof(ledconf_para))){
			return -EFAULT;
		}	
		if( TCSUPPORT_CT_SIMCARD_SEPARATION_VAL && Simcard_Led_hook){
			Simcard_Led_hook(ledconf.type,ledconf.status);
		}
		SIMCARDBUG(SIM_DBG_DEBUG,"simdcard led conf type:%d,status=%d!\n",ledconf.type,ledconf.status);
		break;	
	case SIMCARD_IOC_UPDATE:
		spin_lock(&simlock);
		SIMCARDBUG(SIM_DBG_TRACE, "get update!\n");
		simcmd.cmd = cmd;
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		//if((simcmd.result == SIMCARD_UPDATE_OK)){	
			if(copy_to_user((char *)arg, &simcmd.updatedata[0], UPDATEDATALEN)){
				spin_unlock(&simlock);
				return -EFAULT;
			}
		//}
		spin_unlock(&simlock);
		break;
	case SIMCARD_IOC_ROMRST:
		spin_lock(&simlock);
		SIMCARDBUG(SIM_DBG_DEBUG, "rom reset type=%d!\n");
		if (get_user(simcmd.romrst_type, (unsigned long __user *)arg)){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		simcmd.cmd = cmd;
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		if(put_user(simcmd.result,  (int __user *)arg)){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		spin_unlock(&simlock);
		break;	
	case SIMCARD_IOC_RESET:
		spin_lock(&simlock);
		SIMCARDBUG(SIM_DBG_TRACE, "reset!\n");
		simcmd.cmd = cmd;
		wake_up_interruptible(&simwait);
		wait_for_completion_interruptible(&simComp);
		if(put_user(simcmd.result,  (int __user *)arg)){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		spin_unlock(&simlock);
		break;	
	case SIMCARD_IOC_STATUS:
		spin_lock(&simlock);
		SIMCARDBUG(SIM_DBG_TRACE, "get status!\n");
		if(put_user(sim_on_flag,  (int __user *)arg)){
			spin_unlock(&simlock);
			return -EFAULT;
		}
		spin_unlock(&simlock);
		break;
	default:
		SIMCARDBUG(SIM_DBG_ERR,"not support this type!");
		break;
	}

	return simcmd.result;
}
