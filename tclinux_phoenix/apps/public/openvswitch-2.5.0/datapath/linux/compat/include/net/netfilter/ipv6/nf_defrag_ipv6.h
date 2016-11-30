#ifndef _NF_DEFRAG_IPV6_WRAPPER_H
#define _NF_DEFRAG_IPV6_WRAPPER_H

#include <linux/kconfig.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37)
#include_next <net/netfilter/ipv6/nf_defrag_ipv6.h>
#endif

/* Upstream commit 029f7f3b8701 ("netfilter: ipv6: nf_defrag: avoid/free clone
 * operations") changed the semantics of nf_ct_frag6_gather(), so we backport
 * it for all prior kernels.
 */
#if defined(HAVE_NF_CT_FRAG6_CONSUME_ORIG) || \
    defined(HAVE_NF_CT_FRAG6_OUTPUT)
#if defined(OVS_FRAGMENT_BACKPORT)
#define OVS_NF_DEFRAG6_BACKPORT 1
struct sk_buff *rpl_nf_ct_frag6_gather(struct sk_buff *skb, u32 user);
int __init rpl_nf_ct_frag6_init(void);
void rpl_nf_ct_frag6_cleanup(void);
void rpl_nf_ct_frag6_consume_orig(struct sk_buff *skb);
#else /* !OVS_FRAGMENT_BACKPORT */
static inline struct sk_buff *rpl_nf_ct_frag6_gather(struct sk_buff *skb,
						     u32 user)
{
	return skb;
}
static inline int __init rpl_nf_ct_frag6_init(void) { return 0; }
static inline void rpl_nf_ct_frag6_cleanup(void) { }
static inline void rpl_nf_ct_frag6_consume_orig(struct sk_buff *skb) { }
#endif /* OVS_FRAGMENT_BACKPORT */
#define nf_ct_frag6_gather rpl_nf_ct_frag6_gather
#else /* HAVE_NF_CT_FRAG6_CONSUME_ORIG */
static inline int __init rpl_nf_ct_frag6_init(void) { return 0; }
static inline void rpl_nf_ct_frag6_cleanup(void) { }
static inline void rpl_nf_ct_frag6_consume_orig(struct sk_buff *skb) { }
#endif /* HAVE_NF_CT_FRAG6_CONSUME_ORIG */
#define nf_ct_frag6_init rpl_nf_ct_frag6_init
#define nf_ct_frag6_cleanup rpl_nf_ct_frag6_cleanup
#define nf_ct_frag6_consume_orig rpl_nf_ct_frag6_consume_orig

#endif /* __NF_DEFRAG_IPV6_WRAPPER_H */
