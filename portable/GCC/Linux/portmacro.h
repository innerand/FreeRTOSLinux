/*
 * FreeRTOS Port implementation for Linux using POSIX Threads
 *
 * TODO Describe usage
 *
 * Copyright (C) 2019 Andreas Innerlohninger <innerand@nxa.at>
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * This code is subject to the terms and conditions defined at the MIT Licence
 * which are specified at the end of this file and/or a separate license file.
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Signals used by the port
 *
 * If the application uses multiple threads this signals must be blocked and no
 * signal handler must be installed for them. Blocking those signals ist done by
 * vPortEarlyInit().
 */
#define portSIGTIMER    (SIGRTMIN + 1)
#define portSIGSUSPEND  (SIGRTMIN + 2)
#define portSIGRESUME   (SIGRTMIN + 3)
#define portSIGSCHEDULE (SIGRTMIN + 4)
/*-----------------------------------------------------------*/

/* Type definitions. */
#define portSTACK_TYPE	uint32_t
typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL

    #if defined(__i386__)
	/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
	not need to be guarded with a critical section. */
	#define portTICK_TYPE_IS_ATOMIC 1
    #endif
#endif

#if defined( __x86_64__)
    #define portPOINTER_SIZE_TYPE  uint64_t
#elif defined( __i386__ )
    #define portPOINTER_SIZE_TYPE  uint32_t
#else
    #error "Pointer size type unknown"
#endif

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			( 8 )
/*-----------------------------------------------------------*/

/* Early Initialization
 *
 * Must be called from the main thread before any RTOS functions are called
 *
 * @retval 0        Initialized
 * @retval (other)  Error
 */
int vPortEarlyInit( void );


#if ( configPORT_USE_REINIT != 0 )
/* Reinitialize static variables of FreeRTOS
 *
 * May be useful to get a clean state at runtime (e.g. when running multiple,
 * independent tests)
 *
 * Requires linkage with `freertos_linx.ld` and define `PRIVILEGED_DATA`
 * commented out at the end of the file mpu_wrappers.h
 */
void vPortReinitFreeRtos(void);
#define PRIVILEGED_DATA __attribute__((section ("free_rtos")))

#endif /* configPORT_USE_REINIT */


/* Scheduler utilities. */
extern void vPortYield( void );
#define portYIELD() vPortYield()
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired != pdFALSE ) portYIELD()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )

extern void vPortYieldWithinAPI( void );
#define portYIELD_WITHIN_API() vPortYieldWithinAPI()

/*
 * Pauses execution off all tasks, disables systick handling
 *
 * May be useful to call *FromISR FreeRTOS functions outside of the RTOS context
 * NOTE: If the scheduler hasn't been started, this function will block until it
 *       is.
 */
extern void vPortPause( void );

/*
 * Resumes task execution and systick handling after vPortPause()
 */
extern void vPortResume ( void );
/*-----------------------------------------------------------*/

/* Critical section management. */
#define portSET_INTERRUPT_MASK_FROM_ISR()       0
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    (void)(x)
#define portDISABLE_INTERRUPTS()
#define portENABLE_INTERRUPTS()

extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
#define portENTER_CRITICAL() vPortEnterCritical()
#define portEXIT_CRITICAL() vPortExitCritical()
/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site.  These are
not necessary for to use this port.  They are defined so the common demo files
(which build with all the ports) will build. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )
/*-----------------------------------------------------------*/

/* Task creation hook. Passes the task handle */
void vPortSetupTCB(void * pvTaskHandle);
#define portSETUP_TCB(pxTCB)    vPortSetupTCB((void*)pxTCB)
/*-----------------------------------------------------------*/

void vPortCleanUpTCB(void * pvTaskHandle);
#define portCLEAN_UP_TCB(pxTCB) vPortCleanUpTCB((void*) pxTCB)

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */



/* License notice ----------------------------------------------------------- */

/*
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
