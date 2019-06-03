#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <limits>

typedef struct edge
{
  int i, j;
  double w;
} Edge;

class Graph
{
private:
  Edge *edges;
  int numEdges, numNodes, **edgeIndex;

public:
  Graph(int, double **);
  virtual ~Graph();

  int getNumNodes();
  int getNumEdges();

  double w(int, int);

  int getEdge(int, int);

  void printEdges();

  Edge *getEdges();
};

#endif