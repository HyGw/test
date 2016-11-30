/*	$KAME: dhcp6c_ia.c,v 1.33 2005/07/22 08:50:05 jinmei Exp $	*/

/*
 * Copyright (C) 2003 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <sys/types.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"
#include "timer.h"
#include "dhcp6c.h"
#include "dhcp6c_ia.h"
#include "prefixconf.h"
#include "addrconf.h"

#if !defined(TCSUPPORT_CT)
extern char info_path[256];
extern char pd_addr_info[128];
extern char pd_addr_info_org[128];
extern int pd_update_flag;
#endif
typedef enum {IAS_ACTIVE, IAS_RENEW, IAS_REBIND} iastate_t;

struct ia {
	TAILQ_ENTRY(ia) link;

	/* back pointer to configuration */
	struct ia_conf *conf;

	/* common parameters of IA */
	u_int32_t t1;		/* duration for renewal */
	u_int32_t t2;		/* duration for rebind  */

	/* internal parameters for renewal/rebinding */
	iastate_t state;
	struct dhcp6_timer *timer;
	struct dhcp6_eventdata *evdata;

	/* DHCP related parameters */
	struct dhcp6_if *ifp;	/* DHCP interface */
	struct duid serverid;	/* the server ID that provided this IA */

	/* control information shared with each particular config routine */
	struct iactl *ctl;

	/* authentication parameters for transaction with servers on this IA */
	struct authparam *authparam;
};

#ifdef TCSUPPORT_WPA_SUPPLICANT
extern char interfacename[32];
#endif
static int update_authparam __P((struct ia *, struct authparam *));
static void reestablish_ia __P((struct ia *));
static void callback __P((struct ia *));
static int release_ia __P((struct ia *));
static void remove_ia __P((struct ia *));
static struct ia *get_ia __P((iatype_t, struct dhcp6_if *, struct ia_conf *,
    struct dhcp6_listval *, struct duid *));
static struct ia *find_ia __P((struct ia_conf *, iatype_t, u_int32_t));
static struct dhcp6_timer *ia_timo __P((void *));

static char *iastr __P((iatype_t));
static char *statestr __P((iastate_t));
#ifdef TCSUPPORT_WPA_SUPPLICANT
void renew_handle(struct dhcp6_if *ifp)
{
	struct ia_conf *iac;
	struct ia *ia, *ia_next;
	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		for (ia = TAILQ_FIRST(&iac->iadata); ia; ia = ia_next) {
			ia_next = TAILQ_NEXT(ia, link);
			ia_handle(ia);
		}
	}
	return;
}

