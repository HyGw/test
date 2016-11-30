/* dhcpc.c
 *
 * udhcp DHCP client
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>

#include "dhcpd.h"
#include "dhcpc.h"
#include "options.h"
#include "clientpacket.h"
#include "clientsocket.h"
#include "script.h"
#include "socket.h"
#include "common.h"
#include "signalpipe.h"
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_ARPPING)
#include "arpping.h"
#endif

static int state;
static unsigned long requested_ip; /* = 0 */
static unsigned long server_addr;
#if defined(TCSUPPORT_CT_FJ)
static unsigned long router_addr;
#endif
static unsigned long timeout;
static int packet_num; /* = 0 */
static int fd = -1;

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_FORCERENEW) || defined(TCSUPPORT_CT_DHCP_ARPPING) || defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_JOYME2)
int arpflag = 0;
int secs = 0;
#if defined(TCSUPPORT_CT_JOYME2)
int discover_time[]={0, 10, 20, 80};
#else
#if defined(TCSUPPORT_CT_FJ)
int discover_time[]={0,4,8,16,32,64,124,184,244,304,308};
#else
#if defined(TCSUPPORT_CT_PON_SC)
int discover_time[]={0,10,20,30,90,150,210,270,330,390,450};
#else
int discover_time[]={0,2,4,8,38};
#endif
#endif
#endif
static int arpping_result = 0;
static int arpping_renew_num = 0;
static long TIMEOUT_SENDARP = 10;
#define MAX_PVC_NUM	 	8
#define MAX_SMUX_NUM 	8
#endif

#define LISTEN_NONE 0
#define LISTEN_KERNEL 1
#define LISTEN_RAW 2
static int listen_mode;

struct client_config_t client_config = {
	/* Default options. */
	abort_if_no_lease: 0,
	foreground: 0,
	quit_after_lease: 0,
	background_if_no_lease: 0,
	interface: "eth0",
	pidfile: NULL,
	script: DEFAULT_SCRIPT,
	clientid: NULL,
	hostname: NULL,
#if defined(TCSUPPORT_CT_DHCPC_OPTION60)
	dhcp60: 0,
	dhcp60_option:NULL,
#endif
#if 0
	dhcp125c:0,
	dhcp125s:0,
	dhcp125Code: 0,
	dhcp125String:NULL,
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	ipoe_if: 0,
#endif
	ifindex: 0,
	arp: "\0\0\0\0\0\0",		/* appease gcc-3.0 */
};

#ifndef IN_BUSYBOX
static void __attribute__ ((noreturn)) show_usage(void)
{
	printf(
"Usage: udhcpc [OPTIONS]\n\n"
"  -c, --clientid=CLIENTID         Client identifier\n"
"  -H, --hostname=HOSTNAME         Client hostname\n"
"  -h                              Alias for -H\n"
"  -f, --foreground                Do not fork after getting lease\n"
"  -b, --background                Fork to background if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -i, --interface=INTERFACE       Interface to use (default: eth0)\n"
"  -n, --now                       Exit with failure if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -p, --pidfile=file              Store process ID of daemon in file\n"
"  -q, --quit                      Quit after obtaining lease\n"
"  -r, --request=IP                IP address to request (default: none)\n"
"  -s, --script=file               Run file at dhcp events (default:\n"
"                                  " DEFAULT_SCRIPT ")\n"
"  -v, --version                   Display version\n"
	);
	exit(0);
}
#else
#define show_usage bb_show_usage
extern void show_usage(void) __attribute__ ((noreturn));
#endif


/* just a little helper */
static void change_mode(int new_mode)
{
	DEBUG(LOG_INFO, "entering %s listen mode",
		new_mode ? (new_mode == 1 ? "kernel" : "raw") : "none");
	if (fd >= 0) close(fd);
	fd = -1;
	listen_mode = new_mode;
}


/* perform a renew */
static void perform_renew(void)
{
	LOG(LOG_INFO, "Performing a DHCP renew");
	switch (state) {
	case BOUND:
		change_mode(LISTEN_KERNEL);
	case RENEWING:
	case REBINDING:
		state = RENEW_REQUESTED;
		break;
	case RENEW_REQUESTED: /* impatient are we? fine, square 1 */
		run_script(NULL, "deconfig");
	case REQUESTING:
	case RELEASED:
		change_mode(LISTEN_RAW);
		state = INIT_SELECTING;
		break;
	case INIT_SELECTING:
		break;
	}

	/* start things over */
	packet_num = 0;

	/* Kill any timeouts because the user wants this to hurry along */
	timeout = 0;
}


