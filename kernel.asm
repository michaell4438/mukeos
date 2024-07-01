BITS 32

VGABUFFER equ 0xB8000
SCREEN_WIDTH equ 80
SCREEN_HEIGHT equ 25

global kernel_main
kernel_main:
    mov eax, 0x
    mov ebx, 0x41
    call print_char
    mov [VGABUFFER], word 0x0F41
    hlt

; Print a character to the screen
; Input: EAX = character to print
;        EBX = color to print with
print_char:
    shl eax, 8
    or eax, ebx
    mov word [VGABUFFER], ax

    ret