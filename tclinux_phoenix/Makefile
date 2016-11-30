######################################################################################
#This Makefile is the main entrance for MediaTek CPE SDK maintaining,including:
#firmware building, SDK release, customer SDK release and other test functions for SDK.
#######################################################################################
#######################################################################################
#In 3 cases you need to pay attention to this makefile:
#(1).You have added or modified private source code created by MediaTeK which can not be released to customers directly but can be provided in object format.
#(2).You will be responsbile for new version SDK release, which means you need to update or upgrade this makefile.
#(3).You need to add special target for SDK maintaining or etc.
########################################################################################

#Entrance for firmware building, which will provide default target if no arguments specified.
ifeq ($(strip $(RELEASEBSP) $(CUSTOMERRELEASE)),)
include Project/MakeFile_Main

else
ifneq ($(strip $(BSP)),)
ISBSP=1
else
ISBSP=0
endif
#Entrance for SDK release (by MediaTek).
ifneq ($(strip $(RELEASEBSP)),)	
ifeq ($(strip $(CUSTOMERRELEASE)),)
release:confirm_infor release_all_check release_backup release_profile release_makefile release_config release_doc release_swept release_tar
endif
endif

#Entrance for customer SDK release (by customers)
ifeq ($(strip $(RELEASEBSP)),)	
ifneq ($(strip $(CUSTOMERRELEASE)),)
customer_release:confirm_infor customer_release_backup customer_release_makefile customer_release_swept customer_release_tar
endif
endif

ifneq ($(strip $(CUSTOM)),)
ifneq ($(strip $(KERNEL_2_6_36)),)
include Project/profile/$(CUSTOM)/$(PROFILE)/$(PROFILE)_2_6_36.profile
else
include Project/profile/$(CUSTOM)/$(PROFILE)/$(PROFILE).profile
endif
else
#CUSTOM is the target custom that will be released, and the default custom will be RT.
CUSTOM=RT
ifneq ($(strip $(PROFILE)),)
ifneq ($(strip $(KERNEL_2_6_36)),)
include Project/profile/$(CUSTOM)/$(PROFILE)/$(PROFILE)_2_6_36.profile
else
include Project/profile/$(CUSTOM)/$(PROFILE)/$(PROFILE).profile
endif
endif
endif
include Project/dir.mak

endif
#Error arguments
ifneq ($(strip $(RELEASEBSP)),)	
ifneq ($(strip $(CUSTOMERRELEASE)),)
error_conflict:
	echo "Error argument! You can not release with RELEASEBSP and CUSTOMERRELEASE at the same time!"
	exit 1;
endif
endif

#PROFILE_SET are the set of release profile supported in this release version 
ifneq ($(strip $(KERNEL_2_6_36)),)
PROFILE_SET=rt65168_release rt63365_release rt63368_release mt7510_release mt7505_release CT_E8B_mt7510_E2_release CT_E8C_PON_release mt7515_release mt7520s_fttdp_release mt7520_release en7521_release en7512_release  mt7525_ASB_release en7526_ASB_release
else
PROFILE_SET=tc3162u_release rt65168_release rt63260_release tc3182_release rt63365_release rt63368_release
endif

OPEN_CODE=../open_code
OPEN_CODE_TMP=../open_code_tmp


#Backup_Folder are the backup location for release requisite
BACKUP_FOLDERS=$(APP_BINARY_DIR) \
$(MODULES_PRIV_SRC_DIR)/ko \
$(APP_TCAPILIB_DIR) \
$(APP_PRIVATE_DIR)/TR69_64 \

ifeq ($(strip $(TCSUPPORT_CT)),)
BACKUP_FOLDERS += $(APP_PRIVATE_DIR)/cfg_manager
else
BACKUP_FOLDERS += $(APP_PRIVATE_DIR)/cfg_manager_ct
endif

INFOR0="Attention!!!!"
ifneq ($(strip $(KERNEL_2_6_36)),)
INFOR1="This command can only release SDK with 2.6.36.0 kernel"
INFOR2="If you want to release 2.6.22.15 kernel, please use cmd: make RELEASEBSP=y"
else
INFOR1="This command can only release SDK with 2.6.22.15 kernel"
INFOR2="If you want to release 2.6.36 kernel, please use cmd: make RELEASEBSP=y KERNEL_2_6_36=y"
endif
INFOR3="This command will release Multi-chip SDK, which will remove the un-open files and can't be recovered!!!!!"
INFOR4="Please select the release profile desired. Example: 1 2 3"
INFOR5="Please input your selection:"
INFOR6="Profile you selected is as following:"
INFOR7="if you agree, please input "y",else if you wanna break please input "N". [y/N] ?"
EXIT_INFOR="Your input is not y! The make process will be breaked!"
CONTINUE_INFOR="Entering make process......"
INFOR_CUSTOM="If you wanan release a customer version from SDK, please make sure your command is: "make -f MultiChip.make CUSTOM_RELASE=y""
SEPARATOR="....................................................................................................."


