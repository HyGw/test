#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/version.h>

#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/errno.h>
#include <modules/personality_parm_global_def.h>


static int fhdrv_loaded_component_read_proc(char *page, char **start, off_t off,
    int count, int *eof, void *data)
{
    int len = 0;

    personality_parm_boot_to_kernel_info_t *realtime_info = (personality_parm_boot_to_kernel_info_t *)FH_NW_PRODUCT_BOOT2KRENEL_ENV_ADDR;


    /* export loaded kernel */
    len += sprintf(page + len, "Loaded Kernel: %hhu\n", realtime_info->realtime_env.kernel_run);

    /* export loaded kernel */
    len += sprintf(page + len, "Loaded Rootfs: %hhu\n", realtime_info->realtime_env.rootfs_run);

    /* export actived app_fh */
    len += sprintf(page + len, "Actived aImage_fh: %hhu\n", realtime_info->realtime_env.app1_commit);

    /* export actived app_ex*/
    len += sprintf(page + len, "Actived aImage_ex: %hhu\n", realtime_info->realtime_env.app2_commit);
    
    *eof = 1;

    return len;
}


int personality_parm_proc_create_entry(void)
{
#ifdef CONFIG_PROC_FS

    struct proc_dir_entry *res = NULL;

    res = create_proc_read_entry("driver/fhdrv_kdrv_loaded_component",
        0, NULL, fhdrv_loaded_component_read_proc, NULL);
    if ( NULL == res )
    {
        printk("Failed to create /proc/driver/fhdrv_kdrv_loaded_component.\n");
        return -ENOMEM;
    }

    return 0;

#endif

    return 0;
}

void personality_parm_proc_remove_entry(void)
{
#ifdef CONFIG_PROC_FS

    remove_proc_entry("driver/fhdrv_kdrv_loaded_component", NULL);

#endif

    return;
}
