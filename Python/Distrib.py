#!/usr/bin/env python3

import math, random, time, sys, getopt
#
# Help usage
#
def Help():
    print("""=================================================
Questa funzione considera una variabile casuale continua
avente distribuzione sin(x) con x compreso tra 0 e pi/2
Si considera un intervallo di x centrato su pi/4
di ampiezza 2*delta.
Calcolata la probabilita' che x sia nell'intervallo,
viene effettuata una simulazione e viene valutata la
frequenza dell'evento e lo scarto rispetto la probabilita'
=================================================""")
    print("\nArguments: -d[[=][ ]]delta [--delta[=][ ]delta]")
    print("-s[[=][ ]]simulazioni", 
          "[--simulazioni[=][ ]simulazioni]")
    print("-v [--verbose] -h [--help]")     
#
# Main
#
#
# input di default
#
delta_dflt = math.pi / 8
nsm_dflt = 100000
verb_dflt = False

try:
    opts, args = getopt.getopt(sys.argv[1:],
                    "d:s:vh",
                    ["delta=",
                     "simulazioni=",
                     "verbose", "help"])
except getopt.GetoptError:
    print("\nSyntax error!\n")
    Help()
    sys.exit(1)   
#
# init coi default
#
delta = delta_dflt
nsm = nsm_dflt
verb = verb_dflt

for opt, arg in opts:
    if opt in ("-v", "--verbose"):
        verb = True
    elif opt in ("-h", "--help"):
        Help()
        sys.exit(0)        
    elif opt in ("-d", "--delta"):
        delta = float(arg)
    elif opt in ("-s", "--simulazioni"):
        nsm = int(arg)
#
# Controllo valori
#
maxdelta = math.pi / 4
if delta > maxdelta or delta < 0:
    delta = delta_dflt
if nsm < 0:
    nsm = 0
if verb != True and verb != False:
    verb = False

print("\nSi assume:\n")

print("Delta =\t\t\t",delta)
print("Numero simulazioni =\t",nsm)
print("Verbose =\t\t",verb,"\n")

x1 = maxdelta - delta
x2 = maxdelta + delta

print("\nDistribuzione di densita' sin(x) con 0 <= x <= pi/2")
print("Intervallo= [", x1, ",", x2, "]\n")

print("Calcolo probabilita':")
prob = math.cos(x1) - math.cos(x2)
print("Probabilita'=", prob)

if nsm > 0:
    print("\nInizio simulazione con", nsm, "prove\n")

    np = 0
    fav = 0

    if verb == False:
        nrast = 0
        t = time.time()

    while np < nsm:
        p = math.acos(random.random())
        if verb == True:
            print("\nProva nr=\t", np, "x=", p, end="\t")
        if p >= x1 and p <= x2:
            fav += 1
            if verb == True:
                print("In range")
        else:
            if verb == True:
                print("")              
        if verb == True:
            print("Casi favorevoli=", fav)
        np += 1
    
        if verb == False and time.time() - t > 1:
            print("*",end="")
            sys.stdout.flush()
            t = time.time()
            nrast = (nrast + 1) % 30
            if nrast == 0:
                print("");
      
    print("")
    
    fr = fav / np

    print("\nFine simulazione:")
    print("Su", np, "prove rilevati", fav, "casi favorevoli")
    print("frequenza evento favorevole=", fr)
    print("\nscarto tra probabilita'=", prob, "e frequenza=", fr)
    print(math.fabs(prob - fr))    
