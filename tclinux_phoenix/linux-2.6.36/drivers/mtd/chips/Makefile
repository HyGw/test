#
# linux/drivers/chips/Makefile
#

obj-$(CONFIG_MTD)		+= chipreg.o
obj-$(CONFIG_MTD_CFI)		+= cfi_probe.o
obj-$(CONFIG_MTD_CFI_UTIL)	+= cfi_util.o
obj-$(CONFIG_MTD_CFI_STAA)	+= cfi_cmdset_0020.o
obj-$(CONFIG_MTD_CFI_AMDSTD)	+= cfi_cmdset_0002.o
obj-$(CONFIG_MTD_CFI_INTELEXT)	+= cfi_cmdset_0001.o
obj-$(CONFIG_MTD_GEN_PROBE)	+= gen_probe.o
obj-$(CONFIG_MTD_JEDECPROBE)	+= jedec_probe.o
ifneq ($(TCSUPPORT_NEW_SPIFLASH),)
obj-$(CONFIG_MTD_SPIFLASH)	+= newspiflash.o 
else
obj-$(CONFIG_MTD_SPIFLASH)	+= spiflash_tc3162.o
endif
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
obj-y += spi_nand_flash.o
obj-y += spi_controller.o
endif
obj-$(CONFIG_MTD_RAM)		+= map_ram.o
obj-$(CONFIG_MTD_ROM)		+= map_rom.o
obj-$(CONFIG_MTD_ABSENT)	+= map_absent.o
