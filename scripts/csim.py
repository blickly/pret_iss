#!/usr/bin/env python
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
####################################################################


import cmd
import sys
import os
import dump_reader
import profile_data
import pickle
import stats


base_dir = os.environ['PRET_ISS']
#csim_dir = os.path.dirname(os.path.abspath(__file__))
#base_dir = os.path.dirname(csim_dir)

debugger_dir = os.path.join(base_dir, 'src/debugger')
sys.path.append(debugger_dir)
import libpretdbg
pret = libpretdbg


class CalSim(cmd.Cmd):
   """
   csim: A processor simulator for a precision timed processor.
   (c) 2008 PRET group at UC Berkeley

   csim is modeled after the existing SPARC processor simulator tsim,
   from Gaisler Research.
   """
   breakpoints = []
   threads = [0]
   soft_dir = ""
   skip_fetch_stall = False

   def __init__(self, filename, st):
      #      os.chdir(base_dir)

      cmd.Cmd.__init__(self)
      self.set_soft_dir(filename)
      self.load(filename)
      self.prompt = "csim> "
      self.dreader = dump_reader.DumpReader()
      self.dreader.parse_dumps(self.soft_dir)
      self.profdata = profile_data.profile_data()
      self.profdata._data = st
      # Give access to pret to profdata
      self.profdata.set_pret(pret)

   # Helper functions   
   def set_soft_dir(self, filename):
      if os.path.isdir(filename):
         self.soft_dir = filename
      else:
         for line in open(filename, "rb"):
            self.soft_dir = line.rstrip('\n')

   def load(self, filename):
      """Load the text file containing the names of the SREC files to run."""
      assert os.path.exists(filename), "Unable to find file " + filename
      pret.load(filename)

   def one_step(self, trace = False, stop_break = False, profile = False):
      """Run one cycle of the simulation, and tell if a breakpoint was hit.
         one_step returns True when the execution was normal and
         False when the program end of a breakpoint has been hit"""
      simend = pret.run(1)
      
      fetched_thread = pret.get_cycle_count() % 6
      pc = pret.get_pc(fetched_thread)
      # If we are not skipping stalling in the fetch stage. 
      if not self.skip_fetch_stall:
         if trace and fetched_thread in self.threads:
            print "t%d\t%8x\t" % (fetched_thread, pc),
            print self.dreader.get_line(pc)
            
         if fetched_thread in self.threads and pc in self.breakpoints:
            if stop_break:
               print "t%d\t%8x\t" % (fetched_thread, pc),
               print "Hit breakpoint at address 0x%08x" % pc
               return (True,simend)
            else:
               return (False,simend)
         else:
            return (False,simend)
      else:
         if not pret.is_fetch_stalled(fetched_thread):
            if trace and fetched_thread in self.threads:
               print "t%d\t%8x\t" % (fetched_thread, pc),
               print self.dreader.get_line(pc)

            if fetched_thread in self.threads and profile :
               lab = ""
               if pc in self.breakpoints:
                  lab = self.profdata.get_label(pc)
                  #                  print 'label from pc: %s' %lab
                  
#                if (not pret.is_memory_stalled(fetched_thread)):
#                   if pret.is_memory_read(fetched_thread):
#                      aligned_addr = pret.get_aligned_memory_addr(fetched_thread)
#                      print 'Memory read at: pc: %s, addr:%s, cycle:%s' %(hex(pc),hex(aligned_addr),pret.get_cycle_count())
#                      self.profdata.reached_stack_variable(aligned_addr, 'read')
#                   if pret.is_memory_write(fetched_thread):
#                      aligned_addr = pret.get_aligned_memory_addr(fetched_thread)
                     
