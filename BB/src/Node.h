#ifndef NODE_H
#define NODE_H

#include <vector>
#include "hungarian.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <limits>
#include "Tour.h"

typedef struct node
{
  std::vector<std::pair<int, int>> arcos_proibidos;
  double obj_value;
  bool pruning;
} Node;


 //Calcula soluçao do nó e retorna subtour escolhido
std::vector<int> calcularSolucao(hungarian_problem_t *p, Node &n);

// std::list<Node> branch(Node &root, hungarian_problem_t *p);

#endif