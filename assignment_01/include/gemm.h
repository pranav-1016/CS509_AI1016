#ifndef GEMM_H
#define GEMM_H

#include <vector>

void simple_gemm(
    const std::vector<long long>& A,
    const std::vector<long long>& B,
    std::vector<long long>& C,
    int M,
    int K,
    int N
);

void blocked_gemm(
    const std::vector<long long>& A,
    const std::vector<long long>& B,
    std::vector<long long>& C,
    int M,
    int K,
    int N,
    int block_size
);

#endif