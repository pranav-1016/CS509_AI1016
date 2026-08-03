// This driver should :
// The driver must invoke the core algorithm and produce the required result and timing information.
// The driver should be kept separate from the core algorithm implementation wherever practical.
// The driver must support execution of one selected test file and/or all test files belonging to that assignment.


#include "gemm_driver.h"
#include "../include/gemm.h"

int gemm_driver_main(int argc, char* argv[])
{
    // argument validation
    // input reading
    // algorithm selection
    // timing
    // result printing

    return 0;
}


#ifndef GEMM_DRIVER_NO_MAIN

int main(int argc, char* argv[])
{
    return gemm_driver_main(argc, argv);
}

#endif