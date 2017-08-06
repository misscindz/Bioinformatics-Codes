#ifndef RVEC_H
#define RVEC_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define NDIM 2
typedef double real;
typedef real rvec[NDIM];
typedef unsigned int uint;
typedef unsigned long ulong;

/* u[i] = val for all i */
static void rvec_set(rvec *u, real val) {
    for (uint i = 0; i < NDIM; i++)
        (*u)[i] = val;
}

/* set the components of u to random values uniformly distributed in
   the range [-0.5,0.5] */
/* NOTE: direction of vector is not uniformly distributed */
/* TODO: rand() is a bad RNG */
static void rvec_rand(rvec *u) {
    for (uint i = 0; i < NDIM; i++)
        (*u)[i] = ((real) rand() / RAND_MAX) - 0.5;
}

/* dot product of u and v */
static real rvec_dot(rvec u, rvec v) {
    real r = 0;
    for (uint i = 0; i < NDIM; i++)
        r += u[i] * v[i];
    return r;
}

/* length of vector u */
static real rvec_len(rvec u) {
    return sqrt(rvec_dot(u, u));
}

/* u = v + w */
static void rvec_add(rvec *u, rvec v, rvec w) {
    for (uint i = 0; i < NDIM; i++)
        (*u)[i] = v[i] + w[i];
}

/* u = v - w */
static void rvec_sub(rvec *u, rvec v, rvec w) {
    for (uint i = 0; i < NDIM; i++)
        (*u)[i] = v[i] - w[i];
}

/* u = a * v */
static void rvec_mul(rvec *u, real a, rvec v) {
    for (uint i = 0; i < NDIM; i++)
        (*u)[i] = a * v[i];
}

/* u = v + r * w */
static void rvec_muladd(rvec *u, rvec v, real r, rvec w) {
    for (uint i = 0; i < NDIM; i++)
        (*u)[i] = v[i] + r * w[i];
}

/* calculate v - w, taking periodic boundary conditions into
   account */
static void rvec_sub_pbc(rvec *u, rvec box, rvec v, rvec w) {
    for (uint i = 0; i < NDIM; i++) {
        (*u)[i] = v[i] - w[i];
        while ((*u)[i] < -box[i]/2)
            (*u)[i] += box[i];
        while ((*u)[i] > box[i]/2)
            (*u)[i] -= box[i];
    }
}

/* print vector _without_ trailing newline */
static void rvec_puts(rvec u) {
    for (uint i = 0; i < NDIM; i++)
        printf("%10.5f ", u[i]);
}

/* print vector _with_ trailing newline */
static void rvec_print(rvec u) {
    rvec_puts(u);
    printf("\n");
}


/* functions for rvecary (an array of rvec) */

static void rvecary_set(uint n, rvec *a, real val) {
    for (uint i = 0; i < n; i++)
        rvec_set(&a[i], val);
}

/* a[i] = b[i] + r * c[i] */
static void rvecary_muladd(uint n, rvec *a, rvec *b, real r, rvec *c) {
    for (uint i = 0; i < n; i++)
        rvec_muladd(&a[i], b[i], r, c[i]);
}

/* print an array of rvec */
static void rvecary_print(rvec *a, uint n) {
    for (uint i = 0; i < n; i++)
        rvec_print(a[i]);
}


#endif /* RVEC_H */
