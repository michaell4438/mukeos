#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_MAX_DESCRIPTORS 5 // Null, Kernel Code, Kernel Data, User Code, User Data

typedef struct {
    uint16_t	limit;
    uint32_t	base;
} __attribute__((packed)) gdtr_t;

typedef struct {
    uint16_t    limit_low;      // The lower 16 bits of the GDT's limit
    uint16_t    base_low;       // The lower 16 bits of the GDT's base
    uint8_t     base_middle;    // The next 8 bits of the GDT's base
    uint8_t     access;         // Access flags; see the GDT page
    uint8_t     granularity;    // Granularity; see the GDT page
    uint8_t     base_high;      // The last 8 bits of the GDT's base
} __attribute__((packed)) gdt_entry_t;

static gdt_entry_t gdt[GDT_MAX_DESCRIPTORS]; // Create an array of GDT entries

void gdt_init(void);
void gdt_set_descriptor(uint8_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

#endif