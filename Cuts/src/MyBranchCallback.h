#ifndef MY_BRANCH_CALLBACK_H
#define MY_BRANCH_CALLBACK_H

#include <ilcplex/ilocplex.h>
#include "NodeInfo.h"
#include <mutex>

class MyBranchCallback : public IloCplex::BranchCallbackI
{
public:
    MyBranchCallback(IloEnv env);
    IloCplex::CallbackI *duplicateCallback() const;
    void main();
    static std::mutex lazyMutex;
};

#endif