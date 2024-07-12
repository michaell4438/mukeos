#ifndef INTERRUPTS_INTERRUPTS_H
#define INTERRUPTS_INTERRUPTS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <screen/screen.hxx>
#include <screen/textdisplay.hxx>
#include <io.h>

struct idtr64 {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct idt_desc64 {
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
        idt_desc64 idt[256];
        idtr64* idt_ptr;
        void get_idt_desc64(uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr, idt_desc64* desc);
};

#endif