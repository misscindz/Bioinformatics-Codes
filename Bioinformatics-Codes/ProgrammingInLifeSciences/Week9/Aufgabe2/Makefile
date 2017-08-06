.PHONY: all clean cleanup test
CXX?=g++
CXXFLAGS=-Wall -Wunused-parameter -Werror -g -O3

all: wortketten.x

%.x: %.o
	${CXX} ${CXXFLAGS} -o $@ $^

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x

test: wortketten.x
	@./wortketten.x 5BuchstabenWoerter.txt Wortpaare.txt | diff result.txt -
