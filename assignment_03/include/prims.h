#ifndef PRIM_H
#define PRIM_H

// #include "csr_graph.h"
#include "csr.h"
#include "mst.h"
#include <tuple>
#include <vector>

MSTResult primMST(const CSRGraph& graph, int start_vertex = 0);

#endif