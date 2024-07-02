.set VGABUFFER, 0xC03FF000
.set SCREEN_WIDTH, 80
.set SCREEN_HEIGHT, 25

.global print_char
.global set_term_position
.global print_string

.section .bss
char_index:
    .skip 4

.section .text
print_char:
    # eax = char
    # ebx = color
    push %ebp
    mov %esp, %ebp

    pusha

    # Load VGA buffer address to ecx and offset it by char_index
    mov $VGABUFFER, %ecx
    add char_index, %ecx

    shl $8, %ebx
    or %eax, %ebx
    mov %ebx, (%ecx)
    # increment char_index by 2
    add $2, char_index

    popa

    mov %ebp, %esp
    pop %ebp
    ret

set_term_position:
    # eax = x
    # ebx = y

    push %ebp
    mov %esp, %ebp

    mov $SCREEN_WIDTH, %ecx
    push %eax
    mov %ebx, %eax
    mul %ecx
    pop %ebx
    add %ebx, %eax

    mov %eax, char_index

    mov %ebp, %esp
    pop %ebp
    ret

print_string:
    # eax = string
    # ebx = color
    push %ebp
    mov %esp, %ebp

    pusha

    mov %eax, %ecx

    1:
        # load char from (ecx) to eax
        movzx (%ecx), %eax
        test %eax, %eax
        jz 2f

        mov $0x0F, %ebx
        call print_char
        inc %ecx
        jmp 1b

    2:
        popa
        mov %ebp, %esp
        pop %ebp
        ret