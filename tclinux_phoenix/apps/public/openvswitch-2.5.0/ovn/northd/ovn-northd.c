/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <config.h>

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

#include "command-line.h"
#include "daemon.h"
#include "dirs.h"
#include "dynamic-string.h"
#include "fatal-signal.h"
#include "hash.h"
#include "hmap.h"
#include "json.h"
#include "ovn/lib/lex.h"
#include "ovn/lib/ovn-nb-idl.h"
#include "ovn/lib/ovn-sb-idl.h"
#include "poll-loop.h"
#include "smap.h"
#include "stream.h"
#include "stream-ssl.h"
#include "unixctl.h"
#include "util.h"
#include "uuid.h"
#include "openvswitch/vlog.h"

VLOG_DEFINE_THIS_MODULE(ovn_northd);

static unixctl_cb_func ovn_northd_exit;

struct northd_context {
    struct ovsdb_idl *ovnnb_idl;
    struct ovsdb_idl *ovnsb_idl;
    struct ovsdb_idl_txn *ovnnb_txn;
    struct ovsdb_idl_txn *ovnsb_txn;
};

static const char *ovnnb_db;
static const char *ovnsb_db;

static const char *default_db(void);

/* Pipeline stages. */

/* The two pipelines in an OVN logical flow table. */
enum ovn_pipeline {
    P_IN,                       /* Ingress pipeline. */
    P_OUT                       /* Egress pipeline. */
};

/* The two purposes for which ovn-northd uses OVN logical datapaths. */
enum ovn_datapath_type {
    DP_SWITCH,                  /* OVN logical switch. */
    DP_ROUTER                   /* OVN logical router. */
};

/* Returns an "enum ovn_stage" built from the arguments.
 *
 * (It's better to use ovn_stage_build() for type-safety reasons, but inline
 * functions can't be used in enums or switch cases.) */
#define OVN_STAGE_BUILD(DP_TYPE, PIPELINE, TABLE) \
    (((DP_TYPE) << 9) | ((PIPELINE) << 8) | (TABLE))

/* A stage within an OVN logical switch or router.
 *
 * An "enum ovn_stage" indicates whether the stage is part of a logical switch
 * or router, whether the stage is part of the ingress or egress pipeline, and
 * the table within that pipeline.  The first three components are combined to
 * form the stage's full name, e.g. S_SWITCH_IN_PORT_SEC,
 * S_ROUTER_OUT_DELIVERY. */
enum ovn_stage {
#define PIPELINE_STAGES                                               \
    /* Logical switch ingress stages. */                              \
    PIPELINE_STAGE(SWITCH, IN,  PORT_SEC,    0, "ls_in_port_sec")     \
    PIPELINE_STAGE(SWITCH, IN,  PRE_ACL,     1, "ls_in_pre_acl")      \
    PIPELINE_STAGE(SWITCH, IN,  ACL,         2, "ls_in_acl")          \
    PIPELINE_STAGE(SWITCH, IN,  L2_LKUP,     3, "ls_in_l2_lkup")      \
                                                                      \
    /* Logical switch egress stages. */                               \
    PIPELINE_STAGE(SWITCH, OUT, PRE_ACL,     0, "ls_out_pre_acl")     \
    PIPELINE_STAGE(SWITCH, OUT, ACL,         1, "ls_out_acl")         \
    PIPELINE_STAGE(SWITCH, OUT, PORT_SEC,    2, "ls_out_port_sec")    \
                                                                      \
    /* Logical router ingress stages. */                              \
    PIPELINE_STAGE(ROUTER, IN,  ADMISSION,   0, "lr_in_admission")    \
    PIPELINE_STAGE(ROUTER, IN,  IP_INPUT,    1, "lr_in_ip_input")     \
    PIPELINE_STAGE(ROUTER, IN,  IP_ROUTING,  2, "lr_in_ip_routing")   \
    PIPELINE_STAGE(ROUTER, IN,  ARP,         3, "lr_in_arp")          \
                                                                      \
    /* Logical router egress stages. */                               \
    PIPELINE_STAGE(ROUTER, OUT, DELIVERY,    0, "lr_out_delivery")

#define PIPELINE_STAGE(DP_TYPE, PIPELINE, STAGE, TABLE, NAME)   \
    S_##DP_TYPE##_##PIPELINE##_##STAGE                          \
        = OVN_STAGE_BUILD(DP_##DP_TYPE, P_##PIPELINE, TABLE),
    PIPELINE_STAGES
#undef PIPELINE_STAGE
};

/* Due to various hard-coded priorities need to implement ACLs, the
 * northbound database supports a smaller range of ACL priorities than
 * are available to logical flows.  This value is added to an ACL
 * priority to determine the ACL's logical flow priority. */
#define OVN_ACL_PRI_OFFSET 1000

/* Returns an "enum ovn_stage" built from the arguments. */
static enum ovn_stage
ovn_stage_build(enum ovn_datapath_type dp_type, enum ovn_pipeline pipeline,
                uint8_t table)
{
    return OVN_STAGE_BUILD(dp_type, pipeline, table);
}

/* Returns the pipeline to which 'stage' belongs. */
static enum ovn_pipeline
ovn_stage_get_pipeline(enum ovn_stage stage)
{
    return (stage >> 8) & 1;
}

/* Returns the table to which 'stage' belongs. */
static uint8_t
ovn_stage_get_table(enum ovn_stage stage)
{
    return stage & 0xff;
}

/* Returns a string name for 'stage'. */
static const char *
ovn_stage_to_str(enum ovn_stage stage)
{
    switch (stage) {
#define PIPELINE_STAGE(DP_TYPE, PIPELINE, STAGE, TABLE, NAME)       \
        case S_##DP_TYPE##_##PIPELINE##_##STAGE: return NAME;
    PIPELINE_STAGES
#undef PIPELINE_STAGE
        default: return "<unknown>";
    }
}

static void
usage(void)
{
    printf("\
%s: OVN northbound management daemon\n\
usage: %s [OPTIONS]\n\
\n\
Options:\n\
  --ovnnb-db=DATABASE       connect to ovn-nb database at DATABASE\n\
                            (default: %s)\n\
  --ovnsb-db=DATABASE       connect to ovn-sb database at DATABASE\n\
                            (default: %s)\n\
  -h, --help                display this help message\n\
  -o, --options             list available options\n\
  -V, --version             display version information\n\
", program_name, program_name, default_db(), default_db());
    daemon_usage();
    vlog_usage();
    stream_usage("database", true, true, false);
}

struct tnlid_node {
    struct hmap_node hmap_node;
    uint32_t tnlid;
};

static void
destroy_tnlids(struct hmap *tnlids)
{
    struct tnlid_node *node, *next;
    HMAP_FOR_EACH_SAFE (node, next, hmap_node, tnlids) {
        hmap_remove(tnlids, &node->hmap_node);
        free(node);
    }
    hmap_destroy(tnlids);
}

static void
add_tnlid(struct hmap *set, uint32_t tnlid)
{
    struct tnlid_node *node = xmalloc(sizeof *node);
    hmap_insert(set, &node->hmap_node, hash_int(tnlid, 0));
    node->tnlid = tnlid;
}

static bool
tnlid_in_use(const struct hmap *set, uint32_t tnlid)
{
    const struct tnlid_node *node;
    HMAP_FOR_EACH_IN_BUCKET (node, hmap_node, hash_int(tnlid, 0), set) {
        if (node->tnlid == tnlid) {
            return true;
        }
    }
    return false;
}

static uint32_t
allocate_tnlid(struct hmap *set, const char *name, uint32_t max,
               uint32_t *hint)
{
    for (uint32_t tnlid = *hint + 1; tnlid != *hint;
         tnlid = tnlid + 1 <= max ? tnlid + 1 : 1) {
        if (!tnlid_in_use(set, tnlid)) {
            add_tnlid(set, tnlid);
            *hint = tnlid;
            return tnlid;
        }
    }

    static struct vlog_rate_limit rl = VLOG_RATE_LIMIT_INIT(1, 1);
    VLOG_WARN_RL(&rl, "all %s tunnel ids exhausted", name);
    return 0;
}

/* The 'key' comes from nbs->header_.uuid or nbr->header_.uuid or
 * sb->external_ids:logical-switch. */
struct ovn_datapath {
    struct hmap_node key_node;  /* Index on 'key'. */
    struct uuid key;            /* (nbs/nbr)->header_.uuid. */

    const struct nbrec_logical_switch *nbs;  /* May be NULL. */
    const struct nbrec_logical_router *nbr;  /* May be NULL. */
    const struct sbrec_datapath_binding *sb; /* May be NULL. */

    struct ovs_list list;       /* In list of similar records. */

    /* Logical router data (digested from nbr). */
    ovs_be32 gateway;

