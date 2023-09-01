#include "pin.H"
#include <iostream>
#include <fstream>

// Variable to hold the instruction count
UINT64 insCount = 0;

// Function to be called before every instruction is executed
void docount() {
    insCount++;
}

// Pin calls this function every time a new instruction is encountered
void Instruction(INS ins, void *v) {
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)docount, IARG_END);
}

// This function is called when the application exits
void Fini(INT32 code, void *v) {
    std::cout << "Total number of instructions executed: " << insCount << std::endl;
}

// argc, argv are the entire command line, including pin -t <toolname> -- ...
int main(int argc, char * argv[]) {
    // Initialize pin
    PIN_Init(argc, argv);

    // Register Instruction to be called to instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(Fini, 0);

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}