#if RD wanna release with specified profile, the select and confirm will be canceld
ifneq ($(strip $(PROFILE)),)
PROFILE_SELECT = $(PROFILE)
confirm_infor:
	@echo "release with $(PROFILE) profile now..."
	@sleep 1
else
#if RD wanna release with some dedicated profiles, the select and confirm will be triggerd
confirm_infor:
	@echo "****************************************************************************************************"
	@echo $(INFOR0)
	@echo "....................................................................................................."
	@echo $(INFOR1)
	@echo $(INFOR2)
	@echo "....................................................................................................."
	@echo $(INFOR3)
	@echo $(INFOR4)
	@id=0;\
	for profile in $(PROFILE_SET);\
	do \
		echo [$$id] $$profile;\
		id=`expr $$id + 1`; \
	done;
	@echo "****************************************************************************************************"
	@echo $(INFOR5);
	@rm -rf "./.profile_select.tmp";
	@profile_set=(${PROFILE_SET}); \
	PROFILE_SET_NUM=$${profile_set[@]}; \
	read select;\
	echo $(SEPARATOR);\
	while [ "$$select"x == ""x ] ;\
	do \
		echo $(INFOR5);read select;\
	done; \
	while [ "$$select"x == ""x ] ;\
	do \
		fail=0;\
		for index in $$select; \
		do \
			if [[ ( $$index -lt 0 ) || ( $$index -ge $${PROFILE_SET_NUM} ) ]]; \
			then\
				echo "******Wrong input! Please check again!*****"; \
				fail=1; \
				echo $(INFOR5);read select;\
				while [ "$$select"x == ""x ]];\
				do \
					echo $(INFOR5);read select;\
				done; \
				break; \
			fi; \
		done ;\
		if [ $$fail -eq 1 ];\
		then \
			continue; \
		else \
			break; \
		fi; \
	done;\
	echo $(INFOR6);\
	for index in $$select; \
	do \
		echo "******$${profile_set[$$index]}*****"; \
		echo $${profile_set[$$index]}>>"./.profile_select.tmp"; \
	done
	@echo $(INFOR7);
	@read var ;\
	if [[ $$var == y ]]; \
	then \
		echo $(CONTINUE_INFOR); \
	else \
		echo "" >"./.profile_select.tmp";\
		echo $(EXIT_INFOR);\
		exit 1;\
	fi
	@echo $(SEPARATOR)
	@sleep 1;

PROFILE_SELECT=`cat ./.profile_select.tmp`
endif	

release_all_check:release_checktools
	rm -rf ./.profile_summary
ifneq ($(strip $(KERNEL_2_6_36)),)
	for release in ${PROFILE_SELECT} ; \
	do \
			cat Project/profile/$(CUSTOM)/$${release}/$${release}_2_6_36.profile >>./.profile_summary ;\
	done
else
	for release in ${PROFILE_SELECT} ; \
	do \
		cat Project/profile/$(CUSTOM)/$${release}/$${release}.profile >>./.profile_summary ;\
	done
endif


	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	then echo "Please restore the files first before any modification!"; exit 1; \
	else echo "executing all check"; \
	fi

ifneq ($(strip $(CHKDIR)),)
	echo "checking CHDIR"
ifeq ($(CUSTOM),CT)	
	$(TOOLS_DIR)/cplopts_begin $(TRUNK_DIR)/$(CHKDIR) ./.profile_summary $(PROJECT_MENUCONFIG_DIR)/$(CUSTOM).config
else
	$(TOOLS_DIR)/cplopts_begin $(TRUNK_DIR)/$(CHKDIR) ./.profile_summary
endif
else
	echo "checking all files in trunk dir"
ifeq ($(CUSTOM),CT)	
	$(TOOLS_DIR)/cplopts_begin $(TRUNK_DIR) ./.profile_summary $(PROJECT_MENUCONFIG_DIR)/$(CUSTOM).config
else
	$(TOOLS_DIR)/cplopts_begin $(TRUNK_DIR) ./.profile_summary
endif
endif
	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/error; \
	then echo "Compile code error"; exit 1; \
	fi
	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/parsingfile; \
	then echo "Tool couldn't work totally! The error filename is";\
			 cat $(TOOLS_DIR)/cplopts_checktool/tmp/parsingfile;rm -f $(TOOLS_DIR)/cplopts_checktool/tmp/parsingfile;exit 1; \
	else echo "Tool works totally and Compile code success"; \
	fi

#	rm -rf ./.profile_summary

