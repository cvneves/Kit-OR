#ifndef CG_H
#define CG_H

#include "Data.h"
#include <ilcplex/ilocplex.h>

#define EPSILON 0.000001

typedef struct node
{
    

} Node;

void solve(Data &data, std::vector<std::vector<int>> &patterns);

#endif