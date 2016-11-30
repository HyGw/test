SEDCMD="$SEDCMD -e 's/\$echo/\$ECHO/'"
gen="sed $SEDCMD libtool"
eval $gen > libtool2
cp libtool2 libtool
rm libtool2