/* perform a release */
static void perform_release(void)
{
	char buffer[16];
	struct in_addr temp_addr;

	/* send release packet */
#if defined(TCSUPPORT_CT)
	if (state == BOUND || state == RENEWING || state == REBINDING) {
#else
	if (state == BOUND || state == RENEWING || state == REBINDING || state == REQUESTING || state == RENEW_REQUESTED) {
#endif
		temp_addr.s_addr = server_addr;
		sprintf(buffer, "%s", inet_ntoa(temp_addr));
		temp_addr.s_addr = requested_ip;
		LOG(LOG_INFO, "Unicasting a release of %s to %s",
				inet_ntoa(temp_addr), buffer);
		send_release(server_addr, requested_ip); /* unicast */
		run_script(NULL, "deconfig");
	}
	LOG(LOG_INFO, "Entering released state");

	change_mode(LISTEN_NONE);
	state = RELEASED;
	timeout = 0x7fffffff;
}


static void client_background(void)
{
	background(client_config.pidfile);
	client_config.foreground = 1; /* Do not fork again. */
	client_config.background_if_no_lease = 0;
}

#if defined(DHCP_OPT121)
 uint32_t get32 (	uint8_t	*cp)
{
	uint32_t rval;

	rval = *cp++;
	rval <<= 8;
	rval |= *cp++;
	rval <<= 8;
	rval |= *cp++;
	rval <<= 8;
	rval |= *cp;

	return rval;
} /* get32 */

void dhcpExtractClasslessRoute(uint8_t  *data, int len)
{
	int optionlen, index, i;
	int value, remain;
	uint8_t  ip[4]={0,0,0,0};
	struct in_addr destmask;
	struct in_addr routeIP;
	char cmd[128];
	char tmp[20];
	uint8_t metric = 2;
	uint8_t defRtCh = 0;

	if(!tcapi_get("Dhcpd_Option121","dhcpmetric", tmp))
		metric = atoi(tmp);
	if(!tcapi_get("Dhcpd_Option121","changeDefRt", tmp))
		defRtCh = atoi(tmp);
		
	index = 0;
	optionlen = len;

	if (optionlen < 5)
		return;

	while(index<optionlen){
		memset(ip, 0, 4);
		
		// the max IPv4 address is 32 bits
		if(data[index] > 32)
		{
			tcdbg_printf("parser option121 error ----- IP error.\r\n");
			return;
		}

		if( 0 == data[index] )
		{
			if(defRtCh == 0){
				index += 5;
				continue;
			}
			else{
				destmask.s_addr = 0;		//destination mask	
				routeIP.s_addr = get32(&data[index+1]);
				index += 5;
			}
		}
		else
		{
			value = (data[index]-1)/8;
			remain = data[index]%8;
			if (!remain)
				remain = 8;
			if(index+value+1 >= optionlen){
				tcdbg_printf("parser option121 error ----- expand lenth.\r\n");
				return;
			}

			data[index+value+1] &= ~(0xFF >> remain);
			memcpy(ip,&data[index+1],value+1);		//destination IP
			destmask.s_addr  = (0xFFFFFFFF << (32 -data[index]));		//destination mask			
			routeIP.s_addr = get32(&data[index+value+2]);		//route IP				
			index += (value+6);		
			if(get32(ip)==0){
				continue;
			}
		}
		if (routeIP.s_addr)
		{
			if((get32(ip) == 0) && (destmask.s_addr == 0))
			{
				memset (cmd, 0, 128);
				strcpy(cmd, "/sbin/route del default");
          			system(cmd);
			}
			strcpy(tmp, inet_ntoa(destmask)); //inet_ntoa can't be called twice in an expression
			memset (cmd, 0, 128);
			sprintf( cmd, "/sbin/route add -net %d.%d.%d.%d netmask %s gw %s metric %d", ip[0], ip[1], ip[2], ip[3], tmp, inet_ntoa(routeIP), metric);
			system(cmd);
		}			
	}
		
	return;
}
#endif
#if defined(TCSUPPORT_TR069_OPT43)
int min(int a, int b)
{
	return (a>b) ? b : a;
}
static void dhcpExtractOption43(uint8_t  *data, int len)
{
	uint8_t *datap = data;
	int index = 0, paramLen = 0, ret = 0, spSaveFlag = 0, bootstrapFlag = 0;
	char tempCwmpString[CwmpTempStringMaxLen];
	char buf[CwmpTempStringMaxLen];

	if(data == NULL){
		return;
	}
	
	while (index < len) {
		switch (datap[index]){
			case OPT43_SUBOPT_ACS_URL:
		#ifdef OPT43_DBG
				tcdbg_printf("%s: acs url case\n", __FUNCTION__);
		#endif
				paramLen = min((CwmpURLMaxLen-1), datap[index+1]);
				memset(tempCwmpString, 0, CwmpTempStringMaxLen);
				memcpy(tempCwmpString, &datap[index+2], paramLen);

				memset(buf, 0, sizeof(buf));
				tcapi_get("Cwmp_Entry", "acsUrl", buf);
				
				if (0 != strcmp(buf, tempCwmpString)) {
					tcapi_set("Cwmp_Entry", "acsUrl", (char*)tempCwmpString);
					spSaveFlag = 1;
					bootstrapFlag = 1;
				}
				
	   			break;
			case OPT43_SUBOPT_PRVCODE:
		#ifdef OPT43_DBG
				tcdbg_printf("%s: prv code case.\n", __FUNCTION__);
		#endif
				paramLen = min((CwmpPrvCodeMaxLen-1), datap[index+1]);
				memset(tempCwmpString, 0, CwmpTempStringMaxLen);
				memcpy(tempCwmpString, &datap[index+2], paramLen);

				memset(buf, 0, sizeof(buf));
				tcapi_get("Cwmp_Entry", "PrvCode", buf);

				if (0 != strcmp(buf, tempCwmpString)) {
					tcapi_set("Cwmp_Entry", "PrvCode", (char*)tempCwmpString);
					spSaveFlag = 1;
				}
					
	   			break;
			default:
		#ifdef OPT43_DBG
				tcdbg_printf("%s,default case,and datap[index]=%d\n", __FUNCTION__, datap[index]);
		#endif
				break;
		}

		if (datap[index] == DHCP_PADDING)
			index++;
		else if(datap[index] == DHCP_END)
			break;
		else
			index += (datap[index+1] + 2);
	}

#ifdef OPT43_DBG
	tcdbg_printf("%s spSaveFlag=%d bootstrapFlag=%d\n", __FUNCTION__, spSaveFlag, bootstrapFlag);
#endif

	/* save to flash */
	if (spSaveFlag)
		tcapi_save();
	/* send msg to cwmp for signal inform */
	if (bootstrapFlag)
		tcapi_commit("Cwmp_Entry");
	
	return;
}
#endif

#if 0
long int str_to_uint(char *buff, int len)
{
	unsigned long code = 0;
	int i;
	for(i = 0; i < 2*len; i++)
	{
		code <<= 4;
		if(buff[i] >= '0' && buff[i] <= '9')
			code |= (buff[i] - '0');
		else if(buff[i] >= 'A' && buff[i] <= 'F')
			code |= (buff[i] - 'A' + 10);
		else 
			return 0xffff;
	}

	return code;
	
}
static uint8_t parse_option_from_interface(char *ifname, struct client_config_t *conf)
{
	char *ptr = strstr(ifname,"_");
	int i, j;
	char nodeName[32] = {0};
	char tmp[64] = {0};
	int idx = 0;
	uint8_t ret = 0;

	char *option60Str = xmalloc(136);
	char *option125Str = xmalloc(32);
	char attrName[][32] = 
	{
		{ "option60String"},
		{ "Category"},
		{ "ModelName"},
		{ "CustomerSWVersion"},		
	};

	memset(option60Str, 0, 136);
	memset(option125Str, 0, 32);
	
	if(ptr == NULL)
	{
		LOG(LOG_ERR, "fatal error in parse_option_from_interface option60 & option125 settings wrong!\n");
		free(option60Str); free(option125Str);
		return (1<<3);
	}

	if(ptr[-1] < '0' || ptr[-1] > '7' || ptr[-1] < 0 || ptr[1] > '7')
	{
		LOG(LOG_ERR, "fatal error in parse_option_from_interface option60 & option125 settings wrong!\n");
		free(option60Str); free(option125Str);
		return (1<<3);
	}

	i = ptr[-1] - '0';  j = ptr[1] - '0';
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);

#if defined(TCSUPPORT_CT_DHCPC_OPTION60)
	/* processing option 60 settings*/
	tcapi_get(nodeName, "option60Enable", tmp);

	if(!strcmp(tmp, "Yes"))
	{
		conf->dhcp60 = 1;

		option60Str[0] = 60;
		option60Str[1] = 2;
		memset(tmp, 0, sizeof(tmp));
		tcapi_get(nodeName, "option60Mode", tmp);

		if(atoi(tmp) == 1)
		{
			tcapi_get(nodeName, "option60String", tmp);
			option60Str[1] = strlen(tmp);
			strcpy(&option60Str[2], tmp);
			
		}
		else
		{		
			memset(tmp, 0, sizeof(tmp));
			tcapi_get("DeviceInfo_devParaStatic", "option60Code",tmp );
			
			*((unsigned short *)(option60Str + 2)) = str_to_uint(tmp,2);
			idx = 4;

			if(0xffff == *((unsigned short *)(option60Str + 2)) )
			{
				conf->dhcp60 = 0;
				free(option60Str);
				ret |= 1;
				LOG(LOG_ERR, "setting option60 error, the option checking is ommitted!\n");
				goto out_dhcp60;
			}
			
			for(i = 0; i < sizeof(attrName)/sizeof(attrName[0]); i++)
			{
				memset(tmp, 0, sizeof(tmp));
				if(0 == i)
					tcapi_get(nodeName, attrName[i],tmp );
				else
					tcapi_get("DeviceInfo_devParaStatic", attrName[i],tmp );

				if(0 != strcmp(tmp, ""))
				{
				     	option60Str[idx] = i + 1;
					option60Str[idx+1] = strlen(tmp);
					option60Str[1] += (option60Str[idx+1] + 2); 
					strcpy(&option60Str[idx+2], tmp);
					idx +=  (strlen(tmp) +2);
				}
			}
		}
		conf->dhcp60_option = option60Str;
		
	}

	
	/* processing option 60 settings end*/
out_dhcp60:
	LOG(LOG_ERR, "option60Enable = %d\n",client_config.dhcp60);
#endif


#if defined(TCSUPPORT_CT_DHCPC_OPTION125)
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(nodeName, "option125CodeEnable",tmp );
	if(!strcmp(tmp, "Yes"))
		conf->dhcp125c= 1;

	memset(tmp, 0, sizeof(tmp));
	tcapi_get(nodeName, "option125StringEnable",tmp );
	if(!strcmp(tmp, "Yes"))
		conf->dhcp125s= 1;
	
	memset(tmp, 0, sizeof(tmp));
	tcapi_get("DeviceInfo_devParaStatic", "option125Code",tmp );
	conf->dhcp125Code = str_to_uint(tmp,4);

	if(0xffff == conf->dhcp125Code)
	{
		conf->dhcp125c= 0;
		conf->dhcp125s= 0;
		ret |= (1<<1);
		free(option125Str);
		LOG(LOG_ERR, "setting option125 error, the option checking is ommitted!\n");
		goto out_dhcp125;
	}
	memset(tmp, 0, sizeof(tmp));
	tcapi_get("DeviceInfo_devParaStatic", "option125String",option125Str );
	conf->dhcp125String = option125Str;
out_dhcp125:
	if(ret & 0x1)
		LOG(LOG_ERR, "set option 60 error!\n");
	 if(ret & 0x2)
		LOG(LOG_ERR, "set option 125 error!\n");

	LOG(LOG_ERR, "option125CodeEnable = %d, option125StringEnable = %d, option125Code = %x, option125String = %s\n",
					client_config.dhcp125c, client_config.dhcp125s, client_config.dhcp125Code, client_config.dhcp125String);
#endif
	return ret;
}
#endif

