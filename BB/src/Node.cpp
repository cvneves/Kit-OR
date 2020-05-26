#include "Node.h"

std::vector<int> calcularSolucao(hungarian_problem_t *p, Node &n)
{
    //std::vector<int> vetor_infinito(n.arcos_proibidos.size(), std::numeric_limits<int>::max());
    std::vector<int> vetor_infinito(n.arcos_proibidos.size(), 99999999);


    int i = 0;
    
    for (const auto &arco : n.arcos_proibidos)
    {
        std::swap(p->cost[arco.first - 1][arco.second - 1], vetor_infinito[i]);
        i++;
    }

    //hungarian_print_costmatrix(p);
    n.obj_value = hungarian_solve(p);
    
    i = 0;

    for (const auto &arco : n.arcos_proibidos)
    {
        std::swap(p->cost[arco.first - 1][arco.second - 1], vetor_infinito[i]);
        i++;
    }

    std::vector<std::vector<int>> subtours = construirSubtours(p->assignment, p->num_rows);

    // for(int i = 0; i < subtours.size();i++)
    // {
    //     for(int j = 0; j < subtours[i].size();j++)
    //     {
    //         std::cout << subtours[i][j] - 1 << " ";
    //     }
    //     std::cout << "\n";
    // }
    // std::cout << "\n";

    if (subtours.size() == 1)
        n.pruning = true;
    else
        n.pruning = false;

    return subtours[selecionarSubtour(subtours)];
}