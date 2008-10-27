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
####################################################################
import code_block
from code_block import code_block

class deadline_block(code_block):

    def __init__(self):
        code_block.__init__(self)
        # Deadline value specified by the user.
        self.block_type = "DEAD"
        self.id = 0
        self.execution_time = 0
        self.instruction_count = 0
        self.repetition = 0
        self.deadline_value = 0
        self.wc_execution_time = 0
        self.pcs = 0
        # Data read.
        self.data_read_addresses = dict()
        self.data_write_addresses = dict()

    def init(self, id, extime, instcnt, rep, dval, pcs):
        code_block.__init__(self)
        # Deadline value specified by the user.
        self.block_type = "DEAD"
        self.id = id
        self.execution_time = 0
        self.instruction_count = instcnt
        self.repetition = rep
        self.deadline_value = dval
        self.wc_execution_time = extime
        self.pcs = pcs
        # Data read.
        self.data_read_addresses = dict()
        self.data_write_addresses = dict()

    def is_loop(self):
        return self.block_type == "LOOP"

    def is_sequence(self):
        return self.block_type == "SEQUENCE"


class instructions(code_block):

    def __init__(self):
        code_block.__init__(self)
        self.block_type = "ALLINST"
        self.id = 0
        self.instructions = dict()

class data_addresses(code_block):

    def __init__(self):
        code_block.__init__(self)
        self.block_type = "ALLDATA"
        self.id = 0
        self.read_addresses = dict()
        self.write_addresses = dict()
