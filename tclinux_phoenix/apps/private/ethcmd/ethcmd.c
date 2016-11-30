/************************************************************************
 *
 *	Copyright (C) 2009 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "libcmd.h"
#include "ethercmd.h"

extern cmds_t Cmds[];

/*___________________________________________________________________
**      function name: main
**      descriptions:
**         Ethcmd ci-cmd main function.
**
**      parameters:
**         argc: The arguments number.
**         argv: The vectors arguments 
**  
**      global:
**           Cmds
**             
**      return:
**          0: Success
**          -1: FAIL
**	    
**      call:
**           cmdHelp
**           initEthcmdDbg
**           init_swconfig
**           cmdparse
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int main(int argc, char **argv){
	int i=0;
	void *p=NULL;
	/*char  cmd_string[MAX_CMD_LEN]={0};*/
	struct swconfig *swcfg=NULL;
	swcfg=getSwcfg();
	#if 0 
	for(i=0; i<argc; i++){
		fprintf(stderr, "argv[%d]:%s ",i, argv[i]);	 
	}

	if(argv[1]==NULL){
		cmdHelp();
		return -1;
	}
	
	
	/*skip the ethcmd 1st arg(ethcmd)*/
	for(i=1; i<argc; i++){
		sprintf(cmd_string, "%s %s", cmd_string, argv[i]);
	}
	#endif
	#ifdef ETHCMD_DBG
	initEthcmdDbg();
	#endif
	//If init fail, socket will be closed. We can not close socket twice. shnwind 20110317.
	if(init_swconfig() == 0){
	subcmd(Cmds, argc, argv, p);
	close(swcfg->skfd);
	}	
	return 0;
}/*end main*/
