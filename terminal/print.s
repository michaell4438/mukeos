.set VGABUFFER, 0xC03FF000
.set SCREEN_WIDTH, 80
.set SCREEN_HEIGHT, 25

.global print_char
.global set_term_position
.global print_string
.global print_int
.global print_hex
.global print_newline
.global disable_cursor
.global clear_screen

.section .rodata
newline:
    .byte 0x0A
    .byte 0

hex_prefix:
    .ascii "0x"
    .byte 0

.section .bss
char_index:
    .skip 4

_print_int_counter:
    .skip 1

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
    addl $2, char_index

    popa

    mov %ebp, %esp
    pop %ebp
    ret

set_term_position:
    # eax = x
    # ebx = y

    push %ebp
    mov %esp, %ebp

    pusha

    mov $SCREEN_WIDTH, %ecx # ecx = SCREEN_WIDTH
    push %eax # save x
    mov %ebx, %eax # eax = y
    mul %ecx # eax = y * SCREEN_WIDTH
    mov $2, %ecx # ecx = 2
    mul %ecx # eax = y * SCREEN_WIDTH * 2
    mov %eax, %ebx # ebx = y * SCREEN_WIDTH * 2
    pop %eax # restore x
    mul %ecx # eax = x * 2
    addl %ebx, %eax # eax = y * SCREEN_WIDTH * 2 + x * 2
    mov %eax, char_index

    popa

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

        # handle newline character (\n), or 0x0A
        cmp $0x0A, %eax
        je 3f

        # check for null terminator
        test %eax, %eax
        jz 2f

        call print_char
        inc %ecx
        jmp 1b

    2:
        popa
        mov %ebp, %esp
        pop %ebp
        ret
    3:
        # handle newline character
        # get current line number
        pusha
        call get_current_line # current line number is in eax
        add $1, %eax # increment line number
        mov %eax, %ebx # ebx = y
        mov $0, %eax # eax = x
        call set_term_position
        popa
        inc %ecx
        jmp 1b

print_int:
    # eax = integer
    # ebx = color
    push %ebp
    mov %esp, %ebp

    pusha

    movw $0, _print_int_counter

    1:
        mov $10, %ecx # edx = 10
        mov $0, %edx # edx = 0
        div %ecx # eax = integer / 10, edx = integer % 10
        push %edx # push digit to stack then print it. this fixes the reversed order
        incw _print_int_counter
        test %eax, %eax # if eax == 0 then we are done
        jz 2f
        jmp 1b
    2:
        pop %eax
        add $0x30, %eax # convert digit to ascii
        call print_char
        decw _print_int_counter
        mov _print_int_counter, %ecx
        test %ecx, %ecx
        jnz 2b
    3: 
        popa
        mov %ebp, %esp
        pop %ebp
        ret

print_hex:
    # eax = integer
    # ebx = color
    push %ebp
    mov %esp, %ebp

    pusha

    push %eax
    mov $hex_prefix, %eax
    call print_string
    pop %eax

    movw $0, _print_int_counter

    1:
        mov $16, %ecx # edx = 16
        mov $0, %edx # edx = 0
        div %ecx # eax = integer / 16, edx = integer % 16
        push %edx # push digit to stack then print it. this fixes the reversed order
        incw _print_int_counter
        test %eax, %eax # if eax == 0 then we are done
        jz 2f
        jmp 1b
    2:
        pop %eax
        cmp $10, %eax
        jl 3f
        add $0x37, %eax # convert digit to ascii
        jmp 4f
    3:
        add $0x30, %eax # convert digit to ascii
    4:
        call print_char
        decw _print_int_counter
        mov _print_int_counter, %ecx
        test %ecx, %ecx
        jnz 2b
    5: 
        popa
        mov %ebp, %esp
        pop %ebp
        ret

get_current_line:
    # Get the current line number
    # char_index / SCREEN_WIDTH
    push %ebp
    mov %esp, %ebp

    push %ebx
    push %edx

    mov char_index, %ebx
    mov $SCREEN_WIDTH, %eax
    mov $2, %edx # 2 bytes per character
    mul %edx # eax = SCREEN_WIDTH * 2
    xchg %eax, %ebx # ebx = SCREEN_WIDTH * 2, eax = char_index
    div %ebx # result is in eax, remainder is in edx but we don't need it

    pop %edx
    pop %ebx

    mov %ebp, %esp
    pop %ebp
    ret

print_newline:
    # Print a newline character
    pusha
    mov $newline, %eax
    mov $0x0F, %ebx
    call print_string
    popa
    ret

disable_cursor:
    push %ebp
    mov %esp, %ebp

    pusha

    mov $0, %eax
    mov $0, %edx

    mov $0x3D4, %edx
    mov $0x0A, %al
    out %al, %dx

    inc %edx
    mov $0b00100000, %al
    out %al, %dx

    popa

    mov %ebp, %esp
    pop %ebp
    ret

clear_screen:
    push %ebp
    mov %esp, %ebp

    pusha

    # clear the vga buffer using rep
    lea VGABUFFER, %edi # destination
    mov $0, %eax # value
    mov $SCREEN_WIDTH * SCREEN_HEIGHT * 2, %ecx # count (80 * 25 * 2)
    rep stosb

    popa

    mov %ebp, %esp
    pop %ebp

    ret