int
ia_handle(struct ia *current)
{
	struct ia *ia = (struct ia *)current;
	struct dhcp6_ia iaparam;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;
	struct timeval timo;
	int dhcpstate;

	dprintf(LOG_DEBUG, FNAME, "IA timeout for %s-%lu, state=%s",
	    iastr(ia->conf->type), ia->conf->iaid, statestr(ia->state));

	/* cancel the current event for the prefix. */
	if (ia->evdata) {
		TAILQ_REMOVE(&ia->evdata->event->data_list, ia->evdata, link);
		if (ia->evdata->destructor)
			ia->evdata->destructor(ia->evdata);
		free(ia->evdata);
		ia->evdata = NULL;
	}

	switch (ia->state) {
	case IAS_ACTIVE:
		ia->state = IAS_RENEW;
		dhcpstate = DHCP6S_RENEW;
		timo.tv_sec = ia->t1 < ia->t2 ? ia->t2 - ia->t1 : 0;
		timo.tv_usec = 0;
		dhcp6_set_timer(&timo, ia->timer);
		break;
	case IAS_RENEW:
		ia->state = IAS_REBIND;
		dhcpstate = DHCP6S_REBIND;

		/*
		 * We need keep DUID for sending Release in this state.
		 * But we don't need a timer for the IA.  We'll just wait for a
		 * reply for the REBIND until all associated configuration
		 * parameters for this IA expire.
		 */
		dhcp6_remove_timer(&ia->timer);
		break;
	default:
		dprintf(LOG_ERR, FNAME, "invalid IA state (%d)",
		    (int)ia->state);
		return -1;	/* XXX */
	}

	if ((ev = dhcp6_create_event(ia->ifp, dhcpstate)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	TAILQ_INSERT_TAIL(&ia->ifp->event_list, ev, link);

	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event timer");
		goto fail;
	}

	if ((evd = malloc(sizeof(*evd))) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event data");
		goto fail;
	}
	memset(evd, 0, sizeof(*evd));
	evd->event = ev;
	TAILQ_INSERT_TAIL(&ev->data_list, evd, link);

	if (ia->state == IAS_RENEW) {
		if (duidcpy(&ev->serverid, &ia->serverid)) {
			dprintf(LOG_NOTICE, FNAME, "failed to copy server ID");
			goto fail;
		}
	}

	iaparam.iaid = ia->conf->iaid;
	iaparam.t1 = ia->t1;
	iaparam.t2 = ia->t2;
	switch(ia->state) {
	case IAS_RENEW:
		if (ia->ctl && ia->ctl->renew_data) {
			if ((*ia->ctl->renew_data)(ia->ctl, &iaparam,
			    &ia->evdata, evd)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to make renew data");
				goto fail;
			}
		}
		break;
	case IAS_REBIND:
		if (ia->ctl && ia->ctl->rebind_data) {
			if ((*ia->ctl->rebind_data)(ia->ctl, &iaparam,
			    &ia->evdata, evd)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to make rebind data");
				goto fail;
			}
		}
		break;
	default:
		break;
	}

	ev->timeouts = 0;
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);

	if (ia->authparam != NULL) {
		if ((ev->authparam = copy_authparam(ia->authparam)) == NULL) {
			dprintf(LOG_WARNING, FNAME,
			    "failed to copy authparam");
			goto fail;
		}
	}

	ia->evdata = evd;

	switch(ia->state) {
	case IAS_RENEW:
	case IAS_REBIND:
		client6_send(ev);
		
		break;
	case IAS_ACTIVE:
		/* what to do? */
		break;
	}

	return 0;

  fail:
	if (ev)
		dhcp6_remove_event(ev);

	return -1;
}
#endif
#if !defined(TCSUPPORT_CT)
void update_pd(void)
{
	FILE *infofp = NULL;
	
	if(pd_update_flag != 0)
		return;
	if((infofp = fopen(info_path, "a")) != NULL){
		fprintf(infofp, "prefix-delegation %s\n", pd_addr_info);
		fclose(infofp);
	}	
	if((infofp = fopen(info_path, "a")) != NULL){
		fprintf(infofp, "org-prefix-delegation %s\n", pd_addr_info_org);
		fclose(infofp);
	}	
	dhcp_info_notify(info_path);
	return;
}

#endif

#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
/*******************************************************************************************
**function name
	store_parent_pd
**description:
	store pd prefix to proc file.
 **retrun 
	0:success
	-1:failure
**parameter:
	ifname: interface name
	pd_prefix: PD prefix
	pd_prefix_len: PD prefix length
********************************************************************************************/
int store_parent_pd(char *ifname, struct in6_addr * pd_prefix, int pd_prefix_len){
	FILE *fp = NULL;
	char path[128] = {0};
	char tempstr[64] = {0};
	int ret = -1;

	if(ifname == NULL || pd_prefix == NULL){
		goto end;
	}

	sprintf(path,"/proc/sys/net/ipv6/conf/%s/parent_pd_prefix",ifname);	
	fp = fopen(path, "w");
	if(fp == NULL){
		goto end;
	}
	sprintf(tempstr,"%s/%d",in6addr2str(pd_prefix, 0), pd_prefix_len);
	fputs(tempstr, fp);	

	ret = 0;
end:
	if(fp != NULL){
		fclose(fp);
	}
	return ret;

}


/*******************************************************************************************
**function name
	ipv6_gen_eui64
**description:
	generate ipv6 address for last 64 bits
 **retrun 
	0:success
	-1:failure
**parameter:
	eui:  last 64 bits pointer
	ifname: interface name
********************************************************************************************/	
int ipv6_gen_eui64(unsigned char *eui, char *ifname){
	int ret = -1, result = -1;
	FILE *fp = NULL;
	char tmp_str[64] = {0}, path[64]={0};
	char *tmp_p = NULL;
	unsigned int mac[6];

	if(eui == NULL || ifname == NULL){
		goto end;
	}
	memset(mac, 0, sizeof(mac));
	memset(tmp_str, 0, sizeof(tmp_str));
	
	/*read hwaddr*/
	sprintf(path, "/var/run/%s/child_prefix_hwaddr", ifname);

	if((fp=fopen(path, "r"))==NULL){
		goto end;
	}

	if(fgets(tmp_str, 18, fp) == NULL){
		goto end;
	}

	if(strlen(tmp_str) <= 0){
		goto end;
	}
	
	if (sscanf(tmp_str, "%02x:%02x:%02x:%02x:%02x:%02x",
		   &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) != 6){
		goto end;
	}

	eui[0] = mac[0];
	eui[0] ^= 2;
	eui[1] = mac[1];
	eui[2] = mac[2];
	eui[3] = 0xFF;
	eui[4] = 0xFE;
	eui[5] = mac[3];
	eui[6] = mac[4];
	eui[7] = mac[5];
	
	result = 0;
end:
	
	if(fp != NULL){
		fclose(fp);
	}

	return result;
}

