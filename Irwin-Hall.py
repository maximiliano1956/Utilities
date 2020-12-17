#!/usr/bin/python3

import math, random, time, sys, getopt
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

def Fnx(n, x, verb):
    index = list(range(math.floor(x) + 1))
    
    sum = 0
    if verb == True:
        print("Fnx(", n, ",", x, "): sum =",sum)
    for i in index:
        sum += int((((-1) ** i) * Comb(n, i) * ((x - i) ** n)))
        if verb == True:
            print("Fnx(", n, ",", x, "): sum =",sum)
    sum /= math.factorial(n)
    if verb == True:
        print("")
    return 1 - sum
    
def Pnx(n, x, verb):
    res = Fnx(n, x, verb) - Fnx(n - 1, x, verb)
    return res

def Ekx(x, toll, verb):
    sum = 0
    if verb == True:
        print("Ekx(", x, ",", toll, "): sum =",sum)
    k = math.floor(x) + 1
    nrast = 0
    t = time.time()
    while k < 10000:
        incr = k * Pnx(k, x, verb)
        sum += incr
        if verb == True:
            print("Ekx(", x, ",", toll, "): sum =",sum,"\n")
        if incr != 0 and math.fabs(incr) <= toll:
            break
        k += 1
        
        if verb == False and time.time() - t > 1:
            print("*",end="")
            sys.stdout.flush()
            t = time.time()
            nrast = (nrast + 1) % 30
            if nrast == 0:
                print("");

    print("")
    return sum
#
# Help usage
#
def Help():
    print("""=================================================
Questa funzione valuta a livello teorico il valor medio di
una variabile casuale discreta n
Questa variabile rappresenta quante variabili casuali
uniformi [0,1] bisogna sommare per superare lim
Viene poi effettuta una simulazione. Trovata la media
viene calcolato lo scarto con il valor medio teorico
=================================================""")
    print("\nArguments: -llimite [--limite=limite]",
          "-pprec [--precisione=prec]")
    print("-ssimulazioni [--simulazioni=simulazioni]",
          "-v [--verbose] -h [--help]")     
#
# Main
#
#
# input di default
#
lim_dflt = 1
prec_dflt = 0.0000001
nsm_dflt = 100000
verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "l:p:s:vh",
                    ["limite=",
                     "precisione=", "simulazioni=",
                     "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)
#
# init coi default
#
lim = lim_dflt
prec = prec_dflt
verb = verb_dflt
nsm = nsm_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-l", "--limite"):
        lim = float(arg)
    elif opt in ("-p", "--precisione"):
        prec = float(arg)
    elif opt in ("-s", "--simulazioni"):
        nsm = int(arg)
#
# Controllo valori
#
if lim < 0:
    lim = lim_dflt
if prec <= 0:
    prec = prec_dflt
if nsm < 0:
    nsm = 0
if verb != True and verb != False:
    verb = False

print("\nSi assume:\n")

print("Limite =\t\t",lim)
print("Precisione =\t\t",prec)
print("Numero simulazioni =\t",nsm)
print("Verbose =\t\t",verb,"\n")

print("\nInizio calcolo valor medio\n")

result = Ekx(lim, prec, verb)

print("\nInizio simulazione\n")

random.seed()

np = 0

if verb == False:
    t = time.time()
    nrast = 0

somma = 0
if verb:
    print("accumulo=", somma)
    
while np < nsm:
    sum = 0
    nsum = 0
    while sum < lim:
        sum += random.random()
        nsum += 1
    somma += nsum
    if verb:
        print("sim nr=", np,"n=", nsum,"accumulo=", somma)
        
    np += 1
    
    if verb == False and time.time() - t > 1:
        print("*",end="")
        sys.stdout.flush()
        t = time.time()
        nrast = (nrast + 1) % 30
        if nrast == 0:
            print("");

print("\nValor medio=\t\t",result)
    
if nsm > 0:        
    freq = somma / np
    
    print("Media da simulazione=\t", freq)
    print("Scarto=\t\t\t",math.fabs(result - freq))
