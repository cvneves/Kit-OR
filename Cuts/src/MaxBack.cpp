#include "MaxBack.h"

double maxBack(Graph *G, int a, std::vector<bool> &S)
{
    S[a] = true;
    int N = S.size(), s0_size = 1;
    std::vector<double> b(N);

    double cutval = 0;
    double cutmin = std::numeric_limits<double>::infinity();

    for (int i = 0; i < N - 1; i++)
    {
        if (i == a)
        {
            b[i] = -std::numeric_limits<double>::infinity();
            continue;
        }
        b[i] = G->w(a, i);
        cutval += b[i];
    }

    while (s0_size < S.size())
    {
        int maxB = std::max_element(b.begin(), b.end()) - b.begin();
        s0_size++;

        cutval += 2 - 2 * b[maxB];

        std::cout << cutval << "\n";   

        S[maxB] = true;
        b[maxB] = -std::numeric_limits<double>::infinity();

        for (int i = 0; i < N; i++)
        {
            if (S[i] == true)
            {
                continue;
            }
            b[i] += G->w(maxB, i);
        }
    }

    return cutval;
}