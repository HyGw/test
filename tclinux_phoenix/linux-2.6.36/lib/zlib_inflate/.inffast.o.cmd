cmd_lib/zlib_inflate/inffast.o := mips-buildroot-linux-gnu-gcc -Wp,-MD,lib/zlib_inflate/.inffast.o.d  -nostdinc -isystem /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O2 -ffunction-sections -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/ -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -I/opt/tclinux_phoenix/global_inc -DTCSUPPORT_CPU_MT7520 -DTC3262 -DTCSUPPORT_MT7510_FE -DTCSUPPORT_MT7530_SWITCH_API -DTCSUPPORT_DRIVER_API -DTCSUPPORT_ADDR_MAPPING -DCONFIG_USE_MT7520_ASIC -DTCSUPPORT_CPU_EN7521 -DTCSUPPORT_WLAN_MT7592_PCIE -DTCSUPPORT_UART1_ENHANCE -DTCSUPPORT_2_6_36_KERNEL -DTCSUPPORT_MT7530_SWITCH_API -DCONFIG_GCC_VER=493 -DCONFIG_GLIBC_VER=220 -DCONFIG_GPON_MAX_TCONT=32 -DTCSUPPORT_WAN_GPON -DTCSUPPORT_XPON_IFC -DCONFIG_GPON_MAX_GEMPORT=256 -DTCSUPPORT_WAN_EPON -DTCSUPPORT_XPON_IFC -DCONFIG_EPON_MAX_LLID=8 -DTCSUPPORT_QDMA_BUFMGR -DTCSUPPORT_SLM_EN -DTCSUPPORT_GPON_MAPPING -DTCSUPPORT_EPON_MAPPING -DTCSUPPORT_GPON_DOWNSTREAM_MAPPING -DTCSUPPORT_BB_NAND -DTCSUPPORT_NAND_BMT -DTCSUPPORT_SQUASHFS_ADD_YAFFS -DTCSUPPORT_YAFFS_FS -DTCSUPPORT_DDR_CALI -DTCSUPPORT_BOOTROM_LARGE_SIZE -DSTART_ADDR=0x80002000 -DTCSUPPORT_FREE_BOOTBASE -DTR068_LED -DTCSUPPORT_I2C -DTCSUPPORT_WLAN -DTCSUPPORT_WLAN_MAXSTANUM_GUI -DWSC_AP_SUPPORT -DLED_WPSSPEC_COMPLY -DTCSUPPORT_WLAN_MULTI_WPS -DIGMP_SNOOP_SUPPORT -DTCSUPPORT_WLAN_PMF -DMT7592 -DBBUTOWBU -DTCSUPPORT_WLAN_MT7592 -DMT7612E -DBBUTOWBU -DTCSUPPORT_DUAL_WLAN -DTCSUPPORT_WLAN_AC -DTCSUPPORT_USBHOST -DUSB_AUTOMOUNT -DTCSUPPORT_USB_HOST_LED -DTCSUPPORT_CT_DNSBIND -DCMD_API -DTCSUPPORT_RA_HWNAT -I/opt/tclinux_phoenix/modules/private/ra_hwnat_7510 -DTCSUPPORT_RA_HWNAT_ENHANCE_HOOK -DTCSUPPORT_IPV6 -DIPV6 -DTCSUPPORT_IPV6_ENHANCEMENT -DTCSUPPORT_DSLITE -DTCSUPPORT_IPV6_ADVANCE -DTCSUPPORT_NAND_FLASH -DWITHVOIP -DTCSUPPORT_VOIP -DTCSUPPORT_VOIP_MTK -DTCSUPPORT_ECN_SIP -DTCSUPPORT_VOIP_LED_APPCTRL -DTCSUPPORT_PON_VLAN -DTCSUPPORT_PON_IP_HOST -DTCSUPPORT_PON_MAC_FILTER -DTCSUPPORT_PON_USER_ISOLATION -DTCSUPPORT_PON_VLAN_FILTER -DTCSUPPORT_CWMP -DCWMP -DTCSUPPORT_CT_BUTTONDETECT -DTCSUPPORT_CT_STANDARD_E8C -DSSL -DTR143 -DCLMP -DTCSUPPORT_CLMP -DTCSUPPORT_QOS -DCONFIG_QOS -DTCSUPPORT_CT_QOS -DTCSUPPORT_CT_SWQOS -DTCSUPPORT_CT_URL_FILTER -DTCSUPPORT_CT_HWNAT_OFFLOAD -DTCSUPPORT_IGMP_QOS -DTCSUPPORT_PORTBIND -DTCSUPPORT_CT_PORT_BIND -DTCSUPPORT_CT_VLAN_TAG -DTCSUPPORT_GPON_MAPPING -DTCSUPPORT_GPON_DOWNSTREAM_MAPPING -DSTATIC_DHCP -DDHCP_PROFILE -DTCSUPPORT_E8B -DTCSUPPORT_DNSEACHPVC -DTCSUPPORT_MLD_SNOOPING -DTCSUPPORT_MLD_PROXY -DTCSUPPORT_IGMP_SNOOPING -DTCSUPPORT_IGMPSNOOPING_ENHANCE -DTCSUPPORT_MULTICAST_SPEED -DTCSUPPORT_IGMP_PROXY -DTCSUPPORT_SMUX -DCONFIG_DUAL_IMAGE -DTCSUPPORT_GPON_DUAL_IMAGE -DTCSUPPORT_EPON_DUAL_IMAGE -DTCSUPPORT_MT7520_PCIE -DTCSUPPORT_MTD_PARTITIONS_CMDLINE -DTCSUPPORT_PARTITIONS_CMDLINE_STR=\""16m[tclinux],16m[tclinux_slave],41m[yaffs]"\" -DTCSUPPORT_MTD_ENCHANCEMENT -DTCSUPPORT_RESERVEAREA_BLOCK=7 -DTCSUPPORT_BACKUPROMFILE -DTCSUPPORT_PRODUCTIONLINE -DTCSUPPORT_DMS -DTCSUPPORT_DMS_FULL_FORMAT -DTCSUPPORT_IMPROVE_GUI_PERFM -DTCSUPPORT_WLAN_WDS -DWDS_SUPPORT=1 -DTCSUPPORT_KEYPARA_STORE -DTCSUPPORT_RANDOM_INFORM -DTCSUPPORT_SYSLOG -DTCSUPPORT_LED_BTN_CHECK -DTCSUPPORT_BTN_CHECK -DTCSUPPORT_MEMORY_CONTROL -DTCSUPPORT_TRACEROUTE -DTCSUPPORT_CT_INFORM_NODE -DTCSUPPORT_TEST_LED_ALL -DTCSUPPORT_BRIDGE_MAC_LIMIT -DTCSUPPORT_CT -DTCSUPPORT_CT_USB -DTCSUPPORT_CT_PORT_BIND -DTCSUPPORT_CT_ACCESSLIMIT -DTCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT -DTCSUPPORT_CT_BOOTLOADER_UPGRADE -DTCSUPPORT_CT_E8DDNS -DTCSUPPORT_KEYPARA_STORE -DTCSUPPORT_CT_DEFAULTPARA_STORE -DTCSUPPORT_CT_PORTAL_MANAGEMENT -DTCSUPPORT_CT_NETWORKMANAGESERVICE -DTCSUPPORT_CT_E8GUI -DTCSUPPORT_CT_PON -DTCSUPPORT_CWMP_PRECISE_TIME -DTCSUPPORT_IGMP_QUICK_LEAVE -DTCSUPPORT_CT_PORTSLIMIT -DTCSUPPORT_CT_CWMP_E8BPARAMETER -DTCSUPPORT_CT_2NTP -DTCSUPPORT_CT_5NTP -DTCSUPPORT_CT_NTPSERVERTYPE -DTCSUPPORT_CT_GUIACCESSLIMIT -DTCSUPPORT_CT_ZIPROMFILE -DTCSUPPORT_CT_FW_UPGRADE_16M -DTCSUPPORT_CT_ADSLSETTING -DTCSUPPORT_CT_IPPINGDIAGNOSTIC -DTCSUPPORT_CT_WLAN_NODE -DTCSUPPORT_CT_IPTV_NODE -DTCSUPPORT_CT_RECONNECT_NODE -DTCSUPPORT_CT_INFORM_NODE -DTCSUPPORT_CT_ALARMMONITOR -DTCSUPPORT_CT_ALGABILITY -DCT_COM_DEVICEREG -DTCSUPPORT_CT_PPPCONN_ERROR -DTCSUPPORT_CT_FTP_DOWNLOADCLIENT -DTCSUPPORT_CT_USB_BACKUPRESTORE -DTCSUPPORT_CT_DHCPC_OPTION60 -DTCSUPPORT_CT_DHCPC_OPTION125 -DTCSUPPORT_CT_DHCP6_OPTION -DTCSUPPORT_VIR_SERVER -DTCSUPPORT_CT_DNSMASQ_MONITOR -DTCSUPPORT_WLAN_MAXSTANUM_GUI -DTCSUPPORT_CWMP_XPON -DTCSUPPORT_EPON_ATTACK_ENHANCE -DTCSUPPORT_IGMP_QUICK_LEAVE -DTCSUPPORT_OOM_RB_NEXT -DTCSUPPORT_MT7530_SWITCH_API -DTCSUPPORT_NEW_SPIFLASH -DTCSUPPORT_FTP_CMD -DTCSUPPORT_OMCI -DTCSUPPORT_OMCI_EXTENDED_MSG -DTCSUPPORT_OMCI_CTC -DTCSUPPORT_EPON_OAM -DTCSUPPORT_EPON_OAM_CTC -DTCSUPPORT_EPON_OAM_LAN_DBG -DTCSUPPORT_OMCI_DOT1AG -DTCSUPPORT_HGU_OMCI_MIB_UPLOAD_PPTP_ETH_UNI -DTCSUPPORT_OMCI_LAN_DEBUG -DTCSUPPORT_PONMGR -DTCSUPPORT_PMMGR -DPMMGR_DEBUG -DTCSUPPORT_XPON_IGMP -DTCSUPPORT_XPON_LED -DTCSUPPORT_CT_VLAN_BIND -DTCSUPPORT_CT_FULL_ROUTE -DTCSUPPORT_CT_WAN_STAT -DTCSUPPORT_CT_DUAL_IMAGE -DTCSUPPORT_CT_WAN_CHILD_PREFIX -DTCSUPPORT_CT_DHCP6C_STATUS_CODE -DTCSUPPORT_CT_IPV4_RADIO -DTCSUPPORT_CT_SYSTEM_EX -DTCSUPPORT_CT_DSLITE -DTCSUPPORT_CT_BRIDGEARP_NOFWD_LAN -DTCSUPPORT_CT_UPNP_DM -DTCSUPPORT_CT_SERVICELIST_E8C -DTCSUPPORT_CT_CWMP_ZIPROMFILE -DTCSUPPORT_CT_VOIP_SIP -DTCSUPPORT_VOIP_IMSSIP -DNUM_CHANNELS=2 -DTCSUPPORT_CT_VOIP_QOS -DTCSUPPORT_CT_DHCP_PORT_FILTER -DTCSUPPORT_CT_PON_CY -DTCSUPPORT_RESERVEAREA_EXTEND -DTCSUPPORT_CT_UPG_PINGPONG -DTCSUPPORT_LED_SWITCH_BUTTON -DTCSUPPORT_CT_LONG_RESETBTN -DTCSUPPORT_CT_PPP_ONDEMAND -DTCSUPPORT_CT_ADV_DNSPROXY -DTCSUPPORT_CT_1FXS -DTCSUPPORT_CT_LOOPDETECT -DTCSUPPORT_CT_PHONEAPP -DTCSUPPORT_TRACEROUTE -DTCSUPPORT_CT_JOYME -DTCSUPPORT_CT_JOYME_BANDWIDTH -DTCSUPPORT_CT_OSGI -DTCSUPPORT_CT_PORTSLIMIT -DTCSUPPORT_TR69_IP_HOST -DTCSUPPORT_MT7570 -DTCSUPPORT_CT_TR143_WGET_DOWNLOAD -DTCSUPPORT_CMCC -DTCSUPPORT_CT_DS_LIMIT -DTCSUPPORT_SEPERATED_ETH_ITF -DTCSUPPORT_CT_MAXNET_DPI    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(inffast)"  -D"KBUILD_MODNAME=KBUILD_STR(zlib_inflate)"  -c -o lib/zlib_inflate/inffast.o lib/zlib_inflate/inffast.c

deps_lib/zlib_inflate/inffast.o := \
  lib/zlib_inflate/inffast.c \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
  include/linux/zutil.h \
  include/linux/zlib.h \
  include/linux/zconf.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
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
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/posix_types.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sgidefs.h \
  /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include/stdarg.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/string.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tc3162.h) \
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
  lib/zlib_inflate/inftrees.h \
  lib/zlib_inflate/inflate.h \
  lib/zlib_inflate/inffast.h \

lib/zlib_inflate/inffast.o: $(deps_lib/zlib_inflate/inffast.o)

$(deps_lib/zlib_inflate/inffast.o):
