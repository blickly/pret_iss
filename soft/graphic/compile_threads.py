#!/usr/bin/python
# Author: Ben Lickly
# Creation date: March 2, 2008
# Last Modified: $Date$ 
"""Multiple thread/multiple program compiler

Compiles c files (named thread1.c, thread2.c, ...) and aligns the resulting
srec files (and executables) to be packed together in memory space with 
minimal overlap.

"""
import os;
import sys;

thread_start   = '0x40000000'
thread_spacing = '0x00100000'
         
def compile_prom(thread_num, options, prog_addr, delete_exe=True):
   h_file    = 'prom.h'
   asm_file  = 'prom.S'
   o_file    = 'prom%d.o' % thread_num
   exe_file  = 'prom%d.exe' % thread_num
   srec_file = 'prom%d.srec' % thread_num
   dump_file = 'prom%d.dump' % thread_num
   compile_options = ' -c -I. '
   link_options = ' -nostdlib -Tlinkprom -N -L./ %s -nostartfiles ' % options
   augment_h_file(h_file, prog_addr)
   gcc(asm_file, o_file, compile_options)
   gcc(o_file, exe_file, link_options)
   os.unlink(o_file)
   generate_srec_dump(exe_file, srec_file, dump_file)
   if delete_exe: 
      os.unlink(exe_file)

def compile_thread(thread_num, options, outfile, delete_exe=True):
   c_file = 'thread%d.c' % thread_num
   exe_file = outfile + '.exe'
   srec_file = outfile + '.srec'
   dump_file = outfile + '.dump'
   options = options + ' -g -O2 -lm -msoft-float '
   gcc(c_file, exe_file, options)
   generate_srec_dump(exe_file, srec_file, dump_file)
   if delete_exe: 
      os.unlink(exe_file)

def system(command, abort_on_fail=True):
   """Runs a system call and checks for errors"""
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

def compile_all_curdir(num_threads, delete_exes=True):
   """Compile the final simluation-loadable SREC files for each thread"""
   boot_addr = 0
   prog_addr = int(thread_start, 16)
   for thread in range(0, num_threads):
      boot_ops = start_at(boot_addr)
      print "PROM %d alignments are: %s" % (thread, boot_ops)
      compile_prom(thread, boot_ops, prog_addr, delete_exes)
      boot_addr = align(get_max_address('prom%d.srec' % thread))
      prog_ops = start_at(prog_addr)
      print "Thread %d alignments are: %s" % (thread, prog_ops)
      compile_thread(thread, prog_ops, 'bare_thread%d' % thread, delete_exes)
#      prog_addr = align(get_max_address('thread%d.srec' % thread))
      prog_addr = align(prog_addr + int(thread_spacing, 16))
      assert get_max_address('bare_thread%d.srec' % thread) < prog_addr, \
             "Thread %d is too large to fit in ram" % thread
      merge_srec_files('prom%d.srec' % thread, 
                       'bare_thread%d.srec' % thread, 
                       'thread%d.srec' %thread )

def compile_all(delete_exes = True):
   olddir = os.path.abspath(os.curdir)
   script_dir = os.path.dirname(os.path.abspath(__file__))
   os.chdir(script_dir)
   compile_all_curdir(6, delete_exes)
   os.chdir(olddir)

if __name__ == "__main__":
   if '-x' in sys.argv:
      compile_all(delete_exes = False)
   else:
      compile_all()
