#ifndef MY_BRANCH_CALLBACK_H
#define MY_BRANCH_CALLBACK_H

#include <ilcplex/ilocplex.h>
#include "NodeInfo.h"

class MyBranchCallback : public IloCplex::BranchCallbackI
{
public:
    MyBranchCallback(IloEnv env);
    IloCplex::CallbackI *duplicateCallback() const;
    void main();
};

#endif