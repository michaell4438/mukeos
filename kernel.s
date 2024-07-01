.global kernel_main

.extern print_char

kernel_main:
    mov $0x0F, %ebx
    mov $'H', %eax
    call print_char
    ret