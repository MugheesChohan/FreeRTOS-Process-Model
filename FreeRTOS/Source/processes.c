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
#include "elf32.h"
#include <processes.h>
#include <string.h>

#if (configSUPPORT_PROCESSES == 1)

ProcessCB xProcessList[configMAX_PROCESSES];
ProcessCB *pxCurrentProcess = &xProcessList[0];

#if (configUSER_PROCESS_SUPPORT == 1)
extern uint32_t _proc_symbol_table_start;
extern uint32_t _proc_symbol_table_end;
#endif
extern uint32_t _kproc_symbol_table_start;
extern uint32_t _kproc_symbol_table_end;

static TaskHandle_t xLoadTaskHandle;
static StackType_t xLoadTaskStack[512];
StaticTask_t xLoadTaskTCB;

static StaticQueue_t xLoadQueue;
static uint8_t ucLoadQueueBuffer[sizeof(void*)];
static QueueHandle_t xLoadQueueHandle;

extern StaticTask_t *pxKernelStaticTCB;
extern void vRemoveAllTasksFromProcess(TaskHandle_t xMainTaskHandle);

typedef struct LoadParameterStruct
{
    void* pvElfFile;
    uint32_t uStackSize;
    uint32_t uHeapSize;
    uint8_t uMode;
    uint16_t* puPid;
    UBaseType_t uxPriority;
}LoadParam;

BaseType_t xLoadProcess(void* pvElfFile, uint32_t uStackSize, uint32_t uHeapSize, uint8_t uMode, uint16_t* puPid, UBaseType_t uxPriority);
BaseType_t xUnloadProcess(uint16_t uPid);
extern BaseType_t xProcessReloc(Elf32_Addr elf, Elf32_Addr proc_load_addr, Elf32_Shdr* reloc_sec, Elf32_Shdr* dynsym, Elf32_Shdr* dynstr, uint8_t uMode);

void vLoaderTask( void * pvParameters );

void vInitProcessComponent()
{
    /* Zero out the processes array. */
    memset((void*)xProcessList, 0, sizeof(ProcessCB)*configMAX_PROCESSES);

    /* Zero out the dummy kernel TCB. */
    memset((void*)pxKernelStaticTCB, 0, sizeof(StaticTask_t));

    /* Set Kernel Process main_task_handle to load_task_handle. */
    xProcessList[0].pxMainTaskHandle = (void *)pxKernelStaticTCB;

    /* Create the task without using any dynamic memory allocation. */
    xLoadTaskHandle = xTaskCreateStatic((void*)vLoaderTask, /* Function that implements the task. */
                                "LoadTask", /* Text name for the task. */
                                512, /* The number of indexes in the xStack array. */
                                0, /* Parameter passed into the task. */
                                configMAX_PRIORITIES  - 2,/* Priority at which the task is created. */
                                (void*)xLoadTaskStack, /* Array to use as the task's stack. */
                                &xLoadTaskTCB); /* Variable to hold the task's data structure. */

    xLoadQueueHandle = xQueueCreateStatic(1, sizeof(void*), ucLoadQueueBuffer,    &xLoadQueue);
}

void vLoaderTask( void * pvParameters )
{
    LoadParam *pxLoadParam;

    while (1)
    {
        if( xQueueReceive( xLoadQueueHandle, (void*)&pxLoadParam, portMAX_DELAY ) == pdPASS )
        {
            if (pxLoadParam->pvElfFile)
            {
                xLoadProcess(pxLoadParam->pvElfFile, pxLoadParam->uStackSize, pxLoadParam->uHeapSize, pxLoadParam->uMode, pxLoadParam->puPid, pxLoadParam->uxPriority);
            }
            else
            {
                xUnloadProcess(*(pxLoadParam->puPid));
            }
        }
    }
}

BaseType_t xLoad(void* pvElfFile, uint32_t uStackSize, uint32_t uHeapSize, uint8_t uMode, uint16_t* puPid, UBaseType_t uxPriority)
{
    LoadParam xLoadParam = {pvElfFile, uStackSize, uHeapSize, uMode, puPid, uxPriority};
    const void* pvParam = &xLoadParam;

    xQueueSend( xLoadQueueHandle, (const void *) &pvParam, portMAX_DELAY);

    return pdPASS;
}

BaseType_t xUnload(uint16_t uPid)
{
    LoadParam xLoadParam = {0, 0, 0, 0, &uPid, 0};
    const void* pvParam = &xLoadParam;

    xQueueSend( xLoadQueueHandle, (const void *) &pvParam, portMAX_DELAY);

    return pdPASS;
}

