/*
**  igmpproxy - IGMP proxy based multicast router 
**  Copyright (C) 2005 Johnny Egeland <johnny@rlo.org>
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**----------------------------------------------------------------------------
**
**  This software is derived work from the following software. The original
**  source code has been modified from it's original state by the author
**  of igmpproxy.
**
**  smcroute 0.92 - Copyright (C) 2001 Carsten Schill <carsten@cschill.de>
**  - Licensed under the GNU General Public License, version 2
**  
**  mrouted 3.9-beta3 - COPYRIGHT 1989 by The Board of Trustees of 
**  Leland Stanford Junior University.
**  - Original license can be found in the "doc/mrouted-LINCESE" file.
**
*/
/**
*   rttable.c 
*
*   Updates the routingtable according to 
*     recieved request.
*/

#include "defs.h"
#ifdef TCSUPPORT_IGMP_QUICK_LEAVE
#include <errno.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <linux/sockios.h>
#include <linux/if_bridge.h>
//#ifdef TCSUPPORT_IGMP_SNOOPING_V3
//#include <linux/in.h>
//#endif
#define SIOCDEVPRIVATE	0x89F0
#define BRCTL_GET_BRIDGE_INFO 6
#define BRCTL_GET_MC_FDB_ENTRIES 21
#define CHUNK 128
#endif


/**
*   Routing table structure definition. Double linked list...
*/
struct RouteTable {
    struct RouteTable   *nextroute;     // Pointer to the next group in line.
    struct RouteTable   *prevroute;     // Pointer to the previous group in line.
    uint32              group;          // The group to route
    uint32              originAddr;     // The origin adress (only set on activated routes)
    uint32              vifBits;        // Bits representing recieving VIFs.

    // Keeps the upstream membership state...
    short               upstrState;     // Upstream membership state.

    // These parameters contain aging details.
    uint32              ageVifBits;     // Bits representing aging VIFs.
    int                 ageValue;       // Downcounter for death.          
    int                 ageActivity;    // Records any acitivity that notes there are still listeners.
#ifdef TCSUPPORT_IGMP_QOS
	/* skb mark */
	uint32				skb_mark;
#endif
};
#ifdef TCSUPPORT_IGMP_QUICK_LEAVE
struct mc_fdb_entry
{
	u_int8_t group_addr[16];
	u_int8_t host_addr[6];
	unsigned char is_router;
	u_int16_t port_no;
#ifdef TCSUPPORT_IGMP_SNOOPING_V3
	u_int8_t src_addr[16];
	u_int8_t filter_mode;
#endif
	struct timeval ageing_timer_value;
};

int checkLastMemberInGroup(uint32 group);
int br_read_mc_fdb(const char *bridge, struct mc_fdb_entry *fdbs, unsigned long offset, int num);
static  void copy_mc_fdb(struct mc_fdb_entry *ent, const struct __mc_fdb_entry *f);
static void jiffies_to_tv(struct timeval *tv, unsigned long jiffies);
void getsnoopingfastleaveFlag(const char *bridge, int *snoopingFastLeaveFlag,int *igmpSnoopingFlag);
#endif
                 
// Keeper for the routing table...
static struct RouteTable   *routing_table;

// Prototypes
void logRouteTable(char *header);
int  internAgeRoute(struct RouteTable*  croute);

// Socket for sending join or leave requests.
int mcGroupSock = 0;


/**
*   Function for retrieving the Multicast Group socket.
*/
int getMcGroupSock() {
    if( ! mcGroupSock ) {
        mcGroupSock = openUdpSocket( INADDR_ANY, 0 );;
    }
    return mcGroupSock;
}
 
/**
*   Initializes the routing table.
*/
void initRouteTable() {
    unsigned Ix;
    struct IfDesc *Dp;

    // Clear routing table...
    routing_table = NULL;

    // Join the all routers group on downstream vifs...
    for ( Ix = 0; Dp = getIfByIx( Ix ); Ix++ ) {
        // If this is a downstream vif, we should join the All routers group...
        if( Dp->InAdr.s_addr && ! (Dp->Flags & IFF_LOOPBACK) && Dp->state == IF_STATE_DOWNSTREAM) {
            IF_DEBUG log(LOG_DEBUG, 0, "Joining all-routers group %s on vif %s",
                         inetFmt(allrouters_group,s1),inetFmt(Dp->InAdr.s_addr,s2));
            
            //k_join(allrouters_group, Dp->InAdr.s_addr);
            joinMcGroup( getMcGroupSock(), Dp, allrouters_group );
        }
    }
}

