#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "../lib/libtcapi.h"
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_conntrack.h>

#define NFNL_MAX_SUBSYS 16 /* enough for now */

#ifndef NLA_F_NESTED
#define NLA_F_NESTED            (1 << 15)
#endif

#define NLMSG_TAIL(nlh) \
	(((void *) (nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len))

#define nfnl_parse_nested(tb, max, nfa) \
	nfnl_parse_attr((tb), (max), NFA_DATA((nfa)), NFA_PAYLOAD((nfa)))

#define nfnl_nest(nlh, bufsize, type) 				\
({	struct nfattr *__start = NLMSG_TAIL(nlh);		\
	nfnl_addattr_l(nlh, bufsize, (NLA_F_NESTED | type), NULL, 0); 	\
	__start; })

#define nfnl_nest_end(nlh, tail) 				\
({	(tail)->nfa_len = (void *) NLMSG_TAIL(nlh) - (void *) tail; })

enum {
	NFNL_CB_FAILURE = -1,   /* failure */
	NFNL_CB_STOP = 0,       /* stop the query */
	NFNL_CB_CONTINUE = 1,   /* keep iterating */
};

enum nf_conntrack_query {
	NFCT_Q_DESTROY,
	NFCT_Q_DUMP,
};

/* message type */
enum nf_conntrack_msg_type {
	NFCT_T_UNKNOWN = 0,

	NFCT_T_NEW_BIT = 0,
	NFCT_T_NEW = (1 << NFCT_T_NEW_BIT),

	NFCT_T_UPDATE_BIT = 1,
	NFCT_T_UPDATE = (1 << NFCT_T_UPDATE_BIT),

	NFCT_T_DESTROY_BIT = 2,
	NFCT_T_DESTROY = (1 << NFCT_T_DESTROY_BIT),

	NFCT_T_ALL = NFCT_T_NEW | NFCT_T_UPDATE | NFCT_T_DESTROY,

	NFCT_T_ERROR_BIT = 31,
	NFCT_T_ERROR = (1 << NFCT_T_ERROR_BIT),
};

struct __nfct_tuple {
	u_int32_t	src;
	u_int32_t 	dst;

	u_int8_t	l3protonum;
	u_int8_t	protonum;

	u_int16_t	l4src;
	u_int16_t	l4dst;
};

struct nfnlhdr{
	struct nlmsghdr nlh;
	struct nfgenmsg nfmsg;
};

struct nfnl_callback {
	int (*call)(struct nlmsghdr *nlh, struct nfattr *nfa[], void *data);
	void *data;
	u_int16_t attr_count;
};

struct nfnl_subsys_handle {
	struct nfnl_handle 	*nfnlh;
	u_int32_t		subscriptions;
	u_int8_t		subsys_id;
	u_int8_t		cb_count;
	struct nfnl_callback 	*cb;	/* array of callbacks */
};

struct nfnl_handle{
	int fd;
	struct sockaddr_nl local;
	struct sockaddr_nl peer;
	u_int32_t seq;
	u_int32_t subscriptions;
	struct nfnl_subsys_handle subsys[NFNL_MAX_SUBSYS+1];
};

struct nf_conntrack {
	struct __nfct_tuple 	tuple;
};

struct nfct_handle {
	struct nfnl_handle		*nfnlh;
	struct nfnl_subsys_handle	*nfnlssh_ct;

	struct nfnl_callback		nfnl_cb;

	int			(*cb)(enum nf_conntrack_msg_type type, 
				      struct nf_conntrack *ct,
				      void *data);
};

struct __data_container {
	struct nfct_handle *h;
	enum nf_conntrack_msg_type type;
	void *data;
};

static struct nfct_handle *cth, *ith;



struct nfct_handle *nfct_open(u_int8_t subsys_id, unsigned subscriptions);
struct nfct_handle *nfct_open_nfnl(struct nfnl_handle *nfnlh,
				   u_int8_t subsys_id,
				   unsigned int subscriptions);
int nfct_close(struct nfct_handle *cth);
static int delete_cb(enum nf_conntrack_msg_type type,
		     struct nf_conntrack *ct,
		     void *data);
int nfct_cmp(const struct nf_conntrack *ct1,
	      const struct nf_conntrack *ct2);

static int
cmp_orig_l3proto(const struct nf_conntrack *ct1,
		 const struct nf_conntrack *ct2);

static int 
cmp_orig_l4proto(const struct nf_conntrack *ct1,
		 const struct nf_conntrack *ct2);

static int
cmp_orig_port_dst(const struct nf_conntrack *ct1,
		  const struct nf_conntrack *ct2);

static int 
cmp_orig_ipv4_src(const struct nf_conntrack *ct1,
		  const struct nf_conntrack *ct2);

int 
nfct_callback_register(struct nfct_handle *h,
			   enum nf_conntrack_msg_type type,
			   int (*cb)(enum nf_conntrack_msg_type type,
			   	     struct nf_conntrack *ct, 
				     void *data),
			   void *data);

int 
nfnl_callback_register(struct nfnl_subsys_handle *ssh,
			   u_int8_t type, struct nfnl_callback *cb);

int 
__callback(struct nlmsghdr *nlh, struct nfattr *nfa[], void *data);

int 
__parse_message_type(const struct nlmsghdr *nlh);

void 
__parse_conntrack(const struct nlmsghdr *nlh,
		       const struct nfattr *cda[],
		       struct nf_conntrack *ct);

void 
__parse_tuple(const struct nfattr *attr,
		   struct __nfct_tuple *tuple);

static void 
__parse_ip(const struct nfattr *attr,
		       struct __nfct_tuple *tuple);

static void 
__parse_proto(const struct nfattr *attr,
			  struct __nfct_tuple *tuple);

struct nf_conntrack *nfct_new();

int 
nfct_query(struct nfct_handle *h,
	       const enum nf_conntrack_query qt,
	       const void *data);

int 
nfnl_query(struct nfnl_handle *h, struct nlmsghdr *nlh);


int 
nfct_build_query(struct nfnl_subsys_handle *ssh,
		     const enum nf_conntrack_query qt,
		     const void *data,
		     void *buffer,
		     unsigned int size);
int 
nfnl_send(struct nfnl_handle *nfnlh, struct nlmsghdr *n);

struct nfnl_handle *nfnl_open(void);
struct nfnl_subsys_handle *
nfnl_subsys_open(struct nfnl_handle *nfnlh, u_int8_t subsys_id,
		 u_int8_t cb_count, u_int32_t subscriptions);
static int recalc_rebind_subscriptions(struct nfnl_handle *nfnlh);
void nfnl_fill_hdr(struct nfnl_subsys_handle *ssh,
		    struct nlmsghdr *nlh, unsigned int len, 
		    u_int8_t family,
		    u_int16_t res_id,
		    u_int16_t msg_type,
		    u_int16_t msg_flags);
int nfnl_catch(struct nfnl_handle *h); 
ssize_t 
nfnl_recv(const struct nfnl_handle *h, unsigned char *buf, size_t len);
int nfnl_process(struct nfnl_handle *h, const unsigned char *buf, size_t len);
static int nfnl_step(struct nfnl_handle *h, struct nlmsghdr *nlh);
static int nfnl_is_error(struct nfnl_handle *h, struct nlmsghdr *nlh);

int nfnl_parse_attr(struct nfattr *tb[], int max, struct nfattr *nfa, int len);

void nfnl_subsys_close(struct nfnl_subsys_handle *ssh);
int nfnl_close(struct nfnl_handle *nfnlh);

int nfct_query(struct nfct_handle *h,
	       const enum nf_conntrack_query qt,
	       const void *data);

int nfct_build_query(struct nfnl_subsys_handle *ssh,
		     const enum nf_conntrack_query qt,
		     const void *data,
		     void *buffer,
		     unsigned int size);

int nfct_build_conntrack(struct nfnl_subsys_handle *ssh,
		      struct nfnlhdr *req,
		      size_t size,
		      u_int16_t type,
		      u_int16_t flags,
		      const struct nf_conntrack *ct);

void __build_tuple(struct nfnlhdr *req, 
		   size_t size, 
		   const struct __nfct_tuple *t, 
		   const int type);

void __build_tuple_ip(struct nfnlhdr *req, 
		      size_t size,
		      const struct __nfct_tuple *t);

void __build_tuple_proto(struct nfnlhdr *req,
			 size_t size,
			 const struct __nfct_tuple *t);

static void __build_protoinfo(struct nfnlhdr *req, size_t size);

int nfnl_addattr_l(struct nlmsghdr *n, int maxlen, int type, const void *data,
		   int alen);

int conntrack_delete(u_int8_t l3protonum, u_int8_t protonum, unsigned int src_ip, u_int16_t l4dst)
{
	int ret;

	int family = AF_INET;
	char __obj[sizeof(struct nf_conntrack)];
	struct nf_conntrack *obj = (struct nf_conntrack *)(void*) __obj;

	memset(__obj, 0, sizeof(__obj));

	/* open a socket for netfilter netlink struct */
	cth = nfct_open(NFNL_SUBSYS_CTNETLINK, 0);
	ith = nfct_open(NFNL_SUBSYS_CTNETLINK, 0);
	if (!cth || !ith)
		tcdbg_printf("can't open cth or ith");

	obj->tuple.l3protonum = l3protonum;
	obj->tuple.protonum = protonum;
	obj->tuple.src = src_ip;
	obj->tuple.l4dst = l4dst;

	nfct_callback_register(cth, NFCT_T_ALL, delete_cb, obj);

	ret = nfct_query(cth, NFCT_Q_DUMP, &family);
		
	if (ret == -1)
		tcdbg_printf("delete conntrack (%d)(%s)\n", ret, strerror(errno));
	
	nfct_close(ith);
	nfct_close(cth);

	if (ret == -1 )
		return (EXIT_FAILURE); 
	else
		return (EXIT_SUCCESS);
}

struct nfct_handle *nfct_open(u_int8_t subsys_id, unsigned subscriptions)
{
	struct nfnl_handle *nfnlh = nfnl_open();
	struct nfct_handle *nfcth;

	if (!nfnlh)
		return NULL;

	nfcth = nfct_open_nfnl(nfnlh, subsys_id, subscriptions);
	if (!nfcth)
		nfnl_close(nfnlh);

	return nfcth;
}

struct nfct_handle *nfct_open_nfnl(struct nfnl_handle *nfnlh,
				   u_int8_t subsys_id,
				   unsigned int subscriptions)
{
	struct nfct_handle *cth;

	cth = (struct nfct_handle *) malloc(sizeof(struct nfct_handle));
	if (!cth)
		return NULL;
	
	memset(cth, 0, sizeof(*cth));
	cth->nfnlh = nfnlh;

	if (subsys_id == 0 || subsys_id == NFNL_SUBSYS_CTNETLINK) {
		cth->nfnlssh_ct = nfnl_subsys_open(cth->nfnlh, 
						   NFNL_SUBSYS_CTNETLINK, 
						   IPCTNL_MSG_MAX,
						   subscriptions);
		if (!cth->nfnlssh_ct)
			goto out_free;
	}
	
	return cth;

	out_free:
	if (cth->nfnlssh_ct) {
		nfnl_subsys_close(cth->nfnlssh_ct);
		cth->nfnlssh_ct = NULL;
	}

	free(cth);
	return NULL;
}

int nfct_close(struct nfct_handle *cth)
{
	int err = 0;

	if (cth->nfnlssh_ct) {
		nfnl_subsys_close(cth->nfnlssh_ct);
		cth->nfnlssh_ct = NULL;
	}

	/* required by the new API */
	cth->cb = NULL;
	free(cth->nfnl_cb.data);
		
	cth->nfnl_cb.call = NULL; 
	cth->nfnl_cb.data = NULL;
	cth->nfnl_cb.attr_count = 0;

	err = nfnl_close(cth->nfnlh);
	free(cth);

	return err;
}

static int delete_cb(enum nf_conntrack_msg_type type,
		     struct nf_conntrack *ct,
		     void *data)
{
	int res;
	struct nf_conntrack *obj = data;

	
	if (!nfct_cmp(obj, ct))
		return NFNL_CB_CONTINUE;

	res = nfct_query(ith, NFCT_Q_DESTROY, ct);
	if (res < 0)
	{
		tcdbg_printf("nfct_delete");
	}

	return NFNL_CB_CONTINUE;
}

int nfct_cmp(const struct nf_conntrack *ct1,
	      const struct nf_conntrack *ct2)
{
	if (!cmp_orig_l3proto(ct1, ct2))
		return 0;
	if (!cmp_orig_l4proto(ct1, ct2))
		return 0;
	if (!cmp_orig_port_dst(ct1, ct2))
		return 0;
	if (!cmp_orig_ipv4_src(ct1, ct2))
		return 0;

	return 1;
}

static int
cmp_orig_l3proto(const struct nf_conntrack *ct1,
		 const struct nf_conntrack *ct2)
{
	return (ct1->tuple.l3protonum ==
		ct2->tuple.l3protonum);
}

static int 
cmp_orig_l4proto(const struct nf_conntrack *ct1,
		 const struct nf_conntrack *ct2)
{
	return (ct1->tuple.protonum == ct2->tuple.protonum);
}

static int
cmp_orig_port_dst(const struct nf_conntrack *ct1,
		  const struct nf_conntrack *ct2)
{
	return (ct1->tuple.l4dst == ct2->tuple.l4dst);
}

static int 
cmp_orig_ipv4_src(const struct nf_conntrack *ct1,
		  const struct nf_conntrack *ct2)
{
	return (ct1->tuple.src == ct2->tuple.src);
}

int nfct_callback_register(struct nfct_handle *h,
			   enum nf_conntrack_msg_type type,
			   int (*cb)(enum nf_conntrack_msg_type type,
			   	     struct nf_conntrack *ct, 
				     void *data),
			   void *data)
{
	struct __data_container *container;

	assert(h != NULL);

	container = malloc(sizeof(struct __data_container));
	if (!container)
		return -1;
	memset(container, 0, sizeof(struct __data_container));

	h->cb = cb;
	container->h = h;
	container->type = type;
	container->data = data;

	h->nfnl_cb.call = __callback;
	h->nfnl_cb.data = container;
	h->nfnl_cb.attr_count = CTA_MAX;

	nfnl_callback_register(h->nfnlssh_ct, 
			       IPCTNL_MSG_CT_NEW,
			       &h->nfnl_cb);

	nfnl_callback_register(h->nfnlssh_ct,
			       IPCTNL_MSG_CT_DELETE,
			       &h->nfnl_cb);

	return 0;
}

int nfnl_callback_register(struct nfnl_subsys_handle *ssh,
			   u_int8_t type, struct nfnl_callback *cb)
{
	assert(ssh);
	assert(cb);

	if (type >= ssh->cb_count) {
		errno = EINVAL;
		return -1;
	}

	memcpy(&ssh->cb[type], cb, sizeof(*cb));

	return 0;
}

int __callback(struct nlmsghdr *nlh, struct nfattr *nfa[], void *data)
{
	int ret = NFNL_CB_STOP;
	unsigned int type;
	struct nf_conntrack *ct;
	int len = nlh->nlmsg_len;
	struct __data_container *container = data;

	len -= NLMSG_LENGTH(sizeof(struct nfgenmsg));
	if (len < 0)
		return NFNL_CB_CONTINUE;

	type = __parse_message_type(nlh);
	if (!(type & container->type))
		return NFNL_CB_CONTINUE;

	ct = nfct_new();
	if (!ct)
		return NFNL_CB_CONTINUE;

	__parse_conntrack(nlh, nfa, ct);

	if (container->h->cb)
		ret = container->h->cb(type, ct, container->data);

	switch(ret) {
	case NFNL_CB_FAILURE:
		free(ct);
		ret = NFNL_CB_FAILURE;
		break;
	case NFNL_CB_STOP:
		free(ct);
		ret = NFNL_CB_STOP;
		break;
	case NFNL_CB_CONTINUE:
		free(ct);
		ret = NFNL_CB_CONTINUE;
		break;
	}
	return ret;
}

int __parse_message_type(const struct nlmsghdr *nlh)
{
	u_int16_t type = NFNL_MSG_TYPE(nlh->nlmsg_type);
	int ret = IPCTNL_MSG_MAX;

	if (type == IPCTNL_MSG_CT_DELETE)
		ret = IPCTNL_MSG_CT_DELETE;

	return ret;
}

void __parse_conntrack(const struct nlmsghdr *nlh,
		       const struct nfattr *cda[],
		       struct nf_conntrack *ct)
{
	struct nfgenmsg *nfhdr = NLMSG_DATA(nlh);

	if (cda[CTA_TUPLE_ORIG-1]) {
		ct->tuple.l3protonum = nfhdr->nfgen_family;

		__parse_tuple(cda[CTA_TUPLE_ORIG-1], 
			      &ct->tuple);
	}
}

void __parse_tuple(const struct nfattr *attr,
		   struct __nfct_tuple *tuple)
{	
	struct nfattr *tb[CTA_TUPLE_MAX];

	nfnl_parse_nested(tb, CTA_TUPLE_MAX, attr);

	if (tb[CTA_TUPLE_IP-1])
		__parse_ip(tb[CTA_TUPLE_IP-1], tuple);

	if (tb[CTA_TUPLE_PROTO-1])
		__parse_proto(tb[CTA_TUPLE_PROTO-1], tuple);
}

static void __parse_ip(const struct nfattr *attr,
		       struct __nfct_tuple *tuple)
{
	struct nfattr *tb[CTA_IP_MAX];

    nfnl_parse_nested(tb, CTA_IP_MAX, attr);
	if (tb[CTA_IP_V4_SRC-1]) 
		tuple->src = *(u_int32_t *)NFA_DATA(tb[CTA_IP_V4_SRC-1]);

	if (tb[CTA_IP_V4_DST-1]) 
		tuple->dst = *(u_int32_t *)NFA_DATA(tb[CTA_IP_V4_DST-1]);
}

static void __parse_proto(const struct nfattr *attr,
			  struct __nfct_tuple *tuple)
{
	struct nfattr *tb[CTA_PROTO_MAX];

	nfnl_parse_nested(tb, CTA_PROTO_MAX, attr);

	if (tb[CTA_PROTO_NUM-1]) {
		tuple->protonum = *(u_int8_t *)NFA_DATA(tb[CTA_PROTO_NUM-1]);
	}

	if (tb[CTA_PROTO_SRC_PORT-1]) {
		tuple->l4src =
			*(u_int16_t *)NFA_DATA(tb[CTA_PROTO_SRC_PORT-1]);
	}

	if (tb[CTA_PROTO_DST_PORT-1]) {
		tuple->l4dst =
			*(u_int16_t *)NFA_DATA(tb[CTA_PROTO_DST_PORT-1]);

	}
}

struct nf_conntrack *nfct_new()
{
	struct nf_conntrack *ct;

	ct = malloc(sizeof(struct nf_conntrack));
	if (!ct)
		return NULL;

	memset(ct, 0, sizeof(struct nf_conntrack));

	return ct;
}

int nfct_query(struct nfct_handle *h,
	       const enum nf_conntrack_query qt,
	       const void *data)
{
	size_t size = 4096;	/* enough for now */
	union {
		char buffer[size];
		struct nfnlhdr req;
	} u;

	assert(h != NULL);
	assert(data != NULL);

	if (nfct_build_query(h->nfnlssh_ct, qt, data, &u.req, size) == -1)
		return -1;

	return nfnl_query(h->nfnlh, &u.req.nlh);
}

int nfnl_query(struct nfnl_handle *h, struct nlmsghdr *nlh)
{
	assert(h);
	assert(nlh);

	if (nfnl_send(h, nlh) == -1)
		return -1;

	return nfnl_catch(h);
}


int nfct_build_query(struct nfnl_subsys_handle *ssh,
		     const enum nf_conntrack_query qt,
		     const void *data,
		     void *buffer,
		     unsigned int size)
{
	struct nfnlhdr *req = buffer;
	const u_int32_t *family = data;

	assert(ssh != NULL);
	assert(data != NULL);
	assert(req != NULL);

	memset(req, 0, size);

	switch(qt) {
	case NFCT_Q_DESTROY:
		nfct_build_conntrack(ssh, req, size, IPCTNL_MSG_CT_DELETE, NLM_F_REQUEST|NLM_F_ACK, data);
		break;
	case NFCT_Q_DUMP:
		nfnl_fill_hdr(ssh, &req->nlh, 0, *family, 0, IPCTNL_MSG_CT_GET, NLM_F_REQUEST|NLM_F_DUMP);
		break;
	default:
		errno = ENOTSUP;
		return -1;
	}
	return 1;
}

int nfnl_send(struct nfnl_handle *nfnlh, struct nlmsghdr *n)
{
	assert(nfnlh);
	assert(n);

	return sendto(nfnlh->fd, n, n->nlmsg_len, 0, 
		      (struct sockaddr *)&nfnlh->peer, sizeof(nfnlh->peer));
}

struct nfnl_handle *nfnl_open(void)
{
	struct nfnl_handle *nfnlh;
	unsigned int addr_len;
	int err;

	nfnlh = malloc(sizeof(*nfnlh));
	if (!nfnlh)
		return NULL;

	memset(nfnlh, 0, sizeof(*nfnlh));
	nfnlh->fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_NETFILTER);
	if (nfnlh->fd == -1)
		goto err_free;

	nfnlh->local.nl_family = AF_NETLINK;
	nfnlh->peer.nl_family = AF_NETLINK;

	addr_len = sizeof(nfnlh->local);
	err = getsockname(nfnlh->fd, (struct sockaddr *)&nfnlh->local, 
			  &addr_len);
	if (addr_len != sizeof(nfnlh->local)) {
		errno = EINVAL;
		goto err_close;
	}
	if (nfnlh->local.nl_family != AF_NETLINK) {
		errno = EINVAL;
		goto err_close;
	}
	nfnlh->seq = time(NULL);

	/* let us do the initial bind */
	if (recalc_rebind_subscriptions(nfnlh) < 0)
		goto err_close;

	/* use getsockname to get the netlink pid that the kernel assigned us */
	addr_len = sizeof(nfnlh->local);
	err = getsockname(nfnlh->fd, (struct sockaddr *)&nfnlh->local, 
			  &addr_len);
	if (addr_len != sizeof(nfnlh->local)) {
		errno = EINVAL;
		goto err_close;
	}

	return nfnlh;

err_close:
	close(nfnlh->fd);
err_free:
	free(nfnlh);
	return NULL;
}


