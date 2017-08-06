#ifndef MD_H
#define MD_H

#include "rvec.h"

void setup_fcc_lattice(rvec *x, uint natom, rvec box);
void apply_pbc(uint natom, rvec box, rvec *x);
void remove_com_velocity(uint natom, rvec *v, real *m);
real com_velocity(uint natom, rvec *v, real *m);
void rescale_temperature(uint natom, rvec *v, real *m, uint ndof, real kB, real Tnew);
void init_velocities(uint natom, rvec *v, real *m, real ndof, real kB, real Tnew);

/* these are the functions that have to be implemented */
real calc_energy_force(uint natom, rvec box, rvec *x, rvec *f);
real calc_temperature(uint ndof, real Ekin, real kB);
real calc_kinetic_energy(uint natom, rvec *v, real *m);
real leapfrog_step(uint natom, rvec *x, rvec *v, rvec *f, real *m, real dt, rvec box);


#endif /* MD_H */
