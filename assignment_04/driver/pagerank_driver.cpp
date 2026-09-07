#include "../include/pagerank.h"
#include "../include/assignment_config.h"
#include "csr.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <cmath>

using namespace std;
namespace fs = filesystem;


struct TestResult {
    string name;

    int vertices;
    int edges;

    double damping;
    double tolerance;
    int max_iterations;

    double sum_of_ranks;

    int iterations;
    bool converged;

    double execution_time;

    bool pass;
};


struct PageRankInput {
    AdjacencyGraph graph;

    double damping;
    double tolerance;
    int max_iterations;
};


/*
 * Read a directed unweighted PageRank graph.
 */
static bool read_input(const fs::path& file, PageRankInput& input) {
    ifstream in(file);

    if (!in)
        return false;

    int V, E;

    if (!(in >> V >> E))
        return false;

    if (V <= 0 || E < 0)
        return false;

    input.graph.V = V;
    input.graph.E = E;
    input.graph.weighted = false;
    input.graph.adjacency.resize(V);

    int actual_entries = 0;

    for (int i = 0; i < V; ++i)
    {
        int u, outdegree;

        if (!(in >> u >> outdegree))
            return false;

        if (u < 0 || u >= V || outdegree < 0)
            return false;

        for (int j = 0; j < outdegree; ++j)
        {
            int neighbor;

            if (!(in >> neighbor))
                return false;

            if (neighbor < 0 || neighbor >= V)
                return false;

            if (neighbor == u)
                return false;

            input.graph.adjacency[u].push_back({neighbor, 0});

            ++actual_entries;
        }
    }

    /*
     * For PageRank the adjacency list contains
     * every directed edge exactly once.
     */
    if (actual_entries != E)
    {
        cout << "Actual entries != E error " << endl;
        return false;
    }


    /*
     * Read PageRank parameters.
     */
    string label;

    if (!(in >> label) || label != "DAMPING")
        return false;

    if (!(in >> input.damping))
        return false;

    if (!(in >> label) || label != "TOLERANCE")
        return false;

    if (!(in >> input.tolerance))
        return false;

    if (!(in >> label) || label != "MAX_ITERATIONS")
        return false;

    if (!(in >> input.max_iterations))
        return false;


    /*
     * Validate parameters.
     */
    if (input.damping <= 0.0 || input.damping >= 1.0)
        return false;

    if (input.tolerance <= 0.0)
        return false;

    if (input.max_iterations <= 0)
        return false;

    return true;
}

static bool validate_pagerank(const CSRGraph& graph, const PageRankResult& result) {
    int V = graph.V;

    if (static_cast<int>(result.ranks.size()) != V)
        return false;

    double sum = 0.0;

    for (int v = 0; v < V; ++v)
    {
        /*
         * PageRank values must be finite
         * and non-negative.
         */
        if (!isfinite(result.ranks[v]) || result.ranks[v] < 0.0)
            return false;

        sum += result.ranks[v];
    }

    /*
     * The rank sum should remain approximately 1.
     */
    if (fabs(sum - 1.0) > 0.000001)
        return false;

    return true;
}

static void write_output(
    const fs::path& file,
    const PageRankResult& result,
    double damping,
    double execution_time)
{
    ofstream out(file);

    if (!out)
        throw runtime_error("Could not create output file: " + file.string());

    out << "Algorithm: PageRank\n";

    out << fixed << setprecision(6);

    out << "Damping: " << damping << '\n';

    out << "Vertex ranks:\n";

    for (int v = 0; v < static_cast<int>(result.ranks.size()); ++v)
    {
        out << v << " " << result.ranks[v] << '\n';
    }

    out << "Sum of ranks: " << result.sum_of_ranks << '\n';

    out << "Iterations: " << result.iterations << '\n';

    out << "Converged: " << (result.converged ? "true" : "false") << '\n';

    out << "Execution time: " << execution_time << " ms\n";
}


/*
 * Run one PageRank test.
 */
