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

# Christopher: Remove tests from this directory list
SUBDIRS=src scripts tests

# Christopher: Remove all the targets below for release.
regression: tests/Makefile
	make -f tests/Makefile regression

quick_tests: tests/Makefile
	make -f tests/Makefile quick_tests 

ship_examples:
	-mkdir examples/

# Copy ALL instructions and data in Janne
	-cp -r tests/tests/pret/dma/janne/janne ./examples/ 

# Copy Smile example
	-cp -r tests/tests/pret/sys/smile/smile ./examples/ 

# Copy Synchronization example
	-cp -r tests/tests/pret/dma/sync_example_cases/sync_example_cases ./examples/

# Copy graphic example
	-cp -r soft/graphic ./examples/

# Remove .svn directories.
	-find ./examples/ -name ".svn" | xargs rm -rf

ship_clean: ship_examples
	-rm -rf tests/ soft/ adm/ autom4te.cache/ build-aux/ logs/ oldfiles/ format_code.sh
	-find . -name ".svn" | xargs rm -rf	 


