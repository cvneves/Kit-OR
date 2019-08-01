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

void printSolution(std::vector<int> &s);

std::vector<int> construction(double alpha);

int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
  double alpha = 0.5;
  double valor_obj;

  srand(time(NULL));

  readData(argc, argv, &N, &M);
  printData();

  std::vector<int> s;
  // for (int i = 0; i < N; i++)
  // {
  //   s.push_back(i + 1);
  // }
  // s.push_back(1);

  s = construction(0.5);

  printSolution(s);

  std::cout << calculaCustoAcumulado(s);

  return 0;
}

std::vector<int> construction(double alpha)
{
  int r = 1;
  std::vector<int> s(N+1), CL(N - 1);
  std::vector<double> distances(N - 1);  

  s[0] = s[s.size()-1] = r;

  for (int i = 2; i <= N; i++)
  {
    CL[i - 2] = i;
  }

  int c = 1;
  while(!CL.empty())
  {
    for(int i = 0; i < CL.size(); i++)
    {
      distances[i] = M[r][CL[i]];
    }
    std::sort(distances.begin(), distances.end());

    int a = rand() % (int) std::ceil(alpha * CL.size());
    r = CL[a];
    s[c] = r;
    c++;

    CL.erase(CL.begin() + a);
    distances.erase(distances.begin());
  }

  return s;
}

double calculaCustoSubsequencia(std::vector<int> &s)
{
  double custo = 0;
  for (int i = 0; i < s.size() - 1; i++)
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
    if (s[i] == 1)
      continue;
    for (int j = 0; j < i; j++)
    {
      std::cout << s[j] << " -> " << s[j + 1] << "\n";
      custo += M[s[j]][s[j + 1]];
    }
    std::cout << "\n";
  }

  return custo;
}

void printSolution(std::vector<int> &s)
{
  for (int i = 0; i < s.size(); i++)
  {
    std::cout << s[i] << " ";
  }
  std::cout << "\n";
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