#if defined(TCSUPPORT_CT_DHCPC_OPTION60)
static uint8_t make_option60(char *ifname, struct client_config_t *conf)
{
	char *ptr = strstr(ifname,"_");
	int i, j;
	char nodeName[32] = {0};
	char attrName[32] = {0};
	char tmp[128] = {0};
	int type = 0;
	int valueMode = 0;
	char sipProtocol[16] = {0};
	int idx = 0;
	int len = 0;

	char *option60Str = xmalloc(256);

	memset(option60Str, 0, sizeof(option60Str));
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	if ( client_config.ipoe_if )
	{
		memset(tmp, 0, sizeof(tmp));
		if ( 0 == tcapi_get("IpoeEmulator_Entry", "VendorClassID", tmp)
			&& 0 != tmp[0] )
		{
			len = strlen(tmp);
			option60Str[0] = 60;
			option60Str[1] = len;
			memcpy(&option60Str[2], tmp, len);
			conf->dhcp60 = 1;
			conf->dhcp60_option = option60Str;
		}
		else
			free(option60Str);
		return 1;
	}
#endif

	if(ptr == NULL)
	{
		LOG(LOG_ERR, "fatal error in make_option60 settings wrong!\n");
		free(option60Str);
		return 0;
	}

	if(ptr[-1] < '0' || ptr[-1] > '7' || ptr[1] < 0 || ptr[1] > '7')
	{
		LOG(LOG_ERR, "fatal error in make_option60 settings wrong!\n");
		free(option60Str);
		return 0;
	}

	i = ptr[-1] - '0';  j = ptr[1] - '0';
	
	if(ptr[-2]> '0' && ptr[-2] <= '9')
		i += (ptr[-2] - '0')*10;
	if(ptr[2] > '0' && ptr[2] <= '9')
		j += (ptr[2] - '0')*10;
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);

	for(i=1; i<5; i++){
		memset(tmp, 0, sizeof(tmp));
		sprintf(attrName, "Option60Enable%d", i);
		tcapi_get(nodeName, attrName, tmp);
		if(!strcmp(tmp, "Yes")){
			if(conf->dhcp60 != 1){	//option60 code, lenggth, enterprise code
				conf->dhcp60 = 1;
				option60Str[0] = 60;
				option60Str[1] = 2; //length = enterprise code + field
				option60Str[2] = 0x0000;
				idx = 4;
			}
			memset(tmp, 0, sizeof(tmp));
			sprintf(attrName, "Option60Type%d", i);
			if ( 0 != tcapi_get(nodeName, attrName, tmp) )
				strcpy(tmp, "34");
			type = atoi(tmp);

			memset(tmp, 0, sizeof(tmp));
			sprintf(attrName, "Option60ValueMode%d", i);
#if defined(TCSUPPORT_CT_PON_CN_JS)
			if(tcapi_get(nodeName, attrName, tmp)<0)
			{
				tcapi_get("VoIPBasic_Common", "SIPProtocol", sipProtocol);
				if(!strcmp(sipProtocol, "IMSSIP"))
					strcpy(tmp, "2");
				else
					strcpy(tmp, "3");
			
			}
#else
			tcapi_get(nodeName, attrName, tmp);
#endif
			valueMode = atoi(tmp);	//0:value get from ITMS, 1,2: value generated bye hgw

			memset(tmp, 0, sizeof(tmp));
			switch(type){
				case 1:	//Vendor name
					if(valueMode == 0){
						sprintf(attrName, "Option60Value%d", i);
						tcapi_get(nodeName, attrName, tmp);
					}else{
						tcapi_get("Dhcpd_Common","VendorName",tmp);
					}
					break;
				case 2:	//device type
					if(valueMode == 0){
						sprintf(attrName, "Option60Value%d", i);
						tcapi_get(nodeName, attrName, tmp);
					}else{
						//strcpy(tmp, "HGW");	
						tcapi_get("Dhcpd_Common","DeviceType",tmp);
					}
					break;
				case 3:	//device id
					if(valueMode == 0){
						sprintf(attrName, "Option60Value%d", i);
						tcapi_get(nodeName, attrName, tmp);
					}else{
						tcapi_get("Dhcpd_Common","DeviceId",tmp);
					}
					break;
				case 4:	//sw/hw version
					if(valueMode == 0){
						sprintf(attrName, "Option60Value%d", i);
						tcapi_get(nodeName, attrName, tmp);
					}else{
						tcapi_get("Dhcpd_Common","Version",tmp);
					}
					break;
				case 5:	//port mapping protocol and port number
					if(valueMode == 0){
						sprintf(attrName, "Option60Value%d", i);
						tcapi_get(nodeName, attrName, tmp);
					}else{
						
					}
					break;
				case 34:	//VoIP username
#if defined(TCSUPPORT_CT_PON_CN_JS)
					len = make_option60_voip_msg((unsigned char*)tmp, sizeof(tmp), valueMode);
#else
						// valueMode = 2, F.40
					len = make_option60_voip_msg((unsigned char*)tmp, sizeof(tmp));
#endif
					break;
				default:
					break;
			}
			if(tmp){
				option60Str[idx] = type;				//field type
				if(type == 34){
					option60Str[idx+1] = len;
				}else{
					option60Str[idx+1] = strlen(tmp);		//field length
				}
				option60Str[1] += (option60Str[idx+1] + 2);	//option length
				if((idx+2+option60Str[idx+1]) < 256)
					memcpy(&option60Str[idx+2], tmp, option60Str[idx+1]);			//field value
				idx +=  (option60Str[idx+1] +2);
			}
		}
	}
	conf->dhcp60_option = option60Str;
	
	return 1;
}
#endif

