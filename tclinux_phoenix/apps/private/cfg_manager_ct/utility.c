/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#include <stdio.h>
#include "cfg_manager.h"
#include "utility.h"
#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
#include <stdint.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#endif
#if defined(TCSUPPORT_CT_PON)
#include <dirent.h>
#endif
#include <linux/version.h>
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
#include "sim.h"
#endif

//#include "../lib/libtcapi.h"
#define KERNEL_2_6_36 	(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))


int create_simThread(pthread_t *pth, pthread_attr_t *pattr)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	if( pthread_create(pth, pattr, (void *)simThread, NULL) != 0 ){
		//fprintf(stderr, "pthread_create simTread error!!\n");
		//exit(0);
		return -1;
	}
#endif
	return 0;
}

mxml_node_t* ct_simCardInit(mxml_node_t *top)
{
#if defined(TCSUPPORT_CT_SIMCARD_SEPARATION)
	return simCardInit(top);
#endif

	return NULL;
}


/*_____________________________________________________________________________
**      function name: splitName
**      descriptions:
**            Split the node name into 3-level name.
**            ext: VirServer_PVC0_Entry0
**            nodeName[0]:VirServer
**            nodeName[1]:PVC0
**            nodeName[2]:Entry0
**
**      parameters:
**            tree:   Put the top of cfg node tree that is used to find out value.
**            node:  Put the parant of cfg node.
**            name: Put the split node name.
**            msg:   Put the message that is got from tcapi library interface.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 create_romfile
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
splitName(char *name, char buf[][MAX_NODE_NAME], const char* delim){
	char *pValue=NULL;
	char tmp[32]={0};
	int i=0;

	strncpy(tmp, name, sizeof(tmp));
	/*Get parent node name*/
	strtok(tmp,delim);
	strcpy(buf[0],tmp);
	for(i=1; i < MAX_ARG_NUM; i++){
		pValue=strtok(NULL, delim);
		if(pValue){
			strcpy(buf[i], pValue);
		}
		else{
			break;
		}
	}
}/*end splitName*/
/*_____________________________________________________________________________
**      function name: check_ip_exist
**      descriptions:
**            To check if interface get ip address or not by pvc and isp.
**
**      parameters:
**            interface:   WAN or LAN
**                  pvc:   PVC index
**                  isp:   ISP type
**           check_flag:   A switch to decide to run the IP detection
**
**      global:
**             None
**
**      return:
**             0: SUCCESS
**            -1: FAIL
**
**      call:
**   	      None
**
**      revision:
**      1. rodney_20090420
**____________________________________________________________________________
*/
int check_ip_exist(char *interface, int pvc, int isp, int check_flag){
    char string_temp[100];
    FILE *fd;

    if(check_flag == 0)  /*if this variable is 0, there is no need to check IP address*/
    	return FAIL;

    if(strcmp(interface, "WAN") == 0){  /*check WAN interface*/
    	if(isp == WAN_ENCAP_BRIDGE){
    		return SUCCESS;  /*for bridge mode, there is no need to check IP address*/
    	}
        else if(isp == WAN_ENCAP_PPP){
            sprintf(string_temp,"ifconfig ppp%d | sed -n '2p' > /tmp/iptemp.tmp",pvc);
        }
        else{
        	sprintf(string_temp,"ifconfig nas%d | sed -n '2p' > /tmp/iptemp.tmp",pvc);
        }
    }
    else if(strcmp(interface, "LAN") == 0){  /*check LAN interface*/
    	sprintf(string_temp,"ifconfig br0 | sed -n '2p' > /tmp/iptemp.tmp");
    }
    else{  /*error interface*/
    	fprintf(stderr, "Error interface type\n");
    	return FAIL;
    }
    system(string_temp);
    fd = fopen("/tmp/iptemp.tmp", "r");
    if(fd == NULL){
    	return FAIL;
    }
	fgets(string_temp, 100, fd);
	fclose(fd);
	system("rm -r /tmp/iptemp.tmp");
	if(strstr(string_temp, "inet addr:") == 0){
        return FAIL;
	}
	else{
		return SUCCESS;
	}
}/*end check_ip_exist*/

/*_____________________________________________________________________________
**      function name: getAttrValue
**      descriptions:
**            Try to find out attribution-value of you specify the cfg node.
**
**      parameters:
**            top:   Put the top of cfg node tree that is used to find out value.
**            name:  Cfg node name
**            attr:  Put the attribution that you want to get the value.
**            buf:   Put the buffer
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlElementGetAttr
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
int
getAttrValue(mxml_node_t *top,  char name[][MAX_NODE_NAME], char* attr, char* buf){

	mxml_node_t *curNode=top;
	const char *pValue=NULL;
	int i=0;

	for(i=0;i < MAX_ARG_NUM; i++){
		if(strlen(name[i])==0){
			break;
		}
		curNode=mxmlFindElement(curNode, curNode, name[i],
				NULL, NULL, MXML_DESCEND);
	}

	if(curNode){
		pValue=mxmlElementGetAttr(curNode, attr);
		if(pValue){
			strcpy(buf, pValue);
		}
		else{
			return TCAPI_NONE_ATTR;
		}
	}
	else{
		return TCAPI_NONE_NODE;
	}

	return TCAPI_PROCESS_OK;
}/*end getAttrValue*/
#if 1  /*Rodney_20090724*/
/*_____________________________________________________________________________
**      function name: setAttrValue
**      descriptions:
**            Try to write value to the node which  you specify.
**
**      parameters:
**            top:   Put the top of cfg node tree that is used to find out value.
**            name:  Cfg node name
**            attr:  Put the attribution that you want to get the value.
**            value:   Put the value you want to write .
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlElementSetAttr
**     mxmlNewElement
**      revision:
**
**____________________________________________________________________________
*/

int
setAttrValue(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr, char *value){

  int i;
  mxml_node_t *curNode=top;

  mxml_node_t *parentNode = top;
  
  for(i=0;i < MAX_ARG_NUM; i++){
		if(strlen(name[i]) <= 0){
			break;
		}
		parentNode = curNode;
		curNode=mxmlFindElement(curNode, curNode, name[i],
				NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			/*There is no cfg node information, so we add a sub node into cfg node tree*/
			curNode = mxmlNewElement(parentNode, name[i]);
		}
	}

	if(curNode){
		mxmlElementSetAttr(curNode, attr, value);
	}else{
		curNode = mxmlNewElement(parentNode, name[i - 1]);  /*Rodney_20090724*/
		if (curNode) { /* xyzhu_nj */
			mxmlElementSetAttr(curNode, attr, value);
		}
	}

	return TCAPI_PROCESS_OK;
}/*end SetAttrValue*/
#endif
/*_____________________________________________________________________________
**      function name: tcapi_commit_internal
**      descriptions:
**            Try to commit cfg node, which used by cfg_manager internal
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            node_name: the name of node which needed to be commited 
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**		tcapi_commit_req
**      revision:
**
**____________________________________________________________________________
*/
int tcapi_commit_internal(mxml_node_t *top, char *node_name)
{
	tcapi_msg_t msg;
	memset(&msg,0,sizeof(msg)) ;

	strcpy(msg.node, node_name);
	tcapi_commit_req(top, &msg);

	return msg.retval;
}

/*_____________________________________________________________________________
**      function name: tcapi_set_internal
**      descriptions:
**            Try to set cfg node value, which used by cfg_manager internal
**
**      parameters:
**            top:   Put the top of cfg node tree.
**            node_name: the name of node which needed to be commited 
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**		tcapi_commit_req
**      revision:
**
**____________________________________________________________________________
*/
int tcapi_set_internal(mxml_node_t *top, char *node_name, char *attribute, char *value)
{
	tcapi_msg_t msg;
	memset(&msg,0,sizeof(msg)) ;

	strcpy(msg.node, node_name);
	strcpy(msg.attr, attribute);
      	strcpy(msg.value, value);
      	tcapi_set_req(top, &msg);

	//tcdbg_printf("tcapi_set_internal:%s,%s\n", attribute, value);
	//tcdbg_printf("tcapi_set_internal:reval:%d\n", msg.retval);

	return msg.retval;
}
/*_____________________________________________________________________________
**      function name: fileRead
**      descriptions:
**           Read file context and copy information into buf.
**
**      parameters:
**            path:  File path
**            buf:  Specify the buffer that you want to store it.
**            size: Specify the size that your want to read form file.
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 None
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
fileRead(char *path, char *buf, int size){
	int  fd=0, ret=0;

	memset(buf,0, size);
	fd = open(path,O_RDONLY);
	if(fd == FAIL){
		return;
	}

	ret = read(fd, buf, size);
	if(ret <= 0){
		close(fd);
		return;
	}

	close(fd);
}/*end fileRead*/

/*_____________________________________________________________________________
**      function name: kill_process
**      descriptions:
**           Kill user-space process.
**
**      parameters:
**            pid_path:  Specify the pid path that you want to kill it.
**
**      global:
**             fileRead
**
**      return:
**             None
**
**      call:
**   	 None
**
**      revision:
**      1. shnwind 2008/6/1
**____________________________________________________________________________
*/
void
kill_process(char *pid_path){
	char pid[8]={0};

	fileRead(pid_path, pid, sizeof(pid));
	if(strlen(pid) > 0){
		kill(atoi(pid), SIGKILL);
		unlink(pid_path);
	}
	#if 0
	/*Only to debug area*/
	else{
		fprintf(stderr,"Not found %s file",pid_path);
	}
	#endif
}/*end kill_procesS*/

