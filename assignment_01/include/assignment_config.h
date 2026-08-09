#ifndef ASSIGNMENT_01_CONFIG_H
#define ASSIGNMENT_01_CONFIG_H

#include "../../common_wrapper/assignment_config.h"


inline const AssignmentConfig assignmentConfig = {
    "Assignment 01 - Graph Algorithms",
    "README.md",
    "assignment_01",
    {
        {
            "GEMM",
            "gemm_driver",
            {
                "tests/gemm",
                "actual/gemm",
                "results/gemm"
            }
        },

        {
            "CSR",
            "csr_driver",
            {
                "tests/csr",
                "actual/csr",
                "results/csr"
            }
        }
    }
};

#endif