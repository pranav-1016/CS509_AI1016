#ifndef CSR_H
#define CSR_H

#include <vector>

struct Edge {
    int to;
    long long weight;
};

struct AdjacencyGraph {
    int V;
    int E;
    bool weighted;
    std::vector<std::vector<Edge>> adjacency;
};

struct CSRGraph {
    int V;
    int E;
    bool weighted;
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
    std::vector<long long> values;
};

CSRGraph convert_to_csr(const AdjacencyGraph& graph);

#endif