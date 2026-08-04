#include "csr_driver.h"
#include "../include/csr.h"

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct TestResult {
    std::string name;
    int V, E;
    bool weighted;
    std::size_t row_ptr_size, col_idx_size, values_size;
    double csr_ms;
    bool pass;
};

static bool read_input(const fs::path& file, AdjacencyGraph& graph) {
    std::ifstream in(file);

    if (!in || !(in >> graph.V >> graph.E) || graph.V <= 0 || graph.E < 0)
        return false;

    graph.adjacency.resize(graph.V);

    std::string line;
    std::getline(in, line);

    bool format_detected = false;
    bool weighted = false;

    for (int count = 0; count < graph.V;) {
        if (!std::getline(in, line))
            return false;

        if (line.empty())
            continue;

        std::stringstream ss(line);
        int u, degree;

        if (!(ss >> u >> degree) || u < 0 || u >= graph.V || degree < 0)
            return false;

        std::vector<long long> tokens;
        long long x;

        while (ss >> x)
            tokens.push_back(x);

        if (!format_detected) {
            if (degree == 0)
                return false;

            if (tokens.size() == static_cast<std::size_t>(degree))
                weighted = false;
            else if (tokens.size() == static_cast<std::size_t>(2 * degree))
                weighted = true;
            else
                return false;

            format_detected = true;
        }

        if (!weighted) {
            if (tokens.size() != static_cast<std::size_t>(degree))
                return false;

            for (int i = 0; i < degree; ++i) {
                int v = static_cast<int>(tokens[i]);

                if (v < 0 || v >= graph.V)
                    return false;

                graph.adjacency[u].push_back({v, 0});
            }
        }
        else {
            if (tokens.size() != static_cast<std::size_t>(2 * degree))
                return false;

            for (int i = 0; i < degree; ++i) {
                int v = static_cast<int>(tokens[2 * i]);
                long long w = tokens[2 * i + 1];

                if (v < 0 || v >= graph.V)
                    return false;

                graph.adjacency[u].push_back({v, w});
            }
        }

        ++count;
    }

    graph.weighted = weighted;
    return true;
}

static void write_csr(const fs::path& file, const CSRGraph& csr) {
    std::ofstream out(file);

    if (!out)
        throw std::runtime_error("Cannot create " + file.string());

    out << "CSR Representation\n\n";
    out << "Vertices: " << csr.V << '\n';
    out << "Edges: " << csr.E << '\n';
    out << "Weighted: " << (csr.weighted ? "Yes" : "No") << "\n\n";

    out << "row_ptr:\n";
    for (std::size_t i = 0; i < csr.row_ptr.size(); ++i)
        out << (i ? " " : "") << csr.row_ptr[i];

    out << "\n\n";

    out << "col_idx:\n";
    for (std::size_t i = 0; i < csr.col_idx.size(); ++i)
        out << (i ? " " : "") << csr.col_idx[i];

    out << '\n';

    if (csr.weighted) {
        out << "\nvalues:\n";

        for (std::size_t i = 0; i < csr.values.size(); ++i)
            out << (i ? " " : "") << csr.values[i];

        out << '\n';
    }
}

static TestResult run_test(const fs::path& file, const fs::path& root) {
    AdjacencyGraph graph;

    if (!read_input(file, graph))
        throw std::runtime_error("Invalid input: " + file.string());

    auto start = std::chrono::steady_clock::now();
    CSRGraph csr = convert_to_csr(graph);
    auto end = std::chrono::steady_clock::now();

    TestResult r;

    r.name = file.filename().string();
    r.V = csr.V;
    r.E = csr.E;
    r.weighted = csr.weighted;
    r.row_ptr_size = csr.row_ptr.size();
    r.col_idx_size = csr.col_idx.size();
    r.values_size = csr.values.size();
    r.csr_ms = std::chrono::duration<double, std::milli>(end - start).count();

    r.pass =
        csr.row_ptr.size() == static_cast<std::size_t>(csr.V + 1) &&
        csr.col_idx.size() == static_cast<std::size_t>(csr.row_ptr[csr.V]);

    if (csr.weighted)
        r.pass = r.pass && csr.values.size() == csr.col_idx.size();
    else
        r.pass = r.pass && csr.values.empty();

    fs::create_directories(root / "actual");

    std::string stem = file.stem().string();
    fs::path actual_file = root / "actual" / (stem + "_csr.txt");

    write_csr(actual_file, csr);

    return r;
}

