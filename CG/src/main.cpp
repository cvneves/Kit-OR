#include "Data.h"
#include "GC.h"
#include <stdio.h>
#include <iostream>
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

    //Construçao do subproblema


    //Solve
    std::vector<std::vector<int>> patterns;
    solve(data, patterns);


    return 0;
}