/*******************************************************************************************
**function name
	gen_addr_by_pd
**description:
	according the parent PD and childprefixbits in tr069, generate new global address.
 **retrun 
	0:success
	-1:failure
**parameter:
	pd_prefix: PD prefix
	pd_prefix_len: PD prefix length
	ifname: interface name
	result_addr: result ipv6 addr
	result_mask: result ipv6 mask
********************************************************************************************/
int gen_addr_by_pd(struct in6_addr pd_prefix, int pd_prefix_len, char* ifname, struct in6_addr * result_addr, int * result_mask){
	int ret = -1, result = -1;
	int  child_prefix_orign = 0;
	int child_prefix_len = 0;
	struct in6_addr child_prefix;
	struct in6_addr parent_prefix;	
	int in6_addr_len = sizeof(struct in6_addr);
	char tmp_str[64] = {0}, path[64]={0};
	char * tmp_p = NULL;
	char * tmp2_p = NULL;
	char tmpLen_str[40] = {0};
	int str_len = 0;
	int fd;
	int	temp_u8 = 0;
	int start_mask = 0, end_mask = 0, mask_range = 0;	
	int i = 0, j = 0, k = 0;

	if(ifname == NULL || result_addr == NULL){
		goto end;
	}

	if(pd_prefix_len >= MAX_PD_PREFIX){			
		goto end;
	}
	memset(&(child_prefix), 0, in6_addr_len);
	str_len = sizeof(tmp_str);

	/*read child prefix orgin*/
	sprintf(path, "/proc/sys/net/ipv6/conf/%s/child_prefix_orign", ifname);

	fd=open(path, O_RDONLY);
	if(fd == -1){		
		goto end;
	}
	memset(tmp_str, 0, str_len);
	ret = read(fd, tmp_str, 16);
	close(fd);
	child_prefix_orign = atoi(tmp_str);	

	if(child_prefix_orign != PREFIX_ORIGN_DHCP){		
		goto end;
	}

	/*read child prefix*/
	sprintf(path, "/proc/sys/net/ipv6/conf/%s/child_prefix", ifname);
	fd=open(path, O_RDONLY);
	if(fd == -1){		
		goto end;
	}
	memset(tmp_str, 0, str_len);
	ret = read(fd, tmp_str, 63);
	close(fd);
	if(strlen(tmp_str) == 0){		
		goto end;
	}

	/*get child prefix len*/
	tmp_p = strstr(tmp_str, "/");
	if(tmp_p == NULL){		
		goto end;
	}
	
	tmp2_p = tmp_p+1;
	child_prefix_len = atoi(tmp2_p);

	/*update child prefix string and get the real child prefix*/
	*tmp_p = '\0';
	
	if (inet_pton(AF_INET6, tmp_str, &child_prefix) != 1) {		
		goto end;
	}
	memcpy(&parent_prefix, &pd_prefix,in6_addr_len);
	start_mask = pd_prefix_len;
	end_mask = child_prefix_len;
	mask_range = end_mask - start_mask;
	
	if(mask_range <= 0 ){
		if(mask_range == 0){
			memcpy(result_addr, &(parent_prefix),in6_addr_len);
			*result_mask = pd_prefix_len;
			result = 0;
		}
		goto end;
	}
	/*calculate new prefix based on childprefix and parentprefix*/
	for(i=start_mask; i<end_mask; i++){
		j = i/8;
		k = i%8;
		temp_u8 = (1<<k);
		temp_u8 &= child_prefix.s6_addr[j];
		parent_prefix.s6_addr[j] |= temp_u8;
	}

	memcpy(result_addr, &(parent_prefix),in6_addr_len);
	*result_mask = child_prefix_len;
	
	result = 0;
end:
	if(result == 0){
		if(*result_mask>64){
			*result_mask = 64;
		}
	}
	return result;
}
#endif

