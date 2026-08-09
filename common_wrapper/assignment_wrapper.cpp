#include <iostream>
#include <filesystem>
#include <cstdlib>

#include "assignment_wrapper.h"

using namespace std;
namespace fs = filesystem;

struct TestCaseSelection {
    string cliArgs;
    bool cancelled = false;
};


// This will execute the driver of the selected algorithm.
int runAlgorithm(const fs::path &root, const AlgorithmConfig &algorithm, const string &cliArgs) {
    // Find algorithm driver
    fs::path driver = root / "bin" / algorithm.driver_name;

    if (!fs::exists(driver)) {
        cerr << "Algorithm driver not found: "
             << driver << endl;
        return 1;
    }

    // Execute the algorithm driver.
    string command = "\"" + driver.string() + "\"";

    // Add CLI arguments if provided.
    cout << "This was the cliArgs passed : " << cliArgs << endl;
    if (!cliArgs.empty()) {
        command += " " + cliArgs;
    }

    cout << "\nLaunching: " << algorithm.name << '\n';
    cout << "Executing: " << command << "\n\n";

    return system(command.c_str());
}

TestCaseSelection testCaseSelectionMenu() {
    TestCaseSelection selection;

    while (true) {

        cout << "\n";
        cout << "========================================\n";
        cout << "          Test Case Selection\n";
        cout << "========================================\n";

        cout << "1. Run all test cases\n";
        cout << "2. Select test cases\n";
        cout << "3. Back\n";

        cout << "========================================\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        // Clear invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid choice. Please enter a number.\n";
            continue;
        }

        // ----------------------------------------
        // Option 1: Run all test cases
        // ----------------------------------------
        if (choice == 1) {

            // Empty arguments means:
            // driver will execute all test cases.
            selection.cliArgs = "all";
            selection.cancelled = false;

            return selection;
        }

        // ----------------------------------------
        // Option 2: Select test cases
        // ----------------------------------------
        else if (choice == 2) {

            cout << "\nEnter test file names separated by spaces.\n";
            cout << "Example:\n";
            cout << "gemm_test_01.txt gemm_test_03.txt gemm_test_05.txt\n";
            cout << "\nTest files must be present in the test directory.\n";
            cout << "Enter test cases: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string input;
            getline(cin, input);

            // Remove leading/trailing whitespace
            size_t first = input.find_first_not_of(" \t");
            size_t last  = input.find_last_not_of(" \t");

            if (first == string::npos) {
                cout << "No test cases entered.\n";
                continue;
            }

            input = input.substr(first, last - first + 1);

            // The input is already space-separated, so it can
            // directly become CLI arguments.
            selection.cliArgs = input;
            selection.cancelled = false;

            return selection;
        }

        // ----------------------------------------
        // Option 3: Back
        // ----------------------------------------
        else if (choice == 3) {

            selection.cliArgs = "";
            selection.cancelled = true;

            return selection;
        }

        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}

// This will show the available algorithms in the assignment
// and allow the user to select which algorithm to execute.
void assignmentMenu(const fs::path &root, const AssignmentConfig &assignmentConfig) {
    /*
        This will show the basic information and options:
        How many algorithms are available
        Which algorithm to choose
        etc. etc.
    */

    while (true) {
        cout << "\n";
        cout << "========================================\n";
        cout << "        " << assignmentConfig.name << "\n";
        cout << "========================================\n";

        cout << "Total Algorithms available : " << assignmentConfig.algorithms.size() << endl;
        for (int i = 0; i < assignmentConfig.algorithms.size(); i++) {
            cout << i + 1 << " . " << assignmentConfig.algorithms[i].name << endl;
        }

        cout << assignmentConfig.algorithms.size() + 1 << ". Back" << endl;
        cout << "========================================\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (choice == assignmentConfig.algorithms.size() + 1) {
            cout << "Returning to previous menu" << endl;
            return;
        }

        if (choice < 1 || choice > assignmentConfig.algorithms.size()) {
            cout << "Invalid choice" << endl;
            continue;
        }

        const AlgorithmConfig &selectedAlgorithm = assignmentConfig.algorithms[choice - 1];

        cout << "Selected : " << selectedAlgorithm.name << endl;
        // Now we will choose on what options to run the algorithm drivers
        // 1. On all test cases,
        // 2. On selected test cases,
        TestCaseSelection testSelection = testCaseSelectionMenu();

        // User selected "Back" 
        if (testSelection.cancelled) { continue; }

        // Now we will run the selected algorithm driver.
        // int result = runAlgorithm(root, selectedAlgorithm);

        int result = runAlgorithm( root, selectedAlgorithm, testSelection.cliArgs );

        if (result != 0) {
            cerr << "Algorithm driver exited with " << "error code : " << result << endl;
        }
    }
}
