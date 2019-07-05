\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8
Subject To
 c1: x1 + x6 + x7  = 1
 c2: x2 + x7 + x8  = 1
 c3: x3 + x6 + x8  = 1
 c4: x4  = 1
 c5: x5  = 1
Bounds
      x7 = 0
End
