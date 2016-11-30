cmd_arch/mips/lib/memcpy-inatomic.o := mips-buildroot-linux-gnu-gcc -Wp,-MD,arch/mips/lib/.memcpy-inatomic.o.d  -nostdinc -isystem /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -D"DATAOFFSET=0" -D__ASSEMBLY__ -ffunction-sections  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/ -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic        -c -o arch/mips/lib/memcpy-inatomic.o arch/mips/lib/memcpy-inatomic.S

deps_arch/mips/lib/memcpy-inatomic.o := \
  arch/mips/lib/memcpy-inatomic.S \
    $(wildcard include/config/dma/noncoherent.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/mips/malta.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/cpu/little/endian.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sgidefs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/regdef.h \

arch/mips/lib/memcpy-inatomic.o: $(deps_arch/mips/lib/memcpy-inatomic.o)

$(deps_arch/mips/lib/memcpy-inatomic.o):
