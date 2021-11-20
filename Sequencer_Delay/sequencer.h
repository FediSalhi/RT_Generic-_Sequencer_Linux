/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 05/11/2021
 * Description          : sequencer.h, add decription
 *
 * Revision No          : R000
 * Revision Date        :
 * Revision Description :
 *************************************************************************************/


#ifndef RT_GENERIC_SEQUENCER_SEQUENCER_H
#define RT_GENERIC_SEQUENCER_SEQUENCER_H

#define _GNU_SOURCE

#include "types.h"
#include  <time.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "types.h"

#define SEQUENCER_BASE_FREQUENCY_HZ    100
#define SEQUENCER_BASE_PERIOD_MS       10

#define CLOCK_NANOSLEEP_FLAG_INTERVAL   0
#define CLOCK_NANOSLEEP_FLAG_ABSOLUTE   TIMER_ABSTIME

#define NANOSECOND_PER_MILLISECOND       1000000
#define NANOSECOND_PER_SECOND            1000000000

#define MS_PER_SECOND   1000


extern sem_t semaphore_service_1_t;
extern sem_t semaphore_service_2_t;
extern sem_t semaphore_service_3_t;


_Noreturn void* sequencer(void* thread_params);




#endif //RT_GENERIC_SEQUENCER_SEQUENCER_H
