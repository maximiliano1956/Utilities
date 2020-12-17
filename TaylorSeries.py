#!/usr/bin/env python3

import sympy as sy
import math
#
# Calcola ricorsivamente le combinazioni
# di n elementi a gruppi di k
#
def Comb(n,k):
    if n == 0:
        return 1
    if k == 0 or k == n:
        return 1
    if k == 1 or k == (n - 1):
        return n
    return Comb(n - 1, k -1) + Comb(n - 1, k)
#
# Calcola il numero di Bell
#
def Stirling2(m, n):
    somma = 0
    for k in list(range(n + 1)):
        somma += ((-1) ** k) * Comb(n, k) * ((n - k) ** m)
    return int(somma / math.factorial(n))

x = sy.Symbol('x')
#
# Questa e' la funzione generatrice di S(n,5)
#
# Sviluppandola in serie di Taylor nel punto 0
# avremo che il coefficiente di x ** n
# e' S(n,5)
#
f = x**5/((1-x)*(1-2*x)*(1-3*x)*(1-4*x)*(1-5*x))
#
# Fibonacci
# f = x/(1-x-x**2)
#
x0 = 0
n = 10
i = 0
while i <= n:
    ti = f.diff(x, i).subs(x, x0)/math.factorial(i)
    print("X **", i, "coefficient=", ti)
    if (ti == Stirling2(i,5)):
        print("Okay: equal to S(", i, ",", 5, ")")
    else:
        print("Not good: S(", i, ",", 5, ")=", Stirling2(i,5))
    print("")
    i += 1
