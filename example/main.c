#include <stdio.h>
#include <time.h>
#include <signal.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "main.h"


void task_one(void * par);
void task_two(void * par);
void busy_task(void * par);
void end_scheduler_task(void * par);


int main() {

    /* Start some tasks */
    if( xTaskCreate(task_one, "task1", 128, (void*) 1, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_one, "task2", 128, (void*) 2, 4, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_two, "task3", 128, (void*) 3, 3, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_two, "task4", 128, (void*) 4, 3, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_two, "task5", 128, (void*) 5, 2, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    /* That one will end the scheduler */
    if( xTaskCreate(end_scheduler_task, "task6", 128, (void*) 5, 2, NULL)
            != pdTRUE) {
         printf("Couldn't create task \n");
    }

    /* Start the scheduler */
    vTaskStartScheduler();

    if( xTaskCreate(task_two, "task1", 128, (void*) 1, 2, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    if( xTaskCreate(end_scheduler_task, "task2", 128, (void*) 2, 2, NULL)
            != pdTRUE) {
         printf("Couldn't create task \n");
    }

    /* Restart scheduler */
    vTaskStartScheduler();

    if( xTaskCreate(task_one, "task1", 128, (void*) 1, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_one, "task2", 128, (void*) 2, 4, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_two, "task3", 128, (void*) 3, 3, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    if( xTaskCreate(task_two, "task4", 128, (void*) 4, 3, NULL) != pdTRUE) {
         printf("Couldn't create task \n");
    }

    /* Once more... */
     vTaskStartScheduler();

    return 0;
}


void task_one(void * par) {
    for (;;) {
        taskENTER_CRITICAL();
        printf("task %d\n", (int64_t)par);
        taskEXIT_CRITICAL();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task_two(void * par) {
    for (;;) {
        taskENTER_CRITICAL();
        printf("task %d\n", (int64_t)par);
        taskEXIT_CRITICAL();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void busy_task(void * par) {
    unsigned int ctr = 0;
    for (;;) {
        ctr++;
        if (ctr == 0) {
            taskENTER_CRITICAL();
            printf("busy task %d\n", (int64_t)par);
            taskEXIT_CRITICAL();
        }
    }
}

void end_scheduler_task(void * par) {
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(10000));
        vTaskEndScheduler();
    }
}


const struct timespec ts = { .tv_sec = 0, .tv_nsec = 20000 };

void vApplicationIdleHook( void ) {
    /*
     * The idle task must be kept preemptive. Therefore nanosleep mustn't be put
     * inside a critical section.
     * It is safe to use nanosleep without a critical section as it gets
     * aborted by signals which are used by the Linux port to suspend the
     * task.
     */
    nanosleep(&ts, NULL);
}

