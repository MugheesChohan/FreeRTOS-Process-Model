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

#include "stdint.h"
#include "elf32.h"
#include "task.h"

#define processUSER 1
#define processSUPERVISOR 0

typedef struct SYM_ENTRY_STRUCT
{
    const char*    pcName;
    void*          pvAddress;
} SymbolEntry;

typedef struct PROCESS_CONTROL_BLOCK
{
    uint32_t        uPid;
    void*           pvLoadMemAddr;
    void*           pvLoadMemAddrOriginal;
    uint32_t        uLoadSize;
    void*           pvStack;
    void*           pvHeap;
    TaskHandle_t    pxMainTaskHandle;
    uint8_t         uMode;
}ProcessCB;

#define FREERTOS_PROCESS    int main(void);                                                       \
                            void main_task( void * pvParameters ) __attribute__ ((noreturn));     \
                            void main_task( void * pvParameters )                                 \
                            {                                                                     \
                                main();                                                           \
                                                                                                  \
                                /* Suspend this task, since main has exited.*/                    \
                                vTaskSuspend(xTaskGetCurrentTaskHandle());                        \
                            }

#define EXPORT_KERNEL_SYMBOL(symbol)                                                                                        \
                                                                                                                            \
/* Put string name of the symbol into a string table */                                                                     \
static const char __kstr_##symbol[]    __attribute__((section("kprocsymstr"), aligned(1))) = #symbol;                          \
                                                                                                                            \
/* Put symbol entry into symbol table */                                                                                    \
static const SymbolEntry __ksym_##symbol __attribute__((section("kprocsymtab"), used)) = {__kstr_##symbol, (void *)&(symbol)}

#if (configUSER_PROCESS_SUPPORT == 1)

#define EXPORT_USER_SYMBOL(symbol)                                                                                         \
                                                                                                                           \
/* Put string name of the symbol into a string table */                                                                    \
static const char __str_##symbol[]  __attribute__((section("procsymstr"), aligned(1))) = #symbol;                          \
                                                                                                                           \
/* Wrap original function in syscall code. */                                                                              \
portSYSCALL_ENTER(symbol)                                                                                                  \
                                                                                                                           \
/* Put symbol entry into symbol table */                                                                                   \
static const SymbolEntry __sym_##symbol __attribute__((section("procsymtab"), used)) = {__str_##symbol, (void *)&(__syscall_##symbol) }

#define EXPORT_SYMBOL(symbol)          EXPORT_USER_SYMBOL(symbol); \
                                    EXPORT_KERNEL_SYMBOL(symbol)

/* Export data symbols for kernel mode processes only. */
#define EXPORT_DATA_SYMBOL(symbol)  EXPORT_KERNEL_SYMBOL(symbol)

#else
#define EXPORT_USER_SYMBOL(symbol)    EXPORT_KERNEL_SYMBOL(symbol)
#define EXPORT_SYMBOL(symbol)    EXPORT_KERNEL_SYMBOL(symbol)
#endif

Elf32_Addr vpGetSymbolAddress(char* pcSymbolName, uint8_t uMode);
BaseType_t xLoad(void* pvElfFile, uint32_t uStackSize, uint32_t uHeapSize, uint8_t uMode, uint16_t* puPid, UBaseType_t uxPriority);
BaseType_t xUnload(uint16_t uPid);
void vInitProcessComponent();
uint32_t uxGetProcessId(void);
void vGetProcessHeapAddress(void** heap, uint32_t* size);
void vSwitchProcess();
#if (configMEM_PROTECTED_PROCESSES == 1)
void vUpdateMPU(ProcessCB* pxProcess);
#endif

void vSysCallReturn(void) __attribute__((naked));
