#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/errno.h>
#include <modules/personality_parm_global_def.h>
/******************************************************************************************/
/******************************************************************************************/
static void display_personality_parm_env_info(void);
static int  personality_parm_open(struct inode *inode, struct file *filp);
static long personality_parm_ioctl(struct file *file, unsigned int cmd,    unsigned long arg);
static int  ecnt_personality_parm_get_env_info_ioctl(unsigned long cmd, realtime_env_info_t *pdata);

#define E_PERSONALITY_PARM_NO_INFO_LEVLE    (1 << 0)
#define E_PERSONALITY_PARM_DEBUG_INFO_LEVLE (1 << 1)

static int g_personality_parm_modules_debug_level = E_PERSONALITY_PARM_NO_INFO_LEVLE;

#define PERSONALITY_PARM_MODULES_DEBUG_INFO(fmt, ...)  do{ \
															if(g_personality_parm_modules_debug_level & E_PERSONALITY_PARM_DEBUG_INFO_LEVLE)\
																printk("func: %s, line:%d " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
														}while(0)

/******************************************************************************************/
static void display_personality_parm_env_info(void)
{
	int i = 0 ;
	int j = 0;
	int k = 0;
	realtime_env_info_t env;

	if(g_personality_parm_modules_debug_level & E_PERSONALITY_PARM_DEBUG_INFO_LEVLE)
	{
	    printk("\n");
	    for(i=E_FH_NW_PRODUCT_GET_SNOUI; i<=E_FH_NW_PRODUCT_GET_REALTIME_DUAL_IMAGE_INFO; i++)
	    {
	        k = i;
	        if(0 == k%4)
	        {
	            printk("\n");
	        }
	        memset(&env, 0, sizeof(env));
	        ecnt_personality_parm_get_env_info_api(i, &env);
	        switch(i)
	        {
	            case E_FH_NW_PRODUCT_GET_SNOUI ... E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE19:
	            {
	                printk("i = 0x%X, flash_env = %s\t", i, env.flash_env);
	                break;
	            }
	            case E_FH_NW_PRODUCT_GET_REALTIME_DUAL_IMAGE_INFO:
	            {
	                printk("\n");
	                printk("app1_active = %d, app1_commit = %d\t", env.dual_image_env.app1_active, env.dual_image_env.app1_commit);
	                printk("app2_active = %d, app2_commit = %d\t", env.dual_image_env.app2_active, env.dual_image_env.app2_commit);
	                printk("\n");
	                printk("kernel_active = %d, kernel_commit = %d\t", env.dual_image_env.kernel_active, env.dual_image_env.kernel_commit);
	                printk("rootfs_active = %d, app1_commit = %d\t", env.dual_image_env.rootfs_active, env.dual_image_env.rootfs_commit);
	                printk("\n");
	                printk("kernel_run = %d, kernel_roolback = %d\t",env.dual_image_env.kernel_run, env.dual_image_env.kernel_roolback);
	                printk("rootfs_run = %d, rootfs_roolback = %d\t",env.dual_image_env.rootfs_run, env.dual_image_env.rootfs_roolback);
	                printk("\n");
	                for(j=0; j<FH_NW_PRODUCT_PADDING_SIZE;j++)
	                {
	                    k = j;
	                    if(0 == k%4)
	                    {
	                        printk("\n");
	                    }
	                    printk("padding[%d] = 0x%X\t", j, env.dual_image_env.padding[j]);
	                }
	                break;
	            }
	            case E_FH_NW_PRODUCT_GET_BOOT_VERSION ... E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE6:
	            {
	                printk("i =0x%X, memory_env = %s\t", i, env.memory_env);
	                break;
	            }
	            default:
	            {
	                break;
	            }
	        }
	    }
	}

    printk("\n");
}

static int ecnt_personality_parm_get_env_info_ioctl(unsigned long cmd, realtime_env_info_t *pdata)
{
	personality_parm_boot_to_kernel_info_t *realtime_info = (personality_parm_boot_to_kernel_info_t *)FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;
	switch(cmd)
	{
		case IF_IOG_REALTIME_DUAL_IMAGE_FLAG_INFO:
		{
			memcpy(&pdata->dual_image_env, &realtime_info->realtime_env, sizeof(pdata->dual_image_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_BOOT_VERSION:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.boot_version, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE0:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve0, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE1:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve1, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE2:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve2, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE3:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve3, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE4:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve4, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE5:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve5, sizeof(pdata->memory_env));
			break;
		}
		case IF_IOG_FH_NW_PRODUCT_MEMORY_ENV_RESERVE6:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve6, sizeof(pdata->memory_env));
			break;
		}
		default:
		{
			printk("can't support cmd = 0x%X.\n", cmd);
			break;
		}
	}
	return 0;
}

