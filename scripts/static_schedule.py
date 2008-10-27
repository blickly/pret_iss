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
# Author: Hiren Patel
#
####################################################################
import os
import pickle
import pprint
import sys
import stats
import time
import deadline_block

class data_set:
    """Collection of all data gathered."""

    def __init__(self):
        self.prev_common_inst = []
        self.tests = dict()
#        self.missed = dict()
        # Joined missed deadline blocks instructions only.
        self.join_missed = dict()
        self.all_instructions = deadline_block.instructions()
        self.all_data_addresses = deadline_block.data_addresses()
        
        # Joined missed deadline blocks data only.
        self.prev_scheduled_data = []
        self.join_read_data_missed = dict()
        self.join_write_data_missed = dict()
        # Output file name
        self.ofname = "dude"
        # Revision number
        self.rev = 0
        # State to see whether feasability check must be done (PENDING, VERIFY, PASSED|FAILED)
        self.check_feasability = "PENDING"
        # If revision exists
        self.rev_exists = False
        # Scratchpad Size
        self.spm_size = 1430 #2587
        self.dspm_size = 396 #515
        # Allocation per iteration.
        self.spm_alloc = 50
        self.dspm_alloc = 50
        # All deadlines met.
        self.deadlines_missed = False

    def adddict(self, dict1, dict2):
            return dict(dict1, **dict2)
        
    def any_deadlines_missed(self):
        self.deadlines_missed = len(self.join_missed) > 0
        return  len(self.join_missed) > 0
    
    def check_file_format(self, files):
        fsplit = dict()
        # Strip out the last value if it is an integer. If not, then no value for SPM size specified.
        last_input = files.pop()
        try:
            self.spm_size = int(last_input)
            if self.spm_size == -1:
                print '+ SPM size is infinite for FEASABILITY check'
                self.spm_size = 0
            else:
                print '+ SPM size defined as %d * 32bits'%self.spm_size
        except:
            files.append(last_input)
            
        for f in files:
            fsplit[f] = f.split('-')
            if not ( fsplit[f][0].find('thread') or fsplit[f][1].find('pkl')):
                return False;
            self.ofname = fsplit[f][0]

            try:
                fin = open(f, 'rb')
            except:
                return False
        return True
        
    def common_instructions_between_dblocks(self, most_used):
        common_inst = dict()
        common_intersect = set()
        for db in most_used.keys():
            common_s = set()
            for fname in most_used[db].keys():
                if len(common_s) == 0:
                    common_s = set(most_used[db][fname])
                    print 'first'
                    print common_s
                else:
                    common_s.update(most_used[db][fname])

            print 'common_set: %s' %common_intersect
            if db in common_inst:
                tset = common_inst[db]
                tset.update([common_s])
            else:
                common_inst[db] = common_s
        return common_inst


    def generate_ispm_file(self, merged_inst):
        # Check if the size of SPM will handle the allocation.
        alloc_size = len(merged_inst)
        print '+ Allocation size: %d, ispm size: %d' %(len(merged_inst),self.spm_size)
        if (self.spm_size > 0):
            if (alloc_size <= self.spm_size):
                print '+ Generating static memory allocation: %s ' %self.ofname.strip() + '.ispm'
                of = open(self.ofname + '.ispm', 'w')

                for pc in merged_inst:
                    of.write('%8x %8x\n'%(pc,pc))
                of.close()
            else:
                print 'Error: Computed allocation will not fit in SPM'
                print 'Error: + SPM size: %d, Allocation size: %d (32-bit words)' %(self.spm_size,alloc_size)
        else:
            print '+ Generating static memory allocation: %s ' %self.ofname.strip() + '.ispm'
            of = open(self.ofname + '.ispm', 'w')

            for pc in merged_inst:
                of.write('%8x %8x\n'%(pc,pc))

            of.close()
            
    def generate_dspm_file(self,merged_data):
        # Check if the size of SPM will handle the allocation.
        alloc_size = len(merged_data)
        print 'alloc size: %d, dspm size: %d' %(len(merged_data),self.dspm_size)
        if (self.dspm_size > 0):
            if (alloc_size <= self.dspm_size):
                print '+ Generating static memory allocation for data: %s ' %self.ofname + '.dspm'
                of = open(self.ofname + '.dspm', 'w')

                for pc in merged_data:
                    of.write('%8x %8x\n'%(pc,pc))
                of.close()
            else:
                print 'Error: Computed allocation will not fit in SPM'
                print 'Error: + SPM size: %d, Allocation size: %d (32-bit words)' %(self.dspm_size,alloc_size)
        else:
            print '+ Generating static memory allocation for data: %s ' %self.ofname + '.dspm'
            of = open(self.ofname + '.dspm', 'w')

            #            print self.prev_common_inst
            for pc in merged_data:
                of.write('%8x %8x\n'%(pc,pc))

            of.close()

    def generate_feasability_ispm_file(self):
        print '---------------------------------------------------------------------'
        print 'Generate Feasability ISPM file'
        print '---------------------------------------------------------------------'

        of = open(self.ofname + '.ispm', 'w')
        all_missed_ins = []
        for db in self.join_missed.keys():
            print '+ Deadline block: %s, instruction count: %d' %(self.join_missed[db].id, len(self.join_missed[db].pcs))
            dblock = self.join_missed[db]
            #((extime, inscnt, rep,dval), pcs) = self.join_missed[db]
            for inst in dblock.pcs.keys():
                all_missed_ins.append(inst)

        print '+ Feasability file: %s ' %(self.ofname + '.ispm')
        of = open(self.ofname + '.ispm','w')
        for inst in all_missed_ins:
            of.write('%8x %8x\n'%(inst,inst))
        of.close()

        # Data SPM
        print '+ Feasability DSPM file: %s ' %(self.ofname + '.dspm')
        of = open(self.ofname + '.dspm', 'w')
        for db in self.join_read_data_missed.keys():
            dblock = self.join_read_data_missed[db]
            print 'HDP: %s' %dblock
            for addr in dblock.data_read_addresses.keys():
                of.write('%8x %8x\n'%(addr,addr))
        for db in self.join_write_data_missed.keys():
            dblock = self.join_write_data_missed[db]
            for addr in dblock.data_write_addresses.keys():
                of.write('%8x %8x\n'%(addr,addr))
        of.close()
        
    def get_most_used_instructions(self):
        most_used = dict(dict())
        for db in self.missed.keys():
            if len(self.missed[db]) > 0:
                # Multiple tests failed on this deadline block.
                for fname in self.missed[db].keys():
                    ((extime, ninst, rep, dval), pcs) = self.missed[db][fname]
                    # Take instructions most frequently used.
                    if len(pcs) > 0:
                        (toppc, topcnt) = pcs[0]
                        inst_cands = set()
                        #                    print 'topcnt = %d' %topcnt
                        for idx in range(0,len(pcs)):
                            (pc,cnt) = pcs[idx]
                            if cnt == topcnt:
                                inst_cands.add(pc)
                            
                        if db in most_used:
                            tdict = most_used[db]
                            tdict[fname] = inst_cands
                        else:
                            most_used[db] = {fname:inst_cands}
        return most_used

    def join_stack_variables_missed_deadlines(self, db, src_dblock):
        if src_dblock.wc_execution_time > 6 * src_dblock.deadline_value:
            if db in self.join_read_data_missed.keys():
                missed_dblock = self.join_read_data_missed[db]
                # READ
                joined_stack_variables = missed_dblock.data_read_addresses
                for addr in src_dblock.data_read_addresses.keys():
                    if addr in joined_stack_variables.keys():
                        if joined_stack_variables[addr] < src_dblock.data_read_addresses[addr]:
                            joined_stack_variables[addr] = src_dblock.data_read_addresses[addr]
            else:
                self.join_read_data_missed[db] = src_dblock

            if db in self.join_write_data_missed.keys():
                missed_dblock = self.join_write_data_missed[db]
                # WRITE
                joined_stack_variables = missed_dblock.data_write_addresses
                for addr in src_dblock.data_write_addresses.keys():
                    if addr in joined_stack_variables.keys():
                        if joined_stack_variables[addr] < src_dblock.data_write_addresses[addr]:
                            joined_stack_variables[addr] = src_dblock.data_write_addresses[addr]

            else:
                self.join_write_data_missed[db] = src_dblock

    def merge_max(self, dest, src):
        for k in src.keys():
            if k in dest.keys():
                if dest[k] < src[k]:
                    dest[k] = src[k]
            else:
                dest[k] = src[k]
                
    def join_all_instructions_data(self):
        for fname in self.tests:
            t = self.tests[fname]
            self.merge_max(self.all_instructions.instructions, t._all_instructions.instructions)
            self.merge_max(self.all_data_addresses.read_addresses, t._all_data_addresses.read_addresses)
            self.merge_max(self.all_data_addresses.write_addresses, t._all_data_addresses.write_addresses)
            
    def join_missed_deadlines(self):
        for fname in self.tests:
            t = self.tests[fname]
            
            # Unfortunately this only handled the sequential blocks :(
            for db in t._lbreaks.keys():
                dblock = t._lbreaks[db]
                print 'HDP: test: %s, extime: %s, dval: %d, #pcs: %d' %(fname,dblock.wc_execution_time,int(6*dblock.deadline_value),len(dblock.pcs))
                # Join Stack variables. 
                self.join_stack_variables_missed_deadlines(db, dblock)
                
                if dblock.wc_execution_time > 6 * dblock.deadline_value:
                    print '>> Missed Deadline: %s:%s:%d:%d' %(db,fname,dblock.wc_execution_time,dblock.deadline_value)
                    # Missed deadline
                    if db in self.join_missed.keys():
                        # Two tests with the same deadline block being missed. So join the two on a certain criteria.
                        missed_dblock = self.join_missed[db]
                        joined_pcs = missed_dblock.pcs
                        for pc in dblock.pcs.keys():
                            if pc in joined_pcs.keys():
                                if joined_pcs[pc] < dblock.pcs[pc]:
                                    print 'test %s has db %s with extime (%s,%s) and (%s,%s,%s)' %(fname,db,extime,nextime,pc,joined_pcs[pc],pcs[pc])
                                    joined_pcs[pc] = dblock.pcs[pc]

                        if missed_dblock.wc_execution_time > dblock.wc_execution_time:
                            dblock.wc_execution_time = missed_dblock.wc_execution_time

                    else:
                        self.join_missed[db] = dblock
                        
            # TODO: Add for loops
            for db in t._loops.keys():
                dblock = t._loops[db]
                # Join Stack variables. 
                self.join_stack_variables_missed_deadlines(db, dblock)

                if dblock.wc_execution_time > 6 * dblock.deadline_value:
                    print '>> Missed Deadline: %s:%s:%d:%d' %(db,fname,dblock.wc_execution_time,dblock.deadline_value)
                    # Missed deadline
                    if db in self.join_missed.keys():
                        # Two tests with the same deadline block being missed. So join the two on a certain criteria.
                        missed_dblock = self.join_missed[db]
                        joined_pcs = missed_dblock.pcs
                        # Store the worst of the execution times.
                        for pc in dblock.pcs.keys():
                            if pc in joined_pcs.keys():
                                if joined_pcs[pc] < dblock.pcs[pc]:
                                    joined_pcs[pc] = dblock.pcs[pc]

                        if missed_dblock.wc_execution_time > dblock.wc_execution_time:
                            dblock.wc_execution_time = missed_dblock.wc_execution_time
                    else:
                        self.join_missed[db] = dblock


    def load_files(self, files):
        for f in files:
            fi = open(f, 'rb')
            while True:
                try:
                    self.tests[f] = pickle.load(fi)
                except EOFError:
                    break
            fi.close()

    def partial_schedule(self,filename):
        common_inst = []
        selected_data = []
