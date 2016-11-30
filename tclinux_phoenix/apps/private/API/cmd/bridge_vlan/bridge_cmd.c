#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <errno.h>
#include <asm/param.h>
#include <linux/version.h>
#include <ecnt_lib_dbg.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <netinet/in.h>
#include <libapi_lib_flowmgr.h>
#include <linux/if_bridge.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <libapi_lib_brtable.h>
#include "bridge_cmd.h"

void brtable_show(char **argv) {
	int port_index;
	int num;
	int i;
	struct fdb_entry *entry = NULL;
	
	if(!(argv && argv[0] && 1 == sscanf(argv[0], "%d", &port_index))) {
		printf("param error\n");
		return ;
	}

	if(!(argv && argv[1] && 1 == sscanf(argv[1], "%d", &num))) {
		printf("param error\n");
		return ;
	}

	entry = malloc(num * sizeof(struct fdb_entry));
	if(!entry) {
		printf("alloc memory fail!\n");
		return ;
	}

	if(0 != brmgr_lib_get_brtable(port_index, entry, &num)) {
		printf("get brtable fail!\n");
		free(entry);
		return ;
	}
	
	printf("port_no\tmac\t\t\tvid\n");
	for(i=0; i<num; i++) {
		printf("%3i\t", entry[i].port_no);
		printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t", entry[i].mac_addr[0], entry[i].mac_addr[1], entry[i].mac_addr[2],
		entry[i].mac_addr[3], entry[i].mac_addr[4],entry[i].mac_addr[5]);
		if(entry[i].vlan_layer) {
			printf("%d\n", entry[i].vlan_id);
		}else {
			printf("N\n");
		}
	}
	
	free(entry);

}

void brtable_setage(char **argv) {
	double time = 0;

	if(!(argv && argv[0] && 1 == sscanf(argv[0], "%lf", &time))) {
		printf("param error\n");
		return ;
	}

	if(0 != brmgr_lib_set_brtable_age(time)) {
		printf("set brtable age error!\n");
		return;
	}
	
}

void brtable_clean(char **argv)
{
	if(0 != brmgr_lib_clean_brtable()) 
	{
		printf("clean brtable fail!\n");
		return;
	}
}

int main(int argc, char **argv) {
	
	
	int i;

	if(argc < 2) {
		printf("usage:\n\t%s show port_index num\n\t%s setage time(us)\n", argv[0], argv[0]);
		return -1;
	}

	for(i=0; i<sizeof(cmd)/sizeof(cmd[0]); i++) {
		if( !strcmp(cmd[i].cmd, argv[1]) ) {
			cmd[i].func(argv+2);
		}
	}
	
	
}