struct nfnl_subsys_handle *
nfnl_subsys_open(struct nfnl_handle *nfnlh, u_int8_t subsys_id,
		 u_int8_t cb_count, u_int32_t subscriptions)
{
	struct nfnl_subsys_handle *ssh;

	assert(nfnlh);

	if (subsys_id > NFNL_MAX_SUBSYS) { 
		errno = ENOENT;
		return NULL;
	}

	ssh = &nfnlh->subsys[subsys_id];
	if (ssh->cb) {
		errno = EBUSY;
		return NULL;
	}

	ssh->cb = calloc(cb_count, sizeof(*(ssh->cb)));
	if (!ssh->cb)
		return NULL;

	ssh->nfnlh = nfnlh;
	ssh->cb_count = cb_count;
	ssh->subscriptions = subscriptions;
	ssh->subsys_id = subsys_id;

	/* although now we have nfnl_join to subscribe to certain
	 * groups, just keep this to ensure compatibility */
	if (recalc_rebind_subscriptions(nfnlh) < 0) {
		free(ssh->cb);
		ssh->cb = NULL;
		return NULL;
	}
	
	return ssh;
}

static int recalc_rebind_subscriptions(struct nfnl_handle *nfnlh)
{
	int i, err;
	u_int32_t new_subscriptions = nfnlh->subscriptions;

	for (i = 0; i < NFNL_MAX_SUBSYS; i++)
		new_subscriptions |= nfnlh->subsys[i].subscriptions;

	nfnlh->local.nl_groups = new_subscriptions;
	err = bind(nfnlh->fd, (struct sockaddr *)&nfnlh->local,
		   sizeof(nfnlh->local));
	if (err == -1)
		return -1;

	nfnlh->subscriptions = new_subscriptions;

	return 0;
}

