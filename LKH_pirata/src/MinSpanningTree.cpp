// A C++ program for Prim's Minimum
// Spanning Tree (MST) algorithm. The program is
// for adjacency matrix representation of the graph
#include "MinSpanningTree.h"
using namespace std;

void process(int vtx, std::vector<std::vector<dii>> &AdjList, vi &taken, priority_queue<dii> &pq, bool skip)
{ // so, we use -ve sign to reverse the sort order
    taken[vtx] = 1;
    int start = (int) skip;
    for (int j = start; j < (int)AdjList[vtx].size(); j++)
    {
        dii v = AdjList[vtx][j];
        if (!taken[v.second.first])
            pq.push({-v.first, {-v.second.first, -v.second.second}});
    }
} // sort by (inc) weight then by (inc) id

void MST(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, bool skip)
{
    priority_queue<dii> pq;

    int start = (int) skip;

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
        {                                                  // we have not connected this vertex yet
            mst_cost += w, process(u, AdjList, taken, pq, skip); // take u, process all edges incident to u
            parent[u] = v;
            s[k++] = {v, u};
        }
    } // each edge is in pq only once!

    std::cout << mst_cost << "\n";
}
