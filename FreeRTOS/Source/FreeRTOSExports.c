/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
#include <processes.h>
#include "string.h"

#if (configSUPPORT_PROCESSES == 1)

extern void serial_write (char* pString);
EXPORT_SYMBOL(serial_write);

#if ( INCLUDE_xTaskAbortDelay == 1 )
EXPORT_SYMBOL(xTaskAbortDelay);
#endif

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
EXPORT_SYMBOL(xTaskCallApplicationTaskHook);
#endif

EXPORT_SYMBOL(xTaskCheckForTimeOut);

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
EXPORT_KERNEL_SYMBOL(xTaskCreate);
#endif

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
EXPORT_SYMBOL(xTaskCreateStatic);
#endif

#if ( INCLUDE_vTaskDelay == 1 )
EXPORT_SYMBOL(vTaskDelay);
#endif

#if ( INCLUDE_vTaskDelayUntil == 1 )
EXPORT_SYMBOL(vTaskDelayUntil);
#endif

#if ( INCLUDE_vTaskDelete == 1 )
EXPORT_SYMBOL(vTaskDelete);
#endif

#if 0 /* These are MACROS */
EXPORT_SYMBOL(taskDISABLE_INTERRUPTS);
EXPORT_SYMBOL(taskENABLE_INTERRUPTS);
EXPORT_SYMBOL(taskENTER_CRITICAL);
EXPORT_SYMBOL(taskENTER_CRITICAL_FROM_ISR);
EXPORT_SYMBOL(taskEXIT_CRITICAL);
EXPORT_SYMBOL(taskEXIT_CRITICAL_FROM_ISR);
#endif

#if ( configUSE_APPLICATION_TASK_TAG == 1 )
/* MACROS */
EXPORT_SYMBOL(xTaskGetApplicationTaskTag);
#endif

#if ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) )
EXPORT_SYMBOL(xTaskGetCurrentTaskHandle);
#endif

#if ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
EXPORT_SYMBOL(xTaskGetIdleTaskHandle);
#endif

#if ( INCLUDE_xTaskGetHandle == 1 )
EXPORT_SYMBOL(xTaskGetHandle);
#endif

EXPORT_SYMBOL(uxTaskGetNumberOfTasks);

#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
EXPORT_SYMBOL(vTaskGetRunTimeStats);
#endif

#if ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
EXPORT_SYMBOL(xTaskGetSchedulerState);
#endif

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
EXPORT_SYMBOL(uxTaskGetStackHighWaterMark);
#endif

#if( ( INCLUDE_eTaskGetState == 1 ) || ( configUSE_TRACE_FACILITY == 1 ) )
EXPORT_SYMBOL(eTaskGetState);
#endif

#if ( configUSE_TRACE_FACILITY == 1 )
EXPORT_SYMBOL(uxTaskGetSystemState);
#endif

#if( configUSE_TRACE_FACILITY == 1 )
EXPORT_SYMBOL(vTaskGetTaskInfo);
#endif

#if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
EXPORT_SYMBOL(pvTaskGetThreadLocalStoragePointer);
#endif

EXPORT_SYMBOL(pcTaskGetName);
EXPORT_SYMBOL(xTaskGetTickCount);
EXPORT_SYMBOL(xTaskGetTickCountFromISR);

#if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) )
EXPORT_SYMBOL(vTaskList);
#endif

#if( configUSE_TASK_NOTIFICATIONS == 1 )
EXPORT_SYMBOL(xTaskGenericNotify);
EXPORT_SYMBOL(xTaskGenericNotifyFromISR);
EXPORT_SYMBOL(vTaskNotifyGiveFromISR);
EXPORT_SYMBOL(xTaskNotifyStateClear);
EXPORT_SYMBOL(ulTaskNotifyTake);
EXPORT_SYMBOL(xTaskNotifyWait);
#endif

#if ( INCLUDE_uxTaskPriorityGet == 1 )
EXPORT_SYMBOL(uxTaskPriorityGet);
#endif

#if ( INCLUDE_vTaskPrioritySet == 1 )
EXPORT_SYMBOL(vTaskPrioritySet);
#endif

#if ( INCLUDE_vTaskSuspend == 1 )
EXPORT_SYMBOL(vTaskResume);
#endif

EXPORT_SYMBOL(xTaskResumeAll);

#if ( ( INCLUDE_xTaskResumeFromISR == 1 ) && ( INCLUDE_vTaskSuspend == 1 ) )
EXPORT_SYMBOL(xTaskResumeFromISR);
#endif

#ifdef configUSE_APPLICATION_TASK_TAG
#if (configUSE_APPLICATION_TASK_TAG == 1)
/* MACROS */
EXPORT_SYMBOL(vTaskSetApplicationTaskTag);
#endif
#endif

#if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
/* MACROS */
EXPORT_SYMBOL(vTaskSetThreadLocalStoragePointer);
#endif

EXPORT_SYMBOL(vTaskSetTimeOutState);
EXPORT_SYMBOL(vTaskStartScheduler);

#if ( configUSE_TICKLESS_IDLE != 0 )
EXPORT_SYMBOL(vTaskStepTick);
#endif

