#ifndef CG_H
#define CG_H

#include "Data.h"
#include "Node.h"
#include <ilcplex/ilocplex.h>
#include <cmath>
#include <limits>
#include <algorithm>

#define EPSILON 1e-6
#define M 1e12

class Problema
{
    Data data;
    IloModel masterModel;
    IloNumVarArray lambda;
    IloRangeArray masterRanges;
    IloObjective masterObj;
    // IloCplex master;
    //IloNumArray pi;

    std::vector<std::vector<bool>> lambdaItens;
    std::vector<std::vector<double>> z;

  public:
    Problema(Data &data, double UB);
    std::pair<int, int> solve(Node &node);
    void prune();
    IloEnv env1;
    double bestInteger, UB, LB;
};

#endif