    /* Logical switch data. */
    struct ovn_port **router_ports;
    size_t n_router_ports;

    struct hmap port_tnlids;
    uint32_t port_key_hint;

    bool has_unknown;
};

static struct ovn_datapath *
ovn_datapath_create(struct hmap *datapaths, const struct uuid *key,
                    const struct nbrec_logical_switch *nbs,
                    const struct nbrec_logical_router *nbr,
                    const struct sbrec_datapath_binding *sb)
{
    struct ovn_datapath *od = xzalloc(sizeof *od);
    od->key = *key;
    od->sb = sb;
    od->nbs = nbs;
    od->nbr = nbr;
    hmap_init(&od->port_tnlids);
    od->port_key_hint = 0;
    hmap_insert(datapaths, &od->key_node, uuid_hash(&od->key));
    return od;
}

static void
ovn_datapath_destroy(struct hmap *datapaths, struct ovn_datapath *od)
{
    if (od) {
        /* Don't remove od->list.  It is used within build_datapaths() as a
         * private list and once we've exited that function it is not safe to
         * use it. */
        hmap_remove(datapaths, &od->key_node);
        destroy_tnlids(&od->port_tnlids);
        free(od->router_ports);
        free(od);
    }
}

static struct ovn_datapath *
ovn_datapath_find(struct hmap *datapaths, const struct uuid *uuid)
{
    struct ovn_datapath *od;

    HMAP_FOR_EACH_WITH_HASH (od, key_node, uuid_hash(uuid), datapaths) {
        if (uuid_equals(uuid, &od->key)) {
            return od;
        }
    }
    return NULL;
}

static struct ovn_datapath *
ovn_datapath_from_sbrec(struct hmap *datapaths,
                        const struct sbrec_datapath_binding *sb)
{
    struct uuid key;

    if (!smap_get_uuid(&sb->external_ids, "logical-switch", &key) &&
        !smap_get_uuid(&sb->external_ids, "logical-router", &key)) {
        return NULL;
    }
    return ovn_datapath_find(datapaths, &key);
}

static void
join_datapaths(struct northd_context *ctx, struct hmap *datapaths,
               struct ovs_list *sb_only, struct ovs_list *nb_only,
               struct ovs_list *both)
{
    hmap_init(datapaths);
    list_init(sb_only);
    list_init(nb_only);
    list_init(both);

    const struct sbrec_datapath_binding *sb, *sb_next;
    SBREC_DATAPATH_BINDING_FOR_EACH_SAFE (sb, sb_next, ctx->ovnsb_idl) {
        struct uuid key;
        if (!smap_get_uuid(&sb->external_ids, "logical-switch", &key) &&
            !smap_get_uuid(&sb->external_ids, "logical-router", &key)) {
            ovsdb_idl_txn_add_comment(
                ctx->ovnsb_txn,
                "deleting Datapath_Binding "UUID_FMT" that lacks "
                "external-ids:logical-switch and "
                "external-ids:logical-router",
                UUID_ARGS(&sb->header_.uuid));
            sbrec_datapath_binding_delete(sb);
            continue;
        }

        if (ovn_datapath_find(datapaths, &key)) {
            static struct vlog_rate_limit rl = VLOG_RATE_LIMIT_INIT(5, 1);
            VLOG_INFO_RL(
                &rl, "deleting Datapath_Binding "UUID_FMT" with "
                "duplicate external-ids:logical-switch/router "UUID_FMT,
                UUID_ARGS(&sb->header_.uuid), UUID_ARGS(&key));
            sbrec_datapath_binding_delete(sb);
            continue;
        }

        struct ovn_datapath *od = ovn_datapath_create(datapaths, &key,
                                                      NULL, NULL, sb);
        list_push_back(sb_only, &od->list);
    }

    const struct nbrec_logical_switch *nbs;
    NBREC_LOGICAL_SWITCH_FOR_EACH (nbs, ctx->ovnnb_idl) {
        struct ovn_datapath *od = ovn_datapath_find(datapaths,
                                                    &nbs->header_.uuid);
        if (od) {
            od->nbs = nbs;
            list_remove(&od->list);
            list_push_back(both, &od->list);
        } else {
            od = ovn_datapath_create(datapaths, &nbs->header_.uuid,
                                     nbs, NULL, NULL);
            list_push_back(nb_only, &od->list);
        }
    }

    const struct nbrec_logical_router *nbr;
    NBREC_LOGICAL_ROUTER_FOR_EACH (nbr, ctx->ovnnb_idl) {
        struct ovn_datapath *od = ovn_datapath_find(datapaths,
                                                    &nbr->header_.uuid);
        if (od) {
            if (!od->nbs) {
                od->nbr = nbr;
                list_remove(&od->list);
                list_push_back(both, &od->list);
            } else {
                /* Can't happen! */
                static struct vlog_rate_limit rl = VLOG_RATE_LIMIT_INIT(5, 1);
                VLOG_WARN_RL(&rl,
                             "duplicate UUID "UUID_FMT" in OVN_Northbound",
                             UUID_ARGS(&nbr->header_.uuid));
                continue;
            }
        } else {
            od = ovn_datapath_create(datapaths, &nbr->header_.uuid,
                                     NULL, nbr, NULL);
            list_push_back(nb_only, &od->list);
        }

        od->gateway = 0;
        if (nbr->default_gw) {
            ovs_be32 ip, mask;
            char *error = ip_parse_masked(nbr->default_gw, &ip, &mask);
            if (error || !ip || mask != OVS_BE32_MAX) {
                static struct vlog_rate_limit rl
                    = VLOG_RATE_LIMIT_INIT(5, 1);
                VLOG_WARN_RL(&rl, "bad 'gateway' %s", nbr->default_gw);
                free(error);
            } else {
                od->gateway = ip;
            }
        }
    }
}

static uint32_t
ovn_datapath_allocate_key(struct hmap *dp_tnlids)
{
    static uint32_t hint;
    return allocate_tnlid(dp_tnlids, "datapath", (1u << 24) - 1, &hint);
}

static void
build_datapaths(struct northd_context *ctx, struct hmap *datapaths)
{
    struct ovs_list sb_only, nb_only, both;

    join_datapaths(ctx, datapaths, &sb_only, &nb_only, &both);

    if (!list_is_empty(&nb_only)) {
        /* First index the in-use datapath tunnel IDs. */
        struct hmap dp_tnlids = HMAP_INITIALIZER(&dp_tnlids);
        struct ovn_datapath *od;
        LIST_FOR_EACH (od, list, &both) {
            add_tnlid(&dp_tnlids, od->sb->tunnel_key);
        }

        /* Add southbound record for each unmatched northbound record. */
        LIST_FOR_EACH (od, list, &nb_only) {
            uint16_t tunnel_key = ovn_datapath_allocate_key(&dp_tnlids);
            if (!tunnel_key) {
                break;
            }

            od->sb = sbrec_datapath_binding_insert(ctx->ovnsb_txn);

            char uuid_s[UUID_LEN + 1];
            sprintf(uuid_s, UUID_FMT, UUID_ARGS(&od->key));
            const char *key = od->nbs ? "logical-switch" : "logical-router";
            const struct smap id = SMAP_CONST1(&id, key, uuid_s);
            sbrec_datapath_binding_set_external_ids(od->sb, &id);

            sbrec_datapath_binding_set_tunnel_key(od->sb, tunnel_key);
        }
        destroy_tnlids(&dp_tnlids);
    }

    /* Delete southbound records without northbound matches. */
    struct ovn_datapath *od, *next;
    LIST_FOR_EACH_SAFE (od, next, list, &sb_only) {
        list_remove(&od->list);
        sbrec_datapath_binding_delete(od->sb);
        ovn_datapath_destroy(datapaths, od);
    }
}

struct ovn_port {
    struct hmap_node key_node;  /* Index on 'key'. */
    char *key;                  /* nbs->name, nbr->name, sb->logical_port. */
    char *json_key;             /* 'key', quoted for use in JSON. */

    const struct nbrec_logical_port *nbs;        /* May be NULL. */
    const struct nbrec_logical_router_port *nbr; /* May be NULL. */
    const struct sbrec_port_binding *sb;         /* May be NULL. */

    /* Logical router port data. */
    ovs_be32 ip, mask;          /* 192.168.10.123/24. */
    ovs_be32 network;           /* 192.168.10.0. */
    ovs_be32 bcast;             /* 192.168.10.255. */
    struct eth_addr mac;
    struct ovn_port *peer;

    struct ovn_datapath *od;