#         mainmem_inst = []
#         mainmem_data = []
        if self.check_feasability == "PASSED":
            self.join_select_allocation_ampl()
            self.stack_allocation_ampl()
            #            self.mainmem_allocation_ampl()
            common_inst = self.join_run_lp_solve()
            selected_data = self.data_run_lp_solve()
            #            (mainmem_inst, mainmem_data) = self.mainmem_run_lp_solve()

        print '---------------------------------------------------------------------'
        print 'Generate ISPM file'
        print '---------------------------------------------------------------------'

        # If feasability check is not performed then do it.
        print 'Check feasability state: %s' %self.check_feasability
        if self.check_feasability == "PENDING":
            self.generate_feasability_ispm_file()
            self.check_feasability = "VERIFY"
        else:
            # NOTE: Specific file format required. (thread*-t#.pkl)
#            print 'OMG'
#            print 'PREV:%s'%self.prev_common_inst
#            print 'NEW:%s'%common_inst
            
            self.prev_common_inst.extend(common_inst)
            self.generate_ispm_file(self.prev_common_inst)
            
            self.prev_scheduled_data.extend(selected_data)
            self.generate_dspm_file(self.prev_scheduled_data)

        print '---------------------------------------------------------------------'
        print 'Deadlines missed: %s' %self.any_deadlines_missed()
        print '---------------------------------------------------------------------'

        print '---------------------------------------------------------------------'
        print 'Pickle state elements'
        print '---------------------------------------------------------------------'
        self.pickle_scheduled_inst(common_inst,selected_data)


    def pickle_scheduled_inst(self,common_inst, selected_data):
        self.rev = self.rev + 1
        try:
            frevnum = self.ofname + '.rev'
            filename = self.ofname + '-r' + str(self.rev) + '.pkl'
            print '+ Pickling output to file: %s' %filename
            print '+ VERSION is: %d' %(self.rev)
            print '+ Saving version number %d to %s' %(self.rev,frevnum)
            outrev = open(frevnum, 'wb')
            output = open(filename, 'wb')
