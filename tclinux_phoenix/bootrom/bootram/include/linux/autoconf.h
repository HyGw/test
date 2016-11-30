/*
 * Automatically generated C config: don't edit
 */
#define AUTOCONF_INCLUDED
#define CONFIG_MIPS 1
#undef  CONFIG_SMP
#define CONFIG_UCLINUX 1
#define CONFIG_UID16 1

/*
 * Code maturity level options
 */
#define CONFIG_EXPERIMENTAL 1

/*
 * Machine selection
 */
#undef  CONFIG_TRENDCHIP_TC3160
#define CONFIG_TRENDCHIP_TC3162 1
#define CONFIG_TRENDCHIP_WDL100 1
#define CONFIG_TC3162_RAMSIZE 0x800000
#undef  CONFIG_FULLDEBUG
#undef  CONFIG_SUNPLUS
#undef  SUNPLUS_NETWORK_BOOTROM
#undef  CONFIG_CONTIGUOUS_PAGE_ALLOC
#undef  CONFIG_MEM_MAP
#undef  CONFIG_NO_MMU_LARGE_ALLOCS
#define CONFIG_RWSEM_GENERIC_SPINLOCK 1
#undef  CONFIG_RWSEM_XCHGADD_ALGORITHM
#undef  CONFIG_MCA
#undef  CONFIG_SBUS
#define CONFIG_NEW_IRQ 1
#define CONFIG_NEW_TIME_C 1
#undef  CONFIG_ISA
#undef  CONFIG_EISA
#undef  CONFIG_PCI
#undef  CONFIG_I8259

/*
 * Loadable module support
 */
#undef  CONFIG_MODULES

/*
 * CPU selection
 */
#ifndef TC3262
#define CONFIG_CPU_R3000 1
#endif
#undef  CONFIG_CPU_R6000
#undef  CONFIG_CPU_VR41XX
#undef  CONFIG_CPU_R4300
#undef  CONFIG_CPU_R4X00
#undef  CONFIG_CPU_R5000
#undef  CONFIG_CPU_R5432
#undef  CONFIG_CPU_RM7000
#undef  CONFIG_CPU_NEVADA
#undef  CONFIG_CPU_R10000
#undef  CONFIG_CPU_SB1
#undef  CONFIG_CPU_MIPS32
#undef  CONFIG_CPU_MIPS64
#undef  CONFIG_CPU_ADVANCED
#undef  CONFIG_CPU_HAS_LLSC
#undef  CONFIG_CPU_HAS_LLDSCD
#undef  CONFIG_CPU_HAS_WB

/*
 * General setup
 */
#undef  CONFIG_CPU_LITTLE_ENDIAN
#undef  CONFIG_CPU_LITTLE_ENDIAN
#define CONFIG_KCORE_ELF 1
#define CONFIG_ELF_KERNEL 1
#undef  CONFIG_BINFMT_IRIX
#undef  CONFIG_FORWARD_KEYBOARD
#define CONFIG_BINFMT_FLAT 1
#define CONFIG_BINFMT_ZFLAT 1
#undef  CONFIG_BINFMT_FLAT_SEPARATELY
#undef  CONFIG_BINFMT_AOUT
#undef  CONFIG_BINFMT_ELF
#undef  CONFIG_BINFMT_MISC
#define CONFIG_CMDLINE "root=/dev/ram0"
#define CONFIG_NET 1
#undef  CONFIG_HOTPLUG
#undef  CONFIG_PCMCIA
#undef  CONFIG_SYSVIPC
#undef  CONFIG_BSD_PROCESS_ACCT
#define CONFIG_SYSCTL 1

/*
 * Memory Technology Devices (MTD)
 */
#define CONFIG_MTD 1
#undef  CONFIG_MTD_DEBUG
#define CONFIG_MTD_PARTITIONS 1
#undef  CONFIG_MTD_REDBOOT_PARTS

/*
 * User Modules And Translation Layers
 */
#define CONFIG_MTD_CHAR 1
#define CONFIG_MTD_BLOCK 1
#undef  CONFIG_FTL
#undef  CONFIG_NFTL

/*
 * RAM/ROM/Flash chip drivers
 */
