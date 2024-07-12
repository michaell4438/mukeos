#ifndef GDT_GDT_H
#define GDT_GDT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

class GDT64 {
    public:
        void init();
    private:
        struct gdt64_desc {
            uint16_t limit_low;
            uint16_t base_low;
            uint8_t base_mid;
            uint8_t access;
            uint8_t granularity;
            uint8_t base_high;
        } __attribute__((packed));

        struct gdtr {
            uint16_t limit;
            uint64_t base;
        } __attribute__((packed));

        gdt64_desc gdt[3];
        gdtr* gdtr_ptr;
        void get_gdt_desc(uint64_t base, uint32_t limit, uint8_t access, uint8_t granularity, gdt64_desc* desc);
};

#endif