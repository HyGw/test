#include "cfg_manager.h"
#include "wan.h"
#include "wan_monitor.h"
#include "utility.h"

#if defined(TCSUPPORT_CT_JOYME) 
#include "libtcapi.h"
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
#include <netinet/in.h>
#endif

#if defined(TCSUPPORT_CT_PON)
#include "xpon.h"
#endif

#ifdef VP_MEGACO
#undef TCSUPPORT_VOIP_IMSSIP
#include "sp_ipc_msg.h"
#include "pmegaco_support.h"
#else
#ifdef TCSUPPORT_VOIP_IMSSIP
#include "voip_data_util.h"
#endif
#endif

#if defined(TCSUPPORT_CT_HWQOS)
#define QOS 					"QoS"
#endif

#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
#include <pthread.h>
#endif

#if defined(TCSUPPORT_CT_INFORM_NODE)
#define SRV_TR069_FIND		1
#define SRV_TR069_NO_FIND	0
#endif

#if defined(TCSUPPORT_CT_WAN_PTM)	
int g_dsl_state = ADSL_STATE_DOWN;      //last time dsl up state
#endif

struct if_info g_wan_if[WAN_IF_INFO_NUM];
int g_adsl_state = ADSL_STATE_DOWN;
int g_dhcprelay = 0;
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
pthread_mutex_t dnsmasq_restart_mutex;
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
extern int recheckPingDiagnostic_flag;
#endif
#if defined(TCSUPPORT_CT)
unsigned int ntpdelayflag1 =0;
unsigned int ntpdelayflag2 =0;
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
unsigned int ntpdelayflag1_all[4] ={0,0,0,0};
unsigned int ntpdelayflag2_all[4] ={0,0,0,0};			
#endif
#endif
extern char ntpneedexecute;
#endif
extern int VOIPLineNum;
#if defined(TCSUPPORT_CT_QOS)
extern int is_qos_boot;
#endif


extern void restart_nat(char *wan_if_name);
extern void restart_ddns();
#if defined(TCSUPPORT_SIMPLE_UPNP)
//extern void start_upnp(char *ppp_interface);
extern void restart_upnp(mxml_node_t *top, char *if_name, int if_index);
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
extern int isOndemandWan(mxml_node_t *top, int pvc_index, int entry_index);
extern int delRoute6ViaOndemand(mxml_node_t *top, int pvc_index, int entry_index, char *ifname);
extern int addDnsRoute6forOtherWan(char *ifname, char *ifsvrapp, char *hisaddrV6);
#endif
#if defined(TCSUPPORT_CT_FJ)
int g_OltRegStartTime = 0;
#endif

#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
/*init mutex dnsmasq_restart_mutex*/
int init_dnsmasq_mutex(void){
	int ret = -1;
	ret = pthread_mutex_init(&dnsmasq_restart_mutex,NULL);
	if(ret != 0)
	{
		tcdbg_printf("\r\nDnsmasq_restart_mutex initialization fail!");
		return -1;
	}
	return 0;
}
#endif

#ifdef IPV6
/*_____________________________________________________________________________
**      function name: restart_pd_function
**      descriptions:
**           restart prefix delegation function,eg:dhcp6s and radvd for LAN, when wan get prefix
**
**      parameters:
**           type : 0:dhcp6s, 1:radvd, 2:all
**
**      global:
**             None
**
**      return:
**            None
**
**      call:
**
**      revision:
**      1. jlliu 20100830
**____________________________________________________________________________
*/
void restart_pd_function(int type)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char en[5] = {0}, mode[5] = {0};//, def_route_index_v6[5] = {0};
	mxml_node_t *top = get_node_top();	
#if 0
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"WanInfo");
  	strcpy(nodeName[1], "Common");
	if(getAttrValue(top, nodeName, "DefRouteIndexv6", def_route_index_v6) != TCAPI_PROCESS_OK)
		return;
	/*is N/A*/
	if(!strcmp(def_route_index_v6, "N/A"))
		return;
	
	/*not default route interface*/
	if(atoi(def_route_index_v6) != if_index)
		return;
#endif	
	if ( 0 == type || 2 == type )
	{
	/*Set dhcp6s prefix value*/
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"Dhcp6s");
  	strcpy(nodeName[1], "Entry");

	if(getAttrValue(top, nodeName, "Enable", en) == TCAPI_PROCESS_OK &&
		getAttrValue(top, nodeName, "Mode", mode) == TCAPI_PROCESS_OK){
		/*Dhcp6s is enabled and auto mode*/
		if( ( '1' == en[0] ) && ( '0' == mode[0] || '2' == mode[0]) )			
		{		
			tcapi_commit_internal(top, "Dhcp6s");
		}		
	}
	}

	if ( 1 == type || 2 == type )
	{
#if 1 //defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)				
		/*Set Radvd prefix value*/
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0],"Radvd");
		strcpy(nodeName[1], "Entry");
		
		if(getAttrValue(top, nodeName, "Enable", en) == TCAPI_PROCESS_OK &&
			getAttrValue(top, nodeName, "Mode", mode) == TCAPI_PROCESS_OK){
			/*Radvd is enabled and auto mode*/
			if( ( '1' == en[0] ) && ( '0' == mode[0])){		
				tcapi_commit_internal(top, "Radvd");
			}		
		}

#endif
	}
	return;
}
int get_def_route_index(void)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char def_route_index_v6[5] = {0};
	mxml_node_t *top = get_node_top();	
	
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"WanInfo");
  	strcpy(nodeName[1], "Common");
	if(getAttrValue(top, nodeName, "DefRouteIndexv6", def_route_index_v6) != TCAPI_PROCESS_OK)
		return -2;
	/*is N/A*/
	if(!strcmp(def_route_index_v6, "N/A"))
		return -1;
	
	return atoi(def_route_index_v6);
}

int check_pd_enable(int ifindex)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char pd_index[6] = {0};
	char str_active[8] = {0};
	char str_wanmode[8] = {0};
	char str_servicelist[32] = {0};
	char str_pdenable[8] = {0};
	char str_internet_pd[8] = {0};
	char str_other_pd[8] = {0};
	int pvc_index = 0, entry_index = 0, if_index = 0;
	mxml_node_t *top = get_node_top();

	pvc_index = ifindex / MAX_SMUX_NUM;
	entry_index = ifindex % MAX_SMUX_NUM;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active", str_active) )
		strcpy(str_active, "N/A");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "WanMode", str_wanmode) )
		strcpy(str_wanmode, "Route");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ServiceList", str_servicelist) )
		strcpy(str_servicelist, "INTERNET");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDEnable", str_pdenable) )
		strcpy(str_pdenable, "N/A");

	if ( 0 == strcmp(str_active, "Yes")
		&& 0 == strcmp(str_wanmode, "Route")
		&& 0 == strcmp(str_pdenable, "Yes") )
	{
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],"WanInfo");
	  	strcpy(nodeName[1], "Common");

		if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDINTERNETIFIdx", str_internet_pd) )
			strcpy(str_internet_pd, "N/A");
		if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDOTHERIFIdx", str_other_pd) )
			strcpy(str_other_pd, "N/A");

		if( NULL != strstr(str_servicelist, "INTERNET") )
		{
			if ( 0 == strcmp(str_internet_pd, "N/A") )
			{
				sprintf(pd_index, "%d", ifindex);
				setAttrValue(top, nodeName, "PDINTERNETIFIdx", pd_index);
				setAttrValue(top, nodeName, "PDRUNIFIdx", pd_index);
				return 1;
			}
		}
		else if ( NULL != strstr(str_servicelist, "OTHER") )
		{
			if ( 0 == strcmp(str_other_pd, "N/A")
				&&  0 == strcmp(str_internet_pd, "N/A") )
			{
				sprintf(pd_index, "%d", ifindex);
				setAttrValue(top, nodeName, "PDOTHERIFIdx", pd_index);
				setAttrValue(top, nodeName, "PDRUNIFIdx", pd_index);
				return 1;
			}
		}
	}

	return 0;
}

int get_run_pd_index()
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char pd_index[6] = {0};
	mxml_node_t *top = get_node_top();	
	
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"WanInfo");
  	strcpy(nodeName[1], "Common");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDRUNIFIdx", pd_index) )
		return -1;
	if( 0 == strcmp(pd_index, "N/A") )
		return -2;

	return atoi(pd_index);
}

int get_pd_index(int ifindex)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char pd_index[6] = {0};
	int valid_if[MAX_WAN_IF_INDEX] = {0}, valid_if_num = 0, i = 0;
	int pvc_index = 0, entry_index = 0, if_index = 0;
	char str_servicelist[32] = {0}, str_wanmode[8] = {0}, str_pdenable[8];
	char str_active[8] = {0};
	char str_status6[8] = {0};
	char str_pd6[64] = {0};
	int other_if = -1;
	int internet_if = -1;
	mxml_node_t *top = get_node_top();	

	valid_if_num = get_all_wan_index(valid_if, MAX_WAN_IF_INDEX);
	for(i = 0; i < valid_if_num; i++)
	{
		if ( ifindex == valid_if[i] )
			continue;
		pvc_index = valid_if[i] / MAX_SMUX_NUM;
		entry_index = valid_if[i] % MAX_SMUX_NUM;

		if ( -1 == internet_if )
		{
			memset(nodeName, 0, sizeof(nodeName));
			strcpy(nodeName[0], "Wan");
			sprintf(nodeName[1], "%s%d", "PVC", pvc_index);
			sprintf(nodeName[2], "%s%d", "Entry", entry_index);
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active", str_active) )
				strcpy(str_active, "N/A");
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "WanMode", str_wanmode) )
				strcpy(str_wanmode, "Route");
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ServiceList", str_servicelist) )
				strcpy(str_servicelist, "INTERNET");
			if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDEnable", str_pdenable) )
			{
				if( NULL != strstr(str_servicelist, "INTERNET") )
					strcpy(str_pdenable, "Yes");
				else
					strcpy(str_pdenable, "N/A");
			}

			if ( 0 == strcmp(str_active, "Yes")
				&& 0 == strcmp(str_wanmode, "Route")
				&& 0 == strcmp(str_pdenable, "Yes") )
			{
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0], "WanInfo");
				sprintf(nodeName[1], "Entry%d", valid_if[i]);
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Status6", str_status6) )
					strcpy(str_status6, "N/A");
				if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PD6", str_pd6) )
					strcpy(str_pd6, "N/A");

				if( NULL != strstr(str_servicelist, "INTERNET") )
				{
					if( ('\0' != str_pd6[0] && 0 != strcmp(str_pd6, "N/A")) && 0 == strcmp(str_status6, "up") )
						internet_if = valid_if[i];
				}
				else if ( (-1 == other_if) && NULL != strstr(str_servicelist, "OTHER") )
				{
					if( ('\0' != str_pd6[0] && 0 != strcmp(str_pd6, "N/A")) && 0 == strcmp(str_status6, "up") )
						other_if = valid_if[i];
				}
			}
		}

	}

	if ( -1 == internet_if )
		internet_if = other_if;

	return ( -1 != internet_if ) ? internet_if : -1;
}

void update_run_pd_index(int ifindex)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	mxml_node_t *top = get_node_top();	
	int pd_ifindex = get_pd_index(ifindex);

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "Common");
	setAttrValue(top, nodeName, "PDINTERNETIFIdx", "N/A");
	setAttrValue(top, nodeName, "PDOTHERIFIdx", "N/A");
	setAttrValue(top, nodeName, "PDRUNIFIdx", "N/A");

	if( -1 == pd_ifindex )
		return;

	check_pd_enable(pd_ifindex);
}

int getRaWanPDInterface()
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char val_buf[8] = {0};
	char active[8] = {0};
	int pdIdx = 0;
	int pvc_idx = 0, entry_idx = 0;
	mxml_node_t *top = get_node_top();	
	
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"Radvd");
  	strcpy(nodeName[1], "Entry");

	if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "AutoPrefix", val_buf) )
		return get_run_pd_index();

	if ( 0 == atoi(val_buf) )
	{
		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DelegatedWanConnection", val_buf) )
			return get_run_pd_index();
		else
		{
			if ( '\0' != val_buf[0] )
			{
				pdIdx = atoi(val_buf);
				pvc_idx = pdIdx / MAX_SMUX_NUM;
				entry_idx = pdIdx % MAX_SMUX_NUM;
				memset(nodeName, 0, sizeof(nodeName));
				strcpy(nodeName[0],"Wan");
				sprintf(nodeName[1], "PVC%d", pvc_idx);
				sprintf(nodeName[2], "Entry%d", entry_idx);
				if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active", active) )
				{
					memset(nodeName, 0, sizeof(nodeName));
					strcpy(nodeName[0], "Radvd");
					strcpy(nodeName[1], "Entry");
					setAttrValue(top, nodeName, "DelegatedWanConnection", "");
					return get_run_pd_index();
				}
			}
			else
				return get_run_pd_index();
		}
	}
	else
		return -1;

	return atoi(val_buf);
}