#define CONFIG_MTD_CFI 1
#undef  CONFIG_MTD_JEDECPROBE
#define CONFIG_MTD_GEN_PROBE 1
#undef  CONFIG_MTD_CFI_ADV_OPTIONS
#undef  CONFIG_MTD_CFI_INTELEXT
#define CONFIG_MTD_CFI_AMDSTD 1
#undef  CONFIG_MTD_RAM
#undef  CONFIG_MTD_ROM
#undef  CONFIG_MTD_ABSENT
#define CONFIG_MTD_OBSOLETE_CHIPS 1
#define CONFIG_MTD_AMDSTD 1
#undef  CONFIG_MTD_SHARP
#undef  CONFIG_MTD_JEDEC

/*
 * Mapping drivers for chip access
 */
#undef  CONFIG_MTD_PHYSMAP
#undef  CONFIG_MTD_NETtel
#undef  CONFIG_MTD_NETteluC
#undef  CONFIG_MTD_CSTM_MIPS_IXX
#undef  CONFIG_MTD_OCELOT
#undef  CONFIG_TC_PART

/*
 * Self-contained MTD device drivers
 */
#undef  CONFIG_MTD_PMC551
#undef  CONFIG_MTD_SLRAM
#undef  CONFIG_MTD_MTDRAM
#undef  CONFIG_MTD_BLKMTD

/*
 * Disk-On-Chip Device Drivers
 */
#undef  CONFIG_MTD_DOC1000
#undef  CONFIG_MTD_DOC2000
#undef  CONFIG_MTD_DOC2001
#undef  CONFIG_MTD_DOCPROBE

/*
 * NAND Flash Device Drivers
 */
#undef  CONFIG_MTD_NAND

/*
 * Parallel port support
 */
#undef  CONFIG_PARPORT

/*
 * Block devices
 */
#undef  CONFIG_BLK_DEV_FD
#undef  CONFIG_BLK_DEV_XD
#undef  CONFIG_PARIDE
#undef  CONFIG_BLK_CPQ_DA
#undef  CONFIG_BLK_CPQ_CISS_DA
#undef  CONFIG_BLK_DEV_DAC960
#define CONFIG_BLK_DEV_LOOP 1
#undef  CONFIG_BLK_DEV_NBD
#define CONFIG_BLK_DEV_RAM 1
#define CONFIG_BLK_DEV_RAM_SIZE (2048)
#define CONFIG_BLK_DEV_INITRD 1
#undef  CONFIG_BLK_DEV_BLKMEM

/*
 * MIPS initrd option
 */
#define CONFIG_EMBEDDED_RAMDISK 1
#define CONFIG_COMPRESS_RAMDISK 1

/*
 * Multi-device support (RAID and LVM)
 */
#undef  CONFIG_MD
#undef  CONFIG_BLK_DEV_MD
#undef  CONFIG_MD_LINEAR
#undef  CONFIG_MD_RAID0
#undef  CONFIG_MD_RAID1
#undef  CONFIG_MD_RAID5
#undef  CONFIG_MD_MULTIPATH
#undef  CONFIG_BLK_DEV_LVM

/*
 * Networking options
 */
#define CONFIG_PACKET 1
#undef  CONFIG_PACKET_MMAP
#undef  CONFIG_NETLINK_DEV
#undef  CONFIG_NETFILTER
#undef  CONFIG_FILTER
#define CONFIG_UNIX 1
#define CONFIG_INET 1
#define CONFIG_IP_MULTICAST 1
#undef  CONFIG_IP_ADVANCED_ROUTER
#define CONFIG_IP_PNP 1
#undef  CONFIG_IP_PNP_DHCP
#undef  CONFIG_IP_PNP_BOOTP
#undef  CONFIG_IP_PNP_RARP
#undef  CONFIG_NET_IPIP
#undef  CONFIG_NET_IPGRE
#undef  CONFIG_IP_MROUTE
#undef  CONFIG_ARPD
#undef  CONFIG_INET_ECN
#undef  CONFIG_SYN_COOKIES
#undef  CONFIG_IPV6
#undef  CONFIG_KHTTPD
#undef  CONFIG_ATM
#undef  CONFIG_VLAN_8021Q

/*
 *  
 */
