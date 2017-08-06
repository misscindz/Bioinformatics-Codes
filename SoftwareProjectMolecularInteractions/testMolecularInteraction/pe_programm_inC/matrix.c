#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"
#include "vector.h"
#include "linear_equatations_solver.h"
#include "read_and_print.h"
#include <math.h>

Matrix_half *matrix_new_half(unsigned long rows, unsigned long columns)
{
	Matrix_half *matrix;
	matrix = malloc(sizeof(Matrix));
	if (matrix == NULL)
	{
		printf("Speicherprobleme\n");
		exit(EXIT_FAILURE);		
	}
	if (rows != columns)
	{
		printf("nicht qu\n");
	}
	matrix->zeilen = rows;
	matrix->spalten = columns;
	matrix->bp = calloc((rows*(rows+1))/2, sizeof(Matrix_t));
	if (matrix->bp == NULL)
	{
		printf("Speicherprobleme\n");
		exit(EXIT_FAILURE);		
	}
		
	return matrix;	
}

Matrix *matrix_new(unsigned long rows, unsigned long columns)
{
	Matrix *matrix;
	matrix = malloc(sizeof(Matrix));
	if (matrix == NULL)
	{
		printf("Speicherprobleme\n");
		exit(EXIT_FAILURE);		
	}
	
	matrix->zeilen = rows;
	matrix->spalten = columns;
	matrix->bp = calloc(rows*columns, sizeof(Matrix_t));
	if (matrix->bp == NULL)
	{
		printf("Speicherprobleme\n");
		exit(EXIT_FAILURE);		
	}
		
	return matrix;	
}

void matrix_delete_half(Matrix_half *matrix_s)
{
	free(matrix_s->bp);
	free(matrix_s);	
}

void matrix_delete(Matrix *matrix_s)
{
	free(matrix_s->bp);
	free(matrix_s);	
}

void matrix_print(const Matrix *matrix_s, FILE *stream)
{
	unsigned long i, j;
	Matrix_t *ptr;
	
	ptr = matrix_s->bp;
	
	fprintf(stream, "Ausgabe der Matrix\n");
	fprintf(stream, "\n");
	for (i = 0; i < matrix_s->zeilen; ++i)
	{
		if( i % 3 == 0 && i != 0) fprintf(stream, "\n");
		for(j = 0; j < matrix_s->spalten; ++j)
		{
			if(j % 3 == 0 && j != 0) fprintf(stream, "  ");
			fprintf(stream, "% .5lf  ", *ptr);
			++ptr;
		}
		fprintf(stream, "\n");
	}	
	fprintf(stream, "\n");
}

void matrix_print_half(Matrix_half *matrix_A, FILE *stream)
{
	float_type *ptr;
	unsigned long i,j;
	
	ptr = matrix_A->bp;
	fprintf(stream, "Ausgabe der symmetrischen Matrix\n");
	for(i = 0; i < matrix_A->zeilen; ++i)
	{
		for(j = 0; j <= i; ++j)
		{
			fprintf(stream, "% .5lf  ", *ptr);
			++ptr;
		}
		fprintf(stream, "\n");
	}
	
}

void matrix_print_Octave(const Matrix *matrix_s, FILE *stream)
{
	unsigned long i, j;
	Matrix_t *ptr;
	
	ptr = matrix_s->bp;
	
	fprintf(stream, "\n [");
	for (i = 0; i < matrix_s->zeilen; ++i)
	{
		for(j = 0; j < matrix_s->spalten; ++j)
		{
			fprintf(stream, "%lf ", *ptr);
			++ptr;
		}
		fprintf(stream, ";\n");
	}	
	fprintf(stream, "]\n");
}
/*
Matrix_t *matrix_pointer_to_adress(const Matrix *bp, unsigned long zeile, unsigned long spalte)
{
		return (bp->bp + zeile*bp->spalten + spalte);  
}*/

