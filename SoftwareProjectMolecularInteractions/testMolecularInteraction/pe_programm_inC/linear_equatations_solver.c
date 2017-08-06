#include "matrix.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#include "read_and_print.h"
#include "calculate.h"
#include <string.h>
#include <sys/time.h>
#include <math.h>

#define ABS(X) (X < 0 ? -X : X)
#define MAX(X,Y) (X > Y ? X : Y)
#define MIN(X,Y) (X < Y ? X : Y)
#define RHO 0.6404

Matrix_t *matrix_ptr_to_adr_half(const Matrix_half *bp, unsigned long zeile, unsigned long spalte)
{
	if( spalte > zeile)
	{
		return (bp->bp + (spalte*(spalte+1))/2 + zeile);
	}
	else
	{
		return (bp->bp + (zeile*(zeile+1))/2 + spalte);  
	}
}

Matrix_t *matrix_pointer_to_adress(const Matrix *bp, unsigned long zeile, unsigned long spalte)
{
		return (bp->bp + zeile*bp->spalten + spalte);  
}


void LU_decomposition(Matrix *matrix_A, unsigned long *vector_p)
{
	Matrix_t *A; 
	unsigned long i_max, k, i, j, n, *p_bp, *p1, *p2, save_index;
	float_type max, save, *hp;
	unsigned int threads;
	
	error(matrix_A->zeilen != matrix_A->spalten, "Matrix nicht quadratisch");
	
	n = matrix_A->zeilen;
	p_bp = vector_p;
	
	threads = options.number_of_threads;
		
	for(k = 0; k < n; ++k)
	{
		if(((n-k) < 5*options.number_of_threads) && (((n-k) % 10) == 0)) 
		{
			threads--;
			omp_set_num_threads(threads);
			//printf("Threads  %u   k   %lu\n", options.number_of_threads, k);
		}
		#pragma omp parallel private(i, j, A, hp, save)
		{	
		#pragma omp single
		{
		/*Pivoting*/
		max = 0;
		i_max = k;
		A = matrix_pointer_to_adress(matrix_A, k, k);
		for(i = k; i < n; ++i)
		{
			if (ABS(*A) > max)
			{
				max = ABS(*A);
				i_max = i;
			}			
			A += n;			
		}
		
		/*swap row k and row i_max*/
		p1 = p_bp + k;
		p2 = p_bp + i_max;
		save_index = *p1;
		*p1 = *p2;
		*p2 = save_index;
	}

		#pragma omp barrier
		#pragma omp for 
		for( i = 0; i < n; ++i)
		{
			hp = matrix_pointer_to_adress(matrix_A, i_max, i);
			A = matrix_pointer_to_adress(matrix_A, k, i);
		
			save = *A;
			*A = *hp;
			*hp = save;
		}
		
		#pragma omp for
		for(i = k + 1; i < n; ++i)
		{
			A = matrix_pointer_to_adress(matrix_A, i, k);
			*A /= *matrix_pointer_to_adress(matrix_A, k, k);
		}
	
		#pragma omp for collapse(2)
		for(i = k+1; i < n; ++i)
		{
			for(j = k+1; j < n; ++j)
			{
				*matrix_pointer_to_adress(matrix_A, i, j) -= (*matrix_pointer_to_adress(matrix_A, k, j)) * (*matrix_pointer_to_adress(matrix_A, i, k));	
			}		
		}
	}
	}/*end parallel*/
}

Vector *forward_substitution(Matrix *matrix_LU, const Vector *vector_b)
{
	Vector *vector_y;
	Vector_t *b, *y;
	Matrix_t *LU;
	unsigned long k, j, n;
	float_type sum;
	
	n = vector_b->length;
	vector_y = vector_new(n);
	
	b = vector_pointer_to_adress(vector_b, 0);
	
	for(k = 0; k < n; ++k)
	{
		y = vector_pointer_to_adress(vector_y, 0);
		LU = matrix_pointer_to_adress(matrix_LU, k, 0);
		
		sum = 0;
		for(j = 0; j < k; ++j)
		{
			sum += (*LU) * (*y);
			++y;
			++LU;			
		}	
			
		*y = *b - sum;
		++b;		
	}
	
	return vector_y;
}

Vector *forward_substitution_LDLT(Matrix_half *matrix_LU, const Vector *vector_b, char *elemination_type)
{
	Vector *vector_y;
	Vector_t *b, *y;
	Matrix_t *LU;
	int k, j, n;
	float_type sum;
	
	n = vector_b->length;
	vector_y = vector_new(n);
	
	b = vector_pointer_to_adress(vector_b, 0);
	
	for(k = 0; k < n; ++k)
	{
		y = vector_pointer_to_adress(vector_y, 0);
		LU = matrix_ptr_to_adr_half(matrix_LU, k, 0);
		
		sum = 0;
		for(j = 0; j < k - 1; ++j)
		{
			sum += (*LU) * (*y);
			++y;
			++LU;			
		}				
		
		if(*elemination_type != 0 && k != 0)
		{
			sum += (*LU) * (*y); 
		}
		++elemination_type;
		*vector_pointer_to_adress(vector_y, k) = *b - sum;
		++b;		
	}
	
	return vector_y;	
}

