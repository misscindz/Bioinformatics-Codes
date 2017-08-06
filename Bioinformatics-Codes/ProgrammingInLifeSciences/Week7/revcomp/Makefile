.PHONY: all clean
CC=gcc
CFLAGS=-Wall -Werror -Wunused-parameter -ggdb

all: revcomp.x

%.x: %.o
	${CC} -o $@ $^

clean:
	${RM} -f *.x *.seq.rc short.seq

short.seq: ecoli.seq
	head -c 15 $< > $@
	echo '' >> $@
	head -c 33 $< >> $@
	echo '' >> $@
	head -c 22 $< >> $@
	echo '' >> $@

%.seq.rc: %.seq revcomp.x
	valgrind ./revcomp.x $<

test: short.seq.rc ecoli.seq.rc
	diff ecoli.seq.rc expected_output
	diff short.seq.rc expected_short_output
