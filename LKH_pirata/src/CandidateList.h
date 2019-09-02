#ifndef CANDIDATELIST_H
#define CANDIDATELIST_H

#include "MinSpanningTree.h"
#include <algorithm>
#include <limits>
#include <vector>
#include <map>
#include <stack>

void computeBeta(std::vector<std::vector<double>> &beta, int V,std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList);

void computeAlpha(std::vector<std::vector<double>> &alpha, std::vector<std::vector<double>> &beta, vector<pair<int, int>> &edges, int V, std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList);

void generateCandidateList(std::vector<std::vector<int>> &rankedNodes, double **matrizAdj, int V);

#endif