#include <vector>
#include <iostream>

struct Solution{
    std::vector<int> cycle;
    std::vector<std::vector<int>> T, C, W; 
    int N;
    double **M;
};

void printSolution(Solution &s);
double calculaCusto(Solution &s);