#            print '+ Scheduled instructions so far: %s' %common_inst
#            print '+ Merged instructions: %s' %self.prev_common_inst
#            print '+ Scheduled data so far: %s' %selected_data
#            print '+ Merged data addresses: %s' %self.prev_scheduled_data
            
            pickle.dump(self.prev_common_inst,output)
            pickle.dump(self.prev_scheduled_data,output)
            pickle.dump(self.rev, outrev)
            pickle.dump(self.check_feasability, outrev)
            pickle.dump(self.deadlines_missed, outrev)

            output.close();
            outrev.close();
        except:
            print "Unexpected error:", sys.exc_info()[0]

    def previous_revision_exists(self):
        print '---------------------------------------------------------------------'
        print 'Previous Revision and Feasability State'
        print '---------------------------------------------------------------------'
        try:
            inrev = open(self.ofname + '.rev', 'rb')
            self.rev = pickle.load(inrev)
            self.check_feasability= pickle.load(inrev)
            self.deadlines_missed = pickle.load(inrev)
            print '+ Revision number extracted: %d' %self.rev
            print '+ Feasability check state: %s' %self.check_feasability
            self.rev_exists = True
        except:
            print 'Warning: No previous revisions exist'

    def print_pcs(self):
        for fname in self.tests:
            t = self.tests[fname]
            print '\n+ file: %s' %fname
            for db in t._lbreaks.keys():
                print '\nblock: %s' %db
                ((extime,ninst,dval),pcs) = t._lbreaks[db]
                pad = 8
                for pc in pcs:
                    if pad == 0:
                        print ''
                        pad = 8
                        
                    sys.stdout.write('%8x ' %pc)
                    pad = pad -1

    def print_missed_pcs(self):
        for db in self.missed.keys():
            print '\n+ block: %s' %db
            if len(self.missed[db]) > 0:
                # Multiple tests failed on this deadline block.
                for fname in self.missed[db].keys():
                    print '\nfilename: %s' %fname
                    ((extime, ninst, dval), pcs) = self.missed[db][fname]
                    pad = 8
                    for pc in pcs:
                        if pad == 0:
                            print ''
                            pad = 8
                        
                        sys.stdout.write('%8x ' %pc[0])
                        pad = pad -1

    def sort_dictionary_by_value(self, sdict):
        from operator import itemgetter
        return sorted(sdict.items(), key=itemgetter(1), reverse =True)
                    
    def remove_scheduled_inst(self):
        ofname = self.ofname + '-r' + str(self.rev) + '.pkl'
        print '+ Opening previous revision file: %s' %ofname
        fi = open(ofname, 'rb')
        while True:
            try:
                self.prev_common_inst = pickle.load(fi)
                self.prev_scheduled_data = pickle.load(fi)
