#!/bin/bash
test -e ./uImage && rm -rf uImage

kernel=linux.7z

test ! -e $kernel && echo "linux.7z not exist or wrong name" && exit 0

#Calculating the files' sizes
size_k=$(stat -c%s "$kernel")

let "remainder_k=0"

if [ "$remainder_k" -eq 0 ]
then
	echo "Need NO padding for kernel"
else
	echo "Need a padding for kernel"
	#Preparing the kernel padding for 64k allignment
	split -b $remainder_k padding
	mv xaa padding_k
fi
	echo "../../../tools/trx/trx -k $size_k -f  linux.7z -o uImage -c ../../../tools/trx/trx_config"
	echo `../../../tools/trx/trx -k $size_k -f linux.7z -o uImage -c ../../../tools/trx/trx_config`






