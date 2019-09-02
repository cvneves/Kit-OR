#include "readData.h"
#include "MinSpanningTree.h"
#include "CandidateList.h"

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

// GILS-RVND

void inicializacao(int dimension, int tamanho_sub_rota, std::vector<int> &solucao, std::vector<int> &lista_candidatos);
void computarCustoInsercao(double **matriz_adj, std::vector<int> &solucao, std::vector<int> &lista_candidatos, std::vector<std::pair<std::pair<int, int>, double>> &custo_insercao);
bool comparacaoCusto(std::pair<std::pair<int, int>, double> a, std::pair<std::pair<int, int>, double> b);
double calcularValorObj(std::vector<int> &solucao, double **matriz_adj);

void printSolucao(std::vector<int> &solucao);

void construcao(std::vector<int> &solucao, std::vector<int> &lista_candidatos, int tamanho_sub_rota, std::vector<std::pair<std::pair<int, int>, double>> &custo_insercao, double &valor_obj, double alpha, double **matriz_adj, int dimension);
void RVND(std::vector<int> &solucao, double &valor_obj, double **m);

std::vector<int> GILS_RVND();

void reinsertion(std::vector<int> &solucao, int i, int tamanho_subsequencia, int j);
void swap(std::vector<int> &solucao, int i, int j);
void twoOpt(std::vector<int> &solucao, int i, int j);

void buscaVizinhancaSwap(std::vector<int> &solucao, double &valor_obj, double **matriz_adj);
void buscaVizinhanca2Opt(std::vector<int> &solucao, double &valor_obj, double **matriz_adj);
void buscaVizinhancaReinsertion(std::vector<int> &solucao, double &valor_obj, double **m, int tam_subsequencia);

std::chrono::time_point<std::chrono::system_clock> start, end, temp1, temp2;
std::vector<double> tempos(6, 0);

