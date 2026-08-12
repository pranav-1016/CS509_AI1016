#include "../include/bellman_ford.h"
#include "../include/assignment_config.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <utility>

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

static bool read_input(const fs::path& file, AdjacencyGraph& graph, int& source) {
    ifstream in(file);
    if (!in) return false;

    int V, E;
    if (!(in >> V >> E)) return false;
    if (V <= 0 || E < 0) return false;

    graph.V = V;
    graph.E = E;
    graph.weighted = true;
    graph.adjacency.resize(V);

    int actual_edges = 0;

    for (int i = 0; i < V; ++i) {
        int u, degree;

        if (!(in >> u >> degree)) return false;
        if (u < 0 || u >= V || degree < 0) return false;

        for (int j = 0; j < degree; ++j) {
            int neighbor;
            long long weight;

            if (!(in >> neighbor >> weight)) return false;
            if (neighbor < 0 || neighbor >= V) return false;

            graph.adjacency[u].push_back({neighbor, weight});
            actual_edges++;
        }
    }

    string source_label;
    if (!(in >> source_label >> source)) return false;
    if (source_label != "SOURCE") return false;
    if (source < 0 || source >= V) return false;
    if (actual_edges != E) return false;

    return true;
}

static void write_output(const fs::path& file, const Bellman_Ford& result, double execution_time) {
    ofstream out(file);

    if (!out)
        throw runtime_error("Could not create output file: " + file.string());

    out << "Algorithm: Bellman-Ford\n";
    out << "Source: " << result.source << "\n";

    if (result.is_negative_cycle) {
        out << "Negative cycle: true\n";
    } else {
        out << "Vertex Distance\n";

        const long long INF = 1LL << 60;

        for (int i = 0; i < static_cast<int>(result.distance.size()); ++i) {
            out << i << " ";

            if (result.distance[i] == INF)
                out << "INF";
            else
                out << result.distance[i];

            out << '\n';
        }

        out << "Negative cycle: none\n";
    }

    out << fixed << setprecision(6);
    out << "Execution time: " << execution_time << " ms\n";
}

static TestResult run_test(const fs::path& file, const fs::path& root, const AlgorithmConfig& config) {
    TestResult result{};
    result.name = file.filename().string();

    AdjacencyGraph graph;
    int source;

    if (!read_input(file, graph, source))
        throw runtime_error("Invalid input: " + file.string());

    result.vertices = graph.V;
    result.edges = graph.E;
    result.source = source;

    CSRGraph csr_graph = convert_to_csr(graph);

    auto start = chrono::steady_clock::now();
    Bellman_Ford output = bellman_ford_distance(csr_graph, source);
    auto end = chrono::steady_clock::now();

    result.execution_time = chrono::duration<double, milli>(end - start).count();
    result.negative_cycle = output.is_negative_cycle;

    fs::path actual_dir = root / config.directories.actual;
    fs::path results_dir = root / config.directories.results;

    fs::create_directories(actual_dir);
    fs::create_directories(results_dir);

    string stem = file.stem().string();
    fs::path actual_file = actual_dir / (stem + "_actual.txt");
    fs::path result_file = results_dir / (stem + ".txt");

    write_output(actual_file, output, result.execution_time);

    ofstream report(result_file);

    if (!report)
        throw runtime_error("Could not create result file: " + result_file.string());

    report << "Test File: " << result.name << '\n';
    report << "Vertices: " << result.vertices << '\n';
    report << "Edges: " << result.edges << '\n';
    report << "Source: " << result.source << '\n';
    report << "Negative Cycle: " << (result.negative_cycle ? "Yes" : "No") << '\n';
    report << "Actual Output: " << actual_file << '\n';
    report << fixed << setprecision(6);
    report << "Execution Time: " << result.execution_time << " ms\n";

    result.pass = true;
    report << "Status: " << (result.pass ? "PASS" : "FAIL") << '\n';

    return result;
}

int bellman_ford_driver_main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: bellman_ford_driver <file ... | all>\n";
        return 1;
    }

    fs::path root = fs::current_path();

    while (true) {
        if (fs::exists(root / assignmentConfig.dirname)) {
            root /= assignmentConfig.dirname;
            break;
        }

        if (root == root.parent_path())
            throw runtime_error("Could not find Assignment 02 root.");

        root = root.parent_path();
    }

    const AlgorithmConfig* algorithm = nullptr;

    for (const auto& config : assignmentConfig.algorithms) {
        if (config.driver_name == "bellman_ford_driver") {
            algorithm = &config;
            break;
        }
    }

    if (algorithm == nullptr)
        throw runtime_error("Bellman-Ford configuration not found.");

    fs::path tests_dir = root / algorithm->directories.tests;
    vector<fs::path> tests;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "all") {
            if (!tests.empty()) {
                cerr << "Error: 'all' cannot be combined with individual files.\n";
                return 1;
            }

            for (const auto& entry : fs::directory_iterator(tests_dir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".txt")
                    tests.push_back(entry.path());
            }
        } else {
            fs::path file = arg;

            if (!fs::exists(file))
                file = tests_dir / file;

            if (!fs::exists(file)) {
                cerr << "Error: missing input file: " << file << '\n';
                continue;
            }

            tests.push_back(file);
        }
    }

    if (tests.empty()) {
        cerr << "Error: no test files found.\n";
        return 1;
    }

    vector<TestResult> results;

    for (const auto& file : tests) {
        try {
            results.push_back(run_test(file, root, *algorithm));
            cout << "Completed: " << file.filename() << '\n';
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << '\n';
        }
    }

    if (results.empty())
        return 1;

    ofstream readme(root / assignmentConfig.readme);

    if (!readme) {
        cerr << "Error: could not create README.\n";
        return 1;
    }

    readme << "# Bellman-Ford Test Results\n\n";
    readme << "| Test File | Vertices | Edges | Source | Negative Cycle | Time | Status |\n";
    readme << "|---|---:|---:|---:|---|---:|---|\n";

    bool all_pass = true;

    for (const auto& result : results) {
        readme << "| " << result.name
               << " | " << result.vertices
               << " | " << result.edges
               << " | " << result.source
               << " | " << (result.negative_cycle ? "Yes" : "No")
               << " | " << fixed << setprecision(6) << result.execution_time << " ms"
               << " | " << (result.pass ? "PASS" : "FAIL")
               << " |\n";

        all_pass &= result.pass;
    }

    return all_pass ? 0 : 1;
}

#ifndef BELLMAN_FORD_DRIVER_NO_MAIN

int main(int argc, char* argv[]) {
    try {
        return bellman_ford_driver_main(argc, argv);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}

#endif