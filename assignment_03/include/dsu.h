#ifndef DSU_H
#define DSU_H

#include <vector>
using namespace std;
class DSU {
private:
    vector<int> parent;
    vector<int> rank;

public:
    explicit DSU(int n);

    int find(int x);
    bool unite(int a, int b);
    bool connected(int a, int b);
};

#endif