/*______________________________________________________________________________
** get_profile_str
**
** descriptions:
**     Get the value of specify keyword on the configuration file. The max length of a line in the profile
**     should less than 128!
**
** parameters:
**  keyname:   keyword.
**  str_return:  Put the value into det.
**  type:   NO_QMARKS: The value is not included  QuotationMarks character
**      		QMARKS:	 The value is included  QuotationMarks character
**  size:		the size of str_return
**  fd:    file descript.
**
** return:
**  Success:  length of value(may be zero!!).
**  Otherwise:  FAIL(-1).
**
**	revision:
**      1. krammer 2008/11/14
**____________________________________________________________________________
*/
int
get_profile_str(char *keyname,char *str_return, int size, int type, char *path){
#if 0
	char *str_key= NULL,*str_enter= NULL;
	char stream[200];
	int fd,len,ret,key_len;
	off_t pos;

	fd = open(path,O_RDONLY);
	if(fd == -1){
		//fprintf(stderr,"%s not found\n",path);
		return -1;
	}
	else{

		lseek(fd,0L,SEEK_SET);
		while((len=read(fd,stream,200))>0){
			pos = lseek(fd,0,SEEK_CUR);
			if(pos == -1L){
				close(fd);
				return FAIL;
			}
			pos = pos-len;

			str_key = strstr(stream,keyname);
			str_enter = strrchr(stream,'\n');

			if(str_enter == NULL){
				return FAIL;
			}
			if(str_key == NULL){
				pos = pos + (str_enter-stream) +1;
				if(lseek(fd,pos,SEEK_SET)== -1L){
					close(fd);
					return FAIL;
				}else{
					continue;
				}
			}else{
				str_enter = strchr(str_key,'\n');
				if(str_enter == NULL){
					pos = pos + (str_key-stream);
				if(lseek(fd,pos,SEEK_SET)== -1L){
					close(fd);
					return FAIL;
				}else{
					continue;
				}
				}else{
					key_len = strlen(keyname);
					ret = str_enter-(str_key+key_len);
					if(type == NO_QMARKS){
						memcpy(str_return,str_key+key_len,ret);
						str_return[ret]= '\0';
					}else{ /*QMARKS*/
						if(ret >= QM_LEN){
							memcpy(str_return, str_key+key_len+1, ret-QM_LEN);
							str_return[ret-2]= '\0';
							fprintf(stderr, "length %d str_return %s\n", ret-QM_LEN, str_return);
						}else{
							memset(str_return, 0, sizeof(str_return));
						}
					}
					#if 0
					str_return[ret]= '\0';
					#endif
					close(fd);
					return ret+1;
				}
			}
		}


	}
#endif

//#if 0
/*

profile fomat

example

keyworld length
<--------->
DefaultRoute="YES"\n
<--------------->
      stream length

so if we want to return YES, we should get the pointer which point to the "Y",
and catch the length of "YES" characters.

to get the pointer point to "Y": Y should locate at the (stream length) - (keyworld length)
and should also consider if there the type is qmark, we should skip one character(skipQmark)
because we don't want to get the left qmark.

to get the length of "YES", we can use (stream length) - (keyworld length).
Note 1:
	if the type is qmark, we should substract 2(qmarkLength) because we don't want to get qmark.
Note 2:
	if there is a '\n', we should substract 1(enterOffSet) because we don't want to get '\n'

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Care!!!!!!!!!!!!!!!!!!!!!!!!!!!!
the size of this example can't be less or eaual to 3!!!!!
because we will return a stream "Yes\0", so the minimum size should be 4.

*/
	FILE *fp;
	char *str_key= NULL;//,*str_enter= NULL;
	//int buffersize=100;
	char stream[MAXGET_PROFILE_SIZE]={0};
	int enterOffSet=1;
	int qmarkLength=0;
	int skipQmark=0;
	int totalLength=0;
	//int pos=0;

	fp=fopen(path,"r");
	if(fp==NULL){
		fprintf(stderr,"Can't open %s\n",path);
		return FAIL;
	}

	memset(str_return, 0, size);
	fseek(fp, 0, SEEK_SET);
	if(type==QMARKS){
		qmarkLength=2;
		skipQmark=1;
	}
	else if(type == NO_QMARKS){
		qmarkLength=0;
		skipQmark=0;
	}
	else{
		fprintf(stderr, "The input qmark type of get_profile_str is wrong \n");
		fclose(fp);
		return FAIL;
	}

	while(fgets(stream, MAXGET_PROFILE_SIZE, fp) != NULL){
		//str_enter = strrchr(stream,'\n');
		if(strrchr(stream,'\n') == NULL){
			enterOffSet=0;
		}
		else{
			enterOffSet=1;
		}

		str_key = strstr(stream,keyname);
		if(str_key == NULL || str_key != stream){
			continue;
		}

		totalLength=strlen(stream)-strlen(keyname)-enterOffSet-qmarkLength;
		if(size<totalLength+1){/*total length + '\0' should not less than buffer*/
			fprintf(stderr, "Too small buffer to catch the %s frome get_profile_str\n", keyname);
			fclose(fp);
			return FAIL;
		}
		else if(totalLength<0) {/*can't get a negative length string*/
			fprintf(stderr, "No profile string can get\n");
			fclose(fp);
			return FAIL;
		}
		else{
			strncpy(str_return, stream+strlen(keyname)+skipQmark, totalLength);
			str_return[totalLength]= '\0';
			//fprintf(stderr,"str_return:%s,str_length%d\n",str_return,strlen(str_return));
			fclose(fp);
			return strlen(str_return);
		}
		#if 0
		else{
			if(type == NO_QMARKS){
				if((strlen(stream)-strlen(keyname))>size){
					fprintf(stderr, "Too small buffer to catch the %s frome get_profile_str\n", keyname);
					fclose(fp);
					return FAIL;
				}
				strncpy(str_return, stream+strlen(keyname), strlen(stream)-strlen(keyname));
				str_return[strlen(stream)-strlen(keyname)-1]= '\0';
				fclose(fp);
				return SUCCESS;
			}
			else if(type == QMARKS){
				if((strlen(stream)-strlen(keyname)-2)>size){
					fprintf(stderr, "Too small buffer to catch %s string frome get_profile_str\n", keyname);
					fclose(fp);
					return FAIL;
				}
				//fprintf(stderr, "strlen(stream)= %d length = %d\n", strlen(stream), strlen(stream)-strlen(keyname)-3);
				strncpy(str_return, stream+strlen(keyname)+1, strlen(stream)-strlen(keyname)-3);
				str_return[strlen(stream)-strlen(keyname)-3]= '\0';
				fclose(fp);
				return SUCCESS;
			}
			else{

			}
		}
		#endif
		memset(stream, 0, MAXGET_PROFILE_SIZE);
	}
fclose(fp);
fprintf(stderr,"File %s content %s is worng\n",path,keyname);
return FAIL;
}/* end get_profile_str */
int
get_profile_str_new(char *keyname,char *str_return, int size,char *path)
{
	FILE *fp;
	char *str_key= NULL;//,*str_enter= NULL;
	//int buffersize=100;
	char stream[MAXGET_PROFILE_SIZE]={0};
	int totalLength=0;
	char *p = NULL;

	fp=fopen(path,"r");
	if(fp==NULL){
		fprintf(stderr,"Can't open %s\n",path);
		return FAIL;
	}

	memset(str_return, 0, size);
	fseek(fp, 0, SEEK_SET);

	while(fgets(stream, MAXGET_PROFILE_SIZE, fp) != NULL)
	{
		str_key = strstr(stream,keyname);
		if(str_key == NULL || str_key != stream)
		{
			continue;
		}	

		p = strtok(stream,"\r");
		while(p)
		{
			p = strtok(NULL,"\r");
		}

		p = strtok(stream,"\n");
		while(p)
		{
			p = strtok(NULL,"\n");
		}

		totalLength=strlen(stream)-strlen(keyname);
		if(size<totalLength+1){/*total length + '\0' should not less than buffer*/
			fprintf(stderr, "Too small buffer to catch the %s frome get_profile_str_new\n", keyname);
			fclose(fp);
			return FAIL;
		}
		else if(totalLength<0) {/*can't get a negative length string*/
			fprintf(stderr, "No profile string can get\n");
			fclose(fp);
			return FAIL;
		}
		else{
			strncpy(str_return, stream+strlen(keyname), totalLength);
			str_return[totalLength]= '\0';
			fclose(fp);
			return strlen(str_return);
		}
		memset(stream, 0, MAXGET_PROFILE_SIZE);
	}
fclose(fp);
fprintf(stderr,"File %s content %s is worng\n",path,keyname);
return FAIL;
}/* end get_profile_str */
/*_____________________________________________________________________________
**      function name: mergeNodeName
**      descriptions:
**           In current ASP function isn't support appends a copy of the source string to
**           the destination string.So we need to help to merge cfg node name.
**
**      parameters:
**            top:   Put the top of cfg node tree that is used to find out value.
**            name:  Cfg node name
**            attr:  Put the attribution that you want to get the value.
**            buf:   Put the buffer
**
**      global:
**             None
**
**      return:
**             None
**
**      call:
**   	 mxmlFindElement
**   	 mxmlElementGetAttr
**
**      revision:
**      1. Here 2008/5/7
**____________________________________________________________________________
*/
void
mergeNodeName(mxml_node_t *top,char name[][MAX_NODE_NAME]){
	int valid = FAIL;
	char cur_id[64]={0};
	char nodeName[3][MAX_NODE_NAME];
	cfg_node_t* pAllCfgNode=NULL;
	int i=0;


	/*Find out the node index*/
	for(pAllCfgNode=getAllCfgTable(); pAllCfgNode->name != NULL; pAllCfgNode++){
		if(strcmp(name[0], pAllCfgNode->name) == 0){
			valid=0;
			break;
		}
	}

 	#if DEBUG
	fprintf(stderr,"mergeNodeName func: type:%x name:%s %s %s\r\n",pAllCfgNode->type,name[0],name[1],name[2]);
	#endif

	if(valid==FAIL){
		return;
	}

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WEBCURSET);
	strcpy(nodeName[1], SUB_NODE_NAME);

	if(pAllCfgNode->merge_table !=NULL){
		for(i=0;strlen(pAllCfgNode->merge_table[i]) !=0; i++){
			if( ((pAllCfgNode->type & PVC_TYPE) && (strcmp(name[i+1], WAN_PVC)==0))
			||((pAllCfgNode->type & ENTRY_TYPE) && (strcmp(name[i+1],SUB_NODE_NAME)==0))
			){
				if(getAttrValue(top, nodeName, pAllCfgNode->merge_table[i], cur_id) == 0){
					strcat(name[i+1],cur_id);
				}
			}
		}
	}


}/*end mergeNodeName*/


#ifndef PURE_BRIDGE

/*______________________________________________________________________________
**	function name:decideModulePath
**
**	description:
*     decide module path according to the kernel version
**	parameters:
*     		vername: used to obtain kernel version
**	global:
*     		none
**	return:
*     		none
**	call:
*     		none
**	revision:
*     1.Ian 20091129
**____________________________________________________________________________*/
void decideModulePath(char* vername){
	FILE * Ver;
	char buf[50], *start, *end;

	Ver = fopen ("/proc/version","r");
	if(!Ver){
		tcdbg_printf("Failed to open /proc/version\r\n");
		return;
	}
	fgets(buf, 50, Ver);
	fclose(Ver);
	start = strchr(buf, ' ');
	start = strchr(start + 1, ' ') + 1;
	end = strchr(start + 1, ' ');
	*end = '\0';
	strcpy(vername, start);
}

