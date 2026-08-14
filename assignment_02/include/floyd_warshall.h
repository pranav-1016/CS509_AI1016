#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>
#include "../assignment_01/include/csr.h"

using namespace std;

struct Floyd_Warshall {
    vector<vector<long long>> distance;
    bool is_negative_cycle;
};

Floyd_Warshall floyd_warshall_distance(const vector<vector<long long>>& graph);

#endif