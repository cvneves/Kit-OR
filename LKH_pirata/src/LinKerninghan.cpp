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
    cout << "Tour: ";
    for (int i = 0; i < tour.size(); i++)
    {
        std::cout << tour[i] << " ";
    }
    std::cout << "\n";
    cout << "Reversed: " << reversed << "\n";
    // std::cout << tour[0] << " \n";
}

double Tour::getCost() { return cost; }
void Tour::setCost(double c) { cost = c; }

int Tour::inverse(int node) { return inv[node]; }
int Tour::next(int node)
{
    if (!reversed)
        return tour[(inv[node] + 1) % N];
    return tour[((inv[node] - 1) % N + N) % N];
}
int Tour::prev(int node)
{
    if (!reversed)
        return tour[((inv[node] - 1) % N + N) % N];
    return tour[(inv[node] + 1) % N];
}
int Tour::getN() { return N; }
vi Tour::getTour()
{
    return tour;
}

int breadth(int k)
{
    return (k <= 2) ? 5 : 1;
}

void Tour::flip(int a, int b)
{
    int segment_size = abs(inv[a] - inv[b]) + 1;

    if (!(!reversed && (inv[a] < inv[b]) || reversed && (inv[a] > inv[b])))
    {
        if (segment_size == 2)
        {
            // reversed = !reversed;
            // return;
        }
        a = prev(a);
        b = next(b);
        segment_size = abs(inv[a] - inv[b]) + 1;
        reversed = !reversed;
    }

    if (segment_size > N / 2)
    {
        a = prev(a);
        b = next(b);
        reversed = !reversed;
        segment_size = N - segment_size;
    }

    for (int i = 0; i < segment_size / 2; i++)
    {
        swap(tour[inv[a]], tour[inv[b]]);
        int x = a, y = b;

        a = next(a);
        b = prev(b);

        swap(inv[x], inv[y]);
    }
}
bool Tour::isReversed() { return reversed; }

bool Tour::sequence(int a, int b, int c)
{
    if (!reversed)
    {
        if (inv[a] < inv[b] && inv[b] < inv[c])
            return true;
    }
    else
    {
        if (inv[c] < inv[b] && inv[b] < inv[a])
            return true;
    }
    return false;
}

void lkStep(Tour &T, double **c, vector<vector<int>> &neighbourSet)
{
    double delta = 0;

    T.print();

    int base = 1, a;
    vector<bool> taken;
    taken.assign(T.getN() + 1, false);

    while ((a = findPromisingVertex(T, c, base, delta, taken, neighbourSet)) != -1)
    {
        delta += c[base][T.next(base)] - c[T.next(base)][a] + c[T.prev(a)][a] - c[T.prev(a)][base];
        cout << "\n"
             << T.next(base) << " " << a << " " << T.getCost() - delta << "\n";

        T.flip(T.next(base), T.prev(a));
        // T.print();

        vi s = T.getTour();
        // for (int i = 1; i <= T.getN(); i++)
        // {
        //     cout << T.inverse(i) << " ";
        // }
        std::cout << "\n";
        // printSolucao(s);
        cout << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << "\n";
    }
}

int findPromisingVertex(Tour &T, double **c, int base, double delta, vector<bool> &taken, vector<vector<int>> &neighbourSet)
{
    double A = delta + c[base][T.next(base)];

    pair<double, int> best_a = {-INF, -1};

    int k = 13;

    for (int i = 1; i <= k; i++)
    {
        int a = neighbourSet[T.next(base) - 1][i];
        pair<double, int> cost = {c[T.prev(a)][a] - c[T.next(base)][a], a};
        if (taken[a] == false && cost > best_a && T.prev(a) != base && T.prev(a) != T.next(base) && T.prev(a) != T.prev(base))
        {
            best_a = cost;
        }
    }

    if (best_a.second != -1)
    {
        taken[best_a.second] = true;
    }

    return best_a.second;
}

