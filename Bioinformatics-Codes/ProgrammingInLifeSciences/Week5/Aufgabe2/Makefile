.PHONY: clean cleanup test
CFLAGS=-g -Wall -Werror -O3

all: intset.x
OBJ=intset.o intset_main.o

intset.x:${OBJ}
	${CC} ${CFLAGS} -o $@ ${OBJ}

test: intset.x
	./intset_run.sh

intset.o intset_main.o:intset.h

clean:
	rm -f *.o *.x
	rm -rf *.x.dSYM/
