#!/usr/bin/env python3

import sys,getopt
from _ast import arg
from itertools import combinations, product
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
# Test sulla parita':
# Tutte le colonne e la prima riga devono
# avere la stessa parita'
#
def CheckParity(octd):
    colsom = [0 for i in range(ncol)]
    rigsom = 0
    for el in octd:
        if el <= ncol:
            rigsom += 1
        colsom[(el - 1) % ncol] += 1
    
    par = rigsom % 2
    for i in list(range(ncol)):
        if (colsom[i] % 2) != par:
            return False

    return True
#
# Calcola il numero di blocchi in cui compare
# una ben precisa e-pla
#
# con e = 0 si ottiene il numero totale dei blocchi
# con e = l si ottiene 1 (essendo uno steiner system)
#
def CalcNblo(l,b,n,e):
    if e < 0:
        e = 0
    if e > l:
        e = l
    
    return int (Comb(n - e, l - e) / Comb(b - e, l - e))
#
# Calcola in quanti blocchi e' presente
# la lista subs
#
def Search(subs):
    cnt = 0
    for octd in octads.values():
        for j in subs:
            trovato = False
            for k in octd:
                if j == k:
                    trovato = True
                if j <= k:
                    break
            if trovato == False:
                break    
        if trovato == True:
            cnt += 1
            
    return cnt
#
# Addizione in GF(4,2)
#
def DoAdd(add1,add2):
    return(addition[add1][add2])
#
# Moltiplicazione in GF(4,2)
#
def DoMult(fatt1,fatt2):
    return(multipli[fatt1][fatt2])
#
# dati a,b,c in F4
# calcola a*x*x + b*x + c
# che e' pure un elemento di F4
#
def Func(x, a, b, c):
    f = DoAdd(DoAdd(DoMult(a, DoMult(x, x)), DoMult(b, x)), c)
    return(f)
#
# Genera gli hexacodes
# (stringhe di 6 caratteri presi dai 4 simboli di F4
# a distanza 4) 
#   
def GenHex():   
    disp3 = list(product(range(nrig), range(nrig), range(nrig)))
    for a,b,c in disp3:
        ind = a * 16 + b * 4 + c
        hexacodes[ind] = [0 for i in range(3)]
        hexacodes[ind][0] = Func(1, a, b, c)
        hexacodes[ind][1] = Func(2, a, b, c)
        hexacodes[ind][2] = Func(3, a, b, c)
#
# Controllo se la configurazione soddisfa
# il test sull'hexacode
#        
def CheckHex(octd):
    h = [0 for i in range(ncol)]
    t = [0 for i in range(3)]
    
    for el in octd:
        i = int((el - 1) / ncol)
        j = (el - 1) % ncol
        h[j] = DoAdd(h[j],i)
        
    ind = h[0] * 16 + h[1] * 4 + h[2]
        
    t[0] = hexacodes[ind][0]
    t[1] = hexacodes[ind][1]
    t[2] = hexacodes[ind][2]
    
    return t[0] == h[3] and t[1] == h[4] and t[2] == h[5]
#
# Help usage
#
def Help():
    print("""=================================================
Questa funzione calcola gli octads dello Steiner system (5,8,24)
=================================================""")
    print("-v [--verbose]", "-h [--help]")     
#
# Main
#

#
# input di default
#
Verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "vh",
                    ["verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
Verb = Verb_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        Verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
#
# Controllo valori
#
if Verb != True and Verb != False:
    Verb = False

print("\nSi assume:\n")

print("Verbose =",Verb,"\n")

addition = [[0, 1, 2, 3], [1, 0, 3, 2],
                [2, 3, 0, 1], [3, 2, 1, 0]]

multipli = [[0, 0, 0, 0], [0, 1, 2, 3],
                [0, 2, 3, 1], [0, 3, 1, 2]]

nrig = 4
ncol = 6

np = 24
lb = 8
nt = 5

nblot = [0 for i in range(nt + 1)]
nblo = [0 for i in range(nt + 1)]
#
# Calcolo valori teorici nblo
#
for i in list(range(nt + 1)):
    nblot[i] = CalcNblo(nt, lb, np, i)

octads = {}
hexacodes = {}
#
# Genera gi hexacodes
#
GenHex()

ind1 = 0
ind2 = 0
#
# Calcola i blocchi (octads) dello
# Steiner System (5,8,24)
#
print("\nCalcolo octads\n")

lls = list(combinations([i + 1 for i in range(np)],lb))

count = 0
for octd in lls:
    if CheckParity(octd) and CheckHex(octd):
        octads[count] = octd
        
        print("{val:3d}".format(val=count + 1),end = "    -")
        
        for el in octads[count]:
            print("{val:3d}".format(val=el),end= "")
        
        print("  -")
        
        count += 1
        
nblo[0] = count

print("\nS(", nt, ",", lb, ",", np,
      ") e' composto da ", nblo[0], "octads\n")
#
# Genera gli nblo dagli octads
#
for i in list(range(nt)):
    ll = list(combinations([i + 1 for i in range(np)],i + 1))
    resold = -1
    for j in ll:
        res = Search(j)
        if resold == -1:
            resold = res
        else:
            if res != resold:   # devono essere tutti uguali
                                # tra di loro
                print(i, res, resold, "Errore!")
                break
    
    nblo[i + 1] = res
                
    print("Ogni sottinsieme di { 1,..",np,"} di",i + 1,
          "elementi compare in", res, "\toctads")

#
# Controllo tra valori teorici e valori calcolati dagli octads
#
res = True
for i in list(range(nt + 1)):
    if nblo[i] != nblot[i]:
        print(i, nblo[i], nblot[i],"Errore!")
        res = False

if res == True:
    print("\nTutto ok: i valori trovati coincidono ", end="")
    print("con i valori teorici")
else:
    print("Calcoli errati!")
