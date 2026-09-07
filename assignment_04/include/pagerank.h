#ifndef PAGERANK_H
#define PAGERANK_H

#include "csr.h"
using namespace std;

struct PageRankResult {
    vector<double> ranks;
    int iterations;
    bool converged;
    double sum_of_ranks;
};

PageRankResult pagerank(const CSRGraph& graph, double damping, double tolerance, int max_iterations);

#endif