#undef  CONFIG_IPX
#undef  CONFIG_ATALK
#undef  CONFIG_DECNET
#undef  CONFIG_BRIDGE
#undef  CONFIG_X25
#undef  CONFIG_LAPB
#undef  CONFIG_LLC
#undef  CONFIG_NET_DIVERT
#undef  CONFIG_ECONET
#undef  CONFIG_WAN_ROUTER
#undef  CONFIG_NET_FASTROUTE
#undef  CONFIG_NET_HW_FLOWCONTROL

/*
 * QoS and/or fair queueing
 */
#undef  CONFIG_NET_SCHED

/*
 * Telephony Support
 */
#undef  CONFIG_PHONE
#undef  CONFIG_PHONE_IXJ
#undef  CONFIG_PHONE_IXJ_PCMCIA

/*
 * ATA/IDE/MFM/RLL support
 */
#undef  CONFIG_IDE
#undef  CONFIG_BLK_DEV_IDE_MODES
#undef  CONFIG_BLK_DEV_HD

/*
 * SCSI support
 */
#undef  CONFIG_SCSI

/*
 * I2O device support
 */
#undef  CONFIG_I2O
#undef  CONFIG_I2O_BLOCK
#undef  CONFIG_I2O_LAN
#undef  CONFIG_I2O_SCSI
#undef  CONFIG_I2O_PROC

/*
 * Network device support
 */
#define CONFIG_NETDEVICES 1

/*
 * ARCnet devices
 */
#undef  CONFIG_ARCNET
#undef  CONFIG_DUMMY
#undef  CONFIG_BONDING
#undef  CONFIG_EQUALIZER
#undef  CONFIG_TUN
#undef  CONFIG_ETHERTAP

/*
 * Ethernet (10 or 100Mbit)
 */
#define CONFIG_NET_ETHERNET 1
#undef  CONFIG_SUNLANCE
#undef  CONFIG_SUNBMAC
#undef  CONFIG_SUNQE
#undef  CONFIG_SUNLANCE
#undef  CONFIG_SUNGEM
#undef  CONFIG_NET_VENDOR_3COM
#undef  CONFIG_LANCE
#undef  CONFIG_NET_VENDOR_SMC
#undef  CONFIG_NET_VENDOR_RACAL
#undef  CONFIG_NET_ISA
#undef  CONFIG_NET_PCI
#undef  CONFIG_NET_POCKET
#undef  CONFIG_FEC
#undef  CONFIG_CS89x0

/*
 * Ethernet (1000 Mbit)
 */
#undef  CONFIG_ACENIC
#undef  CONFIG_DL2K
#undef  CONFIG_MYRI_SBUS
#undef  CONFIG_NS83820
#undef  CONFIG_HAMACHI
#undef  CONFIG_YELLOWFIN
#undef  CONFIG_SK98LIN
#undef  CONFIG_FDDI
#undef  CONFIG_HIPPI
#undef  CONFIG_PLIP
#undef  CONFIG_PPP
#undef  CONFIG_SLIP

/*
 * Wireless LAN (non-hamradio)
 */
#undef  CONFIG_NET_RADIO

/*
 * Token Ring devices
 */
#undef  CONFIG_TR
#undef  CONFIG_NET_FC
#undef  CONFIG_RCPCI
#undef  CONFIG_SHAPER

/*
 * Wan interfaces
 */
#undef  CONFIG_WAN

/*
 * Amateur Radio support
 */
#undef  CONFIG_HAMRADIO

/*
 * IrDA (infrared) support
 */
#undef  CONFIG_IRDA

/*
 * ISDN subsystem
 */
#undef  CONFIG_ISDN

/*
 * Old CD-ROM drivers (not SCSI, not IDE)
 */
#undef  CONFIG_CD_NO_IDESCSI

/*
 * TrendChip TC3162 devices
 */
#define CONFIG_TC3162_SIO 1
#define CONFIG_TC3162_SIO_CONSOLE 1
#define CONFIG_TC3162_ETHERNET 1
#undef  CONFIG_TC3162_ADM6996_ETHERNET
#undef  CONFIG_TC3162_PCMCIA
#undef  CONFIG_TC3162_USB
#define CONFIG_TC3162_WATCHDOG_TIMER 1

/*
 * Character devices
 */
#undef  CONFIG_LEDMAN
#undef  CONFIG_VT
#undef  CONFIG_SERIAL
#undef  CONFIG_SERIAL_EXTENDED
#undef  CONFIG_SERIAL_NONSTANDARD
#undef  CONFIG_UNIX98_PTYS

