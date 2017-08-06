CC=clang
CFLAGS= -Wall -Werror -g -O3
all: maxmin.x

%.x: %.c
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o *.x