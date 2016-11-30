release_makefile_bsp:
#copy makefiles
	cp $(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/Makefile.release $(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/Makefile
	cp $(BOOTROM_DIR)/ddr_cal/Makefile.release $(BOOTROM_DIR)/ddr_cal/Makefile
	cp $(BOOTROM_DIR)/ddr_cal_mt7505/Makefile.release $(BOOTROM_DIR)/ddr_cal_mt7505/Makefile
	cp $(BOOTROM_DIR)/ddr_cal_en7512/Makefile.release $(BOOTROM_DIR)/ddr_cal_en7512/Makefile
#########################################################################################
release_bsp: release_bootrom release_drivers release_app_bsp
	

release_bootrom:
#ifneq ($(strip $(TCSUPPORT_CPU_MT7510)),)
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
	@echo "release Bootrom for MT751221"
	mkdir -p $(BOOTROM_DIR)/ddr_cal_en7512/reserved
	cp -Rf $(BOOTROM_DIR)/ddr_cal_en7512/output/* $(BOOTROM_DIR)/ddr_cal_en7512/reserved/
	cp -Rf $(BOOTROM_DIR)/ddr_cal_en7512/spram.c $(BOOTROM_DIR)/ddr_cal_en7512/reserved/
else
ifneq ($(strip $(TCSUPPORT_CPU_MT7510) $(TCSUPPORT_CPU_MT7520)),)
	@echo "release Bootrom for MT751020"
	mkdir -p $(BOOTROM_DIR)/ddr_cal/reserved
	cp -Rf $(BOOTROM_DIR)/ddr_cal/output/* $(BOOTROM_DIR)/ddr_cal/reserved/
	cp -Rf $(BOOTROM_DIR)/ddr_cal/spram.c $(BOOTROM_DIR)/ddr_cal/reserved/
else
ifneq ($(strip $(TCSUPPORT_CPU_MT7505)),)
	@echo "release Bootrom for MT7505"
	mkdir -p $(BOOTROM_DIR)/ddr_cal_mt7505/reserved
	cp -Rf $(BOOTROM_DIR)/ddr_cal_mt7505/output/* $(BOOTROM_DIR)/ddr_cal_mt7505/reserved/
	cp -Rf $(BOOTROM_DIR)/ddr_cal_mt7505/spram.c $(BOOTROM_DIR)/ddr_cal_mt7505/reserved/
else
	echo "nothing to do!";
endif
endif
endif
release_drivers:
	mkdir -p $(MODULES_PRIV_SRC_DIR)/ko/modules/$(TCPLATFORM)
	cp -rf $(FILESYSTEM_DIR)/lib/modules/* $(MODULES_PRIV_SRC_DIR)/ko/modules/$(TCPLATFORM)/
#	rm -rf $(MODULES_PRIV_SRC_DIR)/tcci/version.c
	mkdir -p $(MODULES_PRIV_SRC_DIR)/obj/$(TCPLATFORM)
	cp -rf $(MODULES_PRIV_SRC_DIR)/tcci/version.o $(MODULES_PRIV_SRC_DIR)/obj/$(TCPLATFORM)/

release_app_bsp:
	echo "release app bsp"
	
release_chk:
	if test -d $(MODULES_PRIV_SRC_DIR)/ko/modules/$(TCPLATFORM); \
	then echo "Release Src Code, you can not release second time!"; exit 1;\
	else echo "Origin Src Code"; \
	fi
