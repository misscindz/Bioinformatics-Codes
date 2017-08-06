CC=clang
CFLAGS= -Wall -Werror -g -O3
all: aligns.x

%.x: %.c
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o *.x