/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 05/11/2021
 * Description          : sequencer.c file. Contains the implementation of a
 *                                          generic sequencer.
 *
 * Revision No          : R000
 * Revision Date        :
 * Revision Description :
 *************************************************************************************/

#include "sequencer.h"

_Noreturn void* sequencer(void* thread_params)
{
    struct timespec requested_time_t = {0,0};
    struct timespec remain_time_t = {0, 0};
    UINT32 time_interval_counter_u32 = 0;
    int rc = 0;
    int number_of_interruption_by_a_signal_handler = 0;
    FLOAT32 remain_time_second = 0.0;

    //!< set the base interval
    requested_time_t.tv_sec = 0;
    requested_time_t.tv_nsec = SEQUENCER_BASE_PERIOD_MS * NANOSECOND_PER_MILLISECOND;

    while (1)
    {
        //!< schedule the sequence forever
        do
        {
            rc = clock_nanosleep(CLOCK_REALTIME, CLOCK_NANOSLEEP_FLAG_INTERVAL, &requested_time_t, &remain_time_t);
            remain_time_second = (FLOAT32)remain_time_t.tv_sec + (FLOAT32)(remain_time_t.tv_nsec/NANOSECOND_PER_SECOND);
            if (rc == EINTR)
            {
                //!< the sleep is interrupted by a signal handler
                number_of_interruption_by_a_signal_handler++;
            }
            else if (rc < 0)
            {
                perror("clock_nanosleep");
                exit(-1);
            }
        } while (remain_time_second > 0.0);

        time_interval_counter_u32++;

        if (time_interval_counter_u32 % 2 == 0)
        {
            //!< 50HZ loop
        }

        if (time_interval_counter_u32 % 10 == 0)
        {
            //!< 10HZ loop
            sem_post(&semaphore_service_1_t);
        }

        if (time_interval_counter_u32 % 20 == 0)
        {
            //!< 5HZ loop
            sem_post(&semaphore_service_2_t);
        }

        if (time_interval_counter_u32 % 100 == 0)
        {
            //!< 1HZ loop
            sem_post(&semaphore_service_3_t);
        }
    }
}