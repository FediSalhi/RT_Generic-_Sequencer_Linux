/************************************************************************************
 * Author               : Fedi Salhi <fadi.salhi@outlook.fr>
 * Creation Date        : 05/11/2021
 * Description          : main.c file, used for testing the sequencer
 *
 * Revision No          : R000
 * Revision Date        :
 * Revision Description :
 *************************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include "sequencer.h"
#include "threads.h"
#include <unistd.h>

#define NUMBER_OF_TEST_THREADS  3

sem_t semaphore_service_1_t = {0};
sem_t semaphore_service_2_t = {0};
sem_t semaphore_service_3_t = {0};
sem_t semaphore_sequencer_t = {0};

//TODO: replace all printf's with syslog --> better overhead
//printf has a high function call overhead

int main() {

    //!< init semaphores
    initializing_semaphores(NUMBER_OF_TEST_THREADS,
                            &semaphore_service_1_t,
                            &semaphore_service_2_t,
                            &semaphore_service_3_t);



    create_real_time_thread(
                            95,
                            3,
                            sequencer,
                            NULL);

    create_real_time_thread(
                            90,
                            0,
                            service_1,
                            NULL);

    create_real_time_thread(
                            85,
                            0,
                            service_2,
                            NULL);

    create_real_time_thread(
                            80,
                            0,
                            service_3,
                            NULL);
    while (1)
    {
        usleep(20);
    }
    printf("Hello, World!\n");
    return 0;
}
