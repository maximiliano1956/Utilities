#!/bin/env python3
from algx import solve
from itertools import combinations

# The set to be partitioned is all subsets of {1,2,...,9}
# of size 4, except for {1,2,3,4}.
# Subsets are encoded as tuples.
X = list(combinations(range(1,9+1), 4))[1:]

# The blocks are indexed by subsets of size 5, 
# except for supersets of the removed subset {1,2,3,4}
Y = list([s for s in combinations(range(1,9+1), 5) 
          if s[:4] != (1,2,3,4)])

# We reformat Y to be a dictionary, where
# Y[y] is the list of subsets of y of size 4
Y = {y:[y[:k]+y[k+1:] for k in range(5)] for y in Y}

print('\n\nAlmost S(4,5,9):',end='\n\n')

try:
  print( next(solve(X,Y)), end='\n\n')
except:
  print('No solutions found.', end='\n\n')

# This part tries to find S(k-1,k,n), with n and k as below.
# You can change these values to find other Steiner systems.
# The program will run for a long time if n is too big, 
# but it runs faster if the Steiner system exists.

n = 12
k = 6

def alph_letters(n):
  return [chr(i+97) for i in range(n)]

X = list(combinations(alph_letters(n), k-1))
Y = list(combinations(alph_letters(n), k))
Y = {y:[y[:i]+y[i+1:] for i in range(k)] for y in Y}

print('S('+str(k-1)+','+str(k)+','+str(n)+'): ',end='\n\n')

for g in solve(X, Y, []):

    if g != None:
        counter = 0
        for w in g:
            print(''.join(w) ,end = '  ')
            counter += 1
            if counter == 5:
                print()
                counter = 0

    print()
