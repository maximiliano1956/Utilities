#!/usr/bin/env python3

import math, random, time, sys

random.seed()

t = time.time()
nrast = 0

np = 0
fav = 0

while np < 10000000:
    x = random.random()
    y = random.random()
    
    if x ** 2 + y ** 2 < 1:
        fav += 1
        
    np += 1

    if time.time() - t > 1:
        print("*",end="")
        sys.stdout.flush()
        t = time.time()
        nrast = (nrast + 1) % 30
        if nrast == 0:
            print("");
            
freq = fav / np
            
print("\nFrequenza =", freq)
print("\nPi valutato =", 4 * freq)
print("\nPi teorico =", math.pi)
print("Scarto =", math.fabs(4 * freq - math.pi))
