/**************************************
 ecmh - Easy Cast du Multi Hub
 by Jeroen Massar <jeroen@unfix.org>
***************************************
 $Author: aaron.zhang $
 $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/ecmh-2005.02.09/src/groups.h#1 $
 $Date: 2016/10/14 $
**************************************/

/* The node used to hold the groups we joined */
struct groupnode
{
	struct in6_addr	mca;		/* The Multicast IPv6 address (group) */
	struct list	*interfaces;	/* The list of grpint nodes (interfaces) that */
					/* are interrested in this node */
	time_t		lastforward;	/* The last time we forwarded a report for this group */
	uint64_t	bytes;		/* Number of received bytes */
	uint64_t	packets;	/* Number of received packets */
};

struct groupnode *group_create(const struct in6_addr *mca);
void group_destroy(struct groupnode *groupn);
struct groupnode *group_find(const struct in6_addr *mca);
struct grpintnode *groupint_get(const struct in6_addr *mca, struct intnode *interface, bool *isnew);