#                print 'LOADED instructions %s' %self.prev_common_inst
            except EOFError:
                break
        fi.close()

        # Remove the instructions from the missed deadline instructions.
        for pc in self.prev_common_inst:
#            print '+ Check if pc is missed: %d' %pc
            for db in self.join_missed.keys():
                dblock = self.join_missed[db]
                rmpcs = []
                for inst in dblock.pcs.keys():
                    if pc == inst:
                        del dblock.pcs[pc]
                        
            if pc in self.all_instructions.instructions.keys():
                del self.all_instructions.instructions[pc]
                
        # Remove the data addresses from the missed deadline instructions.
        for pc in self.prev_scheduled_data:
            for db in self.join_read_data_missed.keys():
                dblock = self.join_read_data_missed[db]
                rmpcs = []
                for inst in dblock.data_read_addresses.keys():
                    if pc == inst:
                        del dblock.data_read_addresses[pc]
            for db in self.join_write_data_missed.keys():
                dblock = self.join_write_data_missed[db]
                rmpcs = []
                for inst in dblock.data_write_addresses.keys():
                    if pc == inst:
                        del dblock.data_write_addresses[pc]
                        
            if pc in self.all_data_addresses.read_addresses.keys():
                del self.all_data_addresses.read_addresses[pc]
            if pc in self.all_data_addresses.write_addresses.keys():
                del self.all_data_addresses.write_addresses[pc]

    def verify_feasability(self):
        if self.check_feasability == "VERIFY":
            print 'VERIFY'
            if len(self.join_missed) > 0:
                print 'Error: No allocation scheme will enable meeting the following deadline specifications'
                for db in self.join_missed.keys():
                    print '+ Deadline Block: %s' %(self.join_missed[db].id)
                self.check_feasability = "FAILED"
            else:
                print 'Hm: Supposed to verify but no deadlines missed'
                self.check_feasability = "PASSED"
    def run_mainmem(self):
        self.previous_revision_exists()
        self.load_files(files)
        self.join_all_instructions_data()
        self.join_missed_deadlines()
        self.verify_feasability()
        print 'No deadlines are being missed: Perform freq. main memory optimization'
        self.mainmem_allocation_ampl()
        (mainmem_inst, mainmem_data) = self.mainmem_run_lp_solve()
        self.prev_common_inst.extend(mainmem_inst)
        self.generate_ispm_file(self.prev_common_inst)
        
        self.prev_scheduled_data.extend(mainmem_data)
        self.generate_dspm_file(self.prev_scheduled_data)
        self.pickle_scheduled_inst(mainmem_inst,mainmem_data)
        print '---------------------------------------------------------------------'
        print 'MAINMEM ALLOC'
        print '---------------------------------------------------------------------'
        print '+ instructions'
        print mainmem_inst
        print '+ data'
        print mainmem_data
        
    def allocate_ispm(self, files):
        #        print 'files: %s' %files
        if not self.check_file_format(files):
            print 'Error: Input file names are wrong or do not exit'
        else:
            #self.run_mainmem()
            #return
            self.previous_revision_exists()
            self.load_files(files)
            self.join_all_instructions_data()
            self.join_missed_deadlines()
            print 'verify feasability'
            self.verify_feasability()
            if self.check_feasability == "PASSED" or  (self.check_feasability != "FAILED" and self.any_deadlines_missed() == True):
                if self.rev_exists:
                    self.remove_scheduled_inst()
                self.partial_schedule(files)
            else:
                if self.check_feasability == "PASSED" and self.any_deadlines_missed() == False:
                    print 'No deadlines are being missed: Perform freq. main memory optimization'
                    self.mainmem_allocation_ampl()
                    (mainmem_inst, mainmem_data) = self.mainmem_run_lp_solve()
                    self.prev_common_inst.extend(mainmem_inst)
                    self.generate_ispm_file(self.prev_common_inst)
            
                    self.prev_scheduled_data.extend(mainmem_data)
                    self.generate_dspm_file(self.prev_scheduled_data)
                    self.pickle_scheduled_inst(mainmem_inst,mainmem_data)
                    print '---------------------------------------------------------------------'
                    print 'MAINMEM ALLOC'
                    print '---------------------------------------------------------------------'
                    print '+ instructions'
                    print mainmem_inst
                    print '+ data'
                    print mainmem_data

                else:
#                     if self.check_feasability == "PENDING":
#                         self.partial_schedule(files)
                    if self.check_feasability == "FAILED":
                        print 'Error: Feasability check failed'
                        return
                    
                    

    def join_select_allocation_ampl(self):
        of = open(self.ofname + '.dat', 'w')
        # Header. Same for all allocations.
        of.write('data;\n')

        if self.spm_alloc > self.spm_size - len(self.prev_common_inst):
            # Problem. Readjust size of spm.
            of.write('param ALLOCINST:=' + str(self.spm_size - len(self.prev_common_inst)) + ';\n')
        else:
            of.write('param ALLOCINST:=' + str(self.spm_alloc) + ';\n')
            
        # Set of deadline blocks.
        of.write('set DB:=')

        for db in self.join_missed.keys():
            of.write(db + ' ')

        of.write(';\n')

        # Set of instructions.
        of.write('set INST:=')
        pc_dict = dict()
        pc_cnt = 0
        for db in self.join_missed.keys():
            dblock = self.join_missed[db]

            for pc in dblock.pcs.keys():
                if pc not in pc_dict.keys():
                    pc_dict[pc] = pc_cnt
                    pc_cnt = pc_cnt + 1

        for pc in pc_dict.keys():
            of.write(str(pc)+' ')
        of.write(';\n')

        # Repetition count for deadline blocks.
        of.write('param dbrepcnt:=\n')
        for db in self.join_missed.keys():
            dblock = self.join_missed[db]

            of.write(db + ' ' + str(dblock.repetition) + '\n')
        of.write(';\n')

        # Missed instructions.
        of.write('param db_missed: ')
        for pc in pc_dict.keys():
            of.write(str(pc) + ' ')
        of.write(':=\n')
        for db in self.join_missed.keys():
            of.write(db + ' ')
            dblock = self.join_missed[db]

            for pc in pc_dict.keys():
                if pc in dblock.pcs.keys():
                    of.write(str(dblock.pcs[pc]) + ' ')
                else:
                    of.write('0 ')
            of.write('\n')
        of.write(';\n')
            
        of.close()

    def mainmem_allocation_ampl(self):
        of = open(self.ofname + '.mainmem.dat', 'w')
        # Header. Same for all allocations.
        of.write('data;\n')
        of.write('param DSPMSIZE:=' + str(self.dspm_size - len(self.prev_scheduled_data)) + ';\n')
        of.write('param ISPMSIZE:=' + str(self.spm_size - len(self.prev_common_inst)) + ';\n')        
        # Compute remaining SPM sizes.
        # if self.dspm_alloc > self.dspm_size - len(self.prev_scheduled_data):