Vector *back_substitution_LDLT(Matrix_half *matrix_LU, const Vector *vector_y, char *elemination_type)
{
	
	Vector *vector_x;
	Vector_t *y;
	int k, j, n;
	float_type sum;
	
	n = vector_y->length;
	vector_x = vector_new(n);
	elemination_type += n -1;
	y = vector_pointer_to_adress(vector_y, n - 1);
	
	for(k = n - 1; k >= 0; --k)
	{			
		sum = 0;
		//#pragma omp parallel for reduction(+:sum)
		for(j =  n-1; j > k + 1; --j)
		{
			sum += (*matrix_ptr_to_adr_half(matrix_LU, j, k)) * (*vector_pointer_to_adress(vector_x, j));
		}				
		
		if(*elemination_type != 2 && k != n -1)
		{
			sum += (*matrix_ptr_to_adr_half(matrix_LU, k +1, k)) * (*vector_pointer_to_adress(vector_x, k+1)); 
		}
		--elemination_type;
		*vector_pointer_to_adress(vector_x, k) = *y - sum;
		--y;		
	}
	
	return vector_x;
}

Vector *D_substitution(Matrix_half *matrix_LU, const Vector *vector_y, char *elemination_type)
{
	Vector *vector_z;
	Vector_t *y, *z;
	unsigned long k, n;
	float_type a, b, c, d, det;
	
	n = vector_y->length;
	vector_z = vector_new(n);
	z = vector_z->bp;
	y = vector_y->bp;
	for( k = 0; k < n; ++k) /* Achtung bei Parallelisierung, k nicht immer ++ */
	{
		if(*elemination_type == 2)
		{
			a = *(matrix_ptr_to_adr_half(matrix_LU, k, k));
			b = *(matrix_ptr_to_adr_half(matrix_LU, k, k+1));
			c = *(matrix_ptr_to_adr_half(matrix_LU, k+1, k));
			d = *(matrix_ptr_to_adr_half(matrix_LU, k+1, k+1)); 
			det = a*d-b*c;
			*z = (d * (*y) - b * *(y+1)) / det;
			++z;
			*z = (a * *(y+1) - c * *y) / det;
			++z; y += 2;
			elemination_type += 2; ++k;					
		}
		else
		{
			*z = *y / *(matrix_ptr_to_adr_half(matrix_LU, k, k));
			++z; ++y; ++elemination_type;
		}
		
	}
		
	return vector_z;
}

Vector *backward_substitution(Matrix *matrix_LU, const Vector *vector_y) 
{
	Vector *vector_x;
	Vector_t *x, *y;
	Matrix_t *LU;
	unsigned long k, j, n;
	float_type sum;
	n = vector_y->length;
	vector_x = vector_new(n);
	
	x = vector_x->bp + n - 1;
	y = vector_y->bp + n - 1;
	
	for(k = n; k > 0; --k)
	{		
		LU = matrix_pointer_to_adress(matrix_LU, k -1, n-1);
		x = vector_pointer_to_adress(vector_x, n-1);
		sum = 0;
		for(j = n; j > k; --j)
		{
			sum += (*LU) * (*x);
			--LU;
			--x;
		}
		
		x = vector_pointer_to_adress(vector_x, k - 1);
		*x = (*y - sum) / *LU;
		--y;		
	}
	
	return vector_x;
}

void permutate_back(Vector *vector_x, unsigned long *vector_p)
{
	Vector_t *c, *x, *c_bp;
	unsigned long n;
	
	n = vector_x->length;
	c_bp = calloc(n, sizeof(Vector_t));
	
	for(x = vector_x->bp; x < vector_x->bp + n; ++x)
	{
		c = c_bp + *vector_p;
		*c = *x;
		++vector_p;
	}
	
	free(vector_x->bp);
	vector_x->bp = c_bp;
}


void permutate(Vector *vector_b, unsigned long *vector_p)
{
	Vector_t *c, *b, *c_bp;
	unsigned long n;
	
	n = vector_b->length;
	c_bp = calloc(n, sizeof(Vector_t));
	
	for(c = c_bp; c < c_bp + n; ++c)
	{
		b = vector_b->bp + *vector_p;
		*c = *b;
		++vector_p;
	}
	
	/*float_type *p1;
	for ( p1 = c_bp; p1 < c_bp + n; ++p1)
		{
			printf("%lf  ", (*p1));
		}*/
	
	free(vector_b->bp);
	vector_b->bp = c_bp;
}

Vector *solve_with_LU(Matrix *matrix_A, Vector *vector_b)
{
	Vector *vector_x, *vector_y;
	unsigned long *p, i;
	
	error(matrix_A->zeilen != vector_b->length, "incompatible matrix and vector");
	
	p = malloc(vector_b->length * sizeof(unsigned long));
	

	for(i = 0; i < vector_b->length; ++i)
	{ 
		*p	= i;
		++p;
	}
	p -= vector_b->length;	
	message("calculating LU_decomposition");
	LU_decomposition(matrix_A, p);
	
	permutate(vector_b, p);
	message("solving Ly=b...");
	vector_y = forward_substitution(matrix_A, vector_b);
	message("solving Ux=y...");
	vector_x = backward_substitution(matrix_A, vector_y);
	
	permutate_back(vector_b, p);
	
		
	vector_delete(vector_y);
	free(p);
	return vector_x;	
}

