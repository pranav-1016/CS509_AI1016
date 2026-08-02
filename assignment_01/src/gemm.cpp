#include "../include/gemm.h"

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

namespace gemm {

Matrix allocate_matrix(std::size_t rows, std::size_t cols) {
    Matrix M;
    M.rows = rows;
    M.cols = cols;
    M.data.assign(rows * cols, 0);
    return M;
}

inline long long &at(Matrix &M, std::size_t r, std::size_t c) {
    return M.data[r * M.cols + c];
}

inline const long long &at(const Matrix &M, std::size_t r, std::size_t c) {
    return M.data[r * M.cols + c];
}

bool parse_test_file(const std::string &path, Matrix &A, Matrix &B) {
    std::ifstream in(path);
    if (!in) return false;
    std::size_t M, K, N;
    if (!(in >> M >> K >> N)) return false;
    A = allocate_matrix(M, K);
    B = allocate_matrix(K, N);
    for (std::size_t i = 0; i < M; ++i) {
        for (std::size_t j = 0; j < K; ++j) {
            long long v; if (!(in >> v)) return false; at(A, i, j) = v;
        }
    }
    for (std::size_t i = 0; i < K; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            long long v; if (!(in >> v)) return false; at(B, i, j) = v;
        }
    }
    return true;
}

bool write_matrix_output(const std::string &path, const Matrix &C) {
    std::ofstream out(path);
    if (!out) return false;
    out << C.rows << " " << C.cols << '\n';
    for (std::size_t i = 0; i < C.rows; ++i) {
        for (std::size_t j = 0; j < C.cols; ++j) {
            out << at(C, i, j);
            if (j + 1 < C.cols) out << ' ';
        }
        out << '\n';
    }
    return true;
}

std::string make_output_filename(const std::string &test_path, const std::string &suffix) {
    // extract filename without extension
    auto pos = test_path.find_last_of("/\\");
    std::string name = (pos == std::string::npos) ? test_path : test_path.substr(pos + 1);
    auto dot = name.find_last_of('.');
    if (dot != std::string::npos) name = name.substr(0, dot);
    return name + suffix;
}

void simple_gemm(const Matrix &A, const Matrix &B, Matrix &C) {
    if (A.cols != B.rows) return;
    C = allocate_matrix(A.rows, B.cols);
    for (std::size_t i = 0; i < A.rows; ++i) {
        for (std::size_t k = 0; k < A.cols; ++k) {
            long long a = at(A, i, k);
            for (std::size_t j = 0; j < B.cols; ++j) {
                at(C, i, j) += a * at(B, k, j);
            }
        }
    }
}

void blocked_gemm(const Matrix &A, const Matrix &B, Matrix &C, std::size_t tile) {
    if (A.cols != B.rows) return;
    C = allocate_matrix(A.rows, B.cols);
    std::size_t M = A.rows, K = A.cols, N = B.cols;
    for (std::size_t ii = 0; ii < M; ii += tile) {
        std::size_t i_max = std::min(ii + tile, M);
        for (std::size_t kk = 0; kk < K; kk += tile) {
            std::size_t k_max = std::min(kk + tile, K);
            for (std::size_t jj = 0; jj < N; jj += tile) {
                std::size_t j_max = std::min(jj + tile, N);
                for (std::size_t i = ii; i < i_max; ++i) {
                    for (std::size_t k = kk; k < k_max; ++k) {
                        long long a = at(A, i, k);
                        for (std::size_t j = jj; j < j_max; ++j) {
                            at(C, i, j) += a * at(B, k, j);
                        }
                    }
                }
            }
        }
    }
}

bool compare_matrices(const Matrix &X, const Matrix &Y, long long tol) {
    if (X.rows != Y.rows || X.cols != Y.cols) return false;
    for (std::size_t i = 0; i < X.rows; ++i) {
        for (std::size_t j = 0; j < X.cols; ++j) {
            long long a = at(X, i, j);
            long long b = at(Y, i, j);
            if (std::llabs(a - b) > tol) return false;
        }
    }
    return true;
}

void copy_matrix(const Matrix &src, Matrix &dst) {
    dst = allocate_matrix(src.rows, src.cols);
    dst.data = src.data;
}

} // namespace gemm