#if defined(TCSUPPORT_CT_DHCPC_OPTION125)

static uint8_t get_option125_from_ITMS(char *ifname, struct client_config_t *conf){
	char *ptr = strstr(ifname,"_");
	int i, j;
	char nodeName[32] = {0};
	char attrName[32] = {0};
	char tmp[256] = {0};

#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
	if ( client_config.ipoe_if )
	{
		for ( i = 0; i < 4; i ++ )
		{
			conf->option125_info[i].enable= 0;
		}
		return 1;
	}
#endif

	if(ptr == NULL)
	{
		LOG(LOG_ERR, "fatal error in get_option125_from_ITMS settings wrong!\n");
		return 0;
	}

	if(ptr[-1] < '0' || ptr[-1] > '7' || ptr[1] < 0 || ptr[1] > '7')
	{
		LOG(LOG_ERR, "fatal error in get_option125_from_ITMS settings wrong!\n");
		return 0;
	}

	i = ptr[-1] - '0';  j = ptr[1] - '0';

	if(ptr[-2]> '0' && ptr[-2] <= '9')
		i += (ptr[-2] - '0')*10;
	if(ptr[2] > '0' && ptr[2] <= '9')
		j += (ptr[2] - '0')*10;
	
	sprintf(nodeName, "Wan_PVC%d_Entry%d", i, j);
	for(i=1; i<5; i++){
		memset(tmp, 0, sizeof(tmp));
		sprintf(attrName, "Option125Enable%d", i);
		tcapi_get(nodeName, attrName, tmp);
		if(!strcmp(tmp, "Yes")){
			conf->option125_info[i-1].enable= 1;
			memset(tmp,0,sizeof(tmp));
			sprintf(attrName,"Option125Type%d",i);
			if ( 0 != tcapi_get(nodeName,attrName,tmp) )
				strcpy(tmp, "2");
			conf->option125_info[i-1].type = atoi(tmp);
			memset(tmp,0,sizeof(tmp));
			sprintf(attrName,"Option125SubCode%d",i);
			tcapi_get(nodeName,attrName,tmp);
			conf->option125_info[i-1].subopt_code = atoi(tmp);
			memset(tmp,0,sizeof(tmp));
			sprintf(attrName,"Option125SubData%d",i);
			if ( 0 != tcapi_get(nodeName,attrName,tmp) )
				strcpy(tmp, "1");
			strncpy(conf->option125_info[i-1].subopt_data, tmp, sizeof(conf->option125_info[i-1].subopt_data));
			
			memset(tmp,0,sizeof(tmp));
			sprintf(attrName,"Option125Value%d",i);
			tcapi_get(nodeName,attrName,tmp);
			strcpy(conf->option125_info[i-1].value, tmp);
		}else{
			conf->option125_info[i-1].enable= 0;
		}
	}
 	return 1;
}

