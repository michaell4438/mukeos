#ifndef BUILD_GDT_H
#define BUILD_GDT_H

#include <stdint.h>

#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_ACCESS_ACCESSED                 0b00000001 // Accessed
#define SEG_ACCESS_READWRITE                0b00000010 // Read-write
#define SEG_ACCESS_DIRECTION_CONFORMING     0b00000100 // Direction/Conforming
#define SEG_ACCESS_EXECUTABLE               0b00001000 // Executable
#define SEG_ACCESS_DESCRIPTOR_TYPE          0b00010000 // Descriptor type
#define SEG_ACCESS_PRIVELEGE_0              0b00000000 // Privilege level 0 (kernel, highest)
#define SEG_ACCESS_PRIVELEGE_1              0b00100000 // Privilege level 1
#define SEG_ACCESS_PRIVELEGE_2              0b01000000 // Privilege level 2
#define SEG_ACCESS_PRIVELEGE_3              0b01100000 // Privilege level 3 (user, lowest)
#define SEG_ACCESS_PRESENT                  0b10000000 // Present

void build_default_gdt();
uint64_t gdt_entry(uint32_t base, uint32_t limit, uint16_t flags, uint8_t access);

#endif