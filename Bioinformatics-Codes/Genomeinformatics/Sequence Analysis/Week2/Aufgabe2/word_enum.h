#ifndef WORD_ENUM_H
#define WORD_ENUM_H
typedef struct Wordenumerator Wordenumerator;

Wordenumerator *word_enumerator_new(unsigned long length);

void word_enumerator_delete(Wordenumerator *we);

void word_enumerator_reset(Wordenumerator *we);

const unsigned char *word_enumerator_next(Wordenumerator *we);
#endif
