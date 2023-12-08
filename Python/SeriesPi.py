#!/usr/bin/env python3

import math
from sympy import factorint

def sgn1(n):
    if (n==2 or n==3 or n%4==1):
        return 1
    else:
        return -1

def sgn2(n):
    prod=1
    fact_dict=factorint(n)
    factors=list(fact_dict.keys())
    exps=list(fact_dict.values())
    for p in range(0,len(factors)):
        if sgn1(factors[p])==-1:
            if exps[p]%2==1:
                prod=-prod
    return prod

nloop=2000000

sum=0

for n in range(1,nloop+1):
    sum=sum+sgn2(n)/n

print(sum)

print(math.pi)
