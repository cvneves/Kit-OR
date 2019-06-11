#include "NodeInfo.h"

NodeInfo::NodeInfo()
{
    iterations = 0;
}

int NodeInfo::getIterations() {return iterations;}

void NodeInfo::addIteration() {iterations++;}

void NodeInfo::initRootData()
{
   rootData = new NodeInfo();
}

NodeInfo* NodeInfo::rootData = NULL;