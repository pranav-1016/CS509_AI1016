#ifndef VERTEX_COLORING_H
#define VERTEX_COLORING_H

#include "csr.h"
using namespace std;

struct VertexColoringResult {
    vector<int> colors;
    int colors_used;
    bool valid;
};

VertexColoringResult vertex_coloring(const CSRGraph& graph);

#endif