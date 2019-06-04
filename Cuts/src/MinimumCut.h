#ifndef MINIMUMCUT_H
#define MINIMUMCUT_H

#include "Graph.h"
#include <stdlib.h>

#define EPSILON 1e-8

void minimumCutPhase(Graph *G, int a,std::vector<bool> &deletedNodes, int &l);

void minimumCut(Graph *G, int a);

#endif