release_restore:release_checktools
	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	then echo "Compile bootbase success"; \
	/bin/sh $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	rm -f $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	rm -rf $(TRUNK_DIR)/backup; \
	fi
	echo "Files  restored!"

release_checktools:
	echo making compile options checking  tools
#       make -C $(TOOLS_DIR)/cplopts_checktool clean
	make -C $(TOOLS_DIR)/cplopts_checktool
	cp -f $(TOOLS_DIR)/cplopts_checktool/cplopts_begin $(TOOLS_DIR)
	cp -f $(TOOLS_DIR)/cplopts_checktool/cplopts_formcheck $(TOOLS_DIR)
	cp -f $(TOOLS_DIR)/cplopts_checktool/cplopts_remove $(TOOLS_DIR)

release_checktools_clean:
	echo cleaning compile options checking  tools
	make -C $(TOOLS_DIR)/cplopts_checktool clean

#Make sure: (1)Profile locates in CUSTOM has been removed and (2) Profile that not named with *_demo in CUSTOM foder have been removed
release_profile:
	find $(TRUNK_DIR)/Project/profile/* -maxdepth 0 -type d -not -name "$(CUSTOM)" | xargs rm -rf
	find $(TRUNK_DIR)/Project/profile/$(CUSTOM)/* -maxdepth 0 -not -name '*_demo' | xargs rm -rf
	

#Substitue makefile in special folder. These targets mostly maintain different codes before and after SDK release, and hence need to use makefile.relese instead.
release_makefile: release_makefile_bsp release_makefile_app

include $(PROJECT_DIR)/release_bsp.mak
ifeq ($(strip $(BSP)),)
include $(APP_DIR)/release_app.mak
else
release_makefile_app:
	echo "release_makefile_app do nothing in BSP version"
endif
ifneq ($(strip $(BSP)),)
include $(PROJECT_DIR)/release_backup_bsp
else
include $(PROJECT_DIR)/release_backup_sdk
endif

#clear up the documents in SDK
release_doc:
	chmod 777 -R $(TRUNK_DIR)/doc
	cp -f $(TRUNK_DIR)/doc/MediaTek_Linux_SDK_Release_Notes.pdf $(TRUNK_DIR)/
	find $(TRUNK_DIR)/doc -mindepth 1 -maxdepth 1 -not -name "*SOP" -not -name "*MANUAL" | xargs rm -rf
	find $(TRUNK_DIR)/doc/SOP -type f -not -name "*.pdf" |xargs rm -rf
	find $(TRUNK_DIR)/doc/MANUAL -type f -not -name "*.pdf" |xargs rm -rf
ifneq ($(strip $(TCSUPPORT_CPU_EN7512) $(TCSUPPORT_CPU_EN7521)),)
	rm -rf $(TRUNK_DIR)/*.pdf
endif	
	
#This target is to add key parameter in Config.in to indicate that the code package has been released. The option "TCSUPPORT_RELEASED_CODE" will be used in controlling of customer configure.
release_config:
	echo "choice" >> Project/config/menuconfig/Config.in
	echo "	  prompt \"Release Type\""  >> Project/config/menuconfig/Config.in
	echo "config TCSUPPORT_RELEASED_CODE" >> Project/config/menuconfig/Config.in
	echo "    bool \"TCSUPPORT_RELEASED_CODE RT\"" >> Project/config/menuconfig/Config.in
	echo "    help"  >> Project/config/menuconfig/Config.in
	echo "         The flag of code release" >> Project/config/menuconfig/Config.in
	echo "endchoice" >> Project/config/menuconfig/Config.in
ifneq ($(strip $(KERNEL_2_6_36)),)
	echo "config TC_RELEASE_2_6_36_KERNEL" >> Project/config/menuconfig/Config.in
	echo "    bool \"TC_RELEASE_2_6_36_KERNEL\"" >> Project/config/menuconfig/Config.in
	echo "		default y" >> Project/config/menuconfig/Config.in
else
	echo "config TC_RELEASE_2_6_22_15_KERNEL" >> Project/config/menuconfig/Config.in
	echo "    bool \"TC_RELEASE_2_6_22_15_KERNEL\"" >> Project/config/menuconfig/Config.in
	echo "		default y" >> Project/config/menuconfig/Config.in
endif
ifneq ($(strip $(TCSUPPORT_CT_CUSTOMMENU)),)
	cp -f Project/config/menuconfig/CTRelPONMenuConfig Project/config/menuconfig/CT.config
endif
	rm -f Project/config/menuconfig/CTRelPONMenuConfig

#swept unopen source codes,customerized scripts,webpages,configure profile and other extra files
release_swept:
	echo "********************release_bsp_swept begin:*****************************"; \
	${MAKE} -f "$(PROJECT_DIR)/release_bsp_swept" ALL ; if [ $$? != 0 ];then exit 1;fi;\
	echo "*********************release_bsp_swept finish:****************************"; \
	if [ $(strip $(ISBSP)) != 1 ]; \
	then \
	echo "********************release_app_swept begin:*****************************"; \
	${MAKE} -f "$(APP_DIR)/release_app_swept" ALL ; if [ $$? != 0 ];then exit 1;fi;\
	echo "*********************release_app_swept finish:****************************"; \
	fi; \
	rm -rf $(TRUNK_DIR)/.profile_summary
	
release_tar:
ifneq ($(strip $(CUSTOM)),)
	tar -czvf ../releasesdk_$(CUSTOM)_`date +%Y%m%d`.tgz .
else
	tar -czvf ../releasesdk_`date +%Y%m%d`.tgz .
endif


#release make for custom, similarly to release_make
customer_release_backup:
ifneq ($(strip $(KERNEL_2_6_36)),)
	for release in ${PROFILE_SELECT} ; \
	do \
		echo "*************************************************"; \
		echo "*************************************************"; \
		echo "*****$$release making and backup in progress********"; \
		echo "*************************************************"; \
		sleep 1;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} CUSTOMERRELEASE=y bootbase kernel modules apps;if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} CUSTOMERRELEASE=y customer_release_end; if [ $$? != 0 ];then exit 1;fi;\
		echo "**************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} clean ;if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
	done
else
	for release in ${PROFILE_SELECT} ; \
	do \
		echo "*************************************************"; \
		echo "*************************************************"; \
		echo "*****$$release making and backup in progress********"; \
		echo "*************************************************"; \
		sleep 1;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} CUSTOMERRELEASE=y bootbase kernel modules apps;if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} CUSTOMERRELEASE=y customer_release_end; if [ $$? != 0 ];then exit 1;fi;\
		echo "**************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} clean ;if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
	done
endif


customer_release_swept:
	rm -rf $(APP_CMDCI_DIR)
	rm -rf $(APP_ETHCMD_DIR)
	rm -rf $(APP_CWMP_DIR)
	rm -rf $(APP_TCCI_DIR)
	rm -rf $(APP_PRIVATE_DIR)/sendicmp/
	rm -rf $(FILESYSTEM_DIR)/dev/*
	rm -rf $(TRUNK_DIR)/$(MODULES_PRIV_SRC_DIR)/modules/2.6.22.15
	rm -rf $(TRUNK_DIR)/$(MODULES_PRIV_SRC_DIR)/modules/2.6.36
	
	find $(APP_CFG_MANAGER_DIR) -name "*.c"|grep -v 'init.c' |xargs rm -rf
	
	find $(APP_IPTABLES_DIR) -name "*.d" | xargs rm -rf
	echo "CUSTOMER_BUILD_RELEASECODE=y" > Project/customer_release/c_release.chk
	
customer_release_makefile:
	cp $(BOOTROM_DIR)/Makefile_C.release $(TRUNK_DIR)/bootrom/Makefile
	cp $(APP_CFG_MANAGER_DIR)/Makefile_C.release $(APP_CFG_MANAGER_DIR)/Makefile
	
customer_release_tar:
ifneq ($(strip $(CUSTOM)),)
	tar -czvf ../releasesdk_$(CUSTOM)_`date +%Y%m%d`.tgz .
else
	tar -czvf ../releasesdk_`date +%Y%m%d`.tgz .
endif

	
Option_Check:
ifeq ($(strip $(OPTION)),)
	@echo "Command input error!";
	@echo "Command example: sudo make PROFILE=tc3162u Option_Check OPTION=TCSUPPORT_XXX_SUPPORT";
	@exit 1;
endif
	@echo "Checking compile option "${OPTION}" in un-open files now and may last for sevreal minutes, please wait!";
	@for file in $(UNOPEN_FILE_DELETE);\
	do \
		echo "searching in $${file}";\
		find $${file} -name "*.c"|xargs grep -in "${OPTION}" >>"./.tmp_optioncheck"; \
		find $${file} -name "*.h"|xargs grep -in "${OPTION}" >>"./.tmp_optioncheck"; \
		echo ".........................................";\
	done

	@if [ -s "./.tmp_optioncheck" ]; then \
		echo "SORRY,YOUR CHECK HAS FAILED!!!!!!!!";\
		sleep 1;\
		echo "This compile option has been found in following files:";\
		cat "./.tmp_optioncheck";\
		rm -f "./.tmp_optioncheck";\
		exit 1;\
		else \
	echo "Congratulations, your check has PASSED! This compile option has not been found in un-open codes!";\
	echo "..............................................................................................";\
	fi
