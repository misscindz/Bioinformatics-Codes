#include "calculate.h"
#include "matrix.h"
#include <stdlib.h>
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include "read_and_print.h"
#include "linear_equatations_solver.h"

bool is_in_exclude_list(unsigned long length_exclude_list, unsigned long *ex_s, unsigned long *ex_t)
{
	unsigned long i;

	for( i = 0; i < length_exclude_list; ++i)
	{
		if(*ex_s == *ex_t) return true;
		++ex_t;
	}
	return false;
}


/*
 * Tensors third order
 */
float_type Tensor_aaa(float_type da, float_type div_5, float_type div_7)
{
	return (9 * da/div_5 - 15 * da * da * da / div_7);	
}

float_type Tensor_aab(float_type da, float_type db, float_type div_5, float_type div_7)
{
	return (3 * db/div_5 - 15 * db * da * da / div_7);	
}

float_type Tensor_abc(float_type da, float_type db, float_type dc, float_type div_7)
{
	return -(15 * da * db * dc / div_7);	
}

/*Tensor 4th order */

float_type Tensor_aaab(float_type da, float_type db, float_type div_7, float_type div_9)
{
	return ((105 * da * da * da * db) / div_9   -   (45 * da * db) / div_7);
}

float_type Tensor_aabb(float_type da, float_type db, float_type div_5, float_type div_7, float_type div_9)
{
	return ((105 * da * da * db * db)/ div_9   -   (15 * (da * da + db * db) )/ div_7   +   3 / div_5);
}

float_type Tensor_aabc(float_type da, float_type db, float_type dc, float_type div_7, float_type div_9)
{
	return ((105 * da * da * db * dc) / div_9  -  (15 * db * dc) / div_7);
}

float_type Tensor_aaaa(float_type da, float_type div_5, float_type div_7, float_type div_9)
{
	return ((105 * da * da * da * da)/ div_9 - (90 * da * da) / div_7  +  9 /div_5);
}

void fill_Quadrupole_matrix(Matrix *matrix_q, float_type *q)
{
	float_type *ptr1, *ptr2;
	ptr1 = matrix_q->bp;
	ptr2 = ptr1 += 3;
	
	*ptr1 = *q; /*xx*/
	++ptr1; ++q;
	
	*ptr1 = *ptr2 = *q; /* xy */
	++ptr1; ++q;
	ptr2 += 3;
	
	*ptr1 = *ptr2 = *q; /* xz */
	++q;
	ptr1 += 2; 
	*ptr1 = *q; /* yy */
	
	++ptr1; ++ptr2; ++q;
	*ptr1 = *ptr2 = *q; /* yz */
	
	++q; ++ptr2;
	
	*ptr2 = *q;	/* zz */
}

void fill_Tensor_matrix_quad(Matrix *matrix_t, int i, Coordinates coord_s, Coordinates coord_t)
{
	float_type *ptr1, *ptr2, dx, dy, dz, distance, div_5, div_7;
	ptr1 = matrix_t->bp;
	ptr2 = ptr1 += 3;
	
	dx = (coord_s.x - coord_t.x);
	dy = (coord_s.y - coord_t.y);
	dz = (coord_s.z - coord_t.z);

	distance = pow(dx, 2.0) + pow(dy, 2.0) + pow(dz, 2.0);
	div_5 = pow(distance, 2.5);
	div_7 = pow(distance, 3.5);	
						
	switch(i)
	{
		case 0 : {
		*ptr1 = Tensor_aaa(dx, div_5, div_7); /*xx*/
		++ptr1;
	
		*ptr1 = *ptr2 = Tensor_aab(dx, dy, div_5, div_7); /* xy */
		++ptr1;
		ptr2 += 3;
	
		*ptr1 = *ptr2 = Tensor_aab(dx, dz, div_5, div_7); /* xz */
		ptr1 += 2; 
		*ptr1 = Tensor_aab(dy, dx, div_5, div_7); /* yy */
	
		++ptr1; ++ptr2;
		*ptr1 = *ptr2 = Tensor_abc(dx, dy, dz, div_7); /* yz */
	
		++ptr2;
	
		*ptr2 = Tensor_aab(dz, dx, div_5, div_7); /* zz */
	} break;
		case 1 :
		{
		*ptr1 = Tensor_aab(dx, dy, div_5, div_7); /* xx */
		++ptr1;
	
		*ptr1 = *ptr2 = Tensor_aab(dy, dx, div_5, div_7); /* xy */
		++ptr1;
		ptr2 += 3;
	
		*ptr1 = *ptr2 = Tensor_abc(dx, dy, dz, div_7); /* xz */
		ptr1 += 2; 
		*ptr1 = Tensor_aaa(dy, div_5, div_7); /* yy */
	
		++ptr1; ++ptr2;
		*ptr1 = *ptr2 = Tensor_aab(dy, dz, div_5, div_7); /* yz */
	
		++ptr2;
	
		*ptr2 = Tensor_aab(dz, dy, div_5, div_7); /* zz */			
			} break;
		case 2 : 
		{
		*ptr1 = Tensor_aab(dx, dz, div_5, div_7); /* xx */
		++ptr1;
	
		*ptr1 = *ptr2 = Tensor_abc(dx, dy, dz, div_7); /* xy */
		++ptr1;
		ptr2 += 3;
	
		*ptr1 = *ptr2 = Tensor_aab(dz, dx, dz, div_7); /* xz */
		ptr1 += 2; 
		*ptr1 = Tensor_aab(dy, dz, div_5, div_7); /* yy */
	
		++ptr1; ++ptr2;
		*ptr1 = *ptr2 = Tensor_aab(dz, dy, div_5, div_7); /* yz */
	
		++ptr2;
	
		*ptr2 = Tensor_aaa(dz, div_5, div_7); /* zz */			
			}break;
	}
}

