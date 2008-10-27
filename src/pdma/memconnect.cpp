/*
  Copyright (c) 2007-2008 The Regents of the University of California.
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

#include "memconnect.h"

bool memconnect::id_valid(uint32_t id) {
    return (id > 0) && (id < NUM_THREADS);
}

memconnect::memconnect() {
    for (int i = 0; i < NUM_THREADS; i++) {
        spm[i] = NULL;
    }
    wmem = NULL;
}

void memconnect::mv_to_spm(uint32_t id, uint32_t dest_addr,
                           uint32_t wmem_addr, uint32_t sz) {
    /* Complete the definition */

}

void memconnect::mv_to_wmem(uint32_t id, uint32_t dest_addr,
                            uint32_t spm_addr, uint32_t sz) {
    /* Complete the definition */
}

void memconnect::register_spm(uint32_t id, l1_scratch* s) {
    /* Assume that the register invocations are made for unique spms. */
    if (id_valid(id)) {
        spm[id] = s;
    } else {
        cerr << "memconnect:: SPM id (" << id << ") is invalid" << endl;
    }
}

void memconnect::register_wmem(wheeled_mem* wm) {
    wmem = wm;
}