void step(Tour &T, double **c, int base, int level, float delta, double &final_delta, vector<vector<int>> &neighbourSet, deque<pair<pair<int, int>, double>> &flipSequence, vector<bool> &taken)
{
    // cout << T.getCost() - delta << " " << level << "\n";
    int k = breadth(level);

    // cout << "delta: " << final_delta << "\n";

    // Create lk-ordering for base and next(base)
    vector<pair<pair<double, int>, bool>> lk_ordering;

    int MAX_NEIGHBORS = 10;

    // int lk_ordering_size = T.getN() - 3;
    int lk_ordering_size = 2 * MAX_NEIGHBORS;
    lk_ordering.assign(lk_ordering_size, {{std::numeric_limits<double>::infinity(), 0}, 0});

    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        int a = neighbourSet[T.next(base) - 1][i + 1];
        if (base == a || T.next(base) == a || T.next(base) == T.prev(a))
        {
            continue;
        }
        double greedy_cost = c[T.prev(a)][a] - c[T.next(base)][a];
        lk_ordering[i] = {{-greedy_cost, a}, false};
    }
    for (int i = MAX_NEIGHBORS, j = 0; i < lk_ordering_size; i++, j++)
    {
        int a = neighbourSet[base - 1][j + 1];
        if (base == a || T.next(base) == a || T.next(a) == base)
        {
            continue;
        }
        double greedy_cost = c[a][T.next(a)] - c[base][a];
        lk_ordering[i] = {{-greedy_cost, a}, true};
    }

    sort(lk_ordering.begin(), lk_ordering.end());

    for (int i = 0; i < k; i++)
    {
        // cout << lk_ordering[i].first.second << ", " << lk_ordering[i].second << "\n";
        // cout << "i = " << i << "\n";

        int a = lk_ordering[i].first.second;

        if (taken[a] == true)
        {
            continue;
        }
        // cout << base << " " << a << " " << lk_ordering[i].second << "\n";
        // T.print();

        double g = 0;

        if (lk_ordering[i].second == false)
        {
            taken[a] = true;

            g = (c[base][T.next(base)] - c[T.next(base)][a] + c[T.prev(a)][a] - c[T.prev(a)][base]);

            flipSequence.push_back({{T.next(base), T.prev(a)}, g});
            final_delta += g;

            // T.print();
            // cout << "AQUI: " << base << " " << T.next(base) << " " << a << " " << T.prev(a) << "\n\n";

            // cout << "(a, b): " << T.next(base) << " " << T.prev(a) << "\n";

            T.flip(T.next(base), T.prev(a));

            // cout << "Current level: " << level << "\n";
            // T.print();
            // cout << "\n\n";

            step(T, c, base, level + 1, delta + g, final_delta, neighbourSet, flipSequence, taken);
        }
        else
        {
            taken[a] = true;

            // T.print();
            // cout << "AQUI: " << base << " " << T.next(base) << " " << a << " " << T.next(a) << "\n";

            g = c[base][T.next(base)] - c[base][a] + c[a][T.next(a)] - c[T.next(a)][T.next(base)];
            // cout << "g: " << g << "\n\n";

            flipSequence.push_back({{T.next(a), base}, g});

            int newbase = T.next(a);

            final_delta += g;

            // cout << "(a, b): " << T.next(a) << " " << base << "\n";

            T.flip(T.next(a), base);

            // cout << "Current level: " << level << "\n";
            // T.print();
            // cout << "\n\n";

            step(T, c, newbase, level + 1, delta + g, final_delta, neighbourSet, flipSequence, taken);
        }

        if (final_delta > 0)
        {
            // cout << "Current level: " << level << "\n";
            // cout << "Better tour found\n";
            return;
        }
        else
        {
            // taken[a] = false;
            // cout << "Current level: " << level << "\n";
            // cout << "i <= i + 1\n";
            if (!flipSequence.empty())
            {
                pair<pair<int, int>, double> fl = flipSequence.back();
                T.flip(fl.first.second, fl.first.first);
                final_delta -= fl.second;
                // cout << final_delta << "\n";
                flipSequence.pop_back();
            }
        }
    }

    // cout << "Finished level " << level << "\n";
}

