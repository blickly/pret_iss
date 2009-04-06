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
import sys
import re

def read_ispm_file(ispm_file):
  ifile = open(ispm_file, 'r')
  pattern = re.compile('\D')
  find_address = []
  while 1:
    line = ifile.readline()
    if not line: break
    part = line.split(' ')
    for p in part:
      m = pattern.match(p)
      if m:
        print 'Match found: %s' %p
	find_address.append(p)
      else:
        print 'Match NOT found: %s' %p

  ifile.close()
  
  for line in find_address:
    print line
    read_dump(ispm_file)	



def read_dump(ispm_file):
    part = ispm_file.rstrip('.ispm')
    dump_file = part + '.dump'

    ifile = open(dump_file, 'r')
    while 1:
    	  line = ifile.readline()
	  if not line: break
	  if line.find("<main>:") != -1:
	     part = line.split(' ')
	     print line
	     print part[0]
    ifile.close()


        
if __name__ == '__main__':
    if len(sys.argv) == 2:
       file = sys.argv[1]
       print "%s" %file
       read_ispm_file(file)
    else:
        print "Usage: %s <file-name1> " % sys.argv[0]
        print ""
        print " + file-name: dump file"


