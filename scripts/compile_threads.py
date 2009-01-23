#!/usr/bin/env python
# Author: Ben Lickly
# Creation date: March 2, 2008
# Last Modified: $Date$ 
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
####################################################################

"""Multiple thread/multiple program compiler

Compiles c files (named thread1.c, thread2.c, ...) and aligns the resulting
srec files (and executables) to specified boundaries.

"""
import os
import sys
import shutil

thread_start   = '0x40000000'
# Original LEON3 thread spacing
thread_spacing = '0x00100000'
# Realistic scratchpad sized spacing.
#thread_spacing = '0x00010000'

quiet = True
         
def make_c_file(thread_num, options):
   os.putenv('CFLAGS', options)
   if quiet:
      system('make -f thread%d.makefile > /dev/null' % thread_num)
   else:
      system('make -f thread%d.makefile' % thread_num)

def system(command, abort_on_fail=True):
   """Runs a system call and checks for errors"""
   # Hack to prevent output that would afffect golden output tests
   if quiet:
      command = " >/dev/null " + command
   if os.system(command) != 0:
      print "ERROR running '%s'" % command
      if abort_on_fail:
         sys.exit(1)

def generate_srec_dump(exe_file, srec_file, dump_file):
   """Generate the srec and dump files from a given source file"""
   assert os.path.exists(exe_file), "Cannot find " + exe_file
   system('sparc-elf-objcopy -O srec %s %s' % (exe_file, srec_file))
   system('sparc-elf-objdump -D %s > %s' % (exe_file, dump_file))

def augment_h_file(h_file, prog_addr):
   generator_file = h_file + '.generator'
   assert os.path.exists(generator_file), "Cannot find " + generator_file
   write_file = open(h_file, "w")
   for in_line in open(generator_file, "rb"):
      if 'RAMSTART' in in_line:
         write_file.write("#define RAMSTART   0x%X\n" % prog_addr)
      elif 'RAMSIZE' in in_line:
         write_file.write("#define RAMSIZE   %s\n" % thread_spacing)
      else:
         write_file.write(in_line)
   write_file.close()
   assert os.path.exists(h_file), "Could not create " + h_file
     
def get_max_address(srec_file):
   """Find the address one past the end of the given SREC file"""
   max_addr = 0
   for line in open(srec_file, "rb"):
      if '1' <= line[1] <= '3':
         addr_len = 2 + 2 * int(line[1])
         current_addr = int(line[4:(4 + addr_len)], 16)
         # We really want one past the max address
         current_addr += (2 * addr_len)
         if current_addr > max_addr:
            max_addr = current_addr
   return max_addr

def start_at(addr):
   """Return the flags needed to compile code starting at the given address"""
   return ' -Ttext=0x%X -Tdata=0x%X ' % (addr, addr + int(thread_spacing,16)/2)

def align(addr):
   """Align address to next possible 32-bit aligned address"""
   return ((addr + 31)/32) * 32

def compile_threads(num_threads):
   """Compile the final simluation-loadable SREC files for each thread"""
   prog_addr = int(thread_start, 16)
   for thread in range(num_threads):
      prog_ops = start_at(prog_addr)
      if not quiet:
         print "Thread %d alignments are: %s" % (thread, prog_ops)
      make_c_file(thread, prog_ops)
      prog_addr = align(prog_addr + int(thread_spacing, 16))
      assert get_max_address('bare_thread%d.srec' % thread) < prog_addr, \
             "Thread %d is too large to fit in ram" % thread
      shutil.copy('bare_thread%d.srec' % thread, 'thread%d.srec' %thread)

def compile_all(working_dir):
   olddir = os.path.abspath(os.curdir)
   os.chdir(working_dir)
   compile_threads(6)
   os.chdir(olddir)

if __name__ == "__main__":
   if len(sys.argv) < 2:
      print "Usage: %s <folder_name> [-v]" % sys.argv[0]
      print ""
      print " + folder_name: ",
      print "name of a folder that holds c files and makefiles"
      print " + -v: ",
      print "if present, '-v' flag forces more verbose output"
   else:
      if "-v" in sys.argv:
         quiet = False
      compile_all(sys.argv[1])
