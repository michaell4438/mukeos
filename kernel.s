.global kernel_main

.set VGABUFFER, 0xC03FF000

kernel_main:
    movl $0x0F41, VGABUFFER
    ret