/*
 * I2C support
 */
#undef  CONFIG_I2C

/*
 * Mice
 */
#undef  CONFIG_BUSMOUSE
#undef  CONFIG_MOUSE

/*
 * Joysticks
 */
#undef  CONFIG_INPUT_GAMEPORT

/*
 * Input core support is needed for gameports
 */

/*
 * Input core support is needed for joysticks
 */
#undef  CONFIG_QIC02_TAPE

/*
 * Watchdog Cards
 */
#undef  CONFIG_WATCHDOG
#undef  CONFIG_INTEL_RNG
#undef  CONFIG_NVRAM
#undef  CONFIG_RTC
#undef  CONFIG_DTLK
#undef  CONFIG_R3964
#undef  CONFIG_APPLICOM

/*
 * Ftape, the floppy tape device driver
 */
#undef  CONFIG_FTAPE
#undef  CONFIG_AGP
#undef  CONFIG_DRM

/*
 * Multimedia devices
 */
#undef  CONFIG_VIDEO_DEV

/*
 * File systems
 */
#undef  CONFIG_QUOTA
#undef  CONFIG_AUTOFS_FS
#undef  CONFIG_AUTOFS4_FS
#undef  CONFIG_REISERFS_FS
#undef  CONFIG_REISERFS_CHECK
#undef  CONFIG_REISERFS_PROC_INFO
#undef  CONFIG_ADFS_FS
#undef  CONFIG_ADFS_FS_RW
#undef  CONFIG_AFFS_FS
#undef  CONFIG_HFS_FS
#undef  CONFIG_BFS_FS
#undef  CONFIG_EXT3_FS
#undef  CONFIG_JBD
#undef  CONFIG_JBD_DEBUG
#undef  CONFIG_FAT_FS
#undef  CONFIG_MSDOS_FS
#undef  CONFIG_UMSDOS_FS
#undef  CONFIG_VFAT_FS
#undef  CONFIG_EFS_FS
#undef  CONFIG_JFFS_FS
#undef  CONFIG_JFFS2_FS
#undef  CONFIG_CRAMFS
#undef  CONFIG_TMPFS
#undef  CONFIG_RAMFS
#undef  CONFIG_ISO9660_FS
#undef  CONFIG_JOLIET
#undef  CONFIG_ZISOFS
#undef  CONFIG_MINIX_FS
#undef  CONFIG_VXFS_FS
#undef  CONFIG_NTFS_FS
#undef  CONFIG_NTFS_RW
#undef  CONFIG_HPFS_FS
#define CONFIG_PROC_FS 1
#undef  CONFIG_DEVFS_FS
#undef  CONFIG_DEVFS_MOUNT
#undef  CONFIG_DEVFS_DEBUG
#undef  CONFIG_DEVPTS_FS
#undef  CONFIG_QNX4FS_FS
#undef  CONFIG_QNX4FS_RW
#undef  CONFIG_ROMFS_FS
#define CONFIG_EXT2_FS 1
#undef  CONFIG_SYSV_FS
#undef  CONFIG_UDF_FS
#undef  CONFIG_UDF_RW
#undef  CONFIG_UFS_FS
#undef  CONFIG_UFS_FS_WRITE

/*
 * Network File Systems
 */
#undef  CONFIG_CODA_FS
#undef  CONFIG_INTERMEZZO_FS
#undef  CONFIG_NFS_FS
#undef  CONFIG_NFS_V3
#undef  CONFIG_ROOT_NFS
#undef  CONFIG_NFSD
#undef  CONFIG_NFSD_V3
#undef  CONFIG_SUNRPC
#undef  CONFIG_LOCKD
#undef  CONFIG_SMB_FS
#undef  CONFIG_NCP_FS
#undef  CONFIG_NCPFS_PACKET_SIGNING
#undef  CONFIG_NCPFS_IOCTL_LOCKING
#undef  CONFIG_NCPFS_STRONG
#undef  CONFIG_NCPFS_NFS_NS
#undef  CONFIG_NCPFS_OS2_NS
#undef  CONFIG_NCPFS_SMALLDOS
#undef  CONFIG_NCPFS_NLS
#undef  CONFIG_NCPFS_EXTRAS
#undef  CONFIG_ZISOFS_FS
#undef  CONFIG_ZLIB_FS_INFLATE