/*______________________________________________________________________________
**	function name:check_and_set_filter
**
**	description:
*     check whether to load the filter module or not.
**	parameters:
*     		new_filter_state: the next filter state which system should have
**	global:
*     		pre_sys_state
**	return:
*     		new filter state
**	call:
*     		none
**	revision:
*     1.shnwind 20080530
*     2.krammer 20090211
**____________________________________________________________________________*/
#if 0
int
check_and_set_filter(mxml_node_t *top, int filter_state)
#endif
char
check_and_set_filter(unsigned int new_filter_state)
{
#if 0
	int i=0, acl_flag=0, filter_on=0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[4]={0}, isp[4]={0}, mtu[6]={0}, mss[6]={0};
	char firewall_status[2]={0}, spi_status[2]={0}, acl_status[4]={0}, acl_rule_status[8]={0};
	char element_name[16]={0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	/*	in this case must load filter module:
		mss is not equal 0 in ppp mode | mtu  is not equal 0 in dynamic and static mode | firewall is active.
	*/
	for(i = 0 ; i < PVC_NUM; i++){
		/*nodeName[0]="Wan", nodeName[1]="PVCn", nodeName[2]=""*/
		sprintf(nodeName[1], SUB_PVC_NAME,i);
		if((getAttrValue(top, nodeName, NODE_ACTIVE, active) != TCAPI_PROCESS_OK)
		||(getAttrValue(top, nodeName, WAN_ISP, isp) != TCAPI_PROCESS_OK)){
			continue;
		}
		getAttrValue(top, nodeName, "MTU", mtu);
		getAttrValue(top, nodeName, "MSS", mss);
		/*must active and isp is not bridge mode*/
		if((!strcmp(active,ACTIVE)) && (atoi(isp) != WAN_ENCAP_BRIDGE)){
			/*check mtu is default value or not*/
			if((atoi(isp) == WAN_ENCAP_DYN) || (atoi(isp) == WAN_ENCAP_STATIC)){
				if((atoi(mtu) == 0) || (atoi(mtu) == 1500)){
					continue;
				}else{
					filter_on = 1;
					break;
				}
			}else if(atoi(isp) == WAN_ENCAP_PPP){
				/*check mss is default value or not*/
				/*the filter must on no matter mss is default or not in ppp mode
				krammer 20081024*/
				#if 0
				if((atoi(mss) == 0) || (atoi(mss) == 1492)){
					continue;
				}else{
				#endif
					filter_on = 1;
					break;
				#if 0
				}
				#endif
			}else{
				continue;
			}
		}else{
			continue;
		}
	}
	/*if filter is equal 0, check the firwall is active or not.*/
	if(filter_on == 0){
		strcpy(nodeName[0],FIREWALL);
		strcpy(nodeName[1],SUB_NODE_NAME);
		if((getAttrValue(top, nodeName, "firewall_status", firewall_status) == TCAPI_PROCESS_OK)
		&&(getAttrValue(top, nodeName, "spi_status", spi_status) == TCAPI_PROCESS_OK)){
			if(!strcmp(firewall_status,"1") || !strcmp(spi_status,"1")){
				filter_on = 1;
			}
		}
	}

	/**********20080826 racing add************/
	if(filter_on == 0){
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], ACL);

		for(i=0; i<MAX_ACL_RULE; i++){
			sprintf(element_name,"%s%d", SUB_NODE_NAME, i);
			strcpy(nodeName[1], element_name);

			if(getAttrValue(top, nodeName, "Activate", acl_rule_status) != TCAPI_PROCESS_OK){
				/*fprintf(stderr, "Error occurs while getting ACL_Entry Activate value.\n");*/
				continue;
			}

			if(!strcasecmp(acl_rule_status, "Yes")){
				acl_flag=1;
				break;
			}

		}

		memset(nodeName,0,sizeof(nodeName));

		strcpy(nodeName[0], ACL);
		strcpy(nodeName[1], ACL_COMMON);
		/*get current information*/
		if(getAttrValue(top, nodeName, "Activate", acl_status) == TCAPI_PROCESS_OK){
			if((!strcasecmp(acl_status, "yes"))&&(acl_flag!=0)){
				filter_on = 1;
			}
		}
	}
	/**********20080826 racing add************/

	/**********20081007 racing add************/
	if(filter_on == 0){
		filter_on = ipfilter_check_active(top);
/*
		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0], IPMACFILTER);

		for(i=0;i<MAX_IPMACFILTER_RULE; i++){
			sprintf(element_name,"%s%d", SUB_NODE_NAME, i);
			strcpy(nodeName[1], element_name);

			if(getAttrValue(top, nodeName, "Active", ipmacfilter_rule_status) != TCAPI_PROCESS_OK){
				continue;
			}

			if(!strcasecmp(ipmacfilter_rule_status, "yes")){
				filter_on=1;
				break;
			}
		}
*/
	}

	/**********20081007 racing add************/

	/**********20081022 krammer add************/
	pre_sys_state.l7filter_state= check_and_set_l7filter(top, pre_sys_state.l7filter_state);
	if(filter_on==0){
		filter_on=pre_sys_state.l7filter_state;
	}

	/**********20081022 krammer add************/

	/*check the sytem state */
	if(filter_state != filter_on){
		if(filter_on){
			fprintf(stderr,"========================insmod iptable_filter=======================\n");
#if KERNEL_2_6_36
			system("insmod /lib/modules/2.6.36/kernel/net/ipv4/netfilter/iptable_filter.ko");
#else
			system("insmod /lib/modules/2.6.21.5/kernel/net/ipv4/netfilter/iptable_filter.ko");
			system("insmod /lib/modules/2.6.22.15/kernel/net/ipv4/netfilter/iptable_filter.ko");			
#endif
			/*krammer add 20081022: use default chain*/
			system(LOAD_DEFAULT_CHAIN);
		}else{
		fprintf(stderr,"========================flush iptable_filter=======================\n");
			system("iptables -t filter -F");
			/*krammer add 20081022: delete default chain*/
			fprintf(stderr,"========================flush default chain=======================\n");
			system(FLUSH_DEFAULT_CHAIN);
		fprintf(stderr,"========================rmmod iptable_filter=======================\n");
			system("rmmod iptable_filter");
		}
	}
	return filter_on;
#endif
	if(pre_sys_state.filter_m_state != 0){//now filter is on
		if(new_filter_state == 0){
			fprintf(stderr,"========================flush iptable_filter=======================\n");
			system("iptables -t filter -F");
			/*krammer add 20081022: delete default chain*/
			fprintf(stderr,"========================flush default chain=======================\n");
			system(FLUSH_DEFAULT_CHAIN);
#if defined(TCSUPPORT_CT_PON_C7)
			system("iptables -t filter -X dhcpfilter_chain");
#endif
			//fprintf(stderr,"========================rmmod iptable_filter=======================\n");
			tcdbg_printf("========================rmmod iptable_filter=======================\n");
			system("rmmod iptable_filter");
		}
	}
	else{//now filter is down
		if(new_filter_state != 0){
			char vername[15], module_path[100];
			char *path_temp = "insmod /lib/modules/%s/kernel/net/ipv4/netfilter/iptable_filter.ko";

			decideModulePath(vername);
			sprintf(module_path, path_temp, vername);
			tcdbg_printf("========================insmod iptable_filter=======================\n");
			system(module_path);
			/*krammer add 20081022: use default chain*/
			system(LOAD_DEFAULT_CHAIN);
#if defined(TCSUPPORT_CT_PON_C7)
			system("iptables -t filter -N dhcpfilter_chain");
			system("iptables -t filter -I INPUT -j dhcpfilter_chain");
#endif
		}
	}
	return new_filter_state;
}
/*______________________________________________________________________________
**	function name:check_nat_enable
**
**	description:
*     check nat status of all pvc. if any pvc set the nat enable, flush raw table.
*     if no pvc set the nat enable, set raw table to NOTRACK.
**	parameters:
*     		top:xml top node address
*     		state:nat state now.-1:boot
*            	0:do not use NOTRACK module.
*            	1:use NOTRACK module.
**	global:
*     		none
**	return:
*     		0:do not use NOTRACK module
*     		1:use NOTRACK module
**	call:
*     none
**	revision:
*     1.shnwind 20080530
**____________________________________________________________________________*/
int
check_nat_enable(mxml_node_t *top, int state){
	int i, use_notrack=1;
	//char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[4]={0}, isp[4]={0}, nat_enable[10]={0};

#if 0
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);

	/*	if nat on in each PVC, not use raw module.*/
	for(i = 0 ; i < PVC_NUM; i++){
		/*nodeName[0]="Wan", nodeName[1]="PVCn", nodeName[2]=""*/
		sprintf(nodeName[1], SUB_PVC_NAME, i);
		if((getAttrValue(top, nodeName, NODE_ACTIVE, active) != TCAPI_PROCESS_OK)
		||(getAttrValue(top, nodeName, WAN_ISP, isp) != TCAPI_PROCESS_OK)
		||(getAttrValue(top, nodeName, "NATENABLE", nat_enable) != TCAPI_PROCESS_OK)){
			continue;
		}
		/*PVC active & not bridge mode & PVC nat enable*/
		if((!strcmp(active, ACTIVE))
		&& (atoi(isp) != WAN_ENCAP_BRIDGE)
		&& (!strcmp(nat_enable, "Enable"))){
			/*when nat enable, cpe does not use notrack module.*/
			use_notrack = 0;
			break;
		}else{
			continue;
		}
	}
#endif
	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		memset(active, 0, sizeof(active));
		memset(isp, 0, sizeof(isp));
		memset(nat_enable, 0, sizeof(nat_enable));
		/*Get interface name and check wan interface exited*/
		if(get_waninfo_by_index(i, "Active", active) != SUCCESS ||
			get_waninfo_by_index(i, "ISP", isp) != SUCCESS ||
			get_waninfo_by_index(i, "NATENABLE", nat_enable) != SUCCESS){
			continue;
		}
		/*PVC active & not bridge mode & PVC nat enable*/
		if((!strcmp(active, ACTIVE))
		&& (atoi(isp) != WAN_ENCAP_BRIDGE)
		&& (!strcmp(nat_enable, "Enable"))){
			/*when nat enable, cpe does not use notrack module.*/
			use_notrack = 0;		
			break;
		}else{
			continue;
		}
	}
	/*check the sytem state */
	if(use_notrack != state){
		if(use_notrack == 1){
			system("iptables -t raw -A PREROUTING -j NOTRACK");
			system("iptables -t raw -A OUTPUT -j NOTRACK");
		}else{
			system("iptables -t raw -F");
		}
	}

	/*if(nat_on == state){
		if(nat_on){
			system("iptables -t raw -F");
			ret = 0;
		}else{
			system("iptables -t raw -A PREROUTING -j NOTRACK");
			system("iptables -t raw -A OUTPUT -j NOTRACK");
			ret = 1;
		}
	}*/
	return use_notrack;
}/*End check_nat_enable*/

/**********20080825 racing add************/
/*______________________________________________________________________________
**	function name:get_active_interface
**
**	description: to get active interface name
*
*
**	parameters:
*     		top:xml top node address
*     		interfaces:save interface data
*            intf_tpye:Lan or Wan or Both
*
**	global:
*     		none
**	return:
**		the number of active interface
**	call:
*     none
**	revision:
*     1.racing 20080829
**	2.jlliu 20100803
**____________________________________________________________________________*/
int
acl_get_active_interface(mxml_node_t *top, char interfaces[][8], const char* intf_tpye){
	int i=0;
	int counter=0;
	char nodeName[3][MAX_NODE_NAME];
	char active[6] = {0}, ifname[32] = {0};
	
	/*initializae nodeName and interfaces*/
	memset(nodeName,0,sizeof(nodeName));
	//memset(interfaces,0,(PVC_NUM+1)*8);
	memset(interfaces,0,(MAX_WAN_IF_INDEX+1)*8);

	/*if query type is Lan, return br0*/
	if(!strcasecmp(intf_tpye, "Lan")){
		strcpy(interfaces[0], "br0");
		return SUCCESS;
	}

	/*if query type is Wan or Both, query Wan confiture node tree*/
	if((!strcasecmp(intf_tpye, "Wan"))||(!strcasecmp(intf_tpye, "Both"))){
		#if 0
		strcpy(nodeName[0], "Wan");

		/*for each PVC, check if its interface is activated*/
		for(i=0; i<PVC_NUM; i++){
			/*set query PVC data*/
			sprintf(string, "PVC%d", i);
			strcpy(nodeName[1], string);

			/*retrieve Active value in PVC, if no data is retrieved, jump to next PVC*/
			if(getAttrValue(top, nodeName, "Active", value) != TCAPI_PROCESS_OK){
				fprintf(stderr, "Error occurs while getting WAN PVC%d Activate value.\n", i);
				continue;
			}

			/*retrieve Active value in PVC, if this interface is not activated, jump to next PVC*/
			if(strcasecmp(value, "yes")){
				continue;
			}

			/*check ISP value of PVC and retrieve its interface name*/
			if(getAttrValue(top, nodeName, "ISP", value) == TCAPI_PROCESS_OK){
				isp_num=atoi(value);
				if((isp_num==0)||(isp_num==1)){
					sprintf(interf, "nas%d", i);
					/*fprintf(stderr,"interf=%s\n", interf);*/
					strcpy(interfaces[counter++], interf);
				}else if(isp_num==2){
					sprintf(interf, "ppp%d", i);
					/*fprintf(stderr,"interf=%s\n", interf);*/
					strcpy(interfaces[counter++], interf);
				}
			}else{/*Error occurs while getting ISP value, jump to next PVC*/
				fprintf(stderr, "Error occurs while getting WAN PVC%d ISP value.\n", i);
				return FAIL;
			}
		}
		#endif
		for(i = 0; i < MAX_WAN_IF_INDEX; i++){
			memset(active, 0, sizeof(active));
			memset(ifname, 0, sizeof(ifname));
			if(get_waninfo_by_index(i, "Active", active) == SUCCESS &&
				get_waninfo_by_index(i, WAN_IFNAME, ifname) == SUCCESS){
				if(!strcmp(active, "Yes")){
					strcpy(interfaces[counter++], ifname);
				}
			}
		}

		/*if interface type is Both, we should add Lan interface data to the end of the list*/
		if(!strcasecmp(intf_tpye, "Both")){
			/*fprintf(stderr,"interf=%s\n", "br0");*/
			strcpy(interfaces[counter++], "br0");
		}
	}
	//return SUCCESS;
	return counter;
}/*end acl_get_active_interface*/