int getDhcp6sDNSWanInterface()
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char val_buf[8] = {0};
	mxml_node_t *top = get_node_top();	
	
	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"Dhcp6s");
  	strcpy(nodeName[1], "Entry");

	if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DNSType", val_buf) )
		return -1;

	if ( 0 == atoi(val_buf) )
	{
		if ( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DNSWANConnection", val_buf) )
			return -1;
	}
	else
		return -1;

	return atoi(val_buf);

}

extern int create_br0_v6addr(char *pd, int pd_len, char *newV6addr);
int update_br0_pdaddress(int ifindex, int if_state)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0}; 
	char str_active[8] = {0};
	char str_wanmode[8] = {0};
	char ip_version[20] = {0};
	char dhcpv6_getpd[8] = {0};
	char dhcpv6_pdorig[8] = {0};
	char str_servicelist[32] = {0}; 
	char ipv6_br0[64]= {0};
	char dhcpv6_wanpd[64]= {0};
	char cmd[128] = {0};
	char dhcpv6_wanpd_prefix[64] = {0}, dhcpv6_wanpd_prefix_len[12] = {0};
	char v6addr_str[64] = {0};
	char *pos = NULL;
	int pvc_index = 0, entry_index = 0, if_index = 0;
	mxml_node_t *top = get_node_top();

	pvc_index = ifindex / MAX_SMUX_NUM;
	entry_index = ifindex % MAX_SMUX_NUM;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	sprintf(nodeName[1], "%s%d", WAN_PVC, pvc_index);
	sprintf(nodeName[2], "%s%d", WAN_INTERFACE, entry_index);
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "Active", str_active) )
		strcpy(str_active, "N/A");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "WanMode", str_wanmode) )
		strcpy(str_wanmode, "Route");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "ServiceList", str_servicelist) )
		strcpy(str_servicelist, "INTERNET");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "IPVERSION", ip_version) )
		strcpy(ip_version, "IPv4");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "DHCPv6PD", dhcpv6_getpd) )
		strcpy(dhcpv6_getpd, "N/A");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDOrigin", dhcpv6_pdorig) )
		strcpy(dhcpv6_pdorig, "N/A");
	if( TCAPI_PROCESS_OK != getAttrValue(top, nodeName, "PDPrefix", dhcpv6_wanpd) )
		dhcpv6_wanpd[0] = '\0';

	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, BR0_ADDR, ipv6_br0)
		&& 0 != strcmp(ipv6_br0, "N/A") )
	{
		sprintf(cmd, "ifconfig br0 del %s", ipv6_br0);
		system(cmd);
		setAttrValue(top, nodeName, BR0_ADDR, "N/A");
	}

	if ( 0 == strcmp(str_active, "Yes")
		&& 0 == strcmp(str_wanmode, "Route")
		&& 0 != strcmp(ip_version, "IPv4") // not ipv4 mode
		&& 0 == strcmp("No", dhcpv6_getpd)
		&& 0 == strcmp("Static", dhcpv6_pdorig) // static PD from wan interface
		&& (NULL != strstr(str_servicelist, "INTERNET") || NULL != strstr(str_servicelist, "OTHER")) )
	{
		if ( IF_STATE_UP6 == if_state )
		{
			pos = strtok(dhcpv6_wanpd, "/");
			if ( pos )
			{
				snprintf(dhcpv6_wanpd_prefix, sizeof(dhcpv6_wanpd_prefix) - 1, "%s", pos);
				pos = strtok(NULL, "/");
				if ( pos )
					snprintf(dhcpv6_wanpd_prefix_len, sizeof(dhcpv6_wanpd_prefix_len) - 1, "%s", pos);
			}

			if ( '\0' != dhcpv6_wanpd_prefix[0]
				&& '\0' != dhcpv6_wanpd_prefix_len[0] )
			{
				if ( 0 != create_br0_v6addr(dhcpv6_wanpd_prefix, atoi(dhcpv6_wanpd_prefix_len), v6addr_str) )
				{
					tcdbg_printf("error:update_br0_pdaddress() fail!\n");
					return -1;
				}

				memset(ipv6_br0, 0, sizeof(ipv6_br0));
				snprintf(ipv6_br0, sizeof(ipv6_br0) - 1, "%s/%s", v6addr_str, dhcpv6_wanpd_prefix_len);
				sprintf(cmd, "ifconfig br0 %s", ipv6_br0);
				system(cmd);
				// save br0 address
				setAttrValue(top, nodeName, BR0_ADDR, ipv6_br0);
			}
		}
	}

	return 0;
}

int isWan6UP(int iface_index)
{
	if ( iface_index < 0 || iface_index >= MAX_WAN_IF_INDEX )
		return 0;

	if ( IF_STATE_UP6 == g_wan_if[iface_index].status6 )
		return 1;
	else
		return 0;
}
#endif

/*______________________________________________________________________________
**function name:check_route
**
**description:
*    check the dev in route entry is correct or not..
**parameters:
*    if_name: wan interface name
**global:
*    none
**return:
*    none
**call:
*    none
**revision:
*     1.shnwind 20080526
*	2.jlliu 20100902
**____________________________________________________________________________*/
void
check_route(char *if_name)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i;
	mxml_node_t *curNode = NULL;
	const char *Value=NULL;
	mxml_node_t *top = get_node_top();
	char route_tmp[16]={0}, string[128]={0},tmp[32]={0};
	char route_active[8] = {0};

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE);
	for(i = 0; i < MAX_STATIC_ROUTE_NUM; i++){
		sprintf(nodeName[1], "Entry%d", i);

		if((curNode = search_node(nodeName)) != NULL){
			Value = mxmlElementGetAttr(curNode, "Device");
			
			/*No necessary to reset static route if it is not this wan interface*/
			if(Value && strcmp(if_name, Value))
				continue;
			memset(string, 0, sizeof(string));
			strcpy(string, "/sbin/route add ");

			getAttrValue(top, nodeName, "Active", route_active);
			if ( 0 != strcmp(route_active, "Yes") )
			{
				strcpy(string, "#/sbin/route add "); /* mark add operation. */
			}

			if(getAttrValue(top, nodeName, "DST_IP", route_tmp)== TCAPI_PROCESS_OK){
				sprintf(tmp,"-net %s ",route_tmp);
				strcat(string, tmp);
			}else{			
				continue;
			}
			/*write sub mask*/
			if(getAttrValue(top, nodeName, "Sub_mask", route_tmp)== TCAPI_PROCESS_OK){
				sprintf(tmp,"netmask %s ",route_tmp);
				strcat(string, tmp);
		        }
			/*write device info*/
			if(getAttrValue(top, nodeName, "Device", route_tmp) == TCAPI_PROCESS_OK){
				sprintf(tmp,"dev %s ",route_tmp);
				strcat(string, tmp);
			}
			/*write gateway info*/
			if(getAttrValue(top, nodeName, "Gateway", route_tmp) == TCAPI_PROCESS_OK){
				if(strcmp(route_tmp,"0.0.0.0")){
					sprintf(tmp,"gw %s ",route_tmp);
					strcat(string, tmp);
				}
			}
			/*write metric value*/
			if(getAttrValue(top, nodeName, "metric", route_tmp) == TCAPI_PROCESS_OK){
				if(atoi(route_tmp) > 0){
					sprintf(tmp,"metric %s ",route_tmp);
					strcat(string, tmp);
				}
			}
			//tcdbg_printf("%s: cmd:%s\n",__func__, string);
			system(string);
		}
	}
	//system(ROUTE_PATH);
	return;
}

#ifdef IPV6
extern int route6_cmd(int action, char route6_para[][MAX_ROUTE6_PARA_LEN]);
/*______________________________________________________________________________
**function name:check_static_route6
**
**description:
*    reset ipv6 static route rule based on interface name
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    interface_state_change
**revision:
**____________________________________________________________________________*/
void check_static_route6(char *if_name, int if_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char route6_para[4][MAX_ROUTE6_PARA_LEN];
	int i = 0;
	mxml_node_t *curNode = NULL;
	const char *Value=NULL;
	mxml_node_t *top = get_node_top();

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],ROUTE6);
	for(i = 0; i < MAX_STATIC_ROUTE6_NUM; i++){
		sprintf(nodeName[1], "Entry%d", i);
		if((curNode = search_node(nodeName)) != NULL){
			Value = mxmlElementGetAttr(curNode, "Device");

			/*No necessary to reset static route if it is not this wan interface*/
			if(Value && strcmp(if_name, Value))
				continue;
			
			memset(route6_para, 0, sizeof(route6_para));
			
			getAttrValue(top, nodeName, "DST_IP", route6_para[DST_IP_TYPE]);
			getAttrValue(top, nodeName, "Prefix_len", route6_para[PREFIX_LEN_TYPE]);
			getAttrValue(top, nodeName, "Gateway", route6_para[GATEWAY_TYPE]);
			getAttrValue(top, nodeName, "Device", route6_para[DEVICE_TYPE]);

			route6_cmd(ROUTE6_ADD, route6_para);
		}
	}

	return;
}
#endif

#ifdef TCSUPPORT_DNSEACHPVC
#if defined(TCSUPPORT_CT_FULL_ROUTE)
unsigned int get_default_pvc_DNSOrder(void){

	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME] = {0};
	char default_routeV4[16]={0};
	char default_routeV6[16]={0};
	mxml_node_t *top = get_node_top();
	unsigned int defaut_route_id = 0xFFFFFFFF;
	int tempId = -1;

	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], "WanInfo");
	strcpy(nodeName[1], "Common");

	if(getAttrValue(top, nodeName, "DefRouteIndexv4", default_routeV4) != SUCCESS){
		goto checkv6;
	}
	if(strlen(default_routeV4) == 0 || !strncmp(default_routeV4, "N/A", 3)) {
		goto checkv6;
	}
	defaut_route_id &= ~0xFFFF;
	defaut_route_id |= atoi(default_routeV4);

checkv6:
	if(getAttrValue(top, nodeName, "DefRouteIndexv6", default_routeV6) != SUCCESS){
		goto out;
	}
	if(strlen(default_routeV6) == 0 || !strncmp(default_routeV6, "N/A", 3)) {
		goto out;
	}

	tempId = atoi(default_routeV6);
	defaut_route_id &= ~0xFFFF0000;
	defaut_route_id |= tempId << 16;
	
out:	
	return defaut_route_id;
}
#endif
#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
extern int addEMUDNSConfig(FILE* fp, FILE* fpInfo);
#endif
void restart_dnsmasq()
{
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)	
	pthread_mutex_lock(&dnsmasq_restart_mutex);
#endif
	FILE* fp = NULL;
	FILE* fpInfo = NULL;
	char buf[MAXSIZE] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char dns[20] = {0}, secdns[20] = {0}, ifname[16] = {0}, status[6] = {0}, serviceapp[SERVICE_LIST_LEN] = {0}, isp[3] = {0};
	mxml_node_t *top = get_node_top();
	#ifdef IPV6
	char dns6[40] = {0}, secdns6[40] = {0}, status6[6] = {0};
	#endif
	int i;
#if defined(TCSUPPORT_CT_FULL_ROUTE)
	unsigned int default_pvc = 0;
	unsigned short tempV4 = 0;
	unsigned short tempV6 = 0;
	int flag = 0;
	int fullroutesw = 0;
#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND) || defined(TCSUPPORT_CT_ADV_DNSPROXY)
	char ppphisaddr[64] = {0};
	char ip_version[20] = {0};
	int ipVersion = 0;
