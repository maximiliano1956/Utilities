#!/usr/bin/sage

n=var('n')
P=n^17+9
P.parent()
Q=(n+1)^17+9
res=P.resultant(Q,n)
print("res=",res)

x,y,z=var('x,y,z')
assume(x,y,z,'integer')
s=solve([x^17+9==res*y,(x+1)^17+9==res*z],x,y,z)
print(s)
