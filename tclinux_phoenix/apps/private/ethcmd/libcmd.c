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

/*___________________________________________________________________
**      function name: subcmd
**      descriptions:
**        Register to root ci-cmd of command table.
**
**      parameters:
**         tab: Root ci-cmd table.
**         argc: The arguments number.
**         argv: The vectors arguments 
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         0:succuces
**         -1:fail
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int subcmd(
	const cmds_t tab[], int argc, char *argv[], void *p
)
{
	const cmds_t *cmdp;
	int found = 0;
	int i;

	/* Strip off first token and pass rest of line to subcommand */
	if (argc < 2) {
		if (argc < 1)
			printf("SUBCMD - Don't know what to do?\n");
		else 
			goto print_out_cmds;
			//printk("\"%s\" - takes at least one argument\n",argv[0]);
		return -1;
	}
	argc--;
	argv++;
	for(cmdp = tab;cmdp->name != NULL;cmdp++){
		if(strlen(argv[0]) == strlen(cmdp->name)){		// add length exact match checking, to avoid conflict between "pbr" and "pbrw-test"
			if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0){
				found = 1;
				break;
			}
		}
	}
	if(!found){
		char buf[66];
		
print_out_cmds:
		printf("valid subcommands:\n");
		memset(buf,' ',sizeof(buf));
		buf[64] = '\n';
		buf[65] = '\0';
		for(i=0,cmdp = tab;cmdp->name != NULL;cmdp++){
			strncpy(&buf[i*16],cmdp->name,strlen(cmdp->name));
			if(i == 3){
				printf(buf);
				memset(buf,' ',sizeof(buf));
				buf[64] = '\n';
				buf[65] = '\0';
			}
			i = (i+1)%4;
		}
		if(i != 0)
			printf(buf);
		return -1;
	}
	if(argc <= cmdp->argcmin){
		if(cmdp->argc_errmsg != NULL)
			printf("Usage: %s\n",cmdp->argc_errmsg);
		return -1;
	}
	
	if(cmdp->func==NULL){
		return 0;
	}
	else{
		return (*cmdp->func)(argc,argv,p);
	}
}/*end subcmd*/
#if 0
/*___________________________________________________________________
**      function name: stringparse
**      descriptions:
**        Parser  the ci-cmd string.
**
**      parameters:
**         line: ci-cmd string.
**         
**      global:
**           None
**             
**      return:
**         cmd strings.
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
static char *
stringparse(char *line)
{
	register char *cp = line;
	unsigned long num;

	while ( *line != '\0' && *line != '\"' ) {
		if ( *line == '\\' ) {
			line++;
			switch ( *line++ ) {
			case 'n':
				*cp++ = '\n';
				break;
			case 't':
				*cp++ = '\t';
				break;
			case 'v':
				*cp++ = '\v';
				break;
			case 'b':
				*cp++ = '\b';
				break;
			case 'r':
				*cp++ = '\r';
				break;
			case 'f':
				*cp++ = '\f';
				break;
			case 'a':
				*cp++ = '\a';
				break;
			case '\\':
				*cp++ = '\\';
				break;
			case '\?':
				*cp++ = '\?';
				break;
			case '\'':
				*cp++ = '\'';
				break;
			case '\"':
				*cp++ = '\"';
				break;
			case 'x':
				/*
				num = simple_strtoul( --line, &line, 16 );
				*/
				num=atoi(--line);
				*cp++ = (char) num;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
				/*
				num = simple_strtoul( --line, &line, 8 );
				*/
				num=atoi(--line);
				*cp++ = (char) num;
				break;
			case '\0':
				return NULL;
			default:
				*cp++ = *(line - 1);
				break;
			};
		} else {
			*cp++ = *line++;
		}
	}

	if ( *line == '\"' )
		line++; 	/* skip final quote */
	*cp = '\0';		/* terminate string */
	return line;
}/*end stringparse*/
/*___________________________________________________________________
**      function name: rip
**      descriptions:
**        Replace terminating end of line marker(s) with null 
**
**      parameters:
**         s: ci-cmd string.
**         
**      global:
**           None
**             
**      return:
**         None
**	    
**      call:
**           None
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
/* replace terminating end of line marker(s) with null */
void 
rip(register char *s)
{
	register char *cp;

	if((cp = strchr(s,'\n')) != NULL)
		*cp = '\0';
	if((cp = strchr(s,'\r')) != NULL)
		*cp = '\0';
}/*end rip*/
/*___________________________________________________________________
**      function name: cmdparse
**      descriptions:
**        Accroding the ci-cmd string to find out the function to do it.
**
**      parameters:
**         cmds: Root ci-cmd table.
**         line: Root ci-cmd table.
**         p:function pointer
**         
**      global:
**           None
**             
**      return:
**         None
**	    
**      call:
**           rip
**           stringparse
**      
**      revision:
**      1. Here 2009/11/7
**_______________________________________________________________
*/
int 
cmdparse(
	const cmds_t cmds[], char *line, void *p
)
{
	const cmds_t *cmdp;
	char *argv[NARG],*cp;
	int argc;

	/* Remove cr/lf */
	rip(line);

	for(argc = 0;argc < NARG;argc++)
		argv[argc] = NULL;

	for(argc = 0;argc < NARG;){
		register int qflag = FALSE;

		/* Skip leading white space */
		while(*line == ' ' || *line == '\t')
			line++;
		if(*line == '\0')
			break;
		/* return if comment character first non-white */
		if ( argc == 0  &&  *line == '#' )
			return 0;
		/* Check for quoted token */
		if(*line == '"'){
			line++;	/* Suppress quote */
			qflag = TRUE;
		}
		argv[argc++] = line;	/* Beginning of token */

		if(qflag){
			/* Find terminating delimiter */
			if((line = stringparse(line)) == NULL){
				return -1;
			}
		} else {
			/* Find space or tab. If not present,
			 * then we've already found the last
			 * token.
			 */
			if((cp = strchr(line,' ')) == NULL
			 && (cp = strchr(line,'\t')) == NULL){
				break;
			}
			*cp++ = '\0';
			line = cp;
		}
	}
	if (argc < 1) {		/* empty command line */
		argc = 1;
		argv[0] = "";
	}
	/* Look up command in table; prefix matches are OK */
	for(cmdp = cmds;cmdp->name != NULL;cmdp++){
		if(strncmp(argv[0],cmdp->name,strlen(argv[0])) == 0) {
			break;
		}
	}
	if(cmdp->name == NULL) {
		if(cmdp->argc_errmsg != NULL)
			printf("%s\n",cmdp->argc_errmsg);
		return -1;
	}
	if(argc < cmdp->argcmin) {
		/* Insufficient arguments */
		printf("Usage: %s\n",cmdp->argc_errmsg);
		return -1;
	}
	if(cmdp->func == NULL)
		return 0;
	
	return (*cmdp->func)(argc,argv,p);
}/*end cmdparse*/
#endif