Vector *calculate_F_mul(unsigned int no_mm_sites, unsigned long *exclude_list, unsigned long length_exclude_list, Coordinates *coordinate_list, float_type *mult_pc_list, Coordinates *mult_dp_list, Coordinates6D *mult_qp_list, Coordinates10D *mult_op_list)
{
	unsigned long i, j;
	Vector *vector_f_mul;
	Vector_t *f_mul;
	float_type *q, dx, dy, dz, distance, div_5 = 0, div_3, div_7 = 0, div_9 = 0;
	Coordinates *coord_s, *coord_t, *q_dp;
	Coordinates6D *q_qp;
	Coordinates10D *q_op;
	unsigned long *ex_s, *ex_t;


	message("calculating f_mul ...");
	vector_f_mul = vector_new(options.no_mm_sites * 3); /* 3 entries (x,y,z) per site*/
	
	ex_s = exclude_list;
	coord_s = coordinate_list;
	f_mul = vector_f_mul->bp;
	//#pragma omp parallel for private(q, q_dp, q_qp, q_op, coord_t, ex_t, i, j, dx, dy, dz, div_3, div_5, div_7, div_9, distance)
	for(i = 0; i < no_mm_sites; ++i) /*calculate elements of f_mul*/
	{
		q = mult_pc_list;
		q_dp = mult_dp_list;
		q_qp = mult_qp_list;
		q_op = mult_op_list;
		
		coord_t = coordinate_list;		
		ex_t = exclude_list;
		
		for(j = 0; j < no_mm_sites; ++j) /*other sites*/
		{
			if(is_in_exclude_list(length_exclude_list, ex_s+i*length_exclude_list, ex_t) == false && is_in_exclude_list(length_exclude_list, ex_t, ex_s+i*length_exclude_list) == false)
			{
				dx = ((coord_s+i)->x - coord_t->x);
				dy = ((coord_s+i)->y - coord_t->y);
				dz = ((coord_s+i)->z - coord_t->z);
				
				distance = pow(dx, 2.0) + pow(dy, 2.0) + pow(dz, 2.0);
				div_3 = pow(distance, 1.5);

				f_mul[i*3] += *q * dx / div_3; /*calculate f_mul_x*/
			
				f_mul[i*3 +1] += *q * dy / div_3; /*calculate f_mul_y*/
			
				f_mul[i*3 +2] += *q * dz / div_3; /*calculate f_mul_z*/


				if(mult_dp_list != NULL)
				{			
					div_5 = pow(distance, 2.5);
					
					/*calculate f_mul_x*/
					f_mul[i*3] += q_dp->x * (3.0 * dx * dx / div_5 - 1.0 / div_3) + 
								q_dp->y * 3.0 * dy * dx / div_5 +
								q_dp->z * 3.0 * dz * dx / div_5;
				
					/*calculate f_mul_y*/
					f_mul[i*3 +1] += q_dp->x * 3.0 * dx * dy / div_5 +
									q_dp->y * (3.0 * dy * dy / div_5 - 1.0 / div_3) +
									q_dp->z * 3.0 * dz * dy / div_5;
									
					/*calculate f_mul_z*/
					f_mul[i*3 +2] += q_dp->x * 3.0 * dx * dz / div_5 +
									q_dp->y * 3.0 * dy * dz / div_5 +
									q_dp->z * (3.0 * dz * dz / div_5 - 1.0 / div_3);		
				}
				
				if(mult_qp_list != NULL)
				{
					
					div_7 = pow(distance, 3.5);
					
					/*calculate f_mul_x*/
					f_mul[i*3] -= 0.5 * (q_qp->xx * Tensor_aaa(dx, div_5, div_7) + 2 * q_qp->xy * Tensor_aab(dx, dy, div_5, div_7)
					 + 2 * q_qp->xz * Tensor_aab(dx, dz, div_5, div_7) + q_qp->yy * Tensor_aab(dy, dx, div_5, div_7)
					 + 2 * q_qp->yz * Tensor_abc(dx, dy, dz, div_7) + q_qp->zz * Tensor_aab(dz, dx, div_5, div_7));
					
					/*calculate f_mul_y*/
					f_mul[i*3 +1] -= 0.5 * (q_qp->xx * Tensor_aab(dx, dy, div_5, div_7) + 2 * q_qp->xy * Tensor_aab(dy, dx, div_5, div_7)
					 + 2 * q_qp->xz * Tensor_abc(dx, dy, dz, div_7) + q_qp->yy * Tensor_aaa(dy, div_5, div_7)
					 + 2 * q_qp->yz * Tensor_aab(dy, dz, div_5, div_7) + q_qp->zz * Tensor_aab(dz, dy, div_5, div_7));
									
					/*calculate f_mul_z*/
					f_mul[i*3 +2] -= 0.5 * (q_qp->xx * Tensor_aab(dx, dz, div_5, div_7) + 2 * q_qp->xy * Tensor_abc(dy, dx, dz, div_7)
					 + 2 * q_qp->xz * Tensor_aab(dz, dx, div_5, div_7) + q_qp->yy * Tensor_aab(dy, dz, div_5, div_7)
					 + 2 * q_qp->yz * Tensor_aab(dz, dy, div_5, div_7) + q_qp->zz * Tensor_aaa(dz, div_5, div_7));	
				}
				
				if(mult_op_list != NULL)
				{
					div_9 = pow(distance, 4.5);
					f_mul[i*3] += (q_op->xxx * Tensor_aaaa(dx, div_5, div_7, div_9)  + 3 * q_op->xxy * Tensor_aaab(dx, dy, div_7, div_9) 
									+  3 * q_op->xxz *  Tensor_aaab(dx, dz, div_7, div_9) + 3 * q_op->xyy * Tensor_aabb(dx, dy, div_5, div_7, div_9) 
									+ 6 * q_op->xyz * Tensor_aabc(dx, dy, dz, div_7, div_9) + 3 * q_op->xzz * Tensor_aabb(dx, dz, div_5, div_7, div_9) 
									+ q_op->yyy * Tensor_aaab(dy, dx, div_7, div_9) + 3 * q_op->yyz * Tensor_aabc(dy, dx, dz, div_7, div_9) 
									+ 3 * q_op->yzz * Tensor_aabc(dz, dx, dy, div_7, div_9) + q_op->zzz * Tensor_aaab(dz, dx, div_7, div_9)     ) / 6;
					
					f_mul[i*3+1] += (q_op->xxx * Tensor_aaab(dx, dy, div_7, div_9)  + 3 * q_op->xxy * Tensor_aabb(dx, dy, div_5, div_7, div_9) 
									+  3 * q_op->xxz *  Tensor_aabc(dx, dy, dz, div_7, div_9) + 3 * q_op->xyy * Tensor_aaab(dy, dx, div_7, div_9) 
									+ 6 * q_op->xyz * Tensor_aabc(dy, dx, dz, div_7, div_9) + 3 * q_op->xzz * Tensor_aabc(dz, dx, dy, div_7, div_9) 
									+ q_op->yyy * Tensor_aaaa(dy, div_5, div_7, div_9) + 3 * q_op->yyz * Tensor_aaab(dy, dz, div_7, div_9) 
									+ 3 * q_op->yzz * Tensor_aabb(dz, dy, div_5, div_7, div_9) + q_op->zzz * Tensor_aaab(dz, dy, div_7, div_9)     ) / 6;
					
						
					f_mul[i*3+2] += (q_op->xxx * Tensor_aaab(dx, dz, div_7, div_9)  + 3 * q_op->xxy * Tensor_aabc(dx, dy, dz, div_7, div_9) 
									+  3 * q_op->xxz *  Tensor_aabb(dx, dz, div_5, div_7, div_9) + 3 * q_op->xyy * Tensor_aabc(dy, dx, dz, div_7, div_9) 
									+ 6 * q_op->xyz * Tensor_aabc(dz, dx, dy, div_7, div_9) + 3 * q_op->xzz * Tensor_aaab(dz, dx, div_7, div_9) 
									+ q_op->yyy * Tensor_aaab(dy, dz, div_7, div_9) + 3 * q_op->yyz * Tensor_aabb(dy, dz, div_5, div_7, div_9) 
									+ 3 * q_op->yzz * Tensor_aaab(dz, dy, div_7, div_9) + q_op->zzz * Tensor_aaaa(dz, div_5, div_7, div_9)     ) / 6;
				}
				
			}
				++q; ++q_dp;
				++q_qp;	++q_op;	

				++coord_t; /*next other site*/
				ex_t += length_exclude_list; /*set to begin of exlude list of next site*/
		}									
	}
	 
	return vector_f_mul;
}	

 
void calculate_tensor_second_order(Matrix *matrix_R, unsigned long i, unsigned long j)
{
	Matrix_t *R;
	Coordinates *coord_s, *coord_t;
	float_type div_5, div_3, sign;
	
	coord_s = options.coordinate_list + i;
	coord_t = options.coordinate_list + j;
	
	R = matrix_pointer_to_adress(matrix_R, i*3, j*3);
	div_5 = pow((   pow(coord_t->x-coord_s->x, 2.0) + pow(coord_t->y-coord_s->y, 2.0) + pow(coord_t->z-coord_s->z, 2.0)   ), 2.5);
	div_3 = pow((   pow(coord_t->x-coord_s->x, 2.0) + pow(coord_t->y-coord_s->y, 2.0) + pow(coord_t->z-coord_s->z, 2.0)   ), 1.5);
			
	sign = -1.0;
	*R = sign * ((3.0 * pow( coord_s->x - coord_t->x , 2) / div_5) - (1.0 / div_3));
	++R;
			
	*R = sign * (3.0 * (coord_t->x - coord_s->x) * (coord_t->y - coord_s->y) / div_5);
	++R;
			
	*R = sign * (3.0 * (coord_t->x - coord_s->x) * (coord_t->z - coord_s->z) / div_5);
	R += options.no_mm_sites * 3 - 2;			
	//first row complete
			
	*R = sign * (3.0 * (coord_t->y - coord_s->y) * (coord_t->x - coord_s->x) / div_5);
	++R;
			
	*R = sign * ((3.0 * pow( coord_s->y - coord_t->y , 2) / div_5) - (1.0 / div_3));
	++R;
			
	*R = sign * (3.0 * (coord_t->y - coord_s->y) * (coord_t->z - coord_s->z) / div_5);
	R += options.no_mm_sites * 3 - 2;
	//second row complete

	*R = sign * (3.0 * (coord_t->z - coord_s->z) * (coord_t->x - coord_s->x) / div_5);
	++R;			
			
	*R = sign * (3.0 * (coord_t->z - coord_s->z) * (coord_t->y - coord_s->y) / div_5);
	++R;
			
	*R = sign * ((3.0 * pow( coord_s->z - coord_t->z , 2) / div_5) - (1.0 / div_3));
	//third row complete
} 

