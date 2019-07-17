\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: 1000000000000 x1 + x2 + x3 + x4 + 1000000000000 x5 + x6 + x7
      + 1000000000000 x8 + 1000000000000 x9 + x10 + 1000000000000 x11 + x12
Subject To
 c1: x1 + x2 + x3 + x4  = 1
 c2: x3 + x5 + x6 + x7  = 1
 c3: x2 + x6 + x8  = 1
 c4: x9 + x10  = 1
 c5: x11 + x12  = 1
Bounds
      x2 = 0
      x4 = 0
      x6 = 0
      x7 = 0
End
