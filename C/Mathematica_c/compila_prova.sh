#!/bin/bash
gcc -g prova.c -lWSTP64i4 -lm -lpthread -lrt -lstdc++ -ldl -luuid -o prova
#gcc -g prova.c -l:libWSTP64i4.a -lm -lpthread -lrt -lstdc++ -ldl -luuid -o prova
