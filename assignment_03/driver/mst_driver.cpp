#include "../include/prims.h"
#include "../include/kruskal.h"
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

    long long kruskal_weight;
    long long prim_weight;

    double kruskal_time;
    double prim_time;

    bool kruskal_connected;
    bool prim_connected;

    bool equal;
    bool pass;
};


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
    graph.weighted = true;
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
            long long weight;

            if (!(in >> neighbor >> weight))
                return false;

            if (neighbor < 0 || neighbor >= V)
                return false;

            graph.adjacency[u].push_back({neighbor, weight});
            ++actual_entries;
        }
    }

    /*
     * Every undirected edge appears twice in the adjacency list.
     * Therefore the number of adjacency entries must be 2 * E.
     */
    if (actual_entries != 2 * E)
        return false;

    return true;
}


static void write_output(
    const fs::path& file,
    const MSTResult& kruskal_result,
    const MSTResult& prim_result,
    double kruskal_time,
    double prim_time)
{
    ofstream out(file);

    if (!out)
        throw runtime_error(
            "Could not create output file: " + file.string()
        );

    out << "Algorithm: Kruskal's MST\n";

    out << "MST edges:\n";

    for (const auto& edge : kruskal_result.edges)
    {
        auto [u, v, weight] = edge;
        out << u << " " << v << " " << weight << '\n';
    }

    out << "Total MST weight: "
        << kruskal_result.total_weight << '\n';

    out << fixed << setprecision(6);

    out << "Execution time: "
        << kruskal_time << " ms\n\n";


    out << "Algorithm: Prim's MST\n";

    out << "MST edges:\n";

    for (const auto& edge : prim_result.edges)
    {
        auto [u, v, weight] = edge;
        out << u << " " << v << " " << weight << '\n';
    }

    out << "Total MST weight: "
        << prim_result.total_weight << '\n';

    out << "Execution time: "
        << prim_time << " ms\n\n";


    bool equal =
        kruskal_result.total_weight ==
        prim_result.total_weight;

    out << "MST weight comparison: "
        << (equal ? "EQUAL" : "NOT EQUAL") << '\n';

    out << "Status: "
        << (equal &&
            kruskal_result.connected &&
            prim_result.connected
                ? "PASS"
                : "FAIL")
        << '\n';
}


static TestResult run_test(
    const fs::path& file,
    const fs::path& root,
    const AlgorithmConfig& config)
{
    TestResult result{};

    result.name = file.filename().string();

    AdjacencyGraph graph;

    if (!read_input(file, graph))
        throw runtime_error(
            "Invalid input: " + file.string()
        );

    result.vertices = graph.V;
    result.edges = graph.E;


    /*
     * CSR conversion is preprocessing and must not be timed.
     */
    CSRGraph csr_graph = convert_to_csr(graph);


    /*
     * Kruskal timing.
     *
     * Edge extraction and sorting performed inside
     * kruskalMST() are intentionally included here.
     */
    auto start = chrono::steady_clock::now();

    MSTResult kruskal_result =
        kruskalMST(csr_graph);

    auto end = chrono::steady_clock::now();

    result.kruskal_time =
        chrono::duration<double, milli>(
            end - start
        ).count();

    cout << "Kruskal Completed >>>>>>>>>" << endl;
    /*
     * Prim timing.
     */
    start = chrono::steady_clock::now();

    MSTResult prim_result =
        primMST(csr_graph, 0);

    end = chrono::steady_clock::now();

    result.prim_time =
        chrono::duration<double, milli>(
            end - start
        ).count();


    result.kruskal_weight =
        kruskal_result.total_weight;

    result.prim_weight =
        prim_result.total_weight;

    result.kruskal_connected =
        kruskal_result.connected;

    result.prim_connected =
        prim_result.connected;

    result.equal =
        result.kruskal_weight ==
        result.prim_weight;

        cout << "Prims Completed >>>>>>>>>" << endl;
    /*
     * A valid MST for a connected graph must contain
     * exactly V - 1 edges.
     */
    bool kruskal_valid =
        kruskal_result.connected &&
        kruskal_result.edges.size() ==
            static_cast<size_t>(graph.V - 1);

    bool prim_valid =
        prim_result.connected &&
        prim_result.edges.size() ==
            static_cast<size_t>(graph.V - 1);

    result.pass =
        kruskal_valid &&
        prim_valid &&
        result.equal;


    fs::path actual_dir =
        root / config.directories.actual;

    fs::path results_dir =
        root / config.directories.results;

    fs::create_directories(actual_dir);
    fs::create_directories(results_dir);


    string stem = file.stem().string();

    fs::path actual_file =
        actual_dir / (stem + "_actual.txt");

    fs::path result_file =
        results_dir / (stem + ".txt");


    write_output(
        actual_file,
        kruskal_result,
        prim_result,
        result.kruskal_time,
        result.prim_time
    );


    ofstream report(result_file);

    if (!report)
        throw runtime_error(
            "Could not create result file: " +
            result_file.string()
        );


    report << "Test File: "
           << result.name << '\n';

    report << "Vertices: "
           << result.vertices << '\n';

    report << "Edges: "
           << result.edges << '\n';

    report << "Kruskal Weight: "
           << result.kruskal_weight << '\n';

    report << "Prim Weight: "
           << result.prim_weight << '\n';

    report << "Weights Equal: "
           << (result.equal ? "Yes" : "No") << '\n';

    report << fixed << setprecision(6);

    report << "Kruskal Time: "
           << result.kruskal_time
           << " ms\n";

    report << "Prim Time: "
           << result.prim_time
           << " ms\n";

    report << "Actual Output: "
           << actual_file << '\n';

    report << "Status: "
           << (result.pass ? "PASS" : "FAIL")
           << '\n';


    return result;
}


