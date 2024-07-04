#ifndef GDT_C
#define GDT_C

#include <stdint.h>
#include "gdt.h"

void gdt_set_descriptor(uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entry_t* descriptor = &gdt[index];

    descriptor->limit_low      = limit & 0xFFFF;
    descriptor->base_low       = base & 0xFFFF;
    descriptor->base_middle    = (base >> 16) & 0xFF;
    descriptor->access         = access;
    descriptor->granularity    = granularity;
    descriptor->base_high      = (base >> 24) & 0xFF;
}

void gdt_init() {
    gdtr_t gdtr;

    gdtr.base = (uint32_t)&gdt[0];
    gdtr.limit = sizeof(gdt_entry_t) * GDT_MAX_DESCRIPTORS - 1;

    gdt_set_descriptor(0, 0, 0, 0, 0); // Null descriptor
    gdt_set_descriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code
    gdt_set_descriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data
    gdt_set_descriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code
    gdt_set_descriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data

    __asm__ volatile ("lgdt %0" : : "m"(gdtr)); // load the new GDT
    __asm__ volatile ("mov $0x10, %ax; mov %ax, %ds; mov %ax, %es; mov %ax, %fs; mov %ax, %gs; ljmp $0x08, $next; next:"); // reload the segment registers
}

#endif