#include "Data.h"
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

   solve(data);

   return 0;
}

