#include "Data.h"
#include "Problema.h"
#include "Node.h"
#include <stdio.h>
#include <iostream>
#include <list>
#include <ilcplex/ilocplex.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage:\n./bin instance\n");
        return 0;
    }

    Data data;
    data.readData(argv[1]);

    Problema p(data, std::numeric_limits<double>::infinity());

    Node raiz;
    raiz.is_root = true;

    // std::pair<int, int> branchingPair = {1,2};
    std::pair<int, int> branchingPair = p.solve(raiz);

    Node n1, n2;
    n1.juntos.push_back(branchingPair);
    n1.tipo_branch = true;
    n2.separados.push_back(branchingPair);
    n2.tipo_branch = false;
    int numNodes = 2;

    std::list<Node> tree = {n2, n1};
    auto node_it = tree.begin();

    int k = 100;
    while (!tree.empty())
    {
        node_it = tree.end();
        node_it--;

        branchingPair = p.solve(*node_it);

        if (!(branchingPair.first == 0 && branchingPair.second == 0))
        {
            numNodes += 2;
            Node nj, ns;
            nj = ns = *node_it;

            nj.juntos.push_back(branchingPair);
            nj.tipo_branch = true;
            ns.separados.push_back(branchingPair);
            ns.tipo_branch = false;

            tree.push_back(ns);
            tree.push_back(nj);
        }

        std::cout << "Num nodes: " << numNodes << "\n";
        std::cout << "best integer: " << p.bestInteger << "\n";

        tree.erase(node_it);
    }

    // master.exportModel("modelo.lp");

    // env.end();

    //Solve
    // std::vector<std::vector<int>> patterns;
    // solve(data, patterns);

    return 0;
}