void nfnl_fill_hdr(struct nfnl_subsys_handle *ssh,
		    struct nlmsghdr *nlh, unsigned int len, 
		    u_int8_t family,
		    u_int16_t res_id,
		    u_int16_t msg_type,
		    u_int16_t msg_flags)
{
	assert(ssh);
	assert(nlh);

	struct nfgenmsg *nfg = (struct nfgenmsg *) 
					((void *)nlh + sizeof(*nlh));

	nlh->nlmsg_len = NLMSG_LENGTH(len+sizeof(*nfg));
	nlh->nlmsg_type = (ssh->subsys_id<<8)|msg_type;
	nlh->nlmsg_flags = msg_flags;
	nlh->nlmsg_pid = 0;

	nlh->nlmsg_seq = ++ssh->nfnlh->seq;
	/* kernel uses sequence number zero for events */
	if (!ssh->nfnlh->seq)
			nlh->nlmsg_seq = ssh->nfnlh->seq = time(NULL);

	nfg->nfgen_family = family;
	nfg->version = NFNETLINK_V0;
	nfg->res_id = htons(res_id);
}


int nfnl_catch(struct nfnl_handle *h)
{
	int ret;

	assert(h);

	while (1) {
		unsigned char buf[8192]
			__attribute__ ((aligned));

		ret = nfnl_recv(h, buf, sizeof(buf));
		if (ret == -1) {
			/* interrupted syscall must retry */
			if (errno == EINTR)
				continue;
			break;
		}

		ret = nfnl_process(h, buf, ret);
		if (ret <= NFNL_CB_STOP)
			break; 
	}

	return ret;
}

