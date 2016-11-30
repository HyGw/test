#ifndef BUNDLE_CTRL_H_
#define BUNDLE_CTRL_H_

#include "event.h"
#include "urandom.h"
#include "jsonpkt.h"
#include "list.h"
#include "mobile_manager.h"
#include "parameter_table.h"

#define BCMD_INDEX_EVENT	11
#define ADD_PARAID_LEN		20  /* <,"ID":4294967296} >*/

trans_pkt_t * bundle_sock_com(trans_pkt_t *  sendpkt, int id);
trans_pkt_t * CreateBundleCmdPkt(int index, int argc, char *argv[]);
int bundle_sock_init(void);
void bundle_sock_free(void);


#endif /* BUNDLE_CTRL_H_ */