/*______________________________________________________________________________
**	function name:string_tolower
**
**	description: return lower case string
*
*
**	parameters:
*     		string:input string
*
*
*
**	global:
*     		none
**	return:
*     		lower case string
*
**	call:
*     none
**	revision:
*     1.racing 20080829
**____________________________________________________________________________*/
char *
string_tolower(char *string){
	int i =0;

	while(string[i]!='\0'){
		if (string[i]>=65 && string[i]<=90){
			string[i]=string[i]+32;
		}
		i++;
	}
	return string;
}/*end string_tolower*/

/*______________________________________________________________________________
**	function name:check_ip_format
**
**	description: check if the input string is a valid ip
*
*
**	parameters:
*     		ip_arg:ip string
*
*
*
**	global:
*
**	return:
*     		0:format malform
*     		1:currect format
**	call:
*     none
**	revision:
*     1.racing 20080829
**____________________________________________________________________________*/
int
check_ip_format(char *ip_arg){
	int z;
	char ip[16]={0};
	char * pattern;
	regex_t reg;
	regmatch_t pm[10];
	const size_t nmatch = 10;

	strcpy(ip, ip_arg);
	/*fprintf(stderr,"ip=%s\n", ip);*/

	/*set regular expression of ip format*/
	pattern ="([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})";
	/*compile this regular expression*/
	z = regcomp(&reg, pattern, REG_EXTENDED);

	/*if compile fails, free its memory and return false*/
	if (z != 0){
		fprintf(stderr, "Failed to open regcomp.\n");
		regfree(&reg);
		return 0;
	}

	/*compare input string base on previous syntax*/
	z = regexec(&reg, ip, nmatch, pm, 0);
	/*if format is invalid, free its memory and return false*/
	if (z == REG_NOMATCH) {
		fprintf(stderr, "IP format is wrong.\n");
		regfree(&reg);
		return 0;
	}

	/*free memory anyway*/
	regfree(&reg);

	return 1;
}/*end check_ip_format*/

/*______________________________________________________________________________
**	function name:check_ip_format
**
**	description: check if the input string is a valid ip
*
*
**	parameters:
*     		ip_arg:ip string
*
*
*
**	global:
*
**	return:
*     		0:format malform
*     		1:currect format
**	call:
*     none
**____________________________________________________________________________*/
int
acl_check_ip_format(char *ip_arg){
	int z;
	int i=0;
	char ip[16]={0};
	char * pattern;
	char *delim = ".";
	char *p[4];
	regex_t reg;
	regmatch_t pm[10];
	const size_t nmatch = 10;

	strcpy(ip, ip_arg);
	fprintf(stderr,"ip=%s\n", ip);

	/*check ip format--begin*/
	pattern ="([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})";
	z = regcomp(&reg, pattern, REG_EXTENDED);

	if (z != 0){
		fprintf(stderr, "Failed to open regcomp.\n");
		regfree(&reg);
		return 0;
	}

	z = regexec(&reg, ip, nmatch, pm, 0);
	if (z == REG_NOMATCH) {
		fprintf(stderr, "IP format is wrong.\n");
		regfree(&reg);
		return 0;
	}

	regfree(&reg);
	/*check ip format--end*/

	if(!strcmp(ip, "0.0.0.0")){
		return 1;
	}

	/*decompose ip string into four tokens and check its value*/
	for(i=0; i<4; i++){
		if(i==0){
			p[i] = strtok(ip, delim);
			/*if first token equals to 0, 127 or 223, return false*/
			if((atoi(p[i])==0) || (atoi(p[i])==127) || (atoi(p[i])>223)){
				fprintf(stderr, "IP format is wrong.\n");
				return 0;
			}
		}else{
			p[i] = strtok(NULL, delim);
			/*if any other token equals to 0 or 255, return false*/
			if(i==3){
				if((atoi(p[i])==0) || (atoi(p[i])==255)){
					fprintf(stderr, "IP format is wrong.\n");
					return 0;
				}
			}
		}

		/*if any token greater than 255 or less than 0, return false*/
		if((atoi(p[i])>255) || (atoi(p[i])<0)){
			fprintf(stderr, "IP format is wrong.\n");
			return 0;
		}
	}

	return 1;
}/*end acl_check_ip_format*/

/*______________________________________________________________________________
**	function name:check_ip_range
**
**	description: check if the ip range is valid or not
*
*
**	parameters:
*     		ip_begin_arg:ip begin string
*     		ip_end_arg:ip end string
*
*
**	global:
*     		none
**	return:
*     		0:format malform
*     		1:currect format
**	call:
*     none
**	revision:
*     1.racing 20080829
**____________________________________________________________________________*/
int
acl_check_ip_range(char *ip_begin_arg, char *ip_end_arg){
	int i = 0;
	char *delim = ".";
	char ip_begin[16]={0}, ip_end[16]={0};
	char *ip_begin_tok[4], *ip_end_tok[4];

	strcpy(ip_begin, ip_begin_arg);
	strcpy(ip_end, ip_end_arg);

	/*fprintf(stderr,"ip_begin=%s\n", ip_begin);*/
	/*fprintf(stderr,"ip_end=%s\n", ip_end);*/

	/*if input ip is 0.0.0.0, this means any ip, return true*/
	if((!strcmp(ip_begin, "0.0.0.0")) && (!strcmp(ip_end, "0.0.0.0"))){
		return 1;
	}

	/*decompose ip_begin string into four token*/
	for(i=0; i<4; i++){
		if(i==0){
			ip_begin_tok[i] = strtok(ip_begin, delim);
		}else{
			ip_begin_tok[i] = strtok(NULL, delim);
		}
	}

	/*decompose ip_end string into four token*/
	for(i=0; i<4; i++){
		if(i==0){
			ip_end_tok[i] = strtok(ip_end, delim);
		}else{
			ip_end_tok[i] = strtok(NULL, delim);
		}
	}

	/*check if ip_begin and ip_end are in the same subnet*/
	for(i=0; i<3; i++){
		if((atoi(ip_begin_tok[i]))!=(atoi(ip_end_tok[i]))){
			fprintf(stderr, "Not in the same subnet.\n");
			return 0;
		}
	}

	/*check if ip_end exceed ip_begin*/
	if(atoi(ip_begin_tok[3])>atoi(ip_end_tok[3])){
		fprintf(stderr, "IP range is invalid.\n");
		return 0;
	}

	return 1;
}/*end acl_check_ip_range*/
/**********20080825 racing add************/

/**********20081006 racing add************/
/*______________________________________________________________________________
**	function name:check_mask_format
**
**	description: check if the input string is a valid mask
*
*
**	parameters:
*     		mask_arg:ip string
*
*
*
**	global:
*
**	return:
*     		0:format malform
*     		1:currect format
**	call:
*     none
**____________________________________________________________________________*/
int
check_mask_format(char *mask_arg, char *mask_decimal){
	int i;
	int flag = 0;
	int mask_counter = 0;
	char mask[16]={0};
	char *p[4];
	const char *delim = ".";
	unsigned int sample = 0x0;
	unsigned int binary_mask = 0x0;
	const unsigned int bit_1_mask = 0x1;

	strcpy(mask, mask_arg);
	/*check is this mask a valide ip format*/
	if(!check_ip_format(mask)){
		return 0;
	}

	/*divied mask into four token*/
	for(i=0; i<4; i++){
		if(i==0){
			p[i] = strtok(mask, delim);
		}else{
			p[i] = strtok(NULL, delim);
		}
	}

	/*transfer mask to bit type ex. 255 -> 11111111*/
	for(i=0; i<4; i++){
		binary_mask |= atoi(p[i]);

		if(i!=3){
			binary_mask <<= 8;
		}
	}

	/*check is this mask a valid mask ex. 11111011000 is invalid*/
	for(i=0; i<32; i++){
		sample = binary_mask & bit_1_mask;
		if(sample == 0x0){
			if(mask_counter != 0){
				return 0;
			}
		}else{
			flag = 1;
			mask_counter++;
		}
		binary_mask >>= 1;
	}

	sprintf(mask_decimal, "%d", mask_counter);
	return 1;
}/*end check_mask_format*/

/*______________________________________________________________________________
**	function name:check_mac_format
**
**	description: check if the input string is a valid mac address
*
*
**	parameters:
*     		mac_arg:mac address
*
*
*
**	global:
*
**	return:
*     		0:format malform
*     		1:currect format
**	call:
*     none
**____________________________________________________________________________*/
int
check_mac_format(const char *mac_arg){
	int z;
	char mac[24]={0};
	char * pattern;
	regex_t reg;
	regmatch_t pm[10];
	const size_t nmatch = 10;

	strcpy(mac, mac_arg);
	fprintf(stderr,"mac=%s\n", mac);

	pattern ="([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}";
	z = regcomp(&reg, pattern, REG_EXTENDED);

	if (z != 0){
		fprintf(stderr, "Failed to open regcomp.\n");
		regfree(&reg);
		return 0;
	}

	z = regexec(&reg, mac, nmatch, pm, 0);
	if (z == REG_NOMATCH) {
		fprintf(stderr, "MAC format is wrong.\n");
		regfree(&reg);
		return 0;
	}

	regfree(&reg);

	return 1;
}/*end check_mac_format*/

/*______________________________________________________________________________
**	function name:get_interface_by_PVC
**
**	description:get interface name by pvc
*
*
**	parameters:
*     		top:xml top node address
*     		intf_tpye:interface type
*     		interface:return value
*
*
**	global:
*
**	return:
*     		0:success
*     		-1:otherwise
**	call:
*     none
**____________________________________________________________________________*/
int
get_interface_by_PVC(mxml_node_t *top, const char* intf_type, char *interface){
	int i=0;
	int isp_num=0;
	char nodeName[3][MAX_NODE_NAME];
	char value[4]={0}, interf[8]={0};

	/*initializae nodeName and interfaces*/
	memset(nodeName,0,sizeof(nodeName));

	/*if query type is Lan, return br0*/
	if(!strcasecmp(intf_type, "Lan")){
		strcpy(interface, "br0");
		return SUCCESS;
	}else{
		strcpy(nodeName[0], "Wan");
		/*set query PVC data*/
		strcpy(nodeName[1], intf_type);
		/*retrieve Active value in PVC, if no data is retrieved, return fail*/
		if(getAttrValue(top, nodeName, "Active", value) != TCAPI_PROCESS_OK){
			fprintf(stderr, "Error occurs while getting WAN PVC%d Activate value.\n", i);
			return FAIL;
		}

		/*retrieve Active value in PVC, if this interface is not activated, jump to next PVC*/
		if(strcasecmp(value, "yes")){
			return FAIL;
		}

		/*check ISP value of PVC and retrieve its interface name*/
		if(getAttrValue(top, nodeName, "ISP", value) == TCAPI_PROCESS_OK){
			isp_num=atoi(value);
			if((isp_num==0) || (isp_num==1)){

				sprintf(interf, "nas%c", intf_type[3]);
			}else if(isp_num==2){
				sprintf(interf, "ppp%c", intf_type[3]);
			}else{
				return FAIL;
			}
			strcpy(interface, interf);
		}else{/*Error occurs while getting ISP value, jump to next PVC*/
			fprintf(stderr, "Error occurs while getting WAN PVC%d ISP value.\n", i);
			return FAIL;
		}
	}

	return SUCCESS;
}/*get_interface_by_PVC*/