ssize_t 
nfnl_recv(const struct nfnl_handle *h, unsigned char *buf, size_t len)
{
	socklen_t addrlen;
	int status;
	struct sockaddr_nl peer;

	assert(h);
	assert(buf);
	assert(len > 0);
	
	if (len < sizeof(struct nlmsgerr)
	    || len < sizeof(struct nlmsghdr)) {
	    	errno = EBADMSG;
		return -1; 
	}

	addrlen = sizeof(h->peer);
	status = recvfrom(h->fd, buf, len, 0, (struct sockaddr *)&peer,	
			&addrlen);
	if (status <= 0)
		return status;

	if (addrlen != sizeof(peer)) {
		errno = EINVAL;
		return -1;
	}

	if (peer.nl_pid != 0) {
		errno = ENOMSG;
		return -1;
	}

	return status;
}

int nfnl_process(struct nfnl_handle *h, const unsigned char *buf, size_t len)
{
	int ret = 0;
	struct nlmsghdr *nlh = (struct nlmsghdr *)buf;

	assert(h);
	assert(buf);
	assert(len > 0);

	/* check for out of sequence message */
	if (nlh->nlmsg_seq && nlh->nlmsg_seq != h->seq) {
		errno = EILSEQ;
		return -1;
	}
	
	while (len >= NLMSG_SPACE(0) && NLMSG_OK(nlh, len)) {

		ret = nfnl_step(h, nlh);
		if (ret <= NFNL_CB_STOP)
			break;

		nlh = NLMSG_NEXT(nlh, len);
	}
	return ret;
}


