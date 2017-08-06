.PHONY: all

SRCFILES=linkedlist.c linkedlist_test.c

CFLAGS=-Wall -Werror -g -Wunused-parameter -Wunused-variable

opt?=no

all: linkedlist_test.x

ifeq (${opt}, yes)
  CFLAGS+= -O3
endif

include $(SRCFILES:.c=.d)

%.d: %.c
	@set -e; rm -f $@; \
 	$(CC) -MM $(CFLAGS) $< > $@; \
 	sed -i -e 's,\($*\)\.o[ :]*,\1.o $@ : ,g' -e 's,$*.c ,,' $@

linkedlist_test.x: linkedlist_test.o linkedlist.o
	${CC} ${CFLAGS} -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $<

clean:
	${RM} *.o

cleanup: clean
	${RM} *.x

test: all
	./linkedlist_test.x linkedlist.dat

# vim: ft=make
