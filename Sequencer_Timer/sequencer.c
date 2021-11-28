/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 20/11/2021
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

    timer_t sequencer_base_timer_t;
    struct itimerspec new_value_t;
    struct itimerspec old_value_t;

    //!< creating a timer
    timer_create(CLOCK_REALTIME, NULL, &sequencer_base_timer_t);

    //!< setting the signal handler
    signal(SIGALRM, (void(*)()) sequencer_10ms_signal_handler);

    //!< arming the timer
    new_value_t.it_value.tv_sec     = TIMER_INITIAL_VALUE_S;
    new_value_t.it_value.tv_nsec    = TIMER_INITIAL_VALUE_NS;
    new_value_t.it_interval.tv_sec  = TIMER_INTERVAL_S;
    new_value_t.it_interval.tv_nsec = TIMER_INTERVAL_NS;
    timer_settime(sequencer_base_timer_t, 0, &new_value_t, &old_value_t);

    for (;;)
        pause();

}

void sequencer_10ms_signal_handler(int id)
{
    static UINT32 sequencer_counter_u32 = 0;
    sequencer_counter_u32 ++;

    if (sequencer_counter_u32 % 2 == 0)
    {
        //!< 50HZ loop
    }

    if (sequencer_counter_u32 % 10 == 0)
    {
        //!< 10HZ loop
        sem_post(&semaphore_service_1_t);
    }

    if (sequencer_counter_u32 % 20 == 0)
    {
        //!< 5HZ loop
        sem_post(&semaphore_service_2_t);
    }

    if (sequencer_counter_u32 % 100 == 0)
    {
        //!< 1HZ loop
        sem_post(&semaphore_service_3_t);
    }
}