    struct ovs_list list;       /* In list of similar records. */
};

static struct ovn_port *
ovn_port_create(struct hmap *ports, const char *key,
                const struct nbrec_logical_port *nbs,
                const struct nbrec_logical_router_port *nbr,
                const struct sbrec_port_binding *sb)
{
    struct ovn_port *op = xzalloc(sizeof *op);

    struct ds json_key = DS_EMPTY_INITIALIZER;
    json_string_escape(key, &json_key);
    op->json_key = ds_steal_cstr(&json_key);

    op->key = xstrdup(key);
    op->sb = sb;
    op->nbs = nbs;
    op->nbr = nbr;
    hmap_insert(ports, &op->key_node, hash_string(op->key, 0));
    return op;
}

static void
ovn_port_destroy(struct hmap *ports, struct ovn_port *port)
{
    if (port) {
        /* Don't remove port->list.  It is used within build_ports() as a
         * private list and once we've exited that function it is not safe to
         * use it. */
        hmap_remove(ports, &port->key_node);
        free(port->json_key);
        free(port->key);
        free(port);
    }
}

static struct ovn_port *
ovn_port_find(struct hmap *ports, const char *name)
{
    struct ovn_port *op;

    HMAP_FOR_EACH_WITH_HASH (op, key_node, hash_string(name, 0), ports) {
        if (!strcmp(op->key, name)) {
            return op;
        }
    }
    return NULL;
}

static uint32_t
ovn_port_allocate_key(struct ovn_datapath *od)
{
    return allocate_tnlid(&od->port_tnlids, "port",
                          (1u << 15) - 1, &od->port_key_hint);
}

static void
join_logical_ports(struct northd_context *ctx,
                   struct hmap *datapaths, struct hmap *ports,
                   struct ovs_list *sb_only, struct ovs_list *nb_only,
                   struct ovs_list *both)
{
    hmap_init(ports);
    list_init(sb_only);
    list_init(nb_only);
    list_init(both);

    const struct sbrec_port_binding *sb;
    SBREC_PORT_BINDING_FOR_EACH (sb, ctx->ovnsb_idl) {
        struct ovn_port *op = ovn_port_create(ports, sb->logical_port,
                                              NULL, NULL, sb);
        list_push_back(sb_only, &op->list);
    }

    struct ovn_datapath *od;
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (od->nbs) {
            for (size_t i = 0; i < od->nbs->n_ports; i++) {
                const struct nbrec_logical_port *nbs = od->nbs->ports[i];
                struct ovn_port *op = ovn_port_find(ports, nbs->name);
                if (op) {
                    if (op->nbs || op->nbr) {
                        static struct vlog_rate_limit rl
                            = VLOG_RATE_LIMIT_INIT(5, 1);
                        VLOG_WARN_RL(&rl, "duplicate logical port %s",
                                     nbs->name);
                        continue;
                    }
                    op->nbs = nbs;
                    list_remove(&op->list);
                    list_push_back(both, &op->list);
                } else {
                    op = ovn_port_create(ports, nbs->name, nbs, NULL, NULL);
                    list_push_back(nb_only, &op->list);
                }

                op->od = od;
            }
        } else {
            for (size_t i = 0; i < od->nbr->n_ports; i++) {
                const struct nbrec_logical_router_port *nbr
                    = od->nbr->ports[i];

                struct eth_addr mac;
                if (!eth_addr_from_string(nbr->mac, &mac)) {
                    static struct vlog_rate_limit rl
                        = VLOG_RATE_LIMIT_INIT(5, 1);
                    VLOG_WARN_RL(&rl, "bad 'mac' %s", nbr->mac);
                    continue;
                }

                ovs_be32 ip, mask;
                char *error = ip_parse_masked(nbr->network, &ip, &mask);
                if (error || mask == OVS_BE32_MAX || !ip_is_cidr(mask)) {
                    static struct vlog_rate_limit rl
                        = VLOG_RATE_LIMIT_INIT(5, 1);
                    VLOG_WARN_RL(&rl, "bad 'network' %s", nbr->network);
                    free(error);
                    continue;
                }

                struct ovn_port *op = ovn_port_find(ports, nbr->name);
                if (op) {
                    if (op->nbs || op->nbr) {
                        static struct vlog_rate_limit rl
                            = VLOG_RATE_LIMIT_INIT(5, 1);
                        VLOG_WARN_RL(&rl, "duplicate logical router port %s",
                                     nbr->name);
                        continue;
                    }
                    op->nbr = nbr;
                    list_remove(&op->list);
                    list_push_back(both, &op->list);
                } else {
                    op = ovn_port_create(ports, nbr->name, NULL, nbr, NULL);
                    list_push_back(nb_only, &op->list);
                }

                op->ip = ip;
                op->mask = mask;
                op->network = ip & mask;
                op->bcast = ip | ~mask;
                op->mac = mac;

                op->od = od;
            }
        }
    }

    /* Connect logical router ports, and logical switch ports of type "router",
     * to their peers. */
    struct ovn_port *op;
    HMAP_FOR_EACH (op, key_node, ports) {
        if (op->nbs && !strcmp(op->nbs->type, "router")) {
            const char *peer_name = smap_get(&op->nbs->options, "router-port");
            if (!peer_name) {
                continue;
            }

            struct ovn_port *peer = ovn_port_find(ports, peer_name);
            if (!peer || !peer->nbr) {
                continue;
            }

            peer->peer = op;
            op->peer = peer;
            op->od->router_ports = xrealloc(
                op->od->router_ports,
                sizeof *op->od->router_ports * (op->od->n_router_ports + 1));
            op->od->router_ports[op->od->n_router_ports++] = op;
        } else if (op->nbr && op->nbr->peer) {
            op->peer = ovn_port_find(ports, op->nbr->name);
        }
    }
}

static void
ovn_port_update_sbrec(const struct ovn_port *op)
{
    sbrec_port_binding_set_datapath(op->sb, op->od->sb);
    if (op->nbr) {
        sbrec_port_binding_set_type(op->sb, "patch");

        const char *peer = op->peer ? op->peer->key : "<error>";
        const struct smap ids = SMAP_CONST1(&ids, "peer", peer);
        sbrec_port_binding_set_options(op->sb, &ids);

        sbrec_port_binding_set_parent_port(op->sb, NULL);
        sbrec_port_binding_set_tag(op->sb, NULL, 0);
        sbrec_port_binding_set_mac(op->sb, NULL, 0);
    } else {
        if (strcmp(op->nbs->type, "router")) {
            sbrec_port_binding_set_type(op->sb, op->nbs->type);
            sbrec_port_binding_set_options(op->sb, &op->nbs->options);
        } else {
            sbrec_port_binding_set_type(op->sb, "patch");

            const char *router_port = smap_get(&op->nbs->options,
                                               "router-port");
            if (!router_port) {
                router_port = "<error>";
            }
            const struct smap ids = SMAP_CONST1(&ids, "peer", router_port);
            sbrec_port_binding_set_options(op->sb, &ids);
        }
        sbrec_port_binding_set_parent_port(op->sb, op->nbs->parent_name);
        sbrec_port_binding_set_tag(op->sb, op->nbs->tag, op->nbs->n_tag);
        sbrec_port_binding_set_mac(op->sb, (const char **) op->nbs->addresses,
                                   op->nbs->n_addresses);
    }
}

static void
build_ports(struct northd_context *ctx, struct hmap *datapaths,
            struct hmap *ports)
{
    struct ovs_list sb_only, nb_only, both;

    join_logical_ports(ctx, datapaths, ports, &sb_only, &nb_only, &both);

    /* For logical ports that are in both databases, update the southbound
     * record based on northbound data.  Also index the in-use tunnel_keys. */
    struct ovn_port *op, *next;
    LIST_FOR_EACH_SAFE (op, next, list, &both) {
        ovn_port_update_sbrec(op);

        add_tnlid(&op->od->port_tnlids, op->sb->tunnel_key);
        if (op->sb->tunnel_key > op->od->port_key_hint) {
            op->od->port_key_hint = op->sb->tunnel_key;
        }
    }

    /* Add southbound record for each unmatched northbound record. */
    LIST_FOR_EACH_SAFE (op, next, list, &nb_only) {
        uint16_t tunnel_key = ovn_port_allocate_key(op->od);
        if (!tunnel_key) {
            continue;
        }

        op->sb = sbrec_port_binding_insert(ctx->ovnsb_txn);
        ovn_port_update_sbrec(op);

        sbrec_port_binding_set_logical_port(op->sb, op->key);
        sbrec_port_binding_set_tunnel_key(op->sb, tunnel_key);
    }

    /* Delete southbound records without northbound matches. */
    LIST_FOR_EACH_SAFE(op, next, list, &sb_only) {
        list_remove(&op->list);
        sbrec_port_binding_delete(op->sb);
        ovn_port_destroy(ports, op);
    }
}

#define OVN_MIN_MULTICAST 32768
#define OVN_MAX_MULTICAST 65535

