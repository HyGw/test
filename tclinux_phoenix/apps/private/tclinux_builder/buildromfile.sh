#!/bin/bash
romfile=romfile.cfg

#Checking the files' names
test ! -e $romfile && echo "romfile not exist or wrong name" && exit 0

#because we need create ctromfile.cfg and put it in rootfs first ---added by brian
size_f=$(stat -c%s "$romfile")
echo "../../../tools/trx/trx -d $size_f -f romfile.cfg -o ctromfile.cfg"
echo `../../../tools/trx/trx -d $size_f -f romfile.cfg -o ctromfile.cfg`
echo "cp ctromfile.cfg ../../../filesystem/userfs"
cp ctromfile.cfg ../../../filesystem/userfs

folder=../../../filesystem/userfs
for file in ${folder}/*; do
	name="${file##*/}"
	if [ "$name" != "romfile.cfg" ] && [ "$name" != "romfile_epon.cfg" ] && [ "${name:0:7}" = "romfile" ] && [ "${name/.cfg/}" != "$name" ] && [ "${name/_f.cfg/}" = "$name" ] ;then
		cp $file ./
		size_f=$(stat -c%s "$name")
		echo "../../../tools/trx/trx -d $size_f -f $name -o ct$name"
		echo `../../../tools/trx/trx -d $size_f -f $name -o ct$name`
		echo "cp ct$name $folder"
		cp ct$name $folder
		rm -f $file
#if/*TCSUPPORT_COMPILE*/ defined(TCSUPPORT_CT_ZIPROMFILE)
		if [ "$TCSUPPORT_CT_ZIPROMFILE" != "" ]; then
			cp -f $folder/ct$name ctromfile.cfg
			tar -czvf romfile.gz ctromfile.cfg
			rm -f ctromfile.cfg
			mv -f romfile.gz $folder/ct$name
		fi
#endif/*TCSUPPORT_COMPILE*/
	fi
done