void
update_ia(iatype, ialist, ifp, serverid, authparam)
	iatype_t iatype;
	struct dhcp6_list *ialist;
	struct dhcp6_if *ifp;
	struct duid *serverid;
	struct authparam *authparam;
{
	struct ia *ia;
	struct ia_conf *iac;
	struct iapd_conf *iapdc;
	struct iana_conf *ianac;
	struct dhcp6_listval *iav, *siav;
	struct timeval timo;

#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
	struct dhcp6_statefuladdr new_addr;
	int new_mask = 0;
	struct in6_addr prefix_addr;
	int prefix_len = 0;		
	struct ia_conf * new_iac = NULL;
#endif

	for (iav = TAILQ_FIRST(ialist); iav; iav = TAILQ_NEXT(iav, link)) {
		/* if we're not interested in this IA, ignore it. */
		if ((iac = find_iaconf(&ifp->iaconf_list, iatype,
		    iav->val_ia.iaid)) == NULL) {
			continue;
		}

		/* validate parameters */
		/*
		 * If a client receives an IA_NA with T1 greater than T2, and
		 * both T1 and T2 are greater than 0, the client discards the
		 * IA_NA option and processes the remainder of the message as
		 * though the server had not included the invalid IA_NA option.
		 * [RFC3315 22.4]
		 * We apply the same rule to IA_PD as well.
		 */
		if (iav->val_ia.t2 != 0 && iav->val_ia.t1 > iav->val_ia.t2) {
			dprintf(LOG_INFO, FNAME,
			    "invalid IA: T1(%lu) > T2(%lu)",
			    iav->val_ia.t1, iav->val_ia.t2);
			continue;
		}

		/* locate the local IA or make a new one */
		ia = get_ia(iatype, ifp, iac, iav, serverid);
		if (ia == NULL) {
			dprintf(LOG_WARNING, FNAME, "failed to get an IA "
			    "type: %s, ID: %u", iastr(iac->type), iac->iaid);
			continue;
		}

		/* update authentication parameters */
		if (update_authparam(ia, authparam)) {
			dprintf(LOG_WARNING, FNAME, "failed to update "
			    "authentication param for IA "
			    "type: %s, ID: %u", iastr(iac->type), iac->iaid);
			remove_ia(ia);
			continue;
		}

		/* update IA configuration information */
		for (siav = TAILQ_FIRST(&iav->sublist); siav;
		    siav = TAILQ_NEXT(siav, link)) {
			switch (siav->type) {
			case DHCP6_LISTVAL_PREFIX6:
				/* add or update the prefix */
#if !defined(TCSUPPORT_CT)
				pd_update_flag =1;
#endif
				iapdc = (struct iapd_conf *)iac;
				if (update_prefix(ia, &siav->val_prefix6,
				    &iapdc->iapd_pif_list, ifp, &ia->ctl,
				    callback)) {
					dprintf(LOG_NOTICE, FNAME,
					    "failed to update a prefix %s/%d",
					    in6addr2str(&siav->val_prefix6.addr, 0),
					    siav->val_prefix6.plen);
				}
#if defined(TCSUPPORT_CT_WAN_CHILD_PREFIX)
				/*store PD prefix to file proc*/					
				store_parent_pd(ifp->ifname, &siav->val_prefix6.addr,siav->val_prefix6.plen);

				/* find NA_IA type*/
				if ((new_iac = find_iaconf(&ifp->iaconf_list, IATYPE_NA, IATYPE_NA_IA_ID)) == NULL)
				{				
					
					/*init new_addr*/
					memset(&new_addr, 0, sizeof(struct dhcp6_statefuladdr));
					memcpy(&prefix_addr, &siav->val_prefix6.addr, sizeof(struct in6_addr));
					prefix_len = siav->val_prefix6.plen;
					
					/*general new global address*/
					if(0 == gen_addr_by_pd(prefix_addr,prefix_len,ifp->ifname,&new_addr.addr, &new_mask)){
						new_addr.pltime = siav->val_prefix6.pltime;
						new_addr.vltime = siav->val_prefix6.vltime;
						if(ipv6_gen_eui64(new_addr.addr.s6_addr + 8, ifp->ifname) == 0){
							if (update_address(ia, &new_addr,
							    ifp, &ia->ctl, callback)) {
								// generate new addr failure
							}
						}
					}
				}
#endif
				break;
			case DHCP6_LISTVAL_STATEFULADDR6:
				ianac = (struct iana_conf *)iac;
				if (update_address(ia, &siav->val_statefuladdr6,
				    ifp, &ia->ctl, callback)) {
					dprintf(LOG_NOTICE, FNAME,
					    "failed to update an address %s",
					    in6addr2str(&siav->val_statefuladdr6.addr, 0));
				}
				break;
			case DHCP6_LISTVAL_STCODE:
				dprintf(LOG_INFO, FNAME,
				    "status code for %s-%lu: %s",
				    iastr(iatype), iav->val_ia.iaid,
				    dhcp6_stcodestr(siav->val_num16));
#if defined(TCSUPPORT_CT_DHCP6C_STATUS_CODE)
					if(siav->val_num16 == DH6OPT_STCODE_NOPREFIXAVAIL
						|| siav->val_num16 == DH6OPT_STCODE_NOADDRSAVAIL){
						update_dhcp6c_err_type(ifp,siav->val_num16);
					}					
#endif


				if ((ia->state == IAS_RENEW ||
				    ia->state == IAS_REBIND) &&
				    siav->val_num16 == DH6OPT_STCODE_NOBINDING) {
					/*
					 * For each IA in the original Renew or
					 * Rebind message, the client
					 * sends a Request message if the IA
					 * contained a Status Code option
					 * with the NoBinding status.
					 * [RFC3315 18.1.8]
					 * XXX: what about the PD case?
					 */
					dprintf(LOG_INFO, FNAME,
					    "receive NoBinding against "
					    "renew/rebind for %s-%lu",
					    iastr(ia->conf->type),
					    ia->conf->iaid);
					reestablish_ia(ia);
					goto nextia;
				}
				break;
			default:
				dprintf(LOG_ERR, FNAME, "impossible case");
				goto nextia;
			}
		}
#if !defined(TCSUPPORT_CT)
		update_pd();
#endif

		/* see if this IA is still valid.  if not, remove it. */
		if (ia->ctl == NULL || !(*ia->ctl->isvalid)(ia->ctl)) {
			dprintf(LOG_DEBUG, FNAME, "IA %s-%lu is invalidated",
			    iastr(ia->conf->type), ia->conf->iaid);
			remove_ia(ia);
			continue;
		}

		/* if T1 or T2 is 0, determine appropriate values locally. */
		if (ia->t1 == 0 || ia->t2 == 0) {
			u_int32_t duration;

			if (ia->ctl && ia->ctl->duration)
				duration = (*ia->ctl->duration)(ia->ctl);
			else
				duration = 1800; /* 30min. XXX: no rationale */

			if (ia->t1 == 0) {
				if (duration == DHCP6_DURATION_INFINITE)
					ia->t1 = DHCP6_DURATION_INFINITE;
				else
					ia->t1 = duration / 2;
			}
			if (ia->t2 == 0) {
				if (duration == DHCP6_DURATION_INFINITE)
					ia->t2 = DHCP6_DURATION_INFINITE;
				else
					ia->t2 = duration * 4 / 5;
			}

			/* make sure T1 <= T2 */
			if (ia->t1 > ia->t2)
				ia->t1 = ia->t2 * 5 / 8;

			dprintf(LOG_INFO, FNAME, "T1(%lu) and/or T2(%lu) "
			    "is locally determined",  ia->t1, ia->t2);
		}

		/*
		 * Be proactive for too-small timeout values.  Note that
		 * the adjusted values may make some information expire
		 * without renewal.
		 */
		if (ia->t2 < DHCP6_DURATION_MIN) {
			dprintf(LOG_INFO, FNAME, "T1 (%lu) or T2 (%lu) "
			    "is too small", ia->t1, ia->t2);
			ia->t2 = DHCP6_DURATION_MIN;
			ia->t1 = ia->t2 * 5 / 8;
			dprintf(LOG_INFO, "", "  adjusted to %lu and %lu",
			    ia->t1, ia->t2);
		}

		/* set up a timer for this IA. */
		if (ia->t1 == DHCP6_DURATION_INFINITE) {
			if (ia->timer)
				dhcp6_remove_timer(&ia->timer);
		} else {
			if (ia->timer == NULL)
				ia->timer = dhcp6_add_timer(ia_timo, ia);
			if (ia->timer == NULL) {
				dprintf(LOG_ERR, FNAME,
				    "failed to add IA timer");
				remove_ia(ia); /* XXX */
				continue;
			}
			timo.tv_sec = ia->t1;
			timo.tv_usec = 0;
			dhcp6_set_timer(&timo, ia->timer);
		}

		ia->state = IAS_ACTIVE;

	  nextia:
		;
	}
}

