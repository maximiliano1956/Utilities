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
    for hexd in hexads.values():
        for j in subs:
            trovato = False
            for k in hexd:
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
# Addizione in GF(3,2)
#
def DoAdd(add1,add2):
    return(addition[add1][add2])
#
# Moltiplicazione in GF(3,2)
#
def DoMult(fatt1,fatt2):
    return(multipli[fatt1][fatt2])
#
#
#
def AddT(fatt1,fatt2):
    somma = [[0 for i in range(ncol)] for j in range(nrig)]
    for i in list(range(nrig)):
        for j in list(range(ncol)):
            somma[i][j] = addition[fatt1[i][j]][fatt2[i][j]]
    return(somma)
#
#
#
def SubT(fatt1,fatt2):
    diff = [[0 for i in range(ncol)] for j in range(nrig)]
    for i in list(range(nrig)):
        for j in list(range(ncol)):
            diff[i][j] = subtraction[fatt1[i][j]][fatt2[i][j]]
    return(diff)
#
# dati a,b in F3
# calcola a*x + b
# che e' pure un elemento di F3
#
def Func(x, a, b):
    f = DoAdd(DoMult(a, x), b)
    return(f)
#
# Genera i tetracodes
# (stringhe di 4 caratteri presi dai 3 simboli di F3
# a distanza 3) 
#   
def GenTetra():   
    disp2 = list(product(range(nrig), range(nrig)))
    ind = 0
    for a,b in disp2:
        tetracodes[ind] = [0 for i in range(4)]
        tetracodes[ind][0] = a
        tetracodes[ind][1] = b
        tetracodes[ind][2] = Func(1, a, b)
        tetracodes[ind][3] = Func(2, a, b)
        ind += 1
#
#
#
def GetHexad(m):
    hexad = [0 for i in range(6)]
    ind = 0
    for i in range(nrig):
        for j in range(ncol):
            if m[i][j] != 0:
                hexad[ind] = conv[i][j]
                ind += 1
                
    swap = True
    while swap == True:
        swap = False
        for i in list(range(5)):
            if hexad[i] > hexad[i + 1]:
                ll = hexad[i]
                hexad[i] = hexad[i + 1]
                hexad[i + 1] = ll
                swap = True                           
                
    return(hexad)
#
#
#
def InsHexad(hexd):
    for hexad in hexads.values():
        if hexd == hexad:
            return
    hexads[nblo[0]] = hexd
    nblo[0] += 1   
#
# Help usage
#
def Help():
    print("""=================================================
Questa funzione calcola gli hexads dello Steiner system (5,6,12)
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

addition = [[0, 1, 2], [1, 2, 0], [2, 0, 1]]

subtraction = [[0, 2, 1], [1, 0, 2], [2, 1, 0]]

multipli = [[0, 0, 0], [0, 1, 2], [0, 2, 1]]

tet = [[[1, 1, 1, 1], [0, 0, 0, 0], [0, 0, 0, 0]],
       [[1, 0, 0, 0], [0, 1, 1, 1], [0, 0, 0, 0]],
       [[1, 0, 0, 0], [0, 0, 0, 0], [0, 1, 1, 1]],
       [[0, 1, 0, 0], [1, 0, 1, 0], [0, 0, 0, 1]],
       [[0, 0, 0, 1], [1, 1, 0, 0], [0, 0, 1, 0]],
       [[0, 0, 1, 0], [1, 0, 0, 1], [0, 1, 0, 0]],
       [[0, 1, 0, 0], [0, 0, 0, 1], [1, 0, 1, 0]],
       [[0, 0, 1, 0], [0, 1, 0, 0], [1, 0, 0, 1]],
       [[0, 0, 0, 1], [0, 0, 1, 0], [1, 1, 0, 0]]
      ]

col = [[[1, 0, 0, 0], [1, 0, 0, 0], [1, 0, 0, 0]],
       [[0, 1, 0, 0], [0, 1, 0, 0], [0, 1, 0, 0]],
       [[0, 0, 1, 0], [0, 0, 1, 0], [0, 0, 1, 0]],
       [[0, 0, 0, 1], [0, 0, 0, 1], [0, 0, 0, 1]]
      ]

conv = [[7, 4, 1, 10], [6, 3, 8, 11], [5, 2, 9, 12]]

nrig = 3
ncol = 4

np = 12
lb = 6
nt = 5

minimog = [[0 for i in range(ncol)] for j in range(nrig)]

nblot = [0 for i in range(nt + 1)]
nblo = [0 for i in range(nt + 1)]
#
# Calcolo valori teorici nblo
#
for i in list(range(nt + 1)):
    nblot[i] = CalcNblo(nt, lb, np, i)

hexads = {}
tetracodes = {}
#
# Genera i tetracodes
#
GenTetra()

ind1 = 0
ind2 = 0
#
# Calcola i blocchi (hexads) dello
# Steiner System (5,6,12)
#
print("\nCalcolo hexads\n")

nblo[0] = 0

print("col-col")
InsHexad(GetHexad(SubT(col[0],col[1])))
InsHexad(GetHexad(SubT(col[0],col[2])))
InsHexad(GetHexad(SubT(col[0],col[3])))
InsHexad(GetHexad(SubT(col[1],col[2])))
InsHexad(GetHexad(SubT(col[1],col[3])))
InsHexad(GetHexad(SubT(col[2],col[3])))

print("tet-tet")
for i in list(range(9)):
    for j in list(range(9)):
        if i != j:
            InsHexad(GetHexad(SubT(tet[i],tet[j])))
            
print("col+tet")
for i in list(range(4)):
    for j in list(range(9)):
        InsHexad(GetHexad(AddT(col[i],tet[j])))
        
print("col+col-tet")
colcol0=AddT(col[0],col[0])
colcol1=AddT(col[0],col[1])
colcol2=AddT(col[0],col[2])
colcol3=AddT(col[0],col[3])
colcol4=AddT(col[1],col[1])
colcol5=AddT(col[1],col[2])
colcol6=AddT(col[1],col[3])
colcol7=AddT(col[2],col[2])
colcol8=AddT(col[2],col[3])
colcol9=AddT(col[3],col[3])

for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol0,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol1,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol2,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol3,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol4,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol5,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol6,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol7,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol8,tet[j])))
for j in list(range(9)):
    InsHexad(GetHexad(SubT(colcol9,tet[j])))

swap = True  
while swap == True:
    swap = False
    for count in list(range(nblo[0] - 1)):
        for i in list(range(6)):
            if hexads[count][i] == hexads[count + 1][i]:
                continue
            if hexads[count][i] > hexads[count + 1][i]:
                ll = hexads[count]
                hexads[count] = hexads[count + 1]
                hexads[count + 1]= ll
                swap = True
            break

count = 0
for hex in hexads.values():    
    print("{val:3d}".format(val=count + 1),end = "    -")    
    for el in hex:
        print("{val:3d}".format(val=el),end= "")
        
    print("  -")
        
    count += 1

print("\nS(", nt, ",", lb, ",", np,
      ") e' composto da ", nblo[0], "hexads\n")
#
# Genera gli nblo dagli hexads
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
          "elementi compare in", res, "\thexads")

#
# Controllo tra valori teorici e valori calcolati dagli hexads
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
