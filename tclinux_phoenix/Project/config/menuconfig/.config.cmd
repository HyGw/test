deps_config := \
	Config.in

include/config/auto.conf: \
	$(deps_config)

$(deps_config): ;
