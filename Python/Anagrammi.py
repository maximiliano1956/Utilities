#!/usr/bin/env python3

import math, sys, getopt, time
from random import randint, seed
from _ast import arg
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
# effettua la moltiplicazione polinomiale:
# poly_a * poly_b = poly_c
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
# cioe' il polinomio di Laguerre in forma monica
# (dove il coefficiente di x ** m e' uguale a +1)
#
def mLaguerre(m,alfa,plag):
    for k in list(range(m + 1)):
        plag[k] = Comb(m + alfa, m - k) * \
            ((-1) ** (m + k)) * Disp(m, m - k)
#
# Incrementa gli indici dei ranks
#           
def incr(index,ranks):
    i = 0
    while True:
        index[i] = (index[i] + 1) % (ranks[i] + 1)
        if index[i] != 0:
            return 0
        else:
            i += 1
        if i >= len(index):
            return 1
#
# Simulazione
#        
def DoSim(base, nfissi, npr, verb):   
    n = len(base)

    deck1 = []

    for i in list(range(len(base))):
        deck1.append(base[i])

    seed()
    
    if verb == True:
        print("\nInizio simulazione\n")

    vitt = 0
    nprov = 0

    while nprov < npr:
#
# Fisher-Yates shuffle
#
        i = n - 1
        while i > 0:
            pos = randint(0,i)
            temp = deck1[pos]
            deck1[pos] = deck1[i]
            deck1[i] = temp
            i -= 1
#
# Fine Fisher-Yates shuffle
#
        i = 0
        sync = 0
        while i < n:
            if deck1[i] == base[i]:
                sync += 1
            i += 1
        
        if verb == 1:
            print("".join(deck1), sync)      
   
        if sync == nfissi:
            vitt += 1   
            
        nprov += 1
    
    return vitt, nprov
#
# Calcola casi favorevoli
#
def Calcola(angrm, verb):

    n = len(angrm)
    
    ranks = []
    ranks_lett = ""
    
    if n == 0:
        return 1, 1, 1, len(ranks), ranks, ranks_lett

    mat_let = [0 for i in range(200)]

    for i in list(range(n)):
        mat_let[ord(angrm[i])] += 1
    for i in range(len(mat_let)):
        if mat_let[i] > 0:
            ranks.append(mat_let[i])
            ranks_lett += chr(i)
                
    nranks = len(ranks)

    nrk = 0
    for i in ranks:
        nrk += i   
#
# Controllo
#            
    if nrk != n:
        print("Errore software!")
        sys.exit(1)
 
    if verb == True:
        print("\n\nAnagramma di",angrm,"\n")
    
    mat = {}
    
    for i in list(range(nranks)):   
        mat[ranks_lett[i]] = [0 for j in range(ranks[i] + 1)]
        mLaguerre(ranks[i],0,mat[ranks_lett[i]])
        if verb == True:
            print("Polinomio monico di Laguerre ", i + 1, "\n")
            poly_print(mat[ranks_lett[i]])
        
    poly_finale = [0 for i in range(n + 1)]

    for i in list(range(ranks[0] + 1)):
        poly_finale[i] = mat[ranks_lett[0]][i]
    
    dim = ranks[0]

    for i in list(range(nranks - 1)):
        poly_prod(poly_finale, dim, mat[ranks_lett[i + 1]],
            ranks[i + 1], poly_finale)
        dim += ranks[i + 1]

    if verb == 1:
        print("Polinomio monico di Laguerre finale\n")
        poly_print(poly_finale)
    
    result = 0
    for k in list(range(n + 1)):
        result += poly_finale[k] * math.factorial(k)
        
    den = 1
    for i in list(range(nranks)):
        den *= math.factorial(ranks[i])
    
    result /= den
    result = int(result)
    
    poss = int(math.factorial(n) / den)
    prob = result / poss
    
    return result, poss, prob, nranks, ranks, ranks_lett
