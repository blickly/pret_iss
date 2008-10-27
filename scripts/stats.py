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
# Author: Hiren
####################################################################
import sys
import deadline_block

class stats:
    """
    Structure to store the information about break points.
    """

    # pret simulator object
#    pret = ""
    # Reserved strings in labels
    _loop_str = '_loop'
    _start_str = '_start'
    _end_str = '_end'
    _cnt_str = '_cnt'
    _exit_str = '_exit'


    def __init__(self):
#        print 'default'
        self._break_labels = dict()
        self._lbreaks = dict()
        self._stack = []
        # [label, (delta, rep., stamps) ]
        self._loops = dict()
        # [label, #of times encountered]
        self._nonrt_instructions = dict()
        self._all_instructions = deadline_block.instructions()
        self._all_data_addresses = deadline_block.data_addresses()
        
    def loop_str(self):
        return self._loop_str

    def start_str(self):
        return self._start_str

    def end_str(self):
        return self._end_str

    def cnt_str(self):
        return self._cnt_str

    def exit_str(self):
        return self._exit_str
    
    def exists_lbreak(self, lbl):
        try:
            self._lbreaks[lbl]
            return True
        except:
            return False
        
    def find_stack(self, f):
        """Return first item in sequence where f(item) == True."""
        for item in self._stack:
            if f(item):
                return item

    def insert_label(self, lbl, addr):
        self._break_labels[addr] = lbl

    def compute_max_execution_time(self):
        # Loops are special. They always have two deadline blocks. One
        # for the period and the other for the exit.
        # The exit is labeled with _exit.
        exit_loops = dict()
        for lab in self._loops.keys():
            dblock = self._loops[lab]
            lst = dblock.execution_time
            delta = 0
            instsz = 0
            # Skip last value because it is used for exit block.
            # Get the instruction block size for the block that takes longest ex. time.
            for idx in  range(0,len(lst)-2):
                ctime = lst[idx]
                ntime = lst[idx+1]
                if delta < ntime - ctime: #lst[idx+1] - lst[idx]:
                    delta = ntime - ctime #lst[idx+1] - lst[idx]

            dblock.wc_execution_time = delta
            dblock.repetition = len(lst) -1
            

    def print_computed_loop_data(self):
        print '------------------------------------------------------------------'
        print '+ All Instruction and data (Only first time)'
        print '------------------------------------------------------------------'
        print '+ Instructions: %d'%len(self._all_instructions.instructions)
        print '+ Data: %d'  %(len(self._all_data_addresses.read_addresses) +len(self._all_data_addresses.write_addresses))
        
        print '------------------------------------------------------------------'
        print '+ Deadline loops: Label: (execution time, repetition, deadline value, stamps).'
        print '------------------------------------------------------------------'
        for lab in self._loops:
            dblock = self._loops[lab]

            print '\t %s: (%d, %d, %d, %s)' %(dblock.id,dblock.wc_execution_time, dblock.repetition, dblock.deadline_value, dblock.execution_time)
#             print '+ Stack variables READ in deadline block'
#             pad = 8
#             for k in dblock.data_read_addresses.keys():
#                 if pad == 0:
#                     pad = 8
#                     print ''
                        
#                 sys.stdout.write('%8x:(%d) '%(k,dblock.data_read_addresses[k]))
#                 pad = pad -1
#             print ''
                
#             print '+ Stack variables WRITE in deadline block'
#             pad = 8
#             for k in dblock.data_write_addresses.keys():
#                 if pad == 0:
#                     pad = 8
#                     print ''
                        
#                 sys.stdout.write('%8x:(%d) '%(k,dblock.data_write_addresses[k]))
#                 pad = pad -1
#             print ''

#             print '+ Instructions in deadline block'
#             pad = 8
#             for k in dblock.pcs.keys():
#                 if pad == 0:
#                     pad = 8
#                     print ''
                        
#                 sys.stdout.write('%8x:(%d) '%(k,dblock.pcs[k]))
#                 pad = pad -1
#             print ''

            
    def print_data(self):
        print '----------------- Profile Statistics ------------------\n'
        print '+ Deadline stack: '
        print '------------------------------------------------------------------'
        print self._stack
        print '------------------------------------------------------------------'
        print '+ Collected labels to break on: '
        print '------------------------------------------------------------------'
        for addr in self._break_labels:
            print '\t %s' % self._break_labels[addr]

        print '------------------------------------------------------------------'
        print '+ Sequential deadline blocks: '
        print '------------------------------------------------------------------'
        for lab in self._lbreaks:
            if lab.find(self.loop_str()) == -1 and lab.find(self.cnt_str()) == -1:
                dblock = self._lbreaks[lab]

                print '\t %s: (%d, %d, %d, %d)' %(dblock.id,dblock.wc_execution_time, dblock.instruction_count, dblock.repetition, dblock.deadline_value)
#                 print '+ Stack variables READ in deadline block'
#                 pad = 8
#                 for k in dblock.data_read_addresses.keys():
#                     if pad == 0:
#                         pad = 8
#                         print ''
                        
#                     sys.stdout.write('%8x:(%d) '%(k,dblock.data_read_addresses[k]))
#                     pad = pad -1
#                 print ''
                
#                 print '+ Stack variables WRITE in deadline block'
#                 pad = 8
#                 for k in dblock.data_write_addresses.keys():
#                     if pad == 0:
#                         pad = 8
#                         print ''
                        
#                     sys.stdout.write('%8x:(%d) '%(k,dblock.data_write_addresses[k]))
#                     pad = pad -1
#                 print ''

#                 print '+ Instructions in deadline block'
#                 pad = 8
#                 for k in dblock.pcs.keys():
#                     if pad == 0:
#                         pad = 8
#                         print ''
                        
#                     sys.stdout.write('%8x:(%d) '%(k,dblock.pcs[k]))
#                     pad = pad -1
#                 print ''
                    
        self.print_computed_loop_data()

#         print '------------------------------------------------------------------'
#         print '+ Non-realtime instructions: (pc, #repetition)'
#         print '------------------------------------------------------------------'
        
#         pad = 8
#         #        for pc_tuple in self.sort_dictionary_by_value(self._nonrt_instructions):
#         for pc_tuple in self.sort_dictionary_by_value(self._all_instructions.instructions):
#             if pad == 0:
#                 print ''
#                 pad = 8

#             sys.stdout.write('(%8x: %d) '%(pc_tuple[0],pc_tuple[1]))
#             pad = pad - 1

#         print ''

#         print '------------------------------------------------------------------'
#         print '+ Non-realtime READ data addresses: (data address, #repetition)'
#         print '------------------------------------------------------------------'
        
#         pad = 8
#         #        for pc_tuple in self.sort_dictionary_by_value(self._nonrt_instructions):
#         for pc_tuple in self.sort_dictionary_by_value(self._all_data_addresses.read_addresses):
#             if pad == 0:
#                 print ''
#                 pad = 8

#             sys.stdout.write('(%8x: %d) '%(pc_tuple[0],pc_tuple[1]))
#             pad = pad - 1

#         print ''
        
#         print '------------------------------------------------------------------'
#         print '+ Non-realtime WRITE data addresses: (data address, #repetition)'
#         print '------------------------------------------------------------------'
        
#         pad = 8
#         #        for pc_tuple in self.sort_dictionary_by_value(self._nonrt_instructions):
#         for pc_tuple in self.sort_dictionary_by_value(self._all_data_addresses.write_addresses):
#             if pad == 0:
#                 print ''
#                 pad = 8

#             sys.stdout.write('(%8x: %d) '%(pc_tuple[0],pc_tuple[1]))
#             pad = pad - 1

#         print ''

    def sort_dictionary_by_value(self, sdict):
        from operator import itemgetter
        return sorted(sdict.iteritems(), key=itemgetter(1), reverse=True)
