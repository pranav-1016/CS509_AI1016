#include "../include/kruskal.h"
#include "../include/dsu.h"

#include <algorithm>
#include <tuple>
#include <vector>

using namespace std;

MSTResult kruskalMST(const CSRGraph& graph) {
    MSTResult result;
    result.total_weight = 0;
    result.connected = false;

    vector<tuple<long long, int, int>> edges;

    // Extract each undirected edge only once.
    for (int u = 0; u < graph.V; ++u)
    {
        for (int i = graph.row_ptr[u]; i < graph.row_ptr[u + 1]; ++i)
        {
            int v = graph.col_idx[i];
            long long weight = graph.values[i];

            if (u < v)
                edges.push_back({weight, u, v});
        }
    }

    // Sort edges by non-decreasing weight.
    sort(edges.begin(), edges.end());

    DSU dsu(graph.V);

    // Process edges in increasing order of weight.
    for (const auto& edge : edges)
    {
        auto [weight, u, v] = edge;

        if (dsu.unite(u, v))
        {
            result.edges.push_back({u, v, weight});
            result.total_weight += weight;

            // MST contains exactly V - 1 edges.
            if (result.edges.size() == graph.V - 1)
                break;
        }
    }

    result.connected = (result.edges.size() == graph.V - 1);

    return result;
}