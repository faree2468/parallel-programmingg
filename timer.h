//
// Created by Adnan Hajro on 19. 11. 2025..
//

#ifndef TIMER_H
#define TIMER_H

void cpu_timer_start(struct timespec *tstart_cpu);
double cpu_timer_stop(struct timespec tstart_cpu);

#endif //TIMER_H