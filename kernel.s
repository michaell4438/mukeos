.global kernel_main

.extern print_char

.section .rodata
msg:
    .ascii "Welcome to MukeOS"
    .byte 0

.section .text
kernel_main:
    mov $msg, %eax
    mov $0x0F, %ebx
    call print_string

    ret