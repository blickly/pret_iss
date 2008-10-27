#ifdef _NO_SYSTEMC_
#include "no_systemc.h"
#else
#include "systemc.h"
#endif /* _NO_SYSTEMC_ */
using namespace std;
#include "regression.h"
#include "decode.h"

#ifdef _USE_SYSTEMC_
SC_MODULE(test_driver) {

    sc_in_clk clk;
    sc_out<hw_thread_ptr> out;
    sc_in<hw_thread_ptr> in;

    SC_CTOR(test_driver) {
        SC_METHOD(test_sc) {
            sensitive << clk.pos();
            load_test();
            tcount = 0;
        }
    };

    void test_sc() {
        if (tcount < inst.size()) {
            instruction write_inst(inst[tcount].second);
            //tcount++;
            hthread.inst = write_inst;
            hw_thread_ptr htout(&hthread);
            out.write(htout);
            hw_thread_ptr ht = in.read();
            if (!ht.is_null()) {
                cout << dec << tcount << ": " << hex << ht->inst << endl;
                ++tcount;
            }
        } else {
            sc_stop();
        }
    }

#else
class test_driver {
public:
    test_driver(const string& name) {
        load_test();
        tcount = 0;
    }
#endif /* _USE_SYSTEMC_ */
public:
    unsigned int tcount;
    vector< pair<string, unsigned int> > inst;
    hw_thread hthread;

    void load_test() {
        inst.push_back(pair<string, unsigned int>("nop", 0x01000000));

        // 32 global registers we could write to so let us generate a move
        // to all of them
        for (unsigned int i = 0; i < 32; i++) {
            unsigned int dest_reg = 0x80000000;
            dest_reg = dest_reg | (i << 24);
            inst.push_back(pair<string, unsigned int>("mov %g0,*", dest_reg));
        }
    }

    void dump_test() {
        cout << "------------ Test Dump ---------------" << endl;
        for (unsigned int i = 0; i < inst.size(); i++) {
            pair<string, unsigned int> dump(inst[i]);
            cout <<  dump.first << ", 0x" << hex << dump.second << endl;
            cout << "------------ End    ---------------" << endl;
        }
    }

    void test_cpp(hw_thread_ptr in, hw_thread_ptr& out) {

        if (tcount < inst.size()) {
            instruction write_inst(inst[tcount].second);
            hthread.inst = write_inst;
            hw_thread_ptr htout(&hthread);
            out = htout;
            hw_thread_ptr ht = in;

            if (!ht.is_null()) {
                cout << dec << tcount << ": " << hex << ht->inst << endl;
                ++tcount;
            }
        } else {
            exit(1);
        }
    }

};

#define ITERATION 185

int sc_main(int argc, char *argv[]) {
#ifdef _NO_SYSTEMC_
    cout << "SystemC Simulation" << endl;
#else
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
#endif

#ifdef _NO_SYSTEMC_
    test_driver tst("test_stimulus");
    decode decode_stage("decode_stage");
    hw_thread_ptr in, out;
    tst.dump_test();
    cout << endl;
    for (unsigned int i = 0; i < ITERATION; i++) {
        tst.test_cpp(out, in);
        decode_stage.behavior(in, out);
    }
#else
    sc_clock clock("sc_clock", 5, SC_NS);
    sc_signal<hw_thread_ptr> input, output;

    test_driver tst("test_stimulus");
    tst.clk(clock);
    tst.in(output);
    tst.out(input);

    decode decode_stage("decode_stage");
    decode_stage.clk(clock);
    decode_stage.in(input);
    decode_stage.out(output);

    /* The real test is here */
    tst.dump_test();
    cout << endl;

    sc_start(ITERATION, SC_NS);
#endif
    return(0);

}