struct multicast_group {
    const char *name;
    uint16_t key;               /* OVN_MIN_MULTICAST...OVN_MAX_MULTICAST. */
};

#define MC_FLOOD "_MC_flood"
static const struct multicast_group mc_flood = { MC_FLOOD, 65535 };

#define MC_UNKNOWN "_MC_unknown"
static const struct multicast_group mc_unknown = { MC_UNKNOWN, 65534 };

static bool
multicast_group_equal(const struct multicast_group *a,
                      const struct multicast_group *b)
{
    return !strcmp(a->name, b->name) && a->key == b->key;
}

/* Multicast group entry. */
struct ovn_multicast {
    struct hmap_node hmap_node; /* Index on 'datapath' and 'key'. */
    struct ovn_datapath *datapath;
    const struct multicast_group *group;

    struct ovn_port **ports;
    size_t n_ports, allocated_ports;
};

static uint32_t
ovn_multicast_hash(const struct ovn_datapath *datapath,
                   const struct multicast_group *group)
{
    return hash_pointer(datapath, group->key);
}

static struct ovn_multicast *
ovn_multicast_find(struct hmap *mcgroups, struct ovn_datapath *datapath,
                   const struct multicast_group *group)
{
    struct ovn_multicast *mc;

    HMAP_FOR_EACH_WITH_HASH (mc, hmap_node,
                             ovn_multicast_hash(datapath, group), mcgroups) {
        if (mc->datapath == datapath
            && multicast_group_equal(mc->group, group)) {
            return mc;
        }
    }
    return NULL;
}

static void
ovn_multicast_add(struct hmap *mcgroups, const struct multicast_group *group,
                  struct ovn_port *port)
{
    struct ovn_datapath *od = port->od;
    struct ovn_multicast *mc = ovn_multicast_find(mcgroups, od, group);
    if (!mc) {
        mc = xmalloc(sizeof *mc);
        hmap_insert(mcgroups, &mc->hmap_node, ovn_multicast_hash(od, group));
        mc->datapath = od;
        mc->group = group;
        mc->n_ports = 0;
        mc->allocated_ports = 4;
        mc->ports = xmalloc(mc->allocated_ports * sizeof *mc->ports);
    }
    if (mc->n_ports >= mc->allocated_ports) {
        mc->ports = x2nrealloc(mc->ports, &mc->allocated_ports,
                               sizeof *mc->ports);
    }
    mc->ports[mc->n_ports++] = port;
}

static void
ovn_multicast_destroy(struct hmap *mcgroups, struct ovn_multicast *mc)
{
    if (mc) {
        hmap_remove(mcgroups, &mc->hmap_node);
        free(mc->ports);
        free(mc);
    }
}

static void
ovn_multicast_update_sbrec(const struct ovn_multicast *mc,
                           const struct sbrec_multicast_group *sb)
{
    struct sbrec_port_binding **ports = xmalloc(mc->n_ports * sizeof *ports);
    for (size_t i = 0; i < mc->n_ports; i++) {
        ports[i] = CONST_CAST(struct sbrec_port_binding *, mc->ports[i]->sb);
    }
    sbrec_multicast_group_set_ports(sb, ports, mc->n_ports);
    free(ports);
}

/* Logical flow generation.
 *
 * This code generates the Logical_Flow table in the southbound database, as a
 * function of most of the northbound database.
 */

struct ovn_lflow {
    struct hmap_node hmap_node;

    struct ovn_datapath *od;
    enum ovn_stage stage;
    uint16_t priority;
    char *match;
    char *actions;
};

static size_t
ovn_lflow_hash(const struct ovn_lflow *lflow)
{
    size_t hash = uuid_hash(&lflow->od->key);
    hash = hash_2words((lflow->stage << 16) | lflow->priority, hash);
    hash = hash_string(lflow->match, hash);
    return hash_string(lflow->actions, hash);
}

static bool
ovn_lflow_equal(const struct ovn_lflow *a, const struct ovn_lflow *b)
{
    return (a->od == b->od
            && a->stage == b->stage
            && a->priority == b->priority
            && !strcmp(a->match, b->match)
            && !strcmp(a->actions, b->actions));
}

static void
ovn_lflow_init(struct ovn_lflow *lflow, struct ovn_datapath *od,
              enum ovn_stage stage, uint16_t priority,
              char *match, char *actions)
{
    lflow->od = od;
    lflow->stage = stage;
    lflow->priority = priority;
    lflow->match = match;
    lflow->actions = actions;
}

/* Adds a row with the specified contents to the Logical_Flow table. */
static void
ovn_lflow_add(struct hmap *lflow_map, struct ovn_datapath *od,
              enum ovn_stage stage, uint16_t priority,
              const char *match, const char *actions)
{
    struct ovn_lflow *lflow = xmalloc(sizeof *lflow);
    ovn_lflow_init(lflow, od, stage, priority,
                   xstrdup(match), xstrdup(actions));
    hmap_insert(lflow_map, &lflow->hmap_node, ovn_lflow_hash(lflow));
}

static struct ovn_lflow *
ovn_lflow_find(struct hmap *lflows, struct ovn_datapath *od,
               enum ovn_stage stage, uint16_t priority,
               const char *match, const char *actions)
{
    struct ovn_lflow target;
    ovn_lflow_init(&target, od, stage, priority,
                   CONST_CAST(char *, match), CONST_CAST(char *, actions));

    struct ovn_lflow *lflow;
    HMAP_FOR_EACH_WITH_HASH (lflow, hmap_node, ovn_lflow_hash(&target),
                             lflows) {
        if (ovn_lflow_equal(lflow, &target)) {
            return lflow;
        }
    }
    return NULL;
}

static void
ovn_lflow_destroy(struct hmap *lflows, struct ovn_lflow *lflow)
{
    if (lflow) {
        hmap_remove(lflows, &lflow->hmap_node);
        free(lflow->match);
        free(lflow->actions);
        free(lflow);
    }
}

/* Appends port security constraints on L2 address field 'eth_addr_field'
 * (e.g. "eth.src" or "eth.dst") to 'match'.  'port_security', with
 * 'n_port_security' elements, is the collection of port_security constraints
 * from an OVN_NB Logical_Port row. */
static void
build_port_security(const char *eth_addr_field,
                    char **port_security, size_t n_port_security,
                    struct ds *match)
{
    size_t base_len = match->length;
    ds_put_format(match, " && %s == {", eth_addr_field);

    size_t n = 0;
    for (size_t i = 0; i < n_port_security; i++) {
        struct eth_addr ea;

        if (eth_addr_from_string(port_security[i], &ea)) {
            ds_put_format(match, ETH_ADDR_FMT, ETH_ADDR_ARGS(ea));
            ds_put_char(match, ' ');
            n++;
        }
    }
    ds_chomp(match, ' ');
    ds_put_cstr(match, "}");

    if (!n) {
        match->length = base_len;
    }
}

static bool
lport_is_enabled(const struct nbrec_logical_port *lport)
{
    return !lport->enabled || *lport->enabled;
}

static bool
has_stateful_acl(struct ovn_datapath *od)
{
    for (size_t i = 0; i < od->nbs->n_acls; i++) {
        struct nbrec_acl *acl = od->nbs->acls[i];
        if (!strcmp(acl->action, "allow-related")) {
            return true;
        }
    }

    return false;
}

