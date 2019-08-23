#include "readData.h"
#include "Kruskal.h"
#include "Prim.h"

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
void printSolucao(std::vector<int> &solucao);
double calcularValorObj(std::vector<int> &solucao, double **matriz_adj);

void transformarMatriz(std::vector<std::vector<double>> &C, std::vector<std::vector<double>> &D);
void calcularAlphaNearness(std::vector<std::vector<double>> &C, std::vector<std::vector<double>> &alpha, int N, Kruskal &kr);

int dimension;
double **matrizAdj;

int main(int argc, char **argv)
{
  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);
  printData();
  std::cout << "\n\n";

  double **graph = new double *[dimension];
  for (int i = 0; i < dimension; i++)
  {
    graph[i] = new double[dimension];
  }

  for (int i = 0; i < dimension; i++)
  {
    for (int j = 0; j < dimension; j++)
    {
      graph[i][j] = matrizAdj[i + 1][j + 1];
    }
  }

  primMST(graph, dimension);

  return 0;
}

void calcularAlphaNearness(std::vector<std::vector<double>> &C, std::vector<std::vector<double>> &alpha, int N, Kruskal &kr)
{
  for (auto &arco : kr.getEdges())
  {
    alpha[arco.first + 1][arco.second + 1] = alpha[arco.second + 1][arco.first + 1] = 0;
    std::cout << arco.first + 1 << " " << arco.second + 1 << "\n";
  }

  {
    std::pair<int, int> arco1, arco2;
    arco1 = kr.getEdges()[kr.getEdges().size() - 1];
    arco2 = kr.getEdges()[kr.getEdges().size() - 2];

    double cArestaRemovida = std::max(C[0][arco1.second], C[0][arco2.second]);

    for (int i = 1; i < dimension; i++)
    {
      if (i == arco1.second || i == arco2.second)
      {
        continue;
      }
      alpha[1][i + 1] = alpha[i + 1][1] = C[0][i] - cArestaRemovida;
      std::cout << 1 << " " << i + 1 << "\n";
    }
  }
}

void transformarMatriz(std::vector<std::vector<double>> &C, std::vector<std::vector<double>> &D)
{
  // Construçao
  std::vector<int> solucao(dimension + 1, 1);
  std::vector<int> lista_candidatos = std::vector<int>(dimension - 1);

  for (int i = 2; i <= dimension; i++)
  {
    lista_candidatos[i - 2] = i;
  }

  int n_cidades = lista_candidatos.size();

  double UB = 0;

  for (int i = 1; i < dimension; i++)
  {
    int n = rand() % lista_candidatos.size();
    solucao[i] = lista_candidatos[n];
    lista_candidatos.erase(lista_candidatos.begin() + n);
    UB += C[solucao[i - 1] - 1][solucao[i] - 1];
  }
  UB += C[solucao[dimension - 1] - 1][solucao[dimension] - 1];

  std::vector<double> lambda(dimension, 0);

  // gerar LB

  double e_k = 1;
  double lastZ = 0;
  double deltaZ;
  int p = 0;
  double e_k_lim = 1.0 / 16364.0;
  Kruskal kr(C);
  bool isFeasible = false, pruning;
  double LB;
  std::vector<std::pair<int, int>> s;
  std::vector<int> penalizacao;

  while (1)
  {
    for (int i = 0; i < dimension; i++)
    {
      for (int j = 0; j < dimension; j++)
      {
        D[i][j] = C[i][j] - lambda[i] - lambda[j];
      }
    }

    kr = Kruskal(D);

    LB = kr.MST(dimension);

    for (int i = 1; i < dimension; i++)
    {
      LB += 2 * lambda[i];
    }

    deltaZ = LB - lastZ;

    if (deltaZ <= std::numeric_limits<double>::epsilon())
    {
      p++;
    }
    if (p == 30)
    {
      e_k = e_k / 2;
      p = 0;
    }

    penalizacao = std::vector<int>(dimension, 2);

    isFeasible = true;

    double sumPenalizacoes = 0;

    for (int v = 0; v < dimension; v++)
    {
      for (const auto &arco : kr.getEdges())
      {
        if (arco.first == v || arco.second == v)
        {
          penalizacao[v]--;
        }
      }
      if (std::abs(penalizacao[v]) > 0.000001)
      {
        isFeasible = false;
      }
      sumPenalizacoes += penalizacao[v] * penalizacao[v];
    }

    if (isFeasible || std::abs(LB - UB) <= 0.000001)
    {
      pruning = true;
      s = kr.getEdges();
      break;
    }

    double stepSize = e_k * (UB - LB);

    stepSize = stepSize / sumPenalizacoes;

    for (int i = 0; i < dimension; i++)
    {
      lambda[i] += stepSize * penalizacao[i];
    }

    lastZ = LB;

    if (e_k < e_k_lim)
    {
      pruning = false;
      isFeasible = false;
      s = kr.getEdges();

      break;
    }
  }

  std::cout << LB << "\n";
}

void printSolucao(std::vector<int> &solucao)
{
  for (auto j : solucao)
  {
    std::cout << j << " ";
  }
  std::cout << "\n";
}

double calcularValorObj(std::vector<int> &solucao, double **matriz_adj)
{
  double sum = 0;
  for (int i = 0; i < solucao.size() - 1; i++)
  {
    sum += matriz_adj[solucao[i]][solucao[i + 1]];
  }
  return sum;
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