#ifndef CALCULATE_H 
#define CALCULATE_H

#include "vector.h"
#include "matrix.h"
#include "read_and_print.h"

Vector *calculate_F_mul(unsigned int no_mm_sites, unsigned long *exclude_list, unsigned long length_exclude_list, Coordinates *coordinate_list, float_type *mult_pc_list, Coordinates *mult_dp_list, Coordinates6D *mult_qp_list, Coordinates10D *mult_op_list);

Matrix *calculate_R_inverse(void);

Matrix_half *calculate_R_inverse_half(void);

Vector *calculate_induced_dipols_nomb(Vector *vector_f_mul);

#endif