#                      print 'Memory Write at: pc: %s, addr:%s, cycle:%s' %(hex(pc),hex(aligned_addr), pret.get_cycle_count())
#                      self.profdata.reached_stack_variable(aligned_addr, 'write')

                  

               # Run every cycle for the thread being watched.
               self.profdata.reached_lbreak(lab, pret.get_cycle_count(), pret.get_instruction_count(fetched_thread), fetched_thread);

            if fetched_thread in self.threads and pc in self.breakpoints:
               if stop_break:
                  print "t%d\t%8x\t" % (fetched_thread, pc),
                  print "Hit breakpoint at address 0x%08x" % pc

               if stop_break:
                  return (True,simend)
               else:
                  return (False,simend)
            else:
               return (False,simend)
      if simend == 0:
         return (True,simend)
      else:
         return (False,simend)

   def print_breakpoints(self):
      """Print out all of the currently active breakpoints in any thread."""
      for point in self.breakpoints:
         point_num = self.breakpoints.index(point)
         print "breakpoint %d: %#08x" % (point_num, point)


   # Simulator Commands
   def do_cycle(self, args):
      """Print out the current clock cycle, which is incremented every tick.
      
      Usage: cycle"""
      print "Cycle: %d" % pret.get_cycle_count()


   def do_break(self, args):
      """Set a breakpoint at the given address or print out breakpoints.
      
      Usage: break [hex addr]  or  break"""
      if args == "":
         self.print_breakpoints()
         return
      else:
         try:
            addr = int(args, 16)
         except ValueError:
            print "unable to set breakpoint ('%s' not a valid address)" % args
            return
         if addr not in self.breakpoints:
            self.breakpoints.append(addr)
         print "breakpoint set at address 0x%08x" % addr

   def do_lbreak(self, args):
      """Set a breakpoint at the given label in the given thread.
      
      Usage: lbreak [thread_num] [label_name]"""
      try:
         thread_no = int(args[:1])
         label = args[1:].lstrip(' ')
      except:
         return
      addr = self.dreader.label_address(thread_no, label)
      if addr != None and addr not in self.breakpoints:
        self.breakpoints.append(addr)
        self.profdata.set_lbreak(label, addr);
        print 'breakpoint set at label "%s" in thread %d (address 0x%08x)' \
                % (label, thread_no, addr)

   def do_unbreak(self, args):
      """Remove the nth breakpoint
      
      Usage: unbreak [n]"""
      try:
         num = int(args)
      except ValueError:
         return
      if num < len(self.breakpoints):
         addr = self.breakpoints[num]
         self.breakpoints.remove(addr)
         print "unset breakpoint at address 0x%08x" % addr

   def do_skipfetch(self, args):
      """Skip breaking at PC when a fetch stall occurs.

      Usage: skipfetch """

      self.skip_fetch_stall = not self.skip_fetch_stall
      print "Breaking at instructions stalled in fetch stage: %s" %self.skip_fetch_stall
   
   
   def do_thread(self, args):
      """Set a thread to be watched or print out watched threads
      
      Usage: thread [thread num]  or  thread"""
      try:
         thread = int(args)
      except ValueError:
         print "Watching threads: " + (", ".join(map(str,self.threads)))
         return
      if thread not in self.threads:
         self.threads.append(thread)
         self.threads.sort()
      print "Now watching thread %d" % thread

   def do_unthread(self, args):
      """Stop watching a thread
      
      Usage: unthread [thread num]"""
      try:
         thread = int(args)
      except ValueError:
         return
      if thread in self.threads:
         self.threads.remove(thread)
      print "No longer watching thread %d" % thread

   def do_instruction_count(self, args):
      """Return the instruction count for a thread.
      
      Usage: instruction_count [thread num]"""
      try:
         thread = int(args)
      except ValueError:
         return
      if thread >= 0 and thread < 6:
         print 'Instruction count for thread %d: %d' %(thread,pret.get_instruction_count(thread))
      else:
         print 'Error: thread number range is from 0 to 5.'


   def do_dt(self, args):
      """Print the deadline timers of all watched threads or a given thread

      Usage: dt  or  dt [thread num]"""
      try:
         thread = int(args)
         pret.timer(thread)
      except ValueError:
         for i in self.threads:
            print "Timers for thread %d: " % i
            pret.timer(i)

   def do_quit(self, line):
      """Quit the simulation"""
      print ""

      return 1

   do_EOF = do_quit

   def do_mem(self, args):
      """Show the contents of memory
      
      Usage: mem [hex_address] [num_bytes]"""
      try:
         args = args.split()
         if len(args) == 0:
            return
         elif len(args) == 1:
            bytes = 16
         else:
            bytes = int(args[1])
         start_addr = int(args[0], 16)
      except ValueError:
         return
      end_addr = start_addr + bytes
      start_addr = (start_addr / 4) * 4
      for addr in range(start_addr, end_addr, 4):
         data = pret.get_memory_data(addr)
         print "%8x: %08x" % (addr, data) 

   def do_reg(self, args):
      """Print register contents for all watched threads or given thread

      Usage: reg  or  reg [thread num]"""
      try:
         thread = int(args)
         pret.reg(thread)
      except ValueError:
         for i in self.threads:
            print "Registers for thread %d: " % i
            pret.reg(i)

   def do_reset(self, args):
      """Reset the simulation"""
      os.execl(os.path.join(csim_dir, __file__))

   def do_tra(self, args):
      """Trace through the program for the given number of cycles (defualt is 1)
      
      Usage: tra [num_cycles]"""
      try:
         n = int(args)
      except ValueError:
         n = 1
      for i in range(n):
         (bp, simend) = self.one_step(trace = True, stop_break = True)
         if simend == 0:
            print 'End of simulation reached.'
         elif bp: # (if a breakpoint was hit)
            break

   def do_profile(self, args):
      """Run the profiler until simulation ends.

      Usage: profile"""

      simend = 1
      while simend != 0:
         (bp, simend) = self.one_step(trace = False, stop_break = False, profile = True)

      print 'Total Sim. Time: %d'%(pret.get_cycle_count())

   def do_stats(self, args):
      """Get statistics from the profile.

      Usage: stats filename"""
      self.profdata.print_data()
      try:
         filename = args
         print 'Pickling output to file: %s' %filename
         output = open(filename, 'wb')
         pickle.dump(self.profdata._data, output)
         output.close();
      except:
         print 'Some exception in filename'
         


      
   def do_run(self, args):
      """Run the program until it halts, or for a specified number of cycles
      
      Usage: run  or  run [num_cycles]"""
      try:
         n = int(args)
      except ValueError:
         n = 10000000
      for i in range(n):
         simend = 1
         (bp, simend) = self.one_step(trace = False, stop_break = True)
         if  simend == 0 or bp == True: # (if a breakpoint was hit)
            break

#          if self.one_step(trace = False): # (if a breakpoint was hit)
#             break


      
   def do_help(self, args):
      """Prints out information on available commands

      Usage: help  or  help [command]"""
      if args == "":
         print self.__doc__
      cmd.Cmd.do_help(self, args)


# When run directly, create a Csim instance.
if __name__ == '__main__':

   st = stats.stats()

   if len(sys.argv) == 2:
      csim = CalSim(sys.argv[1], st)
      csim.cmdloop()
   else:
      print "Usage: %s <file-name>" % sys.argv[0]
      print ""
      print " + file-name: text file containing paths of SREC files to run"
    

#    print ' Read pickled data'
#    fi = open('thread0-t1.pkl', 'rb')
#    ti = ""
#    while True:
#       try:
#          ti = pickle.load(fi)
#       except EOFError:
#          break
#       else:
#          print ti._break_labels
#          print ti._lbreaks
#          print ti._loops
         
#    fi.close()

