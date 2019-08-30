#include "CandidateList.h"

void computeBeta(std::vector<std::vector<double>> &beta, int V, std::vector<int> &parent, std::vector<std::vector<dii>> &AdjList)
{
    beta = std::vector<std::vector<double>>(V, std::vector<double>(V));
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
    alpha = std::vector<std::vector<double>>(V, std::vector<double>(V));

    //edges that are already on the MST
    for (int i = 0; i < V - 1; i++)
    {
        alpha[edges[i].first][edges[i].second] = alpha[edges[i].second][edges[i].first] = 0.0;
    }

    //edges that are adjacent to node 0
    {
        dii longestEdge = {-std::numeric_limits<double>::infinity(), {0, 0}};
        for (int i = 0; i < edges.size(); i++)
        {
            if (edges[i].first == 0 || edges[i].second == 0)
            {
                longestEdge = std::max(longestEdge, {AdjList[edges[i].first][edges[i].second].first, edges[i]});
                // std::cout << edges[i].first << " " << edges[i].second << "\n";
            }
        }
        // std::cout << longestEdge.second.second << " " << longestEdge.second.first << "\n";
        
    }
}