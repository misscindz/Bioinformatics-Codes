#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <omp.h>
#include <time.h>
#include "linear_equatations_solver.h"
#include "read_and_print.h"
#include "calculate.h"
#include <sys/time.h>
#include <string.h>
#include <math.h>


Options options;  /* declared in read_and_print.h */

int main(int argc, char * argv[])
{
	Vector *x, *f_mul, *x_0, *f_mul_save;
	Matrix *A;	
	Matrix_half *A_;
	float_type time;
	struct timeval start_time;                             
	struct timeval comp_time;
		
	x = NULL;
	
	read_and_set_options(argc, argv);
	
	if(options.cube == 0)
	{
		read_file(options.file);
		if(options.order_k == OCTOPOLE) printf("Warning, octopoles seem to have a bug!\n");
	}
	else
	{
		cube_test(options.cube);
	}
	

	f_mul = calculate_F_mul(options.no_mm_sites, options.exclude_list, options.length_exclude_list, options.coordinate_list, options.mult_pc_list, options.mult_dp_list, options.mult_qp_list, options.mult_op_list);
	f_mul_save = vector_copy(f_mul);
	
	gettimeofday(&start_time, NULL);
	if(options.method == LDLT_DECOMPOSITION)
	{
		if(strcmp(options.load_matrix_file, "NOFILE") == 0)  A_ = calculate_R_inverse_half();
		else A_ = matrix_new_half(options.no_mm_sites*3, options.no_mm_sites*3);
		
		x = solve_with_LDLT(A_, f_mul);
		matrix_delete_half(A_);
	}
	else if(options.method == LU_DECOMPOSITION)
	{
		A = calculate_R_inverse();
		x = solve_with_LU(A, f_mul);
		matrix_delete(A);
	}
	else if(options.method == JACOBI)
	{
		A = calculate_R_inverse();
		x_0 = vector_new(options.no_mm_sites*3);
		if(strcmp(options.load_startvector_file, "NOFILE") != 0)
		{
			message("loading vector");
			load_vector(x_0, options.load_startvector_file);
		}
		x = solve_with_JC(A, f_mul, x_0);
		matrix_delete(A);
	}
	else if(options.method == GAUSS)
	{
		A = calculate_R_inverse();
		x_0 = vector_new(options.no_mm_sites*3);
		if(strcmp(options.load_startvector_file, "NOFILE") != 0)
		{
			message("loading vector");
			load_vector(x_0, options.load_startvector_file);
		}
		x = solve_with_GS(A, f_mul, x_0);
		matrix_delete(A);
	}
	else if(options.method == GAUSS_HALF)
	{
		A_ = calculate_R_inverse_half();
		x_0 = vector_new(options.no_mm_sites*3);
		if(strcmp(options.load_startvector_file, "NOFILE") != 0)
		{
			message("loading vector");
			load_vector(x_0, options.load_startvector_file);
		}
		x = solve_with_GS_half(A_, f_mul, x_0);
		matrix_delete_half(A_);
	}
	
	
	
	if(strcmp(options.save_dipols_file, "NOFILE") != 0)
	{
		message("saving dipols");
		save_vector(x, options.save_dipols_file);
	}
	
	
	gettimeofday(&comp_time, NULL);
	time = (comp_time.tv_sec - start_time.tv_sec) + (comp_time.tv_usec - start_time.tv_usec) * 1e-6;
	printf("threads:  %d    time needed:    %f s \n", options.number_of_threads, time);
	
	if(options.print_dipols)
	{
		printf("induced dipols: \n");
		vector_print(x, stdout);
	}
	
	printf("Energy is %.15lf\n", -0.5*vector_mult_vector(x, f_mul_save));
	
	vector_delete(x);
	vector_delete(f_mul);
	vector_delete(f_mul_save);
	
	free_option_pointer();
		
	return EXIT_SUCCESS;
}
