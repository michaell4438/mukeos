#include <interrupts/interrupts.hxx>

void InterruptManager::init() {
    generate_idt();
    load_idt();
}

void InterruptManager::set_handler(uint8_t interrupt, void (*handler)(void)) {
    handlers[interrupt] = handler;
}

void default_handler(void) {
    textdisplay_instance->clear(textdisplay_instance->get_color(0, 0, 0xff));
    textdisplay_instance->print("Unhandled interrupt", textdisplay_instance->get_color(0xff, 0, 0), textdisplay_instance->get_color(0, 0, 0xff));
    while (1);
    
    asm("iretq");
}

void InterruptManager::clear_handler(uint8_t interrupt) {
    handlers[interrupt] = default_handler;
}

void InterruptManager::generate_idt() {
    for (int i = 0; i < 256; i++) {
        idt[i].offset_low = (uint16_t)((uint64_t)handlers[i] & 0xffff);
        idt[i].selector = 0x08;
        idt[i].ist = 0;
        idt[i].type_attr = 0x8e;
        idt[i].offset_mid = (uint16_t)(((uint64_t)handlers[i] >> 16) & 0xffff);
        idt[i].offset_high = (uint32_t)(((uint64_t)handlers[i] >> 32) & 0xffffffff);
        idt[i].zero = 0;
    }

    idt_ptr->limit = sizeof(idt_entry) * 256 - 1;
    idt_ptr->base = (uint64_t)&idt;
}

void InterruptManager::load_idt() {
    asm("lidt %0" : : "m"(*idt_ptr));
}

void InterruptManager::initialize_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}