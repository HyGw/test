#ifndef __LINUX_MTD_FTTDPINIC_H__
#define __LINUX_MTD_FTTDPINIC_H__

/* Both files(linux-2.6.36/include/linux/mtd/fttdp_inic.h and bootrom_int/bootram/include/linux/mtd/fttdp_inic.h) 
 * contents must be synchronized. 
 */

#define ETH_P_ROM			0xBEEF
/* iNIC Client files size */
#define INIC_CLIENT_RAM_SIMU_MAX_SIZE	(0x00400000)	// bootloader + romfile +
														// tclinux + reserverarea
#define INIC_CLIENT_BOOTEXT_SIZE		(0x00008000)
#define INIC_CLIENT_BOOTLOADER_SIZE		(0x00020000)
#define INIC_CLIENT_ROMFILE_SIZE		(0x00010000)
#define INIC_CLIENT_ALLINONE_SIZE		(INIC_CLIENT_RAM_SIMU_MAX_SIZE)
#define INIC_CLIENT_MAC_ADDR_OFFSET		(0x0000ff48)

#define INIC_CLIENT_KERNEL_START_ADDR	(0x80020000)
#define INIC_CLIENT_KERNEL_RAM_START 	(0x80600000)
#define INIC_CLIENT_KERNEL_RAM_END   	(0x80780000)

/* iNIC Client files in iNIC Host flash partition name */
#define INIC_CLIENT_BOOTEXT_NAME			"bootext.ram"
#define INIC_CLIENT_ALLINONE_NAME			"VDSL_CO_allinone"
#define INIC_CLIENT_ROMFILE_NAME			"VDSL_CO_romfile"
#define INIC_CLIENT_DEFAULT_ROMFILE_NAME	"VDSL_CO_default_romfile"

#define INIC_CLIENT_FILE_DIR				"/userfs/VDSL_CO_file/"
#define INIC_CLIENT_MTD_FILE_DIR			"/tmp/"
#define INIC_CLIENT_CURRENT_ROMFILE_DIR		INIC_CLIENT_MTD_FILE_DIR
#define INIC_CLIENT_UPLOAD_ROMFILE_DIR	"/var/tmp/"

#define INIC_CLIENT_ROMFILE_PATH	INIC_CLIENT_CURRENT_ROMFILE_DIR INIC_CLIENT_ROMFILE_NAME
#define INIC_CLIENT_UPLOAD_ROMFILE_PATH	INIC_CLIENT_UPLOAD_ROMFILE_DIR INIC_CLIENT_ROMFILE_NAME

#endif


