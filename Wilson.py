#!/usr/bin/env python3

import sys
sys.path.append("/home/lelli/eclipse-workspace/Lucas-Lehmer/src")

import ll

def is_power2(num):
    return num != 0 and ((num & (num - 1)) == 0)

while True:

    print("\n")

    ns = input("Imposta un numero ")

    print("\n")

    try:
        n = int(ns)
    except:
        if ns == "quit":
            print("Bye!")
            sys.exit(0)
        print("Bad input")
        continue

    if n < 2:
        print(n, "non e' primo")
    else:
        fact = 1
        for ind in range(n):
            if ind > 1:
                fact *= ind
                fact %= n
                if (fact == 0):
                    break
        if fact == n - 1:
            print(n, "e' primo")
        else:
            print(n, "non e' primo")

    if is_power2(n + 1):
        print("Test di Lucas-Lehmer: il numero di Mersenne ", n, end=" ")
        if ll.CheckM(n):
            print("e' primo")
        else:
            print("non e' primo")
