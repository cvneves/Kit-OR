#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <limits>
#define EPSILON 0.00001

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
  Graph() {}
  virtual ~Graph();

  int getNumNodes();
  int getNumEdges();

  void setNumNodes(int);
  void setNumEdges(int);

  double w(int, int);
  void setW(int i, int j, double w);

  int getEdge(int, int);

  void printEdges();

  Edge *getEdges();
};

#endif