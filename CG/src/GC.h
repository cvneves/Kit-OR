#ifndef CG_H
#define CG_H

#include "Data.h"
#include <ilcplex/ilocplex.h>

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

public:
    Problema(Data &data);
    void solve();
    IloEnv env;
};

#endif