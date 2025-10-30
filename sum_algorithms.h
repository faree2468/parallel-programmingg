//
// Created by Adnan Hajro on 29. 10. 2025..
//

#ifndef SUM_ALGORITHMS_H
#define SUM_ALGORITHMS_H

/*
 * Standard double precision sum
 */
double do_sum(double* restrict var, long ncells);
double do_kahan_sum(double* restrict var, long ncells);
double do_knuth_sum(double* restrict var, long ncells);
double do_pair_sum(double* restrict var, long ncells);

#endif //SUM_ALGORITHMS_H