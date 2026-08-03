#include "gemm.h"

void simple_gemm(
    const std::vector<long long>& A,
    const std::vector<long long>& B,
    std::vector<long long>& C,
    int M,
    int K,
    int N
) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {

            long long sum = 0;

            for (int k = 0; k < K; k++) {
                sum += A[i * K + k] * B[k * N + j];
            }

            C[i * N + j] = sum;
        }
    }

    return;
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
    for (int ii = 0; ii < M; ii += block_size) {

        for (int kk = 0; kk < K; kk += block_size) {

            for (int jj = 0; jj < N; jj += block_size) {

                for (int i = ii; i < ii + block_size && i < M; i++) {

                    for (int k = kk; k < kk + block_size && k < K; k++) {

                        for (int j = jj; j < jj + block_size && j < N; j++) {

                            C[i * N + j] +=
                                A[i * K + k] * B[k * N + j];
                        }
                    }
                }
            }
        }
    }

    return;
}