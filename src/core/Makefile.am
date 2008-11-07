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

lib_LIBRARIES=libpretcore.a

if USEFPIC
  AM_CXXFLAGS=-fPIC
else
  AM_CXXFLAGS=
endif

libpretcore_a_SOURCES=./dma.cpp ./special_reg.cpp ./fetch.cpp ./module_base.cpp ./register_file.cpp ./decode.cpp ./memory_unit.cpp ./mem.cpp ./hw_thread_controller.cpp ./except.cpp ./wheeled_mem.cpp ./l1_scratch.cpp ./mem_mapped_io.cpp ./execute.cpp ./memory_controller.cpp ./hw_thread_ptr.cpp ./hw_thread.cpp ./srec_parser.cpp ./instruction.cpp ./regacc.cpp ./cycle_counter.cpp ./core.cpp ./pret_clock.cpp ./static_bound_parser.cpp 

INCLUDES =-I@top_srcdir@/include



