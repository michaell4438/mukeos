#include <gdt/gdt.hxx>

void GDT64::init() {
    get_gdt_desc(0, 0, 0, 0, &gdt[0]);
    get_gdt_desc(0, 0xFFFFFFFF, 0x9A, 0xCF, &gdt[1]);
    get_gdt_desc(0, 0xFFFFFFFF, 0x92, 0xCF, &gdt[2]);

    gdtr_ptr = (gdtr*)0x500;
    gdtr_ptr->limit = 3 * sizeof(gdt64_desc) - 1;
    gdtr_ptr->base = (uint64_t)&gdt;

    asm volatile("lgdt %0" : : "m"(*gdtr_ptr));
}

void GDT64::get_gdt_desc(uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity, gdt64_desc* desc) {
    desc->base_low = base & 0xFFFF;
    desc->base_mid = (base >> 16) & 0xFF;
    desc->base_high = (base >> 24) & 0xFF;
    desc->limit_low = limit & 0xFFFF;
    desc->granularity = (limit >> 16) & 0x0F;
    desc->granularity |= granularity & 0xF0;
    desc->access = access;
}

