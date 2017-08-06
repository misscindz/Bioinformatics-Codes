.PHONY:clean test cleanup
CFLAGS=-g -O3 -Wall -Werror -Wunused-parameter -Wpointer-arith 
LDFLAGS=-lm -pthread
CC?=clang

SEQFILE=sw175.fna

OBJ=threads_dp.o multiseq.o unit_edist.o bestkvals_generic.o

all:threads_dp.x

# generic compilation rule which creates dependency file on the fly
%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

threads_dp.x:${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS} 

test: test_expected_results test_multithreaded

benchmark:
	@echo "Threads: 1"
	@time (./threads_dp.x 50 1 ${SEQFILE} > /dev/null)
	@echo "=========="
	@echo "Threads: 4"
	@time (./threads_dp.x 50 4 ${SEQFILE} > /dev/null)
	@echo "=========="
	@echo "Threads: 8"
	@time (./threads_dp.x 50 8 ${SEQFILE} > /dev/null)

test_expected_results:	threads_dp.x ${SEQFILE}
	./threads_dp.x 50 1 ${SEQFILE} | diff - expected.results

test_multithreaded:	threads_dp.x ${SEQFILE}
	./threads_dp.x 50 4 ${SEQFILE} | grep -v '^#' > result.4
	./threads_dp.x 50 1 ${SEQFILE} | grep -v '^#' > result.1
	diff result.1 result.4
	rm -f result.1 result.4

# read dependencies
-include $(wildcard *.d)

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x result* *.d

