#include "NodeInfo.h"

NodeInfo::NodeInfo(int d)
{
    iterations = 0;
    depth = d;
}

int NodeInfo::getIterations() {return iterations;}

void NodeInfo::addIteration() {iterations++;}

void NodeInfo::initRootData()
{
   rootData = new NodeInfo(0);
}

NodeInfo* NodeInfo::rootData = NULL;