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

	/* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
	not need to be guarded with a critical section. */
	#define portTICK_TYPE_IS_ATOMIC 1
#endif

#define portPOINTER_SIZE_TYPE  uint64_t


/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			( 8 )
/*-----------------------------------------------------------*/

/* Scheduler utilities. */
extern void vPortYield( void );
#define portYIELD() vPortYield()
#define portEND_SWITCHING_ISR( xSwitchRequired ) if( xSwitchRequired != pdFALSE ) portYIELD()
#define portYIELD_FROM_ISR( x ) portEND_SWITCHING_ISR( x )
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

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */



/* Licence notice ----------------------------------------------------------- */

/*
 * MIT Licence
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
