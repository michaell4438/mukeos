#include <interrupts/interrupts.hxx>

/*
The GDT (loaded by Limine) has the following entries:
0x00: Null
16-bit code descriptor. Base = 0, limit = 0xffff. Readable.
16-bit data descriptor. Base = 0, limit = 0xffff. Writable.
32-bit code descriptor. Base = 0, limit = 0xffffffff. Readable.
32-bit data descriptor. Base = 0, limit = 0xffffffff. Writable.
64-bit code descriptor. Base and limit irrelevant. Readable. (interrupts will go here) (5th entry, offset 40)
64-bit data descriptor. Base and limit irrelevant. Writable.
*/

__attribute__((interrupt))
void isr_default_int(struct interrupt_frame* frame) {
    textdisplay_instance->print("Interrupt");
    while (1);
    reset_pic();
}

__attribute__((interrupt))
void isr_syscalls(struct interrupt_frame* frame) {
    textdisplay_instance->print("Syscall");
    while (1)
    {
        /* code */
    }
    
    reset_pic();
}

__attribute__((interrupt))
void isr_GP_exc(struct interrupt_frame* frame) {
    textdisplay_instance->println("General Protection Exception");
    while (1);
    reset_pic();
}

__attribute__((interrupt))
void isr_PF_exc(struct interrupt_frame* frame) {
    textdisplay_instance->println("Page Fault Exception");
    while (1);
    reset_pic();
}

void InterruptManager::init() {
    // Set all 256 interrupts to the _asm_syscalls handler
    for (int i = 0; i < 256; i++) {
        get_idt_desc64((uint64_t)&isr_default_int, 40, 0, 0x8E, &idt[i]);
    }

    // Set the default interrupt handlers
    get_idt_desc64((uint64_t)&isr_GP_exc, 40, 0, 0x8E, &idt[13]);
    get_idt_desc64((uint64_t)&isr_PF_exc, 40, 0, 0x8E, &idt[14]);

    idtr.limit = 256 * sizeof(idt_desc64) - 1;
    idtr.base = (uint64_t)&idt;

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");

    init_pic();
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

void InterruptManager::init_pic() {
    // ICW1
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // ICW3
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // OCW1
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void reset_pic() {
    outb(0x20, 0x20);
    outb(0xA0, 0x20);
}