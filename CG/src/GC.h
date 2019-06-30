#ifndef CG_H
#define CG_H

#include "Data.h"
#include <ilcplex/ilocplex.h>

void solve(Data &data,  std::vector<std::vector<int>> &patterns);

#endif