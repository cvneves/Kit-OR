#include "MinimumCut.h"

double minimumCut(Graph *G, int a, std::vector<bool> &S)
{
    std::vector<bool> deletedNodes(G->getNumNodes(), false);
    int l = 0;
    double cutCost, mincutCost = std::numeric_limits<double>::infinity();
    std::vector<std::vector<bool>> partition(G->getNumNodes(), std::vector<bool>(G->getNumNodes(), false)), bestPartition;
    std::vector<int> A;
    int nSubsets = 0;
    int lastNode;
    S = std::vector<bool>(G->getNumNodes(), false);

    for (int i = 0; i < partition.size(); i++)
    {
        partition[i][i] = true;
    }

    for (int i = 0; i < G->getNumNodes() - 1; i++)
    {
        minimumCutPhase(G, 1, deletedNodes, l, cutCost, A);
        if (cutCost - mincutCost < EPSILON)
        {
            mincutCost = cutCost;
            lastNode = A[A.size() - 1];
            bestPartition = partition;
        }

        partition[A[A.size() - 1]][A[A.size() - 2]] = true;
    }

    // for (int i = 0; i < partition.size(); i++)
    // {
    //     for (int j = 0; j < partition.size(); j++)
    //     {
    //         std::cout << bestPartition[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }

    std::cout << "Minimum cut cost: " << mincutCost << "\n";

    {
        int i = 0;
        while (1)
        {
            if (i == bestPartition.size())
            {
                break;
            }
            if (bestPartition[lastNode][i] == true || bestPartition[i][lastNode] == true)
            {
                bestPartition[lastNode][i] = bestPartition[i][lastNode] = false;
                S[lastNode] = S[i] = true;
                lastNode = i;
                i = 0;
                continue;
            }
            i++;
        }
    }

    // std::cout << "S: ";

    // for (const int &i : S)
    // {
    //     std::cout << i << " ";
    // }
    // std::cout << "\n";

    return mincutCost;
}

void minimumCutPhase(Graph *G, int a, std::vector<bool> &deletedNodes, int &l, double &cutCost, std::vector<int> &A)
{
    std::vector<int> V(G->getNumNodes() - 1 - l);
    for (int i = 0, j = 0; i < G->getNumNodes(); i++)
    {
        if (i == a || deletedNodes[i] == true)
            continue;
        V[j] = i;
        j++;
    }

    A = std::vector<int>(V.size() + 1, -1);
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

    // for (int i = 0; i < A.size(); i++)
    // {
    //     std::cout << A[i] << " ";
    // }
    // std::cout << "\n";

    for (int i = 0; i < A.size() - 2; i++)
    {
        G->getEdges()[G->getEdge(A[i], A[A.size() - 2])].w += G->getEdges()[G->getEdge(A[i], A[A.size() - 1])].w;
    }

    // std::cout << greatestWeightSum << "\n\n";
    cutCost = greatestWeightSum;

    deletedNodes[A[A.size() - 1]] = true;
    l++;
}