static uint8_t match_option125_with_ITMS(uint8_t *option125ptr,struct client_config_t *conf)
{
	int i;
	uint8_t *tmp = NULL;
#if defined(TCSUPPORT_CT_PON_CZ_GD)
	uint8_t *data=option125ptr;
	int len = 0;/* option125 value length*/
#endif

	for(i = 0;i < 4; i++){
		if(conf->option125_info[i].enable){
			if(option125ptr){
				if(conf->option125_info[i].type == 2){
					//tcdbg_printf("\nmatch_option125_with_ITMS, type = 2, value = %s", conf->option125_info[i].value);
#if defined(TCSUPPORT_CT_PON_CZ_GD)					
					len = data[4];
					if(strlen(conf->option125_info[i].value) != len)
						return 0;
					if(strncmp(option125ptr+5,conf->option125_info[i].value,len))
						return 0;
#else
					tmp = strstr(option125ptr+5, conf->option125_info[i].value);
					if(!tmp)
						return 0;
#endif
				}
				else{
					tmp = strstr(option125ptr+5,conf->option125_info[i].subopt_data);
					if(!tmp || (tmp[-2] != conf->option125_info[i].subopt_code))
						return 0;
				}
			}
			else{
				return 0;
			}
		}
	}
	tcdbg_printf("\r\nmatch_option125_with_ITMS OK!");
	return 1;			
}
#endif

#ifdef COMBINED_BINARY
int udhcpc_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	uint8_t *temp, *message;
	unsigned long t1 = 0, t2 = 0, xid = 0;
	unsigned long start = 0, lease;
	fd_set rfds;
	int retval;
	struct timeval tv;
	int c, len;
	struct dhcpMessage packet;
	struct in_addr temp_addr;
	long now;
	int max_fd;
	int sig;
	int converge[6];
	int i;
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_FORCERENEW) || defined(TCSUPPORT_CT_DHCP_ARPPING) || defined(TCSUPPORT_CT_PON_SC)
	char TR069ArpPingFlag[4] = {0};
	char VoIPArpPingFlag[4] = {0};
	char wan_node[32] = {0};
	char wan_if_val[20] = {0};
	char servicelist_val[32] = {0};
	int ret = 0;
	int j=0;
	int TR069orVoIPFlag = 0;
	int TimeOutChangedFlag = 0;
	int hitTR069Count = 0;
#endif
#if 0
	char *option125ptr = NULL;
#endif
#if  defined(DHCP_OPT121)
	uint8_t enable121 = 1;
	char tmp[4] = {0};
#endif
	static const struct option arg_options[] = {
		{"clientid",	required_argument,	0, 'c'},
		{"foreground",	no_argument,		0, 'f'},
		{"background",	no_argument,		0, 'b'},
		{"hostname",	required_argument,	0, 'H'},
		{"hostname",    required_argument,      0, 'h'},
		{"interface",	required_argument,	0, 'i'},
		{"now", 	no_argument,		0, 'n'},
		{"pidfile",	required_argument,	0, 'p'},
		{"quit",	no_argument,		0, 'q'},
		{"request",	required_argument,	0, 'r'},
		{"script",	required_argument,	0, 's'},
		{"version",	no_argument,		0, 'v'},
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
		{"ipoesim", no_argument,		0, 'e'},
#endif
		{0, 0, 0, 0}
	};

	/* get options */
	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "c:fbH:h:i:np:qr:s:m:v"
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
			"e"
#endif
			, arg_options, &option_index);
		if (c == -1) break;

		switch (c) {
		case 'c':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			if (client_config.clientid) free(client_config.clientid);
			client_config.clientid = xmalloc(len + 2);
			client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
			client_config.clientid[OPT_LEN] = len;
			client_config.clientid[OPT_DATA] = '\0';
			strncpy(client_config.clientid + OPT_DATA, optarg, len);
			break;
		case 'f':
			client_config.foreground = 1;
			break;
		case 'b':
			client_config.background_if_no_lease = 1;
			break;
		case 'h':
		case 'H':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			if (client_config.hostname) free(client_config.hostname);
			client_config.hostname = xmalloc(len + 2);
			client_config.hostname[OPT_CODE] = DHCP_HOST_NAME;
			client_config.hostname[OPT_LEN] = len;
			strncpy(client_config.hostname + 2, optarg, len);
			break;
		case 'i':
			client_config.interface =  optarg;
			break;
		case 'n':
			client_config.abort_if_no_lease = 1;
			break;
		case 'p':
			client_config.pidfile = optarg;
			break;
		case 'q':
			client_config.quit_after_lease = 1;
			break;
		case 'r':
			requested_ip = inet_addr(optarg);
			break;
		case 's':
			client_config.script = optarg;
			break;
		case 'm':
			sscanf(optarg, "%02x:%02x:%02x:%02x:%02x:%02x", &converge[0], &converge[1], &converge[2], &converge[3], &converge[4], &converge[5]);
			for (i=0; i<6; i++)
				client_config.arp[i] = (uint8_t)converge[i];
			break;
		case 'v':
			printf("udhcpcd, version %s\n\n", VERSION);
			return 0;
			break;
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
		case 'e':
			client_config.ipoe_if = 1;
			break;
#endif
		default:
			show_usage();
		}
	}

	/* Start the log, sanitize fd's, and write a pid file */
	start_log_and_pid("udhcpc", client_config.pidfile);
	if (read_interface(client_config.interface, &client_config.ifindex,
			   NULL, client_config.arp) < 0)
		return 1;

#if 0
	parse_option_from_interface(client_config.interface, &client_config);
#endif
#if defined(TCSUPPORT_CT_DHCPC_OPTION60)
	make_option60(client_config.interface, &client_config);