Vector *solve_with_GS_half(const Matrix_half *matrix_A, Vector *vector_b, Vector *x_0)
{
	Vector_t *x, *b, t_sum, sum, save, residuum;
	unsigned long n, iter;
	unsigned int i, j;
	
	/*Kompatiblität prüfen*/
	
	n = vector_b->length;
	
	x = x_0->bp;	
	b = vector_b->bp;
	
	sum = 0;
	residuum = 100;
	iter = 0;
	
	message("calculating x...");
	while(residuum > options.maxresiduum && iter < options.max_iter)
	{
		residuum = 0;
		#pragma omp parallel private(i, j, t_sum) shared(matrix_A, x, sum, n)
		{			
			for(i = 0; i < n; ++i)
			{
				t_sum = 0;
				
				#pragma omp for nowait
				for(j = 0; j < n; ++j)
				{
					if(i != j)
					{
						t_sum += *matrix_ptr_to_adr_half(matrix_A, i, j) * x[j];
					}
				}
				
				#pragma omp critical
				{
					sum += t_sum;
				}
				
				#pragma omp barrier
				
				#pragma omp single
				{
					save = x[i];
					x[i] = (b[i] - sum) / *matrix_ptr_to_adr_half(matrix_A, i, i);
				
					residuum = MAX(ABS(x[i] - save), residuum);
					sum = 0; 
				}
				
			}
		}

		error(residuum > 100, "no convergence");	
		++iter; 
	}

	printf("Iterationen: %ld\n", iter);
	
	return x_0;
}


Vector *solve_with_GS(const Matrix *matrix_A, Vector *vector_b, Vector *x_0)
{
	Vector_t *x, *b, t_sum, sum, residuum;
	unsigned long n, iter;
	unsigned int i, j;
	
	/*Kompatiblität prüfen*/
	
	n = vector_b->length;
	
	x = x_0->bp;	
	b = vector_b->bp;
	
	sum = 0;
	residuum = 100;
	iter = 0;
	
	message("calculating x...");
	while(residuum > options.maxresiduum && iter < options.max_iter)
	{
		residuum = 0;
		#pragma omp parallel private(i, j, t_sum) shared(matrix_A, x, sum, n)
		{			
			for(i = 0; i < n; ++i)
			{
				t_sum = 0;
				
				#pragma omp for nowait
				for(j = 0; j < n; ++j)
				{
					if(i != j)
					{
						t_sum += *matrix_pointer_to_adress(matrix_A, i, j) * x[j];
					}
				}
				
				#pragma omp critical
				{
					sum += t_sum;
				}
				
				#pragma omp barrier
				
				#pragma omp single
				{
					x[i] = (b[i] - sum) / *matrix_pointer_to_adress(matrix_A, i, i);
				
					//residuum = MAX(ABS(x[i] - save), residuum);
					sum = 0; 
				}
				
			}
		}
		
		residuum = vector_euclidic_norm(matrix_mult_vector(matrix_A, x_0));
		error(residuum > 100, "no convergence");	
		++iter; 
	}
	
	printf("residuum in euclidic norm: %.8lf\n", residuum);

	printf("Iterationen: %ld\n", iter);
	
	return x_0;
}
	
Vector *solve_with_JC(const Matrix *matrix_A, Vector *vector_b, Vector *x_0)
{
	Vector *vector_x_old, *vector_x_new;
	Vector_t *x_old, *x_new, *b, sum, diagonal, *save, residuum;
	Matrix_t *A;
	unsigned long n, iter;
	
	/*Kompatiblität prüfen*/
	
	n = vector_b->length;
	
	vector_x_old = x_0;
	
	vector_x_new = vector_new(n);
	
	x_old = vector_x_old->bp;
	x_new = vector_x_new->bp;
	
	A = matrix_A->bp;
	b = vector_b->bp;
	
	residuum = 100;
	iter = 0;
	
	message("calculating x...");
	while(residuum > options.maxresiduum && iter < options.max_iter)
	{
		/*
		A = matrix_A->bp;
		b = vector_b->bp;*/
		
		#pragma omp parallel private(x_new, x_old, sum, diagonal, A, b)
		{
			#pragma omp for
			for(x_new = vector_x_new->bp; x_new < vector_x_new->bp + n; ++x_new)
			{
				A = matrix_pointer_to_adress(matrix_A, x_new - vector_x_new->bp, 0);
				b = vector_pointer_to_adress(vector_b, (unsigned long)(x_new - vector_x_new->bp)); 
			
				sum = 0;
				diagonal = 0;
				for(x_old = vector_x_old->bp; x_old < vector_x_old->bp + n; ++x_old)
				{
					if( (x_old - vector_x_old->bp) != (x_new - vector_x_new->bp))
					{
						sum += (*A) * (*x_old);
					}
					else
					{
						diagonal = *A;
					}
					++A;
				}
				*x_new = (*b - sum) / diagonal;
				++b;		
			}
		}/*end parallel*/		
		
		/*change vectors*/
		save = vector_x_old->bp;
		vector_x_old->bp = vector_x_new->bp;
		vector_x_new->bp = save;
		
		residuum = vector_euclidic_norm(matrix_mult_vector(matrix_A, vector_x_old));

		error(residuum > 100, "no convergence");
			
		++iter; 
	}
	printf("residuum in euclidic norm: %.8lf\n", residuum);
	printf("Iterationen: %ld\n", iter);
	vector_delete(vector_x_new);
	
	return vector_x_old;
}

