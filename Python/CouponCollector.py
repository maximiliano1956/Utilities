#!/usr/bin/env python3

import math, sys, getopt, time
from random import randint, seed
from _ast import arg
import numpy as np
from mpmath import mp
#
# Simulazione
#        
def DoSim(n):    
    seed()
    somma = 0
    if verb == False:
        t = time.time()
        nrast = 0
        mins = 0
    
    for indx in list(range(nsm)):
        if type_s == 0:
            collection = [0 for i in range(n)]
            collected = 0
            npr = 0
            while collected < n:
                ind = randint(0, n - 1)
                if collection[ind] == 0:
                    collection[ind] = 1
                    collected += 1
                else:
                    collection[ind] += 1
                npr += 1
        else:
            deck = np.random.permutation(n)
            last = deck[n - 1]
            temp = -1
            npr = 0            
            while temp != last:
                ind = randint(0,len(deck) - 1)
                if ind > 0:
                    temp = deck[0]               
                    for k in list(range(ind)):
                        deck[k] =  deck[k + 1]
                    deck[ind] = temp
                npr += 1

        somma += npr
        if verb == False and (time.time() - t) > 2:
            print("*",end="")
            sys.stdout.flush()
            t = time.time()
            nrast = (nrast + 1) % 30
            if nrast == 0:
                mins += 1
                print("\tto go=", nsm - indx, "\ttime elapsed(min.)=", mins);
        
        if verb == True:          
            print("prova nr=", indx, "\t\tvalore=", npr)
            
    expected = somma / nsm
    
    somma = 0
    for i in list(range(n)):
        somma += 1 / (i + 1)
    lim_mean = n * (math.log(n) + mp.euler) + 0.5
    mean = n * somma
    
    scarto1 = math.fabs(expected - mean)
    scarto2 = math.fabs(lim_mean - mean)
    
    print("\n\nexpected=", expected, "\t\tmean=", mean, "\tscarto=", scarto1)
    print("mean=", mean, "\tlimit mean=", lim_mean, "\tscarto=", scarto2)   
#
# Help usage
#
def Help():
    print("""=================================================
Dato n intero vengono generati valori random uniformemente
distribuiti tra 0 e n-1. Il processo continua fino a che
ciascuno degli n valori possibili viene generato almeno
una volta. Il numero dei numeri random generati e' a sua
volta una variabile random.
Ci interessa valutarne il valor medio.
La teoria dice che E(x) = n + n/2 + n/3 + ... + 1
che al crescere di n tende a n*log(n)
Viene effettuata una simulazione e valutato lo
scarto rispetto il valore teorico.
Da notare che questo problema puo' assumere varie forme:
ad es:
1) Abbiamo una permutazione random di n carte
   Ne scegliamo una a caso. La rimettiamo al suo posto
   Iteriamo e ci fermiamo quando tutte le carte sono state
   esaminate almeno una volta. (tipo 1)
   Il valor medio delle scelte e' E(x)
2) Abbiamo un mazzo di carte.
   Iteriamo il "top to random shuffle"
   (Si prende la carta in cima al mazzo e la si inserisce
    random eventualmente rimanendo al primo posto)
   Si dimostra che per avere un mescolamento sufficiente e'
   necessario iterare fino a che tutte le carte sono
   salite in cima al mazzo e inserite random.
   (L'ultima a fare questo sara' l'ultima carta nel mazzo
   di partenza)
   Anche qui il numero medio dei mescolamenti da fare e'
   E(x) (tipo 2)
=================================================""")
    print("\nArguments: -ncarte [--carte=carte] -ttipo [--tipo=tipo]")
    print("-ssimulazioni [--simulazioni=simulazioni]",
          "-v [--verbose] -h [--help]")     
#
# Main
#
#
# input di default
#
n_dflt = 10
nsm_dflt = 10000
type_s_dflt = 0
verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "n:s:t:vh",
                    ["carte=","simulazioni=","tipo="
                     "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
verb = verb_dflt
nsm = nsm_dflt
type_s = type_s_dflt
n = n_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-n", "--carte"):
        n = int(arg)
    elif opt in ("-t", "--tipo"):
        type_s = int(arg)
    elif opt in ("-s", "--simulazioni"):
        nsm = int(arg)
#
# Controllo valori
#
if n < 2:
    n = n_dflt
if nsm < 0:
    nsm = 0
if type_s > 1 or type_s < 0:
    type_s = type_s_dflt
if verb != True and verb != False:
    verb = False

print("\nSi assume:\n")

print("Numero carte =\t\t",n)
print("Numero simulazioni =\t",nsm)
print("Tipo simulazione =\t", type_s)
print("Verbose =\t\t",verb,"\n")

print("\nInizio calcolo valor medio\n")

DoSim(n)

