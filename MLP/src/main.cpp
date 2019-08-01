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
std::vector<std::vector<std::vector<int>>> reOptPreProcessing(std::vector<int> &s);

int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
  double alpha = 0.5;
  double valor_obj;

  srand(time(NULL));

  readData(argc, argv, &N, &M);
  printData();

  std::vector<int> s;
  s = construction(0.5);

  printSolution(s);

  std::cout << calculaCustoAcumulado(s) << "\n";

  reOptPreProcessing(s);

  return 0;
}

std::vector<int> construction(double alpha)
{
  int r = 1;
  std::vector<int> s(N + 1), CL(N - 1);
  std::vector<double> distances(N - 1);

  s[0] = s[s.size() - 1] = r;

  for (int i = 2; i <= N; i++)
  {
    CL[i - 2] = i;
  }

  int c = 1;
  while (!CL.empty())
  {
    for (int i = 0; i < CL.size(); i++)
    {
      distances[i] = M[r][CL[i]];
    }
    std::sort(distances.begin(), distances.end());

    int a = rand() % (int)std::floor(alpha * (CL.size() - 1) + 1);
    r = CL[a];
    s[c] = r;
    c++;

    CL.erase(CL.begin() + a);
    distances.erase(distances.begin());
  }

  return s;
}

std::vector<std::vector<std::vector<int>>> reOptPreProcessing(std::vector<int> &s)
{
  // T = 0, C = 1, W = 2
  std::vector<std::vector<std::vector<int>>> reOpt(3, std::vector<std::vector<int>>(N + 1, std::vector<int>(N + 1, 0)));

  for (int i = 0; i < N; i++)
  {
    reOpt[0][i][i] = 0;
    reOpt[1][i][i] = 0;
    if (s[i] == 1)
    {
      reOpt[2][i][i] = 0;
    }
    else
    {
      reOpt[2][i][i] = 1;
    }
  }

  for (int t = 2; t <= N + 1; t++)
  {
    std::cout << "\n";
    for (int i = 0; i < N - t + 2; i++)
    {
      reOpt[2][i][i + t - 1] = reOpt[2][i][i + t - 2] + reOpt[2][i + t - 1][i + t - 1];
      reOpt[0][i][i + t - 1] = reOpt[0][i][i + t - 2] + M[s[i + t - 2]][s[i + t - 1]] + reOpt[0][i + t - 1][i + t - 1];
      reOpt[1][i][i + t - 1] = reOpt[1][i][i + t - 2] + reOpt[2][i + t - 1][i + t - 1] * (reOpt[0][i][i + t - 2] + M[s[i + t - 2]][s[i + t - 1]]) + reOpt[1][i + t - 1][i + t - 1];
      std::cout << reOpt[1][i][i + t - 1] << " ";

      // for (int j = i; j < i + t; j++)
      // {
      //   std::cout << s[j] << " ";
      // }
      // std::cout << "\n";
    }
  }

  return reOpt;
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
      // std::cout << s[j] << " -> " << s[j + 1] << "\n";
      custo += M[s[j]][s[j + 1]];
    }
    // std::cout << "\n";
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
