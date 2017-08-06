#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stdlib.h>


typedef void (*TokenHandlerFunc)(const char *, void *);

void tokenizer (FILE *fp, TokenHandlerFunc tokenhandler, void *data);

#endif