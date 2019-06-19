#include "MyLazyCallback.h"
#include "NodeInfo.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <stdlib.h>
#include <stack>
#include <algorithm>
#include <exception>
#include <list>

#define EPSILON 0.000001

MyLazyCallback::MyLazyCallback(IloEnv env, const IloBoolVarArray &x_ref, Graph *Gr) : IloCplex::LazyConstraintCallbackI(env), x(env), G(Gr)
{
    x = x_ref;
}

IloCplex::CallbackI *MyLazyCallback::duplicateCallback() const
{
    return new (getEnv()) MyLazyCallback(getEnv(), x, G);
}

void MyLazyCallback::main()
{
    lazyMutex.lock();

    std::vector<IloConstraint> *cons = separate();
    for (int i = 0; i < cons->size(); i++)
    {
        add((*cons)[i]);
    }

    delete cons;

    lazyMutex.unlock();
}

std::vector<IloConstraint> *MyLazyCallback::separate()
{

    std::vector<IloConstraint> *constraints = new std::vector<IloConstraint>();

    IloNumArray x_vals(getEnv(), G->getNumEdges());
    getValues(x_vals, x);

    // std::vector<double> x_vals(G->getNumEdges());
    // for(int i = 0; i < G->getNumEdges();i++)
    // {
    //     x_vals[i] = getValue(x[i]);
    // }

    std::vector<std::vector<int>> m(G->getNumNodes(), std::vector<int>(G->getNumNodes()));

    std::vector<std::pair<int, int>> edges(G->getNumNodes());

    for (int i = 0, j = 0; i < G->getNumEdges(); i++)
    {
        if (x_vals[i] > 1 - EPSILON)
        {
            edges[j].first = G->getEdges()[i].i;
            edges[j].second = G->getEdges()[i].j;
            j++;
        }
    }

    for (int i = 0, l = 0; i < G->getNumNodes(); i++)
    {
        for (int j = i + 1; j < G->getNumNodes(); j++, l++)
        {
            G->setW(i, j, (int)(x_vals[l] > 1 - EPSILON ? 1 : 0));
        }
    }


    // x_vals.end();

    std::vector<bool> S1(G->getNumNodes(), false);
    double mb = maxBack(G, 0, S1);
    int S1_size = 0;

    if (mb >= 2 + EPSILON)
    {
        std::cout << "\nAA\n";
        return constraints;
    }

    while (edges.empty() == false)
    {
        std::vector<int> S;
        int node = edges[0].first;
        bool found = false;

        while (1)
        {
            auto it = edges.begin();
            for (; it != edges.end(); it++)
            {
                if (it->first == node)
                {
                    node = it->second;
                    S.push_back(node);
                    edges.erase(it);
                    found = true;
                    break;
                }
                if (it->second == node)
                {
                    node = it->first;
                    S.push_back(node);
                    edges.erase(it);
                    found = true;
                    break;
                }
            }
            if (found == false)
                break;
            found = false;
        }

        IloExpr sumS(getEnv());
        int S_size = S.size();

        if (S_size == G->getNumNodes())
        {
            return constraints;
        }

        for (int i = 0; i < S_size; i++)
        {
            for (int j = i + 1; j < S_size; j++)
            {
                sumS += x[G->getEdge(S[i], S[j])];
            }
        }

        constraints->push_back(sumS <= S_size - 1);
    }


    return constraints;
}

std::mutex MyLazyCallback::lazyMutex;