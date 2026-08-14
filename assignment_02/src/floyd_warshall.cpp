#include "../include/floyd_warshall.h"

using namespace std;

Floyd_Warshall floyd_warshall_distance(const vector<vector<long long>>& graph) {
    Floyd_Warshall result;

    result.is_negative_cycle = false;

    int V = graph.size();
    const long long INF = 1LL << 60;

    // Copy input graph into distance matrix
    result.distance = graph;

    // Distance from a vertex to itself is 0
    for (int i = 0; i < V; i++)
        result.distance[i][i] = 0;

    // Floyd-Warshall algorithm
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            if (result.distance[i][k] == INF)
                continue;

            for (int j = 0; j < V; j++) {
                if (result.distance[k][j] == INF)
                    continue;

                long long new_distance =
                    result.distance[i][k] +
                    result.distance[k][j];

                if (new_distance < result.distance[i][j])
                    result.distance[i][j] = new_distance;
            }
        }
    }

    // Check for negative-weight cycle
    for (int i = 0; i < V; i++) {
        if (result.distance[i][i] < 0) {
            result.is_negative_cycle = true;
            break;
        }
    }

    return result;
}
