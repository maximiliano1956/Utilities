#!/bin/env wolframscript

pow=17
c=9

p1 = x^pow+c
p2 = (x+1)^pow+c

res=Resultant[p1,p2,x]
Print[res]

Print[FactorInteger[res]]

bbb=Reduce[p1==res*y && p2==res*z,{x,y,z},Integers]
Print[bbb]
