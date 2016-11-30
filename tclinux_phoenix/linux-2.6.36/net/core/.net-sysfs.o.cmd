cmd_net/core/net-sysfs.o := mips-buildroot-linux-gnu-gcc -Wp,-MD,net/core/.net-sysfs.o.d  -nostdinc -isystem /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O2 -ffunction-sections -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/ -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/opt/tclinux_phoenix/global_inc -DTCSUPPORT_CPU_MT7520 -DTC3262 -DTCSUPPORT_MT7510_FE -DTCSUPPORT_MT7530_SWITCH_API -DTCSUPPORT_DRIVER_API -DTCSUPPORT_ADDR_MAPPING -DCONFIG_USE_MT7520_ASIC -DTCSUPPORT_CPU_EN7521 -DTCSUPPORT_WLAN_MT7592_PCIE -DTCSUPPORT_UART1_ENHANCE -DTCSUPPORT_2_6_36_KERNEL -DTCSUPPORT_MT7530_SWITCH_API -DCONFIG_GCC_VER=493 -DCONFIG_GLIBC_VER=220 -DCONFIG_GPON_MAX_TCONT=32 -DTCSUPPORT_WAN_GPON -DTCSUPPORT_XPON_IFC -DCONFIG_GPON_MAX_GEMPORT=256 -DTCSUPPORT_WAN_EPON -DTCSUPPORT_XPON_IFC -DCONFIG_EPON_MAX_LLID=8 -DTCSUPPORT_QDMA_BUFMGR -DTCSUPPORT_SLM_EN -DTCSUPPORT_GPON_MAPPING -DTCSUPPORT_EPON_MAPPING -DTCSUPPORT_GPON_DOWNSTREAM_MAPPING -DTCSUPPORT_BB_NAND -DTCSUPPORT_NAND_BMT -DTCSUPPORT_SQUASHFS_ADD_YAFFS -DTCSUPPORT_YAFFS_FS -DTCSUPPORT_DDR_CALI -DTCSUPPORT_BOOTROM_LARGE_SIZE -DSTART_ADDR=0x80002000 -DTCSUPPORT_FREE_BOOTBASE -DTR068_LED -DTCSUPPORT_I2C -DTCSUPPORT_WLAN -DTCSUPPORT_WLAN_MAXSTANUM_GUI -DWSC_AP_SUPPORT -DLED_WPSSPEC_COMPLY -DTCSUPPORT_WLAN_MULTI_WPS -DIGMP_SNOOP_SUPPORT -DTCSUPPORT_WLAN_PMF -DMT7592 -DBBUTOWBU -DTCSUPPORT_WLAN_MT7592 -DMT7612E -DBBUTOWBU -DTCSUPPORT_DUAL_WLAN -DTCSUPPORT_WLAN_AC -DTCSUPPORT_USBHOST -DUSB_AUTOMOUNT -DTCSUPPORT_USB_HOST_LED -DTCSUPPORT_CT_DNSBIND -DCMD_API -DTCSUPPORT_RA_HWNAT -I/opt/tclinux_phoenix/modules/private/ra_hwnat_7510 -DTCSUPPORT_RA_HWNAT_ENHANCE_HOOK -DTCSUPPORT_IPV6 -DIPV6 -DTCSUPPORT_IPV6_ENHANCEMENT -DTCSUPPORT_DSLITE -DTCSUPPORT_IPV6_ADVANCE -DTCSUPPORT_NAND_FLASH -DWITHVOIP -DTCSUPPORT_VOIP -DTCSUPPORT_VOIP_MTK -DTCSUPPORT_ECN_SIP -DTCSUPPORT_VOIP_LED_APPCTRL -DTCSUPPORT_PON_VLAN -DTCSUPPORT_PON_IP_HOST -DTCSUPPORT_PON_MAC_FILTER -DTCSUPPORT_PON_USER_ISOLATION -DTCSUPPORT_PON_VLAN_FILTER -DTCSUPPORT_CWMP -DCWMP -DTCSUPPORT_CT_BUTTONDETECT -DTCSUPPORT_CT_STANDARD_E8C -DSSL -DTR143 -DCLMP -DTCSUPPORT_CLMP -DTCSUPPORT_QOS -DCONFIG_QOS -DTCSUPPORT_CT_QOS -DTCSUPPORT_CT_SWQOS -DTCSUPPORT_CT_URL_FILTER -DTCSUPPORT_CT_HWNAT_OFFLOAD -DTCSUPPORT_IGMP_QOS -DTCSUPPORT_PORTBIND -DTCSUPPORT_CT_PORT_BIND -DTCSUPPORT_CT_VLAN_TAG -DTCSUPPORT_GPON_MAPPING -DTCSUPPORT_GPON_DOWNSTREAM_MAPPING -DSTATIC_DHCP -DDHCP_PROFILE -DTCSUPPORT_E8B -DTCSUPPORT_DNSEACHPVC -DTCSUPPORT_MLD_SNOOPING -DTCSUPPORT_MLD_PROXY -DTCSUPPORT_IGMP_SNOOPING -DTCSUPPORT_IGMPSNOOPING_ENHANCE -DTCSUPPORT_MULTICAST_SPEED -DTCSUPPORT_IGMP_PROXY -DTCSUPPORT_SMUX -DCONFIG_DUAL_IMAGE -DTCSUPPORT_GPON_DUAL_IMAGE -DTCSUPPORT_EPON_DUAL_IMAGE -DTCSUPPORT_MT7520_PCIE -DTCSUPPORT_MTD_PARTITIONS_CMDLINE -DTCSUPPORT_PARTITIONS_CMDLINE_STR=\""16m[tclinux],16m[tclinux_slave],41m[yaffs]"\" -DTCSUPPORT_MTD_ENCHANCEMENT -DTCSUPPORT_RESERVEAREA_BLOCK=7 -DTCSUPPORT_BACKUPROMFILE -DTCSUPPORT_PRODUCTIONLINE -DTCSUPPORT_DMS -DTCSUPPORT_DMS_FULL_FORMAT -DTCSUPPORT_IMPROVE_GUI_PERFM -DTCSUPPORT_WLAN_WDS -DWDS_SUPPORT=1 -DTCSUPPORT_KEYPARA_STORE -DTCSUPPORT_RANDOM_INFORM -DTCSUPPORT_SYSLOG -DTCSUPPORT_LED_BTN_CHECK -DTCSUPPORT_BTN_CHECK -DTCSUPPORT_MEMORY_CONTROL -DTCSUPPORT_TRACEROUTE -DTCSUPPORT_CT_INFORM_NODE -DTCSUPPORT_TEST_LED_ALL -DTCSUPPORT_BRIDGE_MAC_LIMIT -DTCSUPPORT_CT -DTCSUPPORT_CT_USB -DTCSUPPORT_CT_PORT_BIND -DTCSUPPORT_CT_ACCESSLIMIT -DTCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT -DTCSUPPORT_CT_BOOTLOADER_UPGRADE -DTCSUPPORT_CT_E8DDNS -DTCSUPPORT_KEYPARA_STORE -DTCSUPPORT_CT_DEFAULTPARA_STORE -DTCSUPPORT_CT_PORTAL_MANAGEMENT -DTCSUPPORT_CT_NETWORKMANAGESERVICE -DTCSUPPORT_CT_E8GUI -DTCSUPPORT_CT_PON -DTCSUPPORT_CWMP_PRECISE_TIME -DTCSUPPORT_IGMP_QUICK_LEAVE -DTCSUPPORT_CT_PORTSLIMIT -DTCSUPPORT_CT_CWMP_E8BPARAMETER -DTCSUPPORT_CT_2NTP -DTCSUPPORT_CT_5NTP -DTCSUPPORT_CT_NTPSERVERTYPE -DTCSUPPORT_CT_GUIACCESSLIMIT -DTCSUPPORT_CT_ZIPROMFILE -DTCSUPPORT_CT_FW_UPGRADE_16M -DTCSUPPORT_CT_ADSLSETTING -DTCSUPPORT_CT_IPPINGDIAGNOSTIC -DTCSUPPORT_CT_WLAN_NODE -DTCSUPPORT_CT_IPTV_NODE -DTCSUPPORT_CT_RECONNECT_NODE -DTCSUPPORT_CT_INFORM_NODE -DTCSUPPORT_CT_ALARMMONITOR -DTCSUPPORT_CT_ALGABILITY -DCT_COM_DEVICEREG -DTCSUPPORT_CT_PPPCONN_ERROR -DTCSUPPORT_CT_FTP_DOWNLOADCLIENT -DTCSUPPORT_CT_USB_BACKUPRESTORE -DTCSUPPORT_CT_DHCPC_OPTION60 -DTCSUPPORT_CT_DHCPC_OPTION125 -DTCSUPPORT_CT_DHCP6_OPTION -DTCSUPPORT_VIR_SERVER -DTCSUPPORT_CT_DNSMASQ_MONITOR -DTCSUPPORT_WLAN_MAXSTANUM_GUI -DTCSUPPORT_CWMP_XPON -DTCSUPPORT_EPON_ATTACK_ENHANCE -DTCSUPPORT_IGMP_QUICK_LEAVE -DTCSUPPORT_OOM_RB_NEXT -DTCSUPPORT_MT7530_SWITCH_API -DTCSUPPORT_NEW_SPIFLASH -DTCSUPPORT_FTP_CMD -DTCSUPPORT_OMCI -DTCSUPPORT_OMCI_EXTENDED_MSG -DTCSUPPORT_OMCI_CTC -DTCSUPPORT_EPON_OAM -DTCSUPPORT_EPON_OAM_CTC -DTCSUPPORT_EPON_OAM_LAN_DBG -DTCSUPPORT_OMCI_DOT1AG -DTCSUPPORT_HGU_OMCI_MIB_UPLOAD_PPTP_ETH_UNI -DTCSUPPORT_OMCI_LAN_DEBUG -DTCSUPPORT_PONMGR -DTCSUPPORT_PMMGR -DPMMGR_DEBUG -DTCSUPPORT_XPON_IGMP -DTCSUPPORT_XPON_LED -DTCSUPPORT_CT_VLAN_BIND -DTCSUPPORT_CT_FULL_ROUTE -DTCSUPPORT_CT_WAN_STAT -DTCSUPPORT_CT_DUAL_IMAGE -DTCSUPPORT_CT_WAN_CHILD_PREFIX -DTCSUPPORT_CT_DHCP6C_STATUS_CODE -DTCSUPPORT_CT_IPV4_RADIO -DTCSUPPORT_CT_SYSTEM_EX -DTCSUPPORT_CT_DSLITE -DTCSUPPORT_CT_BRIDGEARP_NOFWD_LAN -DTCSUPPORT_CT_UPNP_DM -DTCSUPPORT_CT_SERVICELIST_E8C -DTCSUPPORT_CT_CWMP_ZIPROMFILE -DTCSUPPORT_CT_VOIP_SIP -DTCSUPPORT_VOIP_IMSSIP -DNUM_CHANNELS=2 -DTCSUPPORT_CT_VOIP_QOS -DTCSUPPORT_CT_DHCP_PORT_FILTER -DTCSUPPORT_CT_PON_CY -DTCSUPPORT_RESERVEAREA_EXTEND -DTCSUPPORT_CT_UPG_PINGPONG -DTCSUPPORT_LED_SWITCH_BUTTON -DTCSUPPORT_CT_LONG_RESETBTN -DTCSUPPORT_CT_PPP_ONDEMAND -DTCSUPPORT_CT_ADV_DNSPROXY -DTCSUPPORT_CT_1FXS -DTCSUPPORT_CT_LOOPDETECT -DTCSUPPORT_CT_PHONEAPP -DTCSUPPORT_TRACEROUTE -DTCSUPPORT_CT_JOYME -DTCSUPPORT_CT_JOYME_BANDWIDTH -DTCSUPPORT_CT_OSGI -DTCSUPPORT_CT_PORTSLIMIT -DTCSUPPORT_TR69_IP_HOST -DTCSUPPORT_MT7570 -DTCSUPPORT_CT_TR143_WGET_DOWNLOAD -DTCSUPPORT_CMCC -DTCSUPPORT_CT_DS_LIMIT -DTCSUPPORT_SEPERATED_ETH_ITF -DTCSUPPORT_CT_MAXNET_DPI    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(net_sysfs)"  -D"KBUILD_MODNAME=KBUILD_STR(net_sysfs)"  -c -o net/core/net-sysfs.o net/core/net-sysfs.c

