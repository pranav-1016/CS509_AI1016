#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <vector>
#include "../assignment_01/include/csr.h"
using namespace std;

struct Bellman_Ford {
    int source;
    vector<long long> distance;
    bool is_negative_cycle;
};

Bellman_Ford bellman_ford_distance(const CSRGraph& graph, int source);

#endif