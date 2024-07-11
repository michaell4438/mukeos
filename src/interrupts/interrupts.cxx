#include <interrupts/interrupts.hxx>

extern uint64_t _asm_syscalls;
extern uint64_t _asm_GP_exc;
extern uint64_t _asm_PF_exc;

void InterruptManager::init() {
    for (int i = 0; i < 256; i++) {
        init_idt_desc(0x08, _asm_syscalls, 0x8e, 0, &idt[i]);
    }

    init_idt_desc(0x08, _asm_GP_exc, 0x8e, 0, &idt[13]);
    init_idt_desc(0x08, _asm_PF_exc, 0x8e, 0, &idt[14]);

    idt_ptr = (idtr*)0x1000;
    idt_ptr->limit = 256 * sizeof(idt_entry) - 1;
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

void InterruptManager::initialize_pic() {
    // ICW1
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2
    outb(0x21, 0x20); // IRQ0-7 mapped to IDT entries 32-39
    outb(0xA1, 0x28); // IRQ8-15 mapped to IDT entries 40-47
    
    // ICW3
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    // Mask all interrupts
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void InterruptManager::init_idt_desc(uint16_t select, uint64_t offset, uint8_t type, uint8_t ist, idt_entry* desc) {
    desc->offset_low = (uint16_t)(offset & 0xffff);
    desc->selector = select;
    desc->ist = ist;
    desc->type_attr = type;
    desc->offset_mid = (uint16_t)((offset >> 16) & 0xffff);
    desc->offset_high = (uint32_t)((offset >> 32) & 0xffffffff);
    desc->zero = 0;
}