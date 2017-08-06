CC=clang
CFLAGS= -Wall -Werror -g -O3
all: CelsiusToFahrenheit.x

%.x: %.c
	${CC} ${CFLAGS} -o $@ $<

clean:
	rm -f *.o *.x