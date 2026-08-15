#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "../../common_wrapper/assignment_wrapper.h"
#include "../include/assignment_config.h"

using namespace std;
namespace fs = filesystem;


// Find the assignment directory irrespective of
// where the executable is launched from.
fs::path findRoot()
{
    fs::path current_path = fs::current_path();

    while (true)
    {
        if (fs::exists(current_path / assignmentConfig.dirname))
        {
            return current_path / assignmentConfig.dirname;
        }

        if (current_path == current_path.parent_path())
            break;

        current_path = current_path.parent_path();
    }

    throw runtime_error(
        "Could not find assignment root.\n"
        "Make sure the assignment configuration is correct."
    );
}


int main()
{
    /*
        - Find the Assignment 03 root directory.
        - assignmentConfig describes the available algorithms.
        - The common assignment wrapper generates the
          assignment-level menu.
        - The user selects MST.
        - mst_driver is then executed.
        - mst_driver automatically runs both Prim and Kruskal.
    */

    try
    {
        fs::path root = findRoot();

        assignmentMenu(root, assignmentConfig);
    }
    catch (const exception& e)
    {
        cerr << "Error : " << e.what() << '\n';
        return 1;
    }

    return 0;
}