/*
Matrix_t *matrix_ptr_to_adr_half(const Matrix *bp, unsigned long zeile, unsigned long spalte)
{
		return (bp->bp + zeile*bp->spalten + spalte);  
}*/

Matrix *matrix_mult(const Matrix *a_matrix_s,
                              const Matrix *b_matrix_s)
{
	unsigned long l, m, n, i, j, s;
	Matrix_t *pa, *pb, *pc, summe;

	
	if (a_matrix_s->spalten != b_matrix_s->zeilen)
	{
		printf("Matrizen inkompatibel");
		exit(EXIT_FAILURE);
	}
	
	l =  a_matrix_s->zeilen;
	m = a_matrix_s->spalten;
	n = b_matrix_s->spalten;
	
	Matrix *c_matrix_s = matrix_new(l, n);
	
	pa = a_matrix_s->bp;
	pb = b_matrix_s->bp;
	pc = c_matrix_s->bp;
	
	for(i = 0; i < l; ++i)
	{
		for(j = 0; j < n; ++j)
		{	
			pa = matrix_pointer_to_adress(a_matrix_s, i, 0);
			pb = matrix_pointer_to_adress(b_matrix_s, 0, j);
			
			summe = 0;
			for(s = 0; s < m; ++s)
			{
				summe += (*pa) * (*pb);
				pa++;
				pb += n;
			}
			*pc = summe;
			pc++;
		}
	}
	
	return c_matrix_s;
}


 
void matrix_fill_random(Matrix *matrix_s, long min, long max)
{
	if(min >= max)
	{
		printf("Fehler: min größer max");
		exit(EXIT_FAILURE);
	}
	
	Matrix_t *ptr;

    for(ptr = matrix_s->bp; ptr < matrix_s->bp + matrix_s->zeilen * matrix_s->spalten; ++ptr)
    {
    *ptr = ((rand() % 10000000*(max - min +1))+10000000*min)/10000000.0;
	}	
} 


/*warnign: bug inside^^*/
Vector *matrix_mult_vector(const Matrix *matrix_A, const Vector *vector_b)
{
	Vector *vector_c;
	Vector_t *b, *c;
	unsigned long i, j, l, m;
	float_type sum;
	
	if(matrix_A->spalten != vector_b->length)
	{
		printf("Inkompatibel!");
		exit(EXIT_FAILURE);		
	}
	l = matrix_A->zeilen;
	m = matrix_A->spalten;
	vector_c = vector_new(l);
	c = vector_c->bp;
	b = vector_b->bp;
	
	#pragma omp parallel for private(sum, j, i)
	for(i = 0; i < l; ++i)
	{
		sum = 0;
		for(j = 0; j < m; ++j)
		{
			sum += *(matrix_A->bp + i*m + j) * b[j];
		}
		c[i] = sum;
	}
		
	return vector_c;	
}

Matrix *matrix_copy(const Matrix *matrix_A)
{
	Matrix *matrix_B;
	float_type *A, *B;
	
	matrix_B = matrix_new(matrix_A->zeilen, matrix_A->spalten);
	matrix_B->zeilen = matrix_A->zeilen;
	matrix_B->spalten = matrix_B->spalten;
	
	A = matrix_A->bp;
	for(B = matrix_B->bp; B < matrix_B->bp + matrix_B->spalten * matrix_B->zeilen; ++B)
	{
		*B = *A;
		++A;
	}
	
	return matrix_B;	
}

void matrix_fill_diagonal_dominant(Matrix *matrix_A)
{
	Matrix_t *A;
	unsigned long j, k, n;
	
	A = matrix_A->bp;
	n = matrix_A->zeilen;
	
	for(j = 0; j < n; ++j)
	{
		A = matrix_pointer_to_adress(matrix_A, j, 0);
		for(k = 0; k < n; ++k)
		{
			if(j == k)
			{
				*A = 10 * n; 
			}
			else
			{
				*A = ((rand() % 20000000 - 10000000)/10000000.0);
			}
			++A;
		}		
	}
	
	
}

