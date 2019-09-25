#ifndef LINKERNINGHAN_H
#define LINKERNINGHAN_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <list>
#include "SolutionUtils.h"
#include <limits>

#define INF std::numeric_limits<double>::infinity()

using namespace std;

typedef vector<int> vi;

class Tour
{
  private:
    vi tour, inv;
    double cost;
    int N;
    bool reversed;

  public:
    Tour(vi &s, int V, double objValue);
    void print();

    double getCost();
    int getN();
    int inverse(int node);
    int next(int node);
    int prev(int node);
    void flip(int a, int b);
    bool sequence(int a, int b, int c);
    vi getTour();

};

void lkStep(Tour &T, double **c, vector<vector<int>> &neighbourSet);
int findPromisingVertex(Tour &T, double **c, int base, double delta, vector<bool> &taken, vector<vector<int>> &neighbourSet);

int breadth(int k);

void step(Tour &T, double **c, int base, int level, float delta, vector<vector<int>> &neighbourSet, stack<pair<int, int>> &flipSequence);

#endif