static int
update_authparam(ia, authparam)
	struct ia *ia;
	struct authparam *authparam;
{
	if (authparam == NULL)
		return (0);

	if (ia->authparam == NULL) {
		if ((ia->authparam = copy_authparam(authparam)) == NULL) {
			dprintf(LOG_WARNING, FNAME,
			    "failed to copy authparam");
			return (-1);
		}
		return (0);
	}

	/* update the previous RD value and flags */
	ia->authparam->prevrd = authparam->prevrd;
	ia->authparam->flags = authparam->flags;

	return (0);
}

static void
reestablish_ia(ia)
	struct ia *ia;
{
	struct dhcp6_ia iaparam;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;

	dprintf(LOG_DEBUG, FNAME, "re-establishing IA: %s-%lu", 
	    iastr(ia->conf->type), ia->conf->iaid);

	if (ia->state != IAS_RENEW && ia->state != IAS_REBIND) {
		dprintf(LOG_ERR, FNAME, "internal error (invalid IA status)");
		exit(1);	/* XXX */
	}

	/* cancel the current event for the prefix. */
	if (ia->evdata) {
		TAILQ_REMOVE(&ia->evdata->event->data_list, ia->evdata, link);
		if (ia->evdata->destructor)
			ia->evdata->destructor(ia->evdata);
		free(ia->evdata);
		ia->evdata = NULL;
	}

