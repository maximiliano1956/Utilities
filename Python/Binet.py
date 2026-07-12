#!/usr/bin/env python3

import math

f1=(1-math.sqrt(5))/2
f2=(1+math.sqrt(5))/2

def f(k):
    return 1/math.sqrt(5)*(f2**k-f1**k)

n=10

for i in range(0,n+1):
    print("F(",i,")=",int(f(i)))
