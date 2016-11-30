#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net-snmp/agent/hardware/cpu.h>

extern NetsnmpCacheLoad netsnmp_cpu_arch_load;
static void _cpu_update_stats( unsigned int, void* );

int _cpuAutoUpdate =  5;
int _cpuHistoryLen;

netsnmp_cpu_info *_cpu_head  = NULL;
netsnmp_cpu_info *_cpu_tail  = NULL;
netsnmp_cache    *_cpu_cache = NULL;

void init_cpu( void ) {
    /*
     * If we're sampling the CPU statistics automatically,
     *   then arrange for this to be triggered regularly,
     *   keeping sufficient samples to cover the last minute.
     * If the system-specific code has already initialised
     *   the list of CPU entries, then retrieve the first set
     *   of stats immediately.
     * Otherwise, wait until the regular sampling kicks in.
     *
     * If we're not sampling these statistics regularly,
     *   create a suitable cache handler instead.
     */
    if ( _cpuAutoUpdate ) {
         
        _cpuHistoryLen = 60/_cpuAutoUpdate;
        snmp_alarm_register(_cpuAutoUpdate, SA_REPEAT, _cpu_update_stats, NULL);
        if ( _cpu_head )
            _cpu_update_stats( 0, NULL );
    } else
    _cpu_cache = netsnmp_cache_create( 5, netsnmp_cpu_arch_load, NULL, NULL, 0);
}



netsnmp_cpu_info *netsnmp_cpu_get_first( void ) {
    return _cpu_head;
}
netsnmp_cpu_info *netsnmp_cpu_get_next( netsnmp_cpu_info *this_ptr ) {
    return ( this_ptr ? this_ptr->next : NULL );
}

    /*
     * Work with a list of CPU entries, indexed numerically
     */
netsnmp_cpu_info *netsnmp_cpu_get_byIdx(  int idx, int create ) {
    netsnmp_cpu_info *cpu, *cpu2;

        /*
         * Find the specified CPU entry
         */
    for ( cpu=_cpu_head; cpu; cpu=cpu->next ) {
        if ( cpu->idx == idx )
            return cpu;
    }
    if (!create)
        return NULL;

        /*
         * Create a new CPU entry, and insert it into the list....
         */
    cpu = SNMP_MALLOC_TYPEDEF( netsnmp_cpu_info );
    if (!cpu)
        return NULL;
    cpu->idx = idx;
        /* ... either as the first (or only) entry....  */
    if ( !_cpu_head || _cpu_head->idx > idx ) {
        cpu->next = _cpu_head;
        _cpu_head = cpu;
        if (!_cpu_tail)
            _cpu_tail = cpu;
        return cpu;
    }
        /* ... or in the appropriate position  */
    for ( cpu2=_cpu_head; cpu2; cpu2=cpu2->next ) {
        if ( !cpu2->next || cpu2->next->idx > idx ) {
            cpu->next  = cpu2->next;
            cpu2->next = cpu;
            if (!cpu->next)
                _cpu_tail = cpu;
            return cpu;
        }
    }
    if (cpu)
        SNMP_FREE(cpu); /* just in case */
    return NULL;  /* Shouldn't happen! */
}

    /*
     * Work with a list of CPU entries, indexed by name
     */
netsnmp_cpu_info *netsnmp_cpu_get_byName( char *name, int create ) {
    netsnmp_cpu_info *cpu;

        /*
         * Find the specified CPU entry
         */
    for ( cpu=_cpu_head; cpu; cpu=cpu->next ) {
        if ( !strcmp(cpu->name, name))
            return cpu;
    }
    if (!create)
        return NULL;

        /*
         * Create a new CPU entry, and append it to the list
         */
    cpu = SNMP_MALLOC_TYPEDEF( netsnmp_cpu_info );
    if (!cpu)
        return NULL;
    strcpy(cpu->name, name);
    if ( _cpu_tail ) {
        cpu->idx = _cpu_tail->idx+1;
        _cpu_tail->next = cpu;
        _cpu_tail       = cpu;
    } else {
        cpu->idx = 0;
        _cpu_head = cpu;
        _cpu_tail = cpu;
    }
    return cpu;
}

netsnmp_cache *netsnmp_cpu_get_cache( void ) {
    return _cpu_cache;
}

int netsnmp_cpu_load( void ) {
        /*
         * If we're automatically updating the stats regularly,
         * then don't invoke the cache handling.
         */
    return ( _cpuAutoUpdate ? 1
                            : netsnmp_cache_check_and_reload( _cpu_cache ));
}

    /*
     * Call the system-specific load routine regularly,
     * keeping track of the relevant earlier results.
     */
static void
_cpu_update_stats( unsigned int reg, void* magic ) {
    netsnmp_cpu_info *cpu;
    int i;

    for ( cpu=_cpu_head; cpu; cpu=cpu->next ) {
        if ( !cpu->history ) {
            /*
             * First time through, we need to create buffers
             * for the historical stats
             */
            cpu->history  = calloc( _cpuHistoryLen, sizeof(struct netsnmp_cpu_history));
        } else {
            /*
             * Otherwise, rotate these values - in descending order
             *   with the earliest (relevant) statistics in entry 0.
             * This means that the code to calculate the rolling averages
             *   is independent of the number of historical samples saved.
             */
            for (i=0; i<_cpuHistoryLen-2; i++) {
                cpu->history[i] = cpu->history[i+1];
            }
            cpu->history[i].user_hist  = cpu->user_ticks;
            cpu->history[i].sys_hist   = cpu->sys_ticks;
            cpu->history[i].idle_hist  = cpu->idle_ticks;
            cpu->history[i].nice_hist  = cpu->nice_ticks;
            cpu->history[i].total_hist = cpu->user_ticks +
                                         cpu->nice_ticks +
                                         cpu->sys_ticks +
                                         cpu->idle_ticks +
                                         cpu->wait_ticks +
                                         cpu->kern_ticks +
                                         cpu->intrpt_ticks +
                                         cpu->sirq_ticks;
            cpu->history[i].ctx_hist   = cpu->nCtxSwitches;
            cpu->history[i].intr_hist  = cpu->nInterrupts;
        }
    }

    /*
     * Now call the system-specific load routine, to
     * retrieve the latest set of data.
     */
    netsnmp_cpu_arch_load( NULL, NULL );
}
