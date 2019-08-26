#include "readData.h"
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

using namespace std;

int dimension;
double **matrizAdj;

typedef pair<int, double> ii;
typedef vector<vector<double>> vvi;
typedef vector<ii> vii;
typedef vector<int> vi;

vi taken;              // global boolean flag to avoid cycle
priority_queue<ii> pq; // priority queue to help choose shorter edges
                       // note: default setting for C++ STL priority_queue is a max heap

int V;
std::vector<std::vector<ii>> AdjList;

void process(int vtx)
{ // so, we use -ve sign to reverse the sort order
  taken[vtx] = 1;
  for (int j = 0; j < (int)AdjList[vtx].size(); j++)
  {
    ii v = AdjList[vtx][j];
    if (!taken[v.first])
      pq.push(ii(-v.second, -v.first));
  }
} // sort by (inc) weight then by (inc) id

int main(int argc, char **argv)
{
  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);
  printData();
  std::cout << "\n\n";

  V = dimension;

  std::chrono::time_point<std::chrono::system_clock> start, end;

  AdjList = std::vector<std::vector<ii>>(V, std::vector<ii>(V));

  for (int i = 0, c = 0; i < V; i++)
  {
    for (int j = 0; j < V; j++, c++)
    {
      AdjList[i][j].second = matrizAdj[i + 1][j + 1];
      AdjList[i][j].first = j;

      // std::cout << AdjList[i][j].first << " ";
    }
    // std::cout << "\n";
  }

  start = std::chrono::system_clock::now();
  vi parent;

  // inside int main()---assume the graph is stored in AdjList, pq is empty
  taken.assign(V, 0); // no vertex is taken at the beginning
  parent.assign(V, -1);
  process(0);         // take vertex 0 and process all edges incident to vertex 0
  int previous = 0;
  double mst_cost = 0;
  int u, w;
  while (!pq.empty())
  { // repeat until V vertices (E=V-1 edges) are taken
    ii front = pq.top();

    pq.pop();
    u = -front.second, w = -front.first; // negate the id and weight again

    if (!taken[u])
    {                            // we have not connected this vertex yet
      mst_cost += w, process(u); // take u, process all edges incident to u
      parent[u] = previous;
      previous = u;
    }
  } // each edge is in pq only once!

  end = std::chrono::system_clock::now();

  int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::cout << "Tempo total (s): " << elapsed_seconds / 1000.0 << "\n\n";
  std::cout << mst_cost << "\n";
  
  // for(int i = 0; i < parent.size(); i++)
  // {
  //   std::cout << i << " " << parent[i] << "\n";
  // }

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