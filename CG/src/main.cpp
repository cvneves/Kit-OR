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

    Problema p(data);

    Node raiz;
    raiz.is_root = true;

    // std::pair<int, int> branchingPair = {1,2};
    std::pair<int, int> branchingPair = p.solve(raiz);

    Node n1, n2;
    n1.juntos.push_back(branchingPair);
    n1.tipo_branch = true;
    n2.separados.push_back(branchingPair);
    n2.tipo_branch = false;

    std::list<Node> tree = {n1, n2};
    auto node_it = tree.begin();

    int k = 1;
    while (k--)
    {
        node_it = tree.end();
        node_it--;

        branchingPair = p.solve(*node_it);

        if (!(branchingPair.first == branchingPair.second == 0))
        {
            Node nj, ns;
            nj = ns = *node_it;

            nj.juntos.push_back(branchingPair);
            nj.tipo_branch = true;
            ns.separados.push_back(branchingPair);
            ns.tipo_branch = false;

            tree.push_back(nj);
            tree.push_back(ns);
        }

        tree.erase(node_it);
    }

    // master.exportModel("modelo.lp");

    // env.end();

    //Solve
    // std::vector<std::vector<int>> patterns;
    // solve(data, patterns);

    return 0;
}
