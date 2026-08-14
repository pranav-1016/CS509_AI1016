#ifndef ASSIGNMENT_WRAPPER_H
#define ASSIGNMENT_WRAPPER_H

#include <filesystem>

#include "assignment_config.h"

using namespace std;
namespace fs = filesystem;


// This will show the assignment menu and
// allow the user to select an algorithm.
void assignmentMenu(
    const fs::path &root,
    const AssignmentConfig &assignmentConfig
);

#endif
