#ifndef LINKERNINGHAN_H
#define LINKERNINGHAN_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <deque>
#include <list>
//#include "SolutionUtils.h"
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
  bool isReversed();
  void setCost(double c);
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

void step(Tour &T, double **c, int base, int level, float delta, double &final_delta, vector<vector<int>> &neighbourSet, deque<pair<pair<int, int>, double>> &flipSequence, vector<bool> &taken);
void alternate_step(Tour &T, double **c, int base, int level, float delta, double &final_delta, vector<vector<int>> &neighbourSet, deque<pair<pair<int, int>, double>> &flipSequence, vector<bool> &taken);
bool lk_search(Tour &T, int v, double **c, vector<vector<int>> &neighbourSet, deque<pair<pair<int, int>, double>> &flipSequence);
void lin_kerninghan(Tour &T, Tour &lk_tour, double **c, vector<vector<int>> &neighbourSet);

void kick(Tour &T, double **c, double &delta, vector<pair<int, pair<int, int>>> t, deque<pair<int, int>> &kickFlips);

void Chained_Lin_Kerninghan(Tour &T, double **c,  vector<vector<int>> &neighbourSet);

#endif