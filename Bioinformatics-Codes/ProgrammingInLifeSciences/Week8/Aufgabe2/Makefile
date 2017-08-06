.PHONY: clean
CXX?=g++
CPPFLAGS=-O3 -g3 -Wall -Werror -Wunused-parameter -Wpointer-arith
LDFLAGS=-lm

company.x: company-test.o company.o
	${CXX} $+ ${LDFLAGS} -o $@

company.o: company.cpp

company.o company-test.o: company.h

test: company.x
	@./company.x

clean:
	${RM} *.o *.x *~
	${RM} -r company.x.dSYM/
