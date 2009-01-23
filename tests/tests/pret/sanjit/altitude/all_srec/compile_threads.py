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
srec files (and executables) to be packed together in memory space with 
minimal overlap.

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
         
def compile_prom(thread_num, options, prog_addr, delete_exe=True):
   h_file    = 'prom.h'
   asm_file  = 'prom.S'
   o_file    = 'prom%d.o' % thread_num
   exe_file  = 'prom%d.exe' % thread_num
   srec_file = 'prom%d.srec' % thread_num
   dump_file = 'prom%d.dump' % thread_num
   compile_options = ' -DPRET -c -I. '
   link_options = ' -nostdlib -Tlinkprom -N -L./ %s -nostartfiles ' % options
   augment_h_file(h_file, prog_addr)
   gcc(asm_file, o_file, compile_options)
   gcc(o_file, exe_file, link_options)
   os.unlink(o_file)
   generate_srec_dump(exe_file, srec_file, dump_file)
   if delete_exe: 
      os.unlink(exe_file)

def compile_c_file(thread_num, options, outfile, delete_exe=True):
   c_file = 'thread%d.c' % thread_num
   exe_file = outfile + '.exe'
   srec_file = outfile + '.srec'
   dump_file = outfile + '.dump'
   options = options + ' -DPRET -O0 -lm -msoft-float '
   gcc(c_file, exe_file, options)
   generate_srec_dump(exe_file, srec_file, dump_file)
   if delete_exe: 
      os.unlink(exe_file)
      
def make_c_file(thread_num, options):
   os.putenv('CFLAGS', options)
   system('make -f thread%d.makefile' % thread_num)

def system(command, abort_on_fail=True):
   """Runs a system call and checks for errors"""
   # Hack to prevent output that would afffect golden output tests
   if quiet:
      command = command #+ "> /dev/null"
   if os.system(command) != 0:
      print "ERROR running '%s'" % command
      if abort_on_fail:
         sys.exit(1)

def gcc(in_file, out_file, options):
   """Compile an input file into an output file using the given options"""
   assert os.path.exists(in_file), "Cannot find " + in_file
   system('sparc-elf-gcc -o %s %s %s ' % (out_file, in_file, options))
   assert os.path.exists(out_file), "Could not create: " + out_file

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
     
def merge_srec_files(boot_srec, prog_srec, new_srec):
   """Merge two srec files into one.  
   The starting address is taken from the first srec file"""
   write_file = open(new_srec, "w")
   for line in open(boot_srec, "rb"):
      write_file.write(line)
   for line in open(prog_srec, "rb"):
      if '1' <= line[1] <= '3':
         write_file.write(line)
   write_file.close()

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
   return ' -Ttext=0x%X' % addr

def align(addr):
   """Align address to next possible 32-bit aligned address"""
   return ((addr + 31)/32) * 32

def compile_proms(num_threads, delete_exes=True):
   """Compile the prom SREC files for each thread"""
   boot_addr = 0
   prog_addr = int(thread_start, 16)
   for thread in range(num_threads):
      boot_ops = start_at(boot_addr)
      if not quiet:
         print "PROM %d alignments are: %s" % (thread, boot_ops)
      compile_prom(thread, boot_ops, prog_addr, delete_exes)
      boot_addr = align(get_max_address('prom%d.srec' % thread))
      prog_addr = align(prog_addr + int(thread_spacing, 16))

def compile_threads(num_threads, delete_exes=True):
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
      merge_srec_files('prom%d.srec' % thread, 
                       'bare_thread%d.srec' % thread, 
                       'thread%d.srec' %thread )

def compile_all(working_dir, delete_exes = True):
   olddir = os.path.abspath(os.curdir)
   working_dir = os.path.abspath(working_dir)
   os.chdir(prom_dir())
   compile_proms(6, delete_exes)
   for i in range(6):
      try:
         shutil.copy("prom%s.srec" % i, working_dir)
         shutil.copy("prom%s.dump" % i, working_dir)
      except shutil.Error:
         pass # If files are the same, do nothing
   os.chdir(working_dir)
   compile_threads(6, delete_exes)
   os.chdir(olddir)

def prom_dir():
   """Return directory that contains prom source and SREC files"""
   pdir = os.path.join(os.environ['PRET_ISS'], 'soft')
   return os.path.abspath(pdir)

if __name__ == "__main__":
   if len(sys.argv) < 2:
      print "Usage: %s <folder_name>" % sys.argv[0]
      print ""
      print " + folder_name: ",
      print "name of a folder that holds c files and makefiles"
   else:
      if "-q" in sys.argv:
         quiet = True
      compile_all(sys.argv[1])
