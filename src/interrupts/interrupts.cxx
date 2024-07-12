#include <interrupts/interrupts.hxx>

extern uint64_t _asm_syscalls;
extern uint64_t _asm_GP_exc;
extern uint64_t _asm_PF_exc;

void InterruptManager::init() {
    // Set all 256 interrupts to the _asm_syscalls handler
    for (int i = 0; i < 256; i++) {
        get_idt_desc64(_asm_syscalls, 0x08, 0, 0x8E, &idt[i]);
    }

    // Set the default interrupts
    get_idt_desc64(_asm_GP_exc, 0x08, 0, 0x8E, &idt[13]); // General Protection Exception
    get_idt_desc64(_asm_PF_exc, 0x08, 0, 0x8E, &idt[14]); // Page Fault Exception

    idt_ptr = (idtr64*)0x1000;
    idt_ptr->limit = 256 * sizeof(idt_desc64) - 1;
    idt_ptr->base = (uint64_t)&idt;

    asm volatile("lidt %0" : : "m"(*idt_ptr));
}

extern "C"
void isr_default_int(uint8_t int_no) {
    textdisplay_instance->print("Interrupt: ");
    char *int_no_str = "00";
    textdisplay_instance->itoa(int_no, int_no_str, 10);
    textdisplay_instance->println(int_no_str);
    while (1);
}

extern "C"
void isr_syscalls(uint8_t int_no) {
    textdisplay_instance->print("Syscall: ");
    char *int_no_str = "00";
    textdisplay_instance->itoa(int_no, int_no_str, 10);
    textdisplay_instance->println(int_no_str);
}

extern "C"
void isr_GP_exc() {
    textdisplay_instance->println("General Protection Exception");
    while (1);
}

extern "C"
void isr_PF_exc() {
    textdisplay_instance->println("Page Fault Exception");
    while (1);
}

void InterruptManager::get_idt_desc64(uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr, idt_desc64* desc) {
    desc->offset_low = offset & 0xFFFF;
    desc->offset_mid = (offset >> 16) & 0xFFFF;
    desc->offset_high = (offset >> 32) & 0xFFFFFFFF;
    desc->selector = selector;
    desc->ist = ist;
    desc->type_attr = type_attr;
    desc->zero = 0;
}