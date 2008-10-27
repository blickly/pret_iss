#!/usr/bin/env python
# Author: Ben Lickly
# Creation date: March 5, 2008
# Last Modified: $Date$ 
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

"""Dump file reader

Prints out the given address from its dump file.
All dump files must be located in the given folder.

Usage:
dump_reader.py <folder> <hex_address>
"""
import sys;
import os;

class DumpReader:

  def __init__(self):
    self.prom   = []
    self.thread = []
    self.labels = []

  def parse_dumps(self, dump_dir):
    prom_list = [os.path.join(dump_dir, "prom%d.dump" % i) for i in range(6)]
    thread_list = [os.path.join(dump_dir, "bare_thread%d.dump" % i) 
                                                          for i in range(6)]
    for pfile, tfile in zip(prom_list, thread_list):
       (plines, plabels) = self.parse_dump(pfile)
       (tlines, tlabels) = self.parse_dump(tfile)
       self.prom.append(plines)
       self.thread.append(tlines)
       self.labels.append(tlabels)
       

  def parse_dump(self, dump_file):
    lines = {}
    labels = {}
    for line in open(dump_file, "rb"):
       if ':' in line:
         colon_pos = line.index(':')
         try:
            addr_string = line[:colon_pos]
            addr = int(addr_string, 16)
            lines[addr] = ' '.join(line.split()[5:])
         except:
            try:
               if line[colon_pos-1:colon_pos] == '>':
                  lt_pos = line.index('<')
                  gt_pos = colon_pos - 1
                  if line[lt_pos-1:lt_pos] == ' ':
                     addr_string = line[:lt_pos-1]
                     addr = int(addr_string, 16)
                     label = line[lt_pos+1:gt_pos]
                     labels[label] = addr
            except:
               pass
    return (lines, labels)

  def get_line(self, addr):
    #if addr < 40000000:
    #   dicts = self.prom
    #else:
    #   dicts = self.thread
    for dict in self.thread + self.prom:
       try:
          return dict[addr]
       except:
          pass

  def label_address(self, thread_no, label):
    try:
       return self.labels[thread_no][label]
    except KeyError:
       return None

if __name__ == "__main__":
   if len(sys.argv) != 3:
      print __doc__
   else:
      search_addr = int(sys.argv[2], 16)
      dreader = DumpReader()
      dreader.parse_dumps(sys.argv[1])
      print dreader.get_line(search_addr)