void swap_permute_vector(unsigned long *vector_p, int swap1, int swap2)
{
	unsigned long save, *ptr1, *ptr2;
	
	ptr1 = vector_p+swap1;
	ptr2 = vector_p+swap2;
	save = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = save;
}

void symmetric_swap(Matrix *matrix_A, int swap1, int swap2)
{
	float_type save, *ptr1, *ptr2;
	unsigned long i;
	
	/*swap columns */
	for(i = 0; i < matrix_A->zeilen; ++i)
	{
		ptr1 = matrix_pointer_to_adress(matrix_A, i, swap1);
		ptr2 = matrix_pointer_to_adress(matrix_A, i, swap2);
		
		save = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = save;
	}
	
	/* sap rows */
	for(i = 0; i < matrix_A->spalten; ++i)
	{
		ptr1 = matrix_pointer_to_adress(matrix_A, swap1, i);
		ptr2 = matrix_pointer_to_adress(matrix_A, swap2, i);
		
		save = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = save;		
	}
}

void symmetric_swap_half(Matrix_half *matrix_A, int swap1, int swap2)
{
	float_type *bp1, *bp2, save;
	int i, n;
	n = matrix_A->zeilen;
	bp1 = malloc(n * sizeof(float_type));
	bp2 = malloc(n * sizeof(float_type));
	
	//#pragma omp parallel private(i)
	{
		//#pragma omp for	
		for(i = 0; i < n; ++i)
		{
			*(bp1+i) = *matrix_ptr_to_adr_half(matrix_A, swap1, i);
			*(bp2+i) = *matrix_ptr_to_adr_half(matrix_A, swap2, i);
		}
		
		//#pragma omp for	
		for(i = 0; i < swap1; ++i)
		{
			*matrix_ptr_to_adr_half(matrix_A, swap1, i) = *(bp2+i);
		}

		//#pragma omp for
		for(i = 0; i < swap2; ++i)
		{
			if (i != swap1)
			{
				*matrix_ptr_to_adr_half(matrix_A, swap2, i) = *(bp1+i);
			}
		}
	
		//#pragma omp for
		for(i = swap1 + 1; i < n; ++i)
		{
			if(i != swap2)
			{
				*matrix_ptr_to_adr_half(matrix_A, i, swap1) = *(bp2+i);
			}
		}
	
		//#pragma omp for
		for(i = swap2 + 1; i < n; ++i)
		{
			*matrix_ptr_to_adr_half(matrix_A, i, swap2) = *(bp1+i);
		}
	} /* end parallel */
	
	save = *matrix_ptr_to_adr_half(matrix_A, swap1, swap1);
	*matrix_ptr_to_adr_half(matrix_A, swap1, swap1) = *matrix_ptr_to_adr_half(matrix_A, swap2, swap2);
	*matrix_ptr_to_adr_half(matrix_A, swap2, swap2) = save;
	
	free(bp1);
	free(bp2);
}

