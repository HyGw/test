/*
 * module to include the ifTable implementation modules  
 */
/*
 */
#if 0 //defined( linux )
config_require(if-mib/ifTable)
#else
config_require(mibII/interfaces)
#endif
