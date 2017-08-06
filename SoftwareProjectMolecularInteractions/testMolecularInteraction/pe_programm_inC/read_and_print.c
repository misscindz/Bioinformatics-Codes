#include "read_and_print.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <getopt.h>
#include <omp.h>
#include "matrix.h"
#include <math.h>

#define ERROR ("invalid file format")
#define AA_TO_AU 1.889725989

void print_help(void)
{
	printf("USSAGE: [options] inputfile \n");
	printf("Options:\n");
	printf("\n");
	
	printf("-m or -method uint: \n");
	printf("0 : (default) LDLT-Decomposition (direct, uses only half matrix)\n");
	printf("1 : LU-Decomposition (direct, slower than LDLT, not recommended)\n");
	printf("2 : Jacobi-method (iterative)\n");
	printf("3 : Gauß-Seidel-method (iterative)\n");
	printf("4 : Gauß-Seidel-method on half-matrix (iterative)\n");
	printf("\n");
	
	printf("-t or -threads uint: number of threads (default maximum availible)\n");
	printf("\n");
	
	printf("-h or -help : print this help\n");
	printf("\n");
	
	printf("-save_matrix or -s file: saves LDLT-Decomposition in file, file will be overwritten\n");
	printf("\n");
	
	printf("-load_matrix or -l file: loads LDLT_Decomposition\n");
	printf("\n");
	
	printf("-save_dipols or -d file: saves induced dipols in a binary file. The file will be overwritten\n");
	printf("\n");
	
	printf("-load_startvector or -v file: loads vector, which will be used for iterative methods\n");
	printf("\n");
	
	printf("-no_dipols: induced dipols are not printed to stdout\n");
	printf("\n");
	
	printf("-maxiter or -i uint: maximum of iterations for iterative methods (default 100)\n");
	printf("\n");
	
	printf("-maxresiduum or -f float: if residuum is less than float, iterative method will be aborded (default 0)\n");
	printf("\n");
}

void message(char *message)
{
	if(options.messages) fprintf(stdout, "%s\n", message);
}
void error(bool condition, char *message)
{
	if(condition)
	{
		fprintf(stderr, "%s\n", message);
		exit(EXIT_FAILURE);
	}
}
 
void read_and_set_options(int argc, char *argv[]) 
{
	int c;
	unsigned int i;
	
	options.cube = 0;
	options.method = LDLT_DECOMPOSITION;
	options.messages = false;
	options.maxresiduum = 0.0;
	options.max_iter = 100;
	options.x_0 = NULL; 
	options.print_dipols = 1;
	options.number_of_threads = omp_get_max_threads();
	strcpy(options.load_matrix_file, "NOFILE");
	strcpy(options.save_matrix_file, "NOFILE");
	strcpy(options.load_startvector_file, "NOFILE");
	strcpy(options.save_dipols_file, "NOFILE");
	
	if(argc == 1) printf("use -h or -help for help\n");
	
	 while (1)
         {
           static struct option long_options[] =
             {
				{"messages", no_argument, &options.messages, 1},
				{"no_dipols", no_argument, &options.print_dipols, 0},
				{"load_matrix", required_argument, 0, 'l'},
				{"save_matrix", required_argument, 0, 's'},
				{"load_startvector", required_argument, 0, 'v'},
				{"save_dipols", required_argument, 0, 'd'},
				{"maxiter", required_argument, 0, 'i'},
				{"maxresiduum", required_argument, 0, 'f'},
				{"method",  required_argument, 0, 'm'},
				{"threads", required_argument, 0, 't'},
				{"cube",    required_argument, 0, 'c'},
				{"help",    no_argument, 0, 'h'},
				{0, 0, 0, 0}
             };
           /* getopt_long stores the option index here. */
           int option_index = 0;
     
           c = getopt_long_only (argc, argv, "m:t:c:hf:l:s:v:d:",
                            long_options, &option_index);
     
           /* Detect the end of the options. */
           if (c == -1)
				break;
     
			switch (c)
			{     
				case 'm':
					error(sscanf(optarg, "%u", &i) == 0, "wrong parameter");
					switch (i)
						{
							case 0 : options.method = LDLT_DECOMPOSITION; break;
							case 1 : options.method = LU_DECOMPOSITION; break;
							case 2 : options.method = JACOBI; break;
							case 3 : options.method = GAUSS; break;
							case 4 : options.method = GAUSS_HALF; break;
							default : error(true, "wrong method");
						}
					break;
     
				case 't':
					error(sscanf(optarg, "%u", &options.number_of_threads) == 0, "wrong parameter");
					error(options.number_of_threads < 1, "number of threads < 1");
					omp_set_num_threads(options.number_of_threads);
					break;
               
				case 'c':
					error(sscanf(optarg, "%u", &options.cube) == 0, "wrong parameter");
					break;
			 
				case 'h':
					print_help();
					exit(EXIT_SUCCESS);
					break;
				
				case 'f':
					error(sscanf(optarg, "%lf", &options.maxresiduum) == 0, "wrong paramter");
					break;
					
				case 'i':
					error(sscanf(optarg, "%u", &options.max_iter) == 0, "wrong parameter");
					break;
					
				case 'l':
					strcpy(options.load_matrix_file, optarg);
					break;
					
				case 's':
					strcpy(options.save_matrix_file, optarg);
					break;
					
				case 'd':
					strcpy(options.save_dipols_file, optarg);
					break;
					
				case 'v':
					strcpy(options.load_startvector_file, optarg);
					break;
					
				}
			}
     
		if(options.cube == 0)
		{
			/* Print any remaining command line arguments (not options). */
			error(optind != argc-1, "no input file");
			strcpy(options.file, argv[optind]);
		}
}

