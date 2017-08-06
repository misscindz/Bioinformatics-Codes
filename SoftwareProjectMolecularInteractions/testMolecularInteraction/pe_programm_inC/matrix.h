#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <stdio.h>
#include "def.h"
#pragma once

typedef float_type Matrix_t;

typedef struct {float_type *bp;
					unsigned long zeilen, spalten;
					} Matrix;

typedef struct {float_type *bp;
					unsigned long zeilen, spalten;
					} Matrix_half;
					
Matrix *matrix_new(unsigned long rows, unsigned long columns);

Matrix_half *matrix_new_half(unsigned long rows, unsigned long columns);

void matrix_delete(Matrix *matrix_s);

void matrix_delete_half(Matrix_half *matrix_s);

void matrix_print(const Matrix *matrix_s, FILE *stream);

void matrix_print_half(Matrix_half *matrix_A, FILE *stream);

void matrix_print_Octave(const Matrix *matrix_s, FILE *stream);

Matrix *matrix_mult(const Matrix *a_matrix_s,
                              const Matrix *b_matrix_s);

void matrix_fill_random(Matrix *matrix_s, long min, long max);



//Matrix_t *matrix_ptr_to_adr_half(const Matrix *bp, unsigned long zeile, unsigned long spalte);

Vector *matrix_mult_vector(const Matrix *matrix_A, const Vector *vector_b);

Matrix *matrix_copy(const Matrix *matrix_A);

void matrix_fill_diagonal_dominant(Matrix *matrix_A);

#endif
