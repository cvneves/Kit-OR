#include "MinimumCut.h"

void minimumCut(Graph *G, int a)
{
    std::vector<bool> deletedNodes(G->getNumNodes(), false);
    int l = 0;

    for (int i = 0; i < G->getNumNodes() - 1; i++)
    {
        minimumCutPhase(G, 1, deletedNodes, l);
    }
}

void minimumCutPhase(Graph *G, int a, std::vector<bool> &deletedNodes, int &l)
{
    std::vector<int> V(G->getNumNodes() - 1 - l);
    for (int i = 0, j = 0; i < G->getNumNodes(); i++)
    {
        if (i == a || deletedNodes[i] == true)
            continue;
        V[j] = i;
        j++;
    }

    std::vector<int> A(V.size() + 1, -1);
    A[0] = a;

    int k = 1;

    double greatestWeightSum;

    while (k < A.size())
    {
        greatestWeightSum = -std::numeric_limits<double>::infinity();
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
    std::cout << "\n";

    for (int i = 0; i < A.size() - 2; i++)
    {
        G->getEdges()[G->getEdge(A[i], A[A.size() - 2])].w += G->getEdges()[G->getEdge(A[i], A[A.size() - 1])].w;
    }

    std::cout << greatestWeightSum << "\n";

    deletedNodes[A[A.size() - 1]] = true;
    l++;
}
