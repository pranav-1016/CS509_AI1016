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
        "driver"
    },
    {
        "Assignment 02 - Bellman-Ford and Floyd-Warshall",
        "assignment_02",
        "driver"
    },
    {
        "Assignment 03 - Minimum Spanning Trees",
        "assignment_03",
        "driver"
    }
    
};

#endif