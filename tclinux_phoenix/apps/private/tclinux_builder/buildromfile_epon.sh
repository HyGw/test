#!/bin/bash
romfile=romfile_epon.cfg

#Checking the files' names
test ! -e $romfile && echo "romfile not exist or wrong name" && exit 0

#because we need create ctromfile.cfg and put it in rootfs first ---added by brian
size_f=$(stat -c%s "$romfile")
echo "../../../tools/trx/trx -d $size_f -f romfile_epon.cfg -o ctromfile_epon.cfg"
echo `../../../tools/trx/trx -d $size_f -f romfile_epon.cfg -o ctromfile_epon.cfg`
echo "cp ctromfile_epon.cfg ../../../filesystem/userfs"
cp ctromfile_epon.cfg ../../../filesystem/userfs

