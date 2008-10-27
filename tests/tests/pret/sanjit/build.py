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


import cmd
import sys
import os, fnmatch, string, re
import shutil
import array
import time

base_dir = os.environ['PRET_ISS']
test_dir = os.path.join(base_dir, 'tests/tests/pret/sanjit')
bddir = test_dir + '/bddir'

arm_sim = 'sima'
h8_sim = 'h8300-elf-run'
leon3_sim = 'tsim-leon3'
pret_sim = base_dir + '/src/pret'
#archs = ['pret','arm','leon3','h8']
archs = ['pret','leon3']

fnames = []
armexf = []
h8exf = []
leon3exf = []

def unique(s):
    """Return a list of the elements in s, but without duplicates.

    For example, unique([1,2,3,1,2,3]) is some permutation of [1,2,3],
    unique("abcabc") some permutation of ["a", "b", "c"], and
    unique(([1, 2], [2, 3], [1, 2])) some permutation of
    [[2, 3], [1, 2]].

    For best speed, all sequence elements should be hashable.  Then
    unique() will usually work in linear time.

    If not possible, the sequence elements should enjoy a total
    ordering, and if list(s).sort() doesn't raise TypeError it's
    assumed that they do enjoy a total ordering.  Then unique() will
    usually work in O(N*log2(N)) time.

    If that's not possible either, the sequence elements must support
    equality-testing.  Then unique() will usually work in quadratic
    time.
    """

    n = len(s)
    if n == 0:
        return []

    # Try using a dict first, as that's the fastest and will usually
    # work.  If it doesn't work, it will usually fail quickly, so it
    # usually doesn't cost much to *try* it.  It requires that all the
    # sequence elements be hashable, and support equality comparison.
    u = {}
    try:
        for x in s:
            u[x] = 1
    except TypeError:
        del u  # move on to the next method
    else:
        return u.keys()

    # We can't hash all the elements.  Second fastest is to sort,
    # which brings the equal elements together; then duplicates are
    # easy to weed out in a single pass.
    # NOTE:  Python's list.sort() was designed to be efficient in the
    # presence of many duplicate elements.  This isn't true of all
    # sort functions in all languages or libraries, so this approach
    # is more effective in Python than it may be elsewhere.
    try:
        t = list(s)
        t.sort()
    except TypeError:
        del t  # move on to the next method
    else:
        assert n > 0
        last = t[0]
        lasti = i = 1
        while i < n:
            if t[i] != last:
                t[lasti] = last = t[i]
                lasti += 1
            i += 1
        return t[:lasti]

    # Brute force is all that's left.
    u = []
    for x in s:
        if x not in u:
            u.append(x)
    return u

def run_pret(outf):
    print 'Executing pret on %s\n' % outf
    outf_del = outf + '.del'
    os.system(pret_sim + ' dir.txt -1 >' + outf_del)

def run_full_pret(outf):
    print 'Executing pret on %s\n' % outf
    outf_del = outf + '.del'
    cmd = base_dir + '/scripts/csim.py dir.txt <'
    if outf.find('altitude') != -1:
        os.system(cmd + 'pret.altitude.cmd >' + outf_del)
    if outf.find('cctask') != -1:
        os.system(cmd + 'pret.cctask.cmd >' + outf_del)
    if outf.find('stabilisation') != -1:
        os.system(cmd + 'pret.stabilisation.cmd >' + outf_del)

    #os.system(pret_sim + ' dir.txt -1 >' + outf_del)
    
def run_leon3(outf):
    print 'Executing tsim-leon3 on %s\n' % outf
    outf_del = outf + '.del'
    os.system(leon3_sim + ' -c leon3.cmd ' + outf + ' >' + outf_del)

def run_full_leon3(outf):
    findstr = ['altitude_control_task', 'climb_control_task','stabilisation_task']
    
    print 'Executing only FULL tsim-leon3 on %s\n' % outf
    outf_del = outf + '.del'
    if outf.find('altitude') != -1:
        os.system(leon3_sim + ' -c leon3.altitude.cmd ' + outf + ' >' + outf_del)
    if outf.find('cctask') != -1:
        os.system(leon3_sim + ' -c leon3.cctask.cmd ' + outf + ' >' + outf_del)
    if outf.find('stabilisation') != -1:
        os.system(leon3_sim + ' -c leon3.stabilisation.cmd ' + outf + ' >' + outf_del)