	/* we don't need a timer for the IA (see comments in ia_timo()) */
	if (ia->timer)
		dhcp6_remove_timer(&ia->timer);

	if ((ev = dhcp6_create_event(ia->ifp, DHCP6S_REQUEST)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	TAILQ_INSERT_TAIL(&ia->ifp->event_list, ev, link);

	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event timer");
		goto fail;
	}

	if ((evd = malloc(sizeof(*evd))) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event data");
		goto fail;
	}
	memset(evd, 0, sizeof(*evd));
	evd->event = ev;
	TAILQ_INSERT_TAIL(&ev->data_list, evd, link);

	if (duidcpy(&ev->serverid, &ia->serverid)) {
		dprintf(LOG_NOTICE, FNAME, "failed to copy server ID");
		goto fail;
	}

	iaparam.iaid = ia->conf->iaid;
	iaparam.t1 = ia->t1;
	iaparam.t2 = ia->t2;

	if (ia->ctl && ia->ctl->reestablish_data) {
		if ((*ia->ctl->reestablish_data)(ia->ctl, &iaparam,
		    &ia->evdata, evd)) {
			dprintf(LOG_NOTICE, FNAME,
			    "failed to make reestablish data");
			goto fail;
		}
	}

	if (ia->authparam != NULL) {
		if ((ev->authparam = copy_authparam(ia->authparam)) == NULL) {
			dprintf(LOG_WARNING, FNAME,
			    "failed to copy authparam");
			goto fail;
		}
	}

	ev->timeouts = 0;
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);

	ia->evdata = evd;

	client6_send(ev);

	return;

  fail:
	if (ev)
		dhcp6_remove_event(ev);

	return;
}

static void
callback(ia)
	struct ia *ia;
{
	/* see if this IA is still valid.  if not, remove it. */
	if (ia->ctl == NULL || !(*ia->ctl->isvalid)(ia->ctl)) {
		dprintf(LOG_DEBUG, FNAME, "IA %s-%lu is invalidated",
		    iastr(ia->conf->type), ia->conf->iaid);
		remove_ia(ia);
	}
}

void
release_all_ia(ifp)
	struct dhcp6_if *ifp;
{
	struct ia_conf *iac;
	struct ia *ia, *ia_next;

	for (iac = TAILQ_FIRST(&ifp->iaconf_list); iac;
	    iac = TAILQ_NEXT(iac, link)) {
		for (ia = TAILQ_FIRST(&iac->iadata); ia; ia = ia_next) {
			ia_next = TAILQ_NEXT(ia, link);

			(void)release_ia(ia);

			/*
			 * The client MUST stop using all of the addresses
			 * being released as soon as the client begins the
			 * Release message exchange process.
			 * [RFC3315 Section 18.1.6]
			 */
			remove_ia(ia);
		}
	}
}

static int
release_ia(ia)
	struct ia *ia;
{
	struct dhcp6_ia iaparam;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;

	dprintf(LOG_DEBUG, FNAME, "release an IA: %s-%lu",
	    iastr(ia->conf->type), ia->conf->iaid);

	if ((ev = dhcp6_create_event(ia->ifp, DHCP6S_RELEASE))
	    == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	TAILQ_INSERT_TAIL(&ia->ifp->event_list, ev, link);


	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event timer");
		goto fail;
	}

	if (duidcpy(&ev->serverid, &ia->serverid)) {
		dprintf(LOG_NOTICE, FNAME, "failed to copy server ID");
		goto fail;
	}

