#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include "def.h"
#pragma once

typedef float_type Vector_t;

typedef struct {Vector_t *bp;
					unsigned long length;} Vector;

Vector *vector_new(unsigned long length);

void vector_delete(Vector *vector);

void vector_print(const Vector* vector, FILE *stream);

void vector_print_Octave(const Vector* vector, FILE *stream);
void vector_fill_random(Vector *vector,long min, long max);

#define vector_pointer_to_adress(X,Y) (X->bp + Y)
//Vector_t *vector_pointer_to_adress(const Vector *vector, unsigned long index);

Vector *vector_sub_vector(const Vector *vector_a, const Vector *vector_b);

float_type vector_euclidic_norm(const Vector *vector_a);

float_type vector_mult_vector(const Vector *vector_a, const Vector *vector_b);

Vector* vector_copy(const Vector *vector);

#endif 
