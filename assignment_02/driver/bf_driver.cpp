#include "../include/bellman_ford.h"
#include "../include/assignment_config.h"

#include<filesystem>
#include<stdexcept>
#include<iostream>

using namespace std;
namespace fs = filesystem;

struct TestResult {
    string name;
    int vertices;
    int edges;
    int source;

    bool negative_cycle;
    double execution_time;
    bool pass;
};

void read_input() {
    // load the test directory first

}

void write_output() {

}

// may be funciton to append the results in the README.md file
void updateReadme() {

} 

TestResult runTest() {

}

int algorithm_driver_main(int argc, char *argv[]) {
    // validate the arguments
    if (argc < 2) {
        cerr << "Usage: bellman_ford_driver " << "<file ... | all>\n"; 
        return 1;
    }

    vector<fs::path> tests;
    // parse the argv array and
    // load the test directory according to the argv
    for (int i=0; i < ) {
        // will push all the test that to be run in the vector
        tests.push_back(files);
    }

    if (tests.empty()) {
        cerr << "Error: no test files found.\n";
        return 1;
    }
    vector<TestResult> results;

    for (const auto &file : tests) {
        try {
            results.push_back(run_test(file, root, *algorithm));
            cout << "Completed: " << file.filename() << '\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }



}

int main(int argc, char *argv[]) {
    try {
        return algorithm_driver_main(argc, argv);
    } catch (const exception &e) {
        cerr << "Error : " << e.what() << endl;

        return 1;
    }
}