/**
*   Internal function to send join or leave requests for
*   a specified route upstream...
*/
void sendJoinLeaveUpstream(struct RouteTable* route, int join) {
    struct IfDesc*      upstrIf;
    
    // Get the upstream VIF...
    upstrIf = getIfByIx( upStreamVif );
    if(upstrIf == NULL) {
        log(LOG_ERR, 0 ,"FATAL: Unable to get Upstream IF.");
    }
    /*
    IF_DEBUG {
        log(LOG_DEBUG, 0, "Upstream IF addr  : %s", inetFmt(upstrIf->InAdr.s_addr,s1));
        log(LOG_DEBUG, 0, "Upstream IF state : %d", upstrIf->state);
        log(LOG_DEBUG, 0, "Upstream IF index : %d", upstrIf->index);
    }*/

    // Send join or leave request...
    if(join) {

        // Only join a group if there are listeners downstream...
        if(route->vifBits > 0) {
            IF_DEBUG log(LOG_DEBUG, 0, "Joining group %s upstream on IF address %s",
                         inetFmt(route->group, s1), 
                         inetFmt(upstrIf->InAdr.s_addr, s2));

            //k_join(route->group, upstrIf->InAdr.s_addr);
            joinMcGroup( getMcGroupSock(), upstrIf, route->group );

            route->upstrState = ROUTESTATE_JOINED;
        } else IF_DEBUG {
            log(LOG_DEBUG, 0, "No downstream listeners for group %s. No join sent.",
                inetFmt(route->group, s1));
        }

    } else {
        // Only leave if group is not left already...
        if(route->upstrState != ROUTESTATE_NOTJOINED) {
            IF_DEBUG log(LOG_DEBUG, 0, "Leaving group %s upstream on IF address %s",
                         inetFmt(route->group, s1), 
                         inetFmt(upstrIf->InAdr.s_addr, s2));
            
            //k_leave(route->group, upstrIf->InAdr.s_addr);
            leaveMcGroup( getMcGroupSock(), upstrIf, route->group );

            route->upstrState = ROUTESTATE_NOTJOINED;
        }
    }
}

/**
*   Clear all routes from routing table, and alerts Leaves upstream.
*/
void clearAllRoutes() {
    struct RouteTable   *croute, *remainroute;

    // Loop through all routes...
    for(croute = routing_table; croute; croute = remainroute) {

        remainroute = croute->nextroute;

        // Log the cleanup in debugmode...
        IF_DEBUG log(LOG_DEBUG, 0, "Removing route entry for %s",
                     inetFmt(croute->group, s1));

        // Uninstall current route
        if(!internUpdateKernelRoute(croute, 0)) {
            log(LOG_WARNING, 0, "The removal from Kernel failed.");
        }

        // Send Leave message upstream.
        sendJoinLeaveUpstream(croute, 0);

        // Clear memory, and set pointer to next route...
        free(croute);
    }
    routing_table = NULL;

    // Send a notice that the routing table is empty...
    log(LOG_NOTICE, 0, "All routes removed. Routing table is empty.");
}
                 
/**
*   Private access function to find a route from a given 
*   Route Descriptor.
*/
struct RouteTable *findRoute(uint32 group) {
    struct RouteTable*  croute;

    for(croute = routing_table; croute; croute = croute->nextroute) {
        if(croute->group == group) {
            return croute;
        }
    }

    return NULL;
}

