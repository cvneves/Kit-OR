#include <bits/stdc++.h>
using namespace std;

typedef pair<double, std::pair<int, int>> dii;
typedef pair<int, int> ii;
typedef vector<vector<double>> vvi;
typedef std::vector<ii> vii;
typedef vector<int> vi;

void process(int vtx, std::vector<std::vector<dii>> &AdjList, vi &taken, priority_queue<dii> &pq, bool skip);

void MST(int V, std::vector<std::vector<dii>> &AdjList, vi &taken, vi &parent, vii &s, bool skip);