void double_elemination1(Matrix_half *matrix_A, int k, int n, Vector *save1, Vector *save2)
{
	
	int i, j, rows, start1, start2, end1, end2, rows_per_thread, id, begin;
	Matrix_t *A;
	float_type *s1, *s2, a, b, c, d, a_, b_, c_, d_, det;
	
	printf("blubb");
	s1 = save1->bp;
	s2 = save2->bp;
			
	*s1 = *(matrix_ptr_to_adr_half(matrix_A, k, k));
	*s2 = *(matrix_ptr_to_adr_half(matrix_A, k+1, k));
	++s1; ++s2;
			
	for(i = k+1; i < n; ++i)
	{
		*s1 = *(matrix_ptr_to_adr_half(matrix_A, i, k));
		*s2 = *(matrix_ptr_to_adr_half(matrix_A, i, k+1));
		++s1; ++s2;
	} 
	
	
	a = *(matrix_ptr_to_adr_half(matrix_A, k, k));
	b = c = *(matrix_ptr_to_adr_half(matrix_A, k+1, k));
	d = *(matrix_ptr_to_adr_half(matrix_A, k+1, k+1));
		
	det = a*d - b*c;

	//#pragma omp parallel for private(a_, b_, c_, d_, i) shared(det, k, a, b, c, d)
	/*for(i = 0; i < (n - 1) - (k + 2); i+= 2)
	{
		a_ = *(matrix_ptr_to_adr_half(matrix_A, k+2+i, k));
		b_ = *(matrix_ptr_to_adr_half(matrix_A, k+2+i, k+1));
		c_ = *(matrix_ptr_to_adr_half(matrix_A, k+3+i, k));
		d_ = *(matrix_ptr_to_adr_half(matrix_A, k+3+i, k+1));		

		*(matrix_ptr_to_adr_half(matrix_A, k+2+i, k)) = (a_*d-b_*c) / det;
		*(matrix_ptr_to_adr_half(matrix_A, k+2+i, k+1)) = (a*b_ - a_*b) / det; 
		*(matrix_ptr_to_adr_half(matrix_A, k+3+i, k)) = (c_*d - c*d_) / det;
		*(matrix_ptr_to_adr_half(matrix_A, k+3+i, k+1)) = (a*d_ - c_*b) / det;	
	}*/
	
	for(i = k + 2; i < n - 1; i += 2)
	{
		a_ = *(matrix_ptr_to_adr_half(matrix_A, i, k));
		b_ = *(matrix_ptr_to_adr_half(matrix_A, i, k+1));
		c_ = *(matrix_ptr_to_adr_half(matrix_A, i+1, k));
		d_ = *(matrix_ptr_to_adr_half(matrix_A, i+1, k+1));		

		*(matrix_ptr_to_adr_half(matrix_A, i, k)) = (a_*d-b_*c) / det;
		*(matrix_ptr_to_adr_half(matrix_A, i, k+1)) = (a*b_ - a_*b) / det; 
		*(matrix_ptr_to_adr_half(matrix_A, i+1, k)) = (c_*d - c*d_) / det;
		*(matrix_ptr_to_adr_half(matrix_A, i+1, k+1)) = (a*d_ - c_*b) / det;	
	}

	if( (n-k) % 2 == 1) /*eine Zeile am Ende über*/
	{
		a_ = *(matrix_ptr_to_adr_half(matrix_A, n-1, k));
		b_ = *(matrix_ptr_to_adr_half(matrix_A, n-1, k+1));		

		*(matrix_ptr_to_adr_half(matrix_A, i, k)) = (a_*d - b_*c) / det;
		*(matrix_ptr_to_adr_half(matrix_A, i, k+1)) = (a*b_ - a_*b) / det;
	}	

	/* erste zwei spalten vorbereitet*/
		
	
	#pragma omp parallel private(A, a, b, s1, s2, i, j, rows, begin, rows_per_thread, start1, start2, end1, end2, id) shared(n, k)
	{	
		rows = n -(k+2);
		begin = rows % (options.number_of_threads * 2);
		rows_per_thread = (rows - begin) / (options.number_of_threads); 

		id = omp_get_thread_num();		

		start1 = k + 2 + begin + (rows_per_thread/2) * id;
		end1 = k + 2 + begin + (rows_per_thread/2) * (id+1);
		
		start2 = n - (rows_per_thread/2) * (id + 1);
		end2 = n - (rows_per_thread/2) * id;

		#pragma omp for nowait
		for(i = k + 2; i < k + 2 + begin; ++i)
		{
			a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
			b = *(matrix_ptr_to_adr_half(matrix_A, i, k + 1));	
	
			s1 = save1->bp+2;
			s2 = save2->bp+2;
			A = matrix_ptr_to_adr_half(matrix_A, i, k+2);
			for( j = k + 2; j < i + 1; ++j)
			{	
				*A -= a* (*s1) + b * (*s2);
				++s1; ++s2; ++A;		
			}	
		}

		for(i = start1; i < end1; ++i)
		{
			a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
			b = *(matrix_ptr_to_adr_half(matrix_A, i, k + 1));	
	
			s1 = save1->bp+2;
			s2 = save2->bp+2;
			A = matrix_ptr_to_adr_half(matrix_A, i, k+2);
			for( j = k + 2; j < i + 1; ++j)
			{	
				*A -= a* (*s1) + b * (*s2);
				++s1; ++s2; ++A;		
			}	
		}
		
		for(i = start2; i < end2; ++i)
		{
			a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
			b = *(matrix_ptr_to_adr_half(matrix_A, i, k + 1));	
	
			s1 = save1->bp+2;
			s2 = save2->bp+2;
			A = matrix_ptr_to_adr_half(matrix_A, i, k+2);
			for( j = k + 2; j < i + 1; ++j)
			{	
				*A -= a* (*s1) + b * (*s2);
				++s1; ++s2; ++A;		
			}	
		}
	}
	/*for(i = k + 2; i < n; i++)
	{
		a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
		b = *(matrix_ptr_to_adr_half(matrix_A, i, k + 1));	
	
		s1 = save1->bp+2;
		s2 = save2->bp+2;
		A = matrix_ptr_to_adr_half(matrix_A, i, k+2);
		for( j = k + 2; j < i + 1; ++j)
		{	
			*A -= a* (*s1) + b * (*s2);
			++s1; ++s2; ++A;		
		}	
	}*/
	
}

int elem_to_line(int elem)
{
	return ceil(sqrt(2*(elem + 1) + 0.25) - 0.5) - 1;
}

int gauss(int n)
{
	if (n <= 0) return 0; 
	else return n*(n +1)/2;
}