int mst_driver_main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: mst_driver <file ... | all>\n";
        return 1;
    }


    /*
     * Locate Assignment 03 root dynamically.
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
            throw runtime_error(
                "Could not find Assignment 03 root."
            );

        root = root.parent_path();
    }


    /*
     * Find MST configuration.
     */
    const AlgorithmConfig* algorithm = nullptr;

    for (const auto& config : assignmentConfig.algorithms)
    {
        if (config.driver_name == "mst_driver")
        {
            algorithm = &config;
            break;
        }
    }

    if (algorithm == nullptr)
        throw runtime_error(
            "MST configuration not found."
        );


    fs::path tests_dir =
        root / algorithm->directories.tests;

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
                cerr <<
                    "Error: 'all' cannot be combined "
                    "with individual files.\n";

                return 1;
            }

            if (!fs::exists(tests_dir))
            {
                cerr <<
                    "Error: test directory does not exist: "
                    << tests_dir << '\n';

                return 1;
            }

            for (const auto& entry :
                 fs::directory_iterator(tests_dir))
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
                cerr <<
                    "Error: missing input file: "
                    << file << '\n';

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
     * Run all selected test cases.
     */
    for (const auto& file : tests)
    {
        try
        {
            results.push_back(
                run_test(
                    file,
                    root,
                    *algorithm
                )
            );

            cout <<
                "Completed: "
                << file.filename()
                << '\n';
        }
        catch (const exception& e)
        {
            cerr <<
                "Error: "
                << e.what()
                << '\n';
        }
    }


    if (results.empty())
        return 1;


    /*
     * Generate README result table.
     */
    ofstream readme(
        root / assignmentConfig.readme
    );

    if (!readme)
    {
        cerr <<
            "Error: could not create README.\n";

        return 1;
    }


    readme << "# MST Test Results\n\n";

    readme
        << "| File | V | E | Expected Wt. | "
           "Kruskal Wt. | Prim Wt. | "
           "Kruskal Time | Prim Time | "
           "Equal? | Status |\n";

    readme
        << "|---|---:|---:|---:|---:|---:|"
           "---:|---:|---|---|\n";


    bool all_pass = true;


    for (const auto& result : results)
    {
        /*
         * Expected weight is not part of the specified
         * MST input format, so it is left as '-'.
         */
        readme
            << "| " << result.name
            << " | " << result.vertices
            << " | " << result.edges
            << " | -"
            << " | " << result.kruskal_weight
            << " | " << result.prim_weight
            << " | " << fixed << setprecision(6)
            << result.kruskal_time << " ms"
            << " | " << result.prim_time << " ms"
            << " | " << (result.equal ? "Yes" : "No")
            << " | " << (result.pass ? "PASS" : "FAIL")
            << " |\n";

        all_pass &= result.pass;
    }


    return all_pass ? 0 : 1;
}


#ifndef MST_DRIVER_NO_MAIN

int main(int argc, char* argv[])
{
    try
    {
        cout << "Coming to the main function of driver >>>>>>>>" << endl;
        return mst_driver_main(argc, argv);
    }
    catch (const exception& e)
    {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}

#endif