/* On error, -1 is returned and errno is set appropiately. On success, 
 * 0 is returned if there is no more data to process, >0 if there is
 * more data to process */
static int nfnl_step(struct nfnl_handle *h, struct nlmsghdr *nlh)
{
	struct nfnl_subsys_handle *ssh;
	u_int8_t type = NFNL_MSG_TYPE(nlh->nlmsg_type);
	u_int8_t subsys_id = NFNL_SUBSYS_ID(nlh->nlmsg_type);

	/* Is this an error message? */
	if (nfnl_is_error(h, nlh)) {
		/* This is an ACK */
		if (errno == 0)
			return 0;
		/* This an error message */
		return -1;
	}
	
	/* nfnetlink sanity checks: check for nfgenmsg size */
	if (nlh->nlmsg_len < NLMSG_SPACE(sizeof(struct nfgenmsg))) {
		errno = ENOSPC;
		return -1;
	}

	if (subsys_id > NFNL_MAX_SUBSYS) {
		errno = ENOENT;
		return -1;
	}

	ssh = &h->subsys[subsys_id];
	if (!ssh) {
		errno = ENOENT;
		return -1;
	}

	if (type >= ssh->cb_count) {
		errno = ENOENT;
		return -1;
	}

	if (ssh->cb[type].attr_count) {
		int err;
		struct nfattr *tb[ssh->cb[type].attr_count];
		struct nfattr *attr = NFM_NFA(NLMSG_DATA(nlh));
		int min_len = NLMSG_SPACE(sizeof(struct nfgenmsg));
		int len = nlh->nlmsg_len - NLMSG_ALIGN(min_len);

		err = nfnl_parse_attr(tb, ssh->cb[type].attr_count, attr, len);
		if (err == -1)
			return -1;

		if (ssh->cb[type].call) {
			/*
			 * On error, the callback returns NFNL_CB_FAILURE and
			 * errno must be explicitely set. On success, 
			 * NFNL_CB_STOP is returned and we're done, otherwise 
			 * NFNL_CB_CONTINUE means that we want to continue 
			 * data processing.
			 */
			return ssh->cb[type].call(nlh,
						  tb,
						  ssh->cb[type].data);
		}
	}
	/* no callback set, continue data processing */
	return 1;
}