void double_elemination2(Matrix_half *matrix_A, int k, int n, Vector *save1, Vector *save2)
{
	
	int i, j;
	int elements, elements_per_threads, rest, id, startelem, startrow, endelem, endrow, startindex, endindex;
	float_type *s1, *s2, a, b, c, d, a_, b_, c_, d_, det;
	
	s1 = save1->bp;
	s2 = save2->bp;
			
	for(i = k+1; i < n; ++i)
	{
		s1[i] = *(matrix_ptr_to_adr_half(matrix_A, i, k));
		s2[i] = *(matrix_ptr_to_adr_half(matrix_A, i, k+1));
	} 	
	
	a = *(matrix_ptr_to_adr_half(matrix_A, k, k));
	b = c = *(matrix_ptr_to_adr_half(matrix_A, k+1, k));
	d = *(matrix_ptr_to_adr_half(matrix_A, k+1, k+1));
		
	det = a*d - b*c;
	
	for(i = k + 2; i < n - 1; i += 2)
	{
		a_ = *(matrix_ptr_to_adr_half(matrix_A, i, k));
		b_ = *(matrix_ptr_to_adr_half(matrix_A, i, k+1));
		c_ = *(matrix_ptr_to_adr_half(matrix_A, i+1, k));
		d_ = *(matrix_ptr_to_adr_half(matrix_A, i+1, k+1));		

		*(matrix_ptr_to_adr_half(matrix_A, i, k)) = (a_*d-b_*c) / det;
		*(matrix_ptr_to_adr_half(matrix_A, i, k+1)) = (a*b_ - a_*b) / det; 
		*(matrix_ptr_to_adr_half(matrix_A, i+1, k)) = (c_*d - c*d_) / det;
		*(matrix_ptr_to_adr_half(matrix_A, i+1, k+1)) = (a*d_ - c_*b) / det;	
	}

	if( (n-k) % 2 == 1) /*eine Zeile am Ende über*/
	{
		a_ = *(matrix_ptr_to_adr_half(matrix_A, n-1, k));
		b_ = *(matrix_ptr_to_adr_half(matrix_A, n-1, k+1));		

		*(matrix_ptr_to_adr_half(matrix_A, i, k)) = (a_*d - b_*c) / det;
		*(matrix_ptr_to_adr_half(matrix_A, i, k+1)) = (a*b_ - a_*b) / det;
	}	

	/* erste zwei spalten vorbereitet*/	
	
	elements = gauss(n-k-2);
	
	elements_per_threads = elements/options.number_of_threads;
	
	/*
	if(elements_per_threads < 50 && options.number_of_threads > 1)
	{
		options.number_of_threads--;
		elements_per_threads = elements/options.number_of_threads;
		omp_set_num_threads(options.number_of_threads);
	}*/
	
	rest = elements % options.number_of_threads;
	
	#pragma omp parallel private(id, startindex, endindex, startrow, endrow, startelem, endelem, i, j)
	{
		id = omp_get_thread_num();
	
		startindex = elements_per_threads * id;
		if(id < rest) startindex += id;
		else startindex += rest;
	
		endindex = startindex + elements_per_threads;
	
		if(id < rest) endindex++;	
		
		startrow = elem_to_line(startindex);
		startelem = startindex - gauss(startrow);
		
		endrow = elem_to_line(endindex);
		endelem = endindex - gauss(endrow);
		
		
		if(startrow != endrow)
		{
			for(j = startelem + k + 2; j < startrow + k + 3; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, startrow + k + 2, j) -= *(matrix_ptr_to_adr_half(matrix_A, startrow + k + 2, k))* *(save1->bp+j) + *(matrix_ptr_to_adr_half(matrix_A, startrow + k + 2, k + 1)) * *(save2->bp+j);			
			}
			
			for(i = startrow + k + 3; i < endrow + k + 2; ++i)
			{			
				for(j = k + 2; j < i + 1; ++j)
				{
					*matrix_ptr_to_adr_half(matrix_A, i, j) -= *(matrix_ptr_to_adr_half(matrix_A, i, k))* *(save1->bp+j) + *(matrix_ptr_to_adr_half(matrix_A, i, k + 1)) * *(save2->bp+j);		
				}
			}
		
			for(j = k + 2; j < k + 2 + endelem; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, endrow + k + 2, j) -= *(matrix_ptr_to_adr_half(matrix_A, endrow + k + 2, k))* *(save1->bp+j) + *(matrix_ptr_to_adr_half(matrix_A, endrow + k + 2, k + 1)) * *(save2->bp+j);
			}
		}
		else 
		{
			for(j = startelem + k + 2; j < k + 2 + endelem; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, endrow + k + 2, j) -= *(matrix_ptr_to_adr_half(matrix_A, endrow + k + 2, k))* *(save1->bp+j) + *(matrix_ptr_to_adr_half(matrix_A, endrow + k + 2, k + 1)) * *(save2->bp+j);
			}			
		}
	}
}


