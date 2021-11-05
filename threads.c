/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 05/11/2021
 * Description          : threads.c file. Contains the threads implementation
 *
 * Revision No          : R000
 * Revision Date        :
 * Revision Description :
 *************************************************************************************/

#define _GNU_SOURCE
#include "threads.h"

void initializing_semaphores(UINT8 number_of_threads, ...)
{
    va_list ap;
    va_start(ap, number_of_threads);

    sem_t* sem_to_initialize_t;
    int rc = - 1;

    //!< initialize the sem_t variables
    for (int i=0; i<number_of_threads; i++) {
        sem_to_initialize_t = va_arg(ap, sem_t*);
        rc = sem_init(sem_to_initialize_t, SEMAPHORE_IS_SHARED_BETWEEN_THREADS, SEMAPHORES_INIT_VALUE);
        if (rc == RETURNED_CODE_ERROR) {
            perror("init thread ");
            exit(-1);
        }
    }
}

RT_THREAD_CREATION_STATUS_E create_real_time_thread(pthread_t thread_t,
                                                 UINT8 priority_u8,
                                                 UINT8 cpu_index_u8,
                                                 pthread_attr_t* p_pthread_attr_t,
                                                 void*(*start_routine)(void*),
                                                 void* args)
{
    UINT8 rt_max_priority = RT_MAX_PRIORITY;
    UINT8 rt_min_priority = RT_MIN_PRIORITY;
    UINT8 rc = 0;
    cpu_set_t thread_cpu_t;
    struct sched_param param;

    if ((priority_u8 >= RT_MIN_PRIORITY) || (priority_u8 <= RT_MAX_PRIORITY))
    {
        param.sched_priority = priority_u8;
    }
    else
    {
        printf("Check the priority value:\nPriority max value = %d\nPriority min value = %d",
               rt_max_priority, rt_min_priority);
        return STATUS_RT_THREAD_INVALID_PRIORITY_VALUE;
    }

    //!< set the scheduling policy FIFO
    rc = pthread_attr_setschedpolicy(p_pthread_attr_t, SCHED_FIFO);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_SCHED_POLICY_FAILED;
    }

    //!< set the thread priority
    rc = pthread_attr_setschedparam(p_pthread_attr_t, &param);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_SCHED_PARAM_FAILED;
    }

    //!< set CPU affinity
    CPU_ZERO(&thread_cpu_t);
    CPU_SET(cpu_index_u8, &thread_cpu_t);
    rc = pthread_attr_setaffinity_np(p_pthread_attr_t, sizeof(cpu_set_t), &thread_cpu_t);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_AFFINITY_FAILED;
    }
    //!< create the thread with RT attributes and chosen cpu
    rc = pthread_create(&thread_t, p_pthread_attr_t, start_routine, args);
    if (rc)
    {
        return STATUS_RT_THREAD_CREATE_FAILED;
    }

    //!< join the thread
    rc = pthread_join(thread_t, NULL);
    if (rc)
    {
        return STATUS_RT_THREAD_JOIN_FAILED;
    }
}

