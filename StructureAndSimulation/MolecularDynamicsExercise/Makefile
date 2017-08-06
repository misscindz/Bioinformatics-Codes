CC = gcc
CFLAGS = -std=c99 -Wall -Wno-unused-function -O3

md: md.c md-extra.c rvec.h md.h
	$(CC) $(CFLAGS) md.c md-extra.c -o md -lm

clean:
	rm -f md
