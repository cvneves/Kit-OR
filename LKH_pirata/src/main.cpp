#include "readData.h"
#include "MinSpanningTree.h"
#include "CandidateList.h"
#include "SolutionUtils.h"
#include "Construction.h"
#include "linKOpt.h"

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

  vector<vector<int>> neighbourSet;
  generateCandidateList(neighbourSet, matrizAdj, dimension);

  std::vector<int> s = construction(0.5, dimension, matrizAdj);
  double objValue = calcularValorObj(s, matrizAdj);

  printSolucao(s);
  std::cout << objValue << "\n";

  lkStep(s, dimension, objValue, matrizAdj);

  return 0;
}

