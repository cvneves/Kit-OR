#ifndef LAGRANGE_H
#define LAGRANGE_H

#include "readData.h"
#include <fstream>
#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <cmath>
#include <limits>
#include "Kruskal.h"
#include <list>

class Node
{
  public:
    std::vector<std::pair<int, int>> arcosProibidos, s;
    std::vector<double> lambda;
    std::vector<int> penalizacao;
    double LB;
    bool isFeasible, pruning;

    void calculateLB(int &, std::vector<std::vector<double>> &, const double &);
};

#endif