	if ((evd = malloc(sizeof(*evd))) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event data");
		goto fail;
	}
	memset(evd, 0, sizeof(*evd));
	iaparam.iaid = ia->conf->iaid;
	/* XXX: should we set T1/T2 to 0?  spec is silent on this. */
	iaparam.t1 = ia->t1;
	iaparam.t2 = ia->t2;

	if (ia->ctl && ia->ctl->release_data) {
		if ((*ia->ctl->release_data)(ia->ctl, &iaparam, NULL, evd)) {
			dprintf(LOG_NOTICE, FNAME,
			    "failed to make release data");
			goto fail;
		}
	}
	TAILQ_INSERT_TAIL(&ev->data_list, evd, link);

	ev->timeouts = 0;
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);

	if (ia->authparam != NULL) {
		if ((ev->authparam = copy_authparam(ia->authparam)) == NULL) {
			dprintf(LOG_WARNING, FNAME,
			    "failed to copy authparam");
			goto fail;
		}
	}

#if defined(TCSUPPORT_CT)
#if defined(TCSUPPORT_CT_PON)
	if(is_pon_link_up())
#else
	if(is_adsl_link_up())
#endif
		client6_send(ev);
	else
		goto fail;
#else
	client6_send(ev);
#endif

	return (0);

  fail:
	if (ev)
		dhcp6_remove_event(ev);

	return (-1);
}

static void
remove_ia(ia)
	struct ia *ia;
{
	struct ia_conf *iac = ia->conf;
	struct dhcp6_if *ifp = ia->ifp;

	dprintf(LOG_DEBUG, FNAME, "remove an IA: %s-%lu",
	    iastr(ia->conf->type), ia->conf->iaid);

	TAILQ_REMOVE(&iac->iadata, ia, link);

	duidfree(&ia->serverid);

	if (ia->timer)
		dhcp6_remove_timer(&ia->timer);

	if (ia->evdata) {
		TAILQ_REMOVE(&ia->evdata->event->data_list, ia->evdata, link);
		if (ia->evdata->destructor)
			ia->evdata->destructor(ia->evdata);
		free(ia->evdata);
		ia->evdata = NULL;
	}

	if (ia->ctl && ia->ctl->cleanup)
		(*ia->ctl->cleanup)(ia->ctl);

	if (ia->authparam != NULL)
		free(ia->authparam);

	free(ia);

	(void)client6_start(ifp);
}

