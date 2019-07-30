#include "readData.h"
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

double calculaCusto(std::vector<int> &s);

int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
  double alpha = 0.5;
  double valor_obj;

  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);
  printData();

  std::vector<int> s;
  for(int i = 0; i < dimension; i++)
  {
    s.push_back(i+1);
  }
  s.push_back(1);

  std::cout << "\n";
  std::cout << calculaCusto(s);

  return 0;
}

double calculaCusto(std::vector<int> &s)
{
  double custo = 0;
  for(int i = 1; i < s.size() - 1; i++)
  {
    for(int j = 0; j < i; j++)
    {
      std::cout << s[j] << " -> " << s[j+1] << "\n";
      custo += matrizAdj[s[j]][s[j+1]];
    }
    std::cout << "\n";
  }

  return custo;
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