void calculate_tensor_second_order_half(Matrix_half *matrix_R, unsigned long i, unsigned long j)
{
	Coordinates *coord_s, *coord_t;
	float_type div_5, div_3, sign;
	
	coord_s = options.coordinate_list + i;
	coord_t = options.coordinate_list + j;
	
	
	div_5 = pow((   pow(coord_t->x-coord_s->x, 2.0) + pow(coord_t->y-coord_s->y, 2.0) + pow(coord_t->z-coord_s->z, 2.0)   ), 2.5);
	div_3 = pow((   pow(coord_t->x-coord_s->x, 2.0) + pow(coord_t->y-coord_s->y, 2.0) + pow(coord_t->z-coord_s->z, 2.0)   ), 1.5);
			
	sign = -1.0;
	*matrix_ptr_to_adr_half(matrix_R, 3*i, 3*j) = sign * ((3.0 * pow( coord_s->x - coord_t->x , 2) / div_5) - (1.0 / div_3));
					
				
	*matrix_ptr_to_adr_half(matrix_R, 3*i, 3*j+1) = *matrix_ptr_to_adr_half(matrix_R, 3*i+1, 3*j) = sign * (3.0 * (coord_t->y - coord_s->y) * (coord_t->x - coord_s->x) / div_5);
			
	*matrix_ptr_to_adr_half(matrix_R, 3*i+1, 3*j+1) = sign * ((3.0 * pow( coord_s->y - coord_t->y , 2) / div_5) - (1.0 / div_3));	
	
	
	*matrix_ptr_to_adr_half(matrix_R, 3*i, 3*j+2) = *matrix_ptr_to_adr_half(matrix_R, 3*i+2, 3*j) = sign * (3.0 * (coord_t->z - coord_s->z) * (coord_t->x - coord_s->x) / div_5);
			
	*matrix_ptr_to_adr_half(matrix_R, 3*i+1, 3*j+2) = *matrix_ptr_to_adr_half(matrix_R, 3*i+2, 3*j+1) = sign * (3.0 * (coord_t->z - coord_s->z) * (coord_t->y - coord_s->y) / div_5);
			
	*matrix_ptr_to_adr_half(matrix_R, 3*i+2, 3*j+2) = sign * ((3.0 * pow( coord_s->z - coord_t->z , 2) / div_5) - (1.0 / div_3));
}

