#ifndef BUILD_GDT_C
#define BUILD_GDT_C
#include <stdint.h>
#include "build_gdt.h"

// Paging is already enabled, and the kernel is mapped to 0xC0000000
// We will use a flat memory model, with everything having base 0 and limit 4GB
// This is because we will use paging to control access to memory

uint64_t gdt_entry(uint32_t base, uint32_t limit, uint16_t flags, uint8_t access) {
    uint64_t entry = 0;
    entry |= limit & 0x0000FFFF;
    entry |= (base & 0x00FFFFFF) << 16;
    entry |= (flags & 0x0F) << 52;
    entry |= (access & 0xFF) << 40;
    entry |= ((limit & 0x000F0000) << 32);
    entry |= ((flags & 0xF0) << 44);
    entry |= ((base & 0xFF000000) << 32);
    return entry;
}

void build_default_gdt() {
    uint64_t gdt[5];    // null, code, data, user code, user data
                        // we need to do a tss later
    gdt[0] = gdt_entry(0, 0, 0, 0); // null
    gdt[1] = gdt_entry(0, 0xFFFFFFFF, 0xC, 0x9A); // code
    gdt[2] = gdt_entry(0, 0xFFFFFFFF, 0xC, 0x92); // data
    gdt[3] = gdt_entry(0, 0xFFFFFFFF, 0xC, 0xFA); // user code
    gdt[4] = gdt_entry(0, 0xFFFFFFFF, 0xC, 0xF2); // user data

    // load the gdt
    asm volatile("lgdt %0" : : "m" (gdt));
    // reload the segment registers
    asm volatile("mov $0x10, %ax\n"
                 "mov %ax, %ds\n"
                 "mov %ax, %es\n"
                 "mov %ax, %fs\n"
                 "mov %ax, %gs\n"
                 "mov %ax, %ss\n"
                 "ljmp $0x08, $next\n"
                 "next:");
}

#endif