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
  srand(1570344553);
  // srand(x);

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

  deque<pair<pair<int, int>, double>> flipSequence;
  vector<bool> taken;
  taken.assign(T.getN() + 1, false);

  // alternate_step(T, matrizAdj, 1, 1, 0, neighbourSet, flipSequence, taken);

  // step(T, matrizAdj, 1, 1, 0, delta, neighbourSet, flipSequence, taken);

  // lk_search(T, 1, matrizAdj, neighbourSet, flipSequence);

  lin_kerninghan(T, T2, matrizAdj, neighbourSet);

  // while (!flipSequence.empty())
  // {
  //   T2.flip(flipSequence.front().first.first, flipSequence.front().first.second);

  //   flipSequence.pop_front();
  // }

  s = T2.getTour();

  cout << calcularValorObj(s, matrizAdj) + matrizAdj[s[s.size() - 1]][s[0]] << "\n";
  cout << "\n"
       << T.getCost() << "\n";
  return 0;
}
