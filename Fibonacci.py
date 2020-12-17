#!/usr/bin/env python3

import numpy as np
import math
import getopt
import sys
#
# Help usage
#
def Help():
    print("""=================================================
Questo programma implementa le successioni di Fibonacci
a n step, partendo da 2 valori iniziali.
Vengono generati vari elementi della successione, viene
valutato il rapporto tra gli ultimi 2 termini consecutivi
e lo scarto rispetto il valore limite calcolato
=================================================""")
    print("\nArguments: \t-nstep [--step=step]",
          "-ainita [--inita=inita] -binitb [--initb=initb]")
    print("\t\t-seln [--seln=seln]",
          "-v [--verbose] -h [--help]")     
#
# Main
#

#
# input di default
#
n_dflt = 2
inita_dflt = 0
initb_dflt = 1
npr_dflt = 100
verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "n:a:b:s:vh",
                    ["step=","inita=","initb"
                     "seln=", "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
n = n_dflt
inita = inita_dflt
initb = initb_dflt
npr = npr_dflt
verb = verb_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-n", "--step"):
        n = int(arg)
    elif opt in ("-a", "--inita"):
        inita = int(arg)
    elif opt in ("-b", "--initb"):
        initb = int(arg)
    elif opt in ("-s", "--seln"):
        npr = int(arg)
#
# Controllo valori
#
if n < 2:
    n = n_dflt
if npr < 0:
    npr = npr_dflt
if verb != True and verb != False:
    verb = False

print("\nAssuming:\n")

print("Step =\t\t\t",n)
print("First element =\t\t",inita)
print("Second element =\t",initb)
print("Number of elements =\t",npr)
print("Verbose =\t\t",verb,"\n")

print("\nLet's begin")
print("===========\n\n")

fibo = {}

fibo[0] = inita
fibo[1] = initb

coefs = [0 for i in range(n + 1)]

for i in list(range(n + 1)):
    if i == 0:
        val = 1
    else:
        val = -1
    coefs[i] = val
    
roots = np.roots(coefs)

init = 1
maxr = 0
for i in list(range(len(roots))):
    if (math.fabs(np.imag(roots[i])) < 0.001):
        if (init == 1 or maxr < np.real(roots[i])):
            maxr = np.real(roots[i])
            init = 0

print("Limit ratio=", maxr)
print("")

i = 2
while i < n:
    j = 0
    somma = 0
    while j < i:
        somma += fibo[j]
        j += 1
    fibo[i] = somma
    i += 1

if verb == True:
    print("Init values", list(fibo.values()))
    print("")

i = n
while i < npr + 2:
    fibo[i] = 0
    for j in list(range(n)):
        fibo[i] += fibo[i - j - 1]
    if verb == True and fibo[i - 1] != 0:
        rapp = fibo[i] / fibo[i - 1]
        print("index=\t", i, "\tratio =\t", 
              rapp, "\tratio deviation=",
              math.fabs(maxr - rapp))
    i += 1

if verb == True:
    print("")
    print("Succession elements", list(fibo.values()))
    print("")

if fibo[npr] != 0:
    rapp = fibo[npr + 1] / fibo[npr]
    print("last ratio= ",fibo[npr + 1], "/",
          fibo[npr], "=", rapp)
    print("\ndeviation=", math.fabs(maxr - rapp))
