#!/usr/bin/env python3

import math
import sys
import random
from random import randint
#
# effettua la moltiplicazione polinomiale poly_a x poly_b = poly_c
#
def poly_prod(poly_a, dim_a, poly_b, dim_b, poly_c):
#
# Per copie argomenti
#    
    poly_a1 = []
    poly_b1 = []
#
# le prossime copie servono per permettere che
# possa essere poly_a = poly_c o poly_b = poly_c o entrambi
#    
# Copia poly_a in poly_a1
#
    for i in list(range(dim_a + 1)):
        poly_a1.append(poly_a[i])
#
# Copia poly_b in poly_b1
#        
    for i in list(range(dim_b + 1)):
        poly_b1.append(poly_b[i])      
#
# Azzera poly_c
#   
    for i in list(range(dim_a + dim_b + 1)):
        poly_c[i] = 0
#
# effettua la moltiplicazione
#    
    for i in list(range(dim_a + 1)):
        for j in list(range(dim_b + 1)):
            poly_c[i + j] += poly_a1[i] * poly_b1[j]

#
# Calcola ((-1) ** m) * m! * Laguerre(m,alfa)
#
def mLaguerre(m,alfa,plag):
    for k in list(range(m + 1)):
        plag[k] = Comb(m + alfa, m - k) * \
            ((-1) ** (m + k)) * Disp(m, m - k)
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
# Stampa un polinomio
#       
def poly_print(poly):
    for i in list(range(len(poly))):    
        print("X **",i,"=",poly[i])
        
    print("\n")   
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
        deck[i] = 1 + (i % cps)
#
# Mescola il mazzo deck con ns trasposizioni
#
def ShuffleDeck():
#
# Fisher-Yates shuffle
#
    i = len(deck) - 1
    while i > 0:
        pos = randint(0,i)
        temp = deck[pos]
        deck[pos] = deck[i]
        deck[i] = temp
        i -= 1
#
# Fa un solitario col mazzo deck
#
# Output:
#
#        0 = non riuscito
#        1 = riuscito
#        
def DoSolitaire():   
    ShuffleDeck()      # mescola mazzo
    
    i = 0
    
    for card in deck:          # scorre il mazzo
        if card == (i + 1):
            return 0        # non riuscito
        i = (i + 1) % nranks
            
    return 1                # riuscito
#
# Simulazione solitario
# n numero solitari da simulare
# p probabilita' di riuscita
#
def DoSim(n,p):
    win = 0

    print("Ora si esegue una simulazione del solitario")
    print("Questa simulazione prevede ",nsm,"smazzate\n")

    InitDeck()

    for np in list(range(n)):
        if DoSolitaire() == 1:
            win += 1
        if np % 1000 == 0:
            print("Numero solitari ancora da eseguire =", \
                  n - np)
     
    print("\nNumero solitari riusciti = ",win)
#
# Ricava frequenza di successo
#
    frwin = win / n

    print("Frequenza vittoria con ", n, "prove =", frwin)
#
# Ricava scarto rispetto a vaolre teorico
#
    print("Scarto rispetto il valore teorico =", \
          math.fabs(frwin - p))
    print("\n")
#
# Help usage
#
def Help():
    print("\n\nParametri di input: suits cps nranks nsm\n")    
#
# Main
#
#
# Carica defaults
#
suits = 4
cps = 10
nranks = 3
nsm = 10000
    
argc = len(sys.argv) - 1        # Nr. parametri in input

if argc == 1:                   # Carica un parametro
    suits = int(sys.argv[1])
elif argc == 2:                 # Carica due  parametri
    suits = int(sys.argv[1])
    cps = int(sys.argv[2])
elif argc == 3:                 # Carica tre parametri
    suits = int(sys.argv[1])
    cps = int(sys.argv[2])    
    nranks = int(sys.argv[3])
elif argc == 4:                 # Carica quattro parametri
    suits = int(sys.argv[1])
    cps = int(sys.argv[2])    
    nranks = int(sys.argv[3])
    nsm = int(sys.argv[4])
elif argc != 0:
    Help()
    sys.exit()
#
# Controllo parametri in input
#
if suits < 1:
    suits = 1
    
if cps < 1:
    cps = 1
    
if nranks < 1:
    nranks = 1 
    
if nranks > cps:
    nranks = cps           # Limita nranks a cps
    
if nsm < 0:
    nsm = 0

Help()
 
print("\nSi assume:\n")

print("Numero semi (suits) =",suits)
print("Numero carte per seme (cps) =",cps)
print("Numero conteggi (nranks) =",nranks)
print("Numero simulazioni (nsm) =",nsm,"\n")
#
# Mazzo per simulazione
#
deck = list(range(suits * cps))
#
# Ricava il numero di carte in ciascun rank (pari a nranks)
#
rank = [0 for i in range(nranks)]

for i in list(range(suits * cps)):
    rank[i % nranks] += 1
    
print("ranks =",rank,"\n")
#
# Ricava i polinomi tipo laguerre generalizzato
# per ciascun rank
#
Lmatrix = [[0] * (suits + 1) for y in range(nranks)]

for i in list(range(nranks)):
    mLaguerre(suits,rank[i] - suits,Lmatrix[i])

for i in list(range(nranks)):
    print("Polinomio ",i + 1,"\n")
    poly_print(Lmatrix[i])    
#
# Calcola il prodotto dei vari polinomi
#
poly_finale = [0 for i in range(nranks * suits + 1)]

for i in list(range(suits + 1)):
    poly_finale[i] = Lmatrix[0][i]
    
dim = suits

for i in list(range(1,nranks)):
    poly_prod(poly_finale, dim, Lmatrix[i], suits, poly_finale)
    dim += suits

print("Polynomio finale\n")

poly_print(poly_finale)
#
# Ricava il numero delle configurazioni vincenti
#
result = 0
for k in list(range(suits * nranks + 1)):
    result += poly_finale[k] * \
        math.factorial(k + (cps - nranks) * suits)
    
print("Numero configurazioni vincenti = ", result)
#
# Calcola la probabilita' di vincita
#
prob = result / math.factorial(cps * suits)
print("Probabilita' che il solitario riesca = ", prob,"\n")

#
# Risultato esatto trovato in rete
# 40 carte 4 semi conteggio 1 2 3
#
#print (1608107296510 * math.factorial(4) ** 3 * math.factorial(28))
#print((2/3) ** 12)      # limite se n grande

#
# Simulazione solitario
#
if nsm > 0:
    DoSim(nsm,prob)
else:
    print("Nessuna simulazione richiesta\n")
