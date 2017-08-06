#ifndef LINEAR_EQUATATIONS_SOLVER_H 
#define LINEAR_EQUATATIONS_SOLVER_H

void LU_decomposition(Matrix *A, unsigned long *vector_p);

Vector *forward_substitution(Matrix *matrix_LU, const Vector *vector_b);

Vector *backward_substitution(Matrix *matrix_LU, const Vector *vector_y);

Vector *forward_substitution_LDLT(Matrix *matrix_LU, const Vector *vector_b, char *elemination_type);

Vector *D_substitution(Matrix *matrix_LU, const Vector *vector_y, char *elemination_type);

Vector *back_substitution_LDLT(Matrix *matrix_LU, const Vector *vector_y, char *elemination_type);

Vector *solve_with_LDLT(Matrix_half *matrix_A, Vector *vector_b);

/*
 * Solves a system of linear equatations ( Ax = b ) with LU-decomposition and returns x.
 * Warning: Matrix A is changed while calculating LU-decomposition.
 * Warning: Vector b is changed while calculating LU-decomposition.
 * 			Use permute_back in solve_with_LU or save before calculating.
 * */
Vector *solve_with_LU(Matrix *matrix_A, Vector *vector_b);

void permutate(Vector *vector_b, unsigned long *vector_p);

void permutate_back(Vector *vector_b, unsigned long *vector_p);

Vector *solve_with_JC(const Matrix *matrix_A, const Vector *vector_b, Vector *x_0);

Vector *solve_with_GS(const Matrix *matrix_A, Vector *vector_b, Vector *x_0);

Vector *solve_with_GS_half(const Matrix_half *matrix_A, Vector *vector_b, Vector *x_0);

void LDLT_decomposition(Matrix *matrix_A, unsigned long *vector_p, char *diagonal);

Matrix_t *matrix_ptr_to_adr_half(const Matrix_half *bp, unsigned long zeile, unsigned long spalte);

Matrix_t *matrix_pointer_to_adress(const Matrix *bp, unsigned long zeile, unsigned long spalte);

void symmetric_swap_half(Matrix_half *matrix_A, int swap1, int swap2);

#endif
