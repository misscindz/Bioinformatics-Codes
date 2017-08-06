#include "md.h"

/* TODO: natom_per_dim should be dependent on box dimensions,
   i.e. have a vector of natom_per_dim */
void setup_fcc_lattice(rvec *x, uint natom, rvec box) {
    uint i, j;
    uint natom_per_dim = ceil(pow(natom, 1.0/NDIM));
    rvec r;
    uint c[NDIM];
    for (i = 0; i < NDIM; i++)
        c[i] = 0;
    rvec_mul(&r, 1.0 / natom_per_dim, box);
    for (i = 0; i < natom; i++) {
        for (j = 0; j < NDIM; j++)
            x[i][j] = c[j] * r[j];
        c[0] += 1;
        for (j = 0; j < NDIM - 1; j++) {
            if (c[j] == natom_per_dim) {
                c[j] = 0;
                c[j+1] += 1;
            }
        }
    }
}

void apply_pbc(uint natom, rvec box, rvec *x) {
    for (uint i = 0; i < natom; i++) {
        for (uint d = 0; d < NDIM; d++) {
            while (x[i][d] < 0)
                x[i][d] += box[d];
            while (x[i][d] > box[d])
                x[i][d] -= box[d];
        }
    }
}

void remove_com_velocity(uint natom, rvec *v, real *m) {
    rvec vsum;
    real msum = 0;
    rvec_set(&vsum, 0);
    for (uint i = 0; i < natom; i++) {
        msum += m[i];
        rvec_add(&vsum, vsum, v[i]);
    }
    for (uint i = 0; i < natom; i++)
        rvec_muladd(&v[i], v[i], - m[i]/msum, vsum);
}

real com_velocity(uint natom, rvec *v, real *m) {
    rvec vsum;
    real msum = 0;
    rvec_set(&vsum, 0);
    for (uint i = 0; i < natom; i++)
        msum += m[i];
    for (uint i = 0; i < natom; i++)
        rvec_muladd(&vsum, vsum, m[i]/msum, v[i]);
    return rvec_len(vsum);
}

void rescale_temperature(uint natom, rvec *v, real *m, uint ndof, real kB, real Tnew) {
    real Ekin = calc_kinetic_energy(natom, v, m); 
    real Told = calc_temperature(ndof, Ekin, kB);
    real vscale;
    if (Told == 0.0) {
        printf("error: couldn't rescale temperature, old temperature is zero (Told = 0)\n");
        exit(EXIT_FAILURE);
    }
    vscale = sqrt(Tnew / Told);
    for (uint i = 0; i < natom; i++)
        rvec_mul(&v[i], vscale, v[i]);
}

void init_velocities(uint natom, rvec *v, real *m, real ndof, real kB, real Tnew) {
    for (uint i = 0; i < natom; i++)
        rvec_rand(&v[i]);
    remove_com_velocity(natom, v, m);
    rescale_temperature(natom, v, m, ndof, kB, Tnew);
}