static int nfnl_is_error(struct nfnl_handle *h, struct nlmsghdr *nlh)
{
	/* This message is an ACK or a DONE */
	if (nlh->nlmsg_type == NLMSG_ERROR ||
	    (nlh->nlmsg_type == NLMSG_DONE &&
	     nlh->nlmsg_flags & NLM_F_MULTI)) {
		if (nlh->nlmsg_len < NLMSG_ALIGN(sizeof(struct nlmsgerr))) {
			errno = EBADMSG;
			return 1;
		}
		errno = -(*((int *)NLMSG_DATA(nlh)));
		return 1;
	}
	return 0;
}

void nfnl_subsys_close(struct nfnl_subsys_handle *ssh)
{
	assert(ssh);

	ssh->subscriptions = 0;
	ssh->cb_count = 0;
	if (ssh->cb) {
		free(ssh->cb);
		ssh->cb = NULL;
	}
}

int nfnl_parse_attr(struct nfattr *tb[], int max, struct nfattr *nfa, int len)
{
	assert(tb);
	assert(max > 0);
	assert(nfa);

	memset(tb, 0, sizeof(struct nfattr *) * max);

	while (NFA_OK(nfa, len)) {
		if (NFA_TYPE(nfa) <= max)
			tb[NFA_TYPE(nfa)-1] = nfa;
                nfa = NFA_NEXT(nfa,len);
	}

	return len;
}