BaseType_t xLoadProcess(void* pvElfFile, uint32_t uStackSize, uint32_t uHeapSize, uint8_t uMode, uint16_t* puPid, UBaseType_t uxPriority)
{
    Elf32_Addr xElf = (Elf32_Addr) pvElfFile;
    Elf32_Ehdr xElfEhdr;
    Elf32_Shdr *pvElfShdrTable,*pxSection;
    char       *pcElfShdrStrTable, *pcSectionName;
    uint8_t    uSectionsCount = 0;
    Elf32_Addr xProcLoadEnd;
    Elf32_Addr       xProcLoadMem = 0, xProcLoadMemAligned = 0;

    Elf32_Shdr      *pxDynSym = 0;
    Elf32_Shdr      *pxDynStr = 0;
    Elf32_Shdr      *pxRelPlt = 0;
    Elf32_Shdr      *pxRelDyn = 0;

    Elf32_Addr      xStack;
    Elf32_Addr      xHeap;
    StaticTask_t*   pxMainTask;
    TaskHandle_t    xMainTaskHandle;
    uint16_t ui;
    uint32_t uSize;
    Elf32_Addr xEntryAddress;

    /* Validate parameters. */
    if (!(xElf && puPid))
    {
        return pdFAIL;
    }

    /* Align heap size. */
    if( ( uHeapSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
    {
        /* Byte alignment required. */
        uHeapSize += ( portBYTE_ALIGNMENT - ( uHeapSize & portBYTE_ALIGNMENT_MASK ) );
    }

    /* Align stack size. */
    if( ( uStackSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
    {
        /* Byte alignment required. */
        uStackSize += ( portBYTE_ALIGNMENT - ( uStackSize & portBYTE_ALIGNMENT_MASK ) );
    }

    /* Get the ELF Header. */
    memcpy(&xElfEhdr, (char*)xElf, sizeof(Elf32_Ehdr));

    /* Get pointer to elf section header table. */
    pvElfShdrTable = (Elf32_Shdr *) ((Elf32_Addr)xElf + xElfEhdr.e_shoff);

    /* Get the section header string table pointer. */
    pcElfShdrStrTable = xElf + (char*)((Elf32_Shdr*)((Elf32_Addr)pvElfShdrTable + (Elf32_Addr)xElfEhdr.e_shstrndx*xElfEhdr.e_shentsize))->sh_offset;

    /* Get the number of sections excluding the first NULL section. */
    uSectionsCount = xElfEhdr.e_shnum - 1;

    /* Get sections required for loading. */
    for (pxSection = (Elf32_Shdr*)((Elf32_Addr)pvElfShdrTable + xElfEhdr.e_shentsize) ; uSectionsCount > 0; pxSection = (Elf32_Shdr*)((Elf32_Addr)pxSection + xElfEhdr.e_shentsize))
    {
        pcSectionName = pcElfShdrStrTable + (pxSection->sh_name);

        if (pxSection->sh_type == SHT_DYNSYM)
        {
            pxDynSym = pxSection;
        }
        else if (pxSection->sh_type == SHT_STRTAB)
        {
            if (strcmp(pcSectionName, ".dynstr") == 0)
            {
                pxDynStr = pxSection;
            }
        }
        else if (pxSection->sh_type == SHT_REL)
        {
            if (strcmp(pcSectionName, ".rel.plt") == 0)
            {
                pxRelPlt = pxSection;
            }
            else if (strcmp(pcSectionName, ".rel.dyn") == 0)
            {
                pxRelDyn = pxSection;
            }
        }
        else if (strcmp(pcSectionName, "proc_end_mark") == 0)
        {
            xProcLoadEnd = pxSection->sh_addr;
        }

        uSectionsCount--;
    }

    /*Align proc_load_end. */
    if( ( xProcLoadEnd & portBYTE_ALIGNMENT_MASK ) != 0x00 )
    {
        /* Byte alignment required. */
        xProcLoadEnd += ( portBYTE_ALIGNMENT - ( xProcLoadEnd & portBYTE_ALIGNMENT_MASK ) );
    }

    uSize = xProcLoadEnd + uStackSize + uHeapSize;
#if (configMEM_PROTECTED_PROCESSES == 0)
    /* Allocate memory from which this process will run. */
    xProcLoadMemAligned = xProcLoadMem = (Elf32_Addr) pvPortMalloc(uSize);
#else
    if (uMode == processUSER)
    {
        /* Allocate memory from which this process will run. */
        xProcLoadMemAligned = xProcLoadMem = (Elf32_Addr) pvPortMalloc(uSize + configPAGE_SIZE);

        /* Ensure alignment on Region Size. */
        if (xProcLoadMem & (configPAGE_SIZE - 1))
        {
            xProcLoadMemAligned = xProcLoadMem + configPAGE_SIZE - ( xProcLoadMem & (configPAGE_SIZE - 1) );
        }
    }
    else
    {

        /* Allocate memory from which this process will run. */
        xProcLoadMemAligned = xProcLoadMem = (Elf32_Addr) pvPortMalloc(uSize);
    }
#endif
    if (xProcLoadMem == 0)
    {
        return pdFAIL;
    }

    /* Get the number of sections excluding the first NULL section. */
    uSectionsCount = xElfEhdr.e_shnum - 1;

    /* Load process sections into memory. */
    for (pxSection = (Elf32_Shdr*)((Elf32_Addr)pvElfShdrTable + xElfEhdr.e_shentsize) ; uSectionsCount > 0; pxSection = (Elf32_Shdr*)((Elf32_Addr)pxSection + xElfEhdr.e_shentsize))
    {
        if ((pxSection->sh_flags & SHF_ALLOC) && (pxSection->sh_size) && (xProcLoadEnd > pxSection->sh_addr))
        {
            if ((pxSection->sh_type & SHT_NOBITS) == 0)
            {
                /* Copy the section contents. */
                memcpy((void*)(xProcLoadMemAligned + pxSection->sh_addr), (void*)(xElf + pxSection->sh_offset), pxSection->sh_size);
            }
            else
            {
                /* This is the .bss section. Clear it. */
                memset((void*)(xProcLoadMemAligned + pxSection->sh_addr), 0, pxSection->sh_size);
            }
        }
        uSectionsCount--;
    }

    if (pxRelDyn)
    {
        /* Process .rel.dyn entries. */
        xProcessReloc(xElf, xProcLoadMemAligned, pxRelDyn, pxDynSym, pxDynStr, uMode);
    }

    if (pxRelPlt)
    {
        /* Process .rel.plt entries. */
        xProcessReloc(xElf, xProcLoadMemAligned, pxRelPlt, pxDynSym, pxDynStr, uMode);
    }

    xEntryAddress = xProcLoadMemAligned + xElfEhdr.e_entry;

    /* Point heap. */
    xHeap = xProcLoadMemAligned + xProcLoadEnd;

    /* Point stack. */
    xStack = xHeap + uHeapSize;

    /* Allocate space for main task handle on this heap. */
    pxMainTask = (StaticTask_t*) xHeap;

    /* Adjust heap. */
    xHeap += sizeof(StaticTask_t);

    for (ui = 1; ui < configMAX_PROCESSES; ui++)
    {
        if (xProcessList[ui].pxMainTaskHandle == 0)
        {
            xProcessList[ui].pvLoadMemAddr = (void*)xProcLoadMemAligned;
            xProcessList[ui].pvLoadMemAddrOriginal = (void*)xProcLoadMem;
            xProcessList[ui].uLoadSize = uSize;
            xProcessList[ui].pvStack = (void*)xStack;
            xProcessList[ui].pvHeap = (void*)xHeap;
            xProcessList[ui].uPid = ui;
            xProcessList[ui].uMode = uMode;
            *puPid = ui;
            break;
        }
    }

    if (ui == configMAX_PROCESSES)
    {
        vPortFree((void*)xProcLoadMem);
        return pdFAIL;
    }

    /* Create the task without using any dynamic memory allocation. */
    xMainTaskHandle = xTaskCreateStatic((void*)xEntryAddress, /* Function that implements the task. */
                                "main_task", /* Text name for the task. */
                                uStackSize>>2, /* The number of indexes in the xStack array. */
                                &xProcessList[*puPid], /* Parameter passed into the task. */
                                uxPriority,/* Priority at which the task is created. */
                                (void*)xStack, /* Array to use as the task's stack. */
                                pxMainTask); /* Variable to hold the task's data structure. */

    xProcessList[*puPid].pxMainTaskHandle = xMainTaskHandle;

    return pdPASS;
}

BaseType_t xUnloadProcess(uint16_t uPid)
{
    if (uPid < configMAX_PROCESSES)
    {
        if (xProcessList[uPid].pxMainTaskHandle)
        {
            vRemoveAllTasksFromProcess(xProcessList[uPid].pxMainTaskHandle);
            vTaskDelete(xProcessList[uPid].pxMainTaskHandle);
            vPortFree(xProcessList[uPid].pvLoadMemAddrOriginal);
            memset((void*)&xProcessList[uPid], 0, sizeof(ProcessCB));
            return pdPASS;
        }
    }

    return pdFAIL;
}

Elf32_Addr vpGetSymbolAddress(char* pcSymbolName, uint8_t uMode)
{
    SymbolEntry *pxSymTableEntry;
    Elf32_Addr xTableStart, xTableEnd;

#if (configUSER_PROCESS_SUPPORT == 1)
    if (uMode == processUSER)
    {
        xTableStart = (Elf32_Addr)&_proc_symbol_table_start;
        xTableEnd = (Elf32_Addr)&_proc_symbol_table_end;
    }
    else
#endif
    {
        xTableStart = (Elf32_Addr)&_kproc_symbol_table_start;
        xTableEnd = (Elf32_Addr)&_kproc_symbol_table_end;
    }

    for (pxSymTableEntry = (SymbolEntry*)xTableStart; (Elf32_Addr)pxSymTableEntry < xTableEnd ; pxSymTableEntry++)
    {
        if (strcmp(pxSymTableEntry->pcName, pcSymbolName) == 0)
        {
            return (Elf32_Addr) pxSymTableEntry->pvAddress;
        }
    }

    return 0;
}

uint32_t uxGetProcessId(void)
{
    return pxCurrentProcess->uPid;
}

void vGetProcessHeapAddress(void** heap, uint32_t* size)
{
    uint32_t i = uxGetProcessId();

    /* Return the starting address of heap and its size to a process. */
    *heap = xProcessList[i].pvHeap;

    /* Return the size of heap. */
    *size = (uint32_t)xProcessList[i].pvStack - (uint32_t)xProcessList[i].pvHeap;
}

#endif
