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
class code_block:
    """
    Structure to store the information about deadline blocks.
    """

    def __init__(self):
        # Label
        self.id = ""
        # Simulated execution time of deadline block. Largest!
        self.execution_time = 0
        # Number of instructions in this deadline block.
        self.instruction_count = 0
        # Number of times this block was executed.
        self.repetition = 0
        # List of PCs that are a part of this deadline block.
        # {pc: number of times executed}
        self.pcs = dict()
        # Type of code block.
        self.block_type = "CODE"
        # Data read.
        self.data_read_addresses = 0
        self.data_write_addresses = 0
        
