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

#include <stdint.h>

#define WRITE32(address, value)     *((volatile uint32_t *)(address)) = value
#define READ32(address)             *((volatile uint32_t *)(address))

extern int main(void);
void vResetHandler(void);
static void vNMIHandler(void);
static void vHardFaultHandler(void);
static void vMPUFaultHandler(void);
static void vBusFaultHandler(void);
static void vUsageFaultHandler(void);
static void vDebugMonitorHandler(void);
static void vInterruptHandler(void);
extern void xPortPendSVHandler(void);
extern void vPortSVCHandler(void);
extern void xPortSysTickHandler(void);

extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

static uint32_t uStack[1024];

__attribute__ ((section(".vectortable"))) void (* const VectorTable[])(void) =
{
    (void (*)(void))((uint32_t)uStack + sizeof(uStack)),  /* System Stack pointer. */
    vResetHandler,
    vNMIHandler,
    vHardFaultHandler,
    vMPUFaultHandler,
    vBusFaultHandler,
    vUsageFaultHandler,
    0,
    0,
    0,
    0,
    vPortSVCHandler,
    vDebugMonitorHandler,
    0,
    xPortPendSVHandler,
    xPortSysTickHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler,
    vInterruptHandler
};

void vResetHandler(void)
{
    uint32_t *puDataLoad, *puData;

    /* Initialize the data section. */
    puDataLoad = &__data_load__;
    for(puData = &__data_start__; puData < &__data_end__; )
    {
        *puData++ = *puDataLoad++;
    }

    /* Zero out the bss section. */
    __asm("    mov     r0, #0\n"
          "    ldr     r1, =__bss_start__\n"
          "    ldr     r2, =__bss_end__\n"
          "    .thumb_func\n"
          "loop:\n"
          "    cmp     r1, r2\n"
          "    it      lt\n"
          "    strlt   r0, [r1], #4\n"
          "    blt     loop");
    
    /* Enable the Floating Point Unit. */
    WRITE32(0xE000ED88, (READ32(0xE000ED88) & (~0x00F00000)) | 0x00F00000);

    /* Call the main() function. */
    main();
}

static void vNMIHandler(void)
{
    while(1)
    {
    }
}

static void vHardFaultHandler(void)
{
    while(1)
    {
    }
}

static void vMPUFaultHandler(void)
{
    while(1)
    {
    }
}

static void vBusFaultHandler(void)
{
    while(1)
    {
    }
}

static void vUsageFaultHandler(void)

{
    while(1)
    {
    }
}

static void vDebugMonitorHandler(void)
{
    while(1)
    {
    }
}

static void vInterruptHandler(void)
{
    while(1)
    {
    }
}
