//
// Created by Adnan Hajro on 29. 10. 2025..
//

#include <stdlib.h>
#include <math.h>
#include "sum_algorithms.h"

double do_sum(double* restrict var, long ncells)
{
   double sum = 0.0;
   for (long i = 0; i < ncells; i++){
      sum += var[i];
   }
   return sum;
}

double do_kahan_sum(double* restrict var, long ncells)
{
   struct esum_type{
      double sum;
      double correction;
   } local;
   local.sum = 0.0;
   local.correction = 0.0;

   for (long i = 0; i < ncells; i++) {
      double corrected_next_term= var[i] + local.correction;
      double new_sum            = local.sum + local.correction;
      local.correction   = corrected_next_term - (new_sum - local.sum);
      local.sum          = new_sum;
   }

   double sum = local.sum + local.correction;
   return(sum);
}

double do_knuth_sum(double* restrict var, long ncells)
{
   struct esum_type{
      double sum;
      double correction;
   };

   double u, v, upt, up, vpp;
   struct esum_type local;

   local.sum = 0.0;
   local.correction = 0.0;
   for (long i = 0; i < ncells; i++) {
      u = local.sum;
      v = var[i] + local.correction;
      upt = u + v;
      up = upt - v;
      vpp = upt - up;
      local.sum = upt;
      local.correction = (u - up) + (v - vpp);
   }

   double sum = local.sum + local.correction;
   return(sum);
}

double do_pair_sum(double* restrict var, long ncells)
{
   // Pair-wise sum
   double *pwsum = (double *)malloc(ncells/2*sizeof(double));

   long nmax = ncells/2;
   for (long i = 0; i<nmax; i++){
      pwsum[i] = var[i*2]+var[i*2+1];
   }

   for (long j = 1; j<log2(ncells); j++){
      nmax /= 2;
      for (long i = 0; i<nmax; i++){
         pwsum[i] = pwsum[i*2]+pwsum[i*2+1];
      }
   }
   double sum = pwsum[0];

   free(pwsum);

   return(sum);
}