cmd_drivers/usb/serial/option.ko := mips-buildroot-linux-gnu-ld -r  -m elf32btsmip -T /opt/tclinux_phoenix/linux-2.6.36/scripts/module-common.lds --build-id  -o drivers/usb/serial/option.ko drivers/usb/serial/option.o drivers/usb/serial/option.mod.o