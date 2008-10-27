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

lib_LIBRARIES=libpretpdma.a

libpretpdma_a_SOURCES=./pdma.cpp ../core/l1_scratch.cpp ../core/memory_controller.cpp ../core/wheeled_mem.cpp

#AM_CXXFLAGS=$(AM_CXXFLAGS)

libpretpdma_a_LIBADD=../core/special_reg.o ../core/fetch.o ../core/module_base.o ../core/register_file.o ../core/decode.o ../core/memory_unit.o ../core/mem.o ../core/hw_thread_controller.o ../core/except.o ../core/execute.o ../core/hw_thread_ptr.o ../core/hw_thread.o ../core/instruction.o ../core/regacc.o ../core/cycle_counter.o ../core/core.o  ../core/srec_parser.o ../core/pret_clock.o ../core/dma.o ../core/static_bound_parser.o

INCLUDES =-I@top_srcdir@/include -DUSE_PDMA

