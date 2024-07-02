.set VGABUFFER, 0xC03FF000
.set SCREEN_WIDTH, 80
.set SCREEN_HEIGHT, 25

.global print_char
.global set_term_position
.global print_string
.global print_int

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

    1:
        mov $10, %ecx # edx = 10
        mov $0, %edx # edx = 0
        div %ecx # eax = integer / 10, edx = integer % 10
        test %edx, %edx # if eax == 0 then we are done
        jz 2f
        push %eax # save eax
        add $0x30, %edx # convert to ASCII, %eax contains char
        mov %edx, %eax # move char to eax   
        call print_char
        pop %eax # restore eax
        jmp 1b
    2: 
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