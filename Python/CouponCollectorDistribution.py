#!/usr/bin/env python3

import math
import sys
import getopt
import time
import numpy as np
import random
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
# Calcola ricorsivamente le disposizioni
# di n elementi a k a k
#
def Disp(n,k):
    if k == 0:
        return 1
    
    return n * Disp(n - 1,k - 1)
#
# Calcola il numero di Bell
#
def Stirling2(m, n):
    somma = 0
    for k in list(range(n + 1)):
        somma += ((-1) ** k) * Comb(n, k) * ((n - k) ** m)
    return int(somma / math.factorial(n))
#
# Help
#
def Help():
    print("""=================================================
Questo applicativo calcola la distribuzione di probabilita'
relativa al problema "coupon collector" (collezionista di figurine)
Dati n coupon diversi in un urna, si effettuano estrazioni
con reinserimento.
Si effettuano le estrazioni e ci si ferma quando sono stati
estratti tutti i coupon.
IL numero di estrazioni e' una variabile casuale discreta.
Viene valutata la sua distribuzione e viene effettuata
una simulazione per vedere gli scarti rispetto i valori
teorici
=================================================""")
    print("\nArguments: \t-nsimboli [--simboli=simboli]")
    print("\t\t-mestrazioni [--estrazioni=estrazioni]")
    print("\t\t-ttipo [--tipo=tipo]")
    print("\t\t-ssimulazioni [--simulazioni=simulazioni]")
    print("\t\t -v [--verbose] -h [--help]")     
#
# Main
#
#
# input di default
#
n_dflt = 5
m_dflt = 7
type_s_dflt = 0
nsm_dflt = 10000
verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
            "n:m:t:s:vh",
            ["simboli=","estrazioni=","tipo=","simulazioni=",
            "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
n = n_dflt
m = m_dflt
type_s = type_s_dflt
nsm = nsm_dflt
verb = verb_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-n", "--simboli"):
        n = int(arg)
    elif opt in ("-m", "--estrazioni"):
        m = int(arg)
    elif opt in ("-t", "--tipo"):
        type_s = int(arg)
    elif opt in ("-s", "--simulazioni"):
        nsm = int(arg)
#
# Controllo valori
#
if n < 1:
    n = n_dflt
if m < 1:
    m = m_dflt
if type_s > 1 or type_s < 0:
    type_s = type_s_dflt
if nsm < 0:
    nsm = nsm_dflt
if verb != True and verb != False:
    verb = False

print("\nSi assume:\n")

print("Numero simboli=\t\t",n)
print("Numero estrazioni=\t",m)
print("Tipo simulazione =\t", type_s)
print("Numero simulazioni=\t",nsm)
print("Verbose=\t\t",verb,"\n")

prob = math.factorial(n) / (n ** m) * Stirling2(m - 1, n - 1)

if nsm > 0:
    print("Inizio simulazione\n")
    
    npr = 0
    nwin = 0
    if verb == False:
        nrast = 0
        minut = 0
        t = time.time()
    while npr < nsm:
        if type_s == 0:
            collected = {}
            nestr = 0
            while len(collected) != n:
                ind = random.randint(0, n - 1)
                if ind in collected:
                    collected[ind] += 1
                else:
                    collected[ind] = 1
                nestr += 1
        else:
            deck = np.random.permutation(n)
            last = deck[n - 1]
            temp = -1
            nestr = 0          
            while temp != last:
                ind = random.randint(0,len(deck) - 1)
                if ind > 0:
                    temp = deck[0]               
                    for k in list(range(ind)):
                        deck[k] =  deck[k + 1]
                    deck[ind] = temp
                nestr += 1
        if nestr == m:
            nwin += 1     
        npr += 1
        if verb:
            print("np=", npr, "\t\tnw=", nwin)
        else:
            if time.time() - t > 2:
                print("*",end="")
                sys.stdout.flush()
                t = time.time()
                nrast = (nrast + 1) % 30
                if nrast == 0:
                    minut += 2
                    print("\tETA:", minut,"\tminuti\tto go:", nsm - np);          
    print("")
        
    freq = nwin / nsm
        
    print("Numero prove=\t\t", nsm)
    print("Numero successi=\t", nwin)
    print("Frequenza=\t\t", freq)
    print("Probabilita'=\t\t", prob)
    print("scarto=\t\t\t", math.fabs(prob - freq))
else:
    print("No simulazione\n")
