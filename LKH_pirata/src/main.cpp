#include "readData.h"
#include "MinSpanningTree.h"

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

using namespace std;

int dimension;
double **matrizAdj;

vi taken;               // global boolean flag to avoid cycle
priority_queue<dii> pq; // priority queue to help choose shorter edges
                        // note: default setting for C++ STL priority_queue is a max heap

int V;
std::vector<std::vector<dii>> AdjList;

int main(int argc, char **argv)
{
  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);
  printData();
  std::cout << "\n\n";

  V = dimension;

  std::chrono::time_point<std::chrono::system_clock> start, end;

  AdjList = std::vector<std::vector<dii>>(V, std::vector<dii>(V));
  std::vector<std::vector<int>> rankedNodes;
  rankedNodes.assign(V, std::vector<int>());

  for (int i = 0, c = 0; i < V; i++)
  {
    for (int j = 0; j < V; j++, c++)
    {
      AdjList[i][j].first = matrizAdj[i + 1][j + 1];
      AdjList[i][j].second = {j, i};
    }
  }

  std::vector<std::vector<dii>> sortedAdjList = AdjList;

  for (int i = 0; i < V; i++)
  {
    std::sort(sortedAdjList[i].begin(), sortedAdjList[i].end());
    rankedNodes[i].assign(V, 0);
    for (int j = 0; j < V; j++)
    {
      rankedNodes[i][j] = sortedAdjList[i][j].second.first;
      // rankedNodes[i][j] = j;
   
    }
  }

  for (int i = 0; i < V; i++)
  {
    for (int j = 0; j < V; j++)
    {
      std::cout << rankedNodes[i][j] << " ";
    }
    std::cout << "\n";
  }

  std::cout << "\n\n";

  for (int i = 0, c = 0; i < V; i++)
  {
    for (int j = 0; j < V; j++, c++)
    {
      std::cout << AdjList[i][j].first << " ";
    }
    std::cout << "\n";
  }

  double tempomedio = 0;
  vii edges;
  edges.assign(V, {-3, -3});
  vi parent;

  for (int A = 0; A < 1; A++)
  {
    parent.assign(V, -1);

    start = std::chrono::system_clock::now();

    // std::cout << MS1T(V, AdjList, taken, parent, edges) << "\n";

    Ascent(V, AdjList, taken, parent, edges, rankedNodes);

    end = std::chrono::system_clock::now();
  }

  int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  for (int i = 0; i < edges.size(); i++)
  {
    std::cout << edges[i].first << " " << edges[i].second << "\n";
  }

  // for (int i = 0, c = 0; i < V; i++)
  // {
  //   for (int j = 0; j < V; j++, c++)
  //   {
  //     std::cout << AdjList[i][j].first << " ";
  //   }
  //   std::cout << "\n";
  // }

  std::cout << "Tempo total (s): " << elapsed_seconds / 1000.0 << "\n\n";

  return 0;
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