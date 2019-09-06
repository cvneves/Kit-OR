#include "readData.h"
#include "MinSpanningTree.h"
#include "CandidateList.h"
#include "SolutionUtils.h"
#include "Construction.h"

#include <fstream>
#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <cmath>
#include <limits>
#include <chrono>


using namespace std;

int dimension;
double **matrizAdj;

int main(int argc, char **argv)
{
  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);
  // printData(matrizAdj, dimension);

  vector<vector<int>> rankedNodes;
  generateCandidateList(rankedNodes, matrizAdj, dimension);

  std::vector<int> s = construction(0.5, dimension, matrizAdj);

  printSolucao(s);

  std::cout << calcularValorObj(s, matrizAdj);

  return 0;
}