static void
build_acls(struct ovn_datapath *od, struct hmap *lflows, struct hmap *ports)
{
    bool has_stateful = has_stateful_acl(od);
    struct ovn_port *op;
    struct ds match_in, match_out;

    /* Ingress and Egress Pre-ACL Table (Priority 0): Packets are
     * allowed by default. */
    ovn_lflow_add(lflows, od, S_SWITCH_IN_PRE_ACL, 0, "1", "next;");
    ovn_lflow_add(lflows, od, S_SWITCH_OUT_PRE_ACL, 0, "1", "next;");

    /* Ingress and Egress ACL Table (Priority 0): Packets are allowed by
     * default.  A related rule at priority 1 is added below if there
     * are any stateful ACLs in this datapath. */
    ovn_lflow_add(lflows, od, S_SWITCH_IN_ACL, 0, "1", "next;");
    ovn_lflow_add(lflows, od, S_SWITCH_OUT_ACL, 0, "1", "next;");

    /* If there are any stateful ACL rules in this dapapath, we must
     * send all IP packets through the conntrack action, which handles
     * defragmentation, in order to match L4 headers. */
    if (has_stateful) {
        HMAP_FOR_EACH (op, key_node, ports) {
            if (op->od == od && !strcmp(op->nbs->type, "router")) {
                /* Can't use ct() for router ports. Consider the following configuration:
                lp1(10.0.0.2) on hostA--ls1--lr0--ls2--lp2(10.0.1.2) on hostB,
                For a ping from lp1 to lp2, First, the response will go through ct()
                with a zone for lp2 in the ls2 ingress pipeline on hostB.
                That ct zone knows about this connection. Next, it goes through ct()
                with the zone for the router port in the egress pipeline of ls2 on hostB.
                This zone does not know about the connection, as the icmp request
                went through the logical router on hostA, not hostB. This would only work
                with distributed conntrack state across all chassis. */

                ds_init(&match_in);
                ds_init(&match_out);
                ds_put_format(&match_in, "ip && inport == %s", op->json_key);
                ds_put_format(&match_out, "ip && outport == %s", op->json_key);
                ovn_lflow_add(lflows, od, S_SWITCH_IN_PRE_ACL, 110, ds_cstr(&match_in), "next;");
                ovn_lflow_add(lflows, od, S_SWITCH_OUT_PRE_ACL, 110, ds_cstr(&match_out), "next;");

                ds_destroy(&match_in);
                ds_destroy(&match_out);
            }
        }

        /* Ingress and Egress Pre-ACL Table (Priority 100).
         *
         * Regardless of whether the ACL is "from-lport" or "to-lport",
         * we need rules in both the ingress and egress table, because
         * the return traffic needs to be followed. */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_PRE_ACL, 100, "ip", "ct_next;");
        ovn_lflow_add(lflows, od, S_SWITCH_OUT_PRE_ACL, 100, "ip", "ct_next;");

        /* Ingress and Egress ACL Table (Priority 1).
         *
         * By default, traffic is allowed.  This is partially handled by
         * the Priority 0 ACL flows added earlier, but we also need to
         * commit IP flows.  This is because, while the initiater's
         * direction may not have any stateful rules, the server's may
         * and then its return traffic would not have an associated
         * conntrack entry and would return "+invalid". */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_ACL, 1, "ip",
                      "ct_commit; next;");
        ovn_lflow_add(lflows, od, S_SWITCH_OUT_ACL, 1, "ip",
                      "ct_commit; next;");

        /* Ingress and Egress ACL Table (Priority 65535).
         *
         * Always drop traffic that's in an invalid state.  This is
         * enforced at a higher priority than ACLs can be defined. */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_ACL, UINT16_MAX,
                      "ct.inv", "drop;");
        ovn_lflow_add(lflows, od, S_SWITCH_OUT_ACL, UINT16_MAX,
                      "ct.inv", "drop;");

        /* Ingress and Egress ACL Table (Priority 65535).
         *
         * Always allow traffic that is established to a committed
         * conntrack entry.  This is enforced at a higher priority than
         * ACLs can be defined. */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_ACL, UINT16_MAX,
                      "ct.est && !ct.rel && !ct.new && !ct.inv",
                      "next;");
        ovn_lflow_add(lflows, od, S_SWITCH_OUT_ACL, UINT16_MAX,
                      "ct.est && !ct.rel && !ct.new && !ct.inv",
                      "next;");

        /* Ingress and Egress ACL Table (Priority 65535).
         *
         * Always allow traffic that is related to an existing conntrack
         * entry.  This is enforced at a higher priority than ACLs can
         * be defined.
         *
         * NOTE: This does not support related data sessions (eg,
         * a dynamically negotiated FTP data channel), but will allow
         * related traffic such as an ICMP Port Unreachable through
         * that's generated from a non-listening UDP port.  */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_ACL, UINT16_MAX,
                      "!ct.est && ct.rel && !ct.new && !ct.inv",
                      "next;");
        ovn_lflow_add(lflows, od, S_SWITCH_OUT_ACL, UINT16_MAX,
                      "!ct.est && ct.rel && !ct.new && !ct.inv",
                      "next;");
    }

    /* Ingress or Egress ACL Table (Various priorities). */
    for (size_t i = 0; i < od->nbs->n_acls; i++) {
        struct nbrec_acl *acl = od->nbs->acls[i];
        bool ingress = !strcmp(acl->direction, "from-lport") ? true :false;
        enum ovn_stage stage = ingress ? S_SWITCH_IN_ACL : S_SWITCH_OUT_ACL;

        if (!strcmp(acl->action, "allow")) {
            /* If there are any stateful flows, we must even commit "allow"
             * actions.  This is because, while the initiater's
             * direction may not have any stateful rules, the server's
             * may and then its return traffic would not have an
             * associated conntrack entry and would return "+invalid". */
            const char *actions = has_stateful ? "ct_commit; next;" : "next;";
            ovn_lflow_add(lflows, od, stage,
                          acl->priority + OVN_ACL_PRI_OFFSET,
                          acl->match, actions);
        } else if (!strcmp(acl->action, "allow-related")) {
            struct ds match = DS_EMPTY_INITIALIZER;

            /* Commit the connection tracking entry, which allows all
             * other traffic related to this entry to flow due to the
             * 65535 priority flow defined earlier. */
            ds_put_format(&match, "ct.new && (%s)", acl->match);
            ovn_lflow_add(lflows, od, stage,
                          acl->priority + OVN_ACL_PRI_OFFSET,
                          ds_cstr(&match), "ct_commit; next;");

            ds_destroy(&match);
        } else if (!strcmp(acl->action, "drop")) {
            ovn_lflow_add(lflows, od, stage,
                          acl->priority + OVN_ACL_PRI_OFFSET,
                          acl->match, "drop;");
        } else if (!strcmp(acl->action, "reject")) {
            /* xxx Need to support "reject". */
            VLOG_INFO("reject is not a supported action");
            ovn_lflow_add(lflows, od, stage,
                          acl->priority + OVN_ACL_PRI_OFFSET,
                          acl->match, "drop;");
        }
    }
}

static void
build_lswitch_flows(struct hmap *datapaths, struct hmap *ports,
                    struct hmap *lflows, struct hmap *mcgroups)
{
    /* This flow table structure is documented in ovn-northd(8), so please
     * update ovn-northd.8.xml if you change anything. */

    /* Build pre-ACL and ACL tables for both ingress and egress.
     * Ingress tables 1 and 2.  Egress tables 0 and 1. */
    struct ovn_datapath *od;
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbs) {
            continue;
        }

        build_acls(od, lflows, ports);
    }

    /* Logical switch ingress table 0: Admission control framework (priority
     * 100). */
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbs) {
            continue;
        }

        /* Logical VLANs not supported. */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_PORT_SEC, 100, "vlan.present",
                      "drop;");

        /* Broadcast/multicast source address is invalid. */
        ovn_lflow_add(lflows, od, S_SWITCH_IN_PORT_SEC, 100, "eth.src[40]",
                      "drop;");

        /* Port security flows have priority 50 (see below) and will continue
         * to the next table if packet source is acceptable. */
    }

    /* Logical switch ingress table 0: Ingress port security (priority 50). */
    struct ovn_port *op;
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbs) {
            continue;
        }

        if (!lport_is_enabled(op->nbs)) {
            /* Drop packets from disabled logical ports (since logical flow
             * tables are default-drop). */
            continue;
        }

        struct ds match = DS_EMPTY_INITIALIZER;
        ds_put_format(&match, "inport == %s", op->json_key);
        build_port_security("eth.src",
                            op->nbs->port_security, op->nbs->n_port_security,
                            &match);
        ovn_lflow_add(lflows, op->od, S_SWITCH_IN_PORT_SEC, 50,
                      ds_cstr(&match), "next;");
        ds_destroy(&match);
    }

    /* Ingress table 3: Destination lookup, broadcast and multicast handling
     * (priority 100). */
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbs) {
            continue;
        }

        if (lport_is_enabled(op->nbs)) {
            ovn_multicast_add(mcgroups, &mc_flood, op);
        }
    }
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbs) {
            continue;
        }

        ovn_lflow_add(lflows, od, S_SWITCH_IN_L2_LKUP, 100, "eth.mcast",
                      "outport = \""MC_FLOOD"\"; output;");
    }

    /* Ingress table 3: Destination lookup, unicast handling (priority 50), */
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbs) {
            continue;
        }

        for (size_t i = 0; i < op->nbs->n_addresses; i++) {
            struct eth_addr mac;

            if (eth_addr_from_string(op->nbs->addresses[i], &mac)) {
                struct ds match, actions;

                ds_init(&match);
                ds_put_format(&match, "eth.dst == "ETH_ADDR_FMT,
                              ETH_ADDR_ARGS(mac));

                ds_init(&actions);
                ds_put_format(&actions, "outport = %s; output;", op->json_key);
                ovn_lflow_add(lflows, op->od, S_SWITCH_IN_L2_LKUP, 50,
                              ds_cstr(&match), ds_cstr(&actions));
                ds_destroy(&actions);
                ds_destroy(&match);
            } else if (!strcmp(op->nbs->addresses[i], "unknown")) {
                if (lport_is_enabled(op->nbs)) {
                    ovn_multicast_add(mcgroups, &mc_unknown, op);
                    op->od->has_unknown = true;
                }
            } else {
                static struct vlog_rate_limit rl = VLOG_RATE_LIMIT_INIT(1, 1);

                VLOG_INFO_RL(&rl,
                             "%s: invalid syntax '%s' in addresses column",
                             op->nbs->name, op->nbs->addresses[i]);
            }
        }
    }

    /* Ingress table 3: Destination lookup for unknown MACs (priority 0). */
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbs) {
            continue;
        }

        if (od->has_unknown) {
            ovn_lflow_add(lflows, od, S_SWITCH_IN_L2_LKUP, 0, "1",
                          "outport = \""MC_UNKNOWN"\"; output;");
        }
    }

    /* Egress table 2: Egress port security multicast/broadcast (priority
     * 100). */
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbs) {
            continue;
        }

        ovn_lflow_add(lflows, od, S_SWITCH_OUT_PORT_SEC, 100, "eth.mcast",
                      "output;");
    }

    /* Egress table 2: Egress port security (priorities 50 and 150).
     *
     * Priority 50 rules implement port security for enabled logical port.
     *
     * Priority 150 rules drop packets to disabled logical ports, so that they
     * don't even receive multicast or broadcast packets. */
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbs) {
            continue;
        }

        struct ds match = DS_EMPTY_INITIALIZER;
        ds_put_format(&match, "outport == %s", op->json_key);
        if (lport_is_enabled(op->nbs)) {
            build_port_security("eth.dst", op->nbs->port_security,
                                op->nbs->n_port_security, &match);
            ovn_lflow_add(lflows, op->od, S_SWITCH_OUT_PORT_SEC, 50,
                          ds_cstr(&match), "output;");
        } else {
            ovn_lflow_add(lflows, op->od, S_SWITCH_OUT_PORT_SEC, 150,
                          ds_cstr(&match), "drop;");
        }

        ds_destroy(&match);
    }
}