static TestResult run_test(const fs::path& file, const fs::path& root, const AlgorithmConfig& config) {
    TestResult result{};

    result.name = file.filename().string();

    PageRankInput input;

    if (!read_input(file, input))
        throw runtime_error("Invalid input: " + file.string());


    result.vertices = input.graph.V;
    result.edges = input.graph.E;
    result.damping = input.damping;
    result.tolerance = input.tolerance;
    result.max_iterations = input.max_iterations;
    CSRGraph csr_graph = convert_to_csr(input.graph);

    auto start = chrono::steady_clock::now();

    PageRankResult pagerank_result = pagerank(
        csr_graph,
        input.damping,
        input.tolerance,
        input.max_iterations
    );

    auto end = chrono::steady_clock::now();

    result.execution_time = chrono::duration<double, milli>(end - start).count();
    result.iterations = pagerank_result.iterations;
    result.converged = pagerank_result.converged;
    result.sum_of_ranks = pagerank_result.sum_of_ranks;

    /*
     * Validate the PageRank result.
     *
     * The assignment allows MAX_ITERATIONS to be
     * reached without convergence, so convergence
     * itself is not treated as a failure.
     */
    bool valid = validate_pagerank(csr_graph, pagerank_result);

    result.pass = valid;

    
    fs::path actual_dir = root / config.directories.actual;
    fs::path results_dir = root / config.directories.results;

    fs::create_directories(actual_dir);
    fs::create_directories(results_dir);

    string stem = file.stem().string();
    fs::path actual_file = actual_dir / (stem + "_actual.txt");
    fs::path result_file = results_dir / (stem + ".txt");

    write_output(
        actual_file,
        pagerank_result,
        result.damping,
        result.execution_time
    );

    ofstream report(result_file);

    if (!report)
        throw runtime_error("Could not create result file: " + result_file.string());

    report << "Test File: " << result.name << '\n';
    report << "Vertices: " << result.vertices << '\n';
    report << "Edges: " << result.edges << '\n';
    report << fixed << setprecision(6);
    report << "Damping: " << result.damping << '\n';
    report << "Tolerance: " << result.tolerance << '\n';
    report << "Max Iterations: " << result.max_iterations << '\n';
    report << "Iterations: " << result.iterations << '\n';
    report << "Converged: " << (result.converged ? "Yes" : "No") << '\n';
    report << "Sum of Ranks: " << result.sum_of_ranks << '\n';
    report << "Execution Time: " << result.execution_time << " ms\n";
    report << "Actual Output: " << actual_file << '\n';
    report << "Status: " << (result.pass ? "PASS" : "FAIL") << '\n';



    return result;
}


int pagerank_driver_main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: pagerank_driver <file ... | all>\n";

        return 1;
    }


    /*
     * Locate Assignment 04 root dynamically.
     */
    fs::path root = fs::current_path();

    while (true)
    {
        if (fs::exists(root / assignmentConfig.dirname))
        {
            root /= assignmentConfig.dirname;

            break;
        }

        if (root == root.parent_path())
        {
            throw runtime_error("Could not find Assignment 04 root.");
        }

        root = root.parent_path();
    }


    /*
     * Find PageRank configuration.
     */
    const AlgorithmConfig* algorithm = nullptr;

    for (const auto& config : assignmentConfig.algorithms)
    {
        if (config.driver_name == "pagerank_driver")
        {
            algorithm = &config;
            break;
        }
    }

    if (algorithm == nullptr)
    {
        throw runtime_error("PageRank configuration not found.");
    }


    fs::path tests_dir = root / algorithm->directories.tests;

    vector<fs::path> tests;


    /*
     * Resolve requested test files.
     */
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];

        if (arg == "all")
        {
            if (!tests.empty())
            {
                cerr << "Error: 'all' cannot be combined "
                        "with individual files.\n";

                return 1;
            }

            if (!fs::exists(tests_dir))
            {
                cerr << "Error: test directory does not "
                        "exist: "
                     << tests_dir
                     << '\n';

                return 1;
            }

            for (const auto& entry : fs::directory_iterator(tests_dir))
            {
                if (entry.is_regular_file() &&
                    entry.path().extension() == ".txt")
                {
                    tests.push_back(entry.path());
                }
            }
        }
        else
        {
            fs::path file = arg;

            if (!fs::exists(file))
                file = tests_dir / file;

            if (!fs::exists(file))
            {
                cerr << "Error: missing input file: "
                     << file
                     << '\n';

                continue;
            }

            tests.push_back(file);
        }
    }


    if (tests.empty())
    {
        cerr << "Error: no test files found.\n";

        return 1;
    }


    vector<TestResult> results;


    /*
     * Run selected test cases.
     */
    for (const auto& file : tests)
    {
        try
        {
            results.push_back(run_test(file, root, *algorithm));

            cout << "Completed: " << file.filename() << '\n';
        }
        catch (const exception& e)
        {
            cerr << "Error: " << e.what() << '\n';
        }
    }


    if (results.empty())
        return 1;


    /*
     * Generate README.
     */
    ofstream readme(root / assignmentConfig.readme);

    if (!readme)
    {
        cerr << "Error: could not create README.\n";

        return 1;
    }


    readme << "# PageRank Test Results\n\n";

    readme << "| File | V | E | Damping | "
              "Tolerance | Iterations | "
              "Converged | Rank Sum | "
              "Time | Status |\n";

    readme << "|---|---:|---:|---:|---:|---:|"
              "---|---:|---:|---:|\n";


    bool all_pass = true;

    for (const auto& result : results)
    {
        readme
            << "| " << result.name
            << " | " << result.vertices
            << " | " << result.edges
            << " | " << fixed
            << setprecision(4)
            << result.damping
            << " | " << setprecision(8)
            << result.tolerance
            << " | " << result.iterations
            << " | "
            << (result.converged ? "Yes" : "No")
            << " | " << setprecision(6)
            << result.sum_of_ranks
            << " | " << setprecision(6)
            << result.execution_time
            << " ms"
            << " | "
            << (result.pass ? "PASS" : "FAIL")
            << " |\n";

        all_pass &= result.pass;
    }


    return all_pass ? 0 : 1;
}


#ifndef PAGERANK_DRIVER_NO_MAIN

int main(int argc, char* argv[])
{
    try
    {
        return pagerank_driver_main(argc, argv);
    }
    catch (const exception& e)
    {
        cerr << "Error: "
             << e.what()
             << '\n';

        return 1;
    }
}

#endif