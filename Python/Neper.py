#!/usr/bin/env python3

import math, random, time, sys

random.seed()

lim = 1     # Compreso tra 0 e 1

media = 0
np = 0

t = time.time()
nrast = 0

while np < 10000000:
    somma = 0
    cont = 0
    
    while somma < lim:
        somma += (random.random())
        cont += 1
    
    media += cont
    np += 1
    
    if time.time() - t > 1:
        print("*",end="")
        sys.stdout.flush()
        t = time.time()
        nrast = (nrast + 1) % 30
        if nrast == 0:
            print(""); 
    
freq = media / np
    
print("\nFrequenza =", freq)
print("e **",lim," =", math.e ** lim)
print("Scarto =", math.fabs(freq - math.e ** lim))
