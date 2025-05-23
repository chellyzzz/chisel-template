// sim_c/sim.cc
#include <iostream>
#include <bitset>

#include <verilated.h>
#include "Vtop.h"
#ifdef VCD
    #include "verilated_vcd_c.h"
    VerilatedVcdC* tfp = nullptr;
#endif

#include "sim.h"

using namespace std; 

// init pointers
const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
const std::unique_ptr<Vtop> top{new Vtop{contextp.get(), "TOP"}};

/* sim initial */
void sim_init(int argc, char *argv[]) {
    top->reset = 1;
    top->clock = 0;
#ifdef VCD
    contextp->traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("build/top.vcd");
#endif
    Verilated::commandArgs(argc,argv);
}

/* sim exit */
void sim_exit() {
    // finish work, delete pointer
    top->final();
#if VCD
    tfp->close();
    tfp = nullptr;
#endif
}

void display(){
    printf("---------new sample-----------\n");

}

void single_cycle() {
    contextp->timeInc(1);
    top->clock = 0; top->eval();
#ifdef VCD
 tfp->dump(contextp->time());
#endif

    contextp->timeInc(1);
    top->clock = 1; top->eval();
#ifdef VCD
 tfp->dump(contextp->time());
#endif
}

void gen_rand_input() {
    //TODO:
}

void get_output() {
    // TODO:
    display();
}   

void reset(int n) {
    top->reset = 1;
    while (n-- > 0) single_cycle();
    top->reset = 0;
    top->eval();
}

void sim_main(int argc, char *argv[]) {
    sim_init(argc, argv);
    reset(10);

    /* main loop */
    while (!contextp->gotFinish()) {
    for (int i = 0; i < 8; i++) {
            gen_rand_input();
            single_cycle();
        }
        break;
    }

    sim_exit();
}