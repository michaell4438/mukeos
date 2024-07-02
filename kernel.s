.global kernel_main

.extern print_char

.section .rodata
msg:
    .ascii "Welcome to MukeOS\nKernel 1.0"
    .byte 0x0A
    .byte 0

.section .text
kernel_main:
    mov $msg, %eax
    mov $0x0F, %ebx
    call print_string

    call print_int

    ret