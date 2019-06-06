#ifndef MINIMUMCUT_H
#define MINIMUMCUT_H

#include "Graph.h"
#include <stdlib.h>
#include <list>

#define EPSILON 1e-8

void minimumCutPhase(Graph *G, int a,std::vector<bool> &deletedNodes, int &l, double &cutCost, std::vector<int> &A);

double minimumCut(Graph *G, int a, std::vector<bool> &S);

#endif