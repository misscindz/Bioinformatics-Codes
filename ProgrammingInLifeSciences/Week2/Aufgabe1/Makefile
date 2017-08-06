.PHONY: all test clean
CFLAGS=-Wall -Werror -g

opt?=no
ifeq (${opt}, yes)
  CFLAGS+= -O3
endif

all: hanoi.x

hanoi.x: hanoi.c
	${CC} ${CFLAGS} -o $@ $^

clean:
	${RM} -rf *.x *.out

valgrind_test: hanoi.x
	valgrind -q ./hanoi.x 3

test_3: hanoi.x
	./hanoi.x 3 > hanoi3.out
	./hanoi_test.rb hanoi3.out 3

test_10: hanoi.x
	./hanoi.x 10 > hanoi10.out
	./hanoi_test.rb hanoi10.out 10

test: valgrind_test test_3 test_10

# vim:ft=make
