#include "LinKerninghan.h"

Tour::Tour(vi &s, int V, double objValue)
{
    tour = s;
    tour.pop_back();
    cost = objValue;
    N = V;
    inv.assign(N + 1, 0);
    for (int i = 0; i < V; i++)
    {
        inv[tour[i]] = i;
    }
    reversed = false;
}

void Tour::print()
{
    for (int i = 0; i < tour.size(); i++)
    {
        std::cout << tour[i] << " ";
    }
    std::cout << "\n";
    // std::cout << tour[0] << " \n";
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
    int x = inv[a];
    int y = inv[b];
    int restart1 = 0, restart2 = 0;

    if (x < y)
    {
        for (int i = x, j = y; i < j; i++, j--)
        {
            swap(tour[i], tour[j]);
            swap(inv[tour[i]], inv[tour[j]]);
        }
    }
    else
    {
        x = inv[next(b)], y = inv[prev(a)];
        for (int i = x, j = y; i < j; i++, j--)
        {
            swap(tour[i], tour[j]);
            swap(inv[tour[i]], inv[tour[j]]);
        }
        reversed = !reversed;
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

    T.print();

    int base = 2, probe;
    vector<bool> taken;
    taken.assign(T.getN() + 1, false);

    while ((probe = findPromisingVertex(T, c, base, delta, taken)) != -1)
    {
        delta += c[base][T.next(base)] - c[T.next(base)][T.next(probe)] + c[probe][T.next(probe)] - c[probe][base];
        cout << "\n"
             << T.next(base) << " " << probe << " " << T.getCost() - delta << "\n";
        cout << base << " " << T.next(base) << " " << probe << " " << T.next(probe) << "\n";
        T.flip(T.next(base), probe);
        T.print();

        vi s = T.getTour();
        printSolucao(s);
        cout << calcularValorObj(s, c) + c[s[s.size() - 1]][1] << "\n";
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