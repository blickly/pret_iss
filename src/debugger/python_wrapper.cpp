/*
  Copyright (c) 2007-2009 The Regents of the University of California.
  All rights reserved.

  Permission is hereby granted, without written agreement and without
  license or royalty fees, to use, copy, modify, and distribute this
  software and its documentation for any purpose, provided that the
  above copyright notice and the following two paragraphs appear in all
  copies of this software.

  IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
  ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
  THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.

  THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
  ENHANCEMENTS, OR MODIFICATIONS.

  PT_COPYRIGHT_VERSION_2
  COPYRIGHTENDKEY
  $Author$
  $Date$
  $Id$

*/



#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#endif
#include <iostream>
#include <Python.h>

#include "core.h"

static core* dbg;

static core* get_dbg() {
    //    if (dbg == NULL) {
    //        dbg = new core("soft.txt");
    //    }
    return dbg;
}

static PyObject *
pret_is_memory_read(PyObject *self, PyObject *args) {
    int thread_num;
    bool success;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    success = get_dbg()->is_memory_read(thread_num);
    return Py_BuildValue("i", success);
}

static PyObject *
pret_is_memory_write(PyObject *self, PyObject *args) {
    int thread_num;
    bool success;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    success = get_dbg()->is_memory_write(thread_num);
    return Py_BuildValue("i", success);
}

static PyObject *
pret_get_aligned_memory_address(PyObject *self, PyObject *args) {
    int thread_num;
    int addr;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    addr = get_dbg()->get_aligned_memory_address(thread_num);
    return Py_BuildValue("I", addr);
}

static PyObject *
pret_load(PyObject *self, PyObject *args) {
    const char* filename;
    if (!PyArg_ParseTuple(args, "s", &filename))
        return NULL;
    if (dbg != NULL) {
        return NULL;
    } else {
        dbg = new core(filename);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pret_mem(PyObject *self, PyObject *args) {
    get_dbg()->print_memory();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pret_get_pc(PyObject *self, PyObject *args) {
    int thread_num;
    uint32_t pc;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    pc = get_dbg()->get_pc(thread_num);
    return Py_BuildValue("I", pc);
}

static PyObject *
pret_get_cycle_count(PyObject *self, PyObject *args) {
    uint32_t cc;
    cc = get_dbg()->get_cycle_count();
    return Py_BuildValue("I", cc);
}

static PyObject *
pret_get_instruction_count(PyObject *self, PyObject *args) {
    unsigned int thread_num;
    unsigned int inst_cnt;
    if (!PyArg_ParseTuple(args, "I", &thread_num))
        return NULL;
    inst_cnt = get_dbg()->get_instruction_count(thread_num);
    return Py_BuildValue("I", inst_cnt);
}

static PyObject *
pret_read_memory(PyObject *self, PyObject *args) {
    uint32_t addr;
    uint32_t data;
    if (!PyArg_ParseTuple(args, "I", &addr))
        return NULL;
    data = get_dbg()->read_memory(addr);
    return Py_BuildValue("I", data);
}

static PyObject *
pret_get_period(PyObject *self, PyObject *args) {
    long int pd;
    pd = get_dbg()->get_period();
    return Py_BuildValue("l", pd);
}


static PyObject *
pret_is_fetch_stalled(PyObject *self, PyObject *args) {
    int thread_num;
    bool fetch_stalled = false;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    fetch_stalled = get_dbg()->is_fetch_stalled(thread_num);
    return Py_BuildValue("i", fetch_stalled);
}

static PyObject *
pret_is_memory_stalled(PyObject *self, PyObject *args) {
    int thread_num;
    bool memory_stalled = false;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    memory_stalled = get_dbg()->is_memory_stalled(thread_num);
    return Py_BuildValue("i", memory_stalled);
}

static PyObject *
pret_reg(PyObject *self, PyObject *args) {
    int thread_num;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    get_dbg()->print_registers(thread_num);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
pret_run(PyObject *self, PyObject *args) {
    int steps;
    bool success;
    if (!PyArg_ParseTuple(args, "i", &steps))
        return NULL;
    success = get_dbg()->run_for_instruction(steps);
    //    cout << "Look at success: " << success << endl;
    return Py_BuildValue("i", success);
}

static PyObject *
pret_timers(PyObject *self, PyObject *args) {
    int thread_num;
    if (!PyArg_ParseTuple(args, "i", &thread_num))
        return NULL;
    get_dbg()->print_timers(thread_num);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef PretMethods[] = {
    {"mem",  pret_mem, METH_NOARGS, "Print the contents of memory."},
    {"load",  pret_load, METH_VARARGS, "Open text file containing the names of SREC files to load."},
    {"is_memory_read", pret_is_memory_read, METH_VARARGS, "True if there is a read from memory."},
    {"is_memory_write", pret_is_memory_write, METH_VARARGS, "True if there is a write to memory."},
    {"get_aligned_memory_addr",  pret_get_aligned_memory_address, METH_VARARGS, "Return the aligned memory address."},
    {"get_pc",  pret_get_pc, METH_VARARGS, "Return the PC of this thread."},
    {"get_cycle_count",  pret_get_cycle_count, METH_VARARGS, "Return the cycle count."},
    {"get_period",  pret_get_period, METH_VARARGS, "Return the period of main clock."},
    {"read_memory",  pret_read_memory, METH_VARARGS, "Return the word at the given address."},
    {"is_fetch_stalled",  pret_is_fetch_stalled, METH_VARARGS, "Return whether fetch stage has caused a stall for this thread."},
    {"is_memory_stalled",  pret_is_memory_stalled, METH_VARARGS, "Return whether memory stage has caused a stall for this thread."},
    {"reg",  pret_reg, METH_VARARGS, "Print the registers of this thread."},
    {"timer",  pret_timers, METH_VARARGS, "Print the deadline timers of this thread."},
    {"run",  pret_run, METH_VARARGS, "Execute n instructions."},
    {"get_instruction_count",  pret_get_instruction_count, METH_VARARGS, "Get the instruction count for thread."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

/*
  PyMODINIT_FUNC
  initpret(void) {
  (void) Py_InitModule("pret", PretMethods);
  }
*/
PyMODINIT_FUNC
initlibpretdbg(void) {
    (void) Py_InitModule("libpretdbg", PretMethods);
}
