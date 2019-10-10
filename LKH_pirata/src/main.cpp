#include "readData.h"
#include "MinSpanningTree.h"
#include "CandidateList.h"
#include "SolutionUtils.h"
#include "Construction.h"
#include "LinKerninghan.h"

#include <fstream>
#include <iostream>

#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <cmath>
#include <limits>
#include <chrono>

using namespace std;

int dimension;
double **matrizAdj;

int main(int argc, char **argv)
{
  int x = time(NULL);
  std::cout << x;
  srand(1570735428);
  // srand(x);
  // 1570403292

  readData(argc, argv, &dimension, &matrizAdj);
  printData(matrizAdj, dimension);

  vector<vector<int>> neighbourSet;
  generateCandidateList(neighbourSet, matrizAdj, dimension);

  double alpha = (double)rand() / RAND_MAX;
  std::vector<int> s = construction(alpha, dimension, matrizAdj);
  double objValue = calcularValorObj(s, matrizAdj);
  std::cout << "Initial obj value: " << objValue << "\n\n";

  Tour T(s, dimension, objValue);
  Tour T2 = T;
  T.print();
  cout << "\n";

  double **c = matrizAdj;

  int base = 1;
  int b = 2;
  int a = 13;

  int b1 = T.next(b);
  int s1 = T.next(base);

  double altDelta1 = 0;

  altDelta1 += c[T.prev(s1)][s1] - c[s1][T.next(b)] + c[b][T.next(b)] - c[b][T.prev(s1)];
  T.flip(s1, b);

  altDelta1 += c[T.prev(b)][b] - c[b][T.next(a)] + c[a][T.next(a)] - c[T.prev(b)][a];
  T.flip(b, a);

  s1 = T.next(base);
  b1 = T.prev(b);

  altDelta1 += c[T.prev(s1)][s1] - c[s1][T.next(a)] + c[a][T.next(a)] - c[T.prev(s1)][a];
  T.flip(s1, a);

  // altDelta1 += c[T.prev(b)][b] - c[b][s1] + c[s1][T.next(s1)] - c[T.prev(s1)][T.prev(b)];
  // T.flip(b, s1);

  // altDelta1 += c[T.prev(a)][a] - c[b1][a] + c[b1][T.next(b1)] - c[T.prev(b1)][T.prev(a)];
  // T.flip(a, b1);

  T.print();

  T.setCost(T.getCost() - altDelta1);
  s = T.getTour();
  cout << T.getCost() << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << "\n";

  // Chained_Lin_Kerninghan(T, matrizAdj, neighbourSet);

  // deque<pair<pair<int, int>, double>> flipSequence;
  // vector<bool> taken(T.getN() + 1, false);
  // double final_delta = 0;

  // alternate_step(T, matrizAdj, 1, 1, 0, final_delta, neighbourSet, flipSequence, taken);

  return 0;
}
