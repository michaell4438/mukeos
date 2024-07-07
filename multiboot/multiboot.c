#ifndef MULTIBOOT_C
#define MULTIBOOT_C
#include <stdint.h>
#include "multiboot.h"
#include "../terminal/print.h"

extern uint32_t _mbootinfo;     // The address of the multiboot info structure
struct multiboot_info* mb_info = (struct multiboot_info*)&_mbootinfo;
struct multiboot_mmap_entry* mmap = (struct multiboot_mmap_entry*)&_mbootinfo + 512;

void init_multiboot_info() {
    print_string("Memory map address: ", terminal_default_color);
    print_hex(mb_info->mmap_addr, terminal_default_color);
}

#endif