/*______________________________________________________________________________
**	function name:ipfilter_check_active
**
**	description:to check if any active rule exists
*
*
**	parameters:
*     		top:xml top node address
*
*
**	global:
*
**	return:
*     		0:no active rule
*     		1:exist active rule
**	call:
*     none
**____________________________________________________________________________*/
int
ipfilter_check_active(mxml_node_t *top){
	int i=0;
	char ipmacfilter_rule_status[8]={0}, element_name[16]={0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], IPMACFILTER);

	/*for each rule in ipfilter function, if any active rule exists, return true*/
	for(i=0; i<MAX_IPMACFILTER_RULE; i++){
		sprintf(element_name,"%s%d", SUB_NODE_NAME, i);
		strcpy(nodeName[1], element_name);

		if(getAttrValue(top, nodeName, "Active", ipmacfilter_rule_status) != TCAPI_PROCESS_OK){
			continue;
		}

		if(!strcasecmp(ipmacfilter_rule_status, "yes")){
			return 1;
}
	}

	return 0;
}
/************************
	Description: check mac filter switch of total
	return 
	0:
		switch is off
	1:
		switch is on;
*************************/
#if defined(TCSUPPORT_CT_E8GUI)
int
ipfilter_check_switch(mxml_node_t *top){
	char ipmacfilter_switch[8]={0}, element_name[16]={0};
	char port_up_filter[8] = {0}, port_down_filter[8] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], IPMACFILTER);
	strcpy(nodeName[1], IPMACFILTER_COMMON);
#if 0
	if(getAttrValue(top, nodeName, "ActiveMac", ipmacfilter_switch) != TCAPI_PROCESS_OK &&
	   getAttrValue(top, nodeName, "ActivePortOut", port_up_filter) != TCAPI_PROCESS_OK &&
	   getAttrValue(top, nodeName, "ActivePortIn", port_down_filter) != TCAPI_PROCESS_OK){
		return 0;
	}
#endif
	getAttrValue(top, nodeName, "ActiveMac", ipmacfilter_switch);
	getAttrValue(top, nodeName, "ActivePortOut", port_up_filter);
	getAttrValue(top, nodeName, "ActivePortIn", port_down_filter);

	//tcdbg_printf("ip mac fitler is %s\n", ipmacfilter_switch);
	//tcdbg_printf("port up filter is %s, port down filter is %s\n", port_up_filter, port_down_filter);
	
	if(!strcmp(ipmacfilter_switch, "1") || 
	   !strcmp(port_up_filter, "1") ||
	   !strcmp(port_down_filter, "1")){
		return 1;
	}
	
	return 0;
}
int
parsedStr(char *str_input, char *str_ret)
{
	char *pf = NULL;
	char *delline1 = '\r';
	char *delline2 = '\n';
	char *ps = str_input;
	int  iLen = 0;
	pf = strrchr(ps, delline1);
	if(NULL != pf){
		iLen++;
	}
	pf = strrchr(ps, delline2);
	if(NULL != pf){
		iLen++;
	}
	strncpy(str_ret, str_input, strlen(str_input)-iLen);
	return SUCCESS;	
}
#endif
/**********20081006 racing add************/

/*20081022 krammer add*/
/*______________________________________________________________________________
**	function name: check_ l7Filter_state
**	description:
**         	Check that whether the l7Filter should be loaded or not. If yes, load the l7Filter module.
** 	   	If not, unload it.
**	parameters:
*     		new_filter_state: newly filter state in the future
**	global:
*     		l7filter_state:means the l7 filter state now.
**	return:
*
**	call:
*     		none
**	revision:
*     		1.krammer
**____________________________________________________________________________
*/
#if 0
int
check_and_set_l7filter(mxml_node_t *top, int l7filter_state)
{
	int filter_on=0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char appfilter_status[5]={0}, urlfilter_status[5]={0};

	if(filter_on==0){
		/*check app filter state*/

		/*flush the buffer*/
		memset(nodeName,0,sizeof(nodeName));
		/*build the node name = AppFilter_Entry*/
		strcpy(nodeName[0], APPFILTER);
		strcpy(nodeName[1], SUB_NODE_NAME);

		/*check the Activate value*/
		if(getAttrValue(top, nodeName, "Activate", appfilter_status) != TCAPI_PROCESS_OK){
			fprintf(stderr,"get app filter activate error in check filter state\n");
		}
		/*see if appfilter's attribute(Activate) = 1*/
		else if(!strcmp(appfilter_status,"1")){
			filter_on=1;/*load the iptable_filter module*/
		}
	}
	if(filter_on==0){
		/*check url filter state*/

		/*flush the buffer*/
		memset(nodeName,0,sizeof(nodeName));

		/*let the node name = UrlFilter_Common*/
		strcpy(nodeName[0], URLFILTER);
		strcpy(nodeName[1], STRING_COMMON);
		/*check the UrlFilter_Common Activate value*/
		if(getAttrValue(top, nodeName, "Activate", urlfilter_status) != TCAPI_PROCESS_OK){
			//fprintf(stderr,"get url filter activate error in check filter state\n");
			fprintf(stderr,"nodename 0 %s nodename 1 %s Activate urlfilter_status %s\n",nodeName[0],nodeName[1],urlfilter_status);
		}
		/*if UrlFilter_Common Activate value=1 then do follows*/
		else if(!strcmp(urlfilter_status,"1")){
			/*flush the node name [1] field*/
			memset(nodeName[1],0,sizeof(nodeName[1]));
			int urlIndex=0;
			/*check url rule activate one by one, if any one is activated, then load the filter module*/
			for(urlIndex=0;urlIndex<MAX_URL_RULE; urlIndex++){
				/*reset the state buffer*/
				strcpy(urlfilter_status,"");
				/*set node name[1]=Entry+number(0~15)*/
				sprintf(nodeName[1], "%s%d", SUB_NODE_NAME, urlIndex);
				if(getAttrValue(top, nodeName, "Activate", urlfilter_status) != TCAPI_PROCESS_OK){
					//fprintf(stderr,"get url filter activate error in check filter state\n");
					continue;
				}
				/*find a rule is activated, load the l7 filter module and break for loop*/
				else if(!strcmp(urlfilter_status,"1")){
					filter_on=1;
					break;
				}
				else{
					/*nothing to do*/
				}
			}

		}
	}
	//fprintf(stderr, "l7filter_state: %d , filter_on: %d, appfilter_status: %s\n", l7filter_state, filter_on, appfilter_status);
	if(l7filter_state != filter_on){
		if(filter_on){
			fprintf(stderr,"========================insmod l7filter=======================\n");
			system("insmod /lib/modules/2.6.22.15/kernel/net/netfilter/xt_layer7.ko");
		}else{
			fprintf(stderr,"========================rmmod l7filter=======================\n");
			system("iptables -t filter -F app_filter_chain");
			system("iptables -t filter -F url_filter_chain");
			system("rmmod xt_layer7");
		}
	}
	return filter_on;
#endif
void
check_and_set_l7filter(unsigned int new_filter_state)
{

	if((pre_sys_state.filter_m_state & L7_BIT_MASK) != 0){//pre is on
		if((new_filter_state & L7_BIT_MASK) == 0){//now is down
			fprintf(stderr,"========================rmmod l7filter=======================\n");
//			tcdbg_printf("========================rmmod l7filter=======================\n");
			system("iptables -t filter -F app_filter_chain");
			system("iptables -t filter -F url_filter_chain");
			system("rmmod xt_layer7");
		}
	}
	else{//pre is down
		if((new_filter_state & L7_BIT_MASK) != 0){//now is down
			fprintf(stderr,"========================insmod l7filter=======================\n");
//			tcdbg_printf("========================insmod l7filter=======================\n");
#if KERNEL_2_6_36
	system("insmod /lib/modules/2.6.36/kernel/net/netfilter/xt_layer7.ko");
#else
			system("insmod /lib/modules/2.6.22.15/kernel/net/netfilter/xt_layer7.ko");
#endif
		}
	}
}
#if 0
/*20081022 krammer add*/
/*20081228 pork add*/
/*______________________________________________________________________________
**	function name: trim
**	description:
**         	Remove leading, trailing spaces in passed string.
**	parameters:
*     		oriStr : original passed string
*     		retStr : the non-space string going to be returned
**	return:
**	call:
*     		none
**	revision:
*     		1.pork
**____________________________________________________________________________
*/
void trim(char *oriStr,char *retStr){
    int oriStrHead = 0;
    int oriStrTail = 0;
    int i, j = 0;
    strncpy(retStr,oriStr,strlen(oriStr));
    while((oriStr[oriStrHead]==' ')||(oriStr[oriStrHead]=='\t')||(oriStr[oriStrHead]=='\n')){
        oriStrHead++;
    }
    oriStrTail = strlen(oriStr)-1;
    while((oriStr[oriStrTail]==' ')||(oriStr[oriStrTail]=='\t')||(oriStr[oriStrTail]=='\n')){
        oriStrTail--;
    }
    if(oriStrHead==0 && oriStrTail==strlen(oriStr)-1) { }
    else if(oriStrHead==0){
    	retStr[oriStrTail+1] = '\0';
    }
    else {
        for(i=oriStrHead; i<=oriStrTail; i++){
        	retStr[j++] = oriStr[i];
        }
        retStr[j] = '\0';
    }
}
/*20081228 pork add*/
#endif
/*20090121 pork add*/
/*______________________________________________________________________________
**	function name: quotHandle
**	description:
**         	convert the quotation of the string passed to ralink driver
**	parameters:
*     		oriStr : original passed string
**	return:
**	call:
*     		none
**	revision:
*     		1.pork
**____________________________________________________________________________
*/
void quotHandle(char *oriStr){
    int i;
    int j = 0;
    char handle[128];
    for(i = 0;i < strlen(oriStr);i++){
    	if(oriStr[i] == '"'){
    		handle[j++]='\\';
    		handle[j++]='"';
    	}
    	else{
    		handle[j] = oriStr[i];
    		j++;
    	}
    }
    handle[j] = '\0';
    strcpy(oriStr,handle);
}
/*20090121 pork add*/


/*______________________________________________________________________________
**	function name: quotEscHandle
**	description:
**         	convert special character by escape and passed to ralink driver
**	parameters:
*     		oriStr : original passed string
**	return:
**	call:
*     		none
**	revision:
*     		1.daixumei
**____________________________________________________________________________
*/
void specialCharacterHandle(char *oriStr){
    int i;
    int j = 0;
    char handle[160];
    for(i = 0;i < strlen(oriStr);i++){
    	if(oriStr[i] == '"'){
    		handle[j++]='\\';
    		handle[j++]='"';
    	}else if(oriStr[i] == '\\'){
    		handle[j++]='\\';
    		handle[j++]='\\';
    	}else if(oriStr[i] == '\$'){
    		handle[j++]='\\';
    		handle[j++]='\$';
    	}else if(oriStr[i] == '\`'){
    		handle[j++]='\\';
    		handle[j++]='\`';
    	}
    	else{
    		handle[j] = oriStr[i];
    		j++;
    	}
    }
    handle[j] = '\0';
    strcpy(oriStr,handle);
}

#endif
/*______________________________________________________________________________
**	function name: setNewFilterState
**	description:
**         	use call back function to decide new filter state
**	parameters:
*     		top : top of node tree
*     		conditionFunction: the call back function which check the bit of the node should be set or not.
*     		vector: the vector of filter bit map
*     		newFilterState: a temp variable to record new filter state
**	return:
**	call:
*     		none
**	revision:
*     		1.krammer
**____________________________________________________________________________
*/
//void
//setNewFilterState(mxml_node_t *top,int (*conditionFunction)(mxml_node_t *top),char vector,char *newFilterState){
//	if(conditionFunction(top)){//filter on
//		*newFilterState=pre_sys_state.filter_m_state|vector;
//	}
//	else{//filter down
//		*newFilterState=pre_sys_state.filter_m_state&(~vector);
//	}
//}
/*______________________________________________________________________________
**	function name: get_entry_number
**	description:
**         	find the entry number in node name
**	parameters:
*     		buffer : node name
*     		keyword : keyword before the number
*     		number : the buffer to get the number after node name
**	return:
**			0:SUCCESS
**			-1:FAIL
**	call:
*     		none
**	revision:
*     		1.krammer
**____________________________________________________________________________
*/
int
get_entry_number(char *buffer, char *keyword, int *number){
	char *tmp=NULL;
	int test=0;
	if((buffer == 0)||(keyword == 0)||(number == 0)){
		return FAIL;
	}
	if(strstr(buffer,keyword)){
		tmp=buffer+strlen(keyword);
		(*number)=atoi(tmp);
		test=atoi(tmp);
		return SUCCESS;
	}
	else{
		return FAIL;
	}
}

/*crc32 function. shnwind add 20080410*/
/**********************************************************************/
/* The following was grabbed and tweaked from the old snippets collection
 * of public domain C code. */

/**********************************************************************\
|* Demonstration program to compute the 32-bit CRC used as the frame  *|
|* check sequence in ADCCP (ANSI X3.66, also known as FIPS PUB 71     *|
|* and FED-STD-1003, the U.S. versions of CCITT's X.25 link-level     *|
|* protocol).  The 32-bit FCS was added via the Federal Register,     *|
|* 1 June 1982, p.23798.  I presume but don't know for certain that   *|
|* this polynomial is or will be included in CCITT V.41, which        *|
|* defines the 16-bit CRC (often called CRC-CCITT) polynomial.  FIPS  *|
|* PUB 78 says that the 32-bit FCS reduces otherwise undetected       *|
|* errors by a factor of 10^-5 over 16-bit FCS.                       *|
\**********************************************************************/

/* Copyright (C) 1986 Gary S. Brown.  You may use this program, or
   code or tables extracted from it, as desired without restriction.*/

/* First, the polynomial itself and its table of feedback terms.  The  */
/* polynomial is                                                       */
/* X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X^1+X^0 */
/* Note that we take it "backwards" and put the highest-order term in  */
/* the lowest-order bit.  The X^32 term is "implied"; the LSB is the   */
/* X^31 term, etc.  The X^0 term (usually shown as "+1") results in    */
/* the MSB being 1.                                                    */

/* Note that the usual hardware shift register implementation, which   */
/* is what we're using (we're merely optimizing it by doing eight-bit  */
/* chunks at a time) shifts bits into the lowest-order term.  In our   */
/* implementation, that means shifting towards the right.  Why do we   */
/* do it this way?  Because the calculated CRC must be transmitted in  */
/* order from highest-order term to lowest-order term.  UARTs transmit */
/* characters in order from LSB to MSB.  By storing the CRC this way,  */
/* we hand it to the UART in the order low-byte to high-byte; the UART */
/* sends each low-bit to hight-bit; and the result is transmission bit */
/* by bit from highest- to lowest-order term without requiring any bit */
/* shuffling on our part.  Reception works similarly.                  */

/* The feedback terms table consists of 256, 32-bit entries.  Notes:   */
/*                                                                     */
/*  1. The table can be generated at runtime if desired; code to do so */
/*     is shown later.  It might not be obvious, but the feedback      */
/*     terms simply represent the results of eight shift/xor opera-    */
/*     tions for all combinations of data and CRC register values.     */
/*                                                                     */
/*  2. The CRC accumulation logic is the same for all CRC polynomials, */
/*     be they sixteen or thirty-two bits wide.  You simply choose the */
/*     appropriate table.  Alternatively, because the table can be     */
/*     generated at runtime, you can start by generating the table for */
/*     the polynomial in question and use exactly the same "updcrc",   */
/*     if your application needn't simultaneously handle two CRC       */
/*     polynomials.  (Note, however, that XMODEM is strange.)          */
/*                                                                     */
/*  3. For 16-bit CRCs, the table entries need be only 16 bits wide;   */
/*     of course, 32-bit entries work OK if the high 16 bits are zero. */
/*                                                                     */
/*  4. The values must be right-shifted by eight bits by the "updcrc"  */
/*     logic; the shift must be unsigned (bring in zeroes).  On some   */
/*     hardware you could probably optimize the shift in assembler by  */
/*     using byte-swap instructions.                                   */

static const unsigned long crc_32_tab[] = { /* CRC polynomial 0xedb88320 */
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

#define UPDC32(octet,crc) (crc_32_tab[((crc) ^ (octet)) & 0xff] ^ ((crc) >> 8))

unsigned long calculate_crc32(int imagefd, long len)
{
	long length=len;	
	int check_len=0, i=0;
	char flag=0;/*for stop loop*/
	unsigned char *buf;
	unsigned long crc;
	
      	crc = 0xFFFFFFFF;
	
	/*Because read 1 byte at a time will spent much time, 
		we read a bigger size at a time and use this space to 
		do checksum. */
   	buf = (unsigned char *)malloc(CHECKSUM_TEMP_BUF_SIZE);

 	while(flag == 0){
		/*decide add length*/
		if(length <= CHECKSUM_TEMP_BUF_SIZE){
			check_len=length;
			flag = 1;	
		}else{
			check_len = CHECKSUM_TEMP_BUF_SIZE;
		}

		length -= check_len;
		read(imagefd, buf, check_len); 
		
    		for(i=0;i < check_len;i++)
    		{
    			crc = UPDC32(*(buf+i), crc);	
		}  
	}  	

    free(buf);
    //tcdbg_printf(" crc %x\n",crc);
    return ((unsigned long)crc);
}

/*______________________________________________________________________________
**	function name: is_adsl_link_up
**	description:
**         	check adsl link status
**	parameters:
**	return:
**			0:adsl link is down
**			1:adsl link is up
**	call:
*     		none
**	revision:
*     		1.jlliu_nj
**____________________________________________________________________________
*/
int is_adsl_link_up()
{
	int fd;
	char string[64] = {0};

	fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
	if( fd <= 0 ){
		return 0;
	}

	if(read(fd, string, 64) <= 0 ){
		close(fd);
		return 0;
	}
	else{
		if(strstr(string, "up")){
			close(fd);
			return 1;
		}
	}
	close(fd);
	return 0;
}

#if defined(TCSUPPORT_CT_PON)
/*______________________________________________________________________________
**	function name: is_pon_link_up
**	description:
**         	check pon link status
**	parameters:
**	return:
**			0:pon link is down
**			1:pon link is up
**	call:
*     		none
**	revision:
*     		
**____________________________________________________________________________
*/
int is_pon_link_up()
{
	int ponUp = 0;
	char string[64] = {0};

	memset(string, 0, sizeof(string));
	tcapi_get("XPON_Common","trafficStatus",string);
	
	if(strcmp(string, "up") == 0) 
	{
		ponUp = 1;
	}
	else
	{
		ponUp = 0;
	}
	return ponUp;
}
#endif

extern mxml_node_t **gTreePtr;

/*_____________________________________________________________________________
**      function name: get_wanindex_by_name
**      descriptions:
**           get wan index based on name
**
**      parameters:
**            wan_if_name:wan interface name,eg:nas2_1
**
**      global:
**             None
**
**      return:
**            wan interface index(0~63)
**
**      call:
**
**      revision:
**      1. jlliu 20100720
**____________________________________________________________________________
*/
int get_wanindex_by_name(char *wan_if_name)
{
#ifndef USE_DEFAULT_IFNAME_FORMAT 
	int pvc_index = -1, entry_index = -1;
#endif
	int if_index = -1;
	char pvc[5] = {0}, entry[5] = {0};
	mxml_node_t *top;

	if((top = *gTreePtr) == NULL || wan_if_name == NULL)
		goto error;
	
#ifdef USE_DEFAULT_IFNAME_FORMAT
	if(strstr(wan_if_name, "ppp")){  /*ppp interface*/
		if(get_entry_number(wan_if_name,"ppp",&if_index) != SUCCESS ){
			goto error;
		}		
	}
	else{   /*nas interface,ext:nas0_1*/
		sscanf(wan_if_name, "nas%1s_%1s", pvc, entry);	
		if(strlen(pvc) == 0 || strlen(entry) == 0){
			goto error;
		}
		if_index = atoi(pvc) * MAX_SMUX_NUM + atoi(entry);
	}
	if(if_index < 0 || if_index > MAX_WAN_IF_INDEX)
		goto error;
	
	return if_index;
#else
	/*Just search index based on interface name*/
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char if_name[16] = {0};
	
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],WAN);
		  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

			if(getAttrValue(top, nodeName, "IFName", if_name) == TCAPI_PROCESS_OK ){
				if(!strcmp(if_name, wan_if_name)){
					if_index = pvc_index * MAX_SMUX_NUM + entry_index;
					return if_index;
				}
			}
		}
	}
