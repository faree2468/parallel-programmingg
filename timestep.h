//
// Created by hajro on 5. 11. 2025..
//

#ifndef PARALLELCOMPUTING_TIMESTEP_H
#define PARALLELCOMPUTING_TIMESTEP_H
#define REAL_CELL 1

double timestep(int ncells, double g, double sigma, int* restrict celltype,
                double *H, double *U, double *V, double *dx, double *dy);
double timestep1(int ncells, double g, double sigma, int* restrict celltype,
                double *H, double *U, double *V, double *dx, double *dy);
double timestep2(int ncells, double g, double sigma, int* restrict celltype,
                double *H, double *U, double *V, double *dx, double *dy);
double timestep3(int ncells, double g, double sigma, int* restrict celltype,
                double *H, double *U, double *V, double *dx, double *dy);
#endif //PARALLELCOMPUTING_TIMESTEP_H