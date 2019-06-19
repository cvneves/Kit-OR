#include "MyCutCallback.h"
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

#define MAX_ITER 100
#define MB_DEPTH 10

MyCutCallback::MyCutCallback(IloEnv env, const IloBoolVarArray &x_ref, Graph *Gr) : IloCplex::UserCutCallbackI(env), x(env), G(Gr)
{
    x = x_ref;
}

IloCplex::CallbackI *MyCutCallback::duplicateCallback() const
{
    return new (getEnv()) MyCutCallback(getEnv(), x, G);
}

void MyCutCallback::main()
{
    lazyMutex.lock();

    NodeInfo *data = dynamic_cast<NodeInfo *>(getNodeData());

    if (!data)
    {
        if (NodeInfo::rootData == NULL)
        {
            NodeInfo::initRootData();
        }
        data = NodeInfo::rootData;
    }

    if (data->getIterations() >= MAX_ITER)
    {

        lazyMutex.unlock();
        return;
    }

    IloNumArray x_vals(getEnv(), G->getNumEdges());
    // IloNumArray x_vals(getEnv(), (130*130-130)/2);

    getValues(x_vals, x);

    Graph *H = G;

    for (int i = 0, l = 0; i < H->getNumNodes(); i++)
    {
        for (int j = i + 1; j < H->getNumNodes(); j++, l++)
        {
            H->getEdges()[l].w = x_vals[l];
        }
    }

    x_vals.end();

    std::vector<bool> S(H->getNumNodes(), false);

    double minCutCost;

    minCutCost = maxBack(H, 0, S);
    if (minCutCost - 2 > EPSILON && data->depth < MB_DEPTH)
    {
        minCutCost = minimumCut(H, 0, S);
    }

    int l = 0;
    for (int i = 0; i < S.size(); i++)
    {
        if (S[i] == true)
        {
            l++;
        }
    }

    if ((l >= 3 && l <= S.size() - 3) && minCutCost - 2 <= EPSILON)
    {
        IloExpr sum(getEnv());
        for (int i = 0; i < S.size(); i++)
        {
            if (S[i] == true)
            {
                for (int j = 0; j < S.size(); j++)
                {
                    if (S[j] == false)
                    {
                        sum += x[H->getEdge(i, j)];
                        // std::cout << i << ", " << j << "\n";
                    }
                }
            }
        }
        add(sum >= 2);
        sum.end();
    }
    data->addIteration();

    lazyMutex.unlock();
}

IloConstraint *MyCutCallback::separate()
{
}

std::mutex MyCutCallback::lazyMutex;