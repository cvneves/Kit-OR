\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj: - 0.5 x1 - 0.5 x2 - 0.5 x3 - x4 - x5 + 1
Subject To
 c1: 3 x1 + 4 x2 + 6 x3 + 8 x4 + 9 x5 <= 10
 c2: 2 x1 <= 1
Bounds
 0 <= x1 <= 1
 0 <= x2 <= 1
 0 <= x3 <= 1
 0 <= x4 <= 1
 0 <= x5 <= 1
Binaries
 x1  x2  x3  x4  x5 
End
