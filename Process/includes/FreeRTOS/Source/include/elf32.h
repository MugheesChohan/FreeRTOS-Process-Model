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

#ifndef ELF32_H_
#define ELF32_H_

typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;

#define EI_NIDENT 16

/* ELF Header. */
typedef struct
{
    unsigned char           e_ident[EI_NIDENT];
    Elf32_Half              e_type;
    Elf32_Half              e_machine;
    Elf32_Word              e_version;
    Elf32_Addr              e_entry;
    Elf32_Off               e_phoff;
    Elf32_Off               e_shoff;
    Elf32_Word              e_flags;
    Elf32_Half              e_ehsize;
    Elf32_Half              e_phentsize;
    Elf32_Half              e_phnum;
    Elf32_Half              e_shentsize;
    Elf32_Half              e_shnum;
    Elf32_Half              e_shstrndx;

} Elf32_Ehdr;

/* e_type values.  */
#define     ET_NONE         0
#define     ET_REL          1
#define     ET_EXEC         2
#define     ET_DYN          3
#define     ET_CORE         4
#define     ET_LOOS         0xfe00
#define     ET_HIOS         0xfeff
#define     ET_LOPROC       0xff00
#define     ET_HIPROC       0xffff

/* e_machine values. */
#define     EM_ARM          40

/* ELF section header. */
typedef struct
{
    Elf32_Word              sh_name;
    Elf32_Word              sh_type;
    Elf32_Word              sh_flags;
    Elf32_Addr              sh_addr;
    Elf32_Off               sh_offset;
    Elf32_Word              sh_size;
    Elf32_Word              sh_link;
    Elf32_Word              sh_info;
    Elf32_Word              sh_addralign;
    Elf32_Word              sh_entsize;

} Elf32_Shdr;

/* sh_type values. */
#define     SHT_NULL                0
#define     SHT_PROGBITS            1
#define     SHT_SYMTAB              2
#define     SHT_STRTAB              3
#define     SHT_RELA                4
#define     SHT_HASH                5
#define     SHT_DYNAMIC             6
#define     SHT_NOTE                7
#define     SHT_NOBITS              8
#define     SHT_REL                 9
#define     SHT_SHLIB               10
#define     SHT_DYNSYM              11
#define     SHT_INIT_ARRAY          14
#define     SHT_FINI_ARRAY          15
#define     SHT_PREINIT_ARRAY       16
#define     SHT_GROUP               17
#define     SHT_SYMTAB_SHNDX        18
#define     SHT_LOOS                0x60000000
#define     SHT_HIOS                0x6fffffff
#define     SHT_LOPROC              0x70000000
#define     SHT_HIPROC              0x7fffffff
#define     SHT_LOUSER              0x80000000
#define     SHT_HIUSER              0xffffffff

/* sh_flags values */
#define     SHF_WRITE       0x1U
#define     SHF_ALLOC       0x2U
#define     SHF_EXECINSTR   0x4U
#define     SHF_MASKPROC    0xf0000000U

/* Symbol table entry */
typedef struct
{
    Elf32_Word              st_name;
    Elf32_Addr              st_value;
    Elf32_Word              st_size;
    unsigned char           st_info;
    unsigned char           st_other;
    Elf32_Half              st_shndx;

} Elf32_Sym;

#define ELF32_ST_BIND(i)   ((i) >> 4)
#define ELF32_ST_SYM(i)	   ((i) & 0xf)
#define ELF32_ST_INFO(b,t) (((b) << 4) + ((t) & 0xf))

/* Relocation entry. */
typedef struct
{
    Elf32_Addr              r_offset;
    Elf32_Word              r_info;

} Elf32_Rel;

#define     ELF32_R_SYM(i)  ((i) >> 8)
#define     ELF32_R_TYPE(i) ((unsigned char)(i))
#define     ELF32_R_INFO(s,t) (((s) << 8) + (unsigned char) (t))

#endif /* ELF32_H_ */
