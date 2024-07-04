#ifndef IDT_C
#define IDT_C
#include <stdint.h>
#include "idt.h"
#include "../terminal/print.h"
#include "pic.h"    

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];
static char* vector_messages[] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Detected overflow",
    "Out-of-bounds",
    "Invalid opcode",
    "No coprocessor",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check",
    "Machine check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Control protection exception"
};

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

void exception_handler() {
    print_string("An exception has occurred! ", VGA_RED_FG);
    // Check why the exception occurred
    uint8_t vector = inb(0x20);
    if (vectors[vector]) {
        print_string(vector_messages[vector-16], VGA_WHITE_FG);
        print_string(" (", VGA_WHITE_FG);
        print_hex(vector, VGA_WHITE_FG);
        print_string(")", VGA_WHITE_FG);
        print_newline();
    } else {
        print_string("Unknown exception: (", VGA_WHITE_FG);
        print_hex(vector, VGA_WHITE_FG);
        print_string(")", VGA_WHITE_FG);
        print_newline();
    }

    while (1);
}

#endif