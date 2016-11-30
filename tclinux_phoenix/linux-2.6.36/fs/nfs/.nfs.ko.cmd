cmd_fs/nfs/nfs.ko := mips-buildroot-linux-gnu-ld -r  -m elf32btsmip -T /opt/tclinux_phoenix/linux-2.6.36/scripts/module-common.lds --build-id  -o fs/nfs/nfs.ko fs/nfs/nfs.o fs/nfs/nfs.mod.o
