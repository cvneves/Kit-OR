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
  // srand(1570852601);
  srand(x);
  // 1570403292

  readData(argc, argv, &dimension, &matrizAdj);
  // printData(matrizAdj, dimension);

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
  double altDelta1 = 0;

  deque<pair<pair<int, int>, double>> flipSequence;
  vector<bool> taken;
  taken.assign(T.getN() + 1, false);
  // step(T, c, 1, 1, 0, altDelta1, neighbourSet, flipSequence, taken);
  // alternate_step(T, c, 1, 1, 0, altDelta1, neighbourSet, flipSequence, taken);

  // s = T.getTour();
  // cout << T.getCost() - altDelta1 << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << " |\n";

  

  lin_kerninghan(T, T2, c, neighbourSet);

  // cout << T2.getCost() << "\n";

  // Chained_Lin_Kerninghan(T, matrizAdj, neighbourSet);

  // deque<pair<pair<int, int>, double>> flipSequence;
  // vector<bool> taken(T.getN() + 1, false);
  // double final_delta = 0;

  // alternate_step(T, matrizAdj, 1, 1, 0, final_delta, neighbourSet, flipSequence, taken);

  return 0;
}