static bool
lrport_is_enabled(const struct nbrec_logical_router_port *lrport)
{
    return !lrport->enabled || *lrport->enabled;
}

static void
add_route(struct hmap *lflows, struct ovn_datapath *od,
          ovs_be32 network, ovs_be32 mask, ovs_be32 gateway)
{
    char *match = xasprintf("ip4.dst == "IP_FMT"/"IP_FMT,
                            IP_ARGS(network), IP_ARGS(mask));

    struct ds actions = DS_EMPTY_INITIALIZER;
    ds_put_cstr(&actions, "ip.ttl--; reg0 = ");
    if (gateway) {
        ds_put_format(&actions, IP_FMT, IP_ARGS(gateway));
    } else {
        ds_put_cstr(&actions, "ip4.dst");
    }
    ds_put_cstr(&actions, "; next;");

    /* The priority here is calculated to implement longest-prefix-match
     * routing. */
    ovn_lflow_add(lflows, od, S_ROUTER_IN_IP_ROUTING,
                  count_1bits(ntohl(mask)), match, ds_cstr(&actions));
    ds_destroy(&actions);
    free(match);
}

static void
build_lrouter_flows(struct hmap *datapaths, struct hmap *ports,
                    struct hmap *lflows)
{
    /* This flow table structure is documented in ovn-northd(8), so please
     * update ovn-northd.8.xml if you change anything. */

    /* Logical router ingress table 0: Admission control framework. */
    struct ovn_datapath *od;
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbr) {
            continue;
        }

        /* Logical VLANs not supported.
         * Broadcast/multicast source address is invalid. */
        ovn_lflow_add(lflows, od, S_ROUTER_IN_ADMISSION, 100,
                      "vlan.present || eth.src[40]", "drop;");
    }

    /* Logical router ingress table 0: match (priority 50). */
    struct ovn_port *op;
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbr) {
            continue;
        }

        if (!lrport_is_enabled(op->nbr)) {
            /* Drop packets from disabled logical ports (since logical flow
             * tables are default-drop). */
            continue;
        }

        char *match = xasprintf(
            "(eth.mcast || eth.dst == "ETH_ADDR_FMT") && inport == %s",
            ETH_ADDR_ARGS(op->mac), op->json_key);
        ovn_lflow_add(lflows, op->od, S_ROUTER_IN_ADMISSION, 50,
                      match, "next;");
        free(match);
    }

    /* Logical router ingress table 1: IP Input. */
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbr) {
            continue;
        }

        /* L3 admission control: drop multicast and broadcast source, localhost
         * source or destination, and zero network source or destination
         * (priority 100). */
        ovn_lflow_add(lflows, od, S_ROUTER_IN_IP_INPUT, 100,
                      "ip4.mcast || "
                      "ip4.src == 255.255.255.255 || "
                      "ip4.src == 127.0.0.0/8 || "
                      "ip4.dst == 127.0.0.0/8 || "
                      "ip4.src == 0.0.0.0/8 || "
                      "ip4.dst == 0.0.0.0/8",
                      "drop;");

        /* Drop Ethernet local broadcast.  By definition this traffic should
         * not be forwarded.*/
        ovn_lflow_add(lflows, od, S_ROUTER_IN_IP_INPUT, 50,
                      "eth.bcast", "drop;");

        /* Drop IP multicast. */
        ovn_lflow_add(lflows, od, S_ROUTER_IN_IP_INPUT, 50,
                      "ip4.mcast", "drop;");

        /* TTL discard.
         *
         * XXX Need to send ICMP time exceeded if !ip.later_frag. */
        char *match = xasprintf("ip4 && ip.ttl == {0, 1}");
        ovn_lflow_add(lflows, od, S_ROUTER_IN_IP_INPUT, 30, match, "drop;");
        free(match);

        /* Pass other traffic not already handled to the next table for
         * routing. */
        ovn_lflow_add(lflows, od, S_ROUTER_IN_IP_INPUT, 0, "1", "next;");
    }

    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbr) {
            continue;
        }

        /* L3 admission control: drop packets that originate from an IP address
         * owned by the router or a broadcast address known to the router
         * (priority 100). */
        char *match = xasprintf("ip4.src == {"IP_FMT", "IP_FMT"}",
                                IP_ARGS(op->ip), IP_ARGS(op->bcast));
        ovn_lflow_add(lflows, op->od, S_ROUTER_IN_IP_INPUT, 100,
                      match, "drop;");
        free(match);

        /* ICMP echo reply.  These flows reply to ICMP echo requests
         * received for the router's IP address. */
        match = xasprintf(
            "inport == %s && (ip4.dst == "IP_FMT" || ip4.dst == "IP_FMT") && "
            "icmp4.type == 8 && icmp4.code == 0",
            op->json_key, IP_ARGS(op->ip), IP_ARGS(op->bcast));
        char *actions = xasprintf(
            "ip4.dst = ip4.src; "
            "ip4.src = "IP_FMT"; "
            "ip.ttl = 255; "
            "icmp4.type = 0; "
            "inport = \"\"; /* Allow sending out inport. */ "
            "next; ",
            IP_ARGS(op->ip));
        ovn_lflow_add(lflows, op->od, S_ROUTER_IN_IP_INPUT, 90,
                      match, actions);
        free(match);
        free(actions);

        /* ARP reply.  These flows reply to ARP requests for the router's own
         * IP address. */
        match = xasprintf(
            "inport == %s && arp.tpa == "IP_FMT" && arp.op == 1",
            op->json_key, IP_ARGS(op->ip));
        actions = xasprintf(
            "eth.dst = eth.src; "
            "eth.src = "ETH_ADDR_FMT"; "
            "arp.op = 2; /* ARP reply */ "
            "arp.tha = arp.sha; "
            "arp.sha = "ETH_ADDR_FMT"; "
            "arp.tpa = arp.spa; "
            "arp.spa = "IP_FMT"; "
            "outport = %s; "
            "inport = \"\"; /* Allow sending out inport. */ "
            "output;",
            ETH_ADDR_ARGS(op->mac),
            ETH_ADDR_ARGS(op->mac),
            IP_ARGS(op->ip),
            op->json_key);
        ovn_lflow_add(lflows, op->od, S_ROUTER_IN_IP_INPUT, 90,
                      match, actions);
        free(match);
        free(actions);

        /* Drop IP traffic to this router. */
        match = xasprintf("ip4.dst == "IP_FMT, IP_ARGS(op->ip));
        ovn_lflow_add(lflows, op->od, S_ROUTER_IN_IP_INPUT, 60,
                      match, "drop;");
        free(match);
    }

    /* Logical router ingress table 2: IP Routing.
     *
     * A packet that arrives at this table is an IP packet that should be
     * routed to the address in ip4.dst. This table sets reg0 to the next-hop
     * IP address (leaving ip4.dst, the packet’s final destination, unchanged)
     * and advances to the next table for ARP resolution. */
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbr) {
            continue;
        }

        add_route(lflows, op->od, op->network, op->mask, 0);
    }
    HMAP_FOR_EACH (od, key_node, datapaths) {
        if (!od->nbr) {
            continue;
        }

        if (od->gateway) {
            add_route(lflows, od, 0, 0, od->gateway);
        }
    }
    /* XXX destination unreachable */

    /* Local router ingress table 3: ARP Resolution.
     *
     * Any packet that reaches this table is an IP packet whose next-hop IP
     * address is in reg0. (ip4.dst is the final destination.) This table
     * resolves the IP address in reg0 into an output port in outport and an
     * Ethernet address in eth.dst. */
    HMAP_FOR_EACH (op, key_node, ports) {
        if (op->nbr) {
            /* XXX ARP for neighboring router */
        } else if (op->od->n_router_ports) {
            for (size_t i = 0; i < op->nbs->n_addresses; i++) {
                struct eth_addr ea;
                ovs_be32 ip;

                if (ovs_scan(op->nbs->addresses[i],
                             ETH_ADDR_SCAN_FMT" "IP_SCAN_FMT,
                             ETH_ADDR_SCAN_ARGS(ea), IP_SCAN_ARGS(&ip))) {
                    for (size_t j = 0; j < op->od->n_router_ports; j++) {
                        /* Get the Logical_Router_Port that the Logical_Port is
                         * connected to, as 'peer'. */
                        const char *peer_name = smap_get(
                            &op->od->router_ports[j]->nbs->options,
                            "router-port");
                        if (!peer_name) {
                            continue;
                        }

                        struct ovn_port *peer
                            = ovn_port_find(ports, peer_name);
                        if (!peer || !peer->nbr) {
                            continue;
                        }

                        /* Make sure that 'ip' is in 'peer''s network. */
                        if ((ip ^ peer->network) & peer->mask) {
                            continue;
                        }

                        char *match = xasprintf("reg0 == "IP_FMT, IP_ARGS(ip));
                        char *actions = xasprintf("eth.src = "ETH_ADDR_FMT"; "
                                                  "eth.dst = "ETH_ADDR_FMT"; "
                                                  "outport = %s; "
                                                  "output;",
                                                  ETH_ADDR_ARGS(peer->mac),
                                                  ETH_ADDR_ARGS(ea),
                                                  peer->json_key);
                        ovn_lflow_add(lflows, peer->od,
                                      S_ROUTER_IN_ARP, 200, match, actions);
                        free(actions);
                        free(match);
                        break;
                    }
                }
            }
        }
    }

    /* Logical router egress table 0: Delivery (priority 100).
     *
     * Priority 100 rules deliver packets to enabled logical ports. */
    HMAP_FOR_EACH (op, key_node, ports) {
        if (!op->nbr) {
            continue;
        }

        if (!lrport_is_enabled(op->nbr)) {
            /* Drop packets to disabled logical ports (since logical flow
             * tables are default-drop). */
            continue;
        }

        char *match = xasprintf("outport == %s", op->json_key);
        ovn_lflow_add(lflows, op->od, S_ROUTER_OUT_DELIVERY, 100,
                      match, "output;");
        free(match);
    }
}

