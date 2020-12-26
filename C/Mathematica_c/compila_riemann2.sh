#!/bin/bash
gcc -g riemann2.c -lWSTP64i4 -lm -lpthread -lrt -lstdc++ -ldl -luuid -o riemann2
