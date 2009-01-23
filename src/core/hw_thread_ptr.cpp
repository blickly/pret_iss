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

  $Id$
*/


#include "hw_thread_ptr.h"

hw_thread_ptr::hw_thread_ptr(const hw_thread_ptr& thread_address) {
    _handle = thread_address._handle;
}

hw_thread_ptr::hw_thread_ptr(): _handle(NULL) {
    //  cout << " This should not be called " << endl;
}

hw_thread_ptr::hw_thread_ptr(const hw_thread* ht) {
    _handle = const_cast<hw_thread*>(ht);
}

bool hw_thread_ptr::is_null() const {
    return (_handle == NULL);
}

hw_thread* hw_thread_ptr::get_handle()  {
    return _handle;
}

hw_thread* hw_thread_ptr::operator->() const {
    return _handle;
}


void hw_thread_ptr::operator=(const hw_thread_ptr & thread_address) {
    _handle = thread_address._handle;
}

bool hw_thread_ptr::operator==(const hw_thread_ptr& thread_address) {
    return (this->_handle == thread_address._handle);
}

