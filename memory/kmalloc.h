#ifndef KMALLOC_H
#define KMALLOC_H

#include <stdint.h>
#include "../multiboot/multiboot.h"

void kmalloc_init(struct multiboot_mmap_entry* mmap_entries, uint32_t mmap_entry_count);
void* kmalloc(uint32_t size);
void kfree(void* ptr);

#endif