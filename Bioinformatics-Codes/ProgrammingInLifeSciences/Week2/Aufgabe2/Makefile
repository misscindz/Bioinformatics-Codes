.PHONY: all test clean
CFLAGS=-Wall -Werror -g

opt?=no
ifeq (${opt}, yes)
  CFLAGS+= -O3
endif

all: queuetest.x

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $^

queuetest.x: queue.o queuetest.o
	${CC} ${CFLAGS} -o $@ $^

clean:
	${RM} -rf *.o *.x

test: queuetest.x
	valgrind -q ./queuetest.x 1000 | diff ./queue1000 -

# vim:ft=make