/**
*   Adds a specified route to the routingtable.
*   If the route already exists, the existing route 
*   is updated...
*/
int insertRoute(uint32 group, int ifx) {
    
    struct Config *conf = getCommonConfig();
    struct RouteTable*  croute;
    int result = 1;

    // Sanitycheck the group adress...
    if( ! IN_MULTICAST( ntohl(group) )) {
        log(LOG_WARNING, 0, "The group address %s is not a valid Multicast group. Table insert failed.",
            inetFmt(group, s1));
        return 0;
    }

    // Santiycheck the VIF index...
    //if(ifx < 0 || ifx >= MAX_MC_VIFS) {
    if(ifx >= MAX_MC_VIFS) {
        log(LOG_WARNING, 0, "The VIF Ix %d is out of range (0-%d). Table insert failed.",ifx,MAX_MC_VIFS);
        return 0;
    }

    // Try to find an existing route for this group...
    croute = findRoute(group);
    if(croute==NULL) {
        struct RouteTable*  newroute;

        IF_DEBUG log(LOG_DEBUG, 0, "No existing route for %s. Create new.",
                     inetFmt(group, s1));


        // Create and initialize the new route table entry..
        newroute = (struct RouteTable*)malloc(sizeof(struct RouteTable));
        // Insert the route desc and clear all pointers...
        newroute->group      = group;
        newroute->originAddr = 0;
        newroute->nextroute  = NULL;
        newroute->prevroute  = NULL;

        // The group is not joined initially.
        newroute->upstrState = ROUTESTATE_NOTJOINED;

        // The route is not active yet, so the age is unimportant.
        newroute->ageValue    = conf->robustnessValue;
        newroute->ageActivity = 0;
	#ifdef TCSUPPORT_IGMP_QOS
		newroute->skb_mark = skb_mark;
	#endif
        
        BIT_ZERO(newroute->ageVifBits);     // Initially we assume no listeners.

        // Set the listener flag...
        BIT_ZERO(newroute->vifBits);    // Initially no listeners...
        if(ifx >= 0) {
            BIT_SET(newroute->vifBits, ifx);
        }

        // Check if there is a table already....
        if(routing_table == NULL) {
            // No location set, so insert in on the table top.
            routing_table = newroute;
            IF_DEBUG log(LOG_DEBUG, 0, "No routes in table. Insert at beginning.");
        } else {

            IF_DEBUG log(LOG_DEBUG, 0, "Found existing routes. Find insert location.");

            // Check if the route could be inserted at the beginning...
            if(routing_table->group > group) {
                IF_DEBUG log(LOG_DEBUG, 0, "Inserting at beginning, before route %s",inetFmt(routing_table->group,s1));

                // Insert at beginning...
                newroute->nextroute = routing_table;
                newroute->prevroute = NULL;
                routing_table = newroute;

                // If the route has a next node, the previous pointer must be updated.
                if(newroute->nextroute != NULL) {
                    newroute->nextroute->prevroute = newroute;
                }

            } else {

                // Find the location which is closest to the route.
                for( croute = routing_table; croute->nextroute != NULL; croute = croute->nextroute ) {
                    // Find insert position.
                    if(croute->nextroute->group > group) {
                        break;
                    }
                }

                IF_DEBUG log(LOG_DEBUG, 0, "Inserting after route %s",inetFmt(croute->group,s1));
                
                // Insert after current...
                newroute->nextroute = croute->nextroute;
                newroute->prevroute = croute;
                if(croute->nextroute != NULL) {
                    croute->nextroute->prevroute = newroute; 
                }
                croute->nextroute = newroute;
            }
        }

        // Set the new route as the current...
        croute = newroute;

        // Log the cleanup in debugmode...
        log(LOG_INFO, 0, "Inserted route table entry for %s on VIF #%d",
            inetFmt(croute->group, s1),ifx);
        
#if defined(TCSUPPORT_XPON_IGMP) && defined(TCSUPPORT_MULTICAST_SPEED)
        system("/usr/bin/xponigmpcmd clearhwdrop");
#endif

    } else if(ifx >= 0) {

        // The route exists already, so just update it.
        BIT_SET(croute->vifBits, ifx);
        
        // Register the VIF activity for the aging routine
        BIT_SET(croute->ageVifBits, ifx);

        // Log the cleanup in debugmode...
        log(LOG_INFO, 0, "Updated route entry for %s on VIF #%d",
            inetFmt(croute->group, s1), ifx);

        // If the route is active, it must be reloaded into the Kernel..
        if(croute->originAddr != 0) {

            // Update route in kernel...
            if(!internUpdateKernelRoute(croute, 1)) {
                log(LOG_WARNING, 0, "The insertion into Kernel failed.");
                return 0;
            }
        }
    }

#ifdef TCSUPPORT_IGMP_QOS
    // Send join message upstream, if the route has no joined flag...
    if ((croute->upstrState != ROUTESTATE_JOINED) || (croute->skb_mark != skb_mark)) {
        // Send Join request upstream
		/* update old skb_mark */
		croute->skb_mark = skb_mark;
	
		//tcdbg_printf("%s, send join leave packet to upstream\n", __FUNCTION__);	
        sendJoinLeaveUpstream(croute, 1);
    }
#else
    // Send join message upstream, if the route has no joined flag...
    if(croute->upstrState != ROUTESTATE_JOINED) {
        // Send Join request upstream
        sendJoinLeaveUpstream(croute, 1);
    }
#endif

    IF_DEBUG logRouteTable("Insert Route");

    return 1;
}

