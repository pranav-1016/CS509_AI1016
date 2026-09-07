#include "../include/vertex_coloring.h"
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

using namespace std;
namespace fs = filesystem;

struct TestResult
{
    string name;
    int vertices;
    int edges;
    int colors_used;
    bool valid;
    double execution_time;
    bool pass;
};

/*
 * Read an unweighted undirected adjacency-list graph.
 */
static bool read_input(const fs::path& file, AdjacencyGraph& graph)
{
    ifstream in(file);

    if (!in)
        return false;

    int V, E;

    if (!(in >> V >> E))
        return false;

    if (V <= 0 || E < 0)
        return false;

    graph.V = V;
    graph.E = E;
    graph.weighted = false;
    graph.adjacency.resize(V);

    int actual_entries = 0;

    for (int i = 0; i < V; ++i)
    {
        int u, degree;

        if (!(in >> u >> degree))
            return false;

        if (u < 0 || u >= V || degree < 0)
            return false;

        for (int j = 0; j < degree; ++j)
        {
            int neighbor;

            if (!(in >> neighbor))
                return false;

            if (neighbor < 0 || neighbor >= V)
                return false;

            if (neighbor == u)
                return false;

            graph.adjacency[u].push_back({neighbor, 0});
            ++actual_entries;
        }
    }

    /*
     * Every undirected edge appears twice in the
     * adjacency list.
     */
    if (actual_entries != 2 * E)
    {
        cout << "undirected edge 2 * E error " << endl;
        return false;
    }

    return true;
}

/*
 * Validate that no adjacent vertices have the same color.
 */
static bool validate_coloring(const CSRGraph& graph, const VertexColoringResult& result)
{
    int V = graph.V;

    if (static_cast<int>(result.colors.size()) != V)
        return false;

    for (int u = 0; u < V; ++u)
    {
        if (result.colors[u] < 0)
            return false;

        for (int j = graph.row_ptr[u]; j < graph.row_ptr[u + 1]; ++j)
        {
            int v = graph.col_idx[j];

            if (result.colors[u] == result.colors[v])
                return false;
        }
    }

    return true;
}

/*
 * Write the complete actual output of one test case.
 */
static void write_output(
    const fs::path& file,
    const VertexColoringResult& result,
    double execution_time)
{
    ofstream out(file);

    if (!out)
        throw runtime_error("Could not create output file: " + file.string());

    out << "Algorithm: Greedy Vertex Coloring\n";
    out << "Vertex colors:\n";

    for (int v = 0; v < static_cast<int>(result.colors.size()); ++v)
        out << v << " " << result.colors[v] << '\n';

    out << "Colors used: " << result.colors_used << '\n';
    out << "Valid coloring: " << (result.valid ? "true" : "false") << '\n';

    out << fixed << setprecision(6);
    out << "Execution time: " << execution_time << " ms\n";
}

/*
 * Run one Vertex Coloring test.
 */
static TestResult run_test(
    const fs::path& file,
    const fs::path& root,
    const AlgorithmConfig& config)
{
    TestResult result{};
    result.name = file.filename().string();

    AdjacencyGraph graph;

    if (!read_input(file, graph))
        throw runtime_error("Invalid input: " + file.string());

    result.vertices = graph.V;
    result.edges = graph.E;

    CSRGraph csr_graph = convert_to_csr(graph);

    auto start = chrono::steady_clock::now();
    VertexColoringResult coloring_result = vertex_coloring(csr_graph);
    auto end = chrono::steady_clock::now();

    result.execution_time = chrono::duration<double, milli>(end - start).count();

    bool valid = coloring_result.valid && validate_coloring(csr_graph, coloring_result);

    result.colors_used = coloring_result.colors_used;
    result.valid = valid;
    result.pass = valid;

    fs::path actual_dir = root / config.directories.actual;
    fs::path results_dir = root / config.directories.results;

    fs::create_directories(actual_dir);
    fs::create_directories(results_dir);

    string stem = file.stem().string();
    fs::path actual_file = actual_dir / (stem + "_actual.txt");
    fs::path result_file = results_dir / (stem + ".txt");

    write_output(actual_file, coloring_result, result.execution_time);

    /*
     * Write summary result.
     */
    ofstream report(result_file);

    if (!report)
        throw runtime_error("Could not create result file: " + result_file.string());

    report << "Test File: " << result.name << '\n';
    report << "Vertices: " << result.vertices << '\n';
    report << "Edges: " << result.edges << '\n';
    report << "Colors Used: " << result.colors_used << '\n';
    report << "Valid Coloring: " << (result.valid ? "Yes" : "No") << '\n';

    report << fixed << setprecision(6);
    report << "Execution Time: " << result.execution_time << " ms\n";
    report << "Actual Output: " << actual_file << '\n';
    report << "Status: " << (result.pass ? "PASS" : "FAIL") << '\n';

    return result;
}

int vertex_coloring_driver_main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: vertex_coloring_driver <file ... | all>\n";
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
            throw runtime_error("Could not find Assignment 04 root.");

        root = root.parent_path();
    }

    /*
     * Find Vertex Coloring configuration.
     */
    const AlgorithmConfig* algorithm = nullptr;

    for (const auto& config : assignmentConfig.algorithms)
    {
        if (config.driver_name == "vertex_coloring_driver")
        {
            algorithm = &config;
            break;
        }
    }

    if (algorithm == nullptr)
        throw runtime_error("Vertex Coloring configuration not found.");

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
                if (entry.is_regular_file() && entry.path().extension() == ".txt")
                    tests.push_back(entry.path());
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

            cout << "Completed: "
                 << file.filename()
                 << '\n';
        }
        catch (const exception& e)
        {
            cerr << "Error: "
                 << e.what()
                 << '\n';
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

    readme << "# Vertex Coloring Test Results\n\n";
    readme << "| File | V | E | Colors Used | "
              "Valid? | Time | Status |\n";
    readme << "|---|---:|---:|---:|---|---:|---|\n";

    bool all_pass = true;

    for (const auto& result : results)
    {
        readme << "| " << result.name
               << " | " << result.vertices
               << " | " << result.edges
               << " | " << result.colors_used
               << " | " << (result.valid ? "Yes" : "No")
               << " | " << fixed << setprecision(6)
               << result.execution_time
               << " ms"
               << " | " << (result.pass ? "PASS" : "FAIL")
               << " |\n";

        all_pass &= result.pass;
    }

    return all_pass ? 0 : 1;
}

#ifndef VERTEX_COLORING_DRIVER_NO_MAIN

int main(int argc, char* argv[])
{
    try
    {
        return vertex_coloring_driver_main(argc, argv);
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
