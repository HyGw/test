/*
 * udp_endpoint data access header
 *
 * $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/net-snmp-5.3.1/agent/mibgroup/udp-mib/data_access/udp_endpoint.h#1 $
 */
/**---------------------------------------------------------------------*/
/*
 * configure required files
 *
 * Notes:
 *
 * 1) prefer functionality over platform, where possible. If a method
 *    is available for multiple platforms, test that first. That way
 *    when a new platform is ported, it won't need a new test here.
 *
 * 2) don't do detail requirements here. If, for example,
 *    HPUX11 had different reuirements than other HPUX, that should
 *    be handled in the *_hpux.h header file.
 */
config_require(udp-mib/data_access/udp_endpoint_common)
#if defined( linux )
config_require(udp-mib/data_access/udp_endpoint_linux)
#else
#   define NETSNMP_UDP_ENDPOINT_COMMON_ONLY
#endif
