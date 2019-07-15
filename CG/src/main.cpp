#include "Data.h"
#include "Problema.h"
#include "Node.h"
#include <stdio.h>
#include <iostream>
#include <list>
#include <ilcplex/ilocplex.h>
#include <chrono>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage:\n./bin instance\n");
        return 0;
    }

    Data data;
    data.readData(argv[1]);

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    Problema p(data, std::numeric_limits<double>::infinity());

    Node raiz;
    raiz.is_root = true;
    raiz.separados = {};
    raiz.juntos = {};

    p.solve(raiz);

    end = std::chrono::system_clock::now();

    int elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Elapsed time (s): " << elapsed_seconds / 1000.0 << "\n\n";


    return 0;
}