#endif
#if defined(TCSUPPORT_CT_DHCPC_OPTION125)
	get_option125_from_ITMS(client_config.interface, &client_config);
#endif
	
	if (!client_config.clientid) {
		client_config.clientid = xmalloc(6 + 3);
		client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
		client_config.clientid[OPT_LEN] = 7;
		client_config.clientid[OPT_DATA] = 1;
		memcpy(client_config.clientid + 3, client_config.arp, 6);
	}

	/* setup the signal pipe */
	udhcp_sp_setup();

	state = INIT_SELECTING;
	run_script(NULL, "deconfig");
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_PON_SC)
	//delay start,0-60s
	sleep(random_xid()%61);
#endif
	change_mode(LISTEN_RAW);

#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_ARPPING) || defined(TCSUPPORT_CT_PON_SC)
	memset(wan_if_val, 0, sizeof(wan_if_val));
	for (i = 0; i < MAX_PVC_NUM; i++) {
		for (j = 0; j < MAX_SMUX_NUM; j++) {
			sprintf(wan_node,  "Wan_PVC%d_Entry%d", i, j);
			ret = tcapi_get(wan_node, "IFName", wan_if_val);
			if (ret < 0) {
				continue;
			}		
			if (!strcmp(wan_if_val, client_config.interface)) {
				tcapi_get(wan_node, "ServiceList", servicelist_val);
				if(strstr(servicelist_val,"TR069")){
					TR069orVoIPFlag = 1;	
					break;
				}
#if defined(TCSUPPORT_CT_FJ)
				if(strstr(servicelist_val,"VOICE")){
					TR069orVoIPFlag = 2;	
					break;
				}
#endif
			}
		}
	}
#endif

	for (;;) {

		tv.tv_sec = timeout - uptime();
		tv.tv_usec = 0;

		if (listen_mode != LISTEN_NONE && fd < 0) {
			if (listen_mode == LISTEN_KERNEL)
				fd = listen_socket(INADDR_ANY, CLIENT_PORT, client_config.interface);
			else
				fd = raw_socket(client_config.ifindex);
			if (fd < 0) {
				LOG(LOG_ERR, "FATAL: couldn't listen on socket, %m");
				return 0;
			}
		}
		max_fd = udhcp_sp_fd_set(&rfds, fd);
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_ARPPING)
		TimeOutChangedFlag = 0;
		if(RENEWING == state || BOUND == state){
			if(0 != TR069orVoIPFlag && tv.tv_sec > TIMEOUT_SENDARP){
				tv.tv_sec = TIMEOUT_SENDARP;
				TimeOutChangedFlag = 1;
			}
			tcapi_get("Cwmp_Entry","Inform_Status",TR069ArpPingFlag);
			if(
#if !defined(TCSUPPORT_CT_FJ)
			0!=strcmp(TR069ArpPingFlag,"8") && 
#endif
			1 == TR069orVoIPFlag){
				hitTR069Count++;
				if(hitTR069Count>=24){
					arpflag = 1;
					hitTR069Count = 0;
				}
			}
			else
				hitTR069Count=0;
#if defined(TCSUPPORT_CT_FJ)			
			if(0==arpflag){
				tcapi_get("System_Entry","VoIPArpPing",VoIPArpPingFlag);
				if(0==strcmp(VoIPArpPingFlag,"1") && 2 == TR069orVoIPFlag){
					arpflag = 2;
				}		
			}
#endif
		}
#endif

		if (tv.tv_sec > 0) {
			DEBUG(LOG_INFO, "Waiting on select...");
			retval = select(max_fd + 1, &rfds, NULL, NULL, &tv);
		} else retval = 0; /* If we already timed out, fall through */

		now = uptime();
		if (retval == 0) {
			/* timeout dropped to zero */
			switch (state) {
			case INIT_SELECTING:
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_PON_SC)
#if defined(TCSUPPORT_CT_PON_SC)
			if( NULL != strstr(servicelist_val,"VOICE") ){
#endif
				if (packet_num < 10) {
#else
#if defined(TCSUPPORT_CT_DHCP_FORCERENEW)
				if (packet_num < 4) {
#else
				if (packet_num < 3) {
#endif
#endif
					if (packet_num == 0)
						xid = random_xid();

#if defined(TCSUPPORT_CT_FJ)
					secs=discover_time[packet_num];
#endif
					/* send discover packet */
					send_discover(xid, requested_ip); /* broadcast */
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_FORCERENEW) || defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_JOYME2)
					timeout = now + (discover_time[packet_num+1]-discover_time[packet_num]);
#else
					timeout = now + ((packet_num == 2) ? 4 : 2);
#endif
					packet_num++;
				} else {
					run_script(NULL, "leasefail");
					if (client_config.background_if_no_lease) {
						LOG(LOG_INFO, "No lease, forking to background.");
						client_background();
					} else if (client_config.abort_if_no_lease) {
						LOG(LOG_INFO, "No lease, failing.");
						return 1;
				  	}
#if defined(TCSUPPORT_CT_PON_SC)
					packet_num = 4;
#else
#if defined(TCSUPPORT_CT_JOYME2)
					packet_num = 2;
#else
					/* wait to try again */
					packet_num = 0;
#endif
#endif
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_FORCERENEW) || defined(TCSUPPORT_CT_PON_SC) || defined(TCSUPPORT_CT_PON_JX) || defined(TCSUPPORT_CT_JOYME2)
					timeout = now;
#if defined(TCSUPPORT_CT_PON_HUB)
					timeout = now + 60;
#endif
#else
					timeout = now + 60;
#endif
				}
#if defined(TCSUPPORT_CT_PON_SC)
			}	
			else{
				if (packet_num < 3) {
					if (packet_num == 0)
						xid = random_xid();
					/* send discover packet */
					send_discover(xid, requested_ip); /* broadcast */
					timeout = now + ((packet_num == 2) ? 4 : 2);
					packet_num++;
				} else {
					run_script(NULL, "leasefail");
					if (client_config.background_if_no_lease) {
						LOG(LOG_INFO, "No lease, forking to background.");
						client_background();
					} else if (client_config.abort_if_no_lease) {
						LOG(LOG_INFO, "No lease, failing.");
						return 1;
				  	}
					packet_num = 0;
					timeout = now + 60;
				}
			}