#endif
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	char query_delay[64] = {0};
#endif

	//tcdbg_printf("\n %s:enter..", __func__);
	
	/* step1: down dnsmasq process */
	system("killall -9 dnsmasq");

	/* step2: write dnsmasq.conf and dnsInfo.conf */
	/* dnsmasq.conf format is:
	** strict-order
	** no-resolv
	** 168.95.1.1@nas0_1
	** 168.95.1.2@ppp1
	** 2001::fe@nas0_1 //for IPv6

	** dnsInfo.conf format is:
	** 168.95.1.1 nas0 Internet
	** 168.95.1.2 ppp1 TR69
	
	** get wan information from WanInfo node
	*/
	fp = fopen("/etc/dnsmasq.conf", "w");
	if(fp == NULL){
		tcdbg_printf("\n %s:open dnsmasq.conf failed", __func__);
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
		pthread_mutex_unlock(&dnsmasq_restart_mutex);
#endif
		return;
	}

	fpInfo = fopen("/etc/dnsInfo.conf", "w");
	if(fpInfo == NULL){
		fclose(fp);
		tcdbg_printf("\n %s: open dnsInfo.conf failed", __func__);
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
		pthread_mutex_unlock(&dnsmasq_restart_mutex);
#endif
		return;
	}
	
	//sprintf(buf, "strict-order\n");
	//fwrite(buf, sizeof(char), strlen(buf), fp);
	sprintf(buf, "no-resolv\n");
	fwrite(buf, sizeof(char), strlen(buf), fp);
#if defined(TCSUPPORT_CT_FULL_ROUTE)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	strcpy(nodeName[1], WAN_COMMON);

	if(getAttrValue(top, nodeName, "IPForwardModeEnable", status) != TCAPI_PROCESS_OK)
		return 0;
	
	if(0 == strncmp(status, "Yes", 3))
	{
		fullroutesw = 1;
	}
	
	if(fullroutesw == 1){
		default_pvc = get_default_pvc_DNSOrder();
		tempV4 = (unsigned short)default_pvc;
		tempV6 = (unsigned short)(default_pvc >> 16);
	}
#endif
	
	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
#if defined(TCSUPPORT_CT_FULL_ROUTE)
		//set default dns first, if flag==2, this means all dns was written finished.
		if(fullroutesw == 1){
			if(flag == 0){//dns v4
				if(tempV4 !=0xFFFF){
					i = tempV4;
				}
				else
					flag = 1;//v4 pass
				if(tempV4 == tempV6)
					flag = 1;
			}

			if(flag == 1){//dns v6
				if(tempV6 !=0xFFFF){
					i = tempV6;
				}
				else{
					flag = 2;//v6 pass
					i = -1;
					continue;
				}
			}
		}
#endif
		memset(ifname, 0, sizeof(ifname));
		memset(serviceapp, 0, sizeof(serviceapp));
		/*Get interface name and check wan interface exited*/
		if(get_waninfo_by_index(i, "IFName", ifname) != SUCCESS ||
			get_waninfo_by_index(i, "ISP", isp) != SUCCESS)
			continue;

		/*skip bridge mode interface*/
		if(!strcmp(isp, BRIDGE_MODE)){
				continue;
		}
		
		if(get_waninfo_by_index(i, "ServiceList", serviceapp) != SUCCESS){
			/*Use others as default service type*/
			strcpy(serviceapp, "OTHER");
		}
		
#if defined(TCSUPPORT_CT_PPP_ONDEMAND) || defined(TCSUPPORT_CT_ADV_DNSPROXY)
		if( SUCCESS == get_waninfo_by_index(i, "IPVERSION", ip_version) )
		{
			if ( 0 == strcmp(ip_version, "IPv4") )
				ipVersion = 0;
			else if ( 0 == strcmp(ip_version, "IPv6") )
				ipVersion = 1;
			else if ( 0 == strcmp(ip_version, "IPv4/IPv6") )
				ipVersion = 2;
		}
#endif
		
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],"WanInfo");
	  	sprintf(nodeName[1], "Entry%d", i);
#if defined(TCSUPPORT_CT_FULL_ROUTE)
		if(fullroutesw == 1){
			if(flag == 2 && i == tempV4){//next time,if it's default DNS no need to write again
				goto v6;
			}

			if(flag == 1 && tempV4 != tempV6)//v6 dns set , if v4==v6, we need write v4&v6 dns to file one time.
				goto v6;
		}
#endif

		if(getAttrValue(top, nodeName, "Status", status) == TCAPI_PROCESS_OK && !strcmp(status, "up")){
			memset(dns, 0, sizeof(dns));
			if(getAttrValue(top, nodeName, "DNS", dns) == TCAPI_PROCESS_OK && strlen(dns) != 0 && isValidDnsIp(dns)){
				memset(buf, 0, sizeof(buf));
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
				sprintf(buf, "server=%s@%s%s\n", dns, ifname, ((2 == ipVersion) ? "$ds" : ""));
#else
				sprintf(buf, "server=%s@%s\n", dns, ifname);			
#endif
				fwrite(buf, sizeof(char), strlen(buf), fp);

				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%s %s %s\n", dns, ifname, serviceapp);
				fwrite(buf, sizeof(char), strlen(buf), fpInfo);
			}
			memset(secdns, 0, sizeof(secdns));
			if(getAttrValue(top, nodeName, "SecDNS", secdns) == TCAPI_PROCESS_OK && strlen(secdns) != 0 && isValidDnsIp(secdns)){
				memset(buf, 0, sizeof(buf));
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
				sprintf(buf, "server=%s@%s%s\n", secdns, ifname, ((2 == ipVersion) ? "$ds" : ""));
#else
				sprintf(buf, "server=%s@%s\n", secdns, ifname);			
#endif
				fwrite(buf, sizeof(char), strlen(buf), fp);

				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%s %s %s\n", secdns, ifname, serviceapp);
				fwrite(buf, sizeof(char), strlen(buf), fpInfo);
			}
		}
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
		else if ( 1 == isOndemandWan(top, i/MAX_SMUX_NUM, i%MAX_SMUX_NUM) )
		{
			if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "OndemandHisAddr", ppphisaddr)
				&& '\0' != ppphisaddr[0] )
			{
				memset(buf, 0, sizeof(buf));
				sprintf(buf, "server=%s@%s\n", ppphisaddr, ifname);			
				fwrite(buf, sizeof(char), strlen(buf), fp);

				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%s %s %s\n", ppphisaddr, ifname, serviceapp);
				fwrite(buf, sizeof(char), strlen(buf), fpInfo);
			}
		}
#endif
		
#if defined(TCSUPPORT_CT_FULL_ROUTE)
	v6:
	if(fullroutesw == 1){
		if(flag == 0)//v4 dns set finish
			goto next;
		if(flag == 2 && i == tempV6)//next time,if it's default DNS no need to write again
			goto next;
	}
#endif
		#ifdef IPV6
		if(getAttrValue(top, nodeName, "Status6", status6) == TCAPI_PROCESS_OK && !strcmp(status6, "up")){
			memset(dns6, 0, sizeof(dns6));
			if(getAttrValue(top, nodeName, "DNS6", dns6) == TCAPI_PROCESS_OK && strlen(dns6) != 0){
				memset(buf, 0, sizeof(buf));
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
				sprintf(buf, "server=%s@%s%s\n", dns6, ifname, ((2 == ipVersion) ? "$ds" : ""));
#else
				sprintf(buf, "server=%s@%s\n", dns6, ifname);			
#endif
				fwrite(buf, sizeof(char), strlen(buf), fp);

				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%s %s %s\n", dns6, ifname, serviceapp);
				fwrite(buf, sizeof(char), strlen(buf), fpInfo);
			}
			memset(secdns6, 0, sizeof(secdns6));
			if(getAttrValue(top, nodeName, "SecDNS6", secdns6) == TCAPI_PROCESS_OK && strlen(secdns6) != 0){
				memset(buf, 0, sizeof(buf));
#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
				sprintf(buf, "server=%s@%s%s\n", secdns6, ifname, ((2 == ipVersion) ? "$ds" : ""));
#else
				sprintf(buf, "server=%s@%s\n", secdns6, ifname);			
#endif
				fwrite(buf, sizeof(char), strlen(buf), fp);

				memset(buf, 0, sizeof(buf));
				sprintf(buf, "%s %s %s\n", secdns6, ifname, serviceapp);
				fwrite(buf, sizeof(char), strlen(buf), fpInfo);
			}
		}
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
		else if ( ( 1 == ipVersion || 2 == ipVersion )
				&& 1 == isOndemandWan(top, i/MAX_SMUX_NUM, i%MAX_SMUX_NUM) )
		{
			sprintf(ppphisaddr, "2014:1211::%d", i+6311);
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "server=%s@%s\n", ppphisaddr, ifname); 		
			fwrite(buf, sizeof(char), strlen(buf), fp);

			memset(buf, 0, sizeof(buf));
			sprintf(buf, "%s %s %s\n", ppphisaddr, ifname, serviceapp);
			fwrite(buf, sizeof(char), strlen(buf), fpInfo);

			addDnsRoute6forOtherWan(ifname, serviceapp, ppphisaddr);
		}
#endif

		#endif
#if defined(TCSUPPORT_CT_FULL_ROUTE)
	next:
	if(fullroutesw == 1){
			if(flag != 2){//set default DNS first
				if(++flag == 2)
					i = -1;
			}
	}
#endif
	}

#if defined(TCSUPPORT_CT_PPPOE_IPV6_EMULATOR)
	addEMUDNSConfig(fp, fpInfo);
#endif

#if defined(TCSUPPORT_CT_ADV_DNSPROXY)
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], WAN);
	strcpy(nodeName[1], WAN_COMMON);

	if( TCAPI_PROCESS_OK == getAttrValue(top, nodeName, "DNSForwardDelay", query_delay)
		&& '\0' != query_delay[0] )
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "query-delay=%s\n", query_delay); 		
		fwrite(buf, sizeof(char), strlen(buf), fp);
	}
#endif

	fclose(fp);
	fclose(fpInfo);

	/* step3: start dnsmasq process */
	system("/userfs/bin/dnsmasq &");

	//tcdbg_printf("\n %s:dnsmasq restart!\n", __func__);
	sleep(1);
#if defined(TCSUPPORT_CT_DNSMASQ_MONITOR)
	pthread_mutex_unlock(&dnsmasq_restart_mutex);	
#endif
	return;
}
#endif

#if defined(TCSUPPORT_IGMP_PROXY)
void restart_igmpproxy(int if_state, char *if_name, int if_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[5] = {0}, upstreamif[16] = {0};
	mxml_node_t *top = get_node_top();

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"IGMPproxy");
  	strcpy(nodeName[1], "Entry");

	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK &&
		getAttrValue(top, nodeName, "UpstreamIF", upstreamif) == TCAPI_PROCESS_OK){
		if(!strcmp(active, ACTIVE) && !strcmp(upstreamif, if_name)){
			//tcdbg_printf("%s: igmp upstream interface %s is up, restart\n",__func__, if_name);
			tcapi_commit_internal(top, "IGMPproxy");
		}
	}
	
}
#endif

#ifdef TCSUPPORT_MLD_PROXY
void restart_mldproxy(int if_state, char *if_name, int if_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char active[5] = {0}, upstreamif[16] = {0};
	mxml_node_t *top = get_node_top();

	memset(nodeName, 0, sizeof(nodeName));
  	strcpy(nodeName[0],"MLDproxy");
  	strcpy(nodeName[1], "Entry");

	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK &&
		getAttrValue(top, nodeName, "UpstreamIF", upstreamif) == TCAPI_PROCESS_OK){
		if(!strcmp(active, ACTIVE) && !strcmp(upstreamif, if_name)){
			//tcdbg_printf("%s: igmp upstream interface %s is up, restart\n",__func__, if_name);
			tcapi_commit_internal(top, "MLDproxy");
		}
	}
	
}
#endif