/******************************************************************************************

Descriptor:	It's used to get env info

Input Args:	cmd: get env info cmd

Output Args:  flash/memory env info

Ret Value:	success: 0
           		fail:       -EINVAL
******************************************************************************************/
int ecnt_personality_parm_get_env_info_api(unsigned long cmd, realtime_env_info_t *pdata)
{
	personality_parm_boot_to_kernel_info_t *realtime_info = (personality_parm_boot_to_kernel_info_t *)FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;

	if(NULL == pdata)
	{
        return -EINVAL;
	}
	
	switch(cmd)
	{
		case E_FH_NW_PRODUCT_GET_REALTIME_DUAL_IMAGE_INFO:
		{
			memcpy(&pdata->dual_image_env, &realtime_info->realtime_env, sizeof(pdata->dual_image_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_SNOUI:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.sn_oui, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_HWCFG:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.hw_cfg, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_ETHERADDR:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.eth_addr, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE0:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve0, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE1:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve1, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE2:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve2, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE3:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve3, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE4:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve4, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE5:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve5, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE6:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve6, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE7:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve7, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE8:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve8, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE9:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve9, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE10:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve10, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE11:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve11, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE12:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve12, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE13:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve13, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE14:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve14, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE15:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve15, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE16:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve16, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE17:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve17, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE18:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve18, sizeof(pdata->flash_env));
			break;
		}		
		case E_FH_NW_PRODUCT_GET_FLASH_ENV_RESERVE19:
		{
			memcpy(&pdata->flash_env, &realtime_info->flash_env.f_reserve19, sizeof(pdata->flash_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_BOOT_VERSION:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.boot_version, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE0:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve0, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE1:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve1, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE2:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve2, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE3:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve3, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE4:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve4, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE5:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve5, sizeof(pdata->memory_env));
			break;
		}
		case E_FH_NW_PRODUCT_GET_MEMORY_ENV_RESERVE6:
		{
			memcpy(&pdata->memory_env, &realtime_info->memory_env.m_reserve6, sizeof(pdata->memory_env));
			break;
		}
		default:
		{
			printk("can't support cmd = 0x%X.\n", cmd);
			return -EINVAL;
			break;
		}
	}

	return 0;
}
EXPORT_SYMBOL(ecnt_personality_parm_get_env_info_api);

/******************************************************************************************/
static int  personality_parm_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static long personality_parm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int ret = -EINVAL;
    realtime_env_info_t data;
    
    if(NULL == arg)
    {
        return -EINVAL;
    }
    void __user *argp = (void __user *) arg;
    
	memset(&data, 0, sizeof(data));
    if(copy_from_user(&data, (realtime_env_info_t *)argp, sizeof(realtime_env_info_t)))
    {
        return -EFAULT;
    }

    ret = ecnt_personality_parm_get_env_info_ioctl(cmd, &data);
    if(0 != ret)
    {
        return -EFAULT;
    }
    
    if(copy_to_user(argp, &data, sizeof(data)))
    {
        return -EFAULT;
    }
    
    return ret;
}

const struct file_operations personality_parm_fops = 
{
    .owner          = THIS_MODULE,
    .open           = personality_parm_open,
    .unlocked_ioctl = personality_parm_ioctl,
};

static int __init personality_parm_init(void) 
{
    int err = 0;
    err = register_chrdev(PERSONALITY_PARM_MAJOR, PERSONALITY_PARM_MODULE_NAME, &personality_parm_fops);
    if(err < 0)
    {
        printk("PERSONALITY_PARM_MAJOR : could not get major number.\n");
        return err;
    }
    
    display_personality_parm_env_info();

    personality_parm_proc_create_entry();

    printk("personality parm insmod.\n");

    return 0;
}

static void __exit personality_parm_exit(void) 
{
	unregister_chrdev(PERSONALITY_PARM_MAJOR, PERSONALITY_PARM_MODULE_NAME);
	personality_parm_proc_remove_entry();
	printk("personality parm rmmod.\n");

	return ;
}

module_init(personality_parm_init);
module_exit(personality_parm_exit);

MODULE_LICENSE("GPL");


