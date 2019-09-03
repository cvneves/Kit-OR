// A C++ program for Prim's Minimum
// Spanning Tree (MST) algorithm. The program is
// for adjacency matrix representation of the graph
#include "MinSpanningTree.h"
using namespace std;

void process(int vtx, std::vector<std::vector<dii>> &AdjList, vi &taken, priority_queue<dii> &pq, bool skipFirstNode, std::vector<std::vector<int>> &rankedNodes)
{ // so, we use -ve sign to reverse the sort order
    taken[vtx] = 1;
    int start = (int)skipFirstNode;

    int max_iterations = std::min((int)AdjList[vtx].size(), 55);

    for (int j = start; j < max_iterations; j++)
    {
        if (skipFirstNode && rankedNodes[vtx][j] == 0)
        {
            continue;
        }

        dii v = AdjList[vtx][rankedNodes[vtx][j]];
        if (!taken[v.second.first])
            pq.push({-v.first, {-v.second.first, -v.second.second}});
    }
} // sort by (inc) weight then by (inc) id

double MST(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, bool skipFirstNode, std::vector<std::vector<int>> &rankedNodes)
{
    priority_queue<dii> pq;

    int start = (int)skipFirstNode;

    // inside int main()---assume the graph is stored in AdjList, pq is empty
    taken.assign(V, 0); // no vertex is taken at the beginning
    parent.assign(V, -1);
    process(start, AdjList, taken, pq, 1, rankedNodes); // take vertex 0 and process all edges incident to vertex 0
    int previous = start;
    double mst_cost = 0;
    int u, v, w, k = start;
    while (!pq.empty())
    { // repeat until V vertices (E=V-1 edges) are taken
        dii front = pq.top();

        pq.pop();
        u = -front.second.first, v = -front.second.second, w = -front.first; // negate the id and weight again

        if (!taken[u])
        {                                                                              // we have not connected this vertex yet
            mst_cost += w, process(u, AdjList, taken, pq, skipFirstNode, rankedNodes); // take u, process all edges incident to u
            parent[u] = v;
            s[k++ + start] = {v, u};
        }
    } // each edge is in pq only once!

    return mst_cost;
}

double MS1T(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, std::vector<std::vector<int>> &rankedNodes)
{
    double cost = MST(V, AdjList, taken, parent, s, true, rankedNodes);
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

void Ascent(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &edges, std::vector<std::vector<int>> &rankedNodes)
{
    // int k = 0;
    double W = -std::numeric_limits<double>::infinity(), prev_W, bestW;
    std::vector<double> pi, best_pi, v, last_v;
    int period = V / 2, iter = 0;
    bool firstPeriod = true;

    std::vector<std::vector<dii>> AdjListCopy = AdjList;

    pi.assign(V, 0);
    best_pi.assign(V, 0);

    v.assign(V, 2);
    last_v.assign(V, 0);

    double t0 = 1.0;

    while (1)
    {
        v.assign(V, 2);
        edges.assign(V, {-3, -3});

        double T = MS1T(V, AdjList, taken, parent, edges, rankedNodes);
        double PI_SUM = 0;
        for (int i = 0; i < V; i++)
        {
            PI_SUM += pi[i];
        }
        T += 2 * PI_SUM;

        prev_W = W;

        double SUM_V = 0;
        for (int i = 1; i < V; i++)
        {
            v[parent[i]]--;
            SUM_V += v[i]--;
        }
        // v[0] = 0;

        bool isFeasible = true;
        for (int i = 0; i < V; i++)
        {
            if (v[i] > 0.0001)
            {
                isFeasible = false;
                break;
            }
        }

        if (isFeasible)
        {
            break;
            // std::cout << "Is feasible\n";
        }

        double t = t0;

        for (int i = 0; i < V; i++)
        {
            pi[i] += 0.7 * t * v[i] + 0.3 * t * last_v[i];
        }

        for (int i = 0; i < V - 1; i++)
        {
            last_v[i] = v[i];
            for (int j = i + 1; j < V; j++)
            {
                AdjList[j][i].first = (AdjList[i][j].first = AdjListCopy[i][j].first - pi[i] - pi[j]);
            }
        }

        if (T > W)
        {
            W = T;
            std::cout << W << "\n";

            if (firstPeriod)
                t0 *= 2;
            if (iter == period)
                period *= 2;
        }
        else if (firstPeriod && iter > V / 4)
        {
            firstPeriod = false;
            iter = 0;
            t0 = 3 * t0 / 4.0;
        }

        iter++;

        if (iter > period)
        {
            iter = 0;
            t0 /= 2.0;
            period /= 2;
        }

        if (period == 0 || t0 < 0.01)
            break;
    }

    edges.assign(V - 1, {0, 0});

    // for (int i = 0; i < V; i++)
    // {
    //     for (int j = 0; j < V; j++)
    //     {
    //         std::cout << AdjList[i][j].first << " ";
    //     }
    //     std::cout << "\n";
    // }

    // std::cout << "LB: " << MST(V, AdjList, taken, parent, edges, false, rankedNodes);

    // for (int i = 0; i < V; i++)
    // {
    // std::cout << v[i] << "\n";
    // }
}
