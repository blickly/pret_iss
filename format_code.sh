#!/bin/sh

version=`astyle --version 2>&1`
if [ x"$PRET_ISS" = x"" ]; then 
    echo "Please set \$PRET_ISS"
elif [ x"$version" != x"Artistic Style Version 1.22" ]; then
    echo "Please install astyle version 1.22" \
         "and make sure that it is in your \$PATH"
else
    cd $PRET_ISS
    astyle --mode=c --style=java --suffix=none \
           --pad=oper --unpad=paren --convert-tabs \
           -R "./*.h" "./*.cpp" \
           | grep "^formatted"
fi
