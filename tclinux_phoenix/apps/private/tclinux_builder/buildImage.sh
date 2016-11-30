#!/bin/bash
test -e ./tclinux && rm -rf *tclinux*
blder=tcboot.bin
romfile=romfile.cfg
if [ "$TCSUPPORT_CT_BACKUPROMFILEENCHANCEMENT" != "" ] ;then
if [ "$TCSUPPORT_CT_PON_CY" != "" ] || [ "$TCSUPPORT_CT_PROLINE_SUPPORT" != "" ] ;then
romfile=ctromfile_f.cfg
else
romfile=ctromfile.cfg
fi
else
if [ "$TCSUPPORT_CY_PON" != "" ] ;then
romfile=romfile_f.cfg	
else
romfile=romfile.cfg	
fi
fi
kernel=linux.7z
rootfs=rootfs

if [ "$TCSUPPORT_CT_CUSTOMMENU" != "" ] ;then
touch small_slave.bin
fi

if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" == "" ] ;then
kernel_slave=linux.7z
rootfs_slave=rootfs_slave
fi
if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" != "" ] && [ "$TCSUPPORT_CT_PON_SMALLSYSTEM" != "" ] && [ "$SLAVEBIN" == "" ] ;then
bin_pon_slave=tclinux_slave.bin
cp -f small_slave.bin $bin_pon_slave
fi

TOP_DIR=`pwd`

if [ "$TCSUPPORT_RESTORE_ROM_T" != "" ] ;then
../../../tools/restore_rom_t_info/restore_rom_t_info
fi
#Checking the files' names
test ! -e $blder && echo "bootloader not exist or wrong name" && exit 0
test ! -e $romfile && echo "romfile not exist or wrong name" && exit 0
test ! -e $kernel && echo "linux.7z not exist or wrong name" && exit 0
test ! -e $rootfs && echo "rootfs not exist or wrong name" && exit 0
if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" == "" ] ;then
test ! -e $kernel_slave && echo "linux.7z not exist or wrong name" && exit 0
test ! -e $rootfs_slave && echo "rootfs_slave not exist or wrong name" && exit 0
fi
if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" != "" ] && [ "$TCSUPPORT_CT_PON_SMALLSYSTEM" != "" ] && [ "$SLAVEBIN" == "" ] ;then
test ! -e $bin_pon_slave && echo "tclinux_slave.bin not exist or wrong name" && exit 0
fi

#Calculating the files' sizes
size_b=$(stat -c%s "$blder")
size_f=$(stat -c%s "$romfile")
size_k=$(stat -c%s "$kernel")
size_r=$(stat -c%s "$rootfs")
if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" == "" ] ;then
size_k_slave=$(stat -c%s "$kernel_slave")
size_r_slave=$(stat -c%s "$rootfs_slave")
let "size_slave = $size_k_slave+$size_r_slave + 0x100"

let "main_slave = 0x500000"
if [ $size_slave -gt $main_slave ]; then
	echo "slave image is more than $main_slave,size_slave=$size_slave"
	exit 1;
fi
fi
#if [ "$1" = "pb" ] 
#then
#	upperBound=655360
#else
#	upperBound=851968
#fi

#Calculating the size of the bootloader paddings
if [ "$TCSUPPORT_BOOTROM_LARGE_SIZE" != "" ] ;then
	let "remainder_b=131072-$size_b"
else
	let "remainder_b=65536-$size_b"
fi

#Calculating the size of the romfile paddings
let "remainder_f=65536-$size_f"


if [ "$remainder_b" -eq 0 ]
then
	echo "Need NO padding for bootloader"
else
	echo "Need a padding for bootloader"
	split -b $remainder_b padding
	mv xaa padding_b
fi

if [ "$remainder_f" -eq 0 ]
then
	echo "Need NO padding for romfile"
else
	echo "Need a padding for romfile"
	split -b $remainder_f padding
	mv xaa padding_f