/**
*   Activates a passive group. If the group is already
*   activated, it's reinstalled in the kernel. If
*   the route is activated, no originAddr is needed.
*/
int activateRoute(uint32 group, uint32 originAddr) {
    struct RouteTable*  croute;
    int result = 0;

    // Find the requested route.
    croute = findRoute(group);
    if(croute == NULL) {
        IF_DEBUG log(LOG_DEBUG, 0, "No table entry for %s [From: %s]. Inserting route.",
            inetFmt(group, s1),inetFmt(originAddr, s2));

        // Insert route, but no interfaces have yet requested it downstream.
        insertRoute(group, -1);

        // Retrieve the route from table...
        croute = findRoute(group);
    }

    if(croute != NULL) {
        // If the origin address is set, update the route data.
        if(originAddr > 0) {
            if(croute->originAddr > 0 && croute->originAddr!=originAddr) {
                log(LOG_WARNING, 0, "The origin for route %s changed from %s to %s",
                    inetFmt(croute->group, s1),
                    inetFmt(croute->originAddr, s2),
                    inetFmt(originAddr, s3));
            }
            croute->originAddr = originAddr;
        }

        // Only update kernel table if there are listeners !
        if(croute->vifBits > 0) {
            result = internUpdateKernelRoute(croute, 1);
        }
    }
    IF_DEBUG logRouteTable("Activate Route");

    return result;
}


/**
*   This function loops through all routes, and updates the age 
*   of any active routes.
*/
void ageActiveRoutes() {
    struct RouteTable   *croute, *nroute;
    
    IF_DEBUG log(LOG_DEBUG, 0, "Aging routes in table.");

    // Scan all routes...
    for( croute = routing_table; croute != NULL; croute = nroute ) {
        
        // Keep the next route (since current route may be removed)...
        nroute = croute->nextroute;

        // Run the aging round algorithm.
        if(croute->upstrState != ROUTESTATE_CHECK_LAST_MEMBER) {
            // Only age routes if Last member probe is not active...
            internAgeRoute(croute);
        }
    }
    IF_DEBUG logRouteTable("Age active routes");
}