#
# Anagrammi
#
def Anagrammi(base, nfissi, FaiSim, Verb, nsm):
    n = len(base)
    
    if nfissi < 0:
        nfissi = 0
    if nfissi > n:
        nfissi = n
        
    tt = Calcola(base, Verb)
    
    possibili = tt[1]
    nranks = tt[3]
    ranks = tt[4]
    ranks_lett = tt[5]

    indx = [0 for i in range(nranks)]
    
    if Verb == True:
        t=time.time()

    somma = 0
    primo_loop = 1
    nrast = 0
    while True:
        fine_loop = 0
        while True:
            while True:
                if Verb == True and (time.time() - t) > 1:
                    print("*",end="")
                    sys.stdout.flush()
                    t = time.time()
                    nrast = (nrast + 1) % 30
                    if nrast == 0:
                        print("");  
                if primo_loop == 0 and incr(indx,ranks) == 1:
                    fine_loop = 1
                    break
                else:
                    primo_loop = 0
                
                    s = 0
                    for i in list(range(nranks)):
                        s += indx[i]
                    if s == (n - nfissi):
                        fine_loop = 2
                        break
            if fine_loop > 0:
                break        
        if fine_loop == 1:
            break
    
        prod = 1
        anag = []
    
        for i in list(range(nranks)):
            prod *= Comb(ranks[i],indx[i])
            for j in list(range(indx[i])):
                anag.append(ranks_lett[i])
            
        prob = Calcola("".join(anag), Verb)[0]
        prod *= prob
    
        somma += prod

    prob = somma / possibili

    print("\nRisultati:\n")
    print("Favorevoli =\t\t\t\t", somma)
    print("Possibili =\t\t\t\t", possibili)
    print("Probabilita' =\t\t\t\t", prob)

    if FaiSim == True:
        vittorie, nprove = DoSim(base, nfissi, nsm, Verb)

        result1 = vittorie/nprove

        print("\nNumero anagrammi simulati =\t\t",nprove)
        print("Numero anagrammi con", nfissi,\
                "lettere fisse =\t",vittorie)
        print("Frequenza  anagrammi =\t\t\t", result1)
        print("\nScarto =\t\t\t\t", math.fabs(result1 - prob))   
#
# Help usage
#
def Help():
    print("""=================================================
Questa funzione calcola il numero di possibili
anagrammi di una stringa di m lettere con n
lettere che rimangono al loro posto (n da 0 a m).
La stringa puo' avere lettere ripetute.
Viene calcolata (in base al numero totale
degli anagrammi possibili) la probabilita'
di avere un anagramma di quel tipo.
Viene anche effettuata una simulazione e valutato
lo scarto tra la frequenza e la probabilita'.
=================================================""")
    print("\nArguments: \t-astring [--anagramma=string]",
          "-f#fisse [--fisse=#fisse]")
    print("\t\t-s#simulazioni",
          "[--simulazioni=#simulazioni] -v [--verbose]",
          "-h [--help]")     
#
# Main
#

#
# input di default
#
base_dflt = "MISSISSIPI"
nfissi_dflt = 5
nsm_dflt = 10000
Verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "a:f:s:vh",
                    ["anagramma=","fisse=",
                     "simulazioni=", "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
base = base_dflt
nfissi = nfissi_dflt
nsm = nsm_dflt
Verb = Verb_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        Verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-a", "--anagramma"):
        base = arg
    elif opt in ("-f", "--fisse"):
        nfissi = int(arg)
    elif opt in ("-s", "--simulazioni"):
        nsm = int(arg)
#
# Controllo valori
#
if len(base) == 0:
    Help()
    sys.exit(1)
if nfissi < 0:
    nfissi = 0
elif nfissi > len(base):
    nfissi = len(base)
if nsm <= 0:
    nsm = 0
    FaiSim = False
else:
    FaiSim = True
if Verb != True and Verb != False:
    Verb = False

print("\nSi assume:\n")

print("Stringa =",base)
print("Numero lettere fisse =",nfissi)
print("Numero simulazioni =",nsm)
print("Verbose =",Verb,"\n")

Anagrammi(base, nfissi, FaiSim, Verb, nsm)

sys.exit(0)
