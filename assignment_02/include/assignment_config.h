#ifndef ASSIGNMENT_02_CONFIG_H
#define ASSIGNMENT_02_CONFIG_H

#include "../../common_wrapper/assignment_config.h"


inline const AssignmentConfig assignmentConfig = {
    "Assignment 02 - Bellman-Ford and Floyd-Warshall",
    "README.md",
    "assignment_02",
    {
        {
            "Bellman-Ford",
            "bellman_ford_driver",
            {
                "tests/bellman_ford",
                "actual/bellman_ford",
                "results/bellman_ford"
            }
        },

        {
            "Floyd-Warshall",
            "floyd_warshall_driver",
            {
                "tests/floyd_warshall",
                "actual/floyd_warshall",
                "results/floyd_warshall"
            }
        }
    }
};

#endif