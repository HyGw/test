/*
 * Database handling functions
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/linux-atm/lane/db.h#1 $
 *
 */
#include <sys/types.h>

#include "lane.h"
#include "connect.h"

/* Protos */
void regdb_add(AtmAddr_t aaddr, LaneDestination_t maddr);
int regdb_remove(AtmAddr_t to_remove);
Reg_t *regdb_find_mac(LaneDestination_t maddr);

void proxydb_add(const Conn_t *conn, int fd);
int proxydb_remove(const Conn_t *conn);
Proxy_t *proxydb_find(LecId_t to_find);

Lecdb_t *leciddb_find(LecId_t to_find);
Lecdb_t *leciddb_find_atm(AtmAddr_t to_find);
void leciddb_add(LecId_t lecid, AtmAddr_t address, const int fd);
int leciddb_remove(LecId_t to_remove);

