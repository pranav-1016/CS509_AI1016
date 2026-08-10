#include "../include/bellman_ford.h"

Bellman_Ford bellman_ford_distance(const CSRGraph& graph, int source)
{
    Bellman_Ford result;

    result.source = source;
    result.is_negative_cycle = false;

    const long long INF = 1LL << 60;

    result.distance.assign(graph.V, INF);
    result.distance[source] = 0;

    // Relax all edges V-1 times
    for (int i = 0; i < graph.V - 1; i++)
    {
        bool changed = false;

        for (int u = 0; u < graph.V; u++)
        {
            // If u is unreachable, there is no point relaxing its edges
            if (result.distance[u] == INF)
                continue;

            for (int j = graph.row_ptr[u]; j < graph.row_ptr[u + 1]; j++)
            {
                int v = graph.col_idx[j];
                long long weight = graph.values[j];

                if (result.distance[u] + weight < result.distance[v])
                {
                    result.distance[v] = result.distance[u] + weight;
                    changed = true;
                }
            }
        }

        // No changes means the shortest distances have already stabilized
        if (!changed)
            break;
    }

    // One additional pass to detect a reachable negative-weight cycle
    for (int u = 0; u < graph.V; u++)
    {
        if (result.distance[u] == INF)
            continue;

        for (int j = graph.row_ptr[u]; j < graph.row_ptr[u + 1]; j++)
        {
            int v = graph.col_idx[j];
            long long weight = graph.values[j];

            if (result.distance[u] + weight < result.distance[v])
            {
                result.is_negative_cycle = true;
                return result;
            }
        }
    }

    return result;
}