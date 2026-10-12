    aftr_tools_ROM_START = __romPos;
    .aftr_tools buffers_VRAM_END : AT(aftr_tools_ROM_START) SUBALIGN(16)
    {
        FILL(0x00000000);
        aftr_tools_VRAM = .;
        build/src/aftr_tools.o(.text)
        build/src/aftr_tools.o(.data)
        build/src/aftr_tools.o(.rodata)
        . = ALIGN(16);
        aftr_tools_VRAM_END = .;
    }
    __romPos += SIZEOF(.aftr_tools);
    aftr_tools_ROM_END = __romPos;
    __romPos = ALIGN(__romPos, 16);

