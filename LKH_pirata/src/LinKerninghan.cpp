#include "LinKerninghan.h"

Tour::Tour(vi &s, int V, double objValue)
{
    tour = s;
    cost = objValue;
    N = V;
    inv.assign(N + 1, 0);
    for (int i = 0; i < V; i++)
    {
        inv[tour[i]] = i;
    }
}

void Tour::Print()
{
    for (int i = 0; i < N; i++)
    {
        std::cout << tour[i] << " ";
    }
    std::cout << tour[0] << " \n";
}

double Tour::getCost() { return cost; }
int Tour::inverse(int node) { return inv[node]; }
int Tour::next(int node) { return tour[(inv[node] + 1) % N]; }
int Tour::prev(int node) { return tour[((inv[node] - 1) % N + N) % N]; }
int Tour::getN() { return N; }
vi Tour::getTour()
{
    return tour;
}

void Tour::flip(int a, int b)
{
    int x, y;
    x = min(inv[a], inv[b]);
    y = max(inv[a], inv[b]);

    for (int i = x, j = y; i < j; i++, j--)
    {
        swap(inv[tour[i]], inv[tour[j]]);
        swap(tour[i], tour[j]);
    }
}
bool Tour::sequence(int a, int b, int c)
{
    if (inv[a] < inv[b] && inv[b] < inv[c] || inv[c] < inv[b] && inv[b] < inv[a])
        return true;

    return false;
}

void lkStep(Tour &T, double **c)
{
    double delta = 0;

    T.Print();

    int base = 2, probe;
    vector<bool> taken;
    taken.assign(T.getN() + 1, false);

    while ((probe = findPromisingVertex(T, c, base, delta, taken)) != -1)
    {
        delta += c[base][T.next(base)] - c[T.next(base)][T.next(probe)] + c[probe][T.next(probe)] - c[probe][base];
        cout << "\n" << T.next(base) << " " << probe << " " << T.getCost() - delta << "\n";
        cout << base << " " << T.next(base) << " " << probe << " " << T.next(probe) << "\n";
        T.flip(T.next(base), probe);
        T.Print();

        vi s = T.getTour();
        printSolucao(s);
        cout << calcularValorObj(s, c) << "\n";
    }
}

int findPromisingVertex(Tour &T, double **c, int base, double delta, vector<bool> &taken)
{
    double A = delta + c[base][T.next(base)];
    for (int probe = 2; probe <= T.getN(); probe++)
    {
        if (taken[probe] == false && A > c[T.next(base)][T.next(probe)] && probe != base && probe != T.next(base) && probe != T.prev(base))
        {
            taken[probe] = true;
            return probe;
        }
    }
    return -1;
}