void save_matrix(Matrix_half *A, unsigned long *vector_p, char *diagonal, char *filename)
{
	unsigned long no;
	unsigned int method;
	FILE *file;
	
	no = options.no_mm_sites * 3;
	method = options.method;
	
	file = fopen(filename, "w");
	error(file == NULL, "error while opening file for write");
	fwrite(&method, sizeof(unsigned int), 1, file);
	fwrite(&no, sizeof(unsigned long), 1, file);
	fwrite(A->bp, sizeof(Matrix_t), A->zeilen*(A->zeilen+1)/2, file);
	fwrite(vector_p, sizeof(unsigned long), no, file);
	fwrite(diagonal, sizeof(char), no, file); 
	fclose(file);
}

void load_matrix(Matrix_half *A, unsigned long *vector_p, char *diagonal, char *filename)
{
	unsigned long no;
	unsigned int method;
	FILE *file;
	
	file = fopen(filename, "r");
	error(file == NULL, "error while opening matrix file for read");
	
	error(fread(&method, sizeof(unsigned int), 1, file) != 1, ERROR);
	error(method != LDLT_DECOMPOSITION, "method of loaded matrix and options not same");
	
	error(fread(&no, sizeof(unsigned long), 1, file) != 1, ERROR);	
	error(no != options.no_mm_sites*3, "number of elements not same");
	
	error(fread(A->bp, sizeof(Matrix_t), A->zeilen*(A->zeilen+1)/2, file) != A->zeilen*(A->zeilen+1)/2, ERROR); 
	error(fread(vector_p, sizeof(unsigned long), no, file) != no, ERROR);
	error(fread(diagonal, sizeof(char), no, file) != no, ERROR);
	
	fclose(file);
}

void save_vector(Vector *vector, char *filename)
{
	unsigned long no;
	FILE *file;
	
	no = options.no_mm_sites*3;
	
	file = fopen(filename, "w");
	error(file == NULL, "error while opening file for write");
	
	fwrite(&no, sizeof(unsigned long), 1, file);
	fwrite(vector->bp, sizeof(Vector_t), no, file);
	
	fclose(file);
}

