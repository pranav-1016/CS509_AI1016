#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "csr.h"
#include "mst.h"
#include <tuple>
#include <vector>

MSTResult kruskalMST(const CSRGraph& graph);

#endif