void calculate_a_inverse_half(Matrix_half *matrix_R, unsigned long k)
{
	float_type a, b, c, d, e, f, g, h, i, div, *pol;
	
	pol = options.polarizability_list+k*6;
		
	a = *pol;
	pol++;
	d = b = *pol;
	pol++;	
	g = c = *pol;
	pol++;
	e = *pol;
	pol++;
	h = f = *pol;
	pol++;
	i = *pol;
	
	div = a*e*i + b*f*c + b*f*c - c*c*e - f*f*a - i*b*b;
	
	*matrix_ptr_to_adr_half(matrix_R, 3*k, 3*k) = (e*i - f*h) / div;
	
	
	*matrix_ptr_to_adr_half(matrix_R, 3*k+1, 3*k) = (f*g - d*i) / div;
	
	*matrix_ptr_to_adr_half(matrix_R, 3*k+1, 3*k+1) = (a*i - c*g) / div;
	
		
	*matrix_ptr_to_adr_half(matrix_R, 3*k+2, 3*k) = (d*h - e*g) / div;
	
	*matrix_ptr_to_adr_half(matrix_R, 3*k+2, 3*k+1) = (b*g-a*h) / div;
	
	*matrix_ptr_to_adr_half(matrix_R, 3*k+2, 3*k+2) = (a*e-b*d) / div;
}