void load_vector(Vector *vector, char *filename)
{
	unsigned long no;
	FILE *file;
	
	file = fopen(filename, "r");
	error(file == NULL, "error while opening  vectorfile for read");
	
	error(fread(&no, sizeof(unsigned long), 1, file) != 1, ERROR);
	error(no != options.no_mm_sites*3, "number of elements not same");
	error(fread(vector->bp, sizeof(Vector_t), no, file) != no, ERROR);
	
	fclose(file);
}

void cube_test(int n)
{
	int i, j, k;
	unsigned long *ex;
	float_type  *q, *pol;
	Coordinates *coord;
	
	options.no_mm_sites = n;
	
	
	options.length_exclude_list = 1;
	options.order_k = POINT;
	options.order_pol = ISOTROPIC;
		
	options.coordinate_list = malloc(options.no_mm_sites * sizeof(Coordinates));
	options.mult_pc_list = malloc(options.no_mm_sites * sizeof(float_type) * options.order_k);
	options.polarizability_list = malloc(options.no_mm_sites * sizeof(float_type) * options.order_pol);
	options.exclude_list = malloc(options.no_mm_sites * sizeof (unsigned long) * options.length_exclude_list);
	
	q = options.mult_pc_list;
	pol = options.polarizability_list;
	coord = options.coordinate_list;

	j = k = 0;
	for(i = 0; i < n; ++i)
	{
		coord->x = AA_TO_AU * 3.0 * (i % (int) round(sqrt(n)));
		coord->y = AA_TO_AU * 3.0 * j;
		coord->z = AA_TO_AU * 3.0 * k;
		*q = 0.03;
		*pol = 5.00;
				
		++coord;
		++q;
		++pol;
		if(i % (int) round(sqrt(n)) == 0) ++j;
		if(j % (int) round(sqrt(n)) == 0) {j = 0; ++k;}
	}
	
	
	ex = options.exclude_list;
	for(i = 0; i < n; ++i)
	{
		*ex = i;
		++ex;
	}
	
	
}

