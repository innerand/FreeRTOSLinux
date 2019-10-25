#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "main.h"

static void task_one(void * par);
static void task_two(void * par);
static void busy_task(void * par);
static void end_scheduler_task(void * par);
static int sigsave_sleep(double seconds);

static uintptr_t test_var;

static void * rtos_main_thread(void *arg) {
    /* Start the scheduler */
    vTaskStartScheduler();

    pthread_exit(11);
}

static void test_task(void * par) {
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(50));
        test_var = (uintptr_t*) par;
    }
}

/*
 * Test if a thread can be created from a different thread
 * then the RTOS main thread before the scheduler has started
 */
static int test_external_task_creation(void) {
    pthread_t thread_id;

    test_var = 0;

    if(xTaskCreate(test_task, "t1", 128, (void*) 41, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    pthread_create(&thread_id, NULL, rtos_main_thread, NULL);

    sigsave_sleep(0.5);

    vPortPause();
    vTaskEndScheduler();
    vPortResume();

    uintptr_t retval;
    pthread_join(thread_id, (void**)&retval);

    if (retval != 11) {
        return 0;
    }

    return (test_var == 41);
}

/*
 * Test if the scheduler can be ended from outside the RTOS context end then
 * started again.
 */
static int test_stop_scheduler(void) {
    pthread_t thread_id;

    test_var = 0;

    if( xTaskCreate(test_task, "t2", 128, (void*) 42, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    if( xTaskCreate(test_task, "t3", 128, (void*) 43, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    pthread_create(&thread_id, NULL, rtos_main_thread, NULL);

    vPortPause();
    if( xTaskCreate(test_task, "t4", 128, (void*) 44, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }
    vPortResume();

    sigsave_sleep(0.7);

    vPortPause();
    vTaskEndScheduler();
    vPortResume();

    uintptr_t retval;
    pthread_join(thread_id, (void**)&retval);

    if (retval != 11) {
        return 0;
    }

    return test_external_task_creation();
}

/*
 * Test if the scheduler can be ended from RTOS context end then started again.
 */
static int test_stop_scheduler_internal(void) {
    pthread_t thread_id;

    test_var = 0;

    if( xTaskCreate(test_task, "t2", 128, (void*) 42, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    if( xTaskCreate(test_task, "t3", 128, (void*) 43, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }

    if( xTaskCreate(end_scheduler_task, "end_task", 128, (void*) 2, 2, NULL)
            != pdTRUE) {
         printf("Couldn't create task \n");
    }

    pthread_create(&thread_id, NULL, rtos_main_thread, NULL);

    vPortPause();
    if( xTaskCreate(test_task, "t4", 128, (void*) 44, 3, NULL) != pdTRUE) {
        printf("Couldn't create task \n");
    }
    vPortResume();

    uintptr_t retval;
    pthread_join(thread_id, (void**)&retval);

    if (retval != 11) {
        return 0;
    }

    return test_external_task_creation();
}

/* TODO: Move tests to own test suite and add actual example */
int main() {
    vPortEarlyInit();

    if (!test_external_task_creation()) {
        printf("External task creation FAILED.\n");
    } else {
        printf("External task creation OK.\n");
    }

    if (!test_stop_scheduler()) {
        printf("Stop scheduler FAILED.\n");
    } else {
        printf("Stop scheduler OK.\n");
    }

    if (!test_stop_scheduler_internal()) {
        printf("Stop scheduler internal FAILED.\n");
    } else {
        printf("Stop scheduler internal OK.\n");
    }

    return 0;

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
        vTaskDelay(pdMS_TO_TICKS(1000));
        vTaskEndScheduler();
    }
}


const struct timespec ts = { .tv_sec = 0, .tv_nsec = 200000 };

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

static int sigsave_sleep(double seconds) {
    struct timespec ts;
    ts.tv_sec =  (time_t) seconds;
    ts.tv_nsec = (long) ((seconds - ts.tv_sec) * 1e+9);

    while (1) {
        int ret = nanosleep (&ts, &ts);
        if (ret == 0)
            return 0;
        else if (errno == EINTR)
            continue;
        else
            return ret;
    }

    return 0;
}