fi

#Claculating the size of the kernel paddings
#if [ "$2" = "tclinux" ]
#then
#	let "remainder_k=$upperBound-$size_k"
#else
#	let "remainder_k=0"
#fi

#Claculating the size of the rootfs paddings
#if [ "$2" = "tclinux" ]
#then
#let "remainder_r=$size_r%0x10000"
#else
#let "remainder_r=0"
#fi
#let "remainder_k=0"
let "remainder_k=0"
let "remainder_r=0"
if [ "$remainder_k" -eq 0 ]
then
	echo "Need NO padding for kernel"
else
	echo "Need a padding for kernel"
	#Preparing the kernel padding for 64k allignment
	split -b $remainder_k padding
	mv xaa padding_k
fi

if [ "$remainder_r" -eq 0 ]
then
	echo "Need NO padding for rootfs"
else
	echo "Need a padding for rootfs"
	#Preparing the rootfs padding for 64k allignment
	let "size_rp=($size_r/0x10000+1)*0x10000-$size_r"
	split -b $size_rp padding
	mv xaa padding_r
fi

if [ "$TCSUPPORT_DUAL_IMAGE_ENHANCE" != "" ] && [ "$ALLINONE_SLAVEBIN" != "" ] ;then
if [ "$TCSUPPORT_DUAL_IMAGE_8M" != "" ];then
#slave image from 0x410000, main image with 4M-128K
main_fw=0x3e0000
else
#slave image from 0x830000, main image with 8M
main_fw=0x800000
fi

if [ "$TCSUPPORT_CT_C5_HEN_SFU" != "" ];then
main_fw=0x400000
fi

let "remainder_m=$main_fw-0x100-$size_k-$size_r-$remainder_r-$remainder_k"
if [ 0 -gt $remainder_m ];then
	echo "main image is more than $main_fw,remainder_m=$remainder_m"
	exit 1;
fi

if [ "$remainder_m" -eq 0 ];then
	echo "Need NO padding for main image"
else
	echo "Need a padding for main image"
	#create file fw_padding for padding main fw
	dd if=/dev/zero of=fw_padding bs=$remainder_m count=1
	split -b $remainder_m fw_padding
	mv xaa fw_padding_m
fi
fi

if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] ;then
#slave image from 0x7a0000, main image with 7.5M
if [ "$TCSUPPORT_CT_PON" != "" ] ;then
if [ "$TCSUPPORT_RESERVEAREA_EXTEND" != "" ] ;then
if ( [ "$TCSUPPORT_BB_NAND" != "" ] && [ "$TCSUPPORT_CT_PON_SMALLSYSTEM" == "" ] ) || [ "$TCSUPPORT_CT_CUSTOMMENU" != "" ] ;then
if [ "$TCSUPPORT_CT_PON_CN" != "" ] ||  [ "$TCSUPPORT_SDN_OVS" != "" ];then
main_fw=0x1400000
else
main_fw=0x1000000
fi
else
main_fw=0x7B0000
fi
else
main_fw=0x780000
fi
else
main_fw=0x680000
fi

if [ "$TCSUPPORT_CT_PON_CAU" != "" ] ;then
main_fw=0x3A0000
fi

let "remainder_m=$main_fw-0x100-$size_k-$size_r-$remainder_r-$remainder_k"
if [ 0 -gt $remainder_m ];then
	echo "main image is more than $main_fw,remainder_m=$remainder_m"
	exit 1;
fi

if [ "$remainder_m" -eq 0 ];then
	echo "Need NO padding for main image"
else
	echo "Need a padding for main image"
	#create file fw_padding for padding main fw
	dd if=/dev/zero of=fw_padding bs=$remainder_m count=1
	split -b $remainder_m fw_padding
	mv xaa fw_padding_m
fi
fi

