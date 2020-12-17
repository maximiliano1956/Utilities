#!/usr/bin/env python3

import math, sys, getopt, time
from random import randint, seed
from _ast import arg
#
# Mescola una mazzo random e ogni permutazione
# ottenuta viene contata; viene valutata la varianza
#
# Simulazione
#        
def DoSim(n):
    
    prove = {}
    
    nsm = 600000

    deck = [0 for i in range(n)]

    seed()
    
    for np in list(range(nsm)):
        for i in list(range(n)):
            deck[i] = i + 1
#
# Fisher-Yates shuffle
#            
        i = n - 1
        while i > 0:
            pos = randint(0,i)
            temp = deck[pos]
            deck[pos] = deck[i]
            deck[i] = temp
            i -= 1
#
# Fine Fisher-Yates shuffle
#       
        ind = int("".join(map(str,deck)))
    
        if ind in prove:
            prove[ind] += 1
        else:
            prove[ind] = 1
            
    vm = 1 / math.factorial(n)
    
    sqm = 0
        
    for key in prove:
        print("permutazione=", key, "\tcounter=", prove[key])
        fr = prove[key] / nsm
        sqm += ((fr - vm) ** 2)
        
    varianza = math.sqrt(sqm)
 
    print("sqm=", sqm)
    
DoSim(4)

