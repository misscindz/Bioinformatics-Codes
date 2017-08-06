.PHONY: test clean
# choose one:
#CC=clang
CC=gcc
CFLAGS=-Wall -Werror -g -Wunused
LDFLAGS=-lm

ifdef opt
CFLAGS+= -O3
endif

# if int_conv is used add int_conv.o like this:
# sorting.x: sorting.o sorting_main.o int_conv.o
sorting.x: sorting.o sorting_main.o
	${CC} ${LDFLAGS} $^ -o $@

clean:
	${RM} *.o *.x

test: sorting.x
	./sorting.x 100
	./sorting.x 1000
	./sorting.x 10000
	./sorting.x 100000
	@# ./sorting.x 1000000
	@# der obere Test dauert seeehr lange ;)
