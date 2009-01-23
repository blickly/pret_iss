####################################################################
#Copyright (c) 2007-2009 The Regents of the University of California.
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

noinst_HEADERS= \
	@top_srcdir@/include/core.h \
	@top_srcdir@/include/cycle_counter.h \
	@top_srcdir@/include/decode.h \
	@top_srcdir@/include/defines.h \
	@top_srcdir@/include/dma.h \
	@top_srcdir@/include/dma_if.h \
	@top_srcdir@/include/except.h \
	@top_srcdir@/include/execute.h \
	@top_srcdir@/include/fetch.h \
	@top_srcdir@/include/hw_thread.h \
	@top_srcdir@/include/hw_thread_controller.h \
	@top_srcdir@/include/hw_thread_ptr.h \
	@top_srcdir@/include/instruction.h \
	@top_srcdir@/include/instruction_defs.h \
	@top_srcdir@/include/l1_scratch.h \
	@top_srcdir@/include/mem.h \
	@top_srcdir@/include/memory_controller.h \
	@top_srcdir@/include/memory_unit.h \
	@top_srcdir@/include/mem_location.h \
	@top_srcdir@/include/mem_mapped_io.h \
	@top_srcdir@/include/module_base.h \
	@top_srcdir@/include/no_systemc.h \
	@top_srcdir@/include/pret_clock.h \
	@top_srcdir@/include/regacc.h \
	@top_srcdir@/include/register_file.h \
	@top_srcdir@/include/special_reg.h \
	@top_srcdir@/include/srec_parser.h \
	@top_srcdir@/include/static_bound_parser.h \
	@top_srcdir@/include/wheeled_mem.h

libpretcore_a_SOURCES= \
	./dma.cpp \
	./special_reg.cpp \
	./fetch.cpp \
	./module_base.cpp \
	./register_file.cpp \
	./decode.cpp \
	./memory_unit.cpp \
	./mem.cpp \
	./hw_thread_controller.cpp\
	./except.cpp \
	./wheeled_mem.cpp \
	./l1_scratch.cpp \
	./mem_mapped_io.cpp \
	./execute.cpp \
	./memory_controller.cpp \
	./hw_thread_ptr.cpp \
	./hw_thread.cpp \
	./srec_parser.cpp \
	./instruction.cpp \
	./regacc.cpp \
	./cycle_counter.cpp \
	./core.cpp \
	./pret_clock.cpp \
	./static_bound_parser.cpp 

INCLUDES =-I@top_srcdir@/include



