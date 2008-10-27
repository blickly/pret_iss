#!/usr/bin/python
# Author: Ben Lickly
# Creation date: March 5, 2008
# Last Modified: $Date$ 
"""Dump file reader

Simply prints out a given address from its dump file

Usage:
dump_reader.py <hex_address>
"""
import sys;
import os;

def get_line(dump_file, addr):
   lines = []
   for line in open(dump_file, "rb"):
      if ':' in line:
         addr_end = line.index(':')
         addr_string = line[:addr_end]
         try:
            #print "parsed %d " % addr
            if addr == int(addr_string, 16):
               lines.append(line)
               return lines     # We really only care about the first match
         except:
            #print "coldn't parse " + addr_string 
            pass
   return lines

def get_file_list(addr):
   script_dir = os.path.dirname(os.path.abspath(__file__))
   dump_dir = script_dir
   prom_list = [os.path.join(dump_dir, "prom%d.dump" % i) for i in range(6)]
   thread_list = [os.path.join(dump_dir, "bare_thread%d.dump" % i) 
                                                          for i in range(6)]
   if addr < 40000000:
      return prom_list
   else:
      return thread_list

def search_dumps(addr):
   for file in get_file_list(addr):
      for line in get_line(file, addr):
         return ' '.join(line.split()[5:])

if __name__ == "__main__":
   if len(sys.argv) != 2:
      print __doc__
   else:
      search_addr = int(sys.argv[1], 16)
      print search_dumps(search_addr)