if [ "$TCSUPPORT_CUC_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_CUSTOMMENU" == "" ] ;then
#slave image from 0x1030000, main image with 16M
let "remainder_m=0x1000000-0x100-$size_k-$size_r-$remainder_r-$remainder_k"
if [ 0 -gt $remainder_m ];then
	echo "main image is more than 16M,remainder_m=$remainder_m"
	exit 1;
fi

if [ "$remainder_m" -eq 0 ];then
	echo "Need NO padding for main image"
else
	echo "Need a padding for main image"
	#create file fw_padding for padding main fw
	dd if=/dev/zero of=fw_padding bs=$remainder_m count=1
	split -b $remainder_m fw_padding
	mv xaa fw_padding_m
fi
fi

#Generating tclinux.bin with trx header
if [ "$remainder_k" -eq 0 ] && [ "$remainder_r" -eq 0 ]
then
	cat linux.7z rootfs > tclinux
elif [ "$remainder_k" -eq 0 ] && [ "$remainder_r" -ne 0 ]
then
	cat linux.7z rootfs padding_r > tclinux
elif [ "$remainder_k" -ne 0 ] && [ "$remainder_r" -eq 0 ]
then
	cat linux.7z padding_k rootfs > tclinux
else
	cat linux.7z padding_k rootfs padding_r > tclinux
fi

#if [ "$2" != "tclinux" ]
#then
#	echo "../../tools/trx/trx -k $size_k -f  tclinux -o tclinux.bin"
#	echo `../../tools/trx/trx -k $size_k -f tclinux -o tclinux.bin`
#fi

#Generating tclinux_slave.bin with trx header
	FWVER=$(cat ../../../filesystem/usr/etc/fwver.conf)
	echo `sed -i "1s/^version.*/version=$FWVER/" ../../../tools/trx/trx_config`
if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" == "" ] ;then
	cat linux.7z rootfs_slave > tclinux_slave	
	if [ "$TCSUPPORT_FREE_BOOTBASE" != "" ] ;then
		echo "../../tools/trx/trx -k $size_k_slave -r $size_r_slave -u $START_ADDR -f tclinux_slave -o tclinux_slave.bin -c ../../tools/trx/trx_config"
		echo `../../../tools/trx/trx -k $size_k_slave -r $size_r_slave -u $START_ADDR -f tclinux_slave -o tclinux_slave.bin -c ../../../tools/trx/trx_config`
	else
		echo "../../tools/trx/trx -k $size_k_slave -r $size_r_slave -f tclinux_slave -o tclinux_slave.bin -c ../../tools/trx/trx_config"
		echo `../../../tools/trx/trx -k $size_k_slave -r $size_r_slave -f tclinux_slave -o tclinux_slave.bin -c ../../../tools/trx/trx_config`
	fi
	test -e ./tclinux_slave

	echo "output file: tclinux_slave.bin"	
fi

if [ "$TCSUPPORT_FREE_BOOTBASE" != "" ] ;then
	echo "../../tools/trx/trx -k $size_k -r $size_r -u $START_ADDR -f tclinux -o tclinux.bin -c ../../tools/trx/trx_config"
	echo `../../../tools/trx/trx -k $size_k -r $size_r -u $START_ADDR -f tclinux -o tclinux.bin -c ../../../tools/trx/trx_config`
else
	echo "../../tools/trx/trx -k $size_k -r $size_r -f tclinux -o tclinux.bin -c ../../tools/trx/trx_config"
	echo `../../../tools/trx/trx -k $size_k -r $size_r -f tclinux -o tclinux.bin -c ../../../tools/trx/trx_config`
fi
rm -rf *xa*
test -e ./padding_k && rm -rf ./padding_k
test -e ./padding_r && rm -rf ./padding_r
#test -e ./tclinux && rm -rf ./tclinux
test -e ./tclinux
#if [ "$2" != "tclinux" ]
#then
	echo "output file: tclinux.bin"
