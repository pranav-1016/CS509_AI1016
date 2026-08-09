#include <iostream>
#include <filesystem>
#include <stdexcept>

#include "../../common_wrapper/assignment_wrapper.h"
#include "../include/assignment_config.h"

using namespace std;
namespace fs = filesystem;


// This will help us to find the assignment directory,
// irrespective of where we run the program.
fs::path findRoot()
{
    cout << "Driver's find root function running " << endl;
    fs::path current_path = fs::current_path();

    while (true) {
        if (fs::exists(current_path / assignmentConfig.dirname)) {
            current_path = current_path / assignmentConfig.dirname;
            return current_path;
        }

        if (current_path == current_path.parent_path()) {
            break;
        }
        current_path = current_path.parent_path();
    }

    throw runtime_error(
        "Could not able to find assignment root\n"
        "Either run directly from assignment directory or\n"
        "make sure the configured README file exists"
    );
}


int main()
{
    /*
        - This will find the assignment root directory.
        - The assignment configuration will tell the
          common assignment wrapper which algorithms
          are available.
        - The common assignment wrapper will generate
          the algorithm menu.
        - Once the user chooses an algorithm, the
          corresponding algorithm driver will be executed.
    */

    try {

        fs::path root = findRoot();

        // Generate the assignment-level MENU.
        assignmentMenu(root, assignmentConfig);

    } catch (const exception &e) {

        cerr << "Error : " << e.what() << '\n';

        return 1;
    }

    return 0;
}