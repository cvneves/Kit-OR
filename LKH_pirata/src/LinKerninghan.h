#ifndef LINKERNINGHAN_H
#define LINKERNINGHAN_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <list>
#include "SolutionUtils.h"

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
    void Print();

    double getCost();
    int getN();
    int inverse(int node);
    int next(int node);
    int prev(int node);
    void flip(int a, int b);
    bool sequence(int a, int b, int c);
    vi getTour();

};

void lkStep(Tour &T, double **c);
int findPromisingVertex(Tour &T, double **c, int base, double delta, vector<bool> &taken);

#endif