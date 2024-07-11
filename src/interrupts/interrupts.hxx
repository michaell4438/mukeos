#ifndef INTERRUPTS_INTERRUPTS_H
#define INTERRUPTS_INTERRUPTS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <screen/screen.hxx>
#include <screen/textdisplay.hxx>
#include <io.h>

struct idtr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));


struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed));

class InterruptManager {
    public:
        void init();
    private:
        idt_entry idt[256];
        idtr* idt_ptr;
        void initialize_pic();
        void init_idt_desc(uint16_t select, uint64_t offset, uint8_t type, uint8_t ist, idt_entry* desc);
};

#endif