#ifndef NODEINFO_H
#define NODEINFO_H

#include <ilcplex/ilocplex.h>

class NodeInfo : public IloCplex::MIPCallbackI::NodeData
{
    int iterations;

public:
    NodeInfo();
    int getIterations();
    void addIteration();
    static NodeInfo *rootData;
    static void initRootData();
};

#endif