void read_file(char *filename)
{
	FILE *file;
	int p, k;
	char *length_unit, c;
	unsigned long *ex, i, j;
	Coordinates *coord, *mul_dp;
	Coordinates6D *mul_qp;
	Coordinates10D *mul_op;
	float_type *pol, *mul_pc;
	float var;
	
	
	file = fopen(filename, "r");
	error(file == NULL, "cannot open file");
	
	c = '0';
	while (c != '\n') /*ignore first line*/
	{
		c = fgetc(file);
	}
	length_unit = malloc(5*sizeof(char));
	error(fscanf(file, "%s", length_unit) != 1, ERROR); 
	
	
	if(strcmp(length_unit, "AA") == 0)	options.unit = AA;
	else if (strcmp(length_unit, "AU") == 0) options.unit = AU;
	else error(true, "wrong length unit");
	
	free(length_unit);
	
	error( fscanf(file, "%lu %i %i %lu", &options.no_mm_sites, &k, &p, &options.length_exclude_list) != 4, ERROR);
	error(options.length_exclude_list < 1, "error: exclude list < 1");
	c = fgetc(file); /*check if line ends */
	while( c == ' ' || c == '\t')
	{
		c = fgetc(file);
	}
	error(c != '\n', ERROR);
	
	switch(k)
	{
		case 0 : options.order_k = POINT; break;
		case 1 : options.order_k = DIPOLE; break;
		case 2 : options.order_k = QUADRUPOLE; break;
		case 3 : options.order_k = OCTOPOLE; break;
	}
	switch(p)
	{
		case 0 : options.order_pol = NONE; break;
		case 1 : options.order_pol = ISOTROPIC; break;
		case 2 : options.order_pol = ANISOTROPIC; break; 		
	}
	
	options.coordinate_list = malloc(options.no_mm_sites * sizeof(Coordinates));
	options.mult_pc_list = malloc(options.no_mm_sites * sizeof(float_type) * options.order_k);
	options.polarizability_list = malloc(options.no_mm_sites * sizeof(float_type) * options.order_pol);
	options.exclude_list = malloc(options.no_mm_sites * sizeof (unsigned long) * options.length_exclude_list);
	
	if(options.order_k >= DIPOLE) options.mult_dp_list = malloc(options.no_mm_sites * sizeof(Coordinates));
	if(options.order_k >= QUADRUPOLE) options.mult_qp_list = malloc(options.no_mm_sites * sizeof(Coordinates6D)); 
	if(options.order_k >= OCTOPOLE) options.mult_op_list = malloc(options.no_mm_sites * sizeof(Coordinates10D));
	
	ex = options.exclude_list;
	coord = options.coordinate_list;
	mul_pc = options.mult_pc_list;
	mul_dp = options.mult_dp_list;
	mul_qp = options.mult_qp_list;
	mul_op = options.mult_op_list;
	pol = options.polarizability_list;
	
	for(i = 0; i < options.no_mm_sites; ++i)
	{
		/*read exclusion list of line*/
		for(j = 0; j < options.length_exclude_list; ++j)
		{
			error(fscanf(file, "%lu", ex) != 1, ERROR);
			++ex;
		}
		
		/*read coordinates of line*/
		error(fscanf(file, "%f", &var) != 1, ERROR);
		coord->x = var;		
		error(fscanf(file, "%f", &var) != 1, ERROR);
		coord->y = var;		
		error(fscanf(file, "%f", &var) != 1, ERROR);
		coord->z = var;
		
	
		if(options.unit == AA) /*convert to AU*/
		{
			coord->x *= AA_TO_AU;
			coord->y *= AA_TO_AU;
			coord->z *= AA_TO_AU;
		}
		++coord;
		
		/*readlist of multipoles point charges*/
		error(fscanf(file, "%f", &var) != 1, ERROR);
		*mul_pc = var;
		++mul_pc;

		/*readlist of multipoles dipole moments*/
		if (options.order_k >= DIPOLE)
		{
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_dp->x = var; 
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_dp->y = var; 
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_dp->z = var; 
			++mul_dp;
		
		}
		
		/*readlist of multipoles quadropule moments*/
		if (options.order_k >= QUADRUPOLE)
		{
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_qp->xx = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_qp->xy = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_qp->xz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_qp->yy = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_qp->yz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_qp->zz = var;
			
			++mul_qp;
		}
		
		/*readlist of multipoles octopole moments*/
		if (options.order_k >= OCTOPOLE)
		{
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->xxx = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->xxy = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->xxz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->xyy = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->xyz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->xzz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->yyy = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->yyz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->yzz = var;
			error(fscanf(file, "%f", &var) != 1, ERROR); mul_op->zzz = var;
			
			++mul_op;
		}
		
		for(j = 0; j < options.order_pol; ++j)
		{
			error(fscanf(file, "%f", &var) != 1, ERROR); *pol = var;
			++pol;
		}
		
		c = fgetc(file); /* check if line ends */
		while( c == ' ' || c == '\t')
		{
			c = fgetc(file);
		}
		error(c != '\n', ERROR);		
	}
	
	error(fclose(file) != 0, "error while closing file");
}

void free_option_pointer(void)
{
	free(options.coordinate_list);
	free(options.mult_pc_list);
	free(options.mult_dp_list);
	free(options.mult_qp_list);
	free(options.mult_op_list);
	free(options.polarizability_list);
	free(options.exclude_list);
}


	/*
	ex = options.exclude_list;
	coord = options.coordinate_list;
	mul = options.multipole_list;
	pol = options.polarizability_list;
	for(i = 0; i < options.no_mm_sites; ++i)
	{
		for(j = 0; j < options.length_exclude_list; ++j)
		{
			printf("%ld  ", *ex);
			++ex;
		}
		
		printf("%lf  ", coord->x);
		printf("%lf  ", coord->y);
		printf("%lf  ", coord->z);
		++coord;
		
		for(j = 0; j < options.order_k; ++j)
		{
			printf("%lf  ", *mul);
			++mul;
		}
		
		for(j = 0; j < options.order_pol; ++j)
		{
			printf("%lf  ", *pol);
			++pol;
		}	
		printf("\n");
	}
	*/
