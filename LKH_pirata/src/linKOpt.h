#ifndef LINKERNINGHAN_H
#define LINKERNINGHAN_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>
#include <list>

using namespace std;

typedef vector<int> vi;

typedef struct node
{
    int id, next, prev, rank;
} Node;

void lkStep(vi &T, int N, double &objValue, double **c);

#endif