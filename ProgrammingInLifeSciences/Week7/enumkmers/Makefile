.PHONY: all clean test
CC=gcc
CFLAGS=-Wall -Werror -Wunused-parameter -ggdb
# uncomment if math.h is in use
#
# LDFLAGS=-lm

all: enumkmers.x

# if int_conv is used, uncomment this line and have int_conv.[ch] in this
# directory
#
# enumkmers.x: int_conv.o

%.x: %.o
	${CC} -o $@ $^ ${LDFLAGS}

clean:
	${RM} *.x *.o

test: enumkmers.x
	./test.sh | diff -b -y test.out - |tee output.txt