deps_net/core/net-sysfs.o := \
  net/core/net-sysfs.c \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/wireless/ext/sysfs.h) \
    $(wildcard include/config/rps.h) \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/wireless/ext.h) \
  include/linux/capability.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/posix_types.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sgidefs.h \
  include/linux/kernel.h \
    $(wildcard include/config/tc3162/imem.h) \
    $(wildcard include/config/tc3162/dmem.h) \
    $(wildcard include/config/mips/tc3262.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include/stdarg.h \
  include/linux/linkage.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/linkage.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cpu.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/cpu-feature-overrides.h \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/cpu/mips64.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/break.h \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/byteorder.h \
  include/linux/byteorder/big_endian.h \
  include/linux/swab.h \
  include/linux/byteorder/generic.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
  include/asm-generic/bitops/sched.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/arch_hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/asm-generic/bitops/minix.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  include/linux/netdevice.h \
    $(wildcard include/config/dcb.h) \
    $(wildcard include/config/wlan.h) \
    $(wildcard include/config/ax25.h) \
    $(wildcard include/config/mac80211/mesh.h) \
    $(wildcard include/config/tr.h) \
    $(wildcard include/config/net/ipip.h) \
    $(wildcard include/config/net/ipgre.h) \
    $(wildcard include/config/ipv6/sit.h) \
    $(wildcard include/config/ipv6/tunnel.h) \
    $(wildcard include/config/netpoll.h) \
    $(wildcard include/config/net/poll/controller.h) \
    $(wildcard include/config/fcoe.h) \
    $(wildcard include/config/smux.h) \
    $(wildcard include/config/net/dsa.h) \
    $(wildcard include/config/net/ns.h) \
    $(wildcard include/config/port/binding.h) \
    $(wildcard include/config/net/dsa/tag/dsa.h) \
    $(wildcard include/config/net/dsa/tag/trailer.h) \
    $(wildcard include/config/netpoll/trap.h) \
    $(wildcard include/config/proc/fs.h) \
  include/linux/if.h \
  include/linux/socket.h \
    $(wildcard include/config/compat.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/socket.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sockios.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/sockios.h \
  include/linux/uio.h \
  include/linux/hdlc/ioctl.h \
  include/linux/if_ether.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/skbuff.h \
    $(wildcard include/config/imq.h) \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/cpu/tc3162.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/qos.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
  include/linux/kmemcheck.h \
    $(wildcard include/config/kmemcheck.h) \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mm/owner.h) \
    $(wildcard include/config/mmu/notifier.h) \
  include/linux/auxvec.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/auxvec.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cachectl.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/prefetch.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/system.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/linux/const.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/dsp.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/watch.h \
    $(wildcard include/config/hardware/watchpoints.h) \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/n32.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock_types.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/spinlock.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/inline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/atomic.h \
  include/asm-generic/atomic64.h \
  include/asm-generic/atomic-long.h \
  include/linux/prio_tree.h \
  include/linux/rbtree.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/completion.h \
  include/linux/wait.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/current.h \
  include/asm-generic/current.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/page.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
  include/linux/pfn.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/io.h \
  include/asm-generic/iomap.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/ioremap.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mmu.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
  include/linux/math64.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/net.h \
  include/linux/random.h \
  include/linux/ioctl.h \
  include/linux/irqnr.h \
  include/linux/fcntl.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutree.h \
    $(wildcard include/config/no/hz.h) \
  include/linux/sysctl.h \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/param.h \
  include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/textsearch.h \
  include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
  include/linux/stat.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/stat.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/errno.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/smp.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/smp-ops.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/topology.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/timex.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kobject_ns.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/mips64/r2.h) \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/cpu/r6000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/rm7000.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/uaccess.h \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/err.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/slub/debug.h) \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/trace/events/kmem.h \
  include/trace/events/gfpflags.h \
  include/net/checksum.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/checksum.h \
  include/linux/in6.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/disable/channel/switch.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/device.h \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
  include/linux/ioport.h \
  include/linux/klist.h \
  include/linux/pm.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/pm/runtime.h) \
    $(wildcard include/config/pm/ops.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/device.h \
  include/asm-generic/device.h \
  include/linux/pm_wakeup.h \
    $(wildcard include/config/pm.h) \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/bug.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/scatterlist.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  include/linux/mm.h \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memory/failure.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/range.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/pgtable.h \
    $(wildcard include/config/cpu/supports/uncached/accelerated.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/pgtable-32.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/fixmap.h \
    $(wildcard include/config/bcm63xx.h) \
  include/asm-generic/pgtable-nopmd.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/s390.h) \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/dma-mapping.h \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  include/linux/if_packet.h \
  include/linux/if_link.h \
  include/linux/netlink.h \
  include/linux/pm_qos_params.h \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/miscdevice.h \
  include/linux/major.h \
  include/linux/delay.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/delay.h \
  include/linux/rculist.h \
  include/linux/ethtool.h \
  include/net/net_namespace.h \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/ip/dccp.h) \
    $(wildcard include/config/netfilter.h) \
    $(wildcard include/config/wext/core.h) \
    $(wildcard include/config/net.h) \
  include/net/netns/core.h \
  include/net/netns/mib.h \
    $(wildcard include/config/xfrm/statistics.h) \
  include/net/snmp.h \
  include/linux/snmp.h \
  include/linux/u64_stats_sync.h \
  include/net/netns/unix.h \
  include/net/netns/packet.h \
  include/net/netns/ipv4.h \
    $(wildcard include/config/ip/multiple/tables.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/ip/mroute.h) \
    $(wildcard include/config/ip/mroute/multiple/tables.h) \
  include/net/inet_frag.h \
  include/net/netns/ipv6.h \
    $(wildcard include/config/ipv6/multiple/tables.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/mroute/multiple/tables.h) \
  include/net/dst_ops.h \
  include/net/netns/dccp.h \
  include/net/netns/x_tables.h \
    $(wildcard include/config/bridge/nf/ebtables.h) \
  include/linux/netfilter.h \
    $(wildcard include/config/netfilter/debug.h) \
    $(wildcard include/config/nf/nat/needed.h) \
  include/linux/in.h \
  include/net/flow.h \
  include/linux/proc_fs.h \
    $(wildcard include/config/proc/devicetree.h) \
    $(wildcard include/config/proc/kcore.h) \
  include/linux/fs.h \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/path.h \
  include/linux/radix-tree.h \
  include/linux/pid.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/percpu_counter.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/inet.h \
  include/linux/magic.h \
  include/net/netns/conntrack.h \
  include/linux/list_nulls.h \
  include/net/netns/xfrm.h \
  include/linux/xfrm.h \
  include/linux/seq_file_net.h \
  include/linux/seq_file.h \
  include/net/dsa.h \
  include/linux/interrupt.h \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/irqreturn.h \
  include/linux/hardirq.h \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/hardirq.h \
  include/asm-generic/hardirq.h \
  include/linux/irq.h \
    $(wildcard include/config/irq/per/cpu.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/intr/remap.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/sparse/irq.h) \
    $(wildcard include/config/numa/irq/desc.h) \
    $(wildcard include/config/generic/hardirqs/no//do/irq.h) \
    $(wildcard include/config/cpumasks/offstack.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
    $(wildcard include/config/mips/mt/smtc/irqaff.h) \
    $(wildcard include/config/mips/mt/smtc/im/backstop.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mipsmtregs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/irq.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/irq.h \
    $(wildcard include/config/irq/cpu/rm7k.h) \
    $(wildcard include/config/irq/cpu/rm9k.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/ptrace.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/isadep.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/irq_regs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/hw_irq.h \
  include/linux/irq_cpustat.h \
  include/linux/if_arp.h \
  include/linux/nsproxy.h \
    $(wildcard include/config/cgroup/ns.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/cgroup/sched.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/sem.h \
  include/linux/ipc.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/ipcbuf.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sembuf.h \
  include/linux/signal.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
  include/asm-generic/signal-defs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sigcontext.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/proportions.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/resource.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  include/linux/key.h \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/aio.h \
  include/linux/aio_abi.h \
  include/net/sock.h \
  include/linux/security.h \
    $(wildcard include/config/security/path.h) \
    $(wildcard include/config/security/network.h) \
    $(wildcard include/config/security/network/xfrm.h) \
    $(wildcard include/config/securityfs.h) \
  include/linux/fsnotify.h \
  include/linux/fsnotify_backend.h \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/fanotify/access/permissions.h) \
  include/linux/idr.h \
  include/linux/audit.h \
    $(wildcard include/config/change.h) \
  include/linux/binfmts.h \
  include/linux/shm.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/shmparam.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/shmbuf.h \
  include/linux/msg.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/msgbuf.h \
    $(wildcard include/config/cpu/little/endian.h) \
  include/linux/filter.h \
  include/linux/rculist_nulls.h \
  include/linux/poll.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/poll.h \
  include/asm-generic/poll.h \
  include/net/dst.h \
    $(wildcard include/config/net/cls/route.h) \
  include/linux/rtnetlink.h \
  include/linux/if_addr.h \
  include/linux/neighbour.h \
  include/net/neighbour.h \
  include/net/rtnetlink.h \
  include/net/netlink.h \
  include/linux/foe_hook.h \
  include/linux/wireless.h \
  include/linux/vmalloc.h \
  include/net/wext.h \
    $(wildcard include/config/wext/proc.h) \
    $(wildcard include/config/wext/priv.h) \
  include/net/iw_handler.h \
  net/core/net-sysfs.h \

net/core/net-sysfs.o: $(deps_net/core/net-sysfs.o)

$(deps_net/core/net-sysfs.o):
