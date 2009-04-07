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

def extract_addresses(ispm_file, find_address):
    part = ispm_file.rstrip('.ispm.conf')
    dump_file = part + '.dump'

    ifile = open(dump_file, 'r')
    while 1:
    	  line = ifile.readline()
	  if not line: break
          for key in find_address.keys():
            temp_line = line
            if temp_line.find(key) != -1:
              part = temp_line.split(' ')
              find_address[key] = part[0]
    ifile.close()

def format_function_name(str):
  return '<'+str.rstrip('\n')+'>:'

def read_ispm_file(ispm_file, find_address):
  ifile = open(ispm_file, 'r')
  pattern = re.compile('\D')
  while 1:
    line = ifile.readline().rstrip('\n')
    if not line: break
    part = line.split(' ')

    for p in part:
      m = pattern.match(p)
      if m:
        print 'Match found: %s' %p
	find_address[format_function_name(p)] = 0

  ifile.close()

def rewrite_ispm(ispm_file, find_address):
  backup_file = ispm_file 
  ifile = open(backup_file, 'r')
  ofile = open(ispm_file, 'w')
  pattern = re.compile('\D')
  while 1:
    line = ifile.readline()
    if not line: break
    part = line.split(' ')

    for p in part:
      m = pattern.match(p)
      if m:
	ofile.write(find_address[format_function_name(p)] + ' ')
      else:
        ofile.write(p.rstrip('\n') + ' ')

    ofile.write('\n')
  ifile.close()
  ofile.close()
  
def usage():
    print "Usage: %s <file-name1.ispm.conf> " % sys.argv[0]
    print ""
    print " + file-name.ispm.conf: dump file"

if __name__ == '__main__':
    if len(sys.argv) == 2:
       file = sys.argv[1]
       print "%s" %file
       if not (".ispm.conf" in file):
           usage()
       else:
           # Open file for reading to see if it exists
           if os.path.exists(file):
               function_names = {}
               read_ispm_file(file, function_names)
               if len(function_names.keys())>0:
                   extract_addresses(file, function_names)
                   rewrite_ispm(file, function_names)
           else:
               print "Error: File %s not found" %file
               usage()
    else:
        usage()

