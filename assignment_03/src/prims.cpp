#include "../include/prims.h"

#include <climits>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

MSTResult primMST(const CSRGraph& graph, int start_vertex) {
    MSTResult result;
    result.total_weight = 0;
    result.connected = false;

    if (graph.V == 0)
        return result;

    if (start_vertex < 0 || start_vertex >= graph.V)
        return result;

    vector<bool> in_mst(graph.V, false);

    // {weight, vertex, parent}
    priority_queue<
        tuple<long long, int, int>,
        vector<tuple<long long, int, int>>,
        greater<tuple<long long, int, int>>
    > pq;

    pq.push({0, start_vertex, -1});

    while (!pq.empty())
    {
        auto [weight, u, parent] = pq.top();
        pq.pop();

        if (in_mst[u])
            continue;

        in_mst[u] = true;

        // The starting vertex has no parent edge.
        if (parent != -1)
        {
            result.edges.push_back({parent, u, weight});
            result.total_weight += weight;
        }

        // Traverse u's CSR adjacency list.
        for (int i = graph.row_ptr[u]; i < graph.row_ptr[u + 1]; ++i)
        {
            int v = graph.col_idx[i];
            long long edge_weight = graph.values[i];

            if (!in_mst[v])
            {
                pq.push({edge_weight, v, u});
            }
        }
    }

    result.connected = (result.edges.size() == graph.V - 1);

    return result;
}