void single_elemination2(Matrix_half *matrix_A, int k, int n, Vector *save1)
{
	int i, j;
	float_type *s1, a;
		
	int elements, elements_per_threads, rest, id, startelem, startrow, endelem, endrow, startindex, endindex;
	
	s1 = save1->bp;
		
	//#pragma omp for 
	for(i = k; i < n; ++i)
	{
		s1[i] = *(matrix_ptr_to_adr_half(matrix_A, i, k));
	} 
	
	
	a = *(matrix_ptr_to_adr_half(matrix_A, k, k));
	//#pragma omp for
	for(i = k + 1; i < n; ++i)
	{
		*(matrix_ptr_to_adr_half(matrix_A, i, k)) /= a;
	}

	
	elements = gauss(n-k-1);
	
	elements_per_threads = elements/options.number_of_threads;
	
	/*
	if(elements_per_threads < 50 && options.number_of_threads > 1)
	{
		options.number_of_threads--;
		elements_per_threads = elements/options.number_of_threads;
		omp_set_num_threads(options.number_of_threads);
	}*/
	
	rest = elements % options.number_of_threads;
	#pragma omp parallel private(id, startindex, endindex, startrow, endrow, startelem, endelem, i, j)
	{
		id = omp_get_thread_num();
	
		startindex = elements_per_threads * id;
		if(id < rest) startindex += id;
		else startindex += rest;
	
		endindex = startindex + elements_per_threads;
	
		if(id < rest) endindex++;	
		
		startrow = elem_to_line(startindex);
		startelem = startindex - gauss(startrow);
		
		endrow = elem_to_line(endindex);
		endelem = endindex - gauss(endrow);
		
		if(startrow != endrow)
		{
			for(j = startelem + k + 1; j < startrow + k + 2; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, startrow + k + 1, j) -= *(matrix_ptr_to_adr_half(matrix_A, startrow + k + 1, k)) * s1[j];			
			}
			
			for(i = startrow + k + 2; i < endrow + k + 1; ++i)
			{			
				for(j = k + 1; j < i + 1; ++j)
				{
					*matrix_ptr_to_adr_half(matrix_A, i, j) -= *(matrix_ptr_to_adr_half(matrix_A, i, k)) * s1[j];			
				}
			}
		
			for(j = k + 1; j < k + 1 + endelem; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, endrow + k + 1, j) -= *(matrix_ptr_to_adr_half(matrix_A, endrow + k + 1, k)) * s1[j];			
			}
		}
		else 
		{
			for(j = startelem + k + 1; j < k + 1 + endelem; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, endrow + k + 1, j) -= *(matrix_ptr_to_adr_half(matrix_A, endrow + k + 1, k)) * s1[j];			
			}			
		}
			
		/*
		for(i = k + 1; i < n; ++i)
		{
			for(j = k + 1; j < i + 1; ++j)
			{
				*matrix_ptr_to_adr_half(matrix_A, i, j) -= *(matrix_ptr_to_adr_half(matrix_A, i, k)) * s1[j];			
			}
		}
		*/
	}
}
void single_elemination1(Matrix_half *matrix_A, int k, int n, Vector *save1)
{
	int i, j, rows, start1, start2, end1, end2, rows_per_thread, id, begin;
	Matrix_t *A;
	float_type *s1, a;
	
	
	#pragma omp parallel private(A, a, s1, i, j, rows, begin, rows_per_thread, start1, start2, end1, end2, id) shared(n, k)
	{
		s1 = save1->bp;
		
		#pragma omp for 
		for(i = k; i < n; ++i)
		{
			*(s1+i) = *(matrix_ptr_to_adr_half(matrix_A, i, k));
		} 
	
	
		a = *(matrix_ptr_to_adr_half(matrix_A, k, k));
		#pragma omp for
		for(i = k + 1; i < n; ++i)
		{
			*(matrix_ptr_to_adr_half(matrix_A, i, k)) /= a;
		}
	
		rows = n-(k+1);
		begin = rows % (options.number_of_threads * 2);
		rows_per_thread = (rows - begin) / (options.number_of_threads); 
		
		id = omp_get_thread_num();		

		start1 = k + 1 + begin + (rows_per_thread/2) * id;
		end1 = k + 1 + begin + (rows_per_thread/2) * (id+1);

		
		start2 = n - (rows_per_thread/2) * (id + 1);
		end2 = n - (rows_per_thread/2) * id;
		
		//printf("k  %d  Thread %d  end:  %d    start1   %d   end1    %d    start2   %d    end2    %d\n", k, id, k + begin, start1, end1, start2, end2);
		
		#pragma omp for nowait
		for(i = k + 1; i < k + begin + 1; ++i)
		{
			a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
			s1 = save1->bp+k+1;
			A = matrix_ptr_to_adr_half(matrix_A, i, k+1);
			for(j = k + 1; j < i + 1; ++j)
			{
				*A -= a * (*s1);
				++A; ++s1;					
			}
		}
		
		for(i = start1; i < end1; ++i)
		{
			a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
			s1 = save1->bp+k+1;
			A = matrix_ptr_to_adr_half(matrix_A, i, k+1);
			for(j = k + 1; j < i + 1; ++j)
			{
				*A -= a * (*s1);
				++A; ++s1;					
			}
		}
		
		for(i = start2; i < end2; ++i)
		{
			a = *(matrix_ptr_to_adr_half(matrix_A, i, k));
			s1 = save1->bp+k+1;
			A = matrix_ptr_to_adr_half(matrix_A, i, k+1);
			for(j = k + 1; j < i + 1; ++j)
			{
				*A -= a * (*s1);
				++A; ++s1;					
			}
		}
	} /*end parallel */
}



