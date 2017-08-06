#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <math.h>
#include "read_and_print.h"
#include "def.h"


Vector *vector_new(unsigned long length)
{
	Vector *vector;
				
	vector = malloc(sizeof(vector));
	if (vector == NULL)
	{
		printf("Speicherprobleme\n");
		exit(EXIT_FAILURE);		
	}
	vector->length = length;
	vector->bp = calloc(length, sizeof(Vector_t)); /*Don't change to malloc*/
	if (vector->bp == NULL)
	{
		printf("Speicherprobleme\n");
		exit(EXIT_FAILURE);		
	}
	
	return vector;
}

void vector_delete(Vector* vector)
{
	free(vector->bp);
	free(vector);
	
}

void vector_print(const Vector* vector, FILE *stream)
{
	unsigned long i, j;
	Vector_t *ptr;
	
	ptr = vector->bp;
	
	for (i = 0; i < vector->length/3; ++i)
	{
		fprintf(stream, "%-3ld   ", i + 1);
		for(j = 0; j < 3; ++j)
		{
			fprintf(stream, "% .8lf     ", *ptr);
			++ptr;
		}
		fprintf(stream, "\n");
	}	
	fprintf(stream, "\n");
} 

void vector_print_Octave(const Vector* vector, FILE *stream)
{
	unsigned long i, j;
	Vector_t *ptr;
	
	ptr = vector->bp;
	
	fprintf(stream, "Ausgabe des Vektors\n");
	fprintf(stream, "[");
	for (i = 0; i < vector->length/3; ++i)
	{
		for(j = 0; j < 3; ++j)
		{
			fprintf(stream, "% .8lf\n", *ptr);
			++ptr;
		}
	}	
	fprintf(stream, "]'\n");
}


void vector_fill_random(Vector *vector, long min, long max)
{
	if(min >= max)
	{
		printf("Fehler: min größer max");
		exit(EXIT_FAILURE);
	}
	
	Vector_t *ptr;

    for(ptr = vector->bp; ptr < vector->bp + vector->length; ++ptr)
    {
    *ptr = ((rand() % 10000000*(max - min +1))+10000000*min)/10000000.0;
	}
}

/*
Vector_t *vector_pointer_to_adress(const Vector *vector, unsigned long index)
	{
		return (vector->bp + index);		
	}
*/	
Vector *vector_sub_vector(const Vector *vector_a, const Vector *vector_b)
{
	Vector *vector_c;
	Vector_t *a, *b, *c;
	
	if(vector_a->length != vector_b->length)
	{
		printf("Vektoren inkompatibel");
		exit(EXIT_FAILURE);
	}
	vector_c = vector_new(vector_a->length);
	vector_c-> length = vector_a->length;
	
	a = vector_a->bp;
	b = vector_b->bp;
	c = vector_c->bp;
	
	for(c = vector_c->bp; c < vector_c->bp + vector_c->length; ++c)
	{
		*c = *a - *b;
		++a;
		++b;
	}
	
	return vector_c;
}

float_type vector_euclidic_norm(const Vector *vector_a)
{
	float_type norm;
	unsigned int i;
	
	norm = 0;
	for(i = 0; i < vector_a->length; ++i)
	{
		if(isnan(vector_a->bp[i])) break;
		norm += pow(vector_a->bp[i], 2);
	}
	
	return sqrt(norm);
}

float_type vector_mult_vector(const Vector *vector_a, const Vector *vector_b)
{
	float_type *a, *b, sum;
	unsigned int i;
	error(vector_a->length != vector_b->length, "vector incompatible");
	
	a = vector_a->bp;
	b = vector_b->bp;
	
	sum = 0;
	for(i = 0; i < vector_a->length; ++i)
	{
		sum += *a * *b;
		++a; ++b;
	}
	
	return sum;
}

Vector* vector_copy(const Vector *vector)
{
	Vector *vector_b;
	unsigned int i;
	
	vector_b = vector_new(vector->length);
	float_type *a, *b;
	
	a = vector->bp;
	b = vector_b->bp;
	for(i = 0; i < vector->length; ++i)
	{
		*b = *a;
		++a; ++b;
	}
	
	return vector_b;	
}
