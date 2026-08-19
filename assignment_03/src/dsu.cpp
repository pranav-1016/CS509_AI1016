#include "../include/dsu.h"
#include <utility>

using namespace std;
using namespace std;

DSU::DSU(int n) {
    parent.resize(n);
    rank.resize(n, 0);

    for (int i = 0; i < n; ++i)
        parent[i] = i;
}

int DSU::find(int x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);

    return parent[x];
}

bool DSU::unite(int a, int b) {
    int root_a = find(a);
    int root_b = find(b);

    if (root_a == root_b)
        return false;

    if (rank[root_a] < rank[root_b])
        swap(root_a, root_b);

    parent[root_b] = root_a;

    if (rank[root_a] == rank[root_b])
        ++rank[root_a];

    return true;
}

bool DSU::connected(int a, int b) {
    return find(a) == find(b);
}