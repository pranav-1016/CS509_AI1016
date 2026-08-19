#ifndef ASSIGNMENT_03_MST_H
#define ASSIGNMENT_03_MST_H

#include <tuple>
#include <vector>
using namespace std;

struct MSTResult {
    vector<tuple<int, int, long long>> edges;
    long long total_weight;
    bool connected;
};

#endif