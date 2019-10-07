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
  srand(1570416785);
  srand(x);
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
  // T.print();
  cout << "\n";
  // s = T2.getTour();
  // cout << calcularValorObj(s, matrizAdj) + matrizAdj[s[s.size() - 1]][s[0]] << "\n";

  Chained_Lin_Kerninghan(T, matrizAdj, neighbourSet);

  // lin_kerninghan(T, T2, matrizAdj, neighbourSet);

  // deque<pair<pair<int, int>, double>> flipSequence;

  // lk_search(T, 1, matrizAdj, neighbourSet, flipSequence);

  // while (!flipSequence.empty())
  // {
  //   int x = flipSequence.front().first.first;
  //   int y = flipSequence.front().first.second;

  //   double g = flipSequence.front().second;

  //   T.print();
  //   cout << x << " " << y << " " << g << "\n\n";

  //   T.flip(x, y);
  //   T.setCost(T.getCost() - g);

  //   flipSequence.pop_front();
  // }

  // T.print();

  // s = T.getTour();
  // cout << T.getCost() << ", " << calcularValorObj(s, matrizAdj) + matrizAdj[s[s.size() - 1]][s[0]] << " |\n";

  return 0;
}
