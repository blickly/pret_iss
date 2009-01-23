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
# Author: Hiren Patel
#
####################################################################
import os
import pickle
import pprint
import sys

class generate_alloc:
    """Driver to automate the allocation generation."""

    def __init__(self):
        self.deadlines_missed = False
        self.check_feasability = ""
        
    def previous_revision_exists(self):
        print '---------------------------------------------------------------------'
        print 'Rerun alloc: Previous Revision and Feasability State'
        print '---------------------------------------------------------------------'
        try:
            inrev = open('thread0' + '.rev', 'rb')
            self.rev = pickle.load(inrev)
            self.check_feasability= pickle.load(inrev)
            self.deadlines_missed= pickle.load(inrev)
            print '+ Revision number extracted: %d' %self.rev
            print '+ Feasability check state: %s' %self.check_feasability
            print '+ Deadlines missed: %s' %self.deadlines_missed
        except:
            print 'Warning: No previous revisions exist'

if __name__ == '__main__':
    alloc = generate_alloc()
    flag = True

    # Create feasability schedule
    cmd = './runtest.sh'
    os.system(cmd)
    # Check feasability schedule
    cmd = './runtest.sh'
    os.system(cmd)
    alloc.previous_revision_exists()

    if alloc.check_feasability == "PASSED":
        while True:
            cmd = './runtest.sh'
            os.system(cmd)
            alloc.previous_revision_exists()
            if  alloc.deadlines_missed == False:
                break;
    else:
        print 'Allocation feasability not possible'
        
        

