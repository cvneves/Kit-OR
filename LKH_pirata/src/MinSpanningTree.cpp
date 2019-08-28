// A C++ program for Prim's Minimum
// Spanning Tree (MST) algorithm. The program is
// for adjacency matrix representation of the graph
#include "MinSpanningTree.h"
using namespace std;

void process(int vtx, std::vector<std::vector<dii>> &AdjList, vi &taken, priority_queue<dii> &pq, bool skipFirstNode)
{ // so, we use -ve sign to reverse the sort order
    taken[vtx] = 1;
    int start = (int)skipFirstNode;
    for (int j = start; j < (int)AdjList[vtx].size(); j++)
    {
        dii v = AdjList[vtx][j];
        if (!taken[v.second.first])
            pq.push({-v.first, {-v.second.first, -v.second.second}});
    }
} // sort by (inc) weight then by (inc) id

double MST(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, bool skipFirstNode)
{
    priority_queue<dii> pq;

    int start = (int)skipFirstNode;

    // inside int main()---assume the graph is stored in AdjList, pq is empty
    taken.assign(V, 0); // no vertex is taken at the beginning
    parent.assign(V, -1);
    process(start, AdjList, taken, pq, 1); // take vertex 0 and process all edges incident to vertex 0
    int previous = start;
    double mst_cost = 0;
    int u, v, w, k = start;
    while (!pq.empty())
    { // repeat until V vertices (E=V-1 edges) are taken
        dii front = pq.top();

        pq.pop();
        u = -front.second.first, v = -front.second.second, w = -front.first; // negate the id and weight again

        if (!taken[u])
        {                                                                 // we have not connected this vertex yet
            mst_cost += w, process(u, AdjList, taken, pq, skipFirstNode); // take u, process all edges incident to u
            parent[u] = v;
            s[k++ + start] = {v, u};
        }
    } // each edge is in pq only once!

    return mst_cost;
}

double MS1T(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s)
{
    double cost = MST(V, AdjList, taken, parent, s, true);
    double c1, c2, a;
    dii e1, e2;
    auto min1 = std::min_element(AdjList[0].begin() + 1, AdjList[0].end());
    auto min2 = min1;

    if (min1 == AdjList[0].end() - 1)
    {
        min2 = std::min_element(AdjList[0].begin() + 1, AdjList[0].end() - 1);
    }
    else if (min1 == AdjList[0].begin() + 1)
    {
        min2 = std::min_element(AdjList[0].begin() + 2, AdjList[0].end());
    }
    else
    {
        auto min22 = std::min_element(AdjList[0].begin() + 1, min1);
        auto min23 = std::min_element(min1 + 1, AdjList[0].end());
        min2 = (min22->first < min23->first) ? min22 : min23;
    }

    parent[1] = 0;

    s[0] = {min1->second.second, min1->second.first};
    s[1] = {min2->second.second, min2->second.first};

    cost += min1->first + min2->first;

    return cost;
}

void Ascent(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &edges)
{
    // int k = 0;
    double W = -std::numeric_limits<double>::infinity();
    std::vector<double> lambda, v;

    lambda.assign(V, 0);

    double t0 = 0.00001;

    int k = 1;
    while (1)
    {
        v.assign(V, 2);
        edges.assign(V, {-3, -3});

        double T = MS1T(V, AdjList, taken, parent, edges);
        double PI_SUM = 0;
        for (int i = 0; i < V; i++)
        {
            PI_SUM += lambda[i];
        }
        T += 2 * PI_SUM;

        std::cout << T << "\n";

        double SUM_V = 0;
        for (int i = 1; i < V; i++)
        {
            v[parent[i]]--;
            SUM_V += v[i]--;
        }
        v[0] = 0;

        bool isFeasible = true;
        for (int i = 0; i < V; i++)
        {
            if (v[i] > std::numeric_limits<double>::epsilon())
            {
                isFeasible = false;
                break;
            }
        }

        if (isFeasible)
        {
            std::cout << "Is feasible\n";
        }

        double t = (double) t0 / k;

        for (int i = 0; i < V; i++)
        {
            lambda[i] += t * v[i];
            // std::cout << lambda[i] << "\n";
        }

        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                AdjList[i][j].first -= lambda[i] + lambda[j];
            }
        }
    }

    

}