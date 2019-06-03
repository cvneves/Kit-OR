#include "MinimumCut.h"

void minimumCut(Graph *G, int a)
{

}

void minimumCutPhase(Graph *G, int a)
{
    std::vector<int> V(G->getNumNodes() - 1);
    std::vector<int> A(G->getNumNodes() - 1, -1);

    A[0] = a;

    for (int i = 0, j = 0; i < G->getNumNodes(); i++)
    {
        if (i == a)
            continue;
        V[j] = i;
        j++;
    }

    int k = 1;

    while (k < A.size())
    {
        double greatestWeightSum = -std::numeric_limits<double>::infinity();
        auto node = V.begin();
        auto jt = V.begin();
        
        for (int j = 0; j < V.size(); j++, jt++)
        {
            double weightSum = 0;
            auto it = A.begin();
            for (int i = 0; i < k; it++, i++)
            {
                weightSum += G->w(*it, *jt);
            }

            if (weightSum - greatestWeightSum > EPSILON)
            {
                greatestWeightSum = weightSum;
                node = jt;
            }
        }

        A[k] = *node;

        V.erase(node);

        k++;
    }

    for (int i = 0; i < A.size(); i++)
    {
        std::cout << A[i] << " ";
    }
}

