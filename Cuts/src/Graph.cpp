#include "Graph.h"

Graph::Graph(int N, double **M)
{
    numEdges = (N * N - N) / 2;
    numNodes = N;

    edges = new Edge[numEdges];

    for (int i = 0, e = 0; i < numNodes; i++)
    {
        for (int j = i + 1; j < numNodes; j++)
        {
            edges[e].w = M[i+1][j+1];
            edges[e].i = i;
            edges[e].j = j;
            e++;
        }
    }

    edgeIndex = new int *[numNodes];
    for (int i = 0, e = 0; i < N; i++)
    {
        edgeIndex[i] = new int[numNodes];
        for (int j = 0; j < N; j++)
        {
            if (i == j)
                continue;
            if (i < j)
            {
                edgeIndex[i][j] = e;
                e++;
                continue;
            }
            edgeIndex[i][j] = edgeIndex[j][i];
        }
    }
}

Graph::~Graph()
{
    for (int i = 0; i < numNodes; i++)
    {
        delete[] edgeIndex[i];
    }
    delete[] edgeIndex;
    delete[] edges;
}

int Graph::getNumNodes() { return numNodes; }
int Graph::getNumEdges() { return numEdges; }

void Graph::setNumNodes(int n) { numNodes = n; }
void Graph::setNumEdges(int n) { numEdges = n; }

double Graph::w(int i, int j) { return edges[edgeIndex[i][j]].w; }
void Graph::setW(int i, int j, double w) { edges[getEdge(i, j)].w = w; }

int Graph::getEdge(int i, int j) { return edgeIndex[i][j]; }

void Graph::printEdges()
{
    for (int i = 0; i < numEdges; i++)
    {
        Edge e = edges[i];
        std::cout << edgeIndex[e.j][e.i] << "{" << e.i << ", " << e.j << "}, w = " << e.w << "\n";
    }
}

Edge *Graph::getEdges()
{
    return edges;
}