void restart_dhcprelay(mxml_node_t *top,int if_index)
{
	char buf[32], nodeName[MAX_ARG_NUM][MAX_NODE_NAME];


	if(g_dhcprelay)
		return;

	memset(nodeName, 0, sizeof(nodeName) );
	sprintf(nodeName[0], WAN);
	sprintf(nodeName[1], "%s%d", "PVC", if_index/MAX_PVC_NUM );
	sprintf(nodeName[2], ENTRY_NODENAME,  if_index%MAX_PVC_NUM );

	memset(buf, 0, sizeof(buf));
	if(getAttrValue(top,nodeName, "ISP", buf) != TCAPI_PROCESS_OK)
	{
		tcdbg_printf("get ISP value fail\n");
		return;
	}

	if(atoi(buf) != 0 && atoi(buf) != 1)
	{
		tcdbg_printf("ISP value fail\n");
		return;
	}

	memset(buf, 0, sizeof(buf));
	if(getAttrValue(top,nodeName, "ServiceList", buf) != TCAPI_PROCESS_OK)
	{
		tcdbg_printf("get ISP ServiceList fail\n");
		return;
	}
			
			
	if(strstr(buf, "INTERNET") == NULL)
	{
		tcdbg_printf("get INTERNET value fail\n");
		return;
	}


	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], LAN);
	strcpy(nodeName[1],LAN_DHCP);

	if(getAttrValue(top,nodeName, LAN_DHCP_TYPE, buf) != TCAPI_PROCESS_OK){
		tcdbg_printf("get dhcp relay type value fail\n");
		return;
	}

	if(0 != strcmp(buf,"2")){/*2:dhcprelay*/
		tcdbg_printf("dhcp relay value not open\n");
		return;
	}

	g_dhcprelay = 1;

	return ;
}
static void send_signal_wan_dhcp(int dhcp_signal_type)
{
	int i;
	char active[6] = {0}, isp[6] = {0}, ifname[32] = {0};
	char udhcp_pid_path[64] = {0}, pid[8] = {0};
	char ipversion[16] = {0};

	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		memset(active, 0, sizeof(active));
		memset(isp, 0, sizeof(isp));
		memset(ifname, 0, sizeof(ifname));
		memset(ipversion, 0, sizeof(ipversion));
		if(get_waninfo_by_index(i, "Active", active) == SUCCESS &&
			get_waninfo_by_index(i, "ISP", isp) == SUCCESS &&
			get_waninfo_by_index(i, WAN_IFNAME, ifname) == SUCCESS &&
			get_waninfo_by_index(i, "IPVERSION", ipversion) == SUCCESS){
			/*not active*/
			if(strcmp(active, "Yes"))
				continue;

			/*not dynamic mode*/
			if(strcmp(isp, DYNMIC_MODE))
				continue;
			
			/*IPv4 Dynamic mode*/
			if(!strcmp(ipversion, "IPv4/IPv6") || !strcmp(ipversion, "IPv4")){
				/*Get udhcpc process-id to send signal to release ip or renew ip*/
				memset(udhcp_pid_path, 0, sizeof(udhcp_pid_path));
				sprintf(udhcp_pid_path, "%s%s/udhcpc.pid", WAN_IF_PATH, ifname);
				fileRead(udhcp_pid_path, pid, sizeof(pid));				
				if(strlen(pid)!=0){
					switch(dhcp_signal_type){
						case DHCP_SIGNAL_RELEASE:
#ifdef WAN_DBUG
							tcdbg_printf("release_dhcp:%s is active dhcp mode interface, send signal to release ip\n", ifname);
#endif
							/*Release WAN IP*/
							kill(atoi(pid), SIGUSR2);
							g_wan_if[i].dhcp_renew_flag = 1;
							break;
						case DHCP_SIGNAL_RENEW:
#ifdef WAN_DBUG
							tcdbg_printf("release_dhcp:%s is active dhcp mode interface, send signal to renew ip\n", ifname);
#endif
							/*Renew WAN IP*/
							if(g_wan_if[i].dhcp_renew_flag){
								kill(atoi(pid), SIGUSR1);
								g_wan_if[i].dhcp_renew_flag = 0;
							}
							break;
						default:
							tcdbg_printf("send_signal_wan_dhcp:unknown dhcp signal type:%d\n",dhcp_signal_type);
							break;
					}
				}
				else{
					tcdbg_printf("release_dhcp:%s do not run udhcpc process, release or renew failed\n", ifname);
				}
			}

#ifdef IPV6				
			/*IPv6 Dynamic mode*/
			if(!strcmp(ipversion, "IPv4/IPv6") || !strcmp(ipversion, "IPv6")){
				/*Get dhcp6c process-id to send signal to release ip*/
				memset(udhcp_pid_path, 0, sizeof(udhcp_pid_path));
				sprintf(udhcp_pid_path, "%s%s/dhcp6c.pid", WAN_IF_PATH, ifname);
				fileRead(udhcp_pid_path, pid, sizeof(pid));
				if(strlen(pid)!=0){
					switch(dhcp_signal_type){
					case DHCP_SIGNAL_RELEASE:
#ifdef WAN_DBUG
						tcdbg_printf("release_dhcp:%s is active IPv6 dhcp mode, send signal(%s) to release ip\n", ifname, pid);
#endif
						/*Release WAN IPv6 IP*/
						kill(atoi(pid), SIGHUP);
						
						break;
					case DHCP_SIGNAL_RENEW:
						//tcdbg_printf("release_dhcp:%s is active IPv6 dhcp mode, send signal to renew ip\n", ifname);
						/*Renew WAN IP*/						
						/*Release WAN IPv6 IP*/
						//kill(atoi(pid), SIGHUP);
						break;
					default:
						tcdbg_printf("send_signal_wan_dhcp:unknown dhcp6c signal type:%d\n",dhcp_signal_type);
						break;
					}
				}
				else{
					tcdbg_printf("release_dhcp:%s do not run dhcp6c process, release or renew failed\n", ifname);
				}
			}				
#endif
			}
		}	
}

static void send_signal_wan_pppd()
{
	int i;
	char active[6], isp[6], ifname[32];
	char ppp_pid_path[64] = {0}, pid[8] = {0};
	
	for(i = 0; i < MAX_WAN_IF_INDEX; i++){
		memset(active, 0, sizeof(active));
		memset(isp, 0, sizeof(isp));
		memset(ifname, 0, sizeof(ifname));
		if(get_waninfo_by_index(i, "Active", active) == SUCCESS &&
			get_waninfo_by_index(i, "ISP", isp) == SUCCESS &&
			get_waninfo_by_index(i, WAN_IFNAME, ifname) == SUCCESS){
			if(!strcmp(active, "Yes") && !strcmp(isp, PPP_MODE)){
				sprintf(ppp_pid_path, "%s%s/%s.pid", WAN_IF_PATH, ifname, ifname);
				fileRead(ppp_pid_path, pid, sizeof(pid));
				if(strlen(pid)==0){
#ifdef WAN_DBUG
					tcdbg_printf("send_signal_wan_pppd:%s do not run ppp process, send signal failed\n", ifname);
#endif
					continue;
				}
#ifdef WAN_DBUG
				tcdbg_printf("send_signal_wan_pppd:send SIGHUP to %s for PADI again\n", ifname);
#endif
				/*to send PADI again*/
				kill(atoi(pid), SIGHUP);
			}
		}		
	}
}
#if defined(TCSUPPORT_CT_PON_CY_JX)
unsigned long secclock()
{
	char tmpbuf[30];
	char cmdbuf[100];
	char *temp;
	char *delim = " ";
	unsigned long sectick = 0;
	char *end;
	char path[] = "/tmp/uptime_cwmp.txt";
	sprintf(cmdbuf,"cat /proc/uptime > %s",path);
	system(cmdbuf);
	
	fileRead(path,tmpbuf,30);
	temp = strtok(tmpbuf,delim);
	sectick = strtoul(temp,&end,0);
	return sectick;

}
static char ponuptime[32] = "0001-01-01T00:00:00";

char * getUpDateTime(void)
{
	unsigned short	years,months,days,hrs,mins,secs;
	unsigned long uptime;
	

	uptime=secclock();
	secs = (unsigned short) (uptime % 60);
	uptime /= 60;
	mins = (unsigned short) (uptime % 60);
	uptime /= 60;
	hrs = (unsigned short) (uptime % 24);
	uptime /= 24;
	days = (unsigned short) (uptime % 30);
	uptime /= 30;
	months = (unsigned short) (uptime % 12);
	uptime /= 12;
	years = (unsigned short) uptime;
	sprintf(ponuptime,"%04u/%02u/%02u %02u:%02u:%02u",years+1,months+1,days+1,hrs,mins,secs);
	return ponuptime;
}

#endif

int adsl_state_change(int adsl_state)
{
// #endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
	char cmdbuf[100]={0};
	int   temp=0;
	FILE *fp=NULL;
#endif
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_PON)
#if !defined(TCSUPPORT_CT_PON_GD) && !defined(TCSUPPORT_CT_PON_CQ)
	int i = 0;
	char nodeName[32] = {0};
	char string[80] = {0};
	int dsl_state = 0;
#endif
	int needCommit = 0;
#endif
#if defined(TCSUPPORT_CT_FJ)
	struct timespec timeSpec;
	char OltRegTime[20] = {0};				
	int regtime;
#endif
	char linkStat[16] = {0};
	char xponModeBuf[16] = {0};
#if defined(TCSUPPORT_CT_PON_CY_JX)
	char cmd[32] ={0};
#endif

	switch(adsl_state){
		case ADSL_STATE_UP:
			if(g_adsl_state == ADSL_STATE_DOWN){
				/*Add action when adsl from down to up*/
#ifdef WAN_DBUG
				tcdbg_printf("adsl_state_change:adsl link from down to up\n");
#endif
#if defined(TCSUPPORT_CT_WAN_PTM)	
				dsl_state = get_dsl_state();
#if defined(TCSUPPORT_CT_DSL_EX)
				copy_wan_node(dsl_state);
#endif

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7512)
				if(g_dsl_state != dsl_state){		//ATM->PTM or PTM->ATM
					if(dsl_state == 2){   //PTM				
#if defined(TCSUPPORT_CT_SWQOS)
						system("rmmod swqos.ko");
#endif
						system("rmmod mt7510sar.ko");
#ifdef TCSUPPORT_CPU_EN7512
						system("rmmod qdma_wan.ko");
#else
                                                     system("rmmod qdma.ko");
#endif
						
#if defined(TCSUPPORT_CT_SWQOS)
						system("insmod /lib/modules/swqos.ko");
						tcapi_commit("QoS");
#endif
#ifdef TCSUPPORT_CPU_EN7512
						system("insmod /lib/modules/qdma_wan.ko macType=2");
#else
						system("sys memwl bfb00070 2");
                                                     system("insmod /lib/modules/qdma.ko");
						system("sys memwl bfb5152c ffffffff");
#endif
						system("insmod /lib/modules/mt7510ptm.ko");
					}else{
						system("rmmod mt7510ptm.ko");
#ifdef TCSUPPORT_CPU_EN7512
						system("rmmod qdma_wan.ko");
#else
                                                     system("rmmod qdma.ko");
#endif
#if defined(TCSUPPORT_CT_SWQOS)
						system("rmmod swqos.ko");
#endif
#ifdef TCSUPPORT_CPU_EN7512
						system("insmod /lib/modules/qdma_wan.ko macType=3");
#else
						system("sys memwl bfb00070 3");
						system("insmod /lib/modules/qdma.ko");
						system("sys memwl bfb5152c ffffffff");
#endif
						system("insmod /lib/modules/mt7510sar.ko");
#if defined(TCSUPPORT_CT_SWQOS)
						system("insmod /lib/modules/swqos.ko");
						tcapi_commit("QoS");
#endif
					}
				}
				else if(g_dsl_state == 2){ //PTM 	
	            /* when vdsl is from down to up, PTM MAC and TC will have chance to be un-sync, so reset is needed */
					system("echo 1 >/proc/tc3162/ptm_do_reset_sequence");
				}
#endif
				needCommit = 1;
				g_dsl_state = dsl_state;
#else
#if defined(TCSUPPORT_CT_PON)
				system("echo up > /tmp/xpon_stats");
				needCommit = 1;
			#if defined(TCSUPPORT_XPON_LED)
				/*xpon_led("2"); //led on*/
			#endif
#if defined(TCSUPPORT_CT_PON_CY)							
							check_ponvlan_status();
#endif
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CQ)
				/*renew all dhcp wan interface*/
				send_signal_wan_dhcp(DHCP_SIGNAL_RELEASE);
				send_signal_wan_dhcp(DHCP_SIGNAL_RENEW);

				/*signal pppd send PADI again for CT work order test failed*/
				send_signal_wan_pppd();			
#endif
#else
				/*renew all dhcp wan interface*/
				send_signal_wan_dhcp(DHCP_SIGNAL_RENEW);

				/*signal pppd send PADI again for CT work order test failed*/
				send_signal_wan_pppd();			