def run_h8(outf):
    print 'Executing gdb-h8 on %s\n' % outf
    outf_del = outf + '.del'
    os.system(h8_sim + ' -v ' + outf + ' >' + outf_del)

def run_arm(outf):
    print 'Executing SimIt-arm on %s\n' % outf
    outf_del = outf + '.del'
    print(arm_sim + ' -v ' + outf + ' 2>' + outf_del)
    os.system(arm_sim + ' -v ' + outf + ' 2>' + outf_del)
    time.sleep(0.2)
    #os.system('sh runall.sh ' + outf)

def collect_data(dir,fname,str,splitstr):
    arrdata = []
    initdata = []
    fulldata = []
    files = os.listdir(dir)
    fo = open(dir + '/' + fname, 'w');
    for fl in files:
        if not os.path.isdir(fl) and fnmatch.fnmatch(fl,'*.exe.del'):
            open_file = dir + '/' + fl
            # Read file and only retrieve cycle names
            fi = open(open_file, 'r')
            for line in fi:
                if line.find(str) != -1:
                    lsplit = line.split(splitstr)
                    #fo.write(fl.rstrip('led.exe.') + '\t' + lsplit[1])
                    arrdata.append([fl.rstrip('led.exe.'), lsplit[1]])
                    #print fl
                    if fl.find('_init_') != -1:
                        newname = (fl.rstrip('led.exe.'))
                        newname = newname.replace('_init','')
                        initdata.append([newname, lsplit[1]])
                    if fl.find('_full_') != -1:
                        newname = (fl.rstrip('led.exe.'))
                        newname = newname.replace('_full','')
                        fulldata.append([newname, lsplit[1]])
                    
            fi.close()
    arrdata.sort()
    initdata.sort()
    fulldata.sort()
    # Find all the unique test names
    uniq = []
    for k,v in fulldata:
        uniq.append(k.rsplit('_')[0])
    uniq = unique(uniq)

    outdat = []
    for t in uniq:
        ft = open(dir + '/' + fname + '.' + t, 'w');
        for k,v in fulldata:
            if k.find(t) != -1:
                for ki,vi in initdata:
                    if ki==k: 
                        outdat.append([k, v, vi, int(v) - int(vi)])
                        ft.write(k.rstrip('\n') + '\t' + v.rstrip('\n') + '\t' + vi.rstrip('\n')
                                 + '\t %d\n' % int(int(v) -int(vi)))
                        break
        ft.close()
#    print outdat

    for a,b,c,d in outdat:
        fo.write(a.rstrip('\n') + '\t' + b.rstrip('\n') + '\t' + c.rstrip('\n') + '\t %d\n' % d)
#     for k,v in arrdata:
#         fo.write(k + '\t'+ v)
            
    fo.close()

def collect_full_data(dir,fname,str,splitstr):
    startdata = []
    enddata = []
    files = os.listdir(dir)
    fo = open(dir + '/' + fname, 'w');
    for fl in files:
        if not os.path.isdir(fl) and fnmatch.fnmatch(fl,'*.exe.del'):
            open_file = dir + '/' + fl
            # Read file and only retrieve cycle names
            fi = open(open_file, 'r')
            first = 0
            for line in fi:
                if line.find(str) != -1:
                    lsplit = line.split(splitstr)

                    if fl.find('_full_') != -1:
                        newname = (fl.rstrip('led.exe.'))
                        newname = newname.replace('_full','')
                        if first == 0:
                            startdata.append([newname, lsplit[1]])
                            first = 1;
                        else:
                            enddata.append([newname,lsplit[1]])
            fi.close()

    startdata.sort()
    enddata.sort()
    # Find all the unique test names
    uniq = []
    for k,v in enddata:
        uniq.append(k.rsplit('_')[0])
    uniq = unique(uniq)

    outdat = []
    for t in uniq:
        ft = open(dir + '/' + fname + '.' + t, 'w');
        for k,v in enddata:
            if k.find(t) != -1:
                for ki,vi in startdata:
                    if ki==k: 
                        outdat.append([k, v, vi, int(v) - int(vi)])
                        ft.write(k.rstrip('\n') + '\t' + v.rstrip('\n') + '\t' + vi.rstrip('\n')
                                 + '\t %d\n' % int(int(v) -int(vi)))
                        break
        ft.close()
