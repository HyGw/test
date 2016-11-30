#include <linux/types.h>
#include <linux/fs.h>
#include "do_mounts.h"
#include "ecnt_init_ext.h"

#if defined(TCSUPPORT_FWC_ENV)
#include <modules/personality_parm_global_def.h>
#endif
/**************************************************************************************************/
#if defined(TCSUPPORT_FWC_ENV)
static int __init do_mount_slave_rootfs(char *name, char *fs, int flags, void *data);
#endif
/**************************************************************************************************/
int __init ecnt_do_mount_root_hook(char *name, char *fs, int flags, void *data, int*err)
{
	if(*err)
	{
#if defined(TCSUPPORT_FWC_ENV)
		*err = do_mount_slave_rootfs(name, fs, flags, data);
#endif
	}
	
	return *err;
}

#if defined(TCSUPPORT_FWC_ENV)
dev_t ROOT_DEV_SLAVE;
static int __init do_mount_slave_rootfs(char *name, char *fs, int flags, void *data)
{
    int err = 0;
	personality_parm_boot_to_kernel_info_t *realtime_info = (personality_parm_boot_to_kernel_info_t *)FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;
	sys_umount("/root",MNT_DETACH);
	create_dev("/dev/root_slave", ROOT_DEV_SLAVE);
	err = sys_mount("/dev/root_slave", "/root", fs, flags, data);
	if (0 == err)
	{
		if(E_FH_NW_PRODUCT_START_UP_A == realtime_info->realtime_env.rootfs_run)
		{
			realtime_info->realtime_env.rootfs_run = E_FH_NW_PRODUCT_START_UP_B;
		}
		else
		{
			realtime_info->realtime_env.rootfs_run = E_FH_NW_PRODUCT_START_UP_A;
		}
		
		realtime_info->realtime_env.rootfs_roolback = E_FH_NW_PRODUCT_ROOLBACK_TRUE;
	}

	return err;
}

#endif