#endif
error:
	tcdbg_printf("get_wanindex_by_name:can not get index from %s \n", wan_if_name);
	return FAIL;
	
}

/*_____________________________________________________________________________
**      function name: get_waninfo_by_index
**      descriptions:
**           get wan information based on index
**
**      parameters:
**            if_index:index of interface (0~63)
**		attribute:the attribute which be geted
**		reval: return value buffer
**
**      global:
**             None
**
**      return:
**            SUCCESS:successfully
**		FAIL:failed
**
**      call:
**
**      revision:
**      1. jlliu 20100720
**____________________________________________________________________________
*/
int get_waninfo_by_index(int if_index, char *attribute, char *reval)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int pvc_index = 0, entry_index = 0;
	mxml_node_t *top;
	
	if(if_index < 0 || if_index > PVC_NUM * MAX_SMUX_NUM)
		return FAIL;
	
	top =  *gTreePtr;
	if(!top || !attribute || !reval)
		return FAIL;
	
	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index % MAX_SMUX_NUM;

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],WAN);
  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

	/*find value from Wan_PVCn firstly*/
	if(getAttrValue(top, nodeName, attribute, reval) == TCAPI_NONE_ATTR){
		/*find value from Wan_PVCn_Entryn if no attribute at Wan_PVCn*/
		sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
		if(getAttrValue(top, nodeName, attribute, reval) != TCAPI_PROCESS_OK ){
			return FAIL;
		}
	}
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: get_waninfo_by_name
**      descriptions:
**           get wan information based on interface name
**
**      parameters:
**            wan_if_name: wan interface name, eg:nas2_1,ppp12
**		attribute:the attribute which be geted
**		reval: return value buffer
**
**      global:
**             None
**
**      return:
**            SUCCESS:successfully
**		FAIL:failed
**
**      call:
**
**      revision:
**      1. jlliu 20100902
**____________________________________________________________________________
*/
int get_waninfo_by_name(char *wan_if_name, char *attribute, char *reval){
	int wan_if_index = 0;
	if((wan_if_index = get_wanindex_by_name(wan_if_name)) < 0)
		return FAIL;

	if(get_waninfo_by_index(wan_if_index, attribute, reval) != SUCCESS){
		return FAIL;
	}
	return SUCCESS;
}

