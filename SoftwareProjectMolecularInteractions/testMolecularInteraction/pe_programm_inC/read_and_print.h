#ifndef READ_AND_PRINT_H
#define READ_AND_PRINT_H

#include <stdio.h>
#include "vector.h"
#include <stdbool.h>
#include "matrix.h"


typedef enum{LDLT_DECOMPOSITION, LU_DECOMPOSITION, JACOBI, GAUSS, GAUSS_HALF} Method;
typedef enum{AA, AU} Unit;
typedef enum{POINT = 1, DIPOLE = 3, QUADRUPOLE = 6, OCTOPOLE = 10} Multipole;
typedef enum{NONE = 0, ISOTROPIC = 1, ANISOTROPIC = 6} Polarizabilities;
typedef struct{float_type x,y,z;} Coordinates;
typedef struct{float_type xx,xy,xz,yy,yz,zz;} Coordinates6D;
typedef struct{float_type xxx,xxy,xxz,xyy,xyz,xzz,yyz,yyy,yzz,zzz;} Coordinates10D;



typedef struct{unsigned int number_of_threads, max_iter, check_iter, cube;
				double maxresiduum;
				unsigned long no_mm_sites, length_exclude_list, *exclude_list;
				Method method;
				int messages, print_dipols;
				char file[250], load_matrix_file[250], save_matrix_file[250], save_dipols_file[250], load_startvector_file[250];
				Unit unit;
				Polarizabilities order_pol;
				Multipole order_k;
				Coordinates *coordinate_list, *mult_dp_list;
				Coordinates6D *mult_qp_list;
				Coordinates10D *mult_op_list;
				float_type *mult_pc_list, *polarizability_list;
				Vector *x_0;
				} Options;

Options options;

void error(bool condition, char *message);

void message(char *message);

void read_and_set_options(int argc,char *argv[]);
 
void read_file(char *filename);

void free_option_pointer(void);

void cube_test(int n);

void save_matrix(Matrix_half *A, unsigned long *vector_p, char *diagonal, char *filename);

void load_matrix(Matrix_half *A, unsigned long *vector_p, char *diagonal, char *filename);

void save_vector(Vector *vector, char *filename);

void load_vector(Vector *vector, char *filename);
 
#endif
