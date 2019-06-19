#ifndef MYCUTCALLBACK_H
#define MYCUTCALLBACK_H

#include <ilcplex/ilocplex.h>
#include <vector>
#include "Graph.h"
#include "MinimumCut.h"
#include "MaxBack.h"
#include <mutex>

class MyCutCallback : public IloCplex::UserCutCallbackI
{
public:
  MyCutCallback(IloEnv env, const IloBoolVarArray &x_ref, Graph *G);
  IloCplex::CallbackI *duplicateCallback() const;
  void main();

  IloConstraint *separate();
  static std::mutex lazyMutex;

private:
  IloBoolVarArray x;
  Graph *G;
};

#endif