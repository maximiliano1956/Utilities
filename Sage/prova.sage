#!/usr/bin/sage
R.<a,b,c> = QQ[]

n = 4
F = a*(a+b)*(c+a)+b*(a+b)*(b+c)+c*(b+c)*(c+a) - n*(a+b)*(b+c)*(c+a)
O = [1,-1,0]
E = EllipticCurve_from_cubic(F,O,morphism = False)
print(E)
f = EllipticCurve_from_cubic(F,O,morphism = True)
print(f)
finv = f.inverse()
print(finv)
E1=E.gens()
print(E1)
P=finv(9*E1[0])
Q=P[0].denominator()
a=Q*P[0]
b=Q*P[1]
c=Q*P[2]
print(a)
print(b)
print(c)
print(a/(b+c)+b/(a+c)+c/(a+b),n)