/*_____________________________________________________________________________
**      function name: get_all_wan_index
**      descriptions:
**           get all wan interface index(index = 0~64)
**
**      parameters:
**            indexbuf:index value array,which should be indexbuf[64]
**
**      global:
**             None
**
**      return:
**            total number of wan interface index
**
**      call:
**
**      revision:
**      1. jlliu 20100720
**____________________________________________________________________________
*/
int get_all_wan_index(int indexbuf[], int buf_size)
{
	int pvc_index, entry_index, if_index, if_num = 0;
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *curNode = NULL;
	mxml_node_t *top = get_node_top();
	if(!indexbuf)
		return 0;
	
	for(pvc_index = 0; pvc_index < PVC_NUM; pvc_index++){
		for(entry_index = 0; entry_index < MAX_SMUX_NUM; entry_index++){
			memset(nodeName, 0, sizeof(nodeName));
		  	strcpy(nodeName[0],WAN);
		  	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
			sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);

			curNode = mxmlFindElement(top, top, nodeName[0],
				NULL, NULL, MXML_DESCEND);
			if(curNode){
				curNode = mxmlFindElement(curNode, curNode, nodeName[1],
				NULL, NULL, MXML_DESCEND);
				if(curNode){
					curNode = mxmlFindElement(curNode, curNode, nodeName[2],
					NULL, NULL, MXML_DESCEND);
					if(curNode){
						//tcdbg_printf("get_all_wan_index(): find Wan_%s_%s,continue!\n", nodeName[1], nodeName[2]);
						/*calculate interface index based on pvc index and entry index*/
						if_index = pvc_index * MAX_SMUX_NUM + entry_index;
						*indexbuf = if_index;
						indexbuf++;
						if_num++;
						buf_size--;
						if(buf_size  <= 0)
							break;
					}
				}
			}
		}
	}
	return if_num;
}

mxml_node_t *get_node_top()
{
	return *gTreePtr;
}

mxml_node_t *search_node(char nodeName[][MAX_NODE_NAME])
{
	int i;
	mxml_node_t *curNode=get_node_top();
	mxml_node_t *parantNode=NULL;

	for(i=0; i < MAX_ARG_NUM; i++){
		if(strlen(nodeName[i])==0){
			break;
		}
		parantNode=curNode;
		curNode=mxmlFindElement(curNode, curNode, nodeName[i],
			NULL, NULL, MXML_DESCEND);
		if(curNode==NULL){
			return NULL;
		}
	}
	return curNode;
}

int copy_node(char dst_node[][MAX_NODE_NAME], char src_node[][MAX_NODE_NAME])
{
	mxml_node_t *p_dst_node = NULL;
	mxml_node_t *p_src_node = NULL;
	mxml_attr_t   *attr= NULL; 
	mxml_node_t *top = get_node_top();
	int i;
	
	if((p_dst_node = search_node(dst_node)) == NULL || (p_src_node = search_node(src_node)) == NULL){
		tcdbg_printf("copy_node failed:can not find node");
		return FAIL;
	}

	attr = p_src_node->value.element.attrs;
	for(i = 0; i < p_src_node->value.element.num_attrs; i++){
		setAttrValue(top, dst_node, attr->name, attr->value);
		attr ++;
	}
	return SUCCESS;
}

#if defined(TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT)
unsigned long compute_crc32buf(char *buf, size_t len)
{
      unsigned long crc;

      crc = 0xFFFFFFFF;

      for ( ; len; --len, ++buf)
      {
            crc = UPDC32(*buf, crc);
      }

      return (unsigned long)crc;
}
#endif

int doValPut(char *path, char *value)
{
	FILE *fp;

	fp = fopen(path,"w");
	if(fp != NULL){
		fprintf(fp, value);
		fclose(fp);
	}else{
		return -1;
	}
	return 0;
}

#ifdef TCSUPPORT_CPU_MT7510
#define GSW_LINK_FILE	"/proc/tc3162/gsw_link_st"
void get3262LanStatus(unsigned char* buf){
	unsigned char line[MAXSIZE] = {0};
	unsigned char index = 0;
	FILE* fp = NULL;
	int portStatus[4]; 

	fp = fopen(GSW_LINK_FILE, "r");
	if(fp == NULL){
		return ;
	}

	for(; index<4; index++){
		if(fgets(line, MAXSIZE, fp) == NULL)
			continue;
		if(strstr(line, "Down")){
			portStatus[index] = 0;
		}else{	
			portStatus[index] = 1;
		}

		
	}
	fclose(fp);
	sprintf(buf, "%d %d %d %d",portStatus[0],portStatus[1],portStatus[2],portStatus[3]);
	return ;
}
#endif
#if defined(TCSUPPORT_CT_PON)
int getPonLanStatus(char* buf)
{
	char line[MAXSIZE] = {0};
	char index = 0;
	FILE* fp = NULL;
	int portStatus[4] = {0}; 

	fp = fopen(ETH_PORT_STATUS_FILE_PATH, "r");
	if( NULL == fp )
		return -1;

	if( fgets(line, sizeof(line),fp ) )
	{
		if(sscanf(line,"%d %d %d %d", &portStatus[0], &portStatus[1],
				&portStatus[2],&portStatus[3] )!=4)
		{
			tcdbg_printf("failed to read %s\n",ETH_PORT_STATUS_FILE_PATH);
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	sprintf(buf, "%d %d %d %d", portStatus[0],portStatus[1],portStatus[2],portStatus[3]);
	return 0;
}

int getPonLanStatus2(int portStatus[])
{
	char line[MAXSIZE] = {0};
	char index = 0;
	FILE* fp = NULL;

	fp = fopen(ETH_PORT_STATUS_FILE_PATH, "r");
	if( NULL == fp )
		return -1;

	if( fgets(line, sizeof(line),fp ) )
	{
		if(sscanf(line,"%d %d %d %d", &portStatus[0], &portStatus[1],
				&portStatus[2],&portStatus[3] )!=4)
		{
			tcdbg_printf("failed to read %s\n",ETH_PORT_STATUS_FILE_PATH);
			fclose(fp);
			return -1;
		}
	}

	fclose(fp);
	return 0;
}

int find_pid_by_name( char *ProcName, int *foundpid, int *foundnum)
{
        DIR             *dir = NULL;
        struct dirent   *d = NULL;
        int             pid = 0, i = 0;
        char            *s = NULL;
        int pnlen = 0;

		if ( NULL == ProcName
			|| NULL == foundpid
			|| NULL == foundnum )
			return -1;
	
        foundpid[0] = 0;
        pnlen = strlen(ProcName);
		*foundnum = 0;

        /* Open the /proc directory. */
        dir = opendir("/proc");
        if (!dir)
        {
                printf("cannot open /proc");
                return -1;
        }

        /* Walk through the directory. */
        while ((d = readdir(dir)) != NULL) {
                char exe[PATH_MAX_DNSMASQ+1];
                char path[PATH_MAX_DNSMASQ+1];
                int len;
                int namelen;

                /* See if this is a process */
                if ((pid = atoi(d->d_name)) == 0)
					continue;

                snprintf(exe, sizeof(exe), "/proc/%s/exe", d->d_name);
				
                if ((len = readlink(exe, path, PATH_MAX_DNSMASQ)) < 0)
                        continue;
                path[len] = '\0';

                /* Find ProcName */
                s = strrchr(path, '/');
                if(s == NULL)
					continue;
                s++;

                /* we don't need small name len */
                namelen = strlen(s);
                if(namelen < pnlen) 
					continue;

                if(!strncmp(ProcName, s, pnlen)) {
                        /* to avoid subname like search proc tao but proc taolinke matched */
                        if(s[pnlen] == ' ' || s[pnlen] == '\0') {
                                foundpid[i] = pid;
                                i++;
                        }
                }
        }

		*foundnum = i;
        foundpid[i] = 0;
        closedir(dir);
        return  0;
}

#endif

void flush_arp()
{
	char cmd[64] = {0};
	sprintf(cmd, "/usr/bin/ip neigh flush dev br0");	
	system(cmd);
}

void get_lan_status(int lanStatus[])
{
	FILE *fp = NULL;
	char str[10] = {0};	
	char cmd[80]={0};
	char temp[80] = {0};
	
#if defined(TCSUPPORT_CT_PON)
	getPonLanStatus2(lanStatus);
#else
#ifdef TCSUPPORT_CPU_MT7510
	get3262LanStatus(temp);
	sprintf(cmd,"echo \"%s\" > /tmp/var/tmp/lan_stats.conf", temp);
#else
#if 0
		getPonLanStatus(temp);
		sprintf(cmd,"echo \"%s\" > /tmp/var/tmp/lan_stats.conf", temp);
#else
		sprintf(cmd,"/userfs/bin/ethphxcmd eth0 status > /tmp/var/tmp/lan_stats.conf");
#endif
#endif

	if (system(cmd) < 0)
	{
		tcdbg_printf("system error\n");
		return ;
	}

	fp = fopen("/tmp/var/tmp/lan_stats.conf", "r");
	if (fp == NULL)
	{
		tcdbg_printf("file open failed\n");
	    	return ;	
	}
	if (fread(str, sizeof(char), sizeof(str), fp) <= 0)
	{
		//tcdbg_printf("file read failed\n");
		fclose(fp);
		return ;
	}
	
	sscanf(str, "%d%d%d%d", &lanStatus[0], &lanStatus[1], &lanStatus[2], &lanStatus[3]);
	fclose(fp);
#endif
}

#if defined(TCSUPPORT_CT_PORTAL_MANAGEMENT)
void check_lanip_exist(void)
{
	FILE *fp;
	uint32_t yiaddr;
	uint8_t matchid;
	char str[64] = {0};
	
	struct in_addr client;
	
	fp = fopen("/tmp/var/tmp/ip_vendor.conf","r");
	if (fp == NULL)
	{
		return;
	}
	
	while (fread(&yiaddr, sizeof(uint32_t), 1, fp) == 1)
	{	
		fread(&matchid, sizeof(uint8_t), 1, fp);
		if(getArpReply("br0", yiaddr, 0) != 0)
		{
			client.s_addr = yiaddr;
			memset(str, 0, sizeof(str));
			snprintf(str, sizeof(str),"iptables -t nat -D PRE_PORTAL -s %s -j RETURN", inet_ntoa(client));
			system(str);
			conntrack_delete(AF_INET, IPPROTO_TCP, yiaddr, 80);
		}
	}
	fclose(fp);
}

int 
inet_open_rawsock(char *iface, int protocol)
{
	int sock;

	if((sock = socket(PF_PACKET, SOCK_RAW, htons(protocol))) < 0)
	{
		perror("socket");
		return -1;
	}

    
	if(iface != NULL)
	{
		struct ifreq ifr;
		struct sockaddr_ll sll;

		memset(&ifr, 0, sizeof(ifr));
		strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name));    

		if ( ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
		{
			perror("ioctl(SIOCGIFINDEX)");
			close(sock);
			return -1;
		}

		memset(&sll, 0, sizeof(sll));
		sll.sll_family = AF_PACKET;
		sll.sll_ifindex = ifr.ifr_ifindex;
		sll.sll_protocol = htons(protocol);

		if ( bind(sock, (struct sockaddr *) &sll, sizeof(sll)) == -1)
		{
			perror("bind()");
			close(sock);
			return -1;
		}
	}

    return sock;
}

int 
inet_get_ifaddr(char *iface, unsigned char* mac, unsigned int *ip)
{
	int sock;
	struct ifreq ifr;

	if((sock = inet_open_rawsock(iface, ETH_P_IP)) < 0){
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, iface, sizeof(ifr.ifr_name));

	if (mac != NULL)
	{
		if ( ioctl(sock, SIOCGIFHWADDR, &ifr) < 0 )
		{
			perror("ioctl(SIOCGIFHWADDR)");
			close(sock);
			return -1;
		}
		memcpy(mac, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
	}

	if (ip != NULL)
	{
		if ( ioctl(sock, SIOCGIFADDR, &ifr) < 0 )
		{
			perror("ioctl(SIOCGIFADDR)");
			close(sock);
			return -1;
		}
		memcpy((char *)ip, ifr.ifr_addr.sa_data+2, sizeof(*ip));
	}
	close(sock);
	return 0;
}


int 
forge_arp_request(arp_packet_t *pkt, unsigned char *smac,
                              unsigned int sip, unsigned int tip)
{
    memset(pkt->ethhdr.tmac, 0xFF, ETH_ALEN);
    memcpy(pkt->ethhdr.smac, smac, ETH_ALEN);
    pkt->ethhdr.type     = htons(ETH_P_ARP);
    pkt->arphdr.hwtype  = htons(HW_TYPE_ETHERNET);
    pkt->arphdr.prot    = htons(ETH_P_IP);
    pkt->arphdr.hwlen   = ETH_ALEN;
    pkt->arphdr.protlen = 4;   /* ipv4 */
    pkt->arphdr.opcode  = htons(ARP_OPCODE_REQUEST);
    memcpy(pkt->arphdr.smac, smac, ETH_ALEN);;
    pkt->arphdr.sip     = sip;
    memset(pkt->arphdr.tmac, 0, ETH_ALEN);
    pkt->arphdr.tip     = tip;

    return 0;
}


static int
get_arp_reply(int sock, arp_packet_t *pkt, int timeout)
{
	fd_set rdfds;
	struct timeval tv;

	memset(pkt, 0, sizeof(arp_packet_t));
	tv.tv_sec = timeout;
	tv.tv_usec = 500000;//5000;

	FD_ZERO(&rdfds);
	FD_SET(sock, &rdfds);

	switch(select(sock + 1, &rdfds, NULL, NULL, &tv))
	{
	    case -1:
	        if (EINTR == errno)
	            break;
	        perror("select");
	    case 0:
	        /* timeout */
	        return -1;
	    default: 
	        if(FD_ISSET(sock, &rdfds))
	        {
	            if(recv(sock, pkt, sizeof(*pkt), 0) < 0 )
	            {
	                perror("recv");
	                return -1;
	            }
	        }
	}

    return 0;
}

/** 
 * Send a arp request and wait for response in timeout seconds, then
 * return the resolved ip address.
 * 
 * @param iface 
 * @param mac 
 * @param timeout 
 * 
 * @return 
 */
int
getArpReply(char *iface, unsigned int tip, int timeout)
{
	int sock;
	arp_packet_t pkt;
	unsigned int sip;
	char smac[ETH_ALEN];
	int ret = 0;

	if((sock = inet_open_rawsock(iface, ETH_P_ARP)) < 0){
		return -1;
	}
	if(inet_get_ifaddr(iface, smac, &sip) < 0){
		ret = -1;
		goto err;
	}
    
	forge_arp_request(&pkt, smac, sip, tip);

	if(send(sock, &pkt, sizeof(pkt), 0) < 0)
	{
		perror("send");
		ret = -1;
		goto err;
	}

	if(get_arp_reply(sock, &pkt, timeout) < 0)
	{
		perror("recv");
		ret = -1;
		goto err;
	}
 	if(pkt.arphdr.opcode == ARP_OPCODE_REPLY && pkt.arphdr.sip == tip){
		ret = 0;
 	}
	else{
		ret = -1;
	}
err:
	close(sock);
	return ret;
}
#endif
#ifdef TCSUPPORT_IPV6
void setIpv6Radio(mxml_node_t *top)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char ipVersion[5] = {0};
#if defined(TCSUPPORT_CT_IPV4_RADIO)			
	char string[64] = {0};
	mxml_node_t *node = NULL;
#endif

	
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],SYS_NODE);
  	strcpy(nodeName[1], SUB_NODE_NAME);
	if(getAttrValue(top, nodeName, SYS_IP_VERSION, ipVersion) == TCAPI_PROCESS_OK){
		/*create system.conf*/
#if defined(TCSUPPORT_CT_IPV4_RADIO)		
		sprintf(string, "/%s/%s", "var/run", "system.conf");
		node = mxmlFindElement(top, top, "Sys",
				NULL, NULL,
				MXML_DESCEND);
		node = mxmlFindElement(node, node, "Entry",
				NULL, NULL,
				MXML_DESCEND);
		if(node_write(node, string, QMARKS) != 0)
			return FAIL;
#endif
		if(!strcmp(ipVersion, "3")){   //IPv6 is enable
			system("echo 0 > /proc/sys/net/ipv6/conf/all/disable_ipv6");
#if defined(TCSUPPORT_CT_IPV4_RADIO)		
			system("echo 0 > /proc/sys/net/ipv4/conf/all/disable_ipv4");
#endif
		}
		else if(!strcmp(ipVersion, "2")){
			system("echo 0 > /proc/sys/net/ipv6/conf/all/disable_ipv6");
#if defined(TCSUPPORT_CT_IPV4_RADIO)		
			system("echo 1 > /proc/sys/net/ipv4/conf/all/disable_ipv4");
#endif
		}
		else if(!strcmp(ipVersion, "1")){   //IPv6 is disabled
			system("echo 1 > /proc/sys/net/ipv6/conf/all/disable_ipv6");
#if defined(TCSUPPORT_CT_IPV4_RADIO)		
			system("echo 0 > /proc/sys/net/ipv4/conf/all/disable_ipv4");
#endif
		}	
	}
	else
	{
		tcdbg_printf("setIpv6Radio: get SYS_IP_VERSION fail! \r\n");
	}		
}
#endif

