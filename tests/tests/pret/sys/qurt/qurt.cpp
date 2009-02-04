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
*/
#ifndef _NO_SYSTEMC_
#include "systemc.h"
#endif

#include "core.h"

/* This include file is automatically generated when running make
   regression. This is a hack to get the PRET_ISS path for the
   following files that are used below. */
#include "regression.h"


#ifndef _NO_SYSTEMC_
int
sc_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

    /* Setup the strings for filenames

       file_name : This is the file that has the location of the tests.
    */

    string file_name = "/tests/tests/pret/sys/qurt/qurt.txt";
    string otxt_name = "rqurt.txt";
    string txt_name;
    string load_file;
    /* Ensure that the include/regression.h set the PRET_ISS path
       correctly */
    if (pret_iss_path != "") {
        txt_name = pret_iss_path + file_name;
        ifstream txt(txt_name.c_str());
        ofstream otxt(otxt_name.c_str());
        if (txt.is_open()) {
            string line;
            getline(txt, line);
            if (line.size() > 0) {
                if (line.at(0) == '.') {
                    line.erase(line.begin());
                }
                load_file = pret_iss_path + "/tests/tests/pret/sys/qurt" + line;
                otxt << load_file << endl;
            }

            otxt.close();
        }
        /* Generate the SREC files*/
        string compile_srec = pret_iss_path + "/scripts/compile_threads.py " + load_file.c_str() + " -q";
        compile_srec = compile_srec + " 2>/dev/null";//cout << "compile command: " << compile_srec << endl;
        int failed = system(compile_srec.c_str());
	if (failed) {
	  cout << "Error: system() failed." << endl;
	}

        core db(otxt_name.c_str());
        db.run(-1);
    } else {
        cout << "Error: regression.h has not set the pret_iss_path string correctly" << endl;
    }

    return(0);


}



