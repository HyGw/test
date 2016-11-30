#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
#ifeq ($(strip $(DEVKITARM)),)
#$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
#endif

#include $(DEVKITARM)/ds_rules

#PREFIX = arm-elf-
ifeq ($(TC3262),1)
PREFIX = mips-linux-gnu-
else
PREFIX = mips-linux-
endif

CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar
OBJCOPY	= $(PREFIX)objcopy

# where is the source
SRC_DIR = src

# the base of the include directories
INC_BASE = $(SRC_DIR)/include

# where the include files live
# IP V4
INC_DIRS = $(INC_BASE) $(INC_BASE)/ipv4 $(INC_BASE)/config
# $(INC_BASE)/arch

# IP V6
#INC_DIRS = $(INC_BASE) $(INC_BASE)/ipv6 $(INC_BASE)/config $(INC_BASE)/arch



#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH = -mthumb -mthumb-interwork

# note: arm9tdmi isn't the correct CPU arch, but anything newer and LD
# *insists* it has a FPU or VFP, and it won't take no for an answer!
CFLAGS = -Wall -O3\
	-mcpu=arm9tdmi -mtune=arm9tdmi -fomit-frame-pointer\
	-ffast-math \
	$(ARCH)

CFLAGS += $(addprefix -I,$(INC_DIRS)) -DARM9
CXXFLAGS = $(CFLAGS)

ASFLAGS = $(ARCH)
LDFLAGS	= -specs=ds_arm9.specs $(ARCH) -mno-fpu -Wl,-Map,$(notdir $*.map)



#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------

lwip_lib = liblwip9.a

# IP V4
lwip_lib_ipv4_files = icmp.c ip_addr.c ip.c ip_frag.c
lwip_lib_sources_core_files = dhcp.c inet.c mem.c memp.c netif.c\
	pbuf.c raw.c stats.c sys.c tcp.c tcp_in.c tcp_out.c udp.c\
	$(addprefix ipv4/,$(lwip_lib_ipv4_files))
lwip_lib_netif_files = etharp.c

# IP V6
#lwip_lib_sources_core_files = dhcp.c inet6.c mem.c memp.c netif.c\
#	 pbuf.c raw.c stats.c sys.c tcp.c tcp_in.c tcp_out.c udp.c
lwip_lib_sources = $(addprefix $(SRC_DIR)/core/, $(lwip_lib_sources_core_files)) \
		$(addprefix $(SRC_DIR)/netif/, $(lwip_lib_netif_files))
lwip_lib_objects = $(lwip_lib_sources:.c=.o)


#---------------------------------------------------------------------------------

.PHONY : all
all : $(lwip_lib)

# main targets
#---------------------------------------------------------------------------------
$(lwip_lib) : $(lwip_lib_objects)
	$(AR) rcs $@ $^

.PHONY : clean
clean :
	rm -f $(lwip_lib_objects)
	rm -f $(lwip_lib)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Generate the dependencies
%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's|\($(notdir $*)\)\.o[ :]*|$*.o $@ : |g' > $@; \
		[ -s $@ ] || rm -f $@

# include the dependencies files
include $(lwip_lib_sources:.c=.d)
