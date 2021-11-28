/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 20/11/2021
 * Description          : threads.c file. Contains the threads implementation
 *                                        and some other useful functions
 *
 * Revision No          : R000
 * Revision Date        :
 * Revision Description :
 *************************************************************************************/

#define _GNU_SOURCE
#include "threads.h"


FLOAT32 compute_delta_time_ms(struct timespec* p_start_time_t, struct timespec* p_end_time_t)
{
    if (p_end_time_t->tv_nsec - p_end_time_t->tv_nsec >= 0)
    {
        return (FLOAT32)(p_end_time_t->tv_sec - p_start_time_t->tv_sec)*MS_PER_SECOND +
               (FLOAT32)(p_end_time_t->tv_nsec - p_start_time_t->tv_nsec)/NANOSECOND_PER_MILLISECOND;
    }
    else
    {
        return (FLOAT32)(p_end_time_t->tv_sec - p_start_time_t->tv_sec)*MS_PER_SECOND +
               (FLOAT32)(p_end_time_t->tv_nsec - p_start_time_t->tv_nsec + NANOSECOND_PER_SECOND)/NANOSECOND_PER_MILLISECOND;
    }
}

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

RT_THREAD_CREATION_STATUS_E create_real_time_thread(UINT8 priority_u8,
                                                    UINT8 cpu_index_u8,
                                                    void*(*start_routine)(void*),
                                                    void* args)

{
    UINT8 rt_max_priority = RT_MAX_PRIORITY;
    UINT8 rt_min_priority = RT_MIN_PRIORITY;
    UINT8 rc = 0;
    cpu_set_t thread_cpu_t;
    struct sched_param param;
    pthread_attr_t attr;

    pthread_t pthread;

    pthread_attr_init(&attr);
    rc = pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_SCHED_POLICY_FAILED;
    }

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
    rc = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_SCHED_POLICY_FAILED;
    }

    //!< set the thread priority
    rc = pthread_attr_setschedparam(&attr, &param);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_SCHED_PARAM_FAILED;
    }

    //!< set CPU affinity
    CPU_ZERO(&thread_cpu_t);
    CPU_SET(cpu_index_u8, &thread_cpu_t);
    rc = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &thread_cpu_t);
    if (rc)
    {
        return STATUS_RT_THREAD_ATTR_SET_AFFINITY_FAILED;
    }



    //!< create the thread with RT attributes and chosen cpu
    rc = pthread_create(&pthread, &attr, start_routine, NULL);
    if (rc)
    {
        return STATUS_RT_THREAD_CREATE_FAILED;
    }

    //!< join the thread
    rc = pthread_detach(pthread);
    if (rc)
    {
        return STATUS_RT_THREAD_JOIN_FAILED;
    }
}

/************************ Services ***********************/

void* service_1(void* p_service_params)
{
    struct timespec start_time_t = {0,0};
    struct timespec end_time_t = {0,0};
    FLOAT32 delta_time_ms_f32 = 0;

    UINT32  rc = 0;
    while (1)
    {
        //!< run the service forever
        rc = clock_gettime(CLOCK_REALTIME, &start_time_t);
        if (rc < 0)
        {
            perror("clock_gettime");
            exit(-1);
        }
        //!< the service should wait for its turn to be given by the sequencer
        //!< this can be a service request from a sequencer as its done here, or a signal handler or ISR (in kernel space)
        rc = sem_wait(&semaphore_service_1_t);
        if (rc < 0)
        {
            perror("semaphore wait thread 1");
            exit(-1);
        }
        //!< the semaphore is taken, start your routine

        UINT32 index = 0;
        for (index=0; index<1000; index++)
        {
            //!< waste CPU cycles..
        }

        rc = clock_gettime(CLOCK_REALTIME, &end_time_t);
        if (rc < 0)
        {
            perror("clock_gettime");
            exit(-1);
        }

        delta_time_ms_f32 = compute_delta_time_ms(&start_time_t, &end_time_t);
        printf("Service 1: Start time = %f ms , End time = %f ms , Delta time = %f ms\n",
               (FLOAT32)start_time_t.tv_sec*MS_PER_SECOND + (FLOAT32)start_time_t.tv_nsec/MS_PER_SECOND,
               (FLOAT32)end_time_t.tv_sec*MS_PER_SECOND + (FLOAT32)end_time_t.tv_nsec/MS_PER_SECOND,
               delta_time_ms_f32);
        show_thread_attributes();
    }
    pthread_exit((void*)0);
}

