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
double calculaCustoSubsequencia(std::vector<int> &s, int i, int j);

void printSolution(std::vector<int> &s);

void reinsertion(std::vector<int> &solucao, int i, int tamanho_subsequencia, int j);
void swap(std::vector<int> &solucao, int i, int j);
void twoOpt(std::vector<int> &solucao, int i, int j);

void buscaVizinhanca2Opt(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> ReOpt, double &valor_obj);

std::vector<int> construction(double alpha);
std::vector<std::vector<std::vector<double>>> reOptPreProcessing(std::vector<int> &s);

int main(int argc, char **argv)
{
  std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
  double alpha = 0.5;

  srand(time(NULL));

  readData(argc, argv, &N, &M);
  printData();

  std::vector<int> s;
  s = construction(0.5);

  std::cout << "\n";

  printSolution(s);
  std::cout << '\n'
            << calculaCustoAcumulado(s) << '\n';
  std::vector<std::vector<std::vector<double>>> reOpt = reOptPreProcessing(s);
  double valor_obj = reOpt[1][0][N];

  // std::cout << valor_obj << '\n';
  buscaVizinhanca2Opt(s, reOpt, valor_obj);

  twoOpt(s, 0, N);

  printSolution(s);
  std::cout << reOpt[1][N][0] << "\n";
  std::cout << calculaCustoAcumulado(s) << "\n";

  // std::cout << calculaCustoSubsequencia(s, 0, 1);

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

std::vector<std::vector<std::vector<double>>> reOptPreProcessing(std::vector<int> &s)
{
  // T = 0, C = 1, W = 2
  std::vector<std::vector<std::vector<double>>> reOpt(3, std::vector<std::vector<double>>(N + 1, std::vector<double>(N + 1, 0)));

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
    for (int i = 0; i < N - t + 2; i++)
    {
      reOpt[2][i][i + t - 1] = reOpt[2][i][i + t - 2] + reOpt[2][i + t - 1][i + t - 1];
      reOpt[0][i][i + t - 1] = reOpt[0][i][i + t - 2] + M[s[i + t - 2]][s[i + t - 1]];
      reOpt[1][i][i + t - 1] = reOpt[1][i][i + t - 2] + reOpt[2][i + t - 1][i + t - 1] * (reOpt[0][i][i + t - 2] + M[s[i + t - 2]][s[i + t - 1]]) + reOpt[1][i + t - 1][i + t - 1];

      reOpt[2][i + t - 1][i] = reOpt[2][i][i + t - 1];
      reOpt[0][i + t - 1][i] = reOpt[0][i][i + t - 1];

      reOpt[1][i + t - 1][i] = reOpt[1][i + t - 2][i] + reOpt[2][i + t - 2][i] * (reOpt[0][i + t - 1][i + t - 1] + M[s[i + t - 1]][s[i + t - 2]]) + reOpt[1][i + t - 1][i + t - 1];
    }
  }

  return reOpt;
}

double calculaCustoSubsequencia(std::vector<int> &s, int i, int j)
{
  double custo = 0;
  for (int u = i; u < j; u++)
  {
    custo += M[s[u]][s[u + 1]];
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

void twoOpt(std::vector<int> &solucao, int i, int j)
{
  std::reverse(solucao.begin() + i, solucao.begin() + j + 1);
}

void reinsertion(std::vector<int> &solucao, int i, int tamanho_subsequencia, int j)
{
  int vertex = solucao[i];

  std::vector<int> subsequencia(solucao.begin() + i, solucao.begin() + i + tamanho_subsequencia);

  solucao.erase(solucao.begin() + i, solucao.begin() + i + tamanho_subsequencia);
  solucao.insert(solucao.begin() + j, subsequencia.begin(), subsequencia.end());
}

void swap(std::vector<int> &solucao, int i, int j)
{
  std::swap(solucao[i], solucao[j]);
}

void buscaVizinhanca2Opt(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> reOpt, double &valor_obj)
{
  double T, C, W, melhor_valor_obj = std::numeric_limits<double>::infinity(), temp_obj;
  int melhor_i, melhor_j;
  bool improved = false;
  for (int i = 1; i < s.size() - 2; i++)
  {
    for (int j = i + 2; j < s.size() - 1; j++)
    {
      W = reOpt[2][0][i] + reOpt[2][j][j];
      T = reOpt[0][0][i] + M[s[i]][s[j]] + reOpt[0][j][j];
      // C = reOpt[1][0][i] + reOpt[2][0][i] * (reOpt[0][i][i] + M[i - 1][i]) + reOpt[1][i][i];

      // std::cout << i << " " << j << "\n";
      // swap(s, i, j);
      // std::cout << calculaCustoSubsequencia(s, 0, i) << "\n";
      // swap(s, i, j);

      // std::cout << T << "\n";
    }
  }

  // if (improved == true)
  // {
  //   valor_obj = melhor_valor_obj;
  //   swap(s, melhor_i, melhor_j);
  // }
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