#             # Problem. Readjust size of spm.
#             of.write('param DSPMSIZE:=' + str(self.dspm_size - len(self.prev_scheduled_data)) + ';\n')
#         else:
#             of.write('param DSPMSIZE:=' + str(self.dspm_alloc) + ';\n')
            
#         if self.spm_alloc > self.spm_size - len(self.prev_common_inst):
#             # Problem. Readjust size of spm.
#             of.write('param ISPMSIZE:=' + str(self.spm_size - len(self.prev_common_inst)) + ';\n')
#         else:
#             of.write('param ISPMSIZE:=' + str(self.spm_alloc) + ';\n')

        # Set of instructions.
        of.write('set INST:=')
        for pc in self.all_instructions.instructions.keys():
            of.write(str((pc)) + ' ')
        of.write(';\n')

        # Set of READ data addresses
        of.write('set READDATA:=')
        for addr in self.all_data_addresses.read_addresses.keys():
            of.write(str(addr) + ' ')
        of.write(';\n')

        # Set of READ data addresses
        of.write('set WRITEDATA:=')
        for addr in self.all_data_addresses.write_addresses.keys():
            of.write(str(addr) + ' ')
        of.write(';\n')

        of.write('param N:=')
        for pc in self.all_instructions.instructions.keys():
            of.write(str((pc)) + ' ' + str(self.all_instructions.instructions[pc]) + '\n')
        of.write(';\n')

        of.write('param N_r:=')
        for addr in self.all_data_addresses.read_addresses.keys():
            of.write(str(addr) + ' ' + str(self.all_data_addresses.read_addresses[addr]) + '\n')

        for addr in self.all_data_addresses.write_addresses.keys():
            if addr not in self.all_data_addresses.read_addresses.keys():
                of.write(str(addr) + ' 0\n' )
        of.write(';\n')

        of.write('param N_w:=')
        for addr in self.all_data_addresses.write_addresses.keys():
            of.write(str(addr) + ' ' + str(self.all_data_addresses.write_addresses[addr]) + '\n')

        for addr in self.all_data_addresses.read_addresses.keys():
            if addr not in self.all_data_addresses.write_addresses.keys():
                of.write(str(addr) + ' 0\n')
        of.write(';\n')
        
        
    def stack_allocation_ampl(self):
        of = open(self.ofname + '.dspm.dat', 'w')
        # Header. Same for all allocations.
        of.write('data;\n')

        if self.dspm_alloc > self.dspm_size - len(self.prev_scheduled_data):
            # Problem. Readjust size of spm.
            of.write('param MAXSIZE:=' + str(self.dspm_size - len(self.prev_scheduled_data)) + ';\n')
        else:
            of.write('param MAXSIZE:=' + str(self.dspm_alloc) + ';\n')

        # Set of deadline blocks.
        of.write('set DB:=')
        for db in self.join_missed.keys():
            of.write(db + ' ')

        of.write(';\n')

        # Set of data addresse

        read_dict = dict()
        read_cnt = 0
        # Read stack variables
        for db in self.join_read_data_missed.keys():
            dblock = self.join_read_data_missed[db]

            for pc in dblock.data_read_addresses.keys():
                if pc not in read_dict.keys():
                    read_dict[pc] = read_cnt
                    read_cnt = read_cnt + 1



        # Set of data addresse

        write_dict = dict()
        write_cnt = 0
        read_cnt = 0
        # Read stack variables
        for db in self.join_write_data_missed.keys():
            dblock = self.join_write_data_missed[db]

            for pc in dblock.data_write_addresses.keys():
                if pc not in write_dict.keys():
                    write_dict[pc] = write_cnt
                    write_cnt = write_cnt + 1

        # Write out the PCs for READDATA
        of.write('set READDATA:=')
        for pc in read_dict.keys():
            of.write(str(pc)+' ')
        of.write(';\n')

        # Write out the PCs for WRITEDATA
        of.write('set WRITEDATA:=')
        for pc in write_dict.keys():
            of.write(str(pc)+' ')
        of.write(';\n')

        # Repetition count for deadline blocks.
        of.write('param dbrepcnt:=\n')
        for db in self.join_missed.keys():
            dblock = self.join_missed[db]
            of.write(db + ' ' + str(dblock.repetition) + '\n')
        of.write(';\n')

        # Missed data
        of.write('param N_r: ')
        for pc in read_dict.keys():
            of.write(str(pc) + ' ')
        for pc in write_dict.keys():
            if pc not in read_dict.keys():
                of.write(str(pc) + ' ')
            
        of.write(':=\n')
        for db in self.join_read_data_missed.keys():
            of.write(db + ' ')
            dblock = self.join_read_data_missed[db]

            for pc in read_dict.keys():
                if pc in dblock.data_read_addresses.keys():
                    of.write(str(dblock.data_read_addresses[pc]) + ' ')
                else:
                    of.write('0 ')
                    
            for pc in write_dict.keys():
                if pc not in read_dict.keys():
                    of.write('0  ')
            of.write('\n')
        of.write(';\n')

        # Missed data
        of.write('param N_w: ')
        for pc in write_dict.keys():
            of.write(str(pc) + ' ')
        # Write the READ data as 0 if they are NOT in write_dict
        for pc in read_dict.keys():
            if pc not in write_dict.keys():
                of.write(str(pc) + ' ')
        of.write(':=\n')
        for db in self.join_write_data_missed.keys():
            of.write(db + ' ')
            dblock = self.join_write_data_missed[db]

            for pc in write_dict.keys():
                if pc in dblock.data_write_addresses.keys():
                    of.write(str(dblock.data_write_addresses[pc]) + ' ')
                else:
                    of.write('0 ')
                    
            for pc in read_dict.keys():
                if pc not in write_dict.keys():
                    of.write('0 ')
            of.write('\n')
        of.write(';\n')
            
        of.close()

    def join_run_lp_solve(self):
        cmd = 'lp_solve -v -rxli xli_MathProg scripts/alloc.lp -rxlidata ' + self.ofname + '.dat > ' + self.ofname + '.alloc'
        print '+ Execute: %s' %cmd
        os.system(cmd)
        #time.sleep(2)
        inf = open(self.ofname + '.alloc', 'r')
        alloc_inst = []
        # HACK. Partition in python would be great. Version 2.5 needed thoguh :( 
        for line in inf:
            if line.find('alloc_spm') != -1:
                part = line.split(' ')
                if int(part[len(part)-1]) == 1:
                    inst = part[0].split(',')[1]
                    # HACK. Need to remove the ] from the instruction. Very weak.
                    alloc_inst.append(int(inst.rstrip(']')))

        print '+ Allocation of instructions: '
        print alloc_inst
        return alloc_inst
                
    def data_run_lp_solve(self):
        cmd = 'lp_solve -v -rxli xli_MathProg scripts/dspm_alloc.lp -rxlidata ' + self.ofname + '.dspm.dat > ' + self.ofname + '.dspm.alloc'
        print '+ Execute: %s' %cmd
        os.system(cmd)
        #time.sleep(2)
        inf = open(self.ofname + '.dspm.alloc', 'r')
        alloc_inst = []
        # HACK. Partition in python would be great. Version 2.5 needed thoguh :( 
        for line in inf:
            if line.find('alloc_spm') != -1:
                part = line.split(' ')
                if int(part[len(part)-1]) == 1:
                    inst = part[0].split(',')[1]
                    # HACK. Need to remove the ] from the instruction. Very weak.
                    alloc_inst.append(int(inst.rstrip(']')))

        print '+ Allocation of data addresses: '
        print alloc_inst
        return alloc_inst

    def mainmem_run_lp_solve(self):
        cmd = 'lp_solve -v -rxli xli_MathProg scripts/mainmem_alloc.lp -rxlidata ' + self.ofname + '.mainmem.dat > ' + self.ofname + '.mainmem.alloc'
        print '+ Execute: %s' %cmd
        os.system(cmd)
        #time.sleep(2)
        inf = open(self.ofname + '.mainmem.alloc', 'r')
        alloc_inst = []
        alloc_data = []
        # HACK. Partition in python would be great. Version 2.5 needed thoguh :( 
        for line in inf:
            # data
            if line.find('alloc_spm') != -1:
                part = line.split(' ')
                if int(part[len(part)-1]) == 1:
                    inst = part[0].strip('alloc_spm[')
                    # HACK. Need to remove the ] from the instruction. Very weak.
                    alloc_data.append(int(inst.rstrip(']')))

            if line.find('alloc_ispm') != -1:
                part = line.split(' ')
                if int(part[len(part)-1]) == 1:
                    inst = part[0].split('alloc_ispm[')[1]
                    # HACK. Need to remove the ] from the instruction. Very weak.
                    alloc_inst.append(int(inst.rstrip(']')))
                    
        print '+ MainMem Allocation of instructions: '
        print alloc_inst
        print '+ MainMem Allocation of data addresses: '
        print alloc_data

        return (alloc_inst, alloc_data)
        
if __name__ == '__main__':
    ds = data_set()
    if len(sys.argv) > 1:
        files = []
        for num_files in range(1,len(sys.argv)):
            files.append(sys.argv[num_files])
        
        ds.allocate_ispm(files)
    else:
        print "Usage: %s <file-name1> <file-name2> ..." % sys.argv[0]
        print ""
        print " + file-name: pickled file"