void calculate_a_inverse(Matrix *matrix_R, unsigned long k)
{
	float_type a, b, c, e, f, i, div, *pol;
	Matrix_t *R, *R_s;
	
	pol = options.polarizability_list+k*6;
	R = matrix_pointer_to_adress(matrix_R, k * 3, k * 3);
	R_s = matrix_pointer_to_adress(matrix_R, k*3 +1, k*3);
	
	a = *pol;
	pol++;
	b = *pol;
	pol++;	
	c = *pol;
	pol++;
	e = *pol;
	pol++;
	f = *pol;
	pol++;
	i = *pol;
	
	div = a*e*i + b*f*c + b*f*c - c*c*e - f*f*a - i*b*b;
	
	*R = (e*i - f*f) / div;
	++R;
	
	*R = (c*f - b*i) / div;
	*R_s = *R;
	++R;
	R_s += options.no_mm_sites * 3;
	
	*R = (b*f - c*e) / div;
	*R_s = *R;
	R += options.no_mm_sites * 3 - 1;
	++R_s;
	
	*R = (a*i - c*c) / div;
	++R;
	
	*R = (c*b-a*f) / div;
	*R_s = *R;
	R += options.no_mm_sites * 3;
	
	*R = (a*e-b*b) / div;
}

Matrix *calculate_R_inverse(void)
{
	Matrix *matrix_R;
	unsigned long i, j;
	float_type a;
	
	matrix_R = matrix_new(options.no_mm_sites * 3, options.no_mm_sites * 3);
	message("calculating R_inverse...");
		
	if(options.order_pol == ISOTROPIC)
	{
		#pragma omp parallel for private(i, a)
		for(i = 0; i < options.no_mm_sites; ++i)
		{
			a = *(options.polarizability_list+i);
			*matrix_pointer_to_adress(matrix_R, 3*i  , 3*i  ) = 1.0 / a;
			*matrix_pointer_to_adress(matrix_R, 3*i+1, 3*i+1) = 1.0 / a;
			*matrix_pointer_to_adress(matrix_R, 3*i+2, 3*i+2) = 1.0 / a;
		}
	}
	else if(options.order_pol == ANISOTROPIC)
	{
		#pragma omp parallel for
		for(i = 0; i < options.no_mm_sites; ++i)
		{
			calculate_a_inverse(matrix_R, i);
		}	
	}
	
	#pragma omp parallel for private(i, j)
	for(i = 0; i < options.no_mm_sites; ++i)
	{
		for(j = 0; j < options.no_mm_sites; ++j)
		{
			if(i != j && !is_in_exclude_list(options.length_exclude_list, options.exclude_list+i*options.length_exclude_list, options.exclude_list+j*options.length_exclude_list) 
			&& !is_in_exclude_list(options.length_exclude_list, options.exclude_list+j*options.length_exclude_list, options.exclude_list+i*options.length_exclude_list))
			{
				calculate_tensor_second_order(matrix_R, i, j);
			}						
		}
	}
		
	return matrix_R;
}

