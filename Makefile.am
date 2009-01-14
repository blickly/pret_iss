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
#
####################################################################

SUBDIRS=src scripts tests
SHIP_DIR=../pret_ship

regression: tests/Makefile
	make -f tests/Makefile regression

quick_tests: tests/Makefile
	make -f tests/Makefile quick_tests 

ship:
# Make shipping directory
	mkdir ${SHIP_DIR}
	cp -r . ${SHIP_DIR}
# Start changes
	-mkdir ${SHIP_DIR}/examples
# Copy ALL instructions and data in Janne
	-cp -r tests/tests/pret/dma/janne/janne ${SHIP_DIR}/examples/ 
# Copy Smile example
	-cp -r tests/tests/pret/sys/smile/smile ${SHIP_DIR}/examples/ 
# Copy Synchronization example
	-cp -r tests/tests/pret/dma/sync_example_cases/sync_example_cases ${SHIP_DIR}/examples/
# Copy graphic example
	-cp -r soft/graphic ${SHIP_DIR}/examples/
# Remove .svn and other directories.
	-rm -rf ${SHIP_DIR}/tests/ ${SHIP_DIR}/soft/ ${SHIP_DIR}/adm/ ${SHIP_DIR}/autom4te.cache/ ${SHIP_DIR}/build-aux/ ${SHIP_DIR}/logs/ ${SHIP_DIR}/oldfiles/,${SHIP_DIR}/format_code.sh
	-find ${SHIP_DIR} -name ".svn" | xargs rm -rf	 
	-find ${SHIP_DIR} -name "*~" | xargs rm -f
	-cp Makefile.dist.am ${SHIP_DIR}/Makefile.am
	-cp configure.dist.in ${SHIP_DIR}/configure.in


