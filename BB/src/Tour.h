#ifndef TOUR_H
#define TOUR_H

#include <vector>
#include "hungarian.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include "Node.h"

std::vector<std::vector<int>> construirSubtours(int **m, const int &N);

void printSubtours(const std::vector<std::vector<int>> &tours);
void printSolucao(const std::vector<int> &s);

int selecionarSubtour(const std::vector<std::vector<int>> &subtours);



#endif