static void update_readme(const fs::path& root, const std::vector<TestResult>& results) {
    fs::path file = root / "README.md";
    std::ifstream in(file);

    if (!in)
        throw std::runtime_error("Cannot open " + file.string());

    std::stringstream buffer;
    buffer << in.rdbuf();

    std::string content = buffer.str();

    const std::string start_marker = "<!-- CSR_RESULTS_START -->";
    const std::string end_marker = "<!-- CSR_RESULTS_END -->";

    std::stringstream section;

    section << start_marker << "\n\n";
    section << "## CSR Results\n\n";

    section << "| Test File | Input Type | V | E | row_ptr | col_idx | values | CSR Time | Status |\n";
    section << "|---|---|---:|---:|---:|---:|---:|---:|---|\n";

    for (const auto& r : results) {
        section << "| " << r.name
                << " | " << (r.weighted ? "Weighted" : "Unweighted")
                << " | " << r.V
                << " | " << r.E
                << " | " << r.row_ptr_size
                << " | " << r.col_idx_size
                << " | " << r.values_size
                << " | " << std::fixed << std::setprecision(6)
                << r.csr_ms << " ms"
                << " | " << (r.pass ? "PASS" : "FAIL")
                << " |\n";
    }

    section << "\n" << end_marker;

    std::string new_section = section.str();

    std::size_t start_pos = content.find(start_marker);
    std::size_t end_pos = content.find(end_marker);

    if (start_pos != std::string::npos &&
        end_pos != std::string::npos &&
        end_pos >= start_pos) {

        end_pos += end_marker.size();
        content.replace(start_pos, end_pos - start_pos, new_section);
    }
    else {
        if (!content.empty() && content.back() != '\n')
            content += '\n';

        content += "\n" + new_section + "\n";
    }

    std::ofstream out(file);

    if (!out)
        throw std::runtime_error("Cannot write " + file.string());

    out << content;
}

int csr_driver_main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: csr_driver <file ... | all>\n";
        return 1;
    }

    fs::path root = fs::current_path();
    std::vector<fs::path> tests;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "all") {
            if (!tests.empty()) {
                std::cerr << "Error: 'all' cannot be combined with files.\n";
                return 1;
            }

            if (!fs::exists(root / "tests")) {
                std::cerr << "Error: tests directory not found.\n";
                return 1;
            }

            for (const auto& e : fs::directory_iterator(root / "tests")) {
                if (e.is_regular_file() &&
                    e.path().extension() == ".txt" &&
                    e.path().filename().string().rfind("csr_", 0) == 0)
                    tests.push_back(e.path());
            }
        }
        else {
            tests.push_back(arg);
        }
    }

    if (tests.empty()) {
        std::cerr << "Error: no test files found.\n";
        return 1;
    }

    std::vector<TestResult> results;

    for (auto file : tests) {
        if (!fs::exists(file))
            file = root / "tests" / file;

        if (!fs::exists(file)) {
            std::cerr << "Error: missing input file: " << file << '\n';
            continue;
        }

        try {
            results.push_back(run_test(file, root));
            std::cout << "Completed: " << file.filename() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    if (results.empty())
        return 1;

    bool all_pass = true;

    for (const auto& r : results)
        all_pass = all_pass && r.pass;

    try {
        update_readme(root, results);
        std::cout << "CSR results updated in README.md\n";
    }
    catch (const std::exception& e) {
        std::cerr << "README update failed: " << e.what() << '\n';
        return 1;
    }

    return all_pass ? 0 : 1;
}

#ifndef CSR_DRIVER_NO_MAIN

int main(int argc, char* argv[]) {
    return csr_driver_main(argc, argv);
}

#endif