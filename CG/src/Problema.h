#ifndef CG_H
#define CG_H

#include "Data.h"
#include "Node.h"
#include <ilcplex/ilocplex.h>
#include <cmath>
#include <limits>

#define EPSILON 0.000001

class Problema
{
    Data data;
    IloModel masterModel, pricingModel;
    IloNumVarArray lambda;
    IloBoolVarArray x;
    IloRangeArray masterRanges;
    IloObjective masterObj, pricingObj;
    IloCplex master, pricing;
    IloNumArray pi;

    std::vector<std::vector<int>> lambdaItens;
    std::vector<std::vector<double>> xPares;

public:
    Problema(Data &data);
    std::pair<int, int> solve(Node& node);
    IloEnv env1, env2;
};

#endif