/**
*   Should be called when a leave message is recieved, to
*   mark a route for the last member probe state.
*/
void setRouteLastMemberMode(uint32 group) {
    struct Config       *conf = getCommonConfig();
    struct RouteTable   *croute;

    croute = findRoute(group);
    if(croute!=NULL) {
        // Check for fast leave mode...
        if(croute->upstrState == ROUTESTATE_JOINED && conf->fastUpstreamLeave) {
            // Send a leave message right away..
            #ifdef TCSUPPORT_IGMP_QUICK_LEAVE 
             if(checkLastMemberInGroup(group))
			#endif
               sendJoinLeaveUpstream(croute, 0);

        }
        // Set the routingstate to Last member check...
        croute->upstrState = ROUTESTATE_CHECK_LAST_MEMBER;
        // Set the count value for expiring... (-1 since first aging)
        croute->ageValue = conf->lastMemberQueryCount;
    }
}
#ifdef TCSUPPORT_IGMP_QUICK_LEAVE
int checkLastMemberInGroup(uint32 group)
{
	char brname[10] ="br0" ;
	int i=0;
	int n=0;
	int groupMemberNum=0;
	struct mc_fdb_entry *fdb = NULL,*head=NULL;
	int offset = 0;
	int igmpSnoopingFlag=0;
	int  snoopingFastLeaveFlag=0;
	
	getsnoopingfastleaveFlag(brname,&snoopingFastLeaveFlag,&igmpSnoopingFlag);
	if(igmpSnoopingFlag==0)
		return 1;
	for(;;) {
		fdb = realloc(fdb, (offset + CHUNK) * sizeof(struct mc_fdb_entry));
		if (!fdb) {
			//fprintf(stderr, "Out of memory\n");
			return 0;
		}
			
		n = br_read_mc_fdb(brname, fdb+offset, offset, CHUNK);
		if (n == 0)
			break;
		if (n < 0) {
			//fprintf(stderr, "read of forward table failed: %s\n",strerror(errno));
			free(fdb);
			return 0;
		}
		offset += n;
	}
     head=fdb;
     for(i=0;i<offset;i++)
     {
		
	     if(inet_addr(fdb->group_addr)==group)
		 	++groupMemberNum;
	     ++fdb;
     }
	if((groupMemberNum==1&&snoopingFastLeaveFlag==0) || (groupMemberNum==0&&snoopingFastLeaveFlag==1))
	 {      
     	free(head);
		return 1;        
     }
     free(head);	
     return 0;
}