#if ( INCLUDE_vTaskSuspend == 1 )
EXPORT_SYMBOL(vTaskSuspend);
#endif

EXPORT_SYMBOL(vTaskSuspendAll);
#if 0 /* TODO */
/* MACROS */
EXPORT_SYMBOL(taskYIELD);
#endif

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
EXPORT_SYMBOL(xQueueGenericCreateStatic);
#endif

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
EXPORT_KERNEL_SYMBOL(xQueueGenericCreate);
#endif

EXPORT_SYMBOL(xQueueGenericSend);
EXPORT_SYMBOL(xQueueGenericSendFromISR);
EXPORT_SYMBOL(xQueueGenericReceive);
EXPORT_SYMBOL(xQueueGenericReset);

#if ( configQUEUE_REGISTRY_SIZE > 0 )
EXPORT_SYMBOL(vQueueAddToRegistry);
#endif

#if ( configUSE_QUEUE_SETS == 1 )
EXPORT_SYMBOL(xQueueAddToSet);
#endif

#if( ( configUSE_QUEUE_SETS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
EXPORT_KERNEL_SYMBOL(xQueueCreateSet);
#endif

#if ( configQUEUE_REGISTRY_SIZE > 0 )
EXPORT_SYMBOL(pcQueueGetName);
#endif

EXPORT_SYMBOL(xQueueIsQueueEmptyFromISR);
EXPORT_SYMBOL(xQueueIsQueueFullFromISR);
EXPORT_SYMBOL(uxQueueMessagesWaitingFromISR);
EXPORT_SYMBOL(xQueuePeekFromISR);

#if ( configUSE_QUEUE_SETS == 1 )
EXPORT_SYMBOL(xQueueRemoveFromSet);
EXPORT_SYMBOL(xQueueSelectFromSet);
EXPORT_SYMBOL(xQueueSelectFromSetFromISR);
#endif

EXPORT_SYMBOL(uxQueueSpacesAvailable);

#if( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
EXPORT_KERNEL_SYMBOL(xQueueCreateCountingSemaphore);
#endif

#if( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
EXPORT_SYMBOL(xQueueCreateCountingSemaphoreStatic);
#endif

#if( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
EXPORT_KERNEL_SYMBOL(xQueueCreateMutex);
#endif

#if( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
EXPORT_SYMBOL(xQueueCreateMutexStatic);
#endif

EXPORT_SYMBOL(vQueueDelete);
EXPORT_SYMBOL(uxQueueMessagesWaiting);

#if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )
EXPORT_SYMBOL(xQueueGetMutexHolder);
#endif
EXPORT_SYMBOL(xQueueGiveFromISR);

#if ( configUSE_RECURSIVE_MUTEXES == 1 )
EXPORT_SYMBOL(xQueueGiveMutexRecursive);
#endif

EXPORT_SYMBOL(xQueueReceiveFromISR);

#if ( configUSE_RECURSIVE_MUTEXES == 1 )
EXPORT_SYMBOL(xQueueTakeMutexRecursive);
#endif

#if ( configUSE_TIMERS == 1 )
EXPORT_SYMBOL(xTimerGenericCommand);

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
EXPORT_KERNEL_SYMBOL(xTimerCreate);
#endif

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
EXPORT_SYMBOL(xTimerCreateStatic);
#endif

EXPORT_SYMBOL(xTimerGetExpiryTime);
EXPORT_SYMBOL(pcTimerGetName);
EXPORT_SYMBOL(xTimerGetPeriod);
EXPORT_SYMBOL(xTimerGetTimerDaemonTaskHandle);
EXPORT_SYMBOL(pvTimerGetTimerID);
EXPORT_SYMBOL(xTimerIsTimerActive);

#if( INCLUDE_xTimerPendFunctionCall == 1 )
EXPORT_SYMBOL(xTimerPendFunctionCall);
EXPORT_SYMBOL(xTimerPendFunctionCallFromISR);
#endif

EXPORT_SYMBOL(vTimerSetTimerID);
#endif

#if  ( configUSE_TRACE_FACILITY == 1 )
#if (( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )
EXPORT_SYMBOL(xEventGroupClearBitsFromISR);
EXPORT_SYMBOL(xEventGroupSetBitsFromISR);
#endif
#endif

EXPORT_SYMBOL(xEventGroupClearBits);
EXPORT_SYMBOL(vEventGroupClearBitsCallback);

#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
EXPORT_KERNEL_SYMBOL(xEventGroupCreate);
#endif

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
EXPORT_SYMBOL(xEventGroupCreateStatic);
#endif

EXPORT_SYMBOL(vEventGroupDelete);
EXPORT_SYMBOL(xEventGroupGetBitsFromISR);
EXPORT_SYMBOL(xEventGroupSetBits);
EXPORT_SYMBOL(xEventGroupSync);
EXPORT_SYMBOL(xEventGroupWaitBits);


EXPORT_SYMBOL(uxGetProcessId);
EXPORT_SYMBOL(vGetProcessHeapAddress);

#endif
