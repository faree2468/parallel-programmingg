//
// Created by hajro on 12. 11. 2025..
//
#include <time.h>
#ifndef PARALLELCOMPUTING_TIMER_H
#define PARALLELCOMPUTING_TIMER_H

void cpu_timer_start(struct timespec *tstart_cpu);
double cpu_timer_stop(struct timespec tstart_cpu);

#endif //PARALLELCOMPUTING_TIMER_H