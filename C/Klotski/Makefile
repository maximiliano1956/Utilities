# Project: UniMake: Universal Makefile
# Just change 'BIN' to the desired output name

BIN     = solveKlotski
CC      = gcc
CPP     = g++
#CFLAGS  = -I. -g
CFLAGS  = -I. -O3
LDFLAGS = 
CPPSRC  = $(wildcard *.cpp)
DEPS    = $(wildcard *.h) Makefile
OBJ     = $(patsubst %.c,%.o,$(CSRC)) $(patsubst %.cpp,%.o,$(CPPSRC))

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CPP) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BIN) $(BIN).o
