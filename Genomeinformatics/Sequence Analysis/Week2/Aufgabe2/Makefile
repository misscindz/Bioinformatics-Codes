.PHONY:clean

CC?=clang
CFLAGS=-Wall -Werror -O3 -g
OBJ=outsenseedist-mn.o readinput.o outsenseedist.o outsenseedist-trim.o \
    word_enum.o unitedist.o testbed.o

outsenseedist.x:${OBJ}
	${CC} ${OBJ} -o $@

test:mutated.plain outsenseedist.x
	@outsenseedist.x -w 9
	@outsenseedist.x -r mutated.plain

trim-match:outsenseedist.x ecoli2.plain ecoli2-match.csv
	outsenseedist.x -r ecoli2.plain -s ecoli2-match.csv -m 43 -d 17 -t | diff - trim-match.result

trim-nomatch:outsenseedist.x ecoli1.plain ecoli1-nomatch.csv
	outsenseedist.x -r ecoli1.plain -s ecoli1-nomatch.csv -m 55 -d 17 -t | diff - trim-nomatch.result

trim-lecture:
	outsenseedist.x -g FREIZEIT ZEITGEIST -m 67 -h 3 -d 2 -t

test-trim:trim-lecture trim-match trim-nomatch

clean:
	${RM} *.[ox] ecoli.plain 
	${RM} -r outsenseedist.x.dSYM/