void* service_2(void* p_service_params)
{

    struct timespec start_time_t = {0,0};
    struct timespec end_time_t = {0,0};
    FLOAT32 delta_time_ms_f32 = 0;

    UINT32  rc = 0;
    while (1)
    {
        //!< run the service forever
        //!< the service should wait for its turn to be given by the sequencer
        rc = clock_gettime(CLOCK_REALTIME, &start_time_t);
        if (rc < 0)
        {
            perror("clock_gettime");
            exit(-1);
        }
        rc = sem_wait(&semaphore_service_2_t);
        if (rc < 0)
        {
            perror("semaphore wait thread 1");
            exit(-1);
        }
        //!< the semaphore is taken, start your routine

        UINT32 index = 0;
        for (index=0; index<1000; index++)
        {
            //!< waste CPU cycles..
        }

        rc = clock_gettime(CLOCK_REALTIME, &end_time_t);
        if (rc < 0)
        {
            perror("clock_gettime");
            exit(-1);
        }

        delta_time_ms_f32 = compute_delta_time_ms(&start_time_t, &end_time_t);
        printf("Service 2: Start time = %f ms , End time = %f ms , Delta time = %f ms\n",
               (FLOAT32)start_time_t.tv_sec*MS_PER_SECOND + (FLOAT32)start_time_t.tv_nsec/MS_PER_SECOND,
               (FLOAT32)end_time_t.tv_sec*MS_PER_SECOND + (FLOAT32)end_time_t.tv_nsec/MS_PER_SECOND,
               delta_time_ms_f32);
        //show_thread_attributes();
    }
    pthread_exit((void*)0);
}

void* service_3(void* p_service_params)
{

    struct timespec start_time_t = {0,0};
    struct timespec end_time_t = {0,0};
    FLOAT32 delta_time_ms_f32 = 0;

    UINT32  rc = 0;
    while (1)
    {
        //!< run the service forever
        //!< the service should wait for its turn to be given by the sequencer
        rc = clock_gettime(CLOCK_REALTIME, &start_time_t);
        if (rc < 0)
        {
            perror("clock_gettime");
            exit(-1);
        }
        rc = sem_wait(&semaphore_service_3_t);
        if (rc < 0)
        {
            perror("semaphore wait thread 1");
            exit(-1);
        }
        //!< the semaphore is taken, start your routine


        UINT32 index = 0;
        for (index=0; index<1000; index++)
        {
            //!< waste CPU cycles..
        }

        rc = clock_gettime(CLOCK_REALTIME, &end_time_t);
        if (rc < 0)
        {
            perror("clock_gettime");
            exit(-1);
        }

        delta_time_ms_f32 = compute_delta_time_ms(&start_time_t, &end_time_t);
        printf("Service 3: Start time = %f ms , End time = %f ms , Delta time = %f ms\n",
               (FLOAT32)start_time_t.tv_sec*MS_PER_SECOND + (FLOAT32)start_time_t.tv_nsec/MS_PER_SECOND,
               (FLOAT32)end_time_t.tv_sec*MS_PER_SECOND + (FLOAT32)end_time_t.tv_nsec/MS_PER_SECOND,
               delta_time_ms_f32);
    }
    pthread_exit((void*)0);
}

void show_thread_attributes()
{
    pthread_attr_t attr;
    struct sched_param param;
    int rc = 0;
    int policy;

    pthread_attr_init(&attr);

    rc = pthread_getattr_np(pthread_self(), &attr);
    if (rc)
    {
        perror("pthread_getattr_np");
        exit(-1);
    }
    rc = pthread_attr_getschedpolicy(&attr, &policy);
    if (rc)
    {
        perror("pthread_attr_getschedpolicy");
        exit(-1);
    }
    rc = pthread_attr_getschedparam(&attr, &param);
    if (rc)
    {
        perror("pthread_attr_getschedpolicy");
        exit(-1);
    }
    if (policy == SCHED_FIFO)
    {
        printf("Policy = SCHED_FIFO | Priority = %d\n", param.sched_priority);
    }
}
