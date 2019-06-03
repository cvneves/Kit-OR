#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <iostream>

  typedef struct edge
  {
    int i, j;
    double w;
  } Edge;


class Graph
{
private:
  Edge* edges;
  int numEdges, numNodes, **edgeIndex;

public:
  Graph(int, double **);
  virtual ~Graph();

  int V();
  int E();

  double w(int, int);
  double w(std::pair<int, int>);

  int getEdge(int, int);

  void printEdges();
  
  Edge * getEdges();
};

#endif