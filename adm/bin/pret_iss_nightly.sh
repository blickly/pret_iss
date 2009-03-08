#!/bin/bash
# $Id$
# $PRET_ISS/adm/bin/pret_iss_nightly.sh - a nightly build script.
#
# This script expects $PRET_ISS to be set
# It creates :
#   $PRET_ISS/logs/pret_issbuild.txt
#   $PRET_ISS/logs/pret_issnightly.txt

if [ -z "$PRETCFE" ]; then
    export PRETCFE=/home/bldmastr/pretcfe/llvm/llvm-objs/Debug/bin
    echo "$0: the PRETCFE environment variable set"
fi
	 
if [ -z "$PRET_ISS" ]; then
    export PRET_ISS=/home/bldmastr/pret_iss
    echo "$0: the PRET_ISS environment variable set"
#    exit 2
fi

if [ -z "$SYSTEMC" ]; then
    export SYSTEMC=/usr/local/systemc-2.2.0
fi 

export SPARC_TOOLCHAIN=$PRET_ISS/toolchain/sparc-elf-3.4.4/bin
export PATH=$PATH:$SPARC_TOOLCHAIN
export PRET_ISS_LOGS="${PRET_ISS}/logs"

cd $PRET_ISS

echo "### Running `pwd`/bootstrap.sh"
echo "# `date`"
echo "# `uptime`"
echo "# Output in ${PRET_ISS_LOGS}/pret_issbuild.txt"
sh ./bootstrap.sh
./configure --enable-fpic --enable-debugger > "${PRET_ISS_LOGS}/pret_issbuild.txt" 2>&1

echo "### Running make clean all"
echo "# `date`"
echo "# `uptime`"
echo "# Output in ${PRET_ISS_LOGS}/pret_issbuild.txt"
make clean all >> "${PRET_ISS_LOGS}/pret_issbuild.txt" 2>&1

source tests/exports.sh
export PATH=$PATH:$PRETCFE
echo "SYSTEMC_HOME:  $SYSTEMC_HOME"
echo "CXXFLAGS:      $CXXFLAGS"
echo "RT_ADDLD_LIBS: $RT_ADD_LDLIBS"

echo "### Setting the time limit for processes to 20 minutes"
ulimit -t 1200

echo "### Running make regression"
echo "# `date`"
echo "# `uptime`"
echo "# Output in ${PRET_ISS_LOGS}/pret_issnightly.txt"
#make quick_tests > "${PRET_ISS_LOGS}/pret_issnightly.txt" 2>&1
make regression > "${PRET_ISS_LOGS}/pret_issnightly.txt" 2>&1