#endif
				break;
			case RENEW_REQUESTED:
			case REQUESTING:
                                /*decrease the number of send request.shnwind modify 3->2*/
				if (packet_num < 2) {
					/* send request packet */
					if (state == RENEW_REQUESTED)
						send_renew(xid, server_addr, requested_ip); /* unicast */
					else send_selecting(xid, server_addr, requested_ip); /* broadcast */

					timeout = now + ((packet_num == 1) ? 10 : 2);
					packet_num++;
				} else {
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
					if ( client_config.ipoe_if )
					{
						run_script(NULL, "requestfail");
						return 0;
					}
#endif
					/* timed out, go back to init state */
					if (state == RENEW_REQUESTED) run_script(NULL, "deconfig");
					state = INIT_SELECTING;
					timeout = now;
					packet_num = 0;
					change_mode(LISTEN_RAW);
				}
				break;
			case BOUND:
				/* Lease is starting to run out, time to enter renewing state */
				state = RENEWING;
				change_mode(LISTEN_KERNEL);
				DEBUG(LOG_INFO, "Entering renew state");
				/* fall right through */
			case RENEWING:
				/* Either set a new T1, or enter REBINDING state */
#if defined(TCSUPPORT_CT_FJ)
				if ((t2 - t1) <= (lease / 14400 + 1) || (t2 - t1) < (0.1875*lease - 1)) {
#else
				if ((t2 - t1) <= (lease / 14400 + 1)) {
#endif
					/* timed out, enter rebinding state */
					state = REBINDING;
					timeout = now + (t2 - t1);
					DEBUG(LOG_INFO, "Entering rebinding state");
				} else {
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_ARPPING)
#if defined(TCSUPPORT_CT_FJ)
					if( 0 == arpping_renew_num )//arping not reply, now send request every 2s
					{
#endif
					arpping_result = 1;
#if defined(TCSUPPORT_CT_FJ)
					}
#endif
					if(0 != arpflag){/* send a arp packet */						
						if (
#if defined(TCSUPPORT_CT_FJ)
						arpping(router_addr, requested_ip, client_config.arp, client_config.interface) == 0
#else
						arpping(server_addr, requested_ip, client_config.arp, client_config.interface) == 0
#endif
						){//arpping ok
							arpping_result = 1;
						}
						else{//arpping timeout
							arpping_result = 0;						
						}
#if defined(TCSUPPORT_CT_DHCP_ARPPING)
						if(1 != arpping_result){
							if (
#if defined(TCSUPPORT_CT_FJ)
								arpping(router_addr, requested_ip, client_config.arp, client_config.interface) == 0
#else
								arpping(server_addr, requested_ip, client_config.arp, client_config.interface) == 0
#endif
							){//arpping ok
								arpping_result = 1;
							}
							else{//arpping timeout
								arpping_result = 0; 					
							}
						}					
#endif
					}
					if(1 != arpping_result){
#if defined(TCSUPPORT_CT_FJ)
						if(arpping_renew_num < 3){
							send_renew(xid, 0, requested_ip); /* broadcast */	
							timeout = uptime() + 2;   //now+2
							arpping_renew_num++;
						}
						else
#endif
						{
							arpping_result = 1;
							arpping_renew_num = 0;
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
							if ( client_config.ipoe_if )
							{
								run_script(NULL, "renewfail");
								return 0;
							}
#endif
							/* timed out, enter init state */
							state = INIT_SELECTING;
							LOG(LOG_INFO, "Lease lost, entering init state");
							run_script(NULL, "deconfig");
							timeout = now;
							packet_num = 0;
							change_mode(LISTEN_RAW);
						}
					}
					else{/* send a request packet */
						if(0 != TimeOutChangedFlag){
							//do nothing
						}
						else{
							send_renew(xid, server_addr, requested_ip); /* unicast */
							t1 = (t2 - t1) / 2 + t1;
							timeout = t1 + start;
						}
					}
					arpflag = 0;
#else
					/* send a request packet */
					send_renew(xid, server_addr, requested_ip); /* unicast */

					t1 = (t2 - t1) / 2 + t1;
					timeout = t1 + start;
#endif
				}
				break;
			case REBINDING:
				/* Either set a new T2, or enter INIT state */
