#include "pagerank.h"

#include <cmath>
#include <vector>

using namespace std;

PageRankResult pagerank(const CSRGraph& graph, double damping, double tolerance, int max_iterations) {
    int n = graph.V;

    PageRankResult result;
    result.iterations = 0;
    result.converged = false;
    result.sum_of_ranks = 0.0;

    if (n == 0) {
        result.converged = true;
        return result;
    }

    /*
     * Initial rank: 1 / N for every vertex.
     */
    double initial_rank = 1.0 / static_cast<double>(n);

    vector<double> rank(n, initial_rank);
    vector<double> next_rank(n, 0.0);

    for (int iteration = 0;
         iteration < max_iterations;
         iteration++) {

        /*
         * Reset next iteration.
         */
        double base_rank =
            (1.0 - damping) / static_cast<double>(n);

        for (int v = 0; v < n; v++) {
            next_rank[v] = base_rank;
        }

        /*
         * Calculate total rank belonging to dangling vertices.
         */
        double dangling_rank = 0.0;

        for (int u = 0; u < n; u++) {
            int outdegree =
                graph.row_ptr[u + 1] - graph.row_ptr[u];

            if (outdegree == 0) {
                dangling_rank += rank[u];
            }
        }

        /*
         * Treat every dangling vertex as linking to
         * every vertex.
         */
        double dangling_share =
            dangling_rank / static_cast<double>(n);

        for (int v = 0; v < n; v++) {
            next_rank[v] += damping * dangling_share;
        }

        /*
         * Distribute rank through normal outgoing edges.
         *
         * The values in rank[] are from the previous iteration,
         * therefore all vertices are updated simultaneously.
         */
        for (int u = 0; u < n; u++) {
            int start = graph.row_ptr[u];
            int end = graph.row_ptr[u + 1];
            int outdegree = end - start;

            if (outdegree == 0) {
                continue;
            }

            double contribution =
                rank[u] / static_cast<double>(outdegree);

            for (int j = start; j < end; j++) {
                int v = graph.col_idx[j];

                next_rank[v] += damping * contribution;
            }
        }

        /*
         * Calculate total change.
         */
        double change = 0.0;

        for (int v = 0; v < n; v++) {
            change += fabs(next_rank[v] - rank[v]);
        }

        /*
         * Normalize to compensate for floating-point
         * accumulation error.
         */
        double rank_sum = 0.0;

        for (int v = 0; v < n; v++) {
            rank_sum += next_rank[v];
        }

        if (rank_sum != 0.0) {
            for (int v = 0; v < n; v++) {
                next_rank[v] /= rank_sum;
            }
        }

        /*
         * Move to the next iteration.
         */
        rank = next_rank;

        result.iterations = iteration + 1;

        if (change <= tolerance) {
            result.converged = true;
            break;
        }
    }

    /*
     * Store final ranks.
     */
    result.ranks = rank;

    result.sum_of_ranks = 0.0;

    for (int v = 0; v < n; v++) {
        result.sum_of_ranks += rank[v];
    }

    return result;
}