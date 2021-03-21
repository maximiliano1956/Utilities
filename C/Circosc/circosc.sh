#!/bin/bash

g++ -g -Wall -D__LINUX_PULSE__ -o circosc circosc.c RtAudio.cpp -lGL -lGLU -lglut -lpthread -lpulse-simple -lpulse