void alternate_step(Tour &T, double **c, int base, int level, float delta, double &final_delta, vector<vector<int>> &neighbourSet, deque<pair<pair<int, int>, double>> &flipSequence, vector<bool> &taken)
{
    // T.print();
    // cout << "\n";

    // Create the A-ordering
    vector<pair<double, int>> A_ordering;

    int MAX_NEIGHBORS = 10;

    A_ordering.assign(MAX_NEIGHBORS, {std::numeric_limits<double>::infinity(), 0});

    int A_ordering_size = 0;
    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        int a = neighbourSet[T.next(base) - 1][i + 1];
        if (base == a || T.next(base) == a || T.next(a) == base || (delta + c[base][T.next(base)] <= c[T.next(base)][T.next(a)]))
        {
            continue;
        }
        double cost = c[T.next(a)][a] - c[T.next(base)][a];
        A_ordering[i] = {-cost, a};
        A_ordering_size++;
    }
    sort(A_ordering.begin(), A_ordering.end());
    // A_ordering.erase(A_ordering.begin() + A_ordering_size, A_ordering.end());

    // iterate through the A-ordering
    int breadthA = 5;
    int breadthB = 5;
    int breadthD = 5;

    vector<pair<pair<double, pair<int, int>>, bool>> B_ordering;
    vector<pair<pair<double, pair<int, int>>, bool>> D_ordering;

    for (int i = 0; i < breadthA && i < A_ordering_size; i++)
    {
        // cout << A_ordering[i].first << " " << A_ordering[i].second << "\n";
        int a = A_ordering[i].second;
        int a1 = T.next(a);

        //Create the B-ordering from the neighbors of next(a)
        B_ordering.assign(2 * MAX_NEIGHBORS, {{std::numeric_limits<double>::infinity(), {0, 0}}, 0});

        int B_ordering_size = 0;
        for (int j = 0; j < 2 * MAX_NEIGHBORS && j < T.getN() - 1; j++)
        {
            int b = neighbourSet[T.next(a) - 1][j / 2 + 1];
            bool next = (j % 2 == 0);
            int b1 = next ? T.prev(b) : T.next(b);

            if (b == base || b == T.next(base) || b == a || b1 == base || b1 == T.next(base) || b1 == a || (c[T.next(a)][b] >= c[a][T.next(a)] + c[base][T.next(base)] - c[T.next(base)][a]))
            {
                continue;
            }

            double cost = c[b1][b] - c[T.next(a)][b];

            B_ordering[j] = {{-cost, {b, b1}}, next};

            B_ordering_size++;
        }

        sort(B_ordering.begin(), B_ordering.end());

        // cout << "\nB_ordering: " << B_ordering_size << "\n";
        // for (int j = 0; j < breadthB && j < B_ordering_size; j++)
        // {
        //     cout << B_ordering[j].first << " " << B_ordering[j].second.first << ", " << B_ordering[j].second.second << "\n";
        // }
        // cout << "\n";

        for (int j = 0; j < breadthB && j < B_ordering_size; j++)
        {
            int b = B_ordering[j].first.second.first;
            int b1 = B_ordering[j].first.second.second;
            // int b1 = B_ordering[j].second.second;
            int s1 = T.next(base);

            double altDelta1 = 0;
            double g;
            if (T.sequence(s1, T.next(b), a) == true)
            {
                deque<pair<int, int>> altSequence1;
                // T.print();

                B_ordering[j].second ? swap(b, b1), 0 : 0;

                // cout << "B: " << b << " " << a << " " << "\n";

                g = c[T.prev(s1)][s1] - c[s1][T.next(b)] + c[b][T.next(b)] - c[b][T.prev(s1)];
                altDelta1 += g;
                altSequence1.push_back({s1, b});
                flipSequence.push_back({{s1, b}, g});
                T.flip(s1, b);

                g = c[T.prev(b)][b] - c[b][T.next(a)] + c[a][T.next(a)] - c[T.prev(b)][a];
                altDelta1 += g;
                altSequence1.push_back({b, a});
                flipSequence.push_back({{b, a}, g});
                T.flip(b, a);
                // s1 = T.next(base);
                b1 = T.prev(b);

                g = c[T.prev(s1)][s1] - c[s1][T.next(a)] + c[a][T.next(a)] - c[T.prev(s1)][a];
                altDelta1 += g;
                altSequence1.push_back({s1, a});
                flipSequence.push_back({{s1, a}, g});
                T.flip(s1, a);

                g = c[T.prev(b)][b] - c[b][T.next(s1)] + c[s1][T.next(s1)] - c[T.prev(b)][s1];
                altDelta1 += g;
                altSequence1.push_back({b, s1});
                flipSequence.push_back({{b, s1}, g});
                T.flip(b, s1);

                g = c[T.prev(a)][a] - c[b1][T.prev(a)] + c[b1][T.next(b1)] - c[T.next(b1)][a];
                altDelta1 += g;
                altSequence1.push_back({a, b1});
                flipSequence.push_back({{a, b1}, g});
                T.flip(a, b1);

                // vector<int> s = T.getTour();
                // cout << T.getCost() - altDelta1 << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << " ||\n";

                double old_final_delta = final_delta;
                final_delta = altDelta1;
                step(T, c, base, 3, altDelta1, final_delta, neighbourSet, flipSequence, taken);

                if (final_delta > 0)
                {
                    return;
                }
                else
                {
                    final_delta = old_final_delta;
                    while (!altSequence1.empty())
                    {
                        pair<int, int> fl = altSequence1.back();
                        T.flip(fl.second, fl.first);
                        altSequence1.pop_back();
                        flipSequence.pop_back();
                    }
                }
            }
            else
            {
                //Create the D-ordering
                D_ordering.assign(2 * MAX_NEIGHBORS, {{std::numeric_limits<double>::infinity(), {0, 0}}, 0});
                int D_ordering_size = 0;

                for (int k = 0; k < 2 * MAX_NEIGHBORS && k < T.getN() - 1; k++)
                {
                    int d = neighbourSet[b1 - 1][k / 2 + 1];
                    bool next = ((k % 2) == 0);
                    int d1 = next ? T.next(d) : T.prev(d);

                    if (d == base || d == T.next(base) || d == a || d == T.next(a) || d1 == base || d1 == T.next(base) || d1 == a || d1 == T.next(a) || d == b || d1 == b || d == b1 || d1 == b1 || c[b1][d] >= c[b][b1] + c[base][T.next(base)] - c[T.next(base)][a] + c[a][T.next(a)] - c[T.next(a)][b])
                    {
                        continue;
                    }

                    double cost = c[d1][d] - c[b1][d];

                    D_ordering[k] = {{-cost, {d, d1}}, next};

                    D_ordering_size++;
                }

                sort(D_ordering.begin(), D_ordering.end());

                // T.print();
                for (int k = 0; k < breadthD && k < D_ordering_size; k++)
                {
                    int d = D_ordering[k].first.second.first;
                    int d1 = D_ordering[k].first.second.second;
                    // int d1 = T.next(d);
                    // cout << "b: " << b << "\n";
                    // cout << "(base, a): " << base << ", " << a << "\n";
                    // cout << "(d, d1): " << d << ", " << d1 << "\n\n";

                    !B_ordering[j].second ? swap(b, b1), 0 : 0;
                    !D_ordering[j].second ? swap(d, d1), 0 : 0;

                    // int b1 = T.prev(b);
                    int s1 = T.next(base);
                    int a1 = T.next(a);

                    // cout << "A\n";

                    double altDelta2 = 0;
                    double g;

                    deque<pair<int, int>> altSequence2;
                    // T.print();

                    // cout << "B: " << b << " " << a << " " << "\n";

                    g = c[T.prev(s1)][s1] - c[s1][T.next(b1)] + c[b1][T.next(b1)] - c[b1][T.prev(s1)];
                    altDelta2 += g;
                    altSequence2.push_back({s1, b1});
                    flipSequence.push_back({{s1, b1}, g});
                    T.flip(s1, b1);

                    g = c[T.prev(b1)][b1] - c[b1][T.next(d1)] + c[d1][T.next(d1)] - c[T.prev(b1)][d1];
                    altDelta2 += g;
                    altSequence2.push_back({b1, d1});
                    flipSequence.push_back({{b1, d1}, g});
                    T.flip(b1, d1);

                    g = c[T.prev(a1)][a1] - c[a1][T.next(s1)] + c[s1][T.next(s1)] - c[T.prev(a1)][s1];
                    altDelta2 += g;
                    altSequence2.push_back({a1, s1});
                    flipSequence.push_back({{a1, s1}, g});
                    T.flip(a1, s1);

                    // b1 = T.prev(b);
                    // d1 = T.prev(d);

                    // cout << T.inverse(s1) - T.inverse(d1) << "\n";
                    g = c[T.prev(s1)][s1] - c[s1][T.next(d1)] + c[d1][T.next(d1)] - c[T.prev(s1)][d1];
                    altDelta2 += g;
                    altSequence2.push_back({s1, d1});
                    flipSequence.push_back({{s1, d1}, g});
                    T.flip(s1, d1);

                    // g = c[T.prev(d)][d] - c[a][T.prev(d)] + c[a][T.next(a)] - c[T.next(a)][d];
                    // altDelta2 += g;
                    // altSequence2.push_back({d, a});
                    // flipSequence.push_back({{d, a}, g});
                    // T.flip(d, a);

                    // g = c[T.prev(a1)][a1] - c[b1][T.prev(a1)] + c[b1][T.next(b1)] - c[T.next(b1)][a1];
                    // altDelta2 += g;
                    // altSequence2.push_back({a1, b1});
                    // flipSequence.push_back({{a1, b1}, g});
                    // T.flip(a1, b1);

                    // vector<int> s = T.getTour();
                    // cout << T.getCost() - altDelta1 << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << " ||\n";

                    double old_final_delta = final_delta;
                    final_delta = altDelta2;
                    step(T, c, base, 4, altDelta2, final_delta, neighbourSet, flipSequence, taken);

                    if (final_delta > 0)
                    {
                        return;
                    }
                    else
                    {
                        final_delta = old_final_delta;
                        while (!altSequence2.empty())
                        {
                            pair<int, int> fl = altSequence2.back();
                            T.flip(fl.second, fl.first);
                            altSequence2.pop_back();
                            flipSequence.pop_back();
                        }
                    }
                }
            }
        }
    }
}

