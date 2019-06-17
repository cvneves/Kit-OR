#include "MaxBack.h"

double maxBack(Graph *G, int a, std::vector<bool> &S1)
{
    std::vector<bool> S = S1;
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

    while (s0_size < S.size() - 1)
    {
        int maxB = std::max_element(b.begin(), b.end()) - b.begin();
        s0_size++;

        cutval += 2 - 2 * b[maxB];

        // std::cout << cutval << "\n";

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

        if (cutval - cutmin < 0)
        {
            cutmin = cutval;
            S1 = S;
        }
    }

    // std::cout << "\n";
    // for (int i = 0; i < S1.size(); i++)
    // {
    //     std::cout << S1[i] << " ";
    // }
    // std::cout << "\n";

    return cutmin;
}