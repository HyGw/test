cmd_arch/mips/kernel/genex.o := mips-buildroot-linux-gnu-gcc -Wp,-MD,arch/mips/kernel/.genex.o.d  -nostdinc -isystem /opt/trendchip/mips-linux-glibc-4.9.3/usr/lib/gcc/mips-buildroot-linux-gnu/4.9.3/include -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0xffffffff80002000" -D"DATAOFFSET=0" -D__ASSEMBLY__ -ffunction-sections  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/ -I/opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic         -c -o arch/mips/kernel/genex.o arch/mips/kernel/genex.S

deps_arch/mips/kernel/genex.o := \
  arch/mips/kernel/genex.S \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
    $(wildcard include/config/cpu/tc3162.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/mips/tc3262.h) \
    $(wildcard include/config/mips/mt/smtc/im/backstop.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/hardware/watchpoints.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/sgidefs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asmmacro.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mips/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asmmacro-32.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/regdef.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/fpregdef.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
  include/linux/linkage.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/linkage.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/tc3162/war.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mipsmtregs.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/cacheops.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/irq/cpu.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/stackframe.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/mips/pgd/c0/context.h) \
    $(wildcard include/config/cpu/jump/workarounds.h) \
    $(wildcard include/config/cpu/loongson2f.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/page.h \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/linux/const.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  /opt/tclinux_phoenix/linux-2.6.36/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/n32.h) \

arch/mips/kernel/genex.o: $(deps_arch/mips/kernel/genex.o)

$(deps_arch/mips/kernel/genex.o):
