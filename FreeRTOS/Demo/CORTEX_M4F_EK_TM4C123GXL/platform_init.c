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
#include <stdint.h>

void serial_write (char* s);

#define WRITE32(address, value)     *((volatile uint32_t *)(address)) = value
#define READ32(address)             *((volatile uint32_t *)(address))

/* Clock Control Register Addresses.*/
#define RCC  0x400FE060
#define RCC2 0x400FE070
#define MISC 0x400FE058
#define RIS  0x400FE050

/* System Control Registers. */
#define RCGC1 0x400FE104
#define RCGC2 0x400FE108

/* UART0 Register Addresses. */
#define UART0_DR   0x4000C000
#define UART0_ECR  0x4000C004
#define UART0_IBRD 0x4000C024
#define UART0_FBRD 0x4000C028
#define UART0_LCRH 0x4000C02C
#define UART0_CTL  0x4000C030
#define UART0_IFLS 0x4000C034
#define UART0_IM   0x4000C038
#define UART0_RIS  0x4000C03C
#define UART0_ICR  0x4000C044

/* GPIO A Register Addresses. */
#define GPIOA_DATA  0x40004000
#define GPIOA_DIR   0x40004400
#define GPIOA_IS    0x40004404
#define GPIOA_IBE   0x40004408
#define GPIOA_IEV   0x4000440C
#define GPIOA_IM    0x40004410
#define GPIOA_RIS   0x40004414
#define GPIOA_MIS   0x40004418
#define GPIOA_ICR   0x4000441C
#define GPIOA_AFSEL 0x40004420
#define GPIOA_DR2R  0x40004500
#define GPIOA_DR4R  0x40004504
#define GPIOA_DR8R  0x40004508
#define GPIOA_ODR   0x4000450C
#define GPIOA_PUR   0x40004510
#define GPIOA_PDR   0x40004514
#define GPIOA_SLR   0x40004518
#define GPIOA_DEN   0x4000451C
#define GPIOA_LOCK  0x40004520
#define GPIOA_CR    0x40004524
#define GPIOA_AMSEL 0x40004528
#define GPIOA_PCTL  0x4000452C

void platform_init(void)
{
    uint32_t uTemp1,uTemp2;

#if ((configSUPPORT_PROCESSES == 1) && (configMEM_PROTECTED_PROCESSES == 1))
    /* Enable MPU and the background region for privileged memory access. */
    WRITE32(0xE000ED94, 0x5);
#endif

    /* Read RCC register. */
    uTemp1 = READ32(RCC);

    /* Clear XTAL and and MOSCDIS fields.*/
    uTemp1 &= ~((0x1F << 6));

    /* Set XTAL to 16MHz and set USESYSDIV. */
    uTemp1 |= (1 << 22) | (0x15 << 6);

    /* Read RCC2 register. */
    uTemp2 = READ32(RCC2);

    /* Clear SYSDIV2, SYSDIV2LSB and PWRDWN2*/
    uTemp2 &= ~((0x3F << 23) | (1 << 22) | (1 << 13));

    /* Enable USERCC2 field, DIV400 bit and program SYSDIV2. */
    uTemp2 |= (1 << 31) | (1 << 30) | (2 << 23);

    /*Clear PLL MIS bit. */
    WRITE32(MISC, 1 << 6);

    /* Write RCC and RCC2 register. */
    WRITE32(RCC, uTemp1);
    WRITE32(RCC2, uTemp2);

    /* Wait for PLL to Lock. */
    while (!(READ32(RIS) & (1 << 6)))
    {
    }

    /* Clear BYPASS2 bit. */
    WRITE32(RCC2, READ32(RCC2)& (~(1 << 11)));

    /* Enable GPIO clock. */
    WRITE32(RCGC2, READ32(RCGC2) | 1);

    /* Enable UART0 clock. */
    WRITE32(RCGC1, READ32(RCGC1) | 1);

    /* Configure gpio pins for UART0. */
    WRITE32 (GPIOA_DIR, (READ32(GPIOA_DIR) & (~0x3)));
    WRITE32 (GPIOA_AFSEL, (READ32(GPIOA_AFSEL) | 0x3));
    WRITE32 (GPIOA_DR2R, (READ32(GPIOA_DR2R) | 0x3));
    WRITE32 (GPIOA_DR4R, (READ32(GPIOA_DR4R) & (~0x3)));
    WRITE32 (GPIOA_DR8R, (READ32(GPIOA_DR8R) & (~0x3)));
    WRITE32 (GPIOA_SLR, (READ32(GPIOA_SLR) & (~0x3)));
    WRITE32 (GPIOA_ODR, (READ32(GPIOA_ODR) & (~0x3)));
    WRITE32 (GPIOA_PUR, (READ32(GPIOA_PUR) & (~0x3)));
    WRITE32 (GPIOA_PDR, (READ32(GPIOA_PDR) & (~0x3)));
    WRITE32 (GPIOA_DEN, (READ32(GPIOA_DEN) | 0x3));

    /* Configure UART0. */

    /* Disable UART. */
    WRITE32 (UART0_CTL, 0);

    /* Disable all Interrupt. */
    WRITE32 (UART0_IM, 0);

    /* Clear Errors. */
    WRITE32 (UART0_ECR, 0xF);

    /* Clear interrupts */
    WRITE32 (UART0_ICR, 0x00031FFF);

    /* Set bits for 8 data bits, no parity and enable FIFO. */
    WRITE32 (UART0_LCRH, 0x60 | 0x10);

    /* Setup Interrupt FIFO Level. */
    WRITE32 (UART0_IFLS, 0);

    /* Set baud rate to 115200 */
    WRITE32 (UART0_IBRD, 43);
    WRITE32 (UART0_FBRD, 26);
    WRITE32(UART0_LCRH, READ32(UART0_LCRH));

    /* Enable the UARTs RX and TX. */
    WRITE32 (UART0_CTL, 0x001 | 0x010 | 0x100 | 0x200);
}

void serial_write (char* pcString)
{
    /* Enable the transmitter */
    WRITE32 (UART0_CTL, READ32(UART0_CTL) | 0x100);

    while ((*(uint8_t*)pcString) != '\0')
    {
        /* Write the character to the data register. */
        *((volatile uint8_t *)(UART0_DR)) = *(uint8_t*)pcString;

        /* Wait for character transfer to complete. */
        while( !(READ32(UART0_RIS) & 0x20));

        /* Clear Transmit Interrupt. */
        WRITE32 (UART0_ICR, 0x20);

        pcString++;
    }
}