void getsnoopingfastleaveFlag(const char *bridge, int *snoopingFastLeaveFlag,int *igmpSnoopingFlag)
{
	struct ifreq ifr;
	struct __bridge_info i;
	int br_socket_fd=-1;
	unsigned long args[4] = { BRCTL_GET_BRIDGE_INFO, (unsigned long) &i, 0, 0 };
	strncpy(ifr.ifr_name, bridge, IFNAMSIZ);

	ifr.ifr_data = (char *) &args;
	if ((br_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return ;
	if (ioctl(br_socket_fd, SIOCDEVPRIVATE, &ifr) < 0) {
		//dprintf("%s: can't get info %s\n",bridge, strerror(errno));
	    if(br_socket_fd>0)
	       close(br_socket_fd);
		return ;
	}
     *snoopingFastLeaveFlag = i.igmpsnoop_quickleave?1:0;
	 *igmpSnoopingFlag = i.igmpsnoop_enabled?1:0;
	   //tcdbg_printf("getsnoopingfastleaveFlag()-->snoopingFastLeaveFlag:%d\n",*snoopingFastLeaveFlag );
     if(br_socket_fd>0)
	    close(br_socket_fd);
		   
return ;
}

int br_read_mc_fdb(const char *bridge, struct mc_fdb_entry *fdbs, 
		unsigned long offset, int num)
{
	int i,  n=0;
	int br_socket_fd=-1;
	struct __mc_fdb_entry fe[CHUNK];

	{
		/* old kernel, use ioctl */
		if ((br_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return errno;
		unsigned long args[4] = { BRCTL_GET_MC_FDB_ENTRIES,
					  (unsigned long) fe,
					  num, offset };
		struct ifreq ifr;
		int retries = 0;

		strncpy(ifr.ifr_name, bridge, IFNAMSIZ);
		ifr.ifr_data = (char *) args;

	retry:
		n = ioctl(br_socket_fd, SIOCDEVPRIVATE, &ifr);
		/* table can change during ioctl processing */
		if (n < 0 && errno == EAGAIN && ++retries < 10) {
			sleep(0);
			goto retry;
		}
	}

	for (i = 0; i < n; i++) 
		copy_mc_fdb(fdbs+i, fe+i);
	

if(br_socket_fd>0)
	close(br_socket_fd);
	
	return n;
}

static  void copy_mc_fdb(struct mc_fdb_entry *ent, 
			      const struct __mc_fdb_entry *f)
{
	memcpy(ent->group_addr, f->group_addr, 16);
	memcpy(ent->host_addr, f->host_addr, 6);
	#ifdef TCSUPPORT_IGMP_SNOOPING_V3
	memcpy(ent->src_addr,f->src_addr,16);
	ent->filter_mode = f->filter_mode;
	#endif
	ent->port_no = f->port_no;
	jiffies_to_tv(&ent->ageing_timer_value, f->ageing_timer_value);
}
static void jiffies_to_tv(struct timeval *tv, unsigned long jiffies)
{
	unsigned long long tvusec;

	tvusec = (1000000ULL*jiffies)/100;
	tv->tv_sec = tvusec/1000000;
	tv->tv_usec = tvusec - 1000000 * tv->tv_sec;
}
#endif
/**
*   Ages groups in the last member check state. If the
*   route is not found, or not in this state, 0 is returned.
*/
int lastMemberGroupAge(uint32 group) {
    struct Config       *conf = getCommonConfig();
    struct RouteTable   *croute;

    croute = findRoute(group);
    if(croute!=NULL) {
        if(croute->upstrState == ROUTESTATE_CHECK_LAST_MEMBER) {
            return !internAgeRoute(croute);
        } else {
            return 0;
        }
    }
    return 0;
}

/**
*   Remove a specified route. Returns 1 on success,
*   and 0 if route was not found.
*/
int removeRoute(struct RouteTable*  croute) {
    struct Config       *conf = getCommonConfig();
    int result = 1;
    
    // If croute is null, no routes was found.
    if(croute==NULL) {
        return 0;
    }

    // Log the cleanup in debugmode...
    IF_DEBUG log(LOG_DEBUG, 0, "Removed route entry for %s from table.",
                 inetFmt(croute->group, s1));

    //BIT_ZERO(croute->vifBits);

    // Uninstall current route from kernel
    if(!internUpdateKernelRoute(croute, 0)) {
        log(LOG_WARNING, 0, "The removal from Kernel failed.");
        result = 0;
    }

    // Send Leave request upstream if group is joined
    if(croute->upstrState == ROUTESTATE_JOINED || 
       (croute->upstrState == ROUTESTATE_CHECK_LAST_MEMBER && !conf->fastUpstreamLeave)) 
    {
        sendJoinLeaveUpstream(croute, 0);
    }

    // Update pointers...
    if(croute->prevroute == NULL) {
        // Topmost node...
        if(croute->nextroute != NULL) {
            croute->nextroute->prevroute = NULL;
        }
        routing_table = croute->nextroute;

    } else {
        croute->prevroute->nextroute = croute->nextroute;
        if(croute->nextroute != NULL) {
            croute->nextroute->prevroute = croute->prevroute;
        }
    }
    // Free the memory, and set the route to NULL...
    free(croute);
    croute = NULL;

    IF_DEBUG logRouteTable("Remove route");

    return result;
}


/**
*   Ages a specific route
*/
int internAgeRoute(struct RouteTable*  croute) {
    struct Config *conf = getCommonConfig();
    int result = 0;

    // Drop age by 1.
    croute->ageValue--;

    // Check if there has been any activity...
    if( croute->ageVifBits > 0 && croute->ageActivity == 0 ) {
        // There was some activity, check if all registered vifs responded.
        if(croute->vifBits == croute->ageVifBits) {
            // Everything is in perfect order, so we just update the route age.
            croute->ageValue = conf->robustnessValue;
            //croute->ageActivity = 0;
        } else {
            // One or more VIF has not gotten any response.
            croute->ageActivity++;

            // Update the actual bits for the route...
            croute->vifBits = croute->ageVifBits;
        }
    } 
    // Check if there have been activity in aging process...
    else if( croute->ageActivity > 0 ) {

        // If the bits are different in this round, we must
        if(croute->vifBits != croute->ageVifBits) {
            // Or the bits together to insure we don't lose any listeners.
            croute->vifBits |= croute->ageVifBits;

            // Register changes in this round as well..
            croute->ageActivity++;
        }
    }

    // If the aging counter has reached zero, its time for updating...
    if(croute->ageValue == 0) {
        // Check for activity in the aging process,
        if(croute->ageActivity>0) {
            
            IF_DEBUG log(LOG_DEBUG, 0, "Updating route after aging : %s",
                         inetFmt(croute->group,s1));
            
            // Just update the routing settings in kernel...
            internUpdateKernelRoute(croute, 1);
    
            // We append the activity counter to the age, and continue...
            croute->ageValue = croute->ageActivity;
            croute->ageActivity = 0;
        } else {

            IF_DEBUG log(LOG_DEBUG, 0, "Removing group %s. Died of old age.",
                         inetFmt(croute->group,s1));

            // No activity was registered within the timelimit, so remove the route.
            removeRoute(croute);
        }
        // Tell that the route was updated...
        result = 1;
    }

    // The aging vif bits must be reset for each round...
    BIT_ZERO(croute->ageVifBits);

    return result;
}

/**
*   Updates the Kernel routing table. If activate is 1, the route
*   is (re-)activated. If activate is false, the route is removed.
*/
int internUpdateKernelRoute(struct RouteTable *route, int activate) {
    struct   MRouteDesc     mrDesc;
    struct   IfDesc         *Dp;
    unsigned                Ix;
    
    if(route->originAddr>0) {

        // Build route descriptor from table entry...
        // Set the source address and group address...
        mrDesc.McAdr.s_addr     = route->group;
        mrDesc.OriginAdr.s_addr = route->originAddr;
    
        // clear output interfaces 
        memset( mrDesc.TtlVc, 0, sizeof( mrDesc.TtlVc ) );
    
        IF_DEBUG log(LOG_DEBUG, 0, "Vif bits : 0x%08x", route->vifBits);

        // Set the TTL's for the route descriptor...
        for ( Ix = 0; Dp = getIfByIx( Ix ); Ix++ ) {
            if(Dp->state == IF_STATE_UPSTREAM) {
                //IF_DEBUG log(LOG_DEBUG, 0, "Identified VIF #%d as upstream.", Dp->index);
                mrDesc.InVif = Dp->index;
            }
            else if(BIT_TST(route->vifBits, Dp->index)) {
                IF_DEBUG log(LOG_DEBUG, 0, "Setting TTL for Vif %d to %d", Dp->index, Dp->threshold);
                mrDesc.TtlVc[ Dp->index ] = Dp->threshold;
            }
        }
    
        // Do the actual Kernel route update...
        if(activate) {
            // Add route in kernel...
            addMRoute( &mrDesc );
    
        } else {
            // Delete the route from Kernel...
            delMRoute( &mrDesc );
        }

    } else {
        log(LOG_NOTICE, 0, "Route is not active. No kernel updates done.");
    }

    return 1;
}

/**
*   Debug function that writes the routing table entries
*   to the log.
*/
void logRouteTable(char *header) {
    IF_DEBUG  {
        struct RouteTable*  croute = routing_table;
        unsigned            rcount = 0;
    
        log(LOG_DEBUG, 0, "\nCurrent routing table (%s);\n-----------------------------------------------------\n", header);
        if(croute==NULL) {
            log(LOG_DEBUG, 0, "No routes in table...");
        } else {
            do {
                /*
                log(LOG_DEBUG, 0, "#%d: Src: %s, Dst: %s, Age:%d, St: %s, Prev: 0x%08x, T: 0x%08x, Next: 0x%08x",
                    rcount, inetFmt(croute->originAddr, s1), inetFmt(croute->group, s2),
                    croute->ageValue,(croute->originAddr>0?"A":"I"),
                    croute->prevroute, croute, croute->nextroute);
                */
                log(LOG_DEBUG, 0, "#%d: Src: %s, Dst: %s, Age:%d, St: %s, OutVifs: 0x%08x",
                    rcount, inetFmt(croute->originAddr, s1), inetFmt(croute->group, s2),
                    croute->ageValue,(croute->originAddr>0?"A":"I"),
                    croute->vifBits);
                  
                croute = croute->nextroute; 
        
                rcount++;
            } while ( croute != NULL );
        }
    
        log(LOG_DEBUG, 0, "\n-----------------------------------------------------\n");
    }
}
