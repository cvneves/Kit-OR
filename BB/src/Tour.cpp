#include "Tour.h"

std::vector<std::vector<int>> construirSubtours(int **m, const int &N)
{
    std::vector<std::vector<int>> subtours;
    std::vector<int> candidatos(N);

    for (int i = 0; i < N; i++)
        candidatos[i] = i + 1;

    while (candidatos.empty() == false)
    {
        int inicio = candidatos[0]; // Primeiro nó do tour
        int no_atual = inicio;

        std::vector<int> tour;
        tour.push_back(inicio);

        do
        {
            for (int j = 0; j < N; j++)
            {
                if (m[no_atual - 1][j] == 1)
                {
                    no_atual = j + 1;
                    candidatos.erase(std::find(candidatos.begin(), candidatos.end(), no_atual));
                    break;
                }
            }
            tour.push_back(no_atual);

        } while (no_atual != inicio);
        subtours.push_back(tour);
    }

    return subtours;
}

int selecionarSubtour(const std::vector<std::vector<int>> &subtours)
{
    int tour = 0;
    int menor_tam_tour = subtours[0].size();

    for (int i = 1; i < subtours.size(); i++)
    {
        if (subtours[i].size() < menor_tam_tour)
        {
            menor_tam_tour = subtours[i].size();
            tour = i;
        }
    }
    return tour;
}



void printSubtours(const std::vector<std::vector<int>> &tours)
{
    for (int i = 0; i < tours.size(); i++)
    {
        for (int j = 0; j < tours[i].size(); j++)
        {
            std::cout << tours[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void printSolucao(const std::vector<int> &s)
{
    for (int j = 0; j < s.size(); j++)
    {
        std::cout << s[j] << " ";
    }
    std::cout << "\n";
}
