CC?=clang
CFLAGS=-Wall -Werror -O3 -g -Wunused-parameter -Wunused
OBJECTS=swalign.o swalign-mn.o scorematrix.o linebuffer.o sequences_read.o alphabet.o

all: swalign.x

# generic compilation rule which creates dependency file on the fly
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

swalign.x: ${OBJECTS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS}

test-gsa: swalign.x gsa-align.txt gsa-seqpair.txt
	./swalign.x -1 unitscore.txt gsa-seqpair.txt | diff - gsa-align.txt
	./swalign.rb -1 unitscore.txt gsa-seqpair.txt | diff - gsa-align.txt

test-small: swalign.x seq18.txt align18.txt
	./swalign.x -4 blosum62.txt seq18.txt | diff - align18.txt
	./swalign.rb -4 blosum62.txt seq18.txt | diff - align18.txt

test-large: swalign.x seq50.txt align50.txt
	./swalign.x -4 blosum62.txt seq50.txt | diff - align50.txt

test:test-gsa test-small test-large

-include $(wildcard *.d)

clean:
	rm -f *.[ox]
