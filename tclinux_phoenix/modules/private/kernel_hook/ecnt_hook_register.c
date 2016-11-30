/***************************************************************
Copyright Statement:

This software/firmware and related documentation (¡°EcoNet Software¡±) 
are protected under relevant copyright laws. The information contained herein 
is confidential and proprietary to EcoNet (HK) Limited (¡°EcoNet¡±) and/or 
its licensors. Without the prior written permission of EcoNet and/or its licensors, 
any reproduction, modification, use or disclosure of EcoNet Software, and 
information contained herein, in whole or in part, shall be strictly prohibited.

EcoNet (HK) Limited  EcoNet. ALL RIGHTS RESERVED.

BY OPENING OR USING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY 
ACKNOWLEDGES AND AGREES THAT THE SOFTWARE/FIRMWARE AND ITS 
DOCUMENTATIONS (¡°ECONET SOFTWARE¡±) RECEIVED FROM ECONET 
AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON AN ¡°AS IS¡± 
BASIS ONLY. ECONET EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, 
WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
OR NON-INFRINGEMENT. NOR DOES ECONET PROVIDE ANY WARRANTY 
WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTIES WHICH 
MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE ECONET SOFTWARE. 
RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTIES FOR ANY AND ALL 
WARRANTY CLAIMS RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES 
THAT IT IS RECEIVER¡¯S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD 
PARTY ALL PROPER LICENSES CONTAINED IN ECONET SOFTWARE.

ECONET SHALL NOT BE RESPONSIBLE FOR ANY ECONET SOFTWARE RELEASES 
MADE TO RECEIVER¡¯S SPECIFICATION OR CONFORMING TO A PARTICULAR 
STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND 
ECONET'S ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE ECONET 
SOFTWARE RELEASED HEREUNDER SHALL BE, AT ECONET'S SOLE OPTION, TO 
REVISE OR REPLACE THE ECONET SOFTWARE AT ISSUE OR REFUND ANY SOFTWARE 
LICENSE FEES OR SERVICE CHARGES PAID BY RECEIVER TO ECONET FOR SUCH 
ECONET SOFTWARE.
***************************************************************/

/************************************************************************
*                  I N C L U D E S
*************************************************************************
*/
#include <linux/init.h>
#include <linux/module.h>
#include <ecnt_hook/ecnt_hook.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>

/************************************************************************
*                  D E F I N E S   &   C O N S T A N T S
*************************************************************************
*/
MODULE_DESCRIPTION("Kernel_Hook");
MODULE_LICENSE("GPL");

/************************************************************************
*                  M A C R O S
*************************************************************************
*/	
#define MAX_CMDLINE_ARGC 4

/************************************************************************
*                  D A T A   T Y P E S
*************************************************************************
*/

/************************************************************************
*                  E X T E R N A L   D A T A   D E C L A R A T I O N S
*************************************************************************
*/


/************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
*************************************************************************
*/


/************************************************************************
*                  P U B L I C   D A T A
*************************************************************************
*/

/************************************************************************
*                  P R I V A T E   D A T A
*************************************************************************
*/
struct ecnt_hook_ops ecnt_ops[] = {};

/************************************************************************
*                  F U N C T I O N   D E F I N I T I O N S
*************************************************************************
*/

/*****************************************************************************
//function :
//		prepare_params
//description : 
//		analyze input parameters
//input :	
//		char * cmdline, size_t * argc, char ** argv
//return :
//		1
******************************************************************************/
static int prepare_params(char * cmdline, size_t * argc, char ** argv)
{
    size_t cnt = 0, pos = 0;
    enum{SEARCH_ARG, HIT_ARG} state = SEARCH_ARG;
    
    for(pos = 0; cmdline[pos] != 0; ++pos)
    {
        switch(state)
        {
            case SEARCH_ARG:
                if('\x20' != cmdline[pos] && '\n' != cmdline[pos])
                {
                    state = HIT_ARG;
                    argv[cnt] = & cmdline[pos];
                }
                break;

            case HIT_ARG:
                if('\x20' == cmdline[pos] || '\n' == cmdline[pos])
                {
                    cmdline[pos] = 0;
                    state = SEARCH_ARG;
                    ++ cnt;
                    if(cnt > MAX_CMDLINE_ARGC)
                    {
                        return 1;
                    }
                }
                break;

            default:
                return 0;
        }
    }

    *argc = cnt;
    return 1;
}