/*
 * Partition Types
 */
#undef  CONFIG_PARTITION_ADVANCED
#define CONFIG_MSDOS_PARTITION 1
#undef  CONFIG_SMB_NLS
#undef  CONFIG_NLS

/*
 * Sound
 */
#undef  CONFIG_SOUND

/*
 * USB support
 */
#undef  CONFIG_USB

/*
 * USB Controllers
 */
#undef  CONFIG_USB_UHCI
#undef  CONFIG_USB_UHCI_ALT
#undef  CONFIG_USB_OHCI

/*
 * USB Device Class drivers
 */
#undef  CONFIG_USB_AUDIO
#undef  CONFIG_USB_BLUETOOTH
#undef  CONFIG_USB_STORAGE
#undef  CONFIG_USB_STORAGE_DEBUG
#undef  CONFIG_USB_STORAGE_DATAFAB
#undef  CONFIG_USB_STORAGE_FREECOM
#undef  CONFIG_USB_STORAGE_ISD200
#undef  CONFIG_USB_STORAGE_DPCM
#undef  CONFIG_USB_STORAGE_HP8200e
#undef  CONFIG_USB_STORAGE_SDDR09
#undef  CONFIG_USB_STORAGE_JUMPSHOT
#undef  CONFIG_USB_ACM
#undef  CONFIG_USB_PRINTER

/*
 * USB Human Interface Devices (HID)
 */

/*
 *   Input core support is needed for USB HID
 */

/*
 * USB Imaging devices
 */
#undef  CONFIG_USB_DC2XX
#undef  CONFIG_USB_MDC800
#undef  CONFIG_USB_SCANNER
#undef  CONFIG_USB_MICROTEK
#undef  CONFIG_USB_HPUSBSCSI

/*
 * USB Multimedia devices
 */

/*
 *   Video4Linux support is needed for USB Multimedia device support
 */

/*
 * USB Network adaptors
 */
#undef  CONFIG_USB_PEGASUS
#undef  CONFIG_USB_KAWETH
#undef  CONFIG_USB_CATC
#undef  CONFIG_USB_CDCETHER
#undef  CONFIG_USB_USBNET

/*
 * USB port drivers
 */
#undef  CONFIG_USB_USS720

/*
 * USB Serial Converter support
 */
#undef  CONFIG_USB_SERIAL
#undef  CONFIG_USB_SERIAL_GENERIC
#undef  CONFIG_USB_SERIAL_BELKIN
#undef  CONFIG_USB_SERIAL_WHITEHEAT
#undef  CONFIG_USB_SERIAL_DIGI_ACCELEPORT
#undef  CONFIG_USB_SERIAL_EMPEG
#undef  CONFIG_USB_SERIAL_FTDI_SIO
#undef  CONFIG_USB_SERIAL_VISOR
#undef  CONFIG_USB_SERIAL_IR
#undef  CONFIG_USB_SERIAL_EDGEPORT
#undef  CONFIG_USB_SERIAL_KEYSPAN_PDA
#undef  CONFIG_USB_SERIAL_KEYSPAN
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA28
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA28X
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA28XA
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA28XB
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA19
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA18X
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA19W
#undef  CONFIG_USB_SERIAL_KEYSPAN_USA49W
#undef  CONFIG_USB_SERIAL_MCT_U232
#undef  CONFIG_USB_SERIAL_PL2303
#undef  CONFIG_USB_SERIAL_CYBERJACK
#undef  CONFIG_USB_SERIAL_XIRCOM
#undef  CONFIG_USB_SERIAL_OMNINET

/*
 * USB Miscellaneous drivers
 */
#undef  CONFIG_USB_RIO500

/*
 * Input core support
 */
#undef  CONFIG_INPUT
#undef  CONFIG_INPUT_KEYBDEV
#undef  CONFIG_INPUT_MOUSEDEV
#undef  CONFIG_INPUT_JOYDEV
#undef  CONFIG_INPUT_EVDEV

/*
 * Kernel hacking
 */
#define CONFIG_CROSSCOMPILE 1
#undef  CONFIG_MAGIC_SYSRQ
#undef  CONFIG_MIPS_UNCACHED
#undef  CONFIG_DEBUG_SLAB