static struct dhcp6_timer *
ia_timo(arg)
	void *arg;
{
	struct ia *ia = (struct ia *)arg;
	struct dhcp6_ia iaparam;
	struct dhcp6_event *ev;
	struct dhcp6_eventdata *evd;
	struct timeval timo;
	int dhcpstate;

	dprintf(LOG_DEBUG, FNAME, "IA timeout for %s-%lu, state=%s",
	    iastr(ia->conf->type), ia->conf->iaid, statestr(ia->state));

	/* cancel the current event for the prefix. */
	if (ia->evdata) {
		TAILQ_REMOVE(&ia->evdata->event->data_list, ia->evdata, link);
		if (ia->evdata->destructor)
			ia->evdata->destructor(ia->evdata);
		free(ia->evdata);
		ia->evdata = NULL;
	}

	switch (ia->state) {
	case IAS_ACTIVE:
		ia->state = IAS_RENEW;
		dhcpstate = DHCP6S_RENEW;
		timo.tv_sec = ia->t1 < ia->t2 ? ia->t2 - ia->t1 : 0;
		timo.tv_usec = 0;
		dhcp6_set_timer(&timo, ia->timer);
		break;
	case IAS_RENEW:
		ia->state = IAS_REBIND;
		dhcpstate = DHCP6S_REBIND;

		/*
		 * We need keep DUID for sending Release in this state.
		 * But we don't need a timer for the IA.  We'll just wait for a
		 * reply for the REBIND until all associated configuration
		 * parameters for this IA expire.
		 */
		dhcp6_remove_timer(&ia->timer);
		break;
	default:
		dprintf(LOG_ERR, FNAME, "invalid IA state (%d)",
		    (int)ia->state);
		return (NULL);	/* XXX */
	}

	if ((ev = dhcp6_create_event(ia->ifp, dhcpstate)) == NULL) {
		dprintf(LOG_NOTICE, FNAME, "failed to create a new event");
		goto fail;
	}
	TAILQ_INSERT_TAIL(&ia->ifp->event_list, ev, link);

	if ((ev->timer = dhcp6_add_timer(client6_timo, ev)) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event timer");
		goto fail;
	}

	if ((evd = malloc(sizeof(*evd))) == NULL) {
		dprintf(LOG_NOTICE, FNAME,
		    "failed to create a new event data");
		goto fail;
	}
	memset(evd, 0, sizeof(*evd));
	evd->event = ev;
	TAILQ_INSERT_TAIL(&ev->data_list, evd, link);

	if (ia->state == IAS_RENEW) {
		if (duidcpy(&ev->serverid, &ia->serverid)) {
			dprintf(LOG_NOTICE, FNAME, "failed to copy server ID");
			goto fail;
		}
	}

	iaparam.iaid = ia->conf->iaid;
	iaparam.t1 = ia->t1;
	iaparam.t2 = ia->t2;
	switch(ia->state) {
	case IAS_RENEW:
		if (ia->ctl && ia->ctl->renew_data) {
			if ((*ia->ctl->renew_data)(ia->ctl, &iaparam,
			    &ia->evdata, evd)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to make renew data");
				goto fail;
			}
		}
		break;
	case IAS_REBIND:
		if (ia->ctl && ia->ctl->rebind_data) {
			if ((*ia->ctl->rebind_data)(ia->ctl, &iaparam,
			    &ia->evdata, evd)) {
				dprintf(LOG_NOTICE, FNAME,
				    "failed to make rebind data");
				goto fail;
			}
		}
		break;
	default:
		break;
	}

	ev->timeouts = 0;
	dhcp6_set_timeoparam(ev);
	dhcp6_reset_timer(ev);

	if (ia->authparam != NULL) {
		if ((ev->authparam = copy_authparam(ia->authparam)) == NULL) {
			dprintf(LOG_WARNING, FNAME,
			    "failed to copy authparam");
			goto fail;
		}
	}

	ia->evdata = evd;

	switch(ia->state) {
	case IAS_RENEW:
	case IAS_REBIND:
		client6_send(ev);
		break;
	case IAS_ACTIVE:
		/* what to do? */
		break;
	}

	return (ia->timer);

  fail:
	if (ev)
		dhcp6_remove_event(ev);

	return (NULL);
}

static struct ia *
get_ia(type, ifp, iac, iaparam, serverid)
	iatype_t type;
	struct dhcp6_if *ifp;
	struct ia_conf *iac;
	struct dhcp6_listval *iaparam;
	struct duid *serverid;
{
	struct ia *ia;
	struct duid newserver;
	int create = 0;

	if (duidcpy(&newserver, serverid)) {
		dprintf(LOG_NOTICE, FNAME, "failed to copy server ID");
		return (NULL);
	}

	if ((ia = find_ia(iac, type, iaparam->val_ia.iaid)) == NULL) {
		if ((ia = malloc(sizeof(*ia))) == NULL) {
			dprintf(LOG_NOTICE, FNAME, "memory allocation failed");
			duidfree(&newserver); /* XXX */
			return (NULL);
		}
		memset(ia, 0, sizeof(*ia));
		ia->state = IAS_ACTIVE;

		TAILQ_INSERT_TAIL(&iac->iadata, ia, link);
		ia->conf = iac;

		create = 1;
	} else
		duidfree(&ia->serverid);

	ia->t1 = iaparam->val_ia.t1;
	ia->t2 = iaparam->val_ia.t2;
	ia->ifp = ifp;
	ia->serverid = newserver;

	dprintf(LOG_DEBUG, FNAME, "%s an IA: %s-%lu",
	    create ? "make" : "update", iastr(type), ia->conf->iaid);

	return (ia);
}

static struct ia *
find_ia(iac, type, iaid)
	struct ia_conf *iac;
	iatype_t type;
	u_int32_t iaid;
{
	struct ia *ia;

	for (ia = TAILQ_FIRST(&iac->iadata); ia;
	    ia = TAILQ_NEXT(ia, link)) {
		if (ia->conf->type == type && ia->conf->iaid == iaid)
			return (ia);
	}

	return (NULL);
}

static char *
iastr(type)
	iatype_t type;
{
	switch (type) {
	case IATYPE_PD:
		return ("PD");
	case IATYPE_NA:
		return ("NA");
	default:
		return ("???");	/* should be a bug */
	}
}

static char *
statestr(state)
	iastate_t state;
{
	switch (state) {
	case IAS_ACTIVE:
		return "ACTIVE";
	case IAS_RENEW:
		return "RENEW";
	case IAS_REBIND:
		return "REBIND";
	default:
		return "???";	/* should be a bug */
	}
}
