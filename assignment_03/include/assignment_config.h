#ifndef ASSIGNMENT_03_CONFIG_H
#define ASSIGNMENT_03_CONFIG_H

#include "../../common_wrapper/assignment_config.h"


inline const AssignmentConfig assignmentConfig = {
    "Assignment Prim's and Kruskal's algorithm",
    "README.md",
    "assignment_03",
    {
        {
            "MST",
            "mst_driver",
            {
                "tests/mst",
                "actual/mst",
                "results/mst"
            }
        }
    }
};

#endif