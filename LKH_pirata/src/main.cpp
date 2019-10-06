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
  srand(x);

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
  deque<pair<int, int>> kickFlips;
  vector<bool> taken;
  taken.assign(T.getN() + 1, false);

  T.print();
  vector<pair<int, pair<int, int>>> t;
  t.assign(4, {0, {0, 0}});

  double delta = 0;

  kick(T, matrizAdj, delta, t, kickFlips);
  // T.flip(9, 5);
  // T.flip(11,13);
  // T.flip(7,9);

  T.print();

  // alternate_step(T, matrizAdj, 1, 1, 0, neighbourSet, flipSequence, taken);

  // step(T, matrizAdj, 1, 1, 0, delta, neighbourSet, flipSequence, taken);

  // lk_search(T, 1, matrizAdj, neighbourSet, flipSequence);

  // lin_kerninghan(T, T2, matrizAdj, neighbourSet);

  // while (!flipSequence.empty())
  // {
  //   T2.flip(flipSequence.front().first.first, flipSequence.front().first.second);

  //   flipSequence.pop_front();
  // }

  s = T.getTour();

  T.setCost(T.getCost() - delta);

  cout << T.getCost() << "\n";

  cout << calcularValorObj(s, matrizAdj) + matrizAdj[s[s.size() - 1]][s[0]] << "\n";

  while (!kickFlips.empty())
  {
    pair<int, int> flip = kickFlips.back();
    kickFlips.pop_back();

    T.flip(flip.second, flip.first);
  }

  T.print();

  T.setCost(T.getCost() + delta);
  s = T.getTour();

  cout << T.getCost() << "\n";

  cout << calcularValorObj(s, matrizAdj) + matrizAdj[s[s.size() - 1]][s[0]] << "\n";

  return 0;
}
