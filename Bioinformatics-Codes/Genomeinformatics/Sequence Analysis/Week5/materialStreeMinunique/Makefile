.PHONY: clean
# set prefix GTDIR for libgenometools
LD:=$(CC)

GT_INCLUDE:=-DWITHOUT_CAIRO -I${GTDIR}/src
GT_CFLAGS:=-Wall -Werror -Wunused-parameter -g -Os -pipe ${GT_INCLUDE}
CFLAGS=-O3
LIBS=$(GTDIR)/lib/libgenometools.a -lpthread -lm

STREE_EXACT_OBJ=gt_suffixtree.o\
                stree-exact.o\
                stree-exact-mn.o

STREE_APPROX_OBJ=gt_suffixtree.o\
                 stree-approx.o\
                 stree-approx-mn.o

STREE_MINUNIQUE_MATCH_OBJ=gt_suffixtree.o\
                          stree-minunique.o\
                          stree-minunique-mn.o

STREE_MUM_OBJ=gt_suffixtree.o\
              stree-mum.o\
              stree-mum-mn.o

all:stree-exact-match.x stree-approx-match.x stree-minunique.x stree-mum.x

test-exact-match:stree-exact-match.x
	./download.sh 1
	./check-exact-match.sh Ecoli_K12.fna Ecoli_K12 patternfile

test-approx-match:stree-approx-match.x
	./download.sh 1
	./check-approx-match.sh Ecoli_K12.fna Ecoli_K12 patternfile 1
	./check-approx-match.sh Ecoli_K12.fna Ecoli_K12 patternfile 2

test-minunique:stree-minunique.x
	./download.sh 1
	./index.sh acac acac.fna  && env -i ./stree-minunique.x acac 2 | diff - acac_MU.csv
	./index.sh agcgacgag agcgacgag.fna && env -i ./stree-minunique.x -s agcgacgag 2 | diff - agcgacgag_MU-s.csv
	./index.sh Ecoli_K12 Ecoli_K12.fna
	env -i ./stree-minunique.x Ecoli_K12 2700 | diff - Ecoli_K12_MU_2700.csv

test-mum:stree-mum.x
	./index.sh example-mum ccttcgt_ctgtcgt.fna && env -i ./stree-mum.x -s example-mum 2 | diff - ccttcgt_ctgtcgt_MUM-s.csv
	./download.sh 1 2
	./index.sh Ecoli_K12_O127 Ecoli_K12.fna Ecoli_O127_H6.fna
	env -i ./stree-mum.x Ecoli_K12_O127 400 | diff - Ecoli_K12_O127_MUM.csv

test:test-exact-match test-approx-match test-minunique test-mum

# generic compilation rule which creates dependency file on the fly
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(GT_CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

stree-exact-match.x:${STREE_EXACT_OBJ}
	$(LD) -o $@ $(LDFLAGS) $(GT_LDFLAGS) $(STREE_EXACT_OBJ) $(LIBS)

stree-approx-match.x:${STREE_APPROX_OBJ}
	$(LD) -o $@ $(LDFLAGS) $(GT_LDFLAGS) $(STREE_APPROX_OBJ) $(LIBS)

stree-minunique.x:${STREE_MINUNIQUE_MATCH_OBJ}
	$(LD) -o $@ $(LDFLAGS) $(GT_LDFLAGS) $(STREE_MINUNIQUE_MATCH_OBJ) $(LIBS)

stree-mum.x:${STREE_MUM_OBJ}
	$(LD) -o $@ $(LDFLAGS) $(GT_LDFLAGS) $(STREE_MUM_OBJ) $(LIBS)

genometools:
	current=`pwd`
	cd ${GTDIR} && rm -f LocalMakefile* && CC=${CC} scripts/skmake.rb && cd ${current}

src_check:
	${GTDIR}/scripts/src_check *.c

patternfile:
	mkvtree -db Ecoli_K12.fna -dna -tis -v -ois -indexname Ecoli_K12
	vsubseqselect -minlength 10 -maxlength 12 -snum 100 Ecoli_K12 |\
                grep -v '^#' | sed -e 's/^/>X/' | tr 'X' '\n' > $@
	cleanpp.sh

# read dependencies
-include $(wildcard *.d)

clean:
	rm -f *.[odx]
	rm -f *.des *.esq *.lcp *.llv *.md5 *.prj *.sds *.ssp *.suf
	rm -f *.tis *.ois *.sti1 *.skp *.bwt *.bck *.al1
