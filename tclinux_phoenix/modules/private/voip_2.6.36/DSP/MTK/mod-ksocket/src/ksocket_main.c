/*********************************************************************
* LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY 
* ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
* DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED FROM MEDIATEK AND/OR 
* ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. 
* MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR 
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY 
* WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH 
* MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK 
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE 
* RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S 
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND 
* CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED 
* HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE 
* MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR 
* SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE
* AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN 
* ACCORDANCE WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING
* ITS CONFLICT OF LAWS PRINCIPLES.
**********************************************************************
*/
/**
 *  @file           ksocket_main.c
 *  @author         Shu-Liang Lee
 *  @description    KSOCKET driver for linux
 *  @date           2008.03.13
 */
/* 
 * $Log: ksocket_main.c,v $
 * Revision 1.3  2008/11/12 07:01:13  mtk01984
 * remove #include <linux/config.h> to support 2.6.26
 *
 * Revision 1.2  2008/11/07 07:57:17  mtk01984
 * remove #include <linux/devfs_fs_kernel.h>
 *
 * Revision 1.1  2008/04/29 14:06:55  mtk01662
 * *: fit new linux platform architecture
 *
 *
 */

/*==========================================================================
 *
 *      Library Inclusion Segment
 *
 *==========================================================================
 */ 
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/interrupt.h>  /* for in_interrupt() */
#include <linux/kmod.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/fs.h>

#include <asm/uaccess.h>
#include <linux/poll.h>

#include <linux/delay.h>
#include <linux/sched.h>

#if CONFIG_PROC_FS
#include <linux/proc_fs.h>
#endif

#include "ksocket.h"
#include <../../install/include/voip_version.h>


/*==========================================================================
 *
 *      External Variable Segment
 *
 *==========================================================================
 */

/*==========================================================================
 *
 *      Constant and Macro Definition Segment
 *
 *==========================================================================
 */
#define MODULE_NAME "ksocket"

/*==========================================================================
 *
 *      Internal Function Segment
 *
 *==========================================================================
 */

/*==========================================================================
 *
 *      Global Variable Segment
 *
 *==========================================================================
 */

