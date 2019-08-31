#include <stdio.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "main.h"

#include <time.h>

void task_one(void * par);
void task_two(void * par);
void busy_task(void * par);


int main() {

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

    vTaskStartScheduler();
    return 0;
}


void task_one(void * par) {
    for (;;) {
        printf("task %d\n", (int64_t)par);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task_two(void * par) {
    for (;;) {
        printf("task %d\n", (int64_t)par);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void busy_task(void * par) {
    unsigned int ctr = 0;
    for (;;) {
        ctr++;
        if (ctr == 0) {
            printf("busy task %d\n", (int64_t)par);
        }
    }
}

const struct timespec ts = { .tv_sec = 0, .tv_nsec = 200000 };

void vApplicationIdleHook( void ) {
    nanosleep(&ts, NULL);
}