#endif
#endif
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
				system("iptables -t nat -F PRE_PONSTATE");
				system("iptables -t nat -Z PRE_PONSTATE");
				system("iptables -t nat -A PRE_PONSTATE -j RETURN");
#endif
#ifdef CWMP
#if defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_MIDWARE)
				sendMsgtoCwmp(2, 1, 0);
#else
				sendMsgtoCwmp(2);
#endif
#endif
#endif
#if defined(TCSUPPORT_CT_PON_CY_JX)
		    	sprintf(cmd,"echo %s > /tmp/pon_uptime.conf",(char*)getUpDateTime());
				system(cmd);
#endif
#if defined(TCSUPPORT_CT_FJ)
				clock_gettime(CLOCK_MONOTONIC,&timeSpec);
				regtime = timeSpec.tv_sec - g_OltRegStartTime;
				sprintf(OltRegTime,"%d",regtime); 
				tcapi_set("deviceAccount_Entry", "OltRegTime", OltRegTime);
#endif
				g_adsl_state = ADSL_STATE_UP;
			}
			break;
			
		case ADSL_STATE_DOWN:
			if(g_adsl_state == ADSL_STATE_UP){
				/*Add action when adsl from up to down*/
#ifdef WAN_DBUG
				tcdbg_printf("adsl_state_change:adsl link from up to down\n");
#endif
#if defined(TCSUPPORT_CT_PON)
			system("echo down > /tmp/xpon_stats");
			#if defined(TCSUPPORT_XPON_LED) && !defined(TCSUPPORT_CUC)
				/*xpon_led("0"); //led off*/
			#endif
#endif

#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_PON)
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CQ)
				/*release all dhcp wan interface*/
				send_signal_wan_dhcp(DHCP_SIGNAL_RELEASE);

				/*signal pppd send PADI again*/
				send_signal_wan_pppd();	
#endif
				needCommit = 1;
#if defined(TCSUPPORT_CT_WAN_PTM)
			 /* when xdsl from up to down, let PTM MAC reset hold, in order to 
             * make sure dmt reset(when vdsl is up) is inside PTM MAC reset */
				if (g_dsl_state != 0)
					system("echo 0 >/proc/tc3162/ptm_do_reset_sequence");
#endif
#else
				/*release all dhcp wan interface*/
				send_signal_wan_dhcp(DHCP_SIGNAL_RELEASE);

				/*signal pppd send PADI again*/
				send_signal_wan_pppd();				
#endif
#if defined(TCSUPPORT_CT_FJ)
				clock_gettime(CLOCK_MONOTONIC,&timeSpec);
				g_OltRegStartTime=timeSpec.tv_sec;
#endif
				g_adsl_state = ADSL_STATE_DOWN;
#if defined(TCSUPPORT_CUC_LANDING_PAGE)
				system("iptables -t nat -F PRE_PONSTATE");
				system("iptables -t nat -Z PRE_PONSTATE");
				system("iptables -t nat -A PRE_PONSTATE -i br0 -p tcp --dport 80 -j REDIRECT");
#endif
			}
			break;
		default:
			tcdbg_printf("adsl_state_change:unknown adsl state type:%d\n",adsl_state);
			break;
	}

	
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_PON)
	if(needCommit == 1){
#if !defined(TCSUPPORT_CT_PON_GD) && !defined(TCSUPPORT_CT_PON_CQ)
		for(i = 0; i < PVC_NUM; i++){
			sprintf(nodeName,"Wan_PVC%d",i);
			tcapi_commit(nodeName);
		}
#endif
#if defined(TCSUPPORT_CT_HWQOS)
		if(g_adsl_state == ADSL_STATE_UP)
		{
#if defined(TCSUPPORT_CT_QOS)
			if ( is_qos_boot )
#endif
				tcapi_commit(QOS);
		}
#endif

#if defined(TCSUPPORT_CT_PON)
		tcapi_get("XPON_Common", PONCOMMON_ATTR_XPON_MODE, xponModeBuf);
		if (!strcmp(xponModeBuf, "EPON")) { /* only for epon product */
			tcapi_get("XPON_LinkCfg","LinkSta",linkStat);
			if (g_adsl_state == ADSL_STATE_DOWN && strcmp(linkStat, "1")) { /* only for epon mode */
				tcdbg_printf("\r\nadsl_state_change(): restart epon after wan stop.");
				setXponModeSwitch(XMCS_IF_WAN_DETECT_MODE_EPON, 0); /* stop first */
				setXponMode(XMCS_IF_WAN_DETECT_MODE_EPON);
			}
		}
#endif
	}
#endif
	return 0;
}

/*****************************************************************************************
****function name
	check_ponvlan_status
****description
	disable ponvlan by the value of cfg_manager node attribute
****input:
****return:
****************************************************************************************/
#if defined(TCSUPPORT_CT_PON_CY)
void check_ponvlan_status(){
	char ponvlan_switch[8] = {0};
	char ponvlan_status[8] = {0};
	
	if ( TCAPI_PROCESS_OK != tcapi_get("SysInfo_Entry", "PonvlanSwitch", ponvlan_switch))
		return;

 	//if ponvlan switch is 0 and ponvlan status is 1, disable ponvlan and modify status to 0
	if ( strcmp(ponvlan_switch, "0") == NULL ){	
		system("/usr/bin/ponvlancmd disable");
	}

	return;
}
#endif

#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
/*______________________________________________________________________________
**function name:recheckPingDiagnostic
**
**description:
*    when wan conncetion ip up and get wan ip,do wan ping if needed
**parameters:
*    none
**global:
*    none
**return:
*    none
**call:
*    interface_state_change
**revision:
**____________________________________________________________________________*/
void recheckPingDiagnostic(mxml_node_t *top,int if_index)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	int i = 0;
	char diagnostic[20] = {0},pingtotal[15] = {0},pingnum[15] = {0}, active[8] = {0};
	//char buf[32] = {0};
	char *temp = NULL;
	char Interfacebuf[32] = {0};
	char cmdbuf[128] = {0};
	int interfaceindex = 0;
	FILE *fp = NULL;
	int totalnum = 0,currentnum = 0;
	int ping_temp=recheckPingDiagnostic_flag;
	//recheckPingDiagnostic_flag=0;
	//mxml_node_t *top = get_node_top();
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],CTCOM_IPPINGDIAGNOSTIC_NODE_NAME);
	strcpy(nodeName[1],CTCOM_PING_COMMON);
	if(getAttrValue(top, nodeName, "Active", active) == TCAPI_PROCESS_OK)
	{
		if(0 == strcmp(active,"No"))
		{
			//tcdbg_printf("\r\nactive is set to No");
			return;
		}
	}
	else
	{
		//tcdbg_printf("\r\nnot fetch the active attribute or active value is no,so do nothing,return!");
		return;
	}

	for(i = 0; i < CTCOM_MAX_IPPINGDIAGNOSTIC_NUM; i++)
	{
		memset(nodeName, 0, sizeof(nodeName));
	  	strcpy(nodeName[0],CTCOM_IPPINGDIAGNOSTIC_NODE_NAME);
	  	sprintf(nodeName[1], "Entry%d",i);
	
		if(getAttrValue(top, nodeName, "DiagnosticsState", diagnostic) == TCAPI_PROCESS_OK &&
			getAttrValue(top, nodeName, "NumberOfRepetitions", pingtotal) == TCAPI_PROCESS_OK	&&
			getAttrValue(top, nodeName, "Interface", Interfacebuf) == TCAPI_PROCESS_OK &&
			getAttrValue(top, nodeName, "PingNum", pingnum) == TCAPI_PROCESS_OK)
		{		
			totalnum = atoi(pingtotal);
			currentnum= atoi(pingnum);	
			//if(!strcmp(diagnostic, "Requested") && (0 == atoi(pingtotal) ||(pingtotal - pingnum) > 0))
			if(strcmp(diagnostic, "None") && ((0 == atoi(pingtotal)) ||(totalnum -currentnum) > 0))//only "None" mean not need to do ping operation
			{
				//if up interface is not the diagnostic interface
				if((temp = strstr(Interfacebuf,"smux")) != NULL)
				{
					temp += strlen("smux");
					interfaceindex = atoi(temp);
					if(if_index != interfaceindex)
					{
						continue;
					}
				}
				else if((temp = strstr(Interfacebuf,LAN_IF)) != NULL)
				{
					continue;
				}
			
			//	sprintf(buf,IPPINGDIAGNOSTIC_ENTRY,i);
				#if 0
				//need to stop ping first wether the ping application is exist or not(not permit to ping more times)
				stopCwmpPing(i);
				#endif
				//we use whether pid file is exist or not as judgement first
				sprintf(cmdbuf,CTCOM_PING_PID_PATH,i);
				fp = fopen(cmdbuf, "r");
				if(fp)
				{
					fclose(fp);
					continue;
				}
				//and then commit the node
 //bit X is 1 meas CtDiagnostic_EntryX need to committed by start_cc
//recheckPingDiagnostic_flag will be checked in start_cc			
				ping_temp|=0x1<<i;
			}
		}
	}
	recheckPingDiagnostic_flag=ping_temp;
	
	return;	
}
#endif

#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
void setNTPDelayFlagByIFindex(int if_index, int index){
	if(if_index > 31)
		ntpdelayflag2_all[index] |=	1 << (if_index - 32);
	else
		ntpdelayflag1_all[index] |=	1 << if_index;
}


void setNTPDelayFlag(char* service_app, int if_index){
	int i = 0;
	
	if (strstr(service_app,INTERNET)){
		setNTPDelayFlagByIFindex(if_index,0);
	}
	
	if(strstr(service_app,VOICE)){
		setNTPDelayFlagByIFindex(if_index,1);
	}
	
	if(strstr(service_app,TR069)){
		setNTPDelayFlagByIFindex(if_index,2);
	}

	if(strstr(service_app,OTHER)){
		setNTPDelayFlagByIFindex(if_index,3);
	}
}

#endif
#endif


int resyncNTPServer(mxml_node_t *top, char *if_name, int if_index, int state)
{
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tz_type[4]={0};
	int pvc_index, entry_index;
	char service_app[SERVICE_LIST_LEN];
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
	char NTPServerType[4] = {0};
	char ntp_service_app[20] = {0};
	int i=0;
#else
	char IFIndex[4] = {0};
#endif
#endif
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0], TIMEZONE);
	strcpy(nodeName[1], SUB_NODE_NAME);
	
	initNTPTimerStruct();
	if(getAttrValue(top, nodeName, "TYPE", tz_type) != TCAPI_PROCESS_OK )
	{
		tcdbg_printf("\r\nresyncNTPServer:get type error!");
		return FAIL;
	}
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
	if(getAttrValue(top, nodeName, TIMEZONE_ATTR_NTPSERVERTYPE, NTPServerType) != TCAPI_PROCESS_OK )
	{
		tcdbg_printf("\r\nresyncNTPServer:get NTPServerType error!");
		return FAIL;
	}
#else
	if(getAttrValue(top, nodeName, TIMEZONE_ATTR_IFINDEX, IFIndex) != TCAPI_PROCESS_OK )
	{
		tcdbg_printf("\r\nresyncNTPServer:get IFIndex error!");
		return FAIL;
	}
#endif
#endif

	if(1)
	{
		/* get service list of this interface */
		pvc_index = if_index / MAX_SMUX_NUM;
		entry_index = if_index - pvc_index * MAX_SMUX_NUM;
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], "Wan");
		sprintf(nodeName[1], "PVC%d", pvc_index);
		sprintf(nodeName[2], "Entry%d", entry_index);
		memset(service_app, 0, sizeof(service_app));
		if (getAttrValue(top, nodeName, "ServiceList", service_app) == TCAPI_PROCESS_OK) 
		{
			if ('\0' == service_app[0]) 
			{
				tcdbg_printf("\r\nresyncNTPServer:without servicelist!");
				return FAIL;
			}
		}
		
#if defined(TCSUPPORT_CT_2NTP)
		if(1)
#else
		/*only handle wan link with internet */
		if (strstr(service_app,INTERNET)) //not only tr069 link
