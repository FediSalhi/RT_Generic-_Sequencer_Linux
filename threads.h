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
#include "sequencer.h"


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


extern sem_t semaphore_service_1_t;
extern sem_t semaphore_service_2_t;
extern sem_t semaphore_service_3_t;
extern sem_t semaphore_sequencer_t;



/*
 * name     : compute_delta_time_ms
 * brief    : computes the time in ms between two moments described by two
 *            'struct timespec' data structures.
 * args     :
 *  p_start_time_t   : 1st moment
 *  p_end_time_t     : 2nd moment
 *
 * return   : delta time in ms
 */
FLOAT32 compute_delta_time_ms(struct timespec* p_start_time_t, struct timespec* p_end_time_t);

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
RT_THREAD_CREATION_STATUS_E create_real_time_thread(UINT8 priority_u8,
                                                    UINT8 cpu_index_u8,
                                                    void*(*start_routine)(void*),
                                                    void* args);


/*
 * name     : service_1
 * brief    : example service
 * args     :
 *  p_service_params   : service params
 *
 * return   : void
 */
void* service_1(void* p_service_params);

/*
 * name     : service_2
 * brief    : example service
 * args     :
 *  p_service_params   : service params
 *
 * return   : void
 */
void* service_2(void* p_service_params);

/*
 * name     : service_3
 * brief    : example service
 * args     :
 *  p_service_params   : service params
 *
 * return   : void
 */
void* service_3(void* p_service_params);

/*
 * name     : show_thread_attributes
 * brief    : prints the scheduling policy
 *            and priority to the std output
 * args     : void
 *
 * return   : void
 */
void show_thread_attributes();
#endif //RT_GENERIC_SEQUENCER_THREADS_H
