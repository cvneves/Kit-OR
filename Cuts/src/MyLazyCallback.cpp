#include "MyLazyCallback.h"

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
    std::vector<IloConstraint> *cons = separate();
    for (int i = 0; i < cons->size(); i++)
    {
        add((*cons)[i]);
    }
    delete cons;
}

std::vector<IloConstraint> *MyLazyCallback::separate()
{
    std::vector<IloConstraint> *constraints = new std::vector<IloConstraint>();

    IloNumArray x_vals(getEnv(), G->E());
    getValues(x_vals, x);

    std::vector<std::vector<int>> m(G->V(), std::vector<int>(G->V()));

    std::vector<std::pair<int, int>> edges(G->V());

    for (int i = 0, j = 0; i < G->E(); i++)
    {
        if (x_vals[i] > 1 - EPSILON)
        {
            edges[j].first = G->getEdges()[i].i;
            edges[j].second = G->getEdges()[i].j;
            j++;
        }
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

        if(S_size == G->V())
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
