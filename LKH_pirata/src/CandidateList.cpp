#include "CandidateList.h"

void computeBeta(std::vector<std::vector<double>> &beta, int V, std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList)
{
    beta = std::vector<std::vector<double>>(V, std::vector<double>(V, std::numeric_limits<double>::infinity()));
    for (int i = 1; i < V; i++)
    {
        beta[i][i] = -std::numeric_limits<double>::infinity();
        for (int j = i + 1; j < V; j++)
        {
            beta[i][j] = beta[j][i] = std::max(beta[i][parent[j]], AdjList[j][parent[j]].first);
        }
    }
}

void computeAlpha(std::vector<std::vector<double>> &alpha, std::vector<std::vector<double>> &beta, vector<pair<int, int>> &edges, int V, std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList)
{
    alpha = std::vector<std::vector<double>>(V, std::vector<double>(V, -1));

    // edges that require computing beta
    computeBeta(beta, V, parent, AdjList);

    alpha[0][0] = -std::numeric_limits<double>::infinity();
    for (int i = 1; i < V; i++)
    {
        for (int j = i+1; j < V; j++)
        {
            alpha[i][j] = alpha[j][i] = (AdjList[i][j].first - beta[i][j]);
            
        }
        alpha[i][i] = -std::numeric_limits<double>::infinity();
    }

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