/* Updates the Logical_Flow and Multicast_Group tables in the OVN_SB database,
 * constructing their contents based on the OVN_NB database. */
static void
build_lflows(struct northd_context *ctx, struct hmap *datapaths,
             struct hmap *ports)
{
    struct hmap lflows = HMAP_INITIALIZER(&lflows);
    struct hmap mcgroups = HMAP_INITIALIZER(&mcgroups);

    build_lswitch_flows(datapaths, ports, &lflows, &mcgroups);
    build_lrouter_flows(datapaths, ports, &lflows);

    /* Push changes to the Logical_Flow table to database. */
    const struct sbrec_logical_flow *sbflow, *next_sbflow;
    SBREC_LOGICAL_FLOW_FOR_EACH_SAFE (sbflow, next_sbflow, ctx->ovnsb_idl) {
        struct ovn_datapath *od
            = ovn_datapath_from_sbrec(datapaths, sbflow->logical_datapath);
        if (!od) {
            sbrec_logical_flow_delete(sbflow);
            continue;
        }

        enum ovn_datapath_type dp_type = od->nbs ? DP_SWITCH : DP_ROUTER;
        enum ovn_pipeline pipeline
            = !strcmp(sbflow->pipeline, "ingress") ? P_IN : P_OUT;
        struct ovn_lflow *lflow = ovn_lflow_find(
            &lflows, od, ovn_stage_build(dp_type, pipeline, sbflow->table_id),
            sbflow->priority, sbflow->match, sbflow->actions);
        if (lflow) {
            ovn_lflow_destroy(&lflows, lflow);
        } else {
            sbrec_logical_flow_delete(sbflow);
        }
    }
    struct ovn_lflow *lflow, *next_lflow;
    HMAP_FOR_EACH_SAFE (lflow, next_lflow, hmap_node, &lflows) {
        enum ovn_pipeline pipeline = ovn_stage_get_pipeline(lflow->stage);
        uint8_t table = ovn_stage_get_table(lflow->stage);

        sbflow = sbrec_logical_flow_insert(ctx->ovnsb_txn);
        sbrec_logical_flow_set_logical_datapath(sbflow, lflow->od->sb);
        sbrec_logical_flow_set_pipeline(
            sbflow, pipeline == P_IN ? "ingress" : "egress");
        sbrec_logical_flow_set_table_id(sbflow, table);
        sbrec_logical_flow_set_priority(sbflow, lflow->priority);
        sbrec_logical_flow_set_match(sbflow, lflow->match);
        sbrec_logical_flow_set_actions(sbflow, lflow->actions);

        const struct smap ids = SMAP_CONST1(&ids, "stage-name",
                                            ovn_stage_to_str(lflow->stage));
        sbrec_logical_flow_set_external_ids(sbflow, &ids);

        ovn_lflow_destroy(&lflows, lflow);
    }
    hmap_destroy(&lflows);

    /* Push changes to the Multicast_Group table to database. */
    const struct sbrec_multicast_group *sbmc, *next_sbmc;
    SBREC_MULTICAST_GROUP_FOR_EACH_SAFE (sbmc, next_sbmc, ctx->ovnsb_idl) {
        struct ovn_datapath *od = ovn_datapath_from_sbrec(datapaths,
                                                          sbmc->datapath);
        if (!od) {
            sbrec_multicast_group_delete(sbmc);
            continue;
        }

        struct multicast_group group = { .name = sbmc->name,
                                         .key = sbmc->tunnel_key };
        struct ovn_multicast *mc = ovn_multicast_find(&mcgroups, od, &group);
        if (mc) {
            ovn_multicast_update_sbrec(mc, sbmc);
            ovn_multicast_destroy(&mcgroups, mc);
        } else {
            sbrec_multicast_group_delete(sbmc);
        }
    }
    struct ovn_multicast *mc, *next_mc;
    HMAP_FOR_EACH_SAFE (mc, next_mc, hmap_node, &mcgroups) {
        sbmc = sbrec_multicast_group_insert(ctx->ovnsb_txn);
        sbrec_multicast_group_set_datapath(sbmc, mc->datapath->sb);
        sbrec_multicast_group_set_name(sbmc, mc->group->name);
        sbrec_multicast_group_set_tunnel_key(sbmc, mc->group->key);
        ovn_multicast_update_sbrec(mc, sbmc);
        ovn_multicast_destroy(&mcgroups, mc);
    }
    hmap_destroy(&mcgroups);
}

static void
ovnnb_db_run(struct northd_context *ctx)
{
    if (!ctx->ovnsb_txn) {
        return;
    }
    VLOG_DBG("ovn-nb db contents may have changed.");
    struct hmap datapaths, ports;
    build_datapaths(ctx, &datapaths);
    build_ports(ctx, &datapaths, &ports);
    build_lflows(ctx, &datapaths, &ports);

    struct ovn_datapath *dp, *next_dp;
    HMAP_FOR_EACH_SAFE (dp, next_dp, key_node, &datapaths) {
        ovn_datapath_destroy(&datapaths, dp);
    }
    hmap_destroy(&datapaths);

    struct ovn_port *port, *next_port;
    HMAP_FOR_EACH_SAFE (port, next_port, key_node, &ports) {
        ovn_port_destroy(&ports, port);
    }
    hmap_destroy(&ports);
}

/*
 * The only change we get notified about is if the 'chassis' column of the
 * 'Port_Binding' table changes.  When this column is not empty, it means we
 * need to set the corresponding logical port as 'up' in the northbound DB.
 */