#endif
		{
			switch (state) 
			{
				case IF_STATE_UP:
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
					/***
					NTPServerType:
					0:INTERNET
					1:VOICE
					2:TR069
					3:OTHER
					***/
					#if 1
					setNTPDelayFlag(service_app,if_index);
					#else
					if (strstr(service_app,INTERNET))
						i=0;
					else if(strstr(service_app,VOICE))
						i=1;
					else if(strstr(service_app,TR069))
						i=2;
					else if(strstr(service_app,OTHER))
						i=3;
					else{
						tcdbg_printf("\r\nresyncNTPServer:servicelist error.");
						return FAIL;						
					}
					if(if_index > 31)
						ntpdelayflag2_all[i] |=	1 << (if_index - 32);
					else
						ntpdelayflag1_all[i] |=	1 << if_index;
					if(strstr(service_app,INTERNET)&&strstr(service_app,TR069)){
						if(if_index > 31)
							ntpdelayflag2_all[2] |=	1 << (if_index - 32);
						else
							ntpdelayflag1_all[2] |=	1 << if_index;						
					}
					#endif
					switch (atoi(NTPServerType))
					{
						case 0:
							strcpy(ntp_service_app,INTERNET);
							break;
						case 1:
							strcpy(ntp_service_app,VOICE);
							break;
						case 2:
							strcpy(ntp_service_app,TR069);
							break;
						case 3:
							strcpy(ntp_service_app,OTHER);
							break;			
						default:
							tcdbg_printf("\r\nresyncNTPServer:get NTPServerType error.");
							return FAIL;
					}
					if (strstr(service_app,ntp_service_app)){
#else
					if (atoi(IFIndex)==if_index){
#endif
#endif
					if(if_index > 31)
						ntpdelayflag2 |=	1 << (if_index - 32);
					else
						ntpdelayflag1 |=	1 << if_index;
#if defined(TCSUPPORT_CT_2NTP)
					}
#endif
					ntpneedexecute = NTPFLAGWITHEXECUTE;
					break;
				case IF_STATE_DOWN:
					if(if_index > 31)
						ntpdelayflag2 &=	~(1 << (if_index - 32));
					else
						ntpdelayflag1 &=	~(1 << if_index);
#if defined(TCSUPPORT_CT_2NTP)
#if defined(TCSUPPORT_CT_NTPSERVERTYPE)
					for(i = 0; i < 4; i++){
						if(if_index > 31)
							ntpdelayflag2_all[i] &=	~(1 << (if_index - 32));
						else
							ntpdelayflag1_all[i] &=	~(1 << if_index);	
					}
#endif
#endif
					 //ntpneedexecute = 0;
					break;
				default:
					break;	
			}	
		}
	}

	return SUCCESS;
}
#endif

#if defined(TCSUPPORT_CT_VOIP_QOS)
#define RTP_BIND_WAN_INDEX		"/proc/tc3162/RtpBindWanIdx"
int setVoipWanIdx(mxml_node_t *top,char *if_name,int if_state){
	int if_index;
	char tmpAttrStr[32]= {0};
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];	
	char cmd[128] = {0};
	int pvc_index = 0;
	int entry_index = 0;

	if((if_index = get_wanindex_by_name(if_name)) < 0){
		return FAIL;
	}
	
	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index % MAX_SMUX_NUM;

	memset(wanNode, 0, sizeof(wanNode));
	strncpy(wanNode[0], "Wan", sizeof(wanNode[0]));
	snprintf(wanNode[1], sizeof(wanNode[1]), "PVC%d", pvc_index);
	snprintf(wanNode[2], sizeof(wanNode[2]), "Entry%d", entry_index);

	memset(tmpAttrStr, 0, sizeof(tmpAttrStr));
	if (getAttrValue(top, wanNode, "ServiceList", tmpAttrStr) == TCAPI_PROCESS_OK) {
		if (strstr(tmpAttrStr, "VOICE") != NULL){		// find internet wan
			if(if_state == IF_STATE_UP)
				sprintf(cmd,"echo %d > %s",if_index, RTP_BIND_WAN_INDEX);
			else
				sprintf(cmd,"echo -1 > %s", RTP_BIND_WAN_INDEX);
			system(cmd);
			return 0;
		}
	}

	return -1;
}
#endif

#if defined(TCSUPPORT_ECN_MEGACO)
int setVoIPWanIP(mxml_node_t *top,int if_index,int if_state){
	char wanInfoNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char tempip[64] = {0};
	char ip[64] = {0};
	
	memset(wanInfoNode, 0, sizeof(wanInfoNode));
	strcpy(wanInfoNode[0], WAN_IF_NODE);
	sprintf(wanInfoNode[1], "%s%d",SUB_NODE_NAME,if_index);
	tcdbg_printf("wanInfoNode=%s_%s,if_state=%d\n",wanInfoNode[0],wanInfoNode[1],if_state);
	if(if_state == IF_STATE_DOWN || getAttrValue(top, wanInfoNode, "IP", tempip) != TCAPI_PROCESS_OK || !strcmp(tempip,"0.0.0.0")){
		//get wan ip fail, set with lan ip
		memset(nodeName, 0, sizeof(nodeName));
		strcpy(nodeName[0], LAN);
		sprintf(nodeName[1],"%s%d" SUB_NODE_NAME,0);
		
		if(getAttrValue(top, nodeName, LAN_IP, ip) != TCAPI_PROCESS_OK){
			//get lan ip fail
			strcpy(ip,"192.168.1.1");
		}	
	}else{
		//get wan ip ok, set with wan ip
		strcpy(ip,tempip);
	}

	tcdbg_printf("setVoIPWanIP:ip=%s\n",ip);
	
	memset(nodeName, 0, sizeof(nodeName));
	strcpy(nodeName[0], VOIPINFO_H248);
	strcpy(nodeName[1], VOIPCOMMON);
	setAttrValue(top, nodeName, "IP", ip);

	return 0;
}


#define VOIP_PID_PATH "/tmp/tcVoIPApiServer.pid"
static void _mgappRestart(mxml_node_t *top)
{    
	FILE *fp = NULL;
	char cmdbuf[128] = {0};
	char voip_pid_tmp[100] = {0};
	int mgapp_pid = 0;
	char FlagFwUp_s[20] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"System");
	strcpy(nodeName[1],"Entry"); 
    
	getAttrValue(top, nodeName, "upgrade_status", FlagFwUp_s);
	fp = fopen(VOIP_PID_PATH, "r");
	if(fp == NULL)
	{
		/*get tcVoIPApiServer process pid*/
		sprintf(cmdbuf, "pidof %s > %s", "tcVoIPApiServer", VOIP_PID_PATH);
		system(cmdbuf);  
		fp = fopen(VOIP_PID_PATH, "r");
	}
	
	if(fp == NULL){
		return;
	}
    
	fgets(voip_pid_tmp, sizeof(voip_pid_tmp), fp);
	fclose(fp);
	unlink(VOIP_PID_PATH);
	tcdbg_printf("\n_mgappRestart, tcVoIPApiServer pid :%s, upgrade_status:%s\n", voip_pid_tmp, FlagFwUp_s);
	if((atoi(voip_pid_tmp) != 0)&&(0 != strcmp(FlagFwUp_s,"true"))){  /*if voip_pid_tmp != 0, that's mean need restart mgappt*/
		tcdbg_printf("\n %d_mgappRestart restart..\n", __LINE__);
		system("killall -9 mgapp");
		system("/userfs/bin/mgapp -id 0 &");
	}

    return;
}

int updateVoIP(mxml_node_t *top,char *if_name,int if_index,int if_state){
	char cmdBuf[256] = {0};	
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];
	char InfoVoIPNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char mgcIP[64] ={0};
	char mgc2IP[64] ={0};		
	char convertIP[64]={0};		
	char convert2IP[64]={0};	
	struct sockaddr_in cvtaddr = {0};
	struct sockaddr_in addr = {0};
	unsigned long theHostAddressPtr;
	int  pvc_index                            = 0;
	int  entry_index                          = 0;
	char str_servicelist[32]                  = {0};
	int flag  = 0;
	char bindIf[32] = {0};
	char preIP[64] = {0};
	char preIP2[64] = {0};
	int ret=0;
	int j=0;
	char InfoVoIPH248[MAX_ARG_NUM][MAX_NODE_NAME];

	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index % MAX_SMUX_NUM;
	
	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],WAN);
	sprintf(nodeName[1],"%s%d",WAN_PVC,pvc_index); 	
	sprintf(nodeName[2],"%s%d",WAN_INTERFACE,entry_index); 	
	
	if(getAttrValue(top, nodeName, "ServiceList", str_servicelist) != TCAPI_PROCESS_OK || NULL == strstr(str_servicelist, "VOICE")){
		//not voip wan
		return -1;
	}	

	setVoIPWanIP(top,if_index,if_state);
	memset(InfoVoIPNode,0,sizeof(InfoVoIPNode));
	strcpy(InfoVoIPNode[0],VOIPINFO_H248);
	strcpy(InfoVoIPNode[1],VOIPCOMMON); 	
	getAttrValue(top, InfoVoIPNode, "bindIf", bindIf);
	
	if(if_state == IF_STATE_UP){
		if(strcmp(bindIf,if_name)){
			setAttrValue(top, InfoVoIPNode, "bindIf", if_name);	
			sprintf(cmdBuf,"varUpd {config,mgBindIf}{%s}",if_name);
			tcdbg_printf("[%s:%d]1 cmdBuf=%s\n",__FUNCTION__,__LINE__,cmdBuf);	
			ret=voipCmdSend(cmdBuf);
			if(ret==-1){
				goto restart_mgapp;
			}
		}

		memset(nodeName,0,sizeof(nodeName));
		strcpy(nodeName[0],VOIPH248);
		strcpy(nodeName[1],VOIPCOMMON); 	
		if(getAttrValue(top, nodeName, MGC_CONTROLER, mgcIP) == TCAPI_PROCESS_OK && strlen(mgcIP) != 0){
            #if 0
			if(inet_aton(mgcIP,&cvtaddr.sin_addr))
			{	
				//address is ipv4
				strcpy(convertIP,mgcIP);
			}
			else{
				//address is a domain name
				theHostAddressPtr = resolveName(mgcIP);
				memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
				strcpy(convertIP, inet_ntoa(addr.sin_addr));
			}
            #endif

			getAttrValue(top, InfoVoIPNode, "preIP", preIP);
			if(strcmp(preIP,mgcIP) == 0){
				tcdbg_printf("updateVoIP:preIP = %s is the same\n",preIP);
			}else{
				//strcpy(preIP, convertIP);
				setAttrValue(top, InfoVoIPNode, "preIP", mgcIP);
				flag = 1;
			}

			
			memset(cmdBuf,0,sizeof(cmdBuf));
			if(getAttrValue(top, nodeName, MGC_SB_CONTROLER, mgc2IP) == TCAPI_PROCESS_OK && strlen(mgc2IP) != 0){
                #if 0
				if(inet_aton(mgc2IP,&cvtaddr.sin_addr))
				{	
					//address is ipv4
					strcpy(convert2IP,mgc2IP);
				}
				else{
					//address is a domain name
					theHostAddressPtr = resolveName(mgc2IP);
					memcpy(&addr.sin_addr, &theHostAddressPtr, sizeof(addr.sin_addr));
					strcpy(convert2IP, inet_ntoa(addr.sin_addr));
				}
                #endif

				getAttrValue(top, InfoVoIPNode, "preIP2", preIP2);
				if(strcmp(preIP2,mgc2IP) == 0){
					tcdbg_printf("updateVoIP:preIP2 = %s is the same\n",preIP2);
				}else{
					//strcpy(preIP2, convert2IP);
					setAttrValue(top, InfoVoIPNode, "preIP2", mgc2IP);
					flag = 1;
				}
				sprintf(cmdBuf,"callCmd {test.mgcReconfig}{%s}{%s}",mgcIP,mgc2IP);
			}else{
				sprintf(cmdBuf,"callCmd {test.mgcReconfig}{%s}",mgcIP);
			}

			if(flag == 1){
				//server ip modify,reconfig
				tcdbg_printf("[%s:%d]2 cmdBuf=%s\n",__FUNCTION__,__LINE__,cmdBuf);	
				ret=voipCmdSend(cmdBuf);
				if(ret==-1){
				goto restart_mgapp;
				}
			}
		}else{
			tcdbg_printf("[%s:%d]get mgcIP fail!!!\n",__FUNCTION__,__LINE__);	
		}
	
		if(flag == 0){
			//server ip is the same, restart
			tcdbg_printf("[%s:%d]if_name=%s,lastVoIPIf=%s,if_state=%d, interface is the same!!!\n",__FUNCTION__,__LINE__,if_name,bindIf,if_state);	
			ret=voipCmdSend("callCmd {test.Restart}");
			if(ret==-1){
				goto restart_mgapp;
			}
		}
	}else{	
		//down
		for(j=0 ; j<VOIPLineNum ; j++){ 
			memset(InfoVoIPH248, 0, sizeof(InfoVoIPH248));
			snprintf(InfoVoIPH248, sizeof(InfoVoIPH248) - 1, "InfoVoIPH248_Entry%d", j);
			strcpy(InfoVoIPH248[0],VOIPINFO_H248);
			sprintf(cmdBuf,"Entry%d",j);
			strcpy(InfoVoIPH248[1],cmdBuf);
			setAttrValue(top, InfoVoIPH248, "RegFailReason", "6");
		}
		setAttrValue(top, InfoVoIPNode, "bindIf", "");
		ret=voipCmdSend("callCmd {test.wanDown}");
		if(ret==-1){
			goto restart_mgapp;
		}
	}
	
	return -1;