#else
#	echo "output file: $2"
#fi
#cp ras ~/src/.1
if [ "$TCSUPPORT_CT" != "" ] && [ "$SLAVEBIN" != "" ] ;then
	cp -f tclinux.bin small_slave.bin
	echo "output file: small_slave.bin"
fi

cat_cmd=tcboot.bin

if [ "$remainder_b" -ne 0 ] ;then
cat_cmd=$cat_cmd" padding_b"
fi

cat_cmd=$cat_cmd" "$romfile

if [ "$remainder_f" -ne 0 ] ;then
cat_cmd=$cat_cmd" padding_f"
fi

cat_cmd=$cat_cmd" tclinux.bin"

if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$SLAVEBIN" == "" ] && ( [ "$TCSUPPORT_CT_PON" == "" ] || [ "$TCSUPPORT_CT_PON_SMALLSYSTEM" != "" ] ) ;then
cat_cmd=$cat_cmd" fw_padding_m tclinux_slave.bin"
elif [ "$TCSUPPORT_CUC_DUAL_IMAGE" != "" ] || ( [ "$TCSUPPORT_DUAL_IMAGE_ENHANCE" != "" ] && [ "$ALLINONE_SLAVEBIN" != "" ] ) || [ "$TCSUPPORT_CT_PON_C9_HUN" != "" ] || [ "$TCSUPPORT_CT_PON_BIGSYSTEM" != "" ] || [ "$TCSUPPORT_CT_PON_CAU" != "" ];then
cat_cmd=$cat_cmd" fw_padding_m tclinux.bin"
fi

echo "cat $cat_cmd > tclinux_allinone"

cat $cat_cmd > tclinux_allinone

if [ "$TCSUPPORT_BOOTROM_LARGE_SIZE" != "" ] && [ "$TCSUPPORT_BB_NAND" != "" ];then
#Generating tclinux_allinone for NAND,tcboot is 256k,romfile is 256k
	let "remainder_b=262144-$size_b"
if [ "$TCSUPPORT_CT_PON_CN" != "" ] ;then
	let "remainder_f=0x200000-$size_f"
else
	let "remainder_f=262144-$size_f"
fi
	echo "Need a padding for NAND bootloader"
	split -b $remainder_b padding
	mv xaa padding_b
	echo "Need a padding for NAND romfile"
	split -b $remainder_f padding
	mv xaa padding_f
if [ "$TCSUPPORT_CT_PON" != "" ] ;then
	cat tcboot.bin padding_b $romfile padding_f tclinux.bin fw_padding_m tclinux.bin > tclinux_allinone_nand
else
	cat tcboot.bin padding_b romfile.cfg padding_f tclinux.bin > tclinux_allinone_nand
fi
	echo `./byteswap tclinux_allinone_nand`
fi

#if [ "$TCSUPPORT_CT" == "" ] ;then
#add crc32 at the end of tclinux_allineone
../../../tools/trx/trx  -g
if [ "$TCSUPPORT_NAND_BMT" != "" ] ;then
../../../tools/trx/trx  -h
fi
#fi

echo `./byteswap tclinux_allinone`

rm -f *xa*
test -e ./padding_b && rm -f ./padding_b
test -e ./padding_f && rm -f ./padding_f
test -e ./tclinux && rm -f ./tclinux
if ( [ "$TCSUPPORT_DUAL_IMAGE_ENHANCE" != "" ] && [ "$ALLINONE_SLAVEBIN" != "" ] ) || [ "$TCSUPPORT_CT_PON_C9_HUN" != "" ] ;then
test -e ./padding_m && rm -f ./fw_padding_m
fi
if [ "$TCSUPPORT_CT_DUAL_IMAGE" != "" ] && [ "$TCSUPPORT_CT_PON" == "" ] ;then
test -e ./padding_m && rm -f ./fw_padding_m
test -e ./tclinux_slave && rm -f ./tclinux_slave
fi
