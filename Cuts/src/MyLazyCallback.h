#ifndef MYLAZYCALLBACK_H
#define MYLAZYCALLBACK_H

#include <ilcplex/ilocplex.h>
#include <vector>
#include "Graph.h"
#include "MaxBack.h"
#include <mutex>

class MyLazyCallback : public IloCplex::LazyConstraintCallbackI
{
public:
  MyLazyCallback(IloEnv env, const IloBoolVarArray &x_ref, Graph *G);
  IloCplex::CallbackI *duplicateCallback() const;
  void main();

  std::vector<IloConstraint> *separate();
  static std::mutex lazyMutex;

private:
  IloBoolVarArray x;
  Graph *G;
};
#endif