int nfnl_close(struct nfnl_handle *nfnlh)
{
	int i, ret=0;

	assert(nfnlh);

	for (i = 0; i < NFNL_MAX_SUBSYS; i++)
		nfnl_subsys_close(&nfnlh->subsys[i]);

	ret = close(nfnlh->fd);
	if (ret < 0)
		return ret;

	free(nfnlh);
	return ret;
}

int nfct_build_conntrack(struct nfnl_subsys_handle *ssh,
		      struct nfnlhdr *req,
		      size_t size,
		      u_int16_t type,
		      u_int16_t flags,
		      const struct nf_conntrack *ct)
{
	memset(req, 0, size);
	nfnl_fill_hdr(ssh, &req->nlh, 0, ct->tuple.l3protonum, 0, type, flags);

	__build_tuple(req, size,
			      &ct->tuple, 
			      CTA_TUPLE_ORIG);
	__build_protoinfo(req, size);

	return 0;
}

void __build_tuple(struct nfnlhdr *req, 
		   size_t size, 
		   const struct __nfct_tuple *t, 
		   const int type)
{
	struct nfattr *nest;

	nest = nfnl_nest(&req->nlh, size, type);

	__build_tuple_ip(req, size, t);
	__build_tuple_proto(req, size, t);

	nfnl_nest_end(&req->nlh, nest);
}

