cmd_fs/fat/msdos.ko := mips-buildroot-linux-gnu-ld -r  -m elf32btsmip -T /opt/tclinux_phoenix/linux-2.6.36/scripts/module-common.lds --build-id  -o fs/fat/msdos.ko fs/fat/msdos.o fs/fat/msdos.mod.o