#define BUFSZ 4096
void cmdPipe(char * command, char ** output)
{
	FILE *fp;
	char buf[BUFSZ];
	int len;

	*output = malloc(1);
	strncpy(*output, "", sizeof(*output));
	if((fp = popen(command, "r"))==NULL){
		return;
	}
	while((fgets(buf, BUFSZ, fp)) != NULL){
		len = strlen(*output) + strlen(buf);
		if((*output = realloc(*output, (sizeof(char)*(len+1))) )==NULL){
			return;
		}

		strcat(*output, buf);
	}

	pclose(fp);
	
}


#if defined(TCSUPPORT_CT_WAN_PTM)	
int get_dsl_state()
{	
	/* 2 : vdsl ; 1:ADSL; 0:down ;  -1: fail */
	int fd, ret;
	char string[160] = {0};
	
    fd = open("/proc/tc3162/adsl_stats",O_RDONLY|O_NONBLOCK);
    if( fd <= 0 ){
        return FAIL;
    }
    memset(string, 0, sizeof(string));
    ret = read(fd, string, sizeof(string));
    close(fd);
    if( ret <= 0 ){
        return FAIL;
    }      
    if(strstr(string, "up")){
        if(strstr(string, "PTM")){
            ret=2; //vdsl
#if defined(TCSUPPORT_CT_SWQOS)
            system("echo 1 > /proc/tc3162/qos_adsl_type");
#endif
        }
        else if(strstr(string, "ATM")){
            ret=1; //adsl  
#if defined(TCSUPPORT_CT_SWQOS)
            system("echo 0 > /proc/tc3162/qos_adsl_type");
#endif
        }else{
	   		ret=1; // means this dmt is not 3095	
#if defined(TCSUPPORT_CT_SWQOS)	 
	   	system("echo 0 > /proc/tc3162/qos_adsl_type");
#endif
        }
    }else{
		ret=0;  
    }
	return ret;
}

int getWanTransModeType(int pvc_index){	
	/* 1 : ATM ; 2:PTM ; 3: ATM&PTM*/
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	mxml_node_t *top = get_node_top();
	int ret = 0;
	char ptmEnable[8] = {0};
	char atmEnable[8] = {0};
	int isAtm = 0;
	int isPtm = 0;
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);

	if(getAttrValue(top, nodeName, "PTMEnable", ptmEnable) != TCAPI_PROCESS_OK ){
		tcdbg_printf("start_interface:fail get PTMEnable\n" );
		return FAIL;
	}

	if(strcmp(ptmEnable,"Yes") == 0)
		isPtm = 1;
	
	if(getAttrValue(top, nodeName, "ATMEnable", atmEnable) != TCAPI_PROCESS_OK ){
		tcdbg_printf("start_interface:fail get ATMEnable\n" );
		return FAIL;
	}
	
	if(strcmp(atmEnable,"Yes") == 0)
		isAtm = 1;

	ret = isAtm | (isPtm << 1);
	return ret;
}
#endif
#if defined(TCSUPPORT_CT_SYSTEM_EX)	
int system_ex(const char *command, int timeout, int times){
	int wait_val, pid;		
	int leftTime = timeout*100 ;
	int rv,i;
	int needRetry = 1;
	__sighandler_t save_quit, save_int, save_chld;	

	if (command == 0)		
		return 1;	

	save_quit = signal(SIGQUIT, SIG_IGN);
	save_int = signal(SIGINT, SIG_IGN);	
	save_chld = signal(SIGCHLD, SIG_DFL);	

	for(i = -1; i< times && 1 == needRetry; i++){
		needRetry = 0;
		if ((pid = vfork()) < 0) {		
			signal(SIGQUIT, save_quit);	
			signal(SIGINT, save_int);	
			signal(SIGCHLD, save_chld);	
			return -1;	
		}	

		if (pid == 0) {	
			signal(SIGQUIT, SIG_DFL);
			signal(SIGINT, SIG_DFL);	
			signal(SIGCHLD, SIG_DFL);	
			execl("/bin/sh", "sh", "-c", command, (char *) 0);	
			_exit(127);	
		}	

		/* Signals are not absolutly guarenteed with vfork */
		signal(SIGQUIT, SIG_IGN);	
		signal(SIGINT, SIG_IGN);	
		while((rv = waitpid(pid,&wait_val,WNOHANG)) <= 0){	
			if( -1 == rv && EINTR != errno){
				wait_val = -1;					
				break;
			}
		
			if(timeout > 0){
				usleep(10000);
				leftTime--;
				if(leftTime <= 0){
					if(kill(pid,9) == 0){
						waitpid(pid,NULL,0);
					}
					
					tcdbg_printf("system_ex:command=%s\n",command);	
					needRetry = 1;
					wait_val = -1;
					break;
				}
			}
		}
	}

	signal(SIGQUIT, save_quit);	
	signal(SIGINT, save_int);	
	signal(SIGCHLD, save_chld);
	return wait_val;
}
int system(const char *cmdStr){
	return system_ex(cmdStr,300, 0);
}
#endif


int isValidDnsIp(char * dnsIP){
	if(!check_ip_format(dnsIP)){		//dnsIp is invalid
		tcdbg_printf("isValidDnsIp:dnsIp is invalid!\r\n");
		return 0;	
	}

	if((strcmp(dnsIP,"255.255.255.255") == 0) || (strcmp(dnsIP,"0.0.0.0") == 0)){		
		tcdbg_printf("isValidDnsIp:dnsIp = %s is invalid!\r\n",dnsIP);
		return 0;
	}

	return 1;
}

int str2Hex(char *str){
      int val = 0;

      for(; *str != '\0'; ++str)
      {
          if(*str >= '0' && *str <= '9')
                  val = (val << 4) + (*str - '0');
          else if(*str >= 'A' && *str <= 'F')
                  val = (val << 4) + (0x0A + (*str - 'A'));
          else if(*str >= 'a' && *str <= 'f')
                  val = (val << 4) + (0x0A + (*str - 'a'));
          else
                 return -1;
      }
      return val;
}

int signalCwmpInfo(int mq, int reserved, int reserved2)
{
	int ret;
	long type = 1;
	int flag = 0;
	cwmp_msg_t message;
	
	memset(&message,0,sizeof(cwmp_msg_t));
	message.cwmptype = mq;
	message.text.reserved[0] = reserved;

	if(reserved2 != 0)
		message.text.reserved[1] = reserved2;

	tcdbg_printf("\r\nmq=%d, reserved=%d,reserved2=%d!!",mq,reserved,reserved2);

	ret = sendmegq(type,&message,flag);
	if(ret < 0)
	{
		tcdbg_printf("\r\nsend message error!!");
		return ret;
	}
	else
	{
		tcdbg_printf("\r\nsend message ok!");
		return ret;
	}
	return 0;
}

#if defined(TCSUPPORT_CT_PON)
int g_pon_state = 0;
int setPonState2(int state)
{
	g_pon_state = state;
}
int getPonState2()
{
	return g_pon_state;
}
#endif

