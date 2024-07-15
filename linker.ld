OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

SECTIONS
{
    . = 0xffffffff80000000 + SIZEOF_HEADERS;

    kernel_start = .;
    .rodata ALIGN(CONSTANT(MAXPAGESIZE)) : {
        *(.rodata .rodata.*)
    }

    .text ALIGN(CONSTANT(MAXPAGESIZE)) : {
        *(.text .text.*)
    }

    .data ALIGN(CONSTANT(MAXPAGESIZE)) : {
        *(.data .data.*)

        KEEP(*(.requests_start_marker))
        KEEP(*(.requests))
        KEEP(*(.requests_end_marker))
    }

    .dynamic : {
        *(.dynamic)
    }

    .bss ALIGN(CONSTANT(MAXPAGESIZE)) : {
        *(.bss .bss.*)
        *(COMMON)
    }
    kernel_end = .;

    /DISCARD/ : {
        *(.interp)
    }
}