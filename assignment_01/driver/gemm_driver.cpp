#include "gemm_driver.h"
#include "../include/gemm.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct TestResult {
    std::string name;
    int M, K, N, block;
    double simple_ms, blocked_ms;
    bool expected, pass;
};

/* Read GEMM input: M K N, followed by A and B. */
static bool read_input(const fs::path& file,
                       std::vector<long long>& A,
                       std::vector<long long>& B,
                       int& M, int& K, int& N)
{
    std::ifstream in(file);
    if (!(in >> M >> K >> N) || M <= 0 || K <= 0 || N <= 0) return false;

    A.resize(M * K);
    B.resize(K * N);

    for (auto& x : A) if (!(in >> x)) return false;
    for (auto& x : B) if (!(in >> x)) return false;

    return true;
}

/* Read expected matrix. */
static bool read_matrix(const fs::path& file, std::vector<long long>& x)
{
    std::ifstream in(file);
    if (!in) return false;

    long long v;
    while (in >> v) x.push_back(v);
    return !in.bad();
}

/* Write matrix in normal row/column format. */
static void write_matrix(const fs::path& file,
                         const std::vector<long long>& C,
                         int M, int N)
{
    std::ofstream out(file);

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j)
            out << (j ? " " : "") << C[i * N + j];
        out << '\n';
    }
}

/* Run both GEMM implementations for one test. */
static TestResult run_test(const fs::path& file,
                           const fs::path& root,
                           int block)
{
    TestResult r{};
    r.name = file.filename().string();
    r.block = block;

    std::vector<long long> A, B;
    if (!read_input(file, A, B, r.M, r.K, r.N))
        throw std::runtime_error("Invalid input: " + file.string());

    std::vector<long long> simple(r.M * r.N);
    std::vector<long long> blocked(r.M * r.N);

    auto time = [](auto&& fn) {
        auto s = std::chrono::steady_clock::now();
        fn();
        return std::chrono::duration<double, std::milli>(
            std::chrono::steady_clock::now() - s).count();
    };

    r.simple_ms = time([&] {
        simple_gemm(A, B, simple, r.M, r.K, r.N);
    });

    r.blocked_ms = time([&] {
        blocked_gemm(A, B, blocked, r.M, r.K, r.N, block);
    });

    fs::create_directories(root / "actual");
    fs::create_directories(root / "results");

    std::string stem = file.stem().string();

    fs::path simple_file =
        root / "actual" / (stem + "_simple.txt");

    fs::path blocked_file =
        root / "actual" / (stem + "_blocked.txt");

    write_matrix(simple_file, simple, r.M, r.N);
    write_matrix(blocked_file, blocked, r.M, r.N);

    std::vector<long long> expected;
    fs::path expected_file =
        root / "expected" / file.filename();

    r.expected = fs::exists(expected_file);

    /*
     * If expected output exists, both implementations
     * must match it. Otherwise status is PASS.
     */
    r.pass = !r.expected ||
             (read_matrix(expected_file, expected) &&
              simple == expected &&
              blocked == expected);

    std::ofstream report(
        root / "results" / (stem + ".txt"));

    report << "Test File: " << r.name << "\n"
           << "Input Type / Size: "
           << r.M << "x" << r.K << " and "
           << r.K << "x" << r.N << "\n"
           << "Expected Output: "
           << (r.expected ? "Available" : "--") << "\n"
           << "Actual Simple: " << simple_file << "\n"
           << "Actual Blocking: " << blocked_file << "\n"
           << std::fixed << std::setprecision(6)
           << "Simple Time: " << r.simple_ms << " ms\n"
           << "Blocking Time: " << r.blocked_ms << " ms\n"
           << "Block Size: " << r.block << "\n"
           << "Status: " << (r.pass ? "PASS" : "FAIL") << '\n';

    return r;
}

/* Reusable entry point for both standalone driver and common wrapper. */
int gemm_driver_main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: gemm_driver <file ... | all> "
                     "[--block-size N]\n";
        return 1;
    }

    fs::path root = fs::current_path();
    root = root.parent_path();
    root = root / "assignment_01";
    // fs::path root = 
    std::vector<fs::path> tests;
    int block = 32;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--block-size" && i + 1 < argc)
            block = std::stoi(argv[++i]);

        else if (arg == "all") {
            if (!tests.empty()) {
                std::cerr << "Error: 'all' cannot be combined with files.\n";
                return 1;
            }

            for (const auto& e : fs::directory_iterator(root / "tests"))
                if (e.is_regular_file() &&
                    e.path().extension() == ".txt")
                    tests.push_back(e.path());
        }

        else
            tests.push_back(arg);
    }

    if (tests.empty()) {
        std::cerr << "Error: no test files found.\n";
        return 1;
    }

    std::vector<TestResult> results;

    for (auto file : tests) {
        if (!fs::exists(file)) file = root / "tests" / file;

        if (!fs::exists(file)) {
            std::cerr << "Error: missing input file: " << file << '\n';
            continue;
        }

        try {
            results.push_back(run_test(file, root, block));
            std::cout << "Completed: " << file.filename() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    if (results.empty()) return 1;

    std::ofstream readme(root / "README.md");

    readme << "# GEMM Test Results\n\n"
           << "| Test File | Input Type / Size | Expected Output | "
              "Simple Time | Blocking Time | Block Size | Status |\n"
           << "|---|---|---|---:|---:|---:|---|\n";

    bool all_pass = true;

    for (const auto& r : results) {
        readme << "| " << r.name
               << " | " << r.M << "x" << r.K
               << " and " << r.K << "x" << r.N
               << " | " << (r.expected ? "Available" : "--")
               << " | " << std::fixed << std::setprecision(6)
               << r.simple_ms << " ms"
               << " | " << r.blocked_ms << " ms"
               << " | " << r.block
               << " | " << (r.pass ? "PASS" : "FAIL")
               << " |\n";

        all_pass &= r.pass;
    }

    return all_pass ? 0 : 1;
}

#ifndef GEMM_DRIVER_NO_MAIN
int main(int argc, char* argv[])
{
    return gemm_driver_main(argc, argv);
}
#endif