static void
ovnsb_db_run(struct northd_context *ctx)
{
    if (!ctx->ovnnb_txn) {
        return;
    }
    struct hmap lports_hmap;
    const struct sbrec_port_binding *sb;
    const struct nbrec_logical_port *nb;

    struct lport_hash_node {
        struct hmap_node node;
        const struct nbrec_logical_port *nb;
    } *hash_node, *hash_node_next;

    VLOG_DBG("Recalculating port up states for ovn-nb db.");

    hmap_init(&lports_hmap);

    NBREC_LOGICAL_PORT_FOR_EACH(nb, ctx->ovnnb_idl) {
        hash_node = xzalloc(sizeof *hash_node);
        hash_node->nb = nb;
        hmap_insert(&lports_hmap, &hash_node->node, hash_string(nb->name, 0));
    }

    SBREC_PORT_BINDING_FOR_EACH(sb, ctx->ovnsb_idl) {
        nb = NULL;
        HMAP_FOR_EACH_WITH_HASH(hash_node, node,
                                hash_string(sb->logical_port, 0),
                                &lports_hmap) {
            if (!strcmp(sb->logical_port, hash_node->nb->name)) {
                nb = hash_node->nb;
                break;
            }
        }

        if (!nb) {
            /* The logical port doesn't exist for this port binding.  This can
             * happen under normal circumstances when ovn-northd hasn't gotten
             * around to pruning the Port_Binding yet. */
            continue;
        }

        if (sb->chassis && (!nb->up || !*nb->up)) {
            bool up = true;
            nbrec_logical_port_set_up(nb, &up, 1);
        } else if (!sb->chassis && (!nb->up || *nb->up)) {
            bool up = false;
            nbrec_logical_port_set_up(nb, &up, 1);
        }
    }

    HMAP_FOR_EACH_SAFE(hash_node, hash_node_next, node, &lports_hmap) {
        hmap_remove(&lports_hmap, &hash_node->node);
        free(hash_node);
    }
    hmap_destroy(&lports_hmap);
}


static char *default_db_;

static const char *
default_db(void)
{
    if (!default_db_) {
        default_db_ = xasprintf("unix:%s/db.sock", ovs_rundir());
    }
    return default_db_;
}

static void
parse_options(int argc OVS_UNUSED, char *argv[] OVS_UNUSED)
{
    enum {
        DAEMON_OPTION_ENUMS,
        VLOG_OPTION_ENUMS,
    };
    static const struct option long_options[] = {
        {"ovnsb-db", required_argument, NULL, 'd'},
        {"ovnnb-db", required_argument, NULL, 'D'},
        {"help", no_argument, NULL, 'h'},
        {"options", no_argument, NULL, 'o'},
        {"version", no_argument, NULL, 'V'},
        DAEMON_LONG_OPTIONS,
        VLOG_LONG_OPTIONS,
        STREAM_SSL_LONG_OPTIONS,
        {NULL, 0, NULL, 0},
    };
    char *short_options = ovs_cmdl_long_options_to_short_options(long_options);

    for (;;) {
        int c;

        c = getopt_long(argc, argv, short_options, long_options, NULL);
        if (c == -1) {
            break;
        }

        switch (c) {
        DAEMON_OPTION_HANDLERS;
        VLOG_OPTION_HANDLERS;
        STREAM_SSL_OPTION_HANDLERS;

        case 'd':
            ovnsb_db = optarg;
            break;

        case 'D':
            ovnnb_db = optarg;
            break;

        case 'h':
            usage();
            exit(EXIT_SUCCESS);

        case 'o':
            ovs_cmdl_print_options(long_options);
            exit(EXIT_SUCCESS);

        case 'V':
            ovs_print_version(0, 0);
            exit(EXIT_SUCCESS);

        default:
            break;
        }
    }

    if (!ovnsb_db) {
        ovnsb_db = default_db();
    }

    if (!ovnnb_db) {
        ovnnb_db = default_db();
    }

    free(short_options);
}

static void
add_column_noalert(struct ovsdb_idl *idl,
                   const struct ovsdb_idl_column *column)
{
    ovsdb_idl_add_column(idl, column);
    ovsdb_idl_omit_alert(idl, column);
}

int
main(int argc, char *argv[])
{
    unsigned int ovnnb_seqno, ovnsb_seqno;
    int res = EXIT_SUCCESS;
    struct unixctl_server *unixctl;
    int retval;
    bool exiting;

    fatal_ignore_sigpipe();
    set_program_name(argv[0]);
    service_start(&argc, &argv);
    parse_options(argc, argv);

    daemonize_start(false);

    retval = unixctl_server_create(NULL, &unixctl);
    if (retval) {
        exit(EXIT_FAILURE);
    }
    unixctl_command_register("exit", "", 0, 0, ovn_northd_exit, &exiting);

    daemonize_complete();

    nbrec_init();
    sbrec_init();

    /* We want to detect all changes to the ovn-nb db. */
    struct ovsdb_idl_loop ovnnb_idl_loop = OVSDB_IDL_LOOP_INITIALIZER(
        ovsdb_idl_create(ovnnb_db, &nbrec_idl_class, true, true));

    struct ovsdb_idl_loop ovnsb_idl_loop = OVSDB_IDL_LOOP_INITIALIZER(
        ovsdb_idl_create(ovnsb_db, &sbrec_idl_class, false, true));

    ovsdb_idl_add_table(ovnsb_idl_loop.idl, &sbrec_table_logical_flow);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_logical_flow_col_logical_datapath);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_logical_flow_col_pipeline);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_logical_flow_col_table_id);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_logical_flow_col_priority);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_logical_flow_col_match);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_logical_flow_col_actions);

    ovsdb_idl_add_table(ovnsb_idl_loop.idl, &sbrec_table_multicast_group);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_multicast_group_col_datapath);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_multicast_group_col_tunnel_key);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_multicast_group_col_name);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_multicast_group_col_ports);

    ovsdb_idl_add_table(ovnsb_idl_loop.idl, &sbrec_table_datapath_binding);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_datapath_binding_col_tunnel_key);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_datapath_binding_col_external_ids);

    ovsdb_idl_add_table(ovnsb_idl_loop.idl, &sbrec_table_port_binding);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_port_binding_col_datapath);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_port_binding_col_logical_port);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_port_binding_col_tunnel_key);
    add_column_noalert(ovnsb_idl_loop.idl,
                       &sbrec_port_binding_col_parent_port);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_port_binding_col_tag);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_port_binding_col_type);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_port_binding_col_options);
    add_column_noalert(ovnsb_idl_loop.idl, &sbrec_port_binding_col_mac);
    ovsdb_idl_add_column(ovnsb_idl_loop.idl, &sbrec_port_binding_col_chassis);

    ovnnb_seqno = ovsdb_idl_get_seqno(ovnnb_idl_loop.idl);
    ovnsb_seqno = ovsdb_idl_get_seqno(ovnsb_idl_loop.idl);

    /* Main loop. */
    exiting = false;
    while (!exiting) {
        struct northd_context ctx = {
            .ovnnb_idl = ovnnb_idl_loop.idl,
            .ovnnb_txn = ovsdb_idl_loop_run(&ovnnb_idl_loop),
            .ovnsb_idl = ovnsb_idl_loop.idl,
            .ovnsb_txn = ovsdb_idl_loop_run(&ovnsb_idl_loop),
        };

        if (ovnnb_seqno != ovsdb_idl_get_seqno(ctx.ovnnb_idl)) {
            ovnnb_seqno = ovsdb_idl_get_seqno(ctx.ovnnb_idl);
            ovnnb_db_run(&ctx);
        }
        if (ovnsb_seqno != ovsdb_idl_get_seqno(ctx.ovnsb_idl)) {
            ovnsb_seqno = ovsdb_idl_get_seqno(ctx.ovnsb_idl);
            ovnsb_db_run(&ctx);
        }

        unixctl_server_run(unixctl);
        unixctl_server_wait(unixctl);
        if (exiting) {
            poll_immediate_wake();
        }
        ovsdb_idl_loop_commit_and_wait(&ovnnb_idl_loop);
        ovsdb_idl_loop_commit_and_wait(&ovnsb_idl_loop);

        poll_block();
        if (should_service_stop()) {
            exiting = true;
        }
    }

    unixctl_server_destroy(unixctl);
    ovsdb_idl_loop_destroy(&ovnnb_idl_loop);
    ovsdb_idl_loop_destroy(&ovnsb_idl_loop);
    service_stop();

    free(default_db_);
    exit(res);
}

static void
ovn_northd_exit(struct unixctl_conn *conn, int argc OVS_UNUSED,
                const char *argv[] OVS_UNUSED, void *exiting_)
{
    bool *exiting = exiting_;
    *exiting = true;

    unixctl_command_reply(conn, NULL);
}
