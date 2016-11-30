#ifndef __MTD_PARTITION_PARSE_H
#define __MTD_PARTITION_PARSE_H


extern int ecnt_parse_cmdline_partitions(void);
extern uint32_t ecnt_get_mtd_offset(char *name);
extern uint32_t ecnt_get_mtd_size(char *name);
extern uint32_t ecnt_get_tclinux_mtd_offset(void);
extern uint32_t ecnt_get_tclinux_slave_mtd_offset(void);
extern uint32_t ecnt_get_tclinux_flash_offset(void);
extern uint32_t ecnt_get_tclinux_slave_flash_offset(void);
extern uint32_t ecnt_get_reservearea_flash_offset(void);
extern uint32_t ecnt_get_tclinux_size(void);
extern uint32_t ecnt_get_tclinux_slave_size(void);
extern uint32_t ecnt_get_kernel_size(unsigned int erasesize);
extern uint32_t ecnt_get_kernel_slave_size(unsigned int erasesize);
extern uint32_t ecnt_get_reservearea_size(void);
extern uint32_t ecnt_get_boot_size(void);
extern uint32_t ecnt_get_romfile_size(void);
extern int do_get_mtd_info(int argc, char *argv[]);

#endif