int main(int argc, char **argv)
{
  srand(time(NULL));

  readData(argc, argv, &dimension, &matrizAdj);

  vector<vector<int>> rankedNodes;
  generateCandidateList(rankedNodes, matrizAdj, dimension);

  // V = dimension;

  // std::chrono::time_point<std::chrono::system_clock> start, end;

  // AdjList = std::vector<std::vector<dii>>(V, std::vector<dii>(V));
  // std::vector<std::vector<int>> rankedNodes;
  // rankedNodes.assign(V, std::vector<int>());

  // for (int i = 0, c = 0; i < V; i++)
  // {
  //   for (int j = 0; j < V; j++, c++)
  //   {
  //     AdjList[i][j].first = matrizAdj[i + 1][j + 1];
  //     AdjList[i][j].second = {j, i};
  //   }
  // }

  // std::vector<std::vector<dii>> sortedAdjList = AdjList;

  // for (int i = 0; i < V; i++)
  // {
  //   std::sort(sortedAdjList[i].begin(), sortedAdjList[i].end());
  //   rankedNodes[i].assign(V, 0);
  //   for (int j = 0; j < V; j++)
  //   {
  //     rankedNodes[i][j] = sortedAdjList[i][j].second.first;
  //     // rankedNodes[i][j] = j;
  //   }
  // }

  // double tempomedio = 0;
  // vii edges;
  // edges.assign(V - 1, {-3, -3});
  // vi parent;

  // for (int A = 0; A < 1; A++)
  // {
  //   parent.assign(V, -1);

  //   start = std::chrono::system_clock::now();

  //   // std::cout << MS1T(V, AdjList, taken, parent, edges) << "\n";

  //   Ascent(V, AdjList, taken, parent, edges, rankedNodes);

  //   end = std::chrono::system_clock::now();
  // }

  // std::cout << "MST cost " << MST(V, AdjList, taken, parent, edges, false, rankedNodes) << "\n";

  // int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  // cout << "Edges\n";
  // for (int i = 0; i < edges.size(); i++)
  // {
  //   std::cout << edges[i].first << " " << edges[i].second << ": " << AdjList[edges[i].first][edges[i].second].first << "\n";
  // }
  // cout << "Parents\n";
  // for (int i = 0; i < parent.size(); i++)
  // {
  //   std::cout << i << " " << parent[i] << "\n";
  // }

  // for (int i = 0, c = 0; i < V; i++)
  // {
  //   for (int j = 0; j < V; j++, c++)
  //   {
  //     std::cout << AdjList[i][j].first << " ";
  //   }
  //   std::cout << "\n";
  // }

  // std::vector<std::vector<double>> matrizAdj2(V + 1, std::vector<double>(V + 1));

  // for (int i = 0, c = 0; i < V; i++)
  // {
  //   for (int j = 0; j < V; j++, c++)
  //   {
  //     matrizAdj2[i + 1][j + 1] = matrizAdj[i + 1][j + 1];
  //     matrizAdj[i + 1][j + 1] = AdjList[i][j].first;
  //   }
  // }

  // std::vector<int> solucao = GILS_RVND();

  // for (int i = 0, c = 0; i < V; i++)
  // {
  //   for (int j = 0; j < V; j++, c++)
  //   {
  //     matrizAdj[i + 1][j + 1] = matrizAdj2[i + 1][j + 1];
  //   }
  // }

  // std::cout << "Tempo total (s): " << elapsed_seconds / 1000.0 << "\n";
  // std::cout << "Valor obj: " << calcularValorObj(solucao, matrizAdj) << "\n";

  // printSolucao(solucao);

  // std::cout << "\n\n";
  // std::vector<std::vector<double>> alpha, beta;
  // computeAlpha(alpha, beta, edges, V, parent, AdjList);

  cout << "\n";

  for(int i = 0; i < rankedNodes.size(); i++)
  {
    for(int j = 0; j < rankedNodes[i].size(); j++)
    {
      // std::cout << rankedNodes[i][j] << " ";
    }
    // std::cout << "\n";
  }

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

//GILS-RVND

void perturb(std::vector<int> &s)
{
  int n_min = 2, n_max = std::ceil(s.size() / 10.0);
  n_max = n_max >= n_min ? n_max : n_min;

  int t1 = n_min == n_max ? n_min : rand() % (n_max - n_min) + n_min;
  int i = rand() % (s.size() - 1 - t1) + 1;

  int j, t2;

  if (i < 1 + n_min)
  {
    j = rand() % ((s.size() - n_min - 1) - (i + t1) + 1) + (i + t1);
    t2 = rand() % (std::min((int)(s.size() - j - 1), n_max) - n_min + 1) + n_min;
  }
  else if (i + t1 >= s.size() - n_min)
  {
    j = rand() % (i - n_min) + 1;
    t2 = rand() % (std::min((i - j), n_max) - n_min + 1) + n_min;
  }
  else
  {
    if (rand() % 2 == 1)
    {
      j = rand() % ((s.size() - n_min - 1) - (i + t1) + 1) + (i + t1);
      t2 = rand() % (std::min((int)(s.size() - j - 1), n_max) - n_min + 1) + n_min;
    }
    else
    {
      j = rand() % (i - n_min) + 1;
      t2 = rand() % (std::min((i - j), n_max) - n_min + 1) + n_min;
    }
  }

  std::vector<int> subsequencia_i(s.begin() + i, s.begin() + i + t1);
  std::vector<int> subsequencia_j(s.begin() + j, s.begin() + j + t2);

  if (i < j)
  {
    s.erase(s.begin() + j, s.begin() + j + t2);
    s.insert(s.begin() + j, subsequencia_i.begin(), subsequencia_i.end());
    s.erase(s.begin() + i, s.begin() + i + t1);
    s.insert(s.begin() + i, subsequencia_j.begin(), subsequencia_j.end());
  }
  else
  {
    s.erase(s.begin() + i, s.begin() + i + t1);
    s.insert(s.begin() + i, subsequencia_j.begin(), subsequencia_j.end());
    s.erase(s.begin() + j, s.begin() + j + t2);
    s.insert(s.begin() + j, subsequencia_i.begin(), subsequencia_i.end());
  }
}

void RVND(std::vector<int> &solucao, double &valor_obj, double **m)
{
  std::vector<int> NL = {1, 2, 3, 4, 5};
  int n;

  std::vector<int> nova_solucao = solucao;
  double novo_valor_obj = valor_obj;

  int i = 0;
  while (NL.empty() == false)
  {
    n = rand() % NL.size();
    switch (NL[n])
    {
    case 1:
      temp1 = std::chrono::system_clock::now();
      buscaVizinhancaSwap(nova_solucao, novo_valor_obj, m);
      temp2 = std::chrono::system_clock::now();
      tempos[0] += std::chrono::duration_cast<std::chrono::microseconds>(temp2 - temp1).count();
      break;
    case 2:
    {
      temp1 = std::chrono::system_clock::now();
      buscaVizinhanca2Opt(nova_solucao, novo_valor_obj, m);
      temp2 = std::chrono::system_clock::now();
      tempos[1] += std::chrono::duration_cast<std::chrono::microseconds>(temp2 - temp1).count();
      break;
    }
    case 3:
      temp1 = std::chrono::system_clock::now();
      buscaVizinhancaReinsertion(nova_solucao, novo_valor_obj, m, 1);
      temp2 = std::chrono::system_clock::now();
      tempos[2] += std::chrono::duration_cast<std::chrono::microseconds>(temp2 - temp1).count();
      break;
    case 4:
      temp1 = std::chrono::system_clock::now();
      buscaVizinhancaReinsertion(nova_solucao, novo_valor_obj, m, 2);
      temp2 = std::chrono::system_clock::now();
      tempos[3] += std::chrono::duration_cast<std::chrono::microseconds>(temp2 - temp1).count();
      break;
    case 5:
    {
      temp1 = std::chrono::system_clock::now();
      buscaVizinhancaReinsertion(nova_solucao, novo_valor_obj, m, 3);
      temp2 = std::chrono::system_clock::now();
      tempos[4] += std::chrono::duration_cast<std::chrono::microseconds>(temp2 - temp1).count();
      break;
    }
    }

    if (novo_valor_obj < valor_obj)
    {
      valor_obj = novo_valor_obj;
      solucao = nova_solucao;
    }
    else
    {
      NL.erase(NL.begin() + n);
      nova_solucao = solucao;
      novo_valor_obj = valor_obj;
    }
  }
}

void construcao(std::vector<int> &solucao, std::vector<int> &lista_candidatos, int tamanho_sub_rota, std::vector<std::pair<std::pair<int, int>, double>> &custo_insercao, double &valor_obj, double alpha, double **matriz_adj, int dimension)
{
  inicializacao(dimension, tamanho_sub_rota, solucao, lista_candidatos);

  while (lista_candidatos.empty() == false)
  {
    computarCustoInsercao(matriz_adj, solucao, lista_candidatos, custo_insercao);
    sort(custo_insercao.begin(), custo_insercao.end(), comparacaoCusto);

    int n = rand() % ((int)std::floor(alpha * (custo_insercao.size() - 1)) + 1);
    int k = custo_insercao[n].first.first;
    int i = custo_insercao[n].first.second;

    solucao.insert(solucao.begin() + i, k);
    lista_candidatos.erase(find(lista_candidatos.begin(), lista_candidatos.end(), k));
  }

  valor_obj = calcularValorObj(solucao, matriz_adj);
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

void inicializacao(int dimension, int tamanho_sub_rota, std::vector<int> &solucao, std::vector<int> &lista_candidatos)
{

  solucao = {1, 1};
  lista_candidatos = std::vector<int>(dimension - 1);

  for (int i = 2; i <= dimension; i++)
  {
    lista_candidatos[i - 2] = i;
  }

  int n_cidades = lista_candidatos.size();

  for (int i = 0; i < tamanho_sub_rota; i++)
  {
    int n = rand() % lista_candidatos.size();
    solucao.insert(solucao.begin() + 1, lista_candidatos[n]);
    lista_candidatos.erase(lista_candidatos.begin() + n);
  }
}

void computarCustoInsercao(double **matriz_adj, std::vector<int> &solucao, std::vector<int> &lista_candidatos, std::vector<std::pair<std::pair<int, int>, double>> &custo_insercao)
{
  custo_insercao = std::vector<std::pair<std::pair<int, int>, double>>((solucao.size() - 1) * lista_candidatos.size());

  for (int i = 0, c = 0, j = 1; i < solucao.size() - 1; i++, j++)
  {
    for (int k = 0; k < lista_candidatos.size(); k++, c++)
    {
      custo_insercao[c].first.first = lista_candidatos[k];
      custo_insercao[c].first.second = j;
      custo_insercao[c].second = -matriz_adj[solucao[i]][solucao[j]] + matriz_adj[solucao[i]][lista_candidatos[k]] + matriz_adj[solucao[j]][lista_candidatos[k]];
    }
  }
}

bool comparacaoCusto(std::pair<std::pair<int, int>, double> a, std::pair<std::pair<int, int>, double> b)
{
  return a.second < b.second;
}

void buscaVizinhancaSwap(std::vector<int> &s, double &valor_obj, double **m)
{
  double novo_valor_obj, perda, ganho, delta, melhor_valor_obj = std::numeric_limits<double>::infinity();
  int melhor_i, melhor_j;
  bool improved = false;
  for (int i = 1; i < s.size() - 2; i++)
  {
    double perda_i = m[s[i]][s[i + 1]] + m[s[i]][s[i - 1]];

    for (int j = i + 1; j < s.size() - 1; j++)
    {
      if (j != i + 1)
      {
        delta = m[s[j]][s[i - 1]] + m[s[j]][s[i + 1]] + m[s[i]][s[j - 1]] + m[s[i]][s[j + 1]] - perda_i - m[s[j]][s[j - 1]] - m[s[j]][s[j + 1]];
      }
      else
      {
        delta = m[s[j]][s[i - 1]] + m[s[i]][s[j + 1]] - m[s[i]][s[i - 1]] - m[s[j]][s[j + 1]];
      }

      if (delta < 0)
      {
        if (valor_obj + delta < melhor_valor_obj - std::numeric_limits<double>::epsilon())
        {
          melhor_valor_obj = valor_obj + delta;
          melhor_i = i;
          melhor_j = j;
          improved = true;
        }
      }
    }
  }

  if (improved == true)
  {
    valor_obj = melhor_valor_obj;
    swap(s, melhor_i, melhor_j);
  }
}

void buscaVizinhancaReinsertion(std::vector<int> &solucao, double &valor_obj, double **m, int tam_subsequencia)
{
  double novo_valor_obj, perda, ganho, delta, melhor_valor_obj = std::numeric_limits<double>::infinity();
  int melhor_i, melhor_j;
  bool improved = false;

  auto rit = solucao.rbegin();
  int j_bk, i_bk;

  for (int i = 1; i < solucao.size() - tam_subsequencia; i++)
  {
    double perda_1 = +m[solucao[i - 1]][solucao[i + tam_subsequencia]] - m[solucao[i - 1]][solucao[i]] - m[solucao[i + tam_subsequencia - 1]][solucao[i + tam_subsequencia]];

    for (int j = 1; j < solucao.size() - tam_subsequencia; j++)
    {
      if (j == i)
        continue;

      if (i > j)
      {
        delta = m[solucao[i]][solucao[j - 1]] + m[solucao[i + tam_subsequencia - 1]][solucao[j]] - m[solucao[j]][solucao[j - 1]] + perda_1;
      }
      else
      {
        j_bk = j;
        i_bk = i;

        i = solucao.size() - i - tam_subsequencia;
        j = solucao.size() - j - tam_subsequencia;

        delta = m[*(rit + i)][*(rit + j - 1)] + m[*(rit + i + tam_subsequencia - 1)][*(rit + j)] + m[*(rit + i - 1)][*(rit + i + tam_subsequencia)] - m[*(rit + i - 1)][*(rit + i)] - m[*(rit + i + tam_subsequencia - 1)][*(rit + i + tam_subsequencia)] - m[*(rit + j)][*(rit + j - 1)];

        j = j_bk;
        i = i_bk;
      }

      if (delta < 0)
      {
        if (valor_obj + delta < melhor_valor_obj - std::numeric_limits<double>::epsilon())
        {
          melhor_valor_obj = valor_obj + delta;
          melhor_i = i;
          melhor_j = j;
          improved = true;
        }
      }
    }
  }

  if (improved == true)
  {
    reinsertion(solucao, melhor_i, tam_subsequencia, melhor_j);
    valor_obj = melhor_valor_obj;
  }
}

void twoOpt(std::vector<int> &solucao, int i, int j)
{
  std::reverse(solucao.begin() + i, solucao.begin() + j + 1);
}

void buscaVizinhanca2Opt(std::vector<int> &solucao, double &valor_obj, double **m)
{
  double novo_valor_obj, perda, ganho, delta, melhor_valor_obj = std::numeric_limits<double>::infinity();
  int melhor_i, melhor_j;
  bool improved = false;

  for (int i = 1; i < solucao.size() - 2; i++)
  {
    for (int j = i + 1; j < solucao.size() - 1; j++)
    {
      if (j == i + 1 || (i == 1 && j == solucao.size() - 2))
        continue;

      perda = m[solucao[i]][solucao[i - 1]] + m[solucao[j]][solucao[j + 1]];
      ganho = m[solucao[j]][solucao[i - 1]] + m[solucao[i]][solucao[j + 1]];

      delta = ganho - perda;

      if (delta < std::numeric_limits<double>::epsilon())
      {
        if (valor_obj + delta < melhor_valor_obj)
        {
          melhor_valor_obj = valor_obj + delta;
          melhor_i = i;
          melhor_j = j;
          improved = true;
        }
      }
    }
  }
  if (improved == true)
  {
    valor_obj = melhor_valor_obj;
    twoOpt(solucao, melhor_i, melhor_j);
  }
}

std::vector<int> GILS_RVND()
{
  int I_max = 50;
  int I_ils;

  if (dimension >= 150)
  {
    I_ils = dimension / 2;
  }
  else
  {
    I_ils = dimension;
  }

  double valor_obj_final = std::numeric_limits<double>::infinity();
  std::vector<int> s_final;

  for (int i = 0; i < I_max; i++)
  {
    double alpha = (double)rand() / RAND_MAX;

    std::vector<int> s, lista_candidatos;
    std::vector<std::pair<std::pair<int, int>, double>> custo_insercao;
    double valor_obj;

    temp1 = std::chrono::system_clock::now();
    construcao(s, lista_candidatos, 3, custo_insercao, valor_obj, alpha, matrizAdj, dimension);
    temp2 = std::chrono::system_clock::now();
    tempos[5] += std::chrono::duration_cast<std::chrono::milliseconds>(temp2 - temp1).count();

    std::vector<int> s_b = s;
    double valor_obj_b = valor_obj;
    int iter_ILS = 0;

    // std::cout << "UAILE\n";

    while (iter_ILS < I_ils)
    {

      RVND(s, valor_obj, matrizAdj);

      //std::cout << valor_obj << ", " << valor_obj_b << "\n";

      if (valor_obj < valor_obj_b - std::numeric_limits<double>::epsilon())
      {
        s_b = s;
        valor_obj_b = valor_obj;
        iter_ILS = 0;
      }

      std::vector<int> temp_s = s_b;

      perturb(temp_s);

      s = temp_s;
      valor_obj = calcularValorObj(s, matrizAdj);
      iter_ILS++;
    }

    // std::cout << "UAILE FIM\n";

    if (valor_obj_b < valor_obj_final)
    {
      s_final = s_b;
      valor_obj_final = valor_obj_b;
    }
  }

  return s_final;
}