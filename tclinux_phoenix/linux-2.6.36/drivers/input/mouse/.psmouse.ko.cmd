cmd_drivers/input/mouse/psmouse.ko := mips-buildroot-linux-gnu-ld -r  -m elf32btsmip -T /opt/tclinux_phoenix/linux-2.6.36/scripts/module-common.lds --build-id  -o drivers/input/mouse/psmouse.ko drivers/input/mouse/psmouse.o drivers/input/mouse/psmouse.mod.o