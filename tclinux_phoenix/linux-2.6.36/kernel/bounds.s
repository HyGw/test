	.file	1 "bounds.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.gnu_attribute 4, 3

 # -G value = 0, Arch = mips32r2, ISA = 33
 # GNU C (Buildroot 2015.08.1) version 4.9.3 (mips-buildroot-linux-gnu)
 #	compiled by GNU C version 3.4.6, GMP version 6.0.0, MPFR version 3.1.3, MPC version 1.0.3
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # 传递的选项： -nostdinc
 # -I /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include -I include
 # -I /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/
 # -I /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic
 # -I /opt/tclinux_phoenix/global_inc
 # -I /opt/tclinux_phoenix/modules/private/ra_hwnat_7510 -D __KERNEL__
 # -D VMLINUX_LOAD_ADDRESS=0xffffffff80002000 -D DATAOFFSET=0
 # -D TCSUPPORT_CPU_MT7520 -D TC3262 -D TCSUPPORT_MT7510_FE
 # -D TCSUPPORT_MT7530_SWITCH_API -D TCSUPPORT_DRIVER_API
 # -D TCSUPPORT_ADDR_MAPPING -D CONFIG_USE_MT7520_ASIC
 # -D TCSUPPORT_CPU_EN7521 -D TCSUPPORT_WLAN_MT7592_PCIE
 # -D TCSUPPORT_UART1_ENHANCE -D TCSUPPORT_2_6_36_KERNEL
 # -D TCSUPPORT_MT7530_SWITCH_API -D CONFIG_GCC_VER=493
 # -D CONFIG_GLIBC_VER=220 -D CONFIG_GPON_MAX_TCONT=32
 # -D TCSUPPORT_WAN_GPON -D TCSUPPORT_XPON_IFC
 # -D CONFIG_GPON_MAX_GEMPORT=256 -D TCSUPPORT_WAN_EPON
 # -D TCSUPPORT_XPON_IFC -D CONFIG_EPON_MAX_LLID=8 -D TCSUPPORT_QDMA_BUFMGR
 # -D TCSUPPORT_SLM_EN -D TCSUPPORT_GPON_MAPPING -D TCSUPPORT_EPON_MAPPING
 # -D TCSUPPORT_GPON_DOWNSTREAM_MAPPING -D TCSUPPORT_BB_NAND
 # -D TCSUPPORT_NAND_BMT -D TCSUPPORT_SQUASHFS_ADD_YAFFS
 # -D TCSUPPORT_YAFFS_FS -D TCSUPPORT_DDR_CALI
 # -D TCSUPPORT_BOOTROM_LARGE_SIZE -D START_ADDR=0x80002000
 # -D TCSUPPORT_FREE_BOOTBASE -D TR068_LED -D TCSUPPORT_I2C
 # -D TCSUPPORT_WLAN -D TCSUPPORT_WLAN_MAXSTANUM_GUI -D WSC_AP_SUPPORT
 # -D LED_WPSSPEC_COMPLY -D TCSUPPORT_WLAN_MULTI_WPS -D IGMP_SNOOP_SUPPORT
 # -D TCSUPPORT_WLAN_PMF -D MT7592 -D BBUTOWBU -D TCSUPPORT_WLAN_MT7592
 # -D MT7612E -D BBUTOWBU -D TCSUPPORT_DUAL_WLAN -D TCSUPPORT_WLAN_AC
 # -D TCSUPPORT_USBHOST -D USB_AUTOMOUNT -D TCSUPPORT_USB_HOST_LED
 # -D TCSUPPORT_CT_DNSBIND -D CMD_API -D TCSUPPORT_RA_HWNAT
 # -D TCSUPPORT_RA_HWNAT_ENHANCE_HOOK -D TCSUPPORT_IPV6 -D IPV6
 # -D TCSUPPORT_IPV6_ENHANCEMENT -D TCSUPPORT_DSLITE
 # -D TCSUPPORT_IPV6_ADVANCE -D TCSUPPORT_NAND_FLASH -D WITHVOIP
 # -D TCSUPPORT_VOIP -D TCSUPPORT_VOIP_MTK -D TCSUPPORT_ECN_SIP
 # -D TCSUPPORT_VOIP_LED_APPCTRL -D TCSUPPORT_PON_VLAN
 # -D TCSUPPORT_PON_IP_HOST -D TCSUPPORT_PON_MAC_FILTER
 # -D TCSUPPORT_PON_USER_ISOLATION -D TCSUPPORT_PON_VLAN_FILTER
 # -D TCSUPPORT_CWMP -D CWMP -D TCSUPPORT_CT_BUTTONDETECT
 # -D TCSUPPORT_CT_STANDARD_E8C -D SSL -D TR143 -D CLMP -D TCSUPPORT_CLMP
 # -D TCSUPPORT_QOS -D CONFIG_QOS -D TCSUPPORT_CT_QOS -D TCSUPPORT_CT_SWQOS
 # -D TCSUPPORT_CT_URL_FILTER -D TCSUPPORT_CT_HWNAT_OFFLOAD
 # -D TCSUPPORT_IGMP_QOS -D TCSUPPORT_PORTBIND -D TCSUPPORT_CT_PORT_BIND
 # -D TCSUPPORT_CT_VLAN_TAG -D TCSUPPORT_GPON_MAPPING
 # -D TCSUPPORT_GPON_DOWNSTREAM_MAPPING -D STATIC_DHCP -D DHCP_PROFILE
 # -D TCSUPPORT_E8B -D TCSUPPORT_DNSEACHPVC -D TCSUPPORT_MLD_SNOOPING
 # -D TCSUPPORT_MLD_PROXY -D TCSUPPORT_IGMP_SNOOPING
 # -D TCSUPPORT_IGMPSNOOPING_ENHANCE -D TCSUPPORT_MULTICAST_SPEED
 # -D TCSUPPORT_IGMP_PROXY -D TCSUPPORT_SMUX -D CONFIG_DUAL_IMAGE
 # -D TCSUPPORT_GPON_DUAL_IMAGE -D TCSUPPORT_EPON_DUAL_IMAGE
 # -D TCSUPPORT_MT7520_PCIE -D TCSUPPORT_MTD_PARTITIONS_CMDLINE
 # -D TCSUPPORT_PARTITIONS_CMDLINE_STR="16m[tclinux],16m[tclinux_slave],41m[yaffs]"
 # -D TCSUPPORT_MTD_ENCHANCEMENT -D TCSUPPORT_RESERVEAREA_BLOCK=7
 # -D TCSUPPORT_BACKUPROMFILE -D TCSUPPORT_PRODUCTIONLINE -D TCSUPPORT_DMS
 # -D TCSUPPORT_DMS_FULL_FORMAT -D TCSUPPORT_IMPROVE_GUI_PERFM
 # -D TCSUPPORT_WLAN_WDS -D WDS_SUPPORT=1 -D TCSUPPORT_KEYPARA_STORE
 # -D TCSUPPORT_RANDOM_INFORM -D TCSUPPORT_SYSLOG
 # -D TCSUPPORT_LED_BTN_CHECK -D TCSUPPORT_BTN_CHECK
 # -D TCSUPPORT_MEMORY_CONTROL -D TCSUPPORT_TRACEROUTE
 # -D TCSUPPORT_CT_INFORM_NODE -D TCSUPPORT_TEST_LED_ALL
 # -D TCSUPPORT_BRIDGE_MAC_LIMIT -D TCSUPPORT_CT -D TCSUPPORT_CT_USB
 # -D TCSUPPORT_CT_PORT_BIND -D TCSUPPORT_CT_ACCESSLIMIT
 # -D TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT
 # -D TCSUPPORT_CT_BOOTLOADER_UPGRADE -D TCSUPPORT_CT_E8DDNS
 # -D TCSUPPORT_KEYPARA_STORE -D TCSUPPORT_CT_DEFAULTPARA_STORE
 # -D TCSUPPORT_CT_PORTAL_MANAGEMENT -D TCSUPPORT_CT_NETWORKMANAGESERVICE
 # -D TCSUPPORT_CT_E8GUI -D TCSUPPORT_CT_PON -D TCSUPPORT_CWMP_PRECISE_TIME
 # -D TCSUPPORT_IGMP_QUICK_LEAVE -D TCSUPPORT_CT_PORTSLIMIT
 # -D TCSUPPORT_CT_CWMP_E8BPARAMETER -D TCSUPPORT_CT_2NTP
 # -D TCSUPPORT_CT_5NTP -D TCSUPPORT_CT_NTPSERVERTYPE
 # -D TCSUPPORT_CT_GUIACCESSLIMIT -D TCSUPPORT_CT_ZIPROMFILE
 # -D TCSUPPORT_CT_FW_UPGRADE_16M -D TCSUPPORT_CT_ADSLSETTING
 # -D TCSUPPORT_CT_IPPINGDIAGNOSTIC -D TCSUPPORT_CT_WLAN_NODE
 # -D TCSUPPORT_CT_IPTV_NODE -D TCSUPPORT_CT_RECONNECT_NODE
 # -D TCSUPPORT_CT_INFORM_NODE -D TCSUPPORT_CT_ALARMMONITOR
 # -D TCSUPPORT_CT_ALGABILITY -D CT_COM_DEVICEREG
 # -D TCSUPPORT_CT_PPPCONN_ERROR -D TCSUPPORT_CT_FTP_DOWNLOADCLIENT
 # -D TCSUPPORT_CT_USB_BACKUPRESTORE -D TCSUPPORT_CT_DHCPC_OPTION60
 # -D TCSUPPORT_CT_DHCPC_OPTION125 -D TCSUPPORT_CT_DHCP6_OPTION
 # -D TCSUPPORT_VIR_SERVER -D TCSUPPORT_CT_DNSMASQ_MONITOR
 # -D TCSUPPORT_WLAN_MAXSTANUM_GUI -D TCSUPPORT_CWMP_XPON
 # -D TCSUPPORT_EPON_ATTACK_ENHANCE -D TCSUPPORT_IGMP_QUICK_LEAVE
 # -D TCSUPPORT_OOM_RB_NEXT -D TCSUPPORT_MT7530_SWITCH_API
 # -D TCSUPPORT_NEW_SPIFLASH -D TCSUPPORT_FTP_CMD -D TCSUPPORT_OMCI
 # -D TCSUPPORT_OMCI_EXTENDED_MSG -D TCSUPPORT_OMCI_CTC
 # -D TCSUPPORT_EPON_OAM -D TCSUPPORT_EPON_OAM_CTC
 # -D TCSUPPORT_EPON_OAM_LAN_DBG -D TCSUPPORT_OMCI_DOT1AG
 # -D TCSUPPORT_HGU_OMCI_MIB_UPLOAD_PPTP_ETH_UNI
 # -D TCSUPPORT_OMCI_LAN_DEBUG -D TCSUPPORT_PONMGR -D TCSUPPORT_PMMGR
 # -D PMMGR_DEBUG -D TCSUPPORT_XPON_IGMP -D TCSUPPORT_XPON_LED
 # -D TCSUPPORT_CT_VLAN_BIND -D TCSUPPORT_CT_FULL_ROUTE
 # -D TCSUPPORT_CT_WAN_STAT -D TCSUPPORT_CT_DUAL_IMAGE
 # -D TCSUPPORT_CT_WAN_CHILD_PREFIX -D TCSUPPORT_CT_DHCP6C_STATUS_CODE
 # -D TCSUPPORT_CT_IPV4_RADIO -D TCSUPPORT_CT_SYSTEM_EX
 # -D TCSUPPORT_CT_DSLITE -D TCSUPPORT_CT_BRIDGEARP_NOFWD_LAN
 # -D TCSUPPORT_CT_UPNP_DM -D TCSUPPORT_CT_SERVICELIST_E8C
 # -D TCSUPPORT_CT_CWMP_ZIPROMFILE -D TCSUPPORT_CT_VOIP_SIP
 # -D TCSUPPORT_VOIP_IMSSIP -D NUM_CHANNELS=2 -D TCSUPPORT_CT_VOIP_QOS
 # -D TCSUPPORT_CT_DHCP_PORT_FILTER -D TCSUPPORT_CT_PON_CY
 # -D TCSUPPORT_RESERVEAREA_EXTEND -D TCSUPPORT_CT_UPG_PINGPONG
 # -D TCSUPPORT_LED_SWITCH_BUTTON -D TCSUPPORT_CT_LONG_RESETBTN
 # -D TCSUPPORT_CT_PPP_ONDEMAND -D TCSUPPORT_CT_ADV_DNSPROXY
 # -D TCSUPPORT_CT_1FXS -D TCSUPPORT_CT_LOOPDETECT -D TCSUPPORT_CT_PHONEAPP
 # -D TCSUPPORT_TRACEROUTE -D TCSUPPORT_CT_JOYME
 # -D TCSUPPORT_CT_JOYME_BANDWIDTH -D TCSUPPORT_CT_OSGI
 # -D TCSUPPORT_CT_PORTSLIMIT -D TCSUPPORT_TR69_IP_HOST -D TCSUPPORT_MT7570
 # -D TCSUPPORT_CT_TR143_WGET_DOWNLOAD -D TCSUPPORT_CMCC
 # -D TCSUPPORT_CT_DS_LIMIT -D TCSUPPORT_SEPERATED_ETH_ITF
 # -D TCSUPPORT_CT_MAXNET_DPI -D KBUILD_STR(s)=#s
 # -D KBUILD_BASENAME=KBUILD_STR(bounds)
 # -D KBUILD_MODNAME=KBUILD_STR(bounds)
 # -isystem /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include
 # -include include/generated/autoconf.h -MD kernel/.bounds.s.d
 # kernel/bounds.c -G 0 -mno-check-zero-division -mabi=32 -mno-abicalls
 # -msoft-float -march=mips32r2 -mllsc -mno-shared
 # -auxbase-strip kernel/bounds.s -O2 -Wall -Wundef -Wstrict-prototypes
 # -Wno-trigraphs -Wframe-larger-than=1024 -Wdeclaration-after-statement
 # -Wno-pointer-sign -fno-strict-aliasing -fno-common -ffunction-sections
 # -fno-pic -ffreestanding -fno-stack-protector -fomit-frame-pointer
 # -fno-strict-overflow -fconserve-stack -fverbose-asm
 # 启用的选项： -faggressive-loop-optimizations -fauto-inc-dec
 # -fbranch-count-reg -fcaller-saves -fcombine-stack-adjustments
 # -fcompare-elim -fcprop-registers -fcrossjumping -fcse-follow-jumps
 # -fdefer-pop -fdelayed-branch -fdelete-null-pointer-checks -fdevirtualize
 # -fdevirtualize-speculatively -fdwarf2-cfi-asm -fearly-inlining
 # -feliminate-unused-debug-types -fexpensive-optimizations
 # -fforward-propagate -ffunction-cse -ffunction-sections -fgcse -fgcse-lm
 # -fgnu-runtime -fgnu-unique -fguess-branch-probability
 # -fhoist-adjacent-loads -fident -fif-conversion -fif-conversion2
 # -findirect-inlining -finline -finline-atomics
 # -finline-functions-called-once -finline-small-functions -fipa-cp
 # -fipa-profile -fipa-pure-const -fipa-reference -fipa-sra
 # -fira-hoist-pressure -fira-share-save-slots -fira-share-spill-slots
 # -fisolate-erroneous-paths-dereference -fivopts -fkeep-static-consts
 # -fleading-underscore -flifetime-dse -fmath-errno -fmerge-constants
 # -fmerge-debug-strings -fmove-loop-invariants -fomit-frame-pointer
 # -foptimize-sibling-calls -foptimize-strlen -fpartial-inlining
 # -fpcc-struct-return -fpeephole -fpeephole2 -fprefetch-loop-arrays
 # -freorder-blocks -freorder-functions -frerun-cse-after-loop
 # -fsched-critical-path-heuristic -fsched-dep-count-heuristic
 # -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
 # -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
 # -fsched-stalled-insns-dep -fschedule-insns -fschedule-insns2
 # -fshow-column -fshrink-wrap -fsigned-zeros -fsplit-ivs-in-unroller
 # -fsplit-wide-types -fstrict-volatile-bitfields -fsync-libcalls
 # -fthread-jumps -ftoplevel-reorder -ftrapping-math -ftree-bit-ccp
 # -ftree-builtin-call-dce -ftree-ccp -ftree-ch -ftree-coalesce-vars
 # -ftree-copy-prop -ftree-copyrename -ftree-cselim -ftree-dce
 # -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre
 # -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
 # -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop -ftree-pre
 # -ftree-pta -ftree-reassoc -ftree-scev-cprop -ftree-sink -ftree-slsr
 # -ftree-sra -ftree-switch-conversion -ftree-tail-merge -ftree-ter
 # -ftree-vrp -funit-at-a-time -fverbose-asm -fzero-initialized-in-bss
 # -mdivide-traps -mdouble-float -meb -mexplicit-relocs -mextern-sdata
 # -mfp-exceptions -mfp32 -mfused-madd -mglibc -mgp32 -mgpopt -mimadd
 # -mllsc -mlocal-sdata -mlong32 -mno-mdmx -mno-mips16 -mno-mips3d
 # -msoft-float -msplit-addresses

	.section	.text.foo,"ax",@progbits
	.align	2
	.globl	foo
	.set	nomips16
	.set	nomicromips
	.ent	foo
	.type	foo, @function
foo:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
 # 16 "kernel/bounds.c" 1
	
->NR_PAGEFLAGS 23 __NR_PAGEFLAGS	 #
 # 0 "" 2
 # 17 "kernel/bounds.c" 1
	
->MAX_NR_ZONES 2 __MAX_NR_ZONES	 #
 # 0 "" 2
#NO_APP
	j	$31
	.end	foo
	.size	foo, .-foo
	.ident	"GCC: (Buildroot 2015.08.1) 4.9.3"
