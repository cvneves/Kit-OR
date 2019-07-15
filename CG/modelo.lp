\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 1000000000000 x1 + 1000000000000 x2 + 1000000000000 x3 + 1000000000000 x4
      + 1000000000000 x5 + x6 + x7 + x8 + x9 + x10
Subject To
 c1: x1 + x6 + x7  = 1
 c2: x2 + x7 + x8  = 1
 c3: x3 + x6 + x8  = 1
 c4: x4 + x9  = 1
 c5: x5 + x10  = 1
End
