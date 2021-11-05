/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 05/11/2021
 * Description          : threads.h file. Contains the threads prototypes
 *
 * Revision No          : R000
 * Revision Date        :
 * Revision Description :
 *************************************************************************************/

#ifndef RT_GENERIC_SEQUENCER_THREADS_H
#define RT_GENERIC_SEQUENCER_THREADS_H


#include <semaphore.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"


#define SEMAPHORE_IS_SHARED_BETWEEN_THREADS     0
#define SEMAPHORE_IS_SHARED_BETWEEN_PROCESSES   1

#define SEMAPHORES_INIT_VALUE   0

#define RETURNED_CODE_SUCCESS   0
#define RETURNED_CODE_ERROR     -1

#define RT_MAX_PRIORITY     sched_get_priority_max(SCHED_FIFO)
#define RT_MIN_PRIORITY     sched_get_priority_min(SCHED_FIFO)

typedef enum RT_THREAD_CREATION_STATUS_E
{
    STATUS_RT_THREAD_INVALID_PRIORITY_VALUE = 0,
    STATUS_RT_THREAD_ATTR_SET_SCHED_POLICY_FAILED,
    STATUS_RT_THREAD_ATTR_SET_SCHED_PARAM_FAILED,
    STATUS_RT_THREAD_ATTR_SET_AFFINITY_FAILED,
    STATUS_RT_THREAD_CREATE_FAILED,
    STATUS_RT_THREAD_JOIN_FAILED,
    STATUS_RT_THREAD_CREATE_SUCCESS,

}RT_THREAD_CREATION_STATUS_E;

typedef enum jetson_nano_thread_indexes_e
{
    JETSON_CPU_CORE_0 = 0,
    JETSON_CPU_CORE_1 = 1,
    JETSON_CPU_CORE_2 = 2,
    JETSON_CPU_CORE_3 = 3,
};

/*
 * name     : initializing_semaphores
 * brief    : initializes the semaphores used by the sequencer
 * args     :
 *  number_of_threads   : number of semaphores to initialize, equals to the number
 *                        of threads
 *  ...                 : pointers to sem_t, semaphores to initialize
 * return   : void
 */
void initializing_semaphores(UINT8 number_of_threads, ...);

/*
 * name     : create_real_time_thread
 * brief    : create a thread with RT parameters
 * args     :
 *  thread_t        : pthread_t instance
 *  priority_u8     : priority of the thread. Generally 1 <= priority <= 99
 *  cpu_index_u8    : used for cpu affinity. The cpu to bind the thread with
 *  p_pthread_attr_t: used to set the thread attributes
 *  start_routine   :  thread enter point
 *  args            : thread parameters
 *  ...                 : pointers to sem_t, semaphores to initialize
 * return   : thread creation status
 */
RT_THREAD_CREATION_STATUS_E create_real_time_thread(pthread_t thread_t,
                                                    UINT8 priority_u8,
                                                    UINT8 cpu_index_u8,
                                                    pthread_attr_t* p_pthread_attr_t,
                                                    void*(*start_routine)(void*),
                                                    void* args);


#endif //RT_GENERIC_SEQUENCER_THREADS_H
