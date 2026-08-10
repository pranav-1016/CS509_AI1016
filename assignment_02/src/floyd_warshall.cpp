#include "../include/floyd_warshall.h"

using namespace std;

Floyd_Warshall floyd_warshall_distance(const CSRGraph& graph)
{
    Floyd_Warshall result;

    result.is_negative_cycle = false;

    const long long INF = 1LL << 60;

    // Create V x V distance matrix
    result.distance.assign(graph.V, vector<long long>(graph.V, INF));

    // Distance from a vertex to itself is 0
    for (int i = 0; i < graph.V; i++)
    {
        result.distance[i][i] = 0;
    }

    // Build the initial distance matrix from CSR
    for (int u = 0; u < graph.V; u++)
    {
        for (int j = graph.row_ptr[u]; j < graph.row_ptr[u + 1]; j++)
        {
            int v = graph.col_idx[j];
            long long weight = graph.values[j];

            result.distance[u][v] = weight;
        }
    }

    // Floyd-Warshall algorithm
    for (int k = 0; k < graph.V; k++)
    {
        for (int i = 0; i < graph.V; i++)
        {
            // No path from i to k
            if (result.distance[i][k] == INF)
                continue;

            for (int j = 0; j < graph.V; j++)
            {
                // No path from k to j
                if (result.distance[k][j] == INF)
                    continue;

                long long new_distance =
                    result.distance[i][k] +
                    result.distance[k][j];

                if (new_distance < result.distance[i][j])
                {
                    result.distance[i][j] = new_distance;
                }
            }
        }
    }

    // Check for negative-weight cycle
    for (int i = 0; i < graph.V; i++)
    {
        if (result.distance[i][i] < 0)
        {
            result.is_negative_cycle = true;
            break;
        }
    }

    return result;
}