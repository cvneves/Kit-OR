#include "SolutionUtils.h"
#include <iostream>

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