bool lk_search(Tour &T, int v, double **c, vector<vector<int>> &neighbourSet, deque<pair<pair<int, int>, double>> &flipSequence)
{
    Tour current_tour = T;

    vector<bool> taken;
    taken.assign(T.getN() + 1, false);

    double delta = 0;

    step(current_tour, c, v, 1, 0, delta, neighbourSet, flipSequence, taken);

    current_tour.setCost(T.getCost() - delta);

    if (current_tour.getCost() < T.getCost())
    {
        // T.setCost(current_tour.getCost());
        return true;
    }
    else
    {
        delta = 0;
        taken.assign(T.getN() + 1, false);
        alternate_step(current_tour, c, v, 1, 0, delta, neighbourSet, flipSequence, taken);
        current_tour.setCost(current_tour.getCost() - delta);

        // vector<int> s = current_tour.getTour();
        // cout << current_tour.getCost() << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << " |\n";

        if (current_tour.getCost() < T.getCost())
        {
            return true;
        }
        return false;
    }
}

void lin_kerninghan(Tour &T, Tour &lk_tour, double **c, vector<vector<int>> &neighbourSet)
{
    lk_tour = T;
    vector<bool> marked;
    marked.assign(T.getN(), true);
    int markedVertices = T.getN() + 1;

    // int k = 2;
    while (true)
    {
        int v;
        for (v = 1; v <= T.getN() && marked[v] == false; v++)
        {
        }

        if (v == T.getN() + 1)
            break;

        deque<pair<pair<int, int>, double>> flipSequence;

        bool flag = lk_search(lk_tour, v, c, neighbourSet, flipSequence);

        if (flag == true)
        {
            while (!flipSequence.empty())
            {
                int x = flipSequence.front().first.first;
                int y = flipSequence.front().first.second;
                double g = flipSequence.front().second;

                lk_tour.flip(x, y);
                lk_tour.setCost(lk_tour.getCost() - g);

                marked[x] = marked[y] = true;

                flipSequence.pop_front();
            }
        }
        else
        {
            marked[v] = false;
        }

        // lk_tour.print();
        vector<int> s = lk_tour.getTour();
        cout << lk_tour.getCost() << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << " |\n";
    }

    return;
}

