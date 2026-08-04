#include "gemm.h"
#include <algorithm>
#include<iostream>

static bool is_zero_matrix(const std::vector<long long>& A, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++)
        if (A[i] != 0)
            return false;

    return true;
}

static bool is_identity_matrix(const std::vector<long long>& A, int rows, int cols) {
    if (rows != cols)
        return false;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long long expected = (i == j) ? 1 : 0;

            if (A[i * cols + j] != expected)
                return false;
        }
    }

    return true;
}

void simple_gemm(
    const std::vector<long long>& A,
    const std::vector<long long>& B,
    std::vector<long long>& C,
    int M,
    int K,
    int N
) {
    if (is_zero_matrix(A, M, K) || is_zero_matrix(B, K, N)) {
        std::fill(C.begin(), C.end(), 0);
        std::cout << "Early exit >>>> zero matrix >>> " << std::endl;
        return;
    }

    if (is_identity_matrix(A, M, K)) {
        std::cout << "Early exit >>>> identity matrix >>> " << std::endl;
        C = B;
        return;
    }

    if (is_identity_matrix(B, K, N)) {
        C = A;
        std::cout << "Early exit >>>> identity matrix >>> " << std::endl;
        return;
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            long long sum = 0;

            for (int k = 0; k < K; k++)
                sum += A[i * K + k] * B[k * N + j];

            C[i * N + j] = sum;
        }
    }
}

void blocked_gemm(
    const std::vector<long long>& A,
    const std::vector<long long>& B,
    std::vector<long long>& C,
    int M,
    int K,
    int N,
    int block_size
) {
    if (block_size <= 0)
        return;

    if (is_zero_matrix(A, M, K) || is_zero_matrix(B, K, N)) {
        std::fill(C.begin(), C.end(), 0);
        return;
    }

    if (is_identity_matrix(A, M, K)) {
        C = B;
        return;
    }

    if (is_identity_matrix(B, K, N)) {
        C = A;
        return;
    }

    std::fill(C.begin(), C.end(), 0);

    for (int ii = 0; ii < M; ii += block_size) {
        for (int kk = 0; kk < K; kk += block_size) {
            for (int jj = 0; jj < N; jj += block_size) {

                int i_end = std::min(ii + block_size, M);
                int k_end = std::min(kk + block_size, K);
                int j_end = std::min(jj + block_size, N);

                for (int i = ii; i < i_end; i++) {
                    for (int k = kk; k < k_end; k++) {
                        for (int j = jj; j < j_end; j++) {
                            C[i * N + j] +=
                                A[i * K + k] * B[k * N + j];
                        }
                    }
                }
            }
        }
    }
}