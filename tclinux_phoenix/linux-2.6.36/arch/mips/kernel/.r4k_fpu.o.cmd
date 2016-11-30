cmd_arch/mips/kernel/r4k_fpu.o := mips-buildroot-linux-gnu-gcc -Wp,-MD,arch/mips/kernel/.r4k_fpu.o.d  -nostdinc -isystem /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -D"DATAOFFSET=0" -D__ASSEMBLY__ -ffunction-sections  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/ -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic         -c -o arch/mips/kernel/r4k_fpu.o arch/mips/kernel/r4k_fpu.S

deps_arch/mips/kernel/r4k_fpu.o := \
  arch/mips/kernel/r4k_fpu.S \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/mips32/compat.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sgidefs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/fpregdef.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/linkage.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/war.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/regdef.h \

arch/mips/kernel/r4k_fpu.o: $(deps_arch/mips/kernel/r4k_fpu.o)

$(deps_arch/mips/kernel/r4k_fpu.o):
