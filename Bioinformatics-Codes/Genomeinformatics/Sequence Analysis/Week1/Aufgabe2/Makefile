.PHONY:clean

CC?=clang
CFLAGS=-Wall -Werror -O3 -g
OBJ=chainer.o

chainer.x:${OBJ}
	${CC} ${OBJ} -o $@

test:matches.txt chainer.x
	./chainer.x 5132068 5449314 matches.txt

clean:
	${RM} *.[ox]
