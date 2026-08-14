#include<filesystem>
#include<iostream>
#include<stdexcept>
#include "wrapper_config.h"

using namespace std;
namespace fs = filesystem;


//This will help us to find the root dir, irrespective of where we run the program
fs::path findRoot() {
    fs::path current_path = fs::current_path();

    while (true) {
        if(fs::exists(current_path /"assignment_01")) {
            return current_path;
        }

        if (current_path == current_path.parent_path()) {
            break;
        }

        current_path = current_path.parent_path();

    }
    throw runtime_error("Could not able to find repository root\n\
        Either run directly from root repository or \n\
        Make structure liek root/assignment_01");
}

int runAssignment(const fs::path &root, const Assignment& assignment) {
    fs::path assignment_path = root / assignment.directory;
    
    // check for assignment directory
    if (!fs::exists(assignment_path)) {
        cerr << "Assignment directory not found : " << assignment_path << endl;
        return 1;
    }

    // Find asssignment driver
    fs::path driver = assignment_path / "bin" / assignment.driver;
    cout << "This is the driver path : " << driver << endl;

    if (!fs::exists(driver)) {
        cerr << "Assignment driver not found: " << driver << endl;

        return 1;
    }

     // Execute the assignment driver.
    string command = "\"" + driver.string() + "\"";
    cout << "\nLaunching: " << assignment.name << '\n';
    cout << "Executing: " << command << "\n\n";

    return system(command.c_str());

    // return 0;
}

void mainMenu(const fs::path &root) {
    /*
        This will show the basic info and option, how many assignment available
        Which assignments to choose etc. etc.
    */ 
    while (true) {
        cout << "\n";
        cout << "========================================\n";
        cout << "        CS509 Common Wrapper\n";
        cout << "========================================\n";
        cout << "Total Assignments available : " << assignments.size() << endl;

        for (int i=0; i < assignments.size(); i++) {
            cout << i + 1 << " . " << assignments[i].name << endl;
        }

        cout << assignments.size() + 1 << ". Exit" << endl;
        cout << "========================================\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == assignments.size() + 1) {
            cout << "Exiting common wrapper" << endl;
            return;
        }

        if (choice < 1 || choice > assignments.size() ) {
            cout << "Invalid choice" << endl;
            continue;
        }

        const Assignment &selectedAssignment = assignments[choice - 1];

        cout << "Selected : " << selectedAssignment.name << endl;

        // Now we will run the assignment
        int result = runAssignment(root, selectedAssignment);
        // int result = 0;
        cout << "Running the assignment >>>>>>>>>>> (placeholder)" << endl;

        if (result != 0) {
            cerr << "Assignmen driver exited with " << "error code : " << result << endl;
        }
    }
}

int main () {
    /* 
        - This look into the directory and find all the assignment_01 directory and based on the list
        - This will give user options or menu for which assignment to exectue
        - once the user choose the assignment the execution will be handled to the driver file of that assignment
    */

    try {
        fs::path root = findRoot();
        // Once we find the correct root position, we will generate top level MENU
        mainMenu(root);

    } catch(const exception& e) {
        cerr << "Error : " << e.what() << '\n';
        return 1;
    }
    

    return 0;
}