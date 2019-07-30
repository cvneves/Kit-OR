#include "readData.h"
#include "Solution.h"
#include <fstream>
#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <cmath>
#include <limits>
#include <chrono>

void printData();

double **matrizAdj; // matriz de adjacencia
int dimension;      // quantidade total de vertices

int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
  double alpha = 0.5;
  double valor_obj;

  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);
  printData();

  Solution s;
  s.N = dimension;
  s.M = matrizAdj;
  for (int i = 0; i < s.N; i++)
  {
    s.cycle.push_back(i + 1);
  }
  s.cycle.push_back(1);

  printSolution(s);

  std::cout << "\n" << calculaCusto(s);

  return 0;
}

void printData()
{
  std::cout << "dimension: " << dimension << std::endl;
  for (size_t i = 1; i <= dimension; i++)
  {
    for (size_t j = 1; j <= dimension; j++)
    {
      std::cout << matrizAdj[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
