#include "CandidateList.h"

void computeBeta(std::vector<std::vector<double>> &beta, int V, std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList)
{
    vector<stack<int>> children;
    children.assign(V, stack<int>());

    std::cout << "\n\n";
    for (int i = 1; i < V; i++)
        children[parent[i]].push(i);

    vector<int> node; //nodes in topological order
    node.assign(V, 0);
    {
        int i = 0, k = 0, j = 0;
        while (k < V - 1)
        {
            while (!children[i].empty())
            {
                node[k++ + 1] = children[i].top();
                children[i].pop();
            }
            i = node[j++ + 1];
        }
    }

    beta = std::vector<std::vector<double>>(V, std::vector<double>(V, std::numeric_limits<double>::infinity()));
    {
        int k = 0;
        for (int m = 0; m < V - 1; m++)
        {
            beta[m][m] = -std::numeric_limits<double>::infinity();
            for (int n = m + 1; n < V; n++)
            {
                int i = node[m], j = node[n];
                if (i == parent[j])
                {
                    beta[i][j] = beta[j][i] = AdjList[i][j].first;
                    continue;
                }
                beta[i][j] = beta[j][i] = std::max(beta[i][parent[j]], AdjList[j][parent[j]].first);
                cout << beta[i][j] << " ";
            }
            cout << "\n";
        }
    }
}

void computeAlpha(std::vector<std::vector<double>> &alpha, std::vector<std::vector<double>> &beta, vector<pair<int, int>> &edges, int V, std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList)
{
    alpha = std::vector<std::vector<double>>(V, std::vector<double>(V, -1));

    //edges that are adjacent to node 0
    {
        dii longestEdge = {-std::numeric_limits<double>::infinity(), {0, 0}};

        for (int i = 0; i < edges.size(); i++)
        {
            if (edges[i].first == 0 || edges[i].second == 0)
            {
                longestEdge = std::max(longestEdge, {AdjList[edges[i].first][edges[i].second].first, edges[i]});
                std::cout << edges[i].first << " " << edges[i].second << "\n";
            }
        }
        // std::cout << longestEdge.second.second << " " << longestEdge.second.first << "\n";
        double removedCost = longestEdge.first;
        std::cout << removedCost << "\n";

        for (int i = 1; i < V; i++)
        {
            alpha[0][i] = alpha[i][0] = (AdjList[0][i].first - removedCost);
        }
    }

    // edges that require computing beta
    computeBeta(beta, V, parent, AdjList);

    alpha[0][0] = -std::numeric_limits<double>::infinity();
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            alpha[i][j] = alpha[j][i] = (AdjList[i][j].first - beta[i][j]);
        }
        alpha[i][i] = -std::numeric_limits<double>::infinity();
    }

    //edges that are already on the MST
    for (int i = 0; i < V - 1; i++)
    {
        alpha[edges[i].first][edges[i].second] = alpha[edges[i].second][edges[i].first] = 0.0;
    }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            std::cout << alpha[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void generateCandidateList(std::vector<std::vector<int>> &rankedNodes, double **matrizAdj, int V)
{
    std::vector<std::vector<dii>> AdjList(V, std::vector<dii>(V));
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
        }
    }

    vii edges;
    edges.assign(V - 1, {-3, -3});
    vi parent;

    vi taken;

    for (int A = 0; A < 1; A++)
    {
        parent.assign(V, -1);
        Ascent(V, AdjList, taken, parent, edges, rankedNodes);
    }

    std::vector<std::vector<double>> alpha, beta;
    computeAlpha(alpha, beta, edges, V, parent, AdjList);

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            sortedAdjList[i][j] = {alpha[i][j], {i, j}};
        }
    }

    for (int i = 0; i < V; i++)
    {
        std::sort(sortedAdjList[i].begin(), sortedAdjList[i].end());
        rankedNodes[i].assign(V, 0);
        for (int j = 0; j < V; j++)
        {
            rankedNodes[i][j] = sortedAdjList[i][j].second.first;
        }
    }
}