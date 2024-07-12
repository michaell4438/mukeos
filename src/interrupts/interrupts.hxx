#ifndef INTERRUPTS_INTERRUPTS_H
#define INTERRUPTS_INTERRUPTS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <screen/screen.hxx>
#include <screen/textdisplay.hxx>
#include <io.hxx>

struct interrupt_frame {
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
} __attribute__((packed));

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

static struct idtr64 idtr;

class InterruptManager {
    public:
        void init();
    private:
        idt_desc64 idt[256];
        void get_idt_desc64(uint64_t offset, uint16_t selector, uint8_t ist, uint8_t type_attr, idt_desc64* desc);
        void init_pic();
};

void reset_pic();

#endif