restart_mgapp:	
		_mgappRestart(top);
		return 0;
}
#endif

#if defined(TCSUPPORT_CT_PHONEAPP)
#define WANCHANGEFIFO  "/tmp/fifo_wanchange"
void sendfifo2mobile(char input){
	int fd = -1, ret = 0;

	fd = open(WANCHANGEFIFO, O_WRONLY|O_NONBLOCK);
	if(fd < 0) {
		tcdbg_printf("sendfifo2mobile:open fifo err!\n");
		return;
	}

	ret = write(fd, &input,sizeof(char));
	if(ret < 0 ){
		tcdbg_printf("sendfifo2mobile:write fifo err!\n");
	}
	//tcdbg_printf("sendfifo2mobile:write fifo :%d!\n", input);
	close(fd);	
	
}
#endif

#if defined(TCSUPPORT_CT_JOYME)
WanIpEvent wanIpCfg;
void wanIpChangeInform(mxml_node_t *top, int if_index, int if_state)
{
	int ipv4_change=0;
	int ipv6_change=0;
	int Cindex = -1;
	int len=0;
	int tempbuf[64] = {0};
	char nodeName[MAX_ARG_NUM][MAX_NODE_NAME];

	memset(nodeName,0,sizeof(nodeName));
	strcpy(nodeName[0],"WanInfo");
	strcpy(nodeName[1],"Common");

	switch(if_state){
		case IF_STATE_DOWN:
			getAttrValue(top, nodeName, "DefRouteIndexv4", tempbuf);
			if(!strcmp(tempbuf,"N/A")){
		         strcpy(wanIpCfg.wanIpAddr,"0.0.0.0");
		         strcpy(wanIpCfg.wanSubnetMask,"0.0.0.0");
				 ipv4_change=1;
	        }
			else
				return;			
			break;
		case IF_STATE_DOWN6:
			getAttrValue(top, nodeName, "DefRouteIndexv6", tempbuf);
		    if(!strcmp(tempbuf,"N/A")){
		       strcpy(wanIpCfg.ipv6WanAddr,"0:0:0:0:0:0:0:0");
		       wanIpCfg.wanPrefixLen = 0;		   
			   ipv6_change=1;
	        }
			else
				return;
			break;
		case IF_STATE_UP:
			getAttrValue(top, nodeName, "DefRouteIndexv4", tempbuf);
			Cindex=atoi(tempbuf);
			if(Cindex == if_index){
				memset(nodeName, 0, sizeof(nodeName));
				strcat(nodeName[0],"WanInfo");
				sprintf(nodeName[1],"Entry%d",atoi(tempbuf));
	
				memset(tempbuf,0,sizeof(tempbuf));
				getAttrValue(top, nodeName, "IP", tempbuf);
				memset(wanIpCfg.wanIpAddr,0,sizeof(wanIpCfg.wanIpAddr));
				len = sizeof(wanIpCfg.wanIpAddr) - 1;
				strncpy(wanIpCfg.wanIpAddr,tempbuf,len);
				
				memset(tempbuf,0,sizeof(tempbuf));
				getAttrValue(top, nodeName, "NetMask", tempbuf);
				len = sizeof(wanIpCfg.wanSubnetMask) - 1;
				memset(wanIpCfg.wanSubnetMask,0,sizeof(wanIpCfg.wanSubnetMask));
				strncpy(wanIpCfg.wanSubnetMask,tempbuf,len);
				ipv4_change=1;
		     }
			else
				return;
			break;
		case IF_STATE_UP6:
			getAttrValue(top, nodeName, "DefRouteIndexv6", tempbuf);
			Cindex=atoi(tempbuf);
			if(Cindex == if_index){
				memset(nodeName, 0, sizeof(nodeName));
				strcat(nodeName[0],"WanInfo");
				sprintf(nodeName[1],"Entry%d",atoi(tempbuf));
	
				memset(tempbuf,0,sizeof(tempbuf));
				getAttrValue(top, nodeName, "IP6", tempbuf);
				memset(wanIpCfg.ipv6WanAddr,0,sizeof(wanIpCfg.ipv6WanAddr));
				len = sizeof(wanIpCfg.ipv6WanAddr) - 1;
				strncpy(wanIpCfg.ipv6WanAddr,tempbuf,len);
				
				memset(tempbuf,0,sizeof(tempbuf));
				getAttrValue(top, nodeName, "PrefixLen6", tempbuf);
				wanIpCfg.wanPrefixLen= atoi(tempbuf);
				ipv6_change=1;
			}
			else
				return;
			break;
		default:
			tcdbg_printf("wanIpChangeInform:unknown interface state type:%d\n", if_state);
			break;
	}
	if(ipv4_change ||  ipv6_change ){
		nlk_msg_t nklmsg;
		int ret = 0;
		memset(&nklmsg, 0, sizeof(nlk_msg_t));	
		nklmsg.nlmhdr.nlmsg_len = sizeof(nlk_msg_t);
		nklmsg.nlmhdr.nlmsg_pid = getpid();  /* self pid */
		nklmsg.nlmhdr.nlmsg_flags = 0;
		
		nklmsg.eventType = MSG_MUlTICAST_EVENT;
		
		sprintf(nklmsg.data.payload, "{\"Event\":\"WAN_IP_CHANGE\",\"IpAddr\":\"%s\",\"SubnetMask\":\"%s\",\"IpV6Addr\":\"%s\",\"PrefixLen\":\"%d\"}",
			wanIpCfg.wanIpAddr,wanIpCfg.wanSubnetMask,wanIpCfg.ipv6WanAddr,wanIpCfg.wanPrefixLen);

		ret = nlkmsg_send(NLKMSG_GRP_MULTI, sizeof(nklmsg), &nklmsg);
		if(ret <0) tcdbg_printf("wanchange send err:%s!\n", strerror(errno));
	}
}
#endif

