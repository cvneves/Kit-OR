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

#define EPSILON 1e-6

void printData();

double **M; // matriz de adjacencia
int N;      // quantidade total de vertices

double calculaCustoAcumulado(std::vector<int> &s);
double calculaCustoSubsequencia(std::vector<int> &s, int i, int j);

void printSolution(std::vector<int> &s);

void reinsertion(std::vector<int> &solucao, int i, int tamanho_subsequencia, int j);
void swap(std::vector<int> &solucao, int i, int j);
void twoOpt(std::vector<int> &solucao, int i, int j);

void buscaVizinhanca2Opt(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj);
void buscaVizinhancaReinsertion(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj, int t);
void buscaVizinhancaSwap(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj);

std::vector<int> construction(double alpha);
void reOptPreProcessing(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt);

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
  std::cout << "\n\n";

  std::vector<std::vector<std::vector<double>>> reOpt(3, std::vector<std::vector<double>>(N + 1, std::vector<double>(N + 1, 0)));
  reOptPreProcessing(s, reOpt);
  double valor_obj = reOpt[1][0][N];

  // buscaVizinhancaReinsertion(s, reOpt, valor_obj, 3);
  // buscaVizinhanca2Opt(s, reOpt, valor_obj);
  buscaVizinhancaSwap(s, reOpt, valor_obj);

  printSolution(s);

  // std::cout << reOpt[1][0][N] << "\n";
  // std::cout << calculaCustoSubsequencia(s, 0, N) << "\n";
  std::cout << calculaCustoAcumulado(s) << "\n";
  std::cout << valor_obj << "\n";
  std::cout << reOpt[1][0][N];
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

void reOptPreProcessing(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt)
{
  // T = 0, C = 1, W = 2
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
    for (int i = 0, j; i < N - t + 2; i++)
    {
      j = i + t - 1;
      reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
      reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
      reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

      reOpt[2][j][i] = reOpt[2][i][j];
      reOpt[0][j][i] = reOpt[0][i][j];

      reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
    }
  }
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
      custo += M[s[j]][s[j + 1]];
    }
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

void buscaVizinhanca2Opt(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj)
{
  double T, C, W, melhor_valor_obj = valor_obj, temp_obj;
  double T1, C1, W1, T2, C2, W2, T3, C3, W3;
  int melhor_i, melhor_j;
  bool improved = false;
  for (int i = 1; i < s.size() - 2; i++)
  {
    for (int j = i + 1; j < s.size() - 1; j++)
    {
      W1 = reOpt[2][j][i];
      W2 = reOpt[2][j + 1][N];

      T1 = reOpt[0][0][i - 1];
      T2 = reOpt[0][0][i - 1] + M[s[i - 1]][s[j]] + reOpt[0][j][i];

      C = reOpt[1][0][i - 1] + W1 * (T1 + M[s[i - 1]][s[j]]) + reOpt[1][j][i] + W2 * (T2 + M[s[i]][s[j + 1]]) + reOpt[1][j + 1][N];

      if (C + EPSILON < melhor_valor_obj)
      {
        improved = true;
        melhor_valor_obj = C;
        melhor_i = i;
        melhor_j = j;
      }
    }
  }

  if (improved == true)
  {
    valor_obj = melhor_valor_obj;
    twoOpt(s, melhor_i, melhor_j);

    for (int j = melhor_i; j <= melhor_j; j++)
    {
      for (int i = j - 1; i >= 0; i--)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = melhor_i; i <= melhor_j; i++)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = melhor_i - 1, k = melhor_i, l; i >= 0; i--)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {

        reOpt[2][i][j] = reOpt[2][i][k] + reOpt[2][k + 1][j];
        reOpt[0][i][j] = reOpt[0][i][k] + M[s[k]][s[k + 1]] + reOpt[0][k + 1][j];
        reOpt[1][i][j] = reOpt[1][i][k] + reOpt[2][k + 1][j] * (reOpt[0][i][k] + M[s[k]][s[k + 1]]) + reOpt[1][k + 1][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][k][i] + reOpt[2][k][i] * (reOpt[0][j][k + 1] + M[s[k + 1]][s[k]]) + reOpt[1][j][k + 1];
      }
    }
  }
}

