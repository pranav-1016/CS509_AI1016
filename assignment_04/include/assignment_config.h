#ifndef ASSIGNMENT_04_CONFIG_H
#define ASSIGNMENT_04_CONFIG_H

#include "../../common_wrapper/assignment_config.h"


inline const AssignmentConfig assignmentConfig = {
    "Assignment 4 Vertex Coloring and PageRank",
    "README.md",
    "assignment_04",
    {
        {
            "Vertex Coloring",
            "vertex_coloring_driver",
            {
                "tests/vc",
                "actual/vc",
                "results/vc"
            }
        },
        {
            "Page Rank",
            "pagerank_driver",
            {
                "tests/pr",
                "actual/pr",
                "results/pr"
            }
        }
    }
};

#endif