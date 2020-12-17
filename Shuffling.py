#!/usr/bin/env python3

import math
import sys
import getopt
import random
import time
from random import randint
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
def Bell(n):
    if n == 0:
        return 1
    somma = 0
    for k in list(range(n)):
        somma += Comb(n - 1,k) * Bell(k)
    return somma
#
# Inizializza il mazzo deck
#
def InitDeck():
    random.seed()       # Init seme con tempo attuale
#
# Carica il mazzo deck con sequenze crescenti da 1 in poi
# ritornando a 1 al cambio di suit
#
    for i in list(range(len(deck))):
        deck[i] = i + 1
#
# Mescola il mazzo deck con ns trasposizioni
#
def ShuffleDeck():
    i = 0
    
    while i < len(deck):
        ind = randint(0,len(deck) - 1) # indice destinazione
        if ind > 0:
            appo = deck[0]
        
            for k in list(range(ind)):
                deck[k] =  deck[k + 1]
            
            deck[ind] = appo
 
        i += 1
#
# Help
#
def Help():
    print("""=================================================
Questa funzione calcola la probabilita' che lo shuffle
del mazzo mantenga il suo ordinamento iniziale.
Se n e' il numero di carte del mazzo, ogni singolo shuffle
consiste nel ripetere n volte l'operazione di prendere
la prima carta del mazzo ed inserirla a random
nel mazzo stesso (eventualmente anche al primo posto
rimanendo percio' al proprio posto).
La teoria dice che la probabilita' e': Bell(n) / (n ** n)
dove Bell(n) e' il numero di Bell e conta il numero
di possibili partizioni diverse di un insieme di n elementi.
Questa probabilita' e' un po' piu' grande di quella che
su ha nel caso di una scelta a random di una permutazione
delle carte (che e' 1 / n!)
=================================================""")
    print("\nArguments: \t-ncards [--carte=cards]")
    print("\t\t-ssimulazioni",
          "[--simulazioni=simulazioni] -v [--verbose]",
          "-h [--help]")     
#
# Main
#
#
# input di default
#
ncards_dflt = 5
nsm_dflt = 10000
Verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "n:s:vh",
                    ["carte=","simulazioni=",
                     "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
ncards = ncards_dflt
nsm = nsm_dflt
Verb = Verb_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        Verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-n", "--carte"):
        ncards = int(arg)
    elif opt in ("-s", "--simulazioni"):
        nsm = int(arg)
#
# Controllo valori
#
if ncards < 1:
    ncards = ncards_dflt
if Verb != True and Verb != False:
    Verb = False

print("\nSi assume:\n")

print("Numero carte=\t\t",ncards)
print("Numero simulazioni=\t",nsm)
print("Verbose=\t\t",Verb,"\n")

n = ncards
b = Bell(n)
f = 1 / math.factorial(n)
nn = n ** n
prob = b / nn

if Verb:
    print("n=\t\t\t\t", n)
    print("Bell=\t\t\t\t", b)
    print("n**n=\t\t\t\t", nn)
    print("Bell / (n ** n)=\t\t", prob)
    print("1/n!=\t\t\t\t", f)
    print("scarto con permutazione casuale=", prob - f)
    print("")
        
deck = list(range(ncards))

if nsm > 0:
    print("Inizio simulazione\n")
    
    np = 0
    nwin = 0
    if Verb == False:
        nrast = 0
        minut = 0
        t = time.time()
    while np < nsm:
        InitDeck()
        ShuffleDeck()
        nob = 0
        for i in list(range(len(deck))):
            if deck[i] != i + 1:
                nob = 1
                break;
        if nob == 0:
            nwin += 1
        
        if Verb:
            print("np=", np, "\t\tnw=", nwin,"\t\t", nob == 0,"\t", deck)
        else:
            if time.time() - t > 2:
                print("*",end="")
                sys.stdout.flush()
                t = time.time()
                nrast = (nrast + 1) % 30
                if nrast == 0:
                    minut += 2
                    print("\tETA:", minut,"\tminuti\tto go:", nsm - np);  
        np += 1
    
    print("")
        
    freq = nwin / nsm
        
    print("Numero prove=\t\t", nsm)
    print("Numero successi=\t", nwin)
    print("Frequenza=\t\t", freq)
    print("Probabilita'=\t\t", prob)
    print("scarto=\t\t\t", math.fabs(prob - freq))
    print("scarto da random=\t", math.fabs(f - freq))
else:
    print("No simulazione\n")
