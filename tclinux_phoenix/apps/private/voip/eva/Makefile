PWD := $(shell pwd)
ifeq ($(DSP),viking)
GDI_DIR := $(PWD)/gdi
endif
ifeq ($(DSP),mtk)
GDI_DIR := $(PWD)/gdi_mtk
endif
ADAM_DIR := $(PWD)/adam
EVCOM_DIR := $(PWD)/evcom
LOADER_DIR := $(PWD)/loader
SLIC_CTRL_API_DIR := $(PWD)/slic_ctrl/lib
SLIC_CTRL_TEST_DIR := $(PWD)/slic_ctrl/test


All: gdi adam evcom loader slic_ctrl_api slic_ctrl_test

clean: gdi_clean adam_clean evcom_clean loader_clean slic_ctrl_api_clean slic_ctrl_test_clean
	rm -rf $(PWD)/bin/*


gdi:gdi_clean
	if test -f $(GDI_DIR)/Makefile; \
	then echo "Build GDI!"; \
	$(MAKE) -C $(GDI_DIR) LINK=dynamic; \
	$(MAKE) -C $(GDI_DIR) LINK=static; \
	fi

ifeq ($(DSP),viking)
	if test -e $(GDI_DIR)/libgdi_viking.a; \
	then echo "Compile gdi success"; \
	else echo "Compile gdi error!"; exit 1; \
	fi
	cp  $(GDI_DIR)/libgdi_viking.a  $(PWD)/bin/
endif
ifeq ($(DSP),mtk)

	if test -e $(GDI_DIR)/libgdi_mtk.so; \
	then echo "Compile gdi so success"; \
	else echo "Compile gdi so error!"; exit 1; \
	fi
#	cp  $(GDI_DIR)/libgdi_mtk.so  $(PWD)/bin/
	if test -e $(GDI_DIR)/libgdi_mtk.a; \
	then echo "Compile gdi success"; \
	else echo "Compile gdi error!"; exit 1; \
	fi
#	cp  $(GDI_DIR)/libgdi_mtk.a  $(PWD)/bin/
endif

gdi_clean:
	if test -f $(GDI_DIR)/Makefile; \
	then echo "Clean GDI!"; \
	$(MAKE) -C $(GDI_DIR) clean; \
	else echo "Clean GDI! do noting!"; \
	fi
	
adam:adam_clean
	echo "Build ADAM!"
	$(MAKE) -C $(ADAM_DIR) LINK=dynamic
	$(MAKE) -C $(ADAM_DIR) LINK=static
	
	
	if test -e $(ADAM_DIR)/libadam.so; \
	then echo "Compile adam so success"; \
	else echo "Compile adam so error!"; exit 1; \
	fi
	cp  $(ADAM_DIR)/libadam.so  $(PWD)/bin/
	if test -e $(ADAM_DIR)/libadam.a; \
	then echo "Compile adam success"; \
	else echo "Compile adam error!"; exit 1; \
	fi
	cp  $(ADAM_DIR)/libadam.a  $(PWD)/bin/	
	
adam_clean:
	echo "Clean ADAM!"
	$(MAKE) -C $(ADAM_DIR) clean
	
evcom:evcom_clean
	echo "Build EVCOM!"
	$(MAKE) -C $(EVCOM_DIR) LINK=dynamic
	$(MAKE) -C $(EVCOM_DIR) LINK=static
	
	if test -e $(EVCOM_DIR)/evcom; \
	then echo "Compile evcom success"; \
	else echo "Compile evcom error!"; exit 1; \
	fi
	cp  $(EVCOM_DIR)/evcom $(PWD)/bin/	
	
	if test -e $(EVCOM_DIR)/evcoms; \
	then echo "Compile evcoms success"; \
	else echo "Compile evcoms error!"; exit 1; \
	fi
	cp  $(EVCOM_DIR)/evcoms $(PWD)/bin/	
	
evcom_clean:
	echo "Clean EVCOM!"
	$(MAKE) -C $(EVCOM_DIR) clean

loader:loader_clean
	echo "Build loader!"
	$(MAKE) -C $(LOADER_DIR)
	if test -e $(LOADER_DIR)/voip_loader; \
	then echo "Compile loader success"; \
	else echo "Compile loader error!"; exit 1; \
	fi
	cp $(LOADER_DIR)/voip_loader $(PWD)/bin/

loader_clean:
	echo "Clean Loader!"
	$(MAKE) -C $(LOADER_DIR) clean
	
slic_ctrl_api:slic_ctrl_api_clean
	echo "Build slic ctrl api!"
	$(MAKE) -C $(SLIC_CTRL_API_DIR) LINK=dynamic
	$(MAKE) -C $(SLIC_CTRL_API_DIR) LINK=static
	
	if test -e $(SLIC_CTRL_API_DIR)/libslic_ctrl.so; \
	then echo "Compile slic ctrl api so success"; \
	else echo "Compile slic ctrl api so error!"; exit 1; \
	fi
	cp $(SLIC_CTRL_API_DIR)/libslic_ctrl.so $(PWD)/bin/
	
	if test -e $(SLIC_CTRL_API_DIR)/libslic_ctrl.a; \
	then echo "Compile slic ctrl api a success"; \
	else echo "Compile slic ctrl api a error!"; exit 1; \
	fi
	cp $(SLIC_CTRL_API_DIR)/libslic_ctrl.a $(PWD)/bin/

slic_ctrl_api_clean:
	echo "Clean slic ctrl api!"
	$(MAKE) -C $(SLIC_CTRL_API_DIR) clean

slic_ctrl_test:slic_ctrl_test_clean
	echo "Build slic ctrl test!"
	$(MAKE) -C $(SLIC_CTRL_TEST_DIR)
	if test -e $(SLIC_CTRL_TEST_DIR)/slic_ctrl_test; \
	then echo "Compile slic ctrl test success"; \
	else echo "Compile slic ctrl test error!"; exit 1; \
	fi
	cp $(SLIC_CTRL_TEST_DIR)/slic_ctrl_test $(PWD)/bin/

slic_ctrl_test_clean:
	echo "Clean slic ctrl test!"
	$(MAKE) -C $(SLIC_CTRL_TEST_DIR) clean
