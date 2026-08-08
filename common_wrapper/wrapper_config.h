#ifndef WRAPPER_CONFIG_H
#define WRAPPER_CONFIG_H

#include <string>
#include <vector>

struct Assignment {
    std::string name;
    std::string directory;
    std::string driver;
};

inline const std::vector<Assignment> assignments = {
    {
        "Assignment 01 - GEMM and CSR",
        "assignment_01",
        "assignment_01_driver"
    },
};

#endif