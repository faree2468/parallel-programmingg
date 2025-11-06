//
// Created by hajro on 4. 11. 2025..
//

#ifndef PARALLELCOMPUTING_TIMER_H
#define PARALLELCOMPUTING_TIMER_H
#include <time.h>

void cpu_timer_start(struct timespec *tstart_cpu);
double cpu_timer_stop(struct timespec tstart_cpu);

#endif