#    print outdat
    for a,b,c,d in outdat:
        fo.write(a.rstrip('\n') + '\t' + b.rstrip('\n') + '\t' + c.rstrip('\n') + '\t %d\n' % d)
            
    fo.close()

def post_process_data(dir,fname,splitstr):
    #   arrdata  = []
    arrdata = []
    files = os.listdir(dir)
    fo = open(dir + '/' + fname, 'w')
    for fl in files:
        if not os.path.isdir(fl) and fnmatch.fnmatch(fl,'*.dump'):
            open_file = dir + '/' + fl
            fi = open(open_file, 'r')
            for line in fi:
                lsplit = line.split(splitstr)
                arrdata.append(lsplit)
                
            fi.close()
    fo.close()

    arrdata.sort()
    print arrdata
    for k,v in arrdata:
        print 'key: %s, value: %s' % (k, v)
        


def build():
    folders = os.listdir(test_dir)

    
    if not os.path.isdir(bddir):
        os.mkdir(bddir)

    for arcdir in archs:
        adir = bddir + '/' + arcdir
        if not os.path.isdir(adir):
            os.mkdir(adir)
            os.mkdir(adir + '/' + 'all_srec');

    for a in archs:
        print 'Architecture: %s' % a
        for f in folders:
            if os.path.isdir(f):
                print 'Folder: %s' % f
                fnames = os.listdir(test_dir + '/' + f)
                for fn in fnames:
                    if fnmatch.fnmatch(fn, '*.c'):
                        print '+ %s' %fn
                        adir = bddir + '/' + a
                        outf = adir + '/' + fn.rstrip('c.') + '.exe'
                        srcf = test_dir + '/' + f + '/' + fn
                        if a=='arm':
                            armexf.append(outf)
                            print 'arm-softfloat-linux-gnu-gcc -msoft-float -O0 -o %s %s -static' % (outf,srcf)
                            os.system( 'arm-softfloat-linux-gnu-gcc -msoft-float -O0 -o %s %s -static' % (outf,srcf) )
                            time.sleep(0.2)
                            # Run arm simulator on generated file
                            run_arm(outf)
                     
                        if a=='leon3':
                            leon3exf.append(outf)
                            print 'sparc-elf-gcc -msoft-float -O0 -o %s %s' % (outf,srcf)
                            os.system( 'sparc-elf-gcc -msoft-float -O0 -o %s %s' % (outf,srcf) )
                            time.sleep(0.2)
                            #run_leon3(outf)
                            run_full_leon3(outf)
                   
                        if a=='h8':
                            h8exf.append(outf)
                            print 'h8300-elf-gcc -O0 -o  %s %s' % (outf,srcf)
                            os.system( 'h8300-elf-gcc -O0 -o %s %s' % (outf,srcf) )
                            time.sleep(0.2)
                            run_h8(outf)

                        if a=='pret':
                            destdir = adir + '/all_srec'
                            print('cp %s %s' % (test_dir + '/' +f + '/all_srec/*', destdir))
                            #shutil.copytree(test_dir +'/'+f+'/all_srec',destdir)
                            os.system('cp %s %s' % (test_dir + '/' +f + '/all_srec/*', destdir))
                            time.sleep(0.2)
                            print('cp %s %s' % (srcf,destdir + '/thread0.c'))
                            #os.system('cp %s %s' % (srcf,destdir + '/thread0.c'))
                            #time.sleep(0.5)
                            shutil.copyfile(srcf, destdir + '/thread0.c')
                            #os.system('make -f ' + destdir + '/thread_common.makefile clean')
                            os.system(destdir + '/compile_threads.py ' + destdir)
                            os.system('echo %s > dir.txt' % destdir)
                            run_full_pret(outf)
                            os.remove(destdir + '/thread0.c')
     
if __name__ == '__main__':
    
    build()

#    collect_data(bddir + '/' + 'arm','arm_data.dump','Total cycles',':')
#    collect_data(bddir + '/' + 'h8','h8_data.dump','#cycles','    ')

collect_full_data(bddir + '/' + 'leon3','leon3_data.dump','Cycles',':')
collect_full_data(bddir + '/' + 'pret','pret_data.dump','Cycle',':')
##    post_process_data(bddir + '/' + 'arm', 'arm_data.dat','\t')
