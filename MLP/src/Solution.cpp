#include "Solution.h"

void printSolution(Solution &s)
{
    for (int i = 0; i < s.cycle.size(); i++)
    {
        std::cout << s.cycle[i] << " ";
    }
    std::cout << "\n";
}

double calculaCusto(Solution &s)
{
    double custo = 0;
    for (int i = 1; i < s.N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            std::cout << s.cycle[j] << " -> " << s.cycle[j + 1] << "\n";
            custo += s.M[s.cycle[j]][s.cycle[j + 1]];
        }
        std::cout << "\n";
    }

    return custo;
}