void buscaVizinhancaReinsertion(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj, int t)
{
  double T, C, W, melhor_valor_obj = valor_obj, temp_obj;
  double T1, C1, W1, T2, C2, W2, T3, C3, W3;
  int melhor_i, melhor_j;
  bool improved = false;
  int i1, i2, j1, j2;
  for (int i = 1; i < N + 1 - t; i++)
  {
    for (int j = 1; j < N + 1 - t; j++)
    {
      if (i == j)
        continue;
      if (i < j)
      {
        i1 = 0, j1 = i - 1;
        i2 = i + t, j2 = j + t - 1;

        W1 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T1 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C1 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        i1 = i, j1 = i + t - 1;
        i2 = j + t, j2 = N;

        W2 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T2 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C2 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        C = C1 + W2 * (T1 + M[s[j + t - 1]][s[i]]) + C2;
      }
      else
      {
        i1 = 0, j1 = j - 1;
        i2 = i, j2 = i + t - 1;

        W1 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T1 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C1 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        i1 = j, j1 = i - 1;
        i2 = i + t, j2 = N;

        W2 = reOpt[2][i1][j1] + reOpt[2][i2][j2];
        T2 = reOpt[0][i1][j1] + M[s[j1]][s[i2]] + reOpt[0][i2][j2];
        C2 = reOpt[1][i1][j1] + reOpt[2][i2][j2] * (reOpt[0][i1][j1] + M[s[j1]][s[i2]]) + reOpt[1][i2][j2];

        C = C1 + W2 * (T1 + M[s[i + t - 1]][s[j]]) + C2;
      }
      if (C + EPSILON < melhor_valor_obj)
      {
        improved = true;
        melhor_valor_obj = C;
        melhor_i = i;
        melhor_j = j;
      }
    }
  }
  if (improved == true)
  {
    valor_obj = melhor_valor_obj;

    int i1, i2, i3, i4;
    int j1, j2, j3, j4;

    if (melhor_i < melhor_j)
    {
      i1 = 0, j1 = melhor_i - 1;
      i2 = melhor_i + t, j2 = melhor_j + t - 1;
      i3 = melhor_i, j3 = melhor_i + t - 1;
      i4 = melhor_j + t, j4 = N;
    }
    else
    {
      i1 = 0, j1 = melhor_j - 1;
      i2 = melhor_i, j2 = melhor_i + t - 1;
      i3 = melhor_j, j3 = melhor_i - 1;
      i4 = melhor_i + t, j4 = N;
    }

    reinsertion(s, melhor_i, t, melhor_j);

    for (int j = j1 + 1; j <= i4 - 1; j++)
    {
      for (int i = j - 1; i >= 0; i--)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = j1 + 1; i <= i4 - 1; i++)
    {
      for (int j = i4 - 1 + 1; j <= N; j++)
      {
        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
    }

    for (int i = j1 + 1 - 1, k = j1 + 1, l; i >= 0; i--)
    {
      for (int j = i4 - 1 + 1; j <= N; j++)
      {

        reOpt[2][i][j] = reOpt[2][i][k] + reOpt[2][k + 1][j];
        reOpt[0][i][j] = reOpt[0][i][k] + M[s[k]][s[k + 1]] + reOpt[0][k + 1][j];
        reOpt[1][i][j] = reOpt[1][i][k] + reOpt[2][k + 1][j] * (reOpt[0][i][k] + M[s[k]][s[k + 1]]) + reOpt[1][k + 1][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][k][i] + reOpt[2][k][i] * (reOpt[0][j][k + 1] + M[s[k + 1]][s[k]]) + reOpt[1][j][k + 1];
      }
    }
  }
}

void buscaVizinhancaSwap(std::vector<int> &s, std::vector<std::vector<std::vector<double>>> &reOpt, double &valor_obj)
{
  double T, C, W, melhor_valor_obj = valor_obj, temp_obj;
  double T1, C1, W1, T2, C2, W2, T3, C3, W3;
  int melhor_i, melhor_j;
  bool improved = false;
  for (int i = 1; i < s.size() - 2; i++)
  {
    for (int j = i + 2; j < s.size() - 1; j++)
    {
      W1 = reOpt[2][0][i - 1] + reOpt[2][j][j];
      W3 = reOpt[2][i][i] + reOpt[2][j + 1][N];

      T1 = reOpt[0][0][i - 1] + M[s[i - 1]][s[j]] + reOpt[0][j][j];
      T3 = reOpt[0][i][i] + M[s[i]][s[j + 1]] + reOpt[0][j + 1][N];

      C1 = reOpt[1][0][i - 1] + reOpt[2][j][j] * (reOpt[0][0][i - 1] + M[s[i - 1]][s[j]]) + reOpt[1][j][j];
      C3 = reOpt[1][i][i] + reOpt[2][j + 1][N] * (reOpt[0][i][i] + M[s[i]][s[j + 1]]) + reOpt[1][j + 1][N];

      W2 = reOpt[2][i + 1][j - 1];
      T2 = reOpt[0][i + 1][j - 1];
      C2 = reOpt[1][i + 1][j - 1];

      C = (C1 + W2 * (T1 + M[s[j]][s[i + 1]]) + C2) + W3 * (T1 + M[s[j]][s[i + 1]] + T2 + M[s[j - 1]][s[i]]) + C3;

      if (C + EPSILON < melhor_valor_obj)
      {
        improved = true;
        melhor_valor_obj = C;
        melhor_i = i;
        melhor_j = j;
      }
    }
  }

  if (improved == true)
  {
    valor_obj = melhor_valor_obj;
    swap(s, melhor_i, melhor_j);

    for (int j = melhor_i; j <= melhor_j; j++)
    {
      for (int i = j - 1; i >= 0; i--)
      {
        std::cout << i << " " << j << "\n";

        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];
      }
      std::cout << "\n";
    }

    for (int i = melhor_i; i <= melhor_j; i++)
    {
      for (int j = melhor_j + 1; j <= N; j++)
      {
        std::cout << i << " " << j << "\n";

        reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
        reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
        reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

        reOpt[2][j][i] = reOpt[2][i][j];
        reOpt[0][j][i] = reOpt[0][i][j];

        reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];

        // std::cout << "\n";
      }
    }

    for (int i = 0, j = melhor_j + 1; j <= N; j++)
    {
      std::cout << i << " " << j << "\n";

      reOpt[2][i][j] = reOpt[2][i][j - 1] + reOpt[2][j][j];
      reOpt[0][i][j] = reOpt[0][i][j - 1] + M[s[j - 1]][s[j]];
      reOpt[1][i][j] = reOpt[1][i][j - 1] + reOpt[2][j][j] * (reOpt[0][i][j - 1] + M[s[j - 1]][s[j]]) + reOpt[1][j][j];

      reOpt[2][j][i] = reOpt[2][i][j];
      reOpt[0][j][i] = reOpt[0][i][j];

      reOpt[1][j][i] = reOpt[1][j - 1][i] + reOpt[2][j - 1][i] * (reOpt[0][j][j] + M[s[j]][s[j - 1]]) + reOpt[1][j][j];

      // std::cout << "\n";
    }

    // reOptPreProcessing(s, reOpt);
  }
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
