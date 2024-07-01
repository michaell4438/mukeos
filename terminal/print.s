.set VGABUFFER, 0xC03FF000

.global print_char
print_char:
    # eax = char
    # ebx = color

    shl $8, %ebx
    or %eax, %ebx
    mov %ebx, VGABUFFER
    ret