int interface_state_change(mxml_node_t *top, int if_state, char *if_name)
{
	int if_index;
	int pvc_index=0;
	int entry_index=0;
	char wanNode[MAX_ARG_NUM][MAX_NODE_NAME];
	char proxyenable[4]={0};
	int i = 0;
	char nat_node[32] = {0};
	char tmpAttrStr[32]= {0};
#ifdef IPV6			
	int def_route_index = 0;
	int run_pd_index = 0, ra_pd_index = 0, dns_wan_index = 0;
#endif
#if defined(TCSUPPORT_CT_INFORM_NODE)
	char cwmpNode[MAX_ARG_NUM][MAX_NODE_NAME];
	int srvFindFlag = SRV_TR069_NO_FIND;
#endif
	/*Get wan interface cfg node index*/
	if((if_index = get_wanindex_by_name(if_name)) < 0){
		return FAIL;
	}
	
	pvc_index = if_index / MAX_SMUX_NUM;
	entry_index = if_index % MAX_SMUX_NUM;

	memset(wanNode, 0, sizeof(wanNode));
	strncpy(wanNode[0], "Wan", sizeof(wanNode[0]));
	snprintf(wanNode[1], sizeof(wanNode[1]), "PVC%d", pvc_index);
	snprintf(wanNode[2], sizeof(wanNode[2]), "Entry%d", entry_index);
#if defined(TCSUPPORT_CT_PPPOEPROXY)
	if (getAttrValue(top, wanNode, "ProxyEnable", proxyenable) != TCAPI_PROCESS_OK) {
                strncpy(proxyenable, "0", sizeof(proxyenable));
		   setAttrValue(top, wanNode, "ProxyEnable", proxyenable);
	}
#endif

#if defined(TCSUPPORT_CT_INFORM_NODE)
	memset(tmpAttrStr, 0, sizeof(tmpAttrStr));
	if (getAttrValue(top, wanNode, "ServiceList", tmpAttrStr) == TCAPI_PROCESS_OK) {
		if (strstr(tmpAttrStr, "TR069") != NULL){
			srvFindFlag = SRV_TR069_FIND;
		}
	}

#endif
#ifdef TCSUPPORT_DNSEACHPVC
	restart_dnsmasq();
#endif
#ifdef WAN_DBUG
	tcdbg_printf("interface_state_change:get_wanindex_by_name:%d\n", if_index);
#endif
	/*Do we need check adsl link status firstly?*/
#if defined(TCSUPPORT_CT_DNSBIND)
	set_policy_route(top, if_name, if_index, if_state);
#endif

/* Add by mtk06404 support ecn sip compile*/
#if VOIP
#if defined(TCSUPPORT_ECN_SIP) || defined(TCSUPPORT_ECN_MEGACO)
#else
#ifdef TCSUPPORT_VOIP_IMSSIP
	send_wan_ip_2voip(1);
#endif
#endif
#endif

	switch(if_state){
		case BRIDGE_IF_STATE_UP:
			if (BRIDGE_IF_STATE_DOWN == g_wan_if[if_index].status_br) {
#if defined(TCSUPPORT_CT_QOS)
#if defined(TCSUPPORT_CT_HWQOS)
				tcapi_commit_internal(top,QOS);		//update QoS
#endif
				update_qos_vlan_rule(top, if_index);
#endif
#if defined(TCSUPPORT_CT)
			/* do something */
			#if 0//def TCSUPPORT_PORTBIND
				set_portbinding_info(top, if_name, if_index, BRIDGE_IF_STATE_UP);	
			#endif
#endif
			}
			g_wan_if[if_index].status_br = BRIDGE_IF_STATE_UP;
			break;
		case BRIDGE_IF_STATE_DOWN:
			if (BRIDGE_IF_STATE_UP == g_wan_if[if_index].status_br) {
#if defined(TCSUPPORT_CT)
			/* do something */
			#if 0//def TCSUPPORT_PORTBIND
				set_portbinding_info(top, if_name, if_index, BRIDGE_IF_STATE_DOWN);	
			#endif
#endif
			}
			g_wan_if[if_index].status_br = BRIDGE_IF_STATE_DOWN;
			break;
		case IF_STATE_UP:
			/*interface down to up*/
			if(g_wan_if[if_index].status == IF_STATE_DOWN){
#if defined(TCSUPPORT_CT_QOS)
#if defined(TCSUPPORT_CT_HWQOS)
				tcapi_commit_internal(top,QOS);		//update QoS
#endif
				update_qos_vlan_rule(top, if_index);
#endif
#ifdef WAN_DBUG
				tcdbg_printf("interface_state_change:%s from down to up\n", if_name);
#endif
				/*reload dmz rule and virtual server rule*/
				/* restart dmz based this wan interface */
				sprintf(nat_node, "Dmz_Entry%d", if_index);
				tcapi_commit_internal(top, nat_node);
				/* restart virtual based this wan interface */
				for(i = 0; i < MAX_VIRSERV_RULE; i++)
				{
					memset(nat_node, 0, sizeof(nat_node));
					sprintf(nat_node, "VirServer_Entry%d_Entry%d", if_index,i);
					tcapi_commit_internal(top, nat_node);
				}
				restart_nat(if_name);
				/*restart ddns*/
				restart_ddns();
#if defined(TCSUPPORT_IGMP_PROXY)
				/*restart igmpproxy*/
				restart_igmpproxy(if_state, if_name, if_index);
#endif
				check_route(if_name);
				#if 0
				/*reload the route entry with correct device*/
				check_route(pvc, isp);
				#endif
#if defined(TCSUPPORT_CT)
				/* set portbinding info */
			#ifdef TCSUPPORT_PORTBIND
#if defined(TCSUPPORT_CT_PPPOEPROXY) && !defined(TCSUPPORT_CT_PON_JS)
				/*ProxyEnable Judgement for updating portbinding ip tables*/
					if(atoi(proxyenable) == 1){
						set_blackhole_route(top, if_name, if_index);
					}
			      		else{
						set_portbinding_info(top, if_name, if_index, IF_STATE_UP);
			      		}
#else
				set_portbinding_info(top, if_name, if_index, IF_STATE_UP);	
#endif
			#endif

				/* set default route info */
				set_default_route(top, if_name, if_index, IF_STATE_UP);
			
#if defined(TCSUPPORT_CT_PHONEAPP)
				sendfifo2mobile(if_index);
#endif
			#ifdef CWMP
				/* update tr069 route config file */
				update_cwmp_route(top, if_name, if_index, IF_STATE_UP);
			
				/* set route for TR069 interface & remove nat info for TR069 interface */
				set_tr069_nat(top, if_name, if_index, IF_STATE_UP);

#if defined(TCSUPPORT_CT_MIDWARE)
				sendMsgtoCwmp(7,IF_STATE_UP,if_index);	//7 is wan config change
				//restart_ctadmin(top,if_index);
#else
				sendMsgtoCwmp(7);	//7 is wan config change
#endif

			#endif
#endif
			/*restart upnp deamon*/
#if defined(TCSUPPORT_SIMPLE_UPNP)
				//start_upnp(if_name);
				restart_upnp(top, if_name, if_index);
#endif
#if defined(TCSUPPORT_CT_IPPINGDIAGNOSTIC)
				recheckPingDiagnostic(top,if_index);
#endif
				restart_dhcprelay(top, if_index);
#if defined(TCSUPPORT_CT)
				resyncNTPServer(top, if_name, if_index, IF_STATE_UP);
#endif
				set_preService_rule(top, if_name, if_index, IF_STATE_UP);
#if defined(TCSUPPORT_CT_INFORM_NODE)
			if(srvFindFlag == SRV_TR069_FIND){
				memset(cwmpNode, 0, sizeof(cwmpNode));
				strcpy(cwmpNode[0], "Cwmp");
				strcpy(cwmpNode[1], "Entry");
				memset(tmpAttrStr, 0, sizeof(tmpAttrStr));
				if (getAttrValue(top, cwmpNode, "account_change_flag", tmpAttrStr) == TCAPI_PROCESS_OK) {
					if(strcmp(tmpAttrStr, "1") == 0){
						tcapi_commit_internal(top, "Cwmp_Entry");
					}
				}
#if defined(TCSUPPORT_CT_WAN_PTM) || defined(TCSUPPORT_CT_PON) || defined(TCSUPPORT_CT_E8B_ADSL)
#if defined(TCSUPPORT_CT_MIDWARE)
				sendMsgtoCwmp(2, if_state, if_index);
#else
				sendMsgtoCwmp(2);
#endif
#endif
			}
#endif
#if defined(TCSUPPORT_CT_VOIP_QOS)
			//setVoipWanIdx(top,if_name,if_state);
#endif
#if defined(TCSUPPORT_ECN_MEGACO)
				updateVoIP(top,if_name,if_index,if_state);
#endif
#if defined(TCSUPPORT_CT_PPPINFORM)
				memset(tmpAttrStr, 0, sizeof(tmpAttrStr));
				if (getAttrValue(top, wanNode, "ServiceList", tmpAttrStr) == TCAPI_PROCESS_OK) {
					if(strstr(tmpAttrStr, "INTERNET") != NULL && strstr(if_name, "ppp") != NULL){
						memset(tmpAttrStr, 0, sizeof(tmpAttrStr));
						if (getAttrValue(top, wanNode, "USERNAME", tmpAttrStr) == TCAPI_PROCESS_OK) {
							if(strcmp(g_wan_if[if_index].username, tmpAttrStr)) {
								sendMsgtoCwmp(18, if_state, if_index);
								memset(g_wan_if[if_index].username, 0, sizeof(g_wan_if[if_index].username));
								strcpy(g_wan_if[if_index].username, tmpAttrStr);
							}				
						}
					}
					else{
						if(srvFindFlag == SRV_TR069_FIND)
							sendMsgtoCwmp(18, if_state, if_index);
					}
				}
#endif
			}
			g_wan_if[if_index].status = IF_STATE_UP;
#if defined(TCSUPPORT_CT_JOYME) 
			wanIpChangeInform(top,if_index, if_state);
#endif
			break;
			
		case IF_STATE_DOWN:
			if(g_wan_if[if_index].status == IF_STATE_UP){
#ifdef WAN_DBUG
				tcdbg_printf("interface_state_change:%s from up to down\n", if_name);
#endif	
				#if 0
				#ifdef TCSUPPORT_DNSEACHPVC
				dnsmasq_restart_down(pvc);
				#endif
				#endif
#if defined(TCSUPPORT_CT)
			#ifdef TCSUPPORT_PORTBIND
#if defined(TCSUPPORT_CT_PPPOEPROXY) && !defined(TCSUPPORT_CT_PON_JS)
					if(atoi(proxyenable) == 1){
						set_blackhole_route(top, if_name, if_index);
					}
			      		else{
						set_portbinding_info(top, if_name, if_index, IF_STATE_DOWN);
			      		}
#else
				set_portbinding_info(top, if_name, if_index, IF_STATE_DOWN);	
#endif
			#endif
			#ifdef CWMP
#if defined(TCSUPPORT_CT_MIDWARE)
				sendMsgtoCwmp(7,IF_STATE_DOWN,if_index);	//7 is wan config change
#else
				sendMsgtoCwmp(7);	//7 is wan config change
#endif
			#endif
				/* update default route info */
				set_default_route(top, if_name, if_index, IF_STATE_DOWN);
			#ifdef CWMP
				/* update tr069 route config file */
				update_cwmp_route(top, if_name, if_index, IF_STATE_DOWN);
			#endif
				resyncNTPServer(top, if_name, if_index, IF_STATE_DOWN);
				set_preService_rule(top, if_name, if_index, IF_STATE_DOWN);
#endif
#if defined(TCSUPPORT_CT_VOIP_QOS)
				//setVoipWanIdx(top,if_name,if_state);
#endif
			
#if defined(TCSUPPORT_ECN_MEGACO)
					updateVoIP(top,if_name,if_index,if_state);
#endif
			}
			g_wan_if[if_index].status = IF_STATE_DOWN;
			
#if defined(TCSUPPORT_CT_JOYME) 
			wanIpChangeInform(top, if_index, if_state);
#endif
			break;
#ifdef IPV6			
		case IF_STATE_UP6:
			if(g_wan_if[if_index].status6 == IF_STATE_DOWN6){
#if defined(TCSUPPORT_CT_QOS)
#if defined(TCSUPPORT_CT_HWQOS)
				tcapi_commit_internal(top,QOS);		//update QoS
#endif
				update_qos_vlan_rule(top, if_index);
#endif
#ifdef WAN_DBUG
				tcdbg_printf("interface_state_change:%s from down to up IPV6\n", if_name);
#endif
#ifdef TCSUPPORT_MLD_PROXY
				/*restart mldproxy*/
				restart_mldproxy(if_state, if_name, if_index);
#endif
				check_static_route6(if_name, if_index);
#if defined(TCSUPPORT_CT)
			#ifdef TCSUPPORT_PORTBIND
#if defined(TCSUPPORT_CT_PPPOEPROXY) && !defined(TCSUPPORT_CT_PON_JS)
					if(atoi(proxyenable) == 1){
						set_blackhole_route(top, if_name, if_index);
					}
			      		else{
						set_portbinding_info(top, if_name, if_index, IF_STATE_UP6);
			      		}
#else
				set_portbinding_info(top, if_name, if_index, IF_STATE_UP6);	
#endif
			#endif
#if defined(TCSUPPORT_CT_PPP_ONDEMAND)
				delRoute6ViaOndemand(top, pvc_index, entry_index, if_name);
#endif
				set_default_route(top, if_name, if_index, IF_STATE_UP6);
#endif
				if ( check_pd_enable(if_index) || getDhcp6sDNSWanInterface() == if_index )
					restart_pd_function(0);

				if ( getRaWanPDInterface() == if_index )
					restart_pd_function(1);

				update_br0_pdaddress(if_index, IF_STATE_UP6);
			}
			g_wan_if[if_index].status6 = IF_STATE_UP6;
#if defined(TCSUPPORT_CT_JOYME) 
			wanIpChangeInform(top, if_index, if_state);
#endif
			break;

		case IF_STATE_DOWN6:
			if(g_wan_if[if_index].status6 == IF_STATE_UP6){
#ifdef WAN_DBUG
				tcdbg_printf("interface_state_change:%s from up to down IPV6\n", if_name);
#endif				
#if defined(TCSUPPORT_CT)
			#ifdef TCSUPPORT_PORTBIND
#if defined(TCSUPPORT_CT_PPPOEPROXY) && !defined(TCSUPPORT_CT_PON_JS)
					if(atoi(proxyenable) == 1){
						set_blackhole_route(top, if_name, if_index);
					}
			      		else{
						set_portbinding_info(top, if_name, if_index, IF_STATE_DOWN6);
			      		}
#else
				set_portbinding_info(top, if_name, if_index, IF_STATE_DOWN6);	
#endif
			#endif
				/* update default route info */
				set_default_route(top, if_name, if_index, IF_STATE_DOWN6);
				run_pd_index = get_run_pd_index();
				dns_wan_index = getDhcp6sDNSWanInterface();
				ra_pd_index = getRaWanPDInterface();
				if ( run_pd_index == if_index )
				{
					update_run_pd_index(if_index);
					restart_pd_function(0);
				}
				else if ( dns_wan_index == if_index )
					restart_pd_function(0);

				if ( ra_pd_index == if_index )
					restart_pd_function(1);

				update_br0_pdaddress(if_index, IF_STATE_DOWN6);

#endif
			}
			g_wan_if[if_index].status6 = IF_STATE_DOWN6;
#if defined(TCSUPPORT_CT_JOYME) 
			wanIpChangeInform(top, if_index, if_state);
#endif
			break;
#endif			
		default:
			tcdbg_printf("interface_state_change:unknown interface state type:%d\n", if_state);
			break;
		
	}
#if VOIP	
#if !defined(TCSUPPORT_ECN_SIP) && !defined(TCSUPPORT_ECN_MEGACO)
#ifdef VP_MEGACO
//voip_h248_wanStateChanged(top);
#else
#ifdef TCSUPPORT_VOIP_IMSSIP
send_wan_ip_2voip(1);
#endif
#endif
#endif
#endif
	return 0;
}

void wan_monitor_init()
{
	int i;
	for(i = 0; i < WAN_IF_INFO_NUM; i++){
		g_wan_if[i].status_br = BRIDGE_IF_STATE_DOWN;
		g_wan_if[i].status = IF_STATE_DOWN;
		g_wan_if[i].status6 = IF_STATE_DOWN6;
#if defined(TCSUPPORT_CT_PON_GD) || defined(TCSUPPORT_CT_PON_CQ)
		g_wan_if[i].dhcp_renew_flag = 1;
#else
		g_wan_if[i].dhcp_renew_flag = 0;
#endif
	}
}

#ifdef CWMP
/*send message to TR069*/
void
sendMsgtoCwmp(int cwmpMsgType
#if defined(TCSUPPORT_CT_MIDWARE)
, int if_state, int if_index 
#endif
)
{
	cwmp_msg_t message;
	long type = 1;	//tr69 must be 1
	int msgFlag = IPC_NOWAIT;//0;
					
	memset(&message,0,sizeof(cwmp_msg_t));
#if defined(TCSUPPORT_CT_MIDWARE)
	message.text.reserved[0] = if_state;
	message.text.reserved[1] = if_index;
#endif
	message.cwmptype = cwmpMsgType; 
	if(sendmegq(type, &message,msgFlag) < 0){
						
		tcdbg_printf("\r\nsend message to cwmp error!!");
	}		
}
#endif
