#include "vertex_coloring.h"

#include <algorithm>
#include <vector>

using namespace std;

VertexColoringResult vertex_coloring(const CSRGraph& graph) {
    int n = graph.V;

    VertexColoringResult result;
    result.colors.assign(n, -1);
    result.colors_used = 0;
    result.valid = true;

    if (n == 0) {
        return result;
    }

    vector<int> degree(n);
    vector<int> order(n);

    /*
     * Compute degree from CSR.
     */
    for (int u = 0; u < n; u++) {
        degree[u] = graph.row_ptr[u + 1] - graph.row_ptr[u];
        order[u] = u;
    }

    /*
     * Welsh-Powell ordering:
     * non-increasing degree.
     * Vertex ID is used as a deterministic tie-breaker.
     */
    sort(order.begin(), order.end(),
         [&degree](int a, int b) {
             if (degree[a] != degree[b]) {
                 return degree[a] > degree[b];
             }

             return a < b;
         });

    /*
     * Greedy coloring.
     */
    vector<bool> color_used(n, false);

    for (int i = 0; i < n; i++) {
        int u = order[i];

        fill(color_used.begin(), color_used.end(), false);

        /*
         * Mark colors used by already-colored neighbours.
         */
        for (int j = graph.row_ptr[u];
             j < graph.row_ptr[u + 1];
             j++) {

            int v = graph.col_idx[j];

            if (result.colors[v] != -1) {
                color_used[result.colors[v]] = true;
            }
        }

        /*
         * Select the smallest available color.
         */
        int color = 0;

        while (color < n && color_used[color]) {
            color++;
        }

        result.colors[u] = color;

        if (color + 1 > result.colors_used) {
            result.colors_used = color + 1;
        }
    }

    /*
     * Validate the coloring.
     */
    for (int u = 0; u < n; u++) {
        for (int j = graph.row_ptr[u];
             j < graph.row_ptr[u + 1];
             j++) {

            int v = graph.col_idx[j];

            if (result.colors[u] == result.colors[v]) {
                result.valid = false;
                return result;
            }
        }
    }

    return result;
}