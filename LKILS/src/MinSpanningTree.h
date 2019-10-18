#ifndef MINSPANNINGTREE_H
#define MINSPANNINGTREE_H


#include <bits/stdc++.h>
#include <algorithm>
#include <limits>
#include <vector>
#include <map>

#define EPSILON 0.00001

using namespace std;

typedef pair<double, std::pair<int, int>> dii;
typedef pair<int, int> ii;
typedef vector<vector<double>> vvi;
typedef std::vector<ii> vii;
typedef vector<int> vi;

void process(int vtx, std::vector<std::vector<dii>> &AdjList, vi &taken, priority_queue<dii> &pq, bool skip, std::vector<std::vector<int>> &rankedNodes);

double MST(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, bool skip, std::vector<std::vector<int>> &rankedNodes, std::vector<int> &v);
double MS1T(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, std::vector<std::vector<int>> &rankedNodes, std::vector<int> &v);

void Ascent(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, std::vector<std::vector<int>> &rankedNodes);

#endif