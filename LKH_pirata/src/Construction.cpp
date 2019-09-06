#include "Construction.h"

std::vector<int> construction(long double alpha, int N, double **M)
{
  int r = 1;
  std::vector<int> s(N + 1), CL(N - 1);
  std::vector<long double> distances(N - 1);

  s[0] = s[s.size() - 1] = r;

  for (int i = 2; i <= N; i++)
  {
    CL[i - 2] = i;
  }

  int c = 1;
  while (!CL.empty())
  {
    for (int i = 0; i < CL.size(); i++)
    {
      distances[i] = M[r][CL[i]];
    }
    std::sort(distances.begin(), distances.end());

    int a = rand() % (int)std::floor(alpha * (CL.size() - 1) + 1);
    r = CL[a];
    s[c] = r;
    c++;

    CL.erase(CL.begin() + a);
    distances.erase(distances.begin());
  }

  return s;
}