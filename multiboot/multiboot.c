#ifndef MULTIBOOT_C
#define MULTIBOOT_C
#include <stdint.h>
#include "multiboot.h"
#include "../terminal/print.h"

struct multiboot_info* multiboot_info;
struct multiboot_mmap_entry* mmap_entries;
uint32_t mmap_entry_count = 0;

void init_multiboot_info(struct multiboot_info* mbi) {
    multiboot_info = mbi;
    mmap_entries = (struct multiboot_mmap_entry*) mbi->mmap_addr;
    for (uint32_t i = 0; i < mbi->mmap_length; i+=sizeof(struct multiboot_mmap_entry)) {
        struct multiboot_mmap_entry* entry = &mmap_entries[i];
        mmap_entry_count++;
    }
}

uint32_t get_mmap_entry_count() {
    return mmap_entry_count;
}

struct multiboot_mmap_entry* get_mmap_entries() {
    return mmap_entries;
}

#endif