void kick(Tour &T, double **c, double &delta, vector<pair<int, pair<int, int>>> t, deque<pair<int, int>> &kickFlips)
{

    vector<int> s = T.getTour();
    int reversed = (T.isReversed() == true) ? -1 : 1;

    for (int i = 0; i < 4; i++)
    {
        int index = rand() % T.getN();
        while (s[index] == -1)
        {
            index = (index == T.getN() - 1) ? 0 : index + 1;
        }
        int a = s[index];
        s[T.inverse(a)] = s[T.inverse(T.prev(a))] = s[T.inverse(T.next(a))] = -1;
        t[i] = {reversed * index, {a, T.next(a)}};
    }

    sort(t.begin(), t.end());

    T.flip(t[0].second.second, t[2].second.first);
    kickFlips.push_back({t[0].second.second, t[2].second.first});
    T.flip(t[3].second.second, t[1].second.second);
    kickFlips.push_back({t[3].second.second, t[1].second.second});
    T.flip(t[0].second.first, t[0].second.second);
    kickFlips.push_back({t[0].second.first, t[0].second.second});

    double delta1 = c[t[0].second.first][t[0].second.second] + c[t[1].second.first][t[1].second.second] + c[t[2].second.first][t[2].second.second] + c[t[3].second.first][t[3].second.second];
    double delta2 = c[t[0].second.first][t[2].second.second] + c[t[0].second.second][t[2].second.first] + c[t[3].second.second][t[1].second.first] + c[t[3].second.first][t[1].second.second];
    delta = delta1 - delta2;
}