void __build_tuple_ip(struct nfnlhdr *req, 
		      size_t size,
		      const struct __nfct_tuple *t)
{
	struct nfattr *nest;

	nest = nfnl_nest(&req->nlh, size, CTA_TUPLE_IP);
	nfnl_addattr_l(&req->nlh, size, CTA_IP_V4_SRC, &t->src,
			       sizeof(u_int32_t));
	nfnl_addattr_l(&req->nlh, size, CTA_IP_V4_DST, &t->dst,
			       sizeof(u_int32_t)); 
	nfnl_nest_end(&req->nlh, nest);
}

void __build_tuple_proto(struct nfnlhdr *req,
			 size_t size,
			 const struct __nfct_tuple *t)
{
	struct nfattr *nest;

	nest = nfnl_nest(&req->nlh, size, CTA_TUPLE_PROTO);

	nfnl_addattr_l(&req->nlh, size, CTA_PROTO_NUM, &t->protonum,
		       sizeof(u_int8_t));

	nfnl_addattr_l(&req->nlh, size, CTA_PROTO_SRC_PORT,
			       &t->l4src, sizeof(u_int16_t));

	nfnl_addattr_l(&req->nlh, size, CTA_PROTO_DST_PORT,
			       &t->l4dst, sizeof(u_int16_t));
	nfnl_nest_end(&req->nlh, nest);
}

static void __build_protoinfo(struct nfnlhdr *req, size_t size)
{
	struct nfattr *nest, *nest_proto;
	nest = nfnl_nest(&req->nlh, size, CTA_PROTOINFO);
	nest_proto = nfnl_nest(&req->nlh, size, CTA_PROTOINFO_TCP);

	nfnl_nest_end(&req->nlh, nest_proto);
	nfnl_nest_end(&req->nlh, nest);
}

int nfnl_addattr_l(struct nlmsghdr *n, int maxlen, int type, const void *data,
		   int alen)
{
	int len = NFA_LENGTH(alen);
	struct nfattr *nfa;

	assert(n);
	assert(maxlen > 0);
	assert(type >= 0);

	if ((NLMSG_ALIGN(n->nlmsg_len) + len) > maxlen) {
		errno = ENOSPC;
		return -1;
	}

	nfa = NLMSG_TAIL(n);
	nfa->nfa_type = type;
	nfa->nfa_len = len;
	memcpy(NFA_DATA(nfa), data, alen);
	n->nlmsg_len = (NLMSG_ALIGN(n->nlmsg_len) + NFA_ALIGN(len));
	return 0;
}
