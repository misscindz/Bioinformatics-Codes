.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3

all: intset.x
OBJ=intset.o intset_main.o

intset.x:${OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ}

intset.o intset_main.o: intset.h

intset.o: intset_simple.c
	${CC} ${CFLAGS} -c -o $@ $<

test: intset.x
	./intset_run.sh

clean:
	rm -f *.o *.x
	rm -rf intset.x.dSYM/
