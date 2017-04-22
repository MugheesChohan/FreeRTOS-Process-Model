ENTRY(main_task);

SECTIONS
{
    .text 0x0 :
    {
        PROVIDE_HIDDEN (_proc_text_start = .) ;
        *(.text .text*) ;
        *(.glue_7t) ;
        *(.glue_7) ;
        *(.gnu.linkonce.t*)
        
        /*. = ALIGN(4) ;*/
    }

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    }

    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN (_proc_text_end = .) ;
    }

    .rodata _proc_text_end : 
    {
        PROVIDE_HIDDEN (_proc_rodata_start = .) ;
        *(.rodata*) ;
    }

    .got.plt :
    {
        *(.got.plt)
    }

    .got :
    {
        *(.got)
        PROVIDE_HIDDEN (_proc_rodata_end = .) ;
    }

    .data _proc_rodata_end :
    {
        PROVIDE_HIDDEN (_proc_data_start = .) ;
        *(.data) ;
        *(.data*) ;
        *(.gnu.linkonce.d*)
        PROVIDE_HIDDEN (_proc_data_end = .) ;
    }

    .bss _proc_data_end :
    {
        PROVIDE_HIDDEN (_proc_bss_start = .) ;
        *(.bss) ;
        *(.bss*) ;
        *(COMMON) ;
        PROVIDE_HIDDEN (_proc_bss_end = .) ;
        PROVIDE_HIDDEN (_proc_load_end = .) ;
    }
    
    proc_end_mark ALIGN(4) :
    {
    	LONG(0xFFFFFFFF);
    }

    .dynsym :
    {
        *(.dynsym)
    }
    
    .dynstr :
    {
        *(.dynstr)
    }
    
    .rel.plt :
    {   
        *(.rel.plt)
    }

    .rel.dyn :
    {
        *(.rel.dyn)
    }
    
    .ARM.attributes :
    {
        *(.ARM.attributes)
    }
    
    .hash :
    {
        *(.hash)
    }

    .shstrtab :
    {
        *(.shstrtab)
    }
    
    /DISCARD/ :
    {
        *(.dynamic)
        *(.iplt)
        *(.igot.plt)
        *(.rel.iplt)
        *(.shstrtab)
        *(.interp)
        *(.comment)
        *(.gnu.version*)
    }
}
