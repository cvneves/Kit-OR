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

double **M; // matriz de adjacencia
int N;      // quantidade total de vertices

double calculaCustoAcumulado(std::vector<int> &s);
double calculaCustoSubsequencia(std::vector<int> &s);

int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
  double alpha = 0.5;
  double valor_obj;

  srand(time(NULL));

  readData(argc, argv, &N, &M);
  printData();

  std::vector<int> s, s1, s2;
  for (int i = 0; i < N; i++)
  {
    s.push_back(i + 1);
  }
  // s.push_back(1);

  for (int i = 0; i < 7; i++)
  {
    s1.push_back(i + 1);
    
  }
  for (int i = 7; i < N; i++)
  {
    s2.push_back(i + 1);
  }
  // s2.push_back(1);

  for(int i = 0; i < s1.size(); i++)
  {
    std::cout << s1[i] << " ";
  }
  std::cout << "\n";

  for(int i = 0; i < s2.size(); i++)
  {
    std::cout << s2[i] << " ";
  }
  std::cout << "\n";


  std::cout << calculaCustoAcumulado(s) << "\n";

  double T1 = calculaCustoSubsequencia(s1);
  double T2 = calculaCustoSubsequencia(s2);

  double C1 = calculaCustoAcumulado(s1);
  double C2 = calculaCustoAcumulado(s2);

  std::cout << C1 + (s2.size()) * (T1 + M[s1[6]][s2[0]]) + C2;

  return 0;
}

double calculaCustoSubsequencia(std::vector<int> &s)
{
  double custo = 0;
  for (int i = 0; i < s.size()-1; i++)
  {
    custo += M[s[i]][s[i + 1]];
  }
  return custo;
}

double calculaCustoAcumulado(std::vector<int> &s)
{
  double custo = 0;
  for (int i = 1; i < s.size(); i++)
  {
    for (int j = 0; j < i; j++)
    {
      std::cout << s[j] << " -> " << s[j + 1] << "\n";
      custo += M[s[j]][s[j + 1]];
    }
    std::cout << "\n";
  }

  return custo;
}

void printData()
{
  std::cout << "dimension: " << N << std::endl;
  for (size_t i = 1; i <= N; i++)
  {
    for (size_t j = 1; j <= N; j++)
    {
      std::cout << M[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