/*==========================================================================
 *
 *      External Function Segment
 *
 *==========================================================================
 */

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static ssize_t ksocket_read(struct file *file, char *buffer, size_t size, loff_t *ppos)
{
    return  0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static ssize_t ksocket_write(struct file *file, const char *buffer, size_t size, loff_t *ppos)
{
    return  0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static unsigned int ksocket_poll(struct file *file, struct poll_table_struct *wait)
{
    return  0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)
static int ksocket_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
#else
static int ksocket_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
#endif
{
    T_KSOCKET_IOCTL_PARAM  params;
        
    if(copy_from_user(&params, (unsigned long*)arg, sizeof(T_KSOCKET_IOCTL_PARAM)))
    {
        printk("%s: fail to copy.\n", __FUNCTION__);
        return -EFAULT;
    }
    
    switch(cmd)
    {
        default:
        {
            printk("%s: cmd = %x is not support yet!\n", __FUNCTION__, cmd);
            break;
        }                                   
    }

    return  0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static int ksocket_close(struct inode * inode, struct file * file)
{
	return 0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static int ksocket_open(struct inode * inode, struct file * file)
{
    return  0;
}

struct file_operations ksocket_op =
{
    read:       ksocket_read,
    write:      ksocket_write,
    poll:       ksocket_poll,
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31)
    unlocked_ioctl:      ksocket_ioctl,
#else
    ioctl:      ksocket_ioctl,
#endif
    open:       ksocket_open,
    release:    ksocket_close,
};

#if defined _UNIT_ksocket_main
#include <base/utest.h>

static exc_t t1(void *p)
{
	UTST(1);
	CERR_Ok();
} 

static int ksocket_init(void)
{
	UTST_Add(t1);
	UTST_Run(0);
	return 0;
}

static void ksocket_dest(void)
{
	printk(KERN_ALERT"unit, clean mod-ksocket\n");
	return;
}

module_init(ksocket_init)
module_exit(ksocket_dest)
#endif


#if !defined _UNIT_

#include "ksocket.h"
           
#ifdef CONFIG_PROC_FS
/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */           
int ksocket_utest(void)
{
    return  0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */           
static int ksocket_read_proc(char *buffer, char **start, off_t offset, int length, int *eof, void *data)
{
    int len=0;
    
    len += sprintf(buffer + len, "[KSOCKET Driver]\n");
    len += sprintf(buffer + len, "offset = %ld length = %d\n", offset, length);
    
    return  len;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */           
static int ksocket_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char        c;
	int         result, cmd;
    
    if(count<1)
    {
        return  count;
    }                

    result = get_user(c, buffer);
    
    cmd = c - '0';
        
    switch(cmd)
    {
        default:
        {
            printk("undefind!!\n");

            break;
        }            
        case    1:  /* unit test */
        {
            printk("UNIT Test Start...\n");     
            ksocket_utest();                
            printk("UNIT Test Done...\n");
            break;
        }
    }

	return count;        
}




static int voip_version_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	
	int len = 0;
	
	len = sprintf(page, "%s\n", RELEASE_VOIP_VERSION);

	len -= off;
	*start = page + off;
	
	if (len > count)
		len = count;
	else
		*eof = 1;
	
	if (len < 0)
		len = 0;
	
	return len;
}
static int voip_version_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	return count;
}

struct proc_dir_entry	*ksocket_proc_dir=0;
#endif


/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static int ksocket_init(void)
{    
    int result;

#if CONFIG_PROC_FS    
    struct proc_dir_entry   *ksocket_dir=0;
#endif    
    
	printk(KERN_ALERT"init mod-ksocket\n");
	/*	
	result = register_chrdev(KSOCKET_MAJOR_NUM , "ksocket", &ksocket_op);
	if(result)
    {
        printk("devfs_register_chrdev failed, result = %d, major = %d\n", result, KSOCKET_MAJOR_NUM);
        return -EBUSY;
	}
	*/
#if CONFIG_PROC_FS
        ksocket_dir = create_proc_read_entry("ksocket_kernel", S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH, 0, ksocket_read_proc, NULL);
        if(ksocket_dir)
        {
            ksocket_dir->write_proc = ksocket_write_proc;
        }

        ksocket_proc_dir = proc_mkdir(MODULE_NAME, NULL);
        ksocket_dir = create_proc_read_entry("ver_ksocket", S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH, ksocket_proc_dir, voip_version_read_proc, NULL);
        if(ksocket_dir)
        {
	        ksocket_dir->write_proc = voip_version_write_proc;
        }
	printk("ksocket[%s]\n",RELEASE_VOIP_VERSION);
#endif

	return 0;
}

/*--------------------------------------------------------------------------
 *                                                                          
 *  FUNCTION NAME :                                                         
 *      
 *                                                                          
 *  DESCRIPTION :                                                           
 *                                                                          
 *  INPUT :                                                                 
 *                                                                          
 *  OUTPUT :                                                                
 *      None                                                                
 *
 *  RETURN :                                                                
 *      None                                                              
 *--------------------------------------------------------------------------
 */
static void ksocket_dest(void)
{
    int result;
    
	printk(KERN_ALERT"clean mod-ksocket\n");
	
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
    unregister_chrdev(KSOCKET_MAJOR_NUM, "ksocket");
#else
    result = unregister_chrdev(KSOCKET_MAJOR_NUM, "ksocket");
	if(result<0)
    {	    
		printk(KERN_ALERT "unregister_chrdev fail, result = %d\n", result);
		return;
    }
#endif

#if CONFIG_PROC_FS
    remove_proc_entry("ksocket_kernel", NULL);
 
    remove_proc_entry("ver_ksocket", ksocket_proc_dir);
    remove_proc_entry(MODULE_NAME, NULL);
#endif

}

module_init(ksocket_init)
module_exit(ksocket_dest)

MODULE_AUTHOR("WIMAX");
MODULE_DESCRIPTION("KSOCKET DRV");
MODULE_LICENSE("MTK");

EXPORT_SYMBOL(inet_addr);
EXPORT_SYMBOL(inet_ntoa);
EXPORT_SYMBOL(ksocket);
EXPORT_SYMBOL(kbind);
EXPORT_SYMBOL(klisten);
EXPORT_SYMBOL(kconnect);
EXPORT_SYMBOL(kaccept);
EXPORT_SYMBOL(krecv);
EXPORT_SYMBOL(ksend);
EXPORT_SYMBOL(kshutdown);
EXPORT_SYMBOL(kclose);
EXPORT_SYMBOL(krecvfrom);
EXPORT_SYMBOL(ksendto);
EXPORT_SYMBOL(kgetsockname);
EXPORT_SYMBOL(kgetpeername);
EXPORT_SYMBOL(ksetsockopt);
EXPORT_SYMBOL(kgetsockopt);

#endif	