/*****************************************************************************
//function :
//		print_ecnt_hook_help
//description : 
//		show ecnt_hook write proc cmd
//input :	
//		void
//return :
//		void
******************************************************************************/
static void print_ecnt_hook_help(void)
{
    printk("Command e.g.:\n"
           "    help   #show this message\n"
           "    get_hookfn <maintype> <subtype>  #get hook funtions with the same maintype and subtype\n"
           "    set_stop <maintype> <subtype> <hookid>   #set hook function stop\n"
           "    set_execute <maintype> <subtype> <hookid>   #set hook function execute\n"
           "    ops_unregister <maintype> <subtype> <hookid>  #set ops unregistered\n"
           );
}

/*****************************************************************************
//function :
//		ecnt_hook_read_proc
//description : 
//		/proc/ecnt_hook read function
//input :	
//		char *page, char **start, off_t off,int count, int *eof, void *data
//return :
//		0
******************************************************************************/
static int ecnt_hook_read_proc(char *page, char **start, off_t off,int count, int *eof, void *data)
{
	print_ecnt_hook_help();
	show_all_ecnt_hookfn();
	return 0;
}

/*****************************************************************************
//function :
//		ecnt_hook_write_proc
//description : 
//		/proc/ecnt_hook write function
//input :	
//		struct file *file, const char *buffer, unsigned long count, void *data
//return :
//		-EINVAL
//		-EFAULT
//		count
******************************************************************************/
static int ecnt_hook_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char string[64] = {0}, *argv[MAX_CMDLINE_ARGC];
    	size_t argc = 0;
	int maintype, subtype, hookid;
	
	if (count > sizeof(string) - 1)
		return -EINVAL ;

	if (copy_from_user(string, buffer, count))
		return -EFAULT ;

    	string[count] = 0;
    
    	prepare_params(string, &argc, argv);

	if(0 == argc){
		printk("Command needed!\n");
		print_ecnt_hook_help();
		return count;
	}

	if(!strcmp("help", argv[0]) ){
		print_ecnt_hook_help();
		return count;
	}
	else if(!strcmp("get_hookfn", argv[0]) ){
		if(3 != argc){
	    		printk("need 3 parameters get_hookfn <maintype> <subtype>\n");
	    		return count;
		}
		sscanf(argv[1], "%d", &maintype);
		sscanf(argv[2], "%d", &subtype);
		get_ecnt_hookfn(maintype, subtype);
	}
	else if(!strcmp("set_execute", argv[0]) ||!strcmp("set_stop", argv[0]) ||!strcmp("ops_unregister", argv[0])){
		if(4 != argc){
	    		printk("need 4 parameters <cmd> <maintype> <subtype> <hookid>\n");
	    		return count;
		}
		sscanf(argv[1], "%d", &maintype);
		sscanf(argv[2], "%d", &subtype);
		sscanf(argv[3], "%d", &hookid);
		if(!strcmp("set_execute", argv[0]))
			set_ecnt_hookfn_execute_or_not(maintype, subtype, hookid, 1);
		else if(!strcmp("set_stop", argv[0]))
			set_ecnt_hookfn_execute_or_not(maintype, subtype, hookid, 0);
		else if(!strcmp("ops_unregister", argv[0]))
			ecnt_ops_unregister(maintype, subtype, hookid);
	}
	else{
		print_ecnt_hook_help();
	}
	
	return count;
}

/*****************************************************************************
//function :
//		ecnt_hook_register_init
//description : 
//		called by module_init()
//input :	
//		void
//return :
//		-1: fail
//		0: success
******************************************************************************/
static int ecnt_hook_register_init(void)
{	
	struct proc_dir_entry *ecnt_hook_proc;
	int ret = 0;
	
	ecnt_hook_proc = create_proc_entry("ecnt_hook", 0, NULL);
	if(!ecnt_hook_proc){
		printk("ecnt_hook_register_init, create ecnt_hook proc fail\n");
		return -1;
	}
	ecnt_hook_proc->read_proc = ecnt_hook_read_proc;
	ecnt_hook_proc->write_proc = ecnt_hook_write_proc;

	ret = ecnt_register_hooks(ecnt_ops, ARRAY_SIZE(ecnt_ops));
	if (ret < 0) {
		pr_err("ecnt_hook_register_init: can't register hooks.\n");
		return -1;
	}
	
	return 0;
}

/*****************************************************************************
//function :
//		ecnt_hook_register_exit
//description : 
//		called by module_exit
//input :	
//		void
//return :
//		void
******************************************************************************/
static void ecnt_hook_register_exit(void)
{
	remove_proc_entry("ecnt_hook",NULL);
	ecnt_unregister_hooks(ecnt_ops, ARRAY_SIZE(ecnt_ops));
}

module_init(ecnt_hook_register_init);
module_exit(ecnt_hook_register_exit);

