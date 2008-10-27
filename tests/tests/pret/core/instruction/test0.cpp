#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include "systemc.h"
#endif /* _NO_SYSTEMC_ */

#include "instruction.h"


int sc_main(int argc, char *argv[]) {
#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

    instruction in;

    cout << "Dummy PRET Test." << endl;
    cout << in << endl;
    return(0);

}



