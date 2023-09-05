
#include "pin.H"
#include <iostream>
#include <fstream>
#include <map>
#include <string>

// required global variables
//--------------------------------------------------------------
bool Record = false; // indicates if inside the main function
std::ofstream OutFile;

// map that will hold the state of each allocated memory region
// ADDRINT is a type defined by pin.H and represents a memory address
// the bool value will be TRUE if an address has been deallocated
std::map<ADDRINT, bool> MallocMap;

// a KNOB represents a command-line switch to the Pintool
// The template type parameter string represents the type of the values that the
// switch will take. Here, the KNOB lets you specify the name of the output
// file of the Pintool through the 'o' switch. The default value is memleakcheck.txt.
KNOB<std::string> OutFileName(KNOB_MODE_WRITEONCE, "Pintool",
                              "o", "memleakcheck.txt", "Memory trace file name");
//--------------------------------------------------------------

// two analysis routines to mark the execution and return of the main function
VOID RecordMainBegin()
{
    Record = true;
}

VOID RecordMainEnd()
{
    Record = false;
}

// Define an analysis function that will be called just after malloc returns to
// record the address of the allocated memory.
VOID RecordMalloc(ADDRINT addr)
{
    if (!Record)
    {
        std::cout << "malloc was called but main was not being run!" << std::endl;
        return;
    }

    if (addr == 0)
    {
        std::cerr << "Heap full!";
        return;
    }

    std::map<ADDRINT, bool>::iterator it = MallocMap.find(addr);

    if (it != MallocMap.end())
    {
        if (it->second)
        {
            // Allocating a previously allocated and freed memory.
            it->second = false;
        }
        else
        {
            // Malloc should not allocate memory that has
            // already been allocated but not freed.
            std::cerr << "Impossible!" << std::endl;
        }
    }
    else
    {
        // First time allocating at this address.
        MallocMap.insert(std::pair<ADDRINT, bool>(addr, false));
    }
}

VOID RecordFree(ADDRINT addr)
{
    if (!Record)
    {
        std::cout << "free was called but main was not being run!" << std::endl;
        return;
    }

    std::map<ADDRINT, bool>::iterator it = MallocMap.find(addr);

    if (it != MallocMap.end())
    {
        if (it->second)
        {
            // Double freeing.
            OutFile << "Object at address " << std::hex << addr << "has been freed more than once." << std::endl;
        }
        else
        {
            it->second = true; // Mark as freed.
        }
    }
    else
    {
        // Freeing unallocated memory.
        OutFile << "Freeing unallocated memory at " << std::hex << addr << "." << std::endl;
    }
}

// The IMG object represents the executable image. All Pin functions that operate at the image level start with IMG_*.
// For example, IMG_Name returns the name of the specified image.
VOID Image(IMG img, VOID *v)
{
    OutFile << "%s is loaded\n" << IMG_Name(img) << std::endl;

    // all Pin functions that operate at the routine level start with RTN_*.
    // For example, RTN_FindByName accepts an image and a C-style string and returns
    // a RTN object representing the routine being searched for

    // The calls to RTN_InsertCall must be wrapped by calls to RTN_Open and
    // RTN_Close so the Pintool can insert the analysis routines.

    // find the malloc function within the program image and obtain a RTN handle for it
    RTN mallocRtn = RTN_FindByName(img, "malloc");
    if (mallocRtn.is_valid())
    {
        RTN_Open(mallocRtn);
        // insert a pin call before the function that will call RecordMalloc
        // to pass the return value of the malloc function to the analysis routine, specify IARG_FUNCRET_EXITPOINT_VALUE
        RTN_InsertCall(mallocRtn, IPOINT_AFTER, (AFUNPTR)RecordMalloc,
                       IARG_FUNCRET_EXITPOINT_VALUE,
                       IARG_END);
        RTN_Close(mallocRtn);
    }

    // find the free function within the program image and obtain a RTN handle for it
    RTN freeRtn = RTN_FindByName(img, "free");
    if (freeRtn.is_valid())
    {
        RTN_Open(freeRtn);
        // insert a pin call before the function that will call RecordFree
        // to pass the argument of the free function to the analysis routine, specify IARG_FUNCARG_ENTRYPOINT_VALUE
        // followed by the index of the argument of the free function
        RTN_InsertCall(freeRtn, IPOINT_BEFORE, (AFUNPTR)RecordFree,
                       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
                       IARG_END);
        RTN_Close(freeRtn);
    }

    // find the main function within the program image and obtain a RTN handle for it
    RTN mainRtn = RTN_FindByName(img, "main");
    if (mainRtn.is_valid())
    {
        RTN_Open(mainRtn);
        // insert a pin call before and after the function that will set the Record boolean
        // based on entering/leaving main
        RTN_InsertCall(mainRtn, IPOINT_BEFORE, (AFUNPTR)RecordMainBegin,
                       IARG_END);
        RTN_InsertCall(mainRtn, IPOINT_AFTER, (AFUNPTR)RecordMainEnd,
                       IARG_END);
        RTN_Close(mainRtn);
    }
}

// define a finalization routine. This will be called upon termination of the instrumented program.
// accepts two arguments: one being the code argument that holds the value returned from the main function of the program
// the other...
VOID Fini(INT32 code, VOID *v)
{
    // all that needs to be done in the finalization routine is to iterate over MallocMap and detect
    // allocations that haven’t been freed.
    // The return from Fini marks the end of the instrumentation process.
    for (std::pair<ADDRINT, bool> p : MallocMap)
    {
        if (!p.second)
        {
            // Unfreed memory.
            OutFile << "Memory at " << std::hex << p.first << "allocated but not freed." << std::endl;
        }
    }

    OutFile.close();
}

/*
    In the main function, PIN_Init is called to have Pin parse the command line to initialize the Knobs.
    Because we are searching for functions using their names, PIN has to load the symbol table of the program image,
    which is done by calling PIN_InitSymbols. The function IMG_AddInstrumentFunction registers the instrumentation
    function Image to be called every time an image is loaded.

    Also, the finalization function is registered using PIN_AddFiniFunction.
    Note that the second argument to these functions is passed to the v parameter.
    I can use this parameter to pass any additional information to instrumentation functions.
*/
int main(int argc, char *argv[])
{
    PIN_InitSymbols();

    PIN_Init(argc, argv);

    OutFile.open(OutFileName.Value().c_str());

    IMG_AddInstrumentFunction(Image, 0);
    PIN_AddFiniFunction(Fini, 0);

    // PIN_StartProgram is called to start the program being analyzed. This function actually
    // never returns to the main function. Once it’s called, Pin takes over everything.
    PIN_StartProgram();

    return 0;
}