Matrix_half *calculate_R_inverse_half(void)
{
	Matrix_half *matrix_R;
	unsigned long i, j;
	float_type a;
	
	message("calculating R_inverse...");
	matrix_R = matrix_new_half(options.no_mm_sites * 3, options.no_mm_sites * 3);
	
	if(options.order_pol == ISOTROPIC)
	{
		#pragma omp parallel for private(i, a)
		for(i = 0; i < options.no_mm_sites; ++i)
		{
			a = *(options.polarizability_list+i);
			*matrix_ptr_to_adr_half(matrix_R, 3*i  , 3*i  ) = 1.0 / a;
			*matrix_ptr_to_adr_half(matrix_R, 3*i+1, 3*i+1) = 1.0 / a;
			*matrix_ptr_to_adr_half(matrix_R, 3*i+2, 3*i+2) = 1.0 / a;
		}
	}
	else if(options.order_pol == ANISOTROPIC)
	{
		#pragma omp parallel for
		for(i = 0; i < options.no_mm_sites; ++i)
		{
			calculate_a_inverse_half(matrix_R, i);
		}	
	}
	
	//#pragma omp parallel for private(i, j)
	for(i = 0; i < options.no_mm_sites; ++i)
	{
		for(j = i + 1; j < options.no_mm_sites; ++j)
		{
			if(!is_in_exclude_list(options.length_exclude_list, options.exclude_list+i*options.length_exclude_list, options.exclude_list+j*options.length_exclude_list) 
				&& !is_in_exclude_list(options.length_exclude_list, options.exclude_list+j*options.length_exclude_list, options.exclude_list+i*options.length_exclude_list))
			{
				calculate_tensor_second_order_half(matrix_R, i, j);
			}						
		}
	}
		
	return matrix_R;
}

Vector *calculate_induced_dipols_nomb(Vector *vector_f_mul)
{
	Vector *dipols;
	unsigned long length, i;
	float_type *p, *f_mul, *d;
	
	length = vector_f_mul->length;
	dipols = vector_new(length);
	
	d = dipols->bp;
	f_mul = vector_f_mul->bp;
	p = options.polarizability_list;
	
	if( options.order_pol == NONE)
	{
		for(i = 0; i < length; ++i)
		{
			*d = 0;
			++d;
		}
	}
	else if(options.order_pol == ISOTROPIC)
	{
		for(i = 0; i < length; ++i)
		{
			if(i % 3 == 0 && i != 0) ++p;
			*d = *p * *f_mul;
			++d; ++f_mul;
		}
	}
	else if(options.order_pol == ANISOTROPIC)
	{
		for(i = 0; i < options.no_mm_sites; ++i)
		{
			*d += *p * *f_mul;
			++f_mul; ++p;
			*d += *p * *f_mul;
			++f_mul; ++p;
			*d += *p * *f_mul;
			
			f_mul -= 2;
			++d;
			--p;
			*d += *p * *f_mul;
			++f_mul; p += 2;
			*d += *p * *f_mul;
			++f_mul; ++p;
			*d += *p * *f_mul;
			
			f_mul -= 2;
			++d;
			p -= 2;
			*d += *p * *f_mul;
			++f_mul; p += 2;
			*d += *p * *f_mul;
			++f_mul; ++p;
			*d += *p * *f_mul;
		}		
	}
	return dipols;
}