void LDLT_decomposition(Matrix_half *matrix_A, unsigned long *vector_p, char *diagonal)
{
	bool double_elemination;
	float_type lambda, lambda_t, sigma, sigma_t;//, time;
	Vector *save1, *save2;
	int n, k, i, r, r_t;
	char *elemination_type;
	
	n = matrix_A->zeilen;
	save1 = vector_new(n);
	save2 = vector_new(n);
			
	elemination_type = diagonal;
	double_elemination = false;
		
	int threads = options.number_of_threads;
	
	for(k = 0; k < n; ++k)
	{
		/*pivoting see Mathematics of Computation, Vol. 31, No. 137 (Jan., 1977), pp 163-179 algorithm A*/
	
		/* (1) */
		lambda = 0;
		r = k;
		//#pragma omp parallel private (lambda_t, r_t, i) shared (lambda, r)
		{
			lambda_t = 0;
			r_t = k;
			//#pragma omp for
			for(i = k + 1; i < n; ++i)
			{
				if (ABS(*matrix_ptr_to_adr_half(matrix_A, i, k)) > lambda_t)
				{
					lambda_t = ABS(*matrix_ptr_to_adr_half(matrix_A, i, k));
					r_t = i;
				}
			}
			//#pragma omp critical
			{
				if(lambda_t > lambda)
				{
					lambda = lambda_t;
					r = r_t; 
				}
				else if(lambda_t == lambda) r = MIN(r_t, r);
			}
		} /* end parallel */
		
		if(lambda == 0) 
		{
			*elemination_type = 1;
			++elemination_type;
			continue;
		}
		
		/* (2) */
		if (ABS(*matrix_ptr_to_adr_half(matrix_A, k, k)) >= RHO * lambda)
		{
			double_elemination = false;
		}
		else /* (3) */
		{
			sigma = 0;
			//#pragma omp parallel private(i, sigma_t) shared(r, k, sigma)
			{
				sigma_t = 0;
				//#pragma omp for 
				for( i = k; i < n; ++i)
				{
					if(i != r)
					{
						sigma_t = MAX(sigma_t, ABS(*matrix_ptr_to_adr_half(matrix_A, i, r)));
					}
				}
				//#pragma omp critical
				{
					sigma = MAX(sigma, sigma_t);
				}
			} 
			/* (4) */
			if( RHO * lambda * lambda <= sigma * ABS(*matrix_ptr_to_adr_half(matrix_A, k, k)))
			{
				double_elemination = false;
			}
			else /* (5) */
			{
				if(ABS(*matrix_ptr_to_adr_half(matrix_A, r, r)) >= RHO * sigma)
				{
					symmetric_swap_half(matrix_A, k, r); //swap first and rth rows and columns
					swap_permute_vector(vector_p, k, r);
					double_elemination = false;
				}
				else /* (6) */
				{
					symmetric_swap_half(matrix_A, k + 1, r);
					swap_permute_vector(vector_p, k + 1, r); //swap second and rth rows and columns so that  |A(2,1)| = lambda
					double_elemination = true;
				}
			}
		}		
		
		
		/* 1x1 Pivot*/
		if(double_elemination == false)
		{
			single_elemination2(matrix_A, k, n, save1);
							
			*elemination_type = 1;
			++elemination_type;	
		}
		else /* 2x2 Pivot */
		{
			double_elemination2(matrix_A, k, n, save1, save2);
			
			++k;
			*elemination_type = 2;
			++elemination_type;
			*elemination_type = 0;
			++elemination_type;
		}	
	}
	options.number_of_threads = threads;
	vector_delete(save1);
	vector_delete(save2);
}


Vector *solve_with_LDLT(Matrix_half *matrix_A, Vector *vector_b)
{
	Vector *z, *x;
	unsigned long *vector_p, *p, i;
	char *diagonal;
		
	vector_p = malloc(matrix_A->zeilen * sizeof(unsigned long));
	diagonal = malloc(matrix_A->zeilen*sizeof(char));
	p = vector_p;
	
	if(strcmp(options.load_matrix_file, "NOFILE") == 0)
	{
		for(i = 0; i < matrix_A->zeilen; ++i)
		{
			*p = i;
			++p;
		}
		message("calculating LDLT_decomposition...");
		LDLT_decomposition(matrix_A, vector_p, diagonal);
	}
	else
	{
		message("loading matrix");
		load_matrix(matrix_A, p, diagonal, options.load_matrix_file);
	}

	permutate(vector_b, vector_p);
	
	message("solving Lz=b...");
	x = forward_substitution_LDLT(matrix_A, vector_b, diagonal);
	message("solving Dy=z...");
	z = D_substitution(matrix_A, x, diagonal);
	vector_delete(x);
	message("solving LTx=y...");
	x = back_substitution_LDLT(matrix_A, z, diagonal);
	vector_delete(z);
	permutate_back(x, vector_p);

	
	if(strcmp(options.save_matrix_file, "NOFILE") != 0)
	{
		message("saving matrix");
		save_matrix(matrix_A, vector_p, diagonal, options.save_matrix_file);
	}
	free(vector_p);
	free(diagonal);
	
	return x;	
}
