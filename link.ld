/* Memory layout for STM32F407 */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Where to put our code */
SECTIONS
{
    /* The vector table MUST go first */
    .vectors : {
        *(.vectors)
    } > FLASH
    
    /* Code goes after vectors */
    .text : {
        *(.text)
        *(.text*)
		*(.rodata)
		*(.rodata*)
    } > FLASH
	
	/* Mark where .data initial values start in flash */
    _sidata = LOADADDR(.data);
	
	.data : {
        _sdata = .; 			/* . = 0x20000000 (start of RAM) */
        *(.data)				/* Linker places all .data here, say 256 bytes */
        *(.data*)
        _edata = .; 			/* . = 0x20000100 (moved forward by data size) */
    } > RAM AT > FLASH
    
    .bss : {
        _sbss = .;				/* . = 0x20000100 (continues from where .data ended) */
        *(.bss)					/* Linker places all .bss here, say 1024 bytes */
        *(.bss*)
        *(COMMON)
        _ebss = .;				/* . = 0x20000500 (moved forward by bss size) */
    } > RAM
    
    /* Stack at top of RAM */
    _estack = 0x20020000;
}