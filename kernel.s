.global kernel_main

.include "macros/colors.S"

.section .rodata
msg:
    .ascii "Welcome to MukeOS\nKernel 1.0"
    .byte 0x0A
    .byte 0

.section .text
kernel_main:
    call disable_cursor

    mov $msg, %eax
    movl $(VGA_BLACK_BG | VGA_LIGHT_GREY_FG), %ebx
    call print_string

    ret
