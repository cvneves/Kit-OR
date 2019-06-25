#include "MyBranchCallback.h"

MyBranchCallback::MyBranchCallback(IloEnv env) : IloCplex::BranchCallbackI(env)
{
}

IloCplex::CallbackI *MyBranchCallback::duplicateCallback() const
{
    return new (getEnv()) MyBranchCallback(getEnv());
}

void MyBranchCallback::main()
{
    lazyMutex.lock();

    IloInt const nbranch = getNbranches();

    if (nbranch > 0)
    {
        NodeInfo *data = dynamic_cast<NodeInfo *>(getNodeData());

        if (!data)
        {
            if (NodeInfo::rootData == NULL)
            {
                NodeInfo::initRootData();
            }
            data = NodeInfo::rootData;

            // data->setNodeObjValue(getObjValue());

            // MyBranchCallback::branched = true;
        }

        IloNumVarArray vars(getEnv());
        IloNumArray bounds(getEnv());
        IloCplex::BranchDirectionArray dirs(getEnv());

        for (IloInt i = 0; i < nbranch; ++i)
        {
            IloNum const est = getBranch(vars, bounds, dirs, i);
            makeBranch(vars, bounds, dirs, est, new NodeInfo(data->depth + 1));
        }

        dirs.end();
        bounds.end();
        vars.end();
    }
    else
    {
        prune();
    }


    lazyMutex.unlock();
}

std::mutex MyBranchCallback::lazyMutex;