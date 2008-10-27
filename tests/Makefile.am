####################################################################
#Copyright (c) 2007-2008 The Regents of the University of California.
#All rights reserved.
#
#Permission is hereby granted, without written agreement and without
#license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in all
# copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
# THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
# PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
# CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
# ENHANCEMENTS, OR MODIFICATIONS.
#
# 						PT_COPYRIGHT_VERSION_2
# 						COPYRIGHTENDKEY
# $Author$
# $Date$
# $Id$
#####################################################################

#bin_SCRIPTS=verify.pl

# verify.pl: scripts/verify.pl.in
# 	cxx='echo "${CXXFLAGS}" |  sed -e 's/\//\\\//g''
# 	cat scripts/verify.pl.in | sed -e 's/"pret_ccflags"/\" ${cxx} \"/' > scripts/verify.pl	

bin_SCRIPTS=exports.sh verify.pl regression.h


if USESYSTEMC
  ADDFLAG=
  SCLDFLAG=-lsystemc
  SCTESTS=pret/core
else
  ADDFLAG=-D_NO_SYSTEMC_
  SCLDFLAG=
  SCTESTS=
endif

verify.pl: 
	cat ${PRET_ISS}/tests/scripts/verify.pl.in | sed -e 's/-lsystemc/${SCLDFLAG}/g' > ${PRET_ISS}/tests/scripts/verify.pl
	chmod +x ${PRET_ISS}/tests/scripts/verify.pl

exports.sh: ../scripts/exports.sh.in
	 cat ${PRET_ISS}/scripts/exports.sh.in | sed -e  's/ADDFLAGS/${ADDFLAG}/' > ${PRET_ISS}/tests/exports.sh

regression.h:
	echo "#ifndef _REGRESSION_H_" > ${PRET_ISS}/include/regression.h
	echo "#define _REGRESSION_H_" >> ${PRET_ISS}/include/regression.h
	echo "#include <string>" >> ${PRET_ISS}/include/regression.h
	echo "static string pret_iss_path = \"${PRET_ISS}\";" >> ${PRET_ISS}/include/regression.h
	echo "#endif" >> ${PRET_ISS}/include/regression.h

regression: regression.h verify.pl
	${PRET_ISS}/tests/scripts/verify.pl -T ${SCTESTS} pret/sys pret/core pret/dma

quick_tests: regression.h verify.pl
	${PRET_ISS}/tests/scripts/verify.pl -T -v ${SCTESTS} pret/sys/cnt pret/sys/bubsort pret/sys/expint pret/sys/float_addi pret/sys/smile pret/sys/binsearch pret/sys/compress pret/sys/crc pret/sys/insertsort pret/sys/edn pret/sys/printf pret/sys/janne  pret/sys/fac pret/sys/ludcmp pret/sys/ndes pret/sys/fibcall pret/sys/fft pret/sys/qurt pret/sys/float_mult pret/sys/statemate pret/sys/synchronization pret/dma


pdma_tests:
	${PRET_ISS}/tests/scripts/verify-pdma.pl -T -v pret/pdma 

clean:
	-$(RM) exports.sh scripts/verify.pl 

distclean:
	-$(RM) ${PRET_ISS}/include/regression.h ${PRET_ISS}/tests/exports.sh Makefile

INCLUDES =-I@top_srcdir@/include