void Chained_Lin_Kerninghan(Tour &S, double **c, vector<vector<int>> &neighbourSet)
{
    Tour T = S, T1 = S;
    lin_kerninghan(S, T, c, neighbourSet);
    // T.print();
    // vector<int> s = T.getTour();
    // cout << T.getCost() << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << "\n";

    int N_ITER, MAX_ITERATIONS = 100;

    while (N_ITER++ < MAX_ITERATIONS)
    {
        deque<pair<int, int>> kickFlips;
        vector<pair<int, pair<int, int>>> t;
        t.assign(4, {0, {0, 0}});
        double delta = 0;

        kick(T, c, delta, t, kickFlips);
        // T.print();

        // cout << delta << "\n\n";

        T.setCost(T.getCost() - delta);

        lin_kerninghan(T, T1, c, neighbourSet);

        // cout << T.getCost() << "\n";
        // cout << T1.getCost() << "\n\n";

        if (T1.getCost() < T.getCost())
        {
            T = T1;
        }

        else
        {
            // cout << "E\n";
            while (!kickFlips.empty())
            {
                pair<int, int> flip = kickFlips.back();
                kickFlips.pop_back();

                T.flip(flip.second, flip.first);
            }

            T.setCost(T.getCost() + delta);
        }
    }

    T.print();
    vector<int> s = T.getTour();
    cout << T.getCost() << ", " << calcularValorObj(s, c) + c[s[s.size() - 1]][s[0]] << "\n";
}