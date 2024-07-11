%macro SAVE_REGS 0
    pusha
    push ds
    push es
    push fs
    push gs
    push ebx
    mov bx, 0x10
    mov ds, bx
    pop ebx
%endmacro

%macro RESTORE_REGS 0
    pop gs
    pop fs
    pop es
    pop ds
    popa
%endmacro

%macro INTERRUPT 1
global _asm_int_handler_%1
_asm_int_handler_%1:
    SAVE_REGS
    push %1
    call isr_default_int
    pop eax
    mov al, 0x20
    out 0x20, al
    RESTORE_REGS
    iret
%endmacro

extern isr_GP_exc, isr_PF_exc
global _asm_syscalls, _asm_exc_GP, _asm_exc_PF
_asm_syscalls:
    SAVE_REGS
    push eax
    call isr_syscalls
    pop eax
    cli
    sti
    RESTORE_REGS
    iret

_asm_exc_GP:
    SAVE_REGS
    call isr_GP_exc
    RESTORE_REGS
    add esp, 4
    iret

_asm_exc_PF:
    SAVE_REGS
    call isr_PF_exc
    RESTORE_REGS
    add esp, 4
    iret