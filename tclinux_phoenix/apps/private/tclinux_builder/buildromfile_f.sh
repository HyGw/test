#!/bin/bash
romfile=romfile_f.cfg

#Checking the files' names
test ! -e $romfile && echo "romfile not exist or wrong name" && exit 0

#because we need create ctromfile.cfg and put it in rootfs first ---added by brian
size_f=$(stat -c%s "$romfile")
echo "../../../tools/trx/trx -d $size_f -f romfile_f.cfg -o ctromfile_f.cfg"
echo `../../../tools/trx/trx -d $size_f -f romfile_f.cfg -o ctromfile_f.cfg`
echo "cp ctromfile_f.cfg ../../../filesystem/userfs"
cp ctromfile_f.cfg ../../../filesystem/userfs

