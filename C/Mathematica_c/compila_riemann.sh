#!/bin/bash
gcc -g riemann.c -lWSTP64i4 -lm -lpthread -lrt -lstdc++ -ldl -luuid -o riemann
