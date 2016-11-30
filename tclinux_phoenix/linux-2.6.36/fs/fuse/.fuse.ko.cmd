cmd_fs/fuse/fuse.ko := mips-buildroot-linux-gnu-ld -r  -m elf32btsmip -T /opt/tclinux_phoenix/linux-2.6.36/scripts/module-common.lds --build-id  -o fs/fuse/fuse.ko fs/fuse/fuse.o fs/fuse/fuse.mod.o
