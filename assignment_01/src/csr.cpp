#include "csr.h"

CSRGraph convert_to_csr(const AdjacencyGraph& graph) {
    CSRGraph csr;

    csr.V = graph.V;
    csr.E = graph.E;
    csr.weighted = graph.weighted;

    csr.row_ptr.resize(graph.V + 1);
    csr.row_ptr[0] = 0;

    for (int v = 0; v < graph.V; ++v)
        csr.row_ptr[v + 1] =
            csr.row_ptr[v] + graph.adjacency[v].size();

    int total_edges = csr.row_ptr[graph.V];

    csr.col_idx.resize(total_edges);

    if (graph.weighted)
        csr.values.resize(total_edges);

    for (int v = 0; v < graph.V; ++v) {
        int pos = csr.row_ptr[v];

        for (const auto& edge : graph.adjacency[v]) {
            csr.col_idx[pos] = edge.to;

            if (graph.weighted)
                csr.values[pos] = edge.weight;

            ++pos;
        }
    }

    return csr;
}