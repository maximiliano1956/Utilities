#!/usr/bin/env python3

import math

nloop=1000000
#sum=0
#for n in range(1,nloop+1):
#    sum=sum+1/(4*n+1)-1/(4*n-1)

sum1=0    
sum2=0    
sum3=0    
sum4=0    

for n in range(0,nloop+1):
    sum1=sum1+1/(12*n+1)
for n in range(0,nloop+1):
    sum2=sum2+1/(12*n+5)
for n in range(1,nloop+1):
    sum3=sum3+1/(12*n-1)
for n in range(1,nloop+1):
    sum4=sum4+1/(12*n-5)

sum=sum1-sum3+sum2-sum4    

pi=sum*3
print(pi)
print(math.pi)
