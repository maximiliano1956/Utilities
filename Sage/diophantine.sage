#!/usr/bin/sage

R.<a,b,c> = QQ[]

n = 46
m = 201
n = 38
m = 659
F = a*(a+b)*(c+a)+b*(a+b)*(b+c)+c*(b+c)*(c+a) - n*(a+b)*(b+c)*(c+a)
O = [1,-1,0]
E = EllipticCurve_from_cubic(F,O,morphism = False)
f = EllipticCurve_from_cubic(F,O,morphism = True)
finv = f.inverse()
E1=E.gens()
P=finv(m*E1[0])
Q=P[0].denominator()
a=Q*P[0]
b=Q*P[1]
c=Q*P[2]
print("a=",a)
print("====")
print("b=",b)
print("====")
print("c=",c)
print("====")
print(a/(b+c)+b/(a+c)+c/(a+b),n)
la=len(str(a))
lb=len(str(b))
lc=len(str(c))
m=la
if (lb>m):
  m=lb
  if (lc>m):
    m=lc
print(m)
if a<0:
  print("a<0")
if b<0:
  print("b<0")
if c<0:
  print("c<0")
