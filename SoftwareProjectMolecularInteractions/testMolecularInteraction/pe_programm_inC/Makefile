.PHONY: all clean

# Common definitions
CC = gcc

# Compiler flags, paths and libraries
CFLAGS = -std=c99 -pedantic -Wall -Wextra -fopenmp -O3 -Wunused-parameter -Wpointer-arith 
LFLAGS = $(CFLAGS)
LIBS   = -lm

OBJ=matrix.o projekt.o vector.o linear_equatations_solver.o read_and_print.o calculate.o

# Rule to create *.o from *.c
.c.o:
	$(CC) -c $(CFLAGS) $*.c

# Targets ...
all: projekt.x

projekt.x : $(OBJ) Makefile
	$(CC) $(LFLAGS) -o $@ $(OBJ) $(LIBS)

matrix.o: matrix.h Makefile

projekt.o: matrix.h Makefile

vector.o : vector.h Makefile

read_and_print.o : read_and_print.h

linear_equatations_solver.o : linear_equatations_solver.h Makefile

calculate.o : calculate.h

clean:
	${RM} *.x *.o *~
