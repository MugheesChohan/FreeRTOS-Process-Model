ENTRY(vResetHandler)
MEMORY
{
    FLASH (RX) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    SRAM (WX)  : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS {

    PROVIDE (_vectors_base_address = 0x0);

    .vectortable (_vectors_base_address) : AT (_vectors_base_address) {
        KEEP (*(.vectortable))
    } > FLASH

    PROVIDE (_vtable_base_address = 0x20000000);

    .vtable (_vtable_base_address) : AT (_vtable_base_address) {
        KEEP (*(.vtable))
    } > SRAM

    .text : {
        _text_start = . ;
        CREATE_OBJECT_SYMBOLS
        *(.text)
        *(.text.*)
        . = ALIGN(0x4);
        KEEP (*(.ctors))
        . = ALIGN(0x4);
        KEEP (*(.dtors))
        . = ALIGN(0x4);
        __init_array_start = .;
        KEEP (*(.init_array*))
        __init_array_end = .;
        *(.init)
        *(.fini*)
        _text_end = . ;
    } > FLASH

    PROVIDE (__etext = .);
    PROVIDE (_etext = .);
    PROVIDE (etext = .);

    .rodata : {
        *(.rodata)
        *(.rodata*)
    } > FLASH

    .data : ALIGN (4) {
        __data_load__ = LOADADDR (.data);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN (4);
        __data_end__ = .;
    } > SRAM AT> FLASH

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH

    .ARM.extab : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH
    
    procsymstr :
    {
        KEEP(*(procsymstr)) ;
    } > FLASH
    
    procsymtab :
    {
        PROVIDE_HIDDEN (_proc_symbol_table_start = .) ;
        KEEP(*(procsymtab)) ;
        PROVIDE_HIDDEN (_proc_symbol_table_end = .) ;
    } > FLASH  
    
    kprocsymstr :
    {
        KEEP(*(kprocsymstr)) ;
    } > FLASH
    
    kprocsymtab :
    {
        PROVIDE_HIDDEN (_kproc_symbol_table_start = .) ;
        KEEP(*(kprocsymtab)) ;
        PROVIDE_HIDDEN (_kproc_symbol_table_end = .) ;
    } > FLASH  

    .bss : {
        __bss_start__ = .;
        *(.shbss)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN (4);
        __bss_end__ = .;
    } > SRAM

    .heap : {
        __heap_start__ = .;
        end = __heap_start__;
        _end = end;
        __end = end;
        KEEP(*(.heap))
        __heap_end__ = .;
        __HeapLimit = __heap_end__;
    } > SRAM

    .stack : ALIGN(0x8) {
        _stack = .;
        __stack = .;
        KEEP(*(.stack))
    } > SRAM
}