#if defined(TCSUPPORT_CT_FJ)
				if ((lease - t2) <= (lease / 14400 + 1) || (lease - t2) < (0.0625*lease - 1)) {
#else
				if ((lease - t2) <= (lease / 14400 + 1)) {
#endif
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
					if ( client_config.ipoe_if )
					{
						run_script(NULL, "rebindfail");
						return 0;
					}
#endif
					/* timed out, enter init state */
					state = INIT_SELECTING;
					LOG(LOG_INFO, "Lease lost, entering init state");
					run_script(NULL, "deconfig");
					timeout = now;
					packet_num = 0;
					change_mode(LISTEN_RAW);
				} else {
					/* send a request packet */
					send_renew(xid, 0, requested_ip); /* broadcast */

					t2 = (lease - t2) / 2 + t2;
					timeout = t2 + start;
				}
				break;
			case RELEASED:
				/* yah, I know, *you* say it would never happen */
				timeout = 0x7fffffff;
				break;
			}
		} else if (retval > 0 && listen_mode != LISTEN_NONE && FD_ISSET(fd, &rfds)) {
			/* a packet is ready, read it */

			if (listen_mode == LISTEN_KERNEL)
				len = get_packet(&packet, fd);		
			else len = get_raw_packet(&packet, fd);

			if (len == -1 && errno != EINTR) {
				DEBUG(LOG_INFO, "error on read, %m, reopening socket");
				change_mode(listen_mode); /* just close and reopen */
			}
			if (len < 0) continue;

			if (packet.xid != xid) {
				DEBUG(LOG_INFO, "Ignoring XID %lx (our xid is %lx)",
					(unsigned long) packet.xid, xid);
				continue;
			}

			if ((message = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldnt get option from packet -- ignoring");
				continue;
			}

			switch (state) {
			case INIT_SELECTING:
				/* Must be a DHCPOFFER to one of our xid's */
				if (*message == DHCPOFFER) {
#if defined(TCSUPPORT_CT_DHCPC_OPTION125)
					temp = get_option(&packet, OPT_VENDOR_INFOR);
					if(NULL == temp)
					{
						LOG(LOG_ERR, "\nDHCP option125 field does not exist!\n");
					}
					if(0 == match_option125_with_ITMS(temp,&client_config)){
						tcdbg_printf("\r\nmatch_option125_with_ITMS fail!");
						break;
					}
#endif
					if ((temp = get_option(&packet, DHCP_SERVER_ID))) {
						memcpy(&server_addr, temp, 4);
						xid = packet.xid;
						requested_ip = packet.yiaddr;
#if defined(TCSUPPORT_CT_FJ)
						if ((temp = get_option(&packet, DHCP_ROUTER))) {
							memcpy(&router_addr, temp, 4);
						} else {
							router_addr = server_addr ; //if needed???
						    DEBUG(LOG_ERR, "No router ip in message");
					    }
#endif
						/* enter requesting state */
						state = REQUESTING;
						timeout = now;
						packet_num = 0;
					} else {
						DEBUG(LOG_ERR, "No server ID in message");
					}
				}
				break;
			case RENEW_REQUESTED:
			case REQUESTING:
			case RENEWING:
			case REBINDING:
#if defined(TCSUPPORT_CT_DHCP_FORCERENEW)
				if (*message == DHCPFORCERENEW) {
					state = RENEWING;
					timeout = now;
					continue;
				}
#endif
				if (*message == DHCPACK) {
#if 0
					if(client_config.dhcp125c)
					{
						option125ptr = get_option(&packet, OPT_VENDOR_INFOR);
						if(NULL == option125ptr)
						{
							LOG(LOG_ERR, "\nDHCP option125 check error: the field does not exist!\n");
							break;
						}
						else if(*((unsigned int *)option125ptr) != client_config.dhcp125Code)
						{
							LOG(LOG_ERR, "\nDHCP option125 check error: the field CODE does not match!\n");
							break;
						}
						else if(client_config.dhcp125s && 0 != strncmp( client_config.dhcp125String, option125ptr+5, option125ptr[4] ) )
						{
							LOG(LOG_ERR, "\nDHCP option125 check error: the field STRING does not match!\n");
							break;
						}
					}
#endif								
					if (!(temp = get_option(&packet, DHCP_LEASE_TIME))) {
						LOG(LOG_ERR, "No lease time with ACK, using 1 hour lease");
						lease = 60 * 60;
					} else {
						memcpy(&lease, temp, 4);
						lease = ntohl(lease);
					}

					/* enter bound state */
					t1 = lease / 2;

					/* little fixed point for n * .875 */
					t2 = (lease * 0x7) >> 3;
					temp_addr.s_addr = packet.yiaddr;
					LOG(LOG_INFO, "Lease of %s obtained, lease time %ld",
						inet_ntoa(temp_addr), lease);
					start = now;
					timeout = t1 + start;
#if defined(TCSUPPORT_CT_FJ) || defined(TCSUPPORT_CT_DHCP_ARPPING)
					arpping_result = 1;
					arpping_renew_num = 0;
#endif
					requested_ip = packet.yiaddr;
#if defined(TCSUPPORT_CT)
					LOG(LOG_INFO, "before run script, state = %d", state);
					if(state != RENEWING){
#endif
					run_script(&packet,
						   ((state == RENEWING || state == REBINDING) ? "renew" : "bound"));
#if defined(TCSUPPORT_CT)
					LOG(LOG_INFO, "run script end");
					}
#endif
					#if defined(DHCP_OPT121)
					if(!tcapi_get("Dhcpd_Option121", "Active", tmp))
						enable121 = atoi(tmp);
					if(enable121)
					{
						if (temp = get_option(&packet, OPT_CLASSLESS_STATIC_ROUTE)) {
							if(*(temp - OPT_DATA + OPT_LEN) > 4) //its minimum length is 5
								dhcpExtractClasslessRoute(temp, *(temp - OPT_DATA + OPT_LEN));
						}
					}
					#endif

					#if defined(TCSUPPORT_TR069_OPT43)
						/* get acs url and provisioningCode from dhcp option 43, 
						   then send msg to cwmp to send inform */
						if (temp = get_option(&packet, OPT_VENDOR_SPECIFIC_INFOR)) {
							if (*(temp - OPT_DATA + OPT_LEN) > 4)
								dhcpExtractOption43(temp, *(temp - OPT_DATA + OPT_LEN));
						}
					#ifdef OPT43_DBG
						else {
							tcdbg_printf("\n%s: get option43 failed.\n", __FUNCTION__);
						}
					#endif
					#endif

					state = BOUND;
					change_mode(LISTEN_NONE);
					if (client_config.quit_after_lease)
						return 0;
					if (!client_config.foreground)
						client_background();

				} else if (*message == DHCPNAK) {
					/* return to init state */
					LOG(LOG_INFO, "Received DHCP NAK");
					run_script(&packet, "nak");
#if defined(TCSUPPORT_CT_IPOE_EMULATOR)
					if ( client_config.ipoe_if )
						return 0;
#endif
					if (state != REQUESTING)
						run_script(NULL, "deconfig");
					state = INIT_SELECTING;
					timeout = now;
					requested_ip = 0;
					packet_num = 0;
					change_mode(LISTEN_RAW);
					sleep(3); /* avoid excessive network traffic */
				}
				break;
			/* case BOUND, RELEASED: - ignore all packets */
			}
		} else if (retval > 0 && (sig = udhcp_sp_read(&rfds))) {
			switch (sig) {
			case SIGUSR1:
				perform_renew();
				break;
			case SIGUSR2:
				perform_release();
				break;
			case SIGTERM:
				LOG(LOG_INFO, "Received SIGTERM");
				/*send release shnwind 2008.4.17*/
                                perform_release();
				return 0;
			}
		} else if (retval == -1 && errno == EINTR) {
			/* a signal was caught */
		} else {
			/* An error occured */
			DEBUG(LOG_ERR, "Error on select");
		}

	}
	return 0;
}
