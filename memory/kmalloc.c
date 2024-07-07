#ifndef KMALLOC_C
#define KMALLOC_C

#include <stdint.h>
#include "kmalloc.h"

struct memory_block {
    uint32_t size;
    uint32_t start;
    struct memory_block* next;
};

struct memory_block* first_block;

void kmalloc_init(struct multiboot_mmap_entry* mmap_entries, uint32_t mmap_entry_count) {
    for (uint8_t i = 0; i < mmap_entry_count; i++) {
        struct multiboot_mmap_entry* entry = &mmap_entries[i];
        if (entry->type == 1) {
            struct memory_block* block = (struct memory_block*)entry->addr;
            block->size = entry->len - sizeof(struct memory_block);
            block->start = entry->addr + sizeof(struct memory_block);
            block->next = first_block;
            first_block = block;
        }
    }
    
}

void* kmalloc(uint32_t size) {
    struct memory_block* current_block = first_block;
    struct memory_block* previous_block = 0;

    while (current_block != 0) {
        if (current_block->size >= size) {
            if (current_block->size > size + sizeof(struct memory_block)) {
                struct memory_block* new_block = (struct memory_block*)(current_block->start + size);
                new_block->size = current_block->size - size - sizeof(struct memory_block);
                new_block->start = current_block->start + size + sizeof(struct memory_block);
                new_block->next = current_block->next;
                current_block->size = size;
                current_block->next = new_block;
            }

            if (previous_block != 0) {
                previous_block->next = current_block->next;
            } else {
                first_block = current_block->next;
            }

            return (void*)current_block->start;
        }

        previous_block = current_block;
        current_block = current_block->next;
    }

    return 0;
}

void kfree(void* ptr) {
    struct memory_block* current_block = first_block;
    struct memory_block* previous_block = 0;

    while (current_block != 0) {
        if (current_block->start == (uint32_t)ptr) {
            if (previous_block != 0) {
                previous_block->next = current_block->next;
            } else {
                first_block = current_block->next;
            }

            if (current_block->next != 0) {
                if ((uint32_t)current_block + current_block->size + sizeof(struct memory_block) == current_block->next->start) {
                    current_block->size += current_block->next->size + sizeof(struct memory_block);
                    current_block->next = current_block->next->next;
                }
            }

            return;
        }

        previous_block = current_block;
        current_block = current_block->next;
    }
}

#endif