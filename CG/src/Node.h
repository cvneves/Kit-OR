#ifndef NODE_H
#define NODE_H

#include "Data.h"
#include <ilcplex/ilocplex.h>
#include <vector>
#include <map>

#define EPSILON 0.000001

class Node
{
    std::vector<std::pair<int, int>> separados, juntos;
};


#endif