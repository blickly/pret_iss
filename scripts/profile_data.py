#!/usr/bin/env python
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

import cmd
import sys
import os
import pickle
import deadline_block
    
class profile_data:
    """
    Stores the data used during profiling of a simulation.
    """
    pret = ""
    _data = ""
    last_inst = dict()
    
    def __init__(self):
        for idx in range(0,5):
            self.last_inst[idx] = ""
        
    def get_label(self, addr):
        return self._data._break_labels[addr]

    def strip_deadline(self, lbl):
        split_lbl = lbl.split('xx')
        # NOTE: Last label text should be xxDVALxx
        try:
            return int(split_lbl[len(split_lbl) -2])
        except ValueError:
            print 'Error: Labels are incorrent'
            return 0
        
    def is_loop(self, lbl):
        return lbl.find(self._data.loop_str()) != -1

    def is_start(self, lbl):
        return lbl.find(self._data.start_str()) != -1

    def is_end(self, lbl):
        return lbl.find(self._data.end_str()) != -1

    def is_cnt(self, lbl):
        return lbl.find(self._data.cnt_str()) != -1
    
    def set_lbreak(self, lbl, addr):
        self._data.insert_label(lbl, addr)

    def get_stack_variable_spm(self, addr):
        thread_start = 0x40000000
        thread_spacing = 0x001000000
        #        print 'ADDR: %x' %(addr)
        for id in range(1,6):
            #   print '--gt: %x, lt: %x'%(thread_start + (id-1)*thread_spacing, thread_start + id*thread_spacing)
            if hex(addr) >= hex(thread_start + (id-1)*thread_spacing) and hex(addr) < hex(thread_start + id*thread_spacing):
                return id-1
        
        return -1
    
    def reached_lbreak(self, lbl, etime, instcnt, tid):
        str_lbl = lbl
        self.reached_loop_dblock(str_lbl, etime, instcnt, tid)
        self.reached_sequential_dblock(str_lbl, etime, instcnt, tid)
        self.add_pc_dblock(self.cut_label(str_lbl), instcnt, tid)

    def reached_count(self, lbl, etime, instcnt, tid):
        # Select the largest size in instructions for this block as well.
        if not self._data.exists_lbreak(lbl):
            self._data._lbreaks[lbl] = (1, instcnt)
        else:
            (prep, pinst) = self._data._lbreaks[lbl]
            if instcnt - pinst > pinst:
                self._data._lbreaks[lbl] = (prep + 1, instcnt - pinst)
            else:
                self._data._lbreaks[lbl] = (prep + 1, pinst)


    def reached_loop_dblock(self, lbl, etime, instcnt, tid):
        # Start of loop reached again.
        if self.is_start(lbl) and self.is_loop(lbl):
            str_lbl = lbl.replace(self._data.start_str(),"")
            if str_lbl not in self._data._stack:
                self._data._stack.append(str_lbl)
                print 'HDP LOOP: pushed %s, stack: %s' %(str_lbl,self._data._stack)
            if not self._data.exists_lbreak(str_lbl):
                dblock = deadline_block.deadline_block()
                dblock.id = str_lbl
                dblock.block_type = "LOOP"
                dblock.execution_time = [etime]
                dblock.instruction_count = [instcnt]
                dblock.deadline_value = self.strip_deadline(lbl)
                dblock.pcs = dict()
                self._data._lbreaks[str_lbl] = dblock

            else:
                dblock = self._data._lbreaks[str_lbl]
                dblock.execution_time.append(etime)

        # End of loop reached.
        elif self.is_end(lbl) and self.is_loop(lbl):
            str_lbl = lbl.replace(self._data.end_str(),"")
            # Exit end of a loop 
            if lbl.find("_end_end") != -1:
                if self._data.exists_lbreak(str_lbl):
                    dblock = self._data._lbreaks[str_lbl]
                    dblock.execution_time.append(etime)
                    self._data._loops[str_lbl + '_' + str(len(self._data._loops))] = dblock
                    del self._data._lbreaks[str_lbl]

                    if len(self._data._stack) > 0: 
                        lab = self._data._stack.pop()
                        print 'HDP LOOP: popped %s, stack: %s' %(lab,self._data._stack)
            else:
                if self._data.exists_lbreak(str_lbl):
                    dblock = self._data._lbreaks[str_lbl]
                    dblock.execution_time.append(etime)

    def reached_sequential_dblock(self, lbl, etime, instcnt, tid):
        str_lbl = lbl
        if self.is_end(str_lbl) and not self.is_loop(str_lbl):
            str_lbl = str_lbl.replace(self._data.end_str(),"")
            # Exit end of a sequential deadline.            
            
            if lbl.find("_end_end") != -1 and len(self._data._stack) > 0:
                lab = self._data._stack.pop()
                print 'HDP: popped %s, stack: %s' %(lab,self._data._stack)
                if lab == str_lbl:
                    dblock = self._data._lbreaks[str_lbl]
                    dblock.wc_execution_time = etime - dblock.wc_execution_time
                    dblock.instruction_count = instcnt - dblock.instruction_count
                
        elif self.is_start(str_lbl) and not self.is_loop(str_lbl) :
            # Start of a sequential deadline.
            str_lbl = str_lbl.replace(self._data.start_str(),"")
            dblock = deadline_block.deadline_block()
            dblock.init(str_lbl, etime, instcnt, 1, self.strip_deadline(lbl),dict())
            dblock.block_type = "SEQUENCE"

            self._data._lbreaks[str_lbl] = dblock
            self._data._stack.append(str_lbl)

    def reached_stack_variables(self, fetched_thread, dblock, pc):
        read_or_write = ""
        aligned_addr = 0
        #        print 'reached_stack_variables: tid: %s' %(fetched_thread)
        if (not self.pret.is_memory_stalled(fetched_thread)):
            if self.pret.is_memory_read(fetched_thread):
                aligned_addr = self.pret.get_aligned_memory_addr(fetched_thread)
#                print 'Memory read at: pc: %s, addr:%s, cycle:%s' %(hex(pc),hex(aligned_addr),self.pret.get_cycle_count())
                read_or_write = 'read'
            
            if self.pret.is_memory_write(fetched_thread):
                aligned_addr = self.pret.get_aligned_memory_addr(fetched_thread)
#                print 'Memory Write at: pc: %s, addr:%s, cycle:%s' %(hex(pc),hex(aligned_addr), self.pret.get_cycle_count())
                read_or_write = 'write'

            thread_id = self.get_stack_variable_spm(aligned_addr)
            if thread_id != -1 and read_or_write == "read":
#                print 'increment read'
                if  aligned_addr not in dblock.data_read_addresses.keys():
                    dblock.data_read_addresses[aligned_addr] = 1
                else:
                    dblock.data_read_addresses[aligned_addr] = dblock.data_read_addresses[aligned_addr] + 1
            if thread_id != -1 and read_or_write == "write":
#                print 'increment write'
                if aligned_addr not in dblock.data_write_addresses.keys():
                    dblock.data_write_addresses[aligned_addr] = 1
                else:
                    dblock.data_write_addresses[aligned_addr] = dblock.data_write_addresses[aligned_addr] + 1
                
    def add_pc_dblock(self, str, lbl, tid):
        # This is odd. Loops are added into _lbreaks and then removed
        # once completed. Sequential blocks on the other hand are
        # added into _lbreaks and kept there. The _stack is what keeps
        # count of active sequential blocks.

        # Create list of active labels only. This takes labels for the
        # loops and unioning it with the labels for the sequential
        # dblocks in stack.
        npc = self.pret.get_pc(tid)
        if len(self._data._stack) > 0:
            active_db = self._data._stack[len(self._data._stack) - 1]
            # Update the stack variable usage.
            self.reached_stack_variables(tid, self._data._lbreaks[active_db], npc)
            # Check if there are any active deadline blocks.
            #            pcs = self._data._lbreaks[active_db][1]
            pcs = self._data._lbreaks[active_db].pcs
            if npc in pcs:
                pcs[npc] = pcs[npc] + 1
            else:
                pcs[npc] = 1
        else:
            # There are no active deadline blocks to insert the PC
            # in. Therefore, it must be in non-realtime code.
            if npc in self._data._nonrt_instructions:
                self._data._nonrt_instructions[npc] += 1
            else:
                self._data._nonrt_instructions[npc] = 1

        # Globally record all instructions and data
        if npc in self._data._all_instructions.instructions.keys():
            self._data._all_instructions.instructions[npc] += 1
        else:
            self._data._all_instructions.instructions[npc] = 1
            
        read_or_write = ""
        aligned_addr = 0
        if (not self.pret.is_memory_stalled(tid)):
            if self.pret.is_memory_read(tid):
                aligned_addr = self.pret.get_aligned_memory_addr(tid)
                read_or_write = 'read'
            if self.pret.is_memory_write(tid):
                aligned_addr = self.pret.get_aligned_memory_addr(tid)
                read_or_write = 'write'
            thread_id = self.get_stack_variable_spm(aligned_addr)
            if thread_id != -1 and read_or_write == "read":
                #                print 'increment read'
                if  aligned_addr not in self._data._all_data_addresses.read_addresses.keys():
                    self._data._all_data_addresses.read_addresses[aligned_addr] = 1
                else:
                    self._data._all_data_addresses.read_addresses[aligned_addr] += 1
            if thread_id != -1 and read_or_write == "write":
                #                print 'increment write'
                if aligned_addr not in self._data._all_data_addresses.write_addresses.keys():
                    self._data._all_data_addresses.write_addresses[aligned_addr] = 1
                else:
                    self._data._all_data_addresses.write_addresses[aligned_addr] += 1


    def cut_label(self, lbl):
        if self.is_end(lbl):
            return lbl.replace(self._data.end_str(),"")
        elif self.is_start(lbl):
            return lbl.replace(self._data.start_str(),"")
        
    def print_data(self):
        self._data.compute_max_execution_time()
        self._data.print_data()
        

    def set_pret(self,p):
        self.pret = p

    def counting_started(self, lbl):
        return self.find_stack(lbl) != -1

    def is_in_sequential_deadblock(self):
        return (len(self._data._stack) > 0)
                
