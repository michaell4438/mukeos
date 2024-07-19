#ifndef MEMORY_PMM_H
#define MEMORY_PMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bootloader/limine.h>

struct PhysicalMemoryMap {
    uint64_t base;
    uint64_t length;
    uint8_t type;
    struct PhysicalMemoryMap* next;
} __attribute__((packed));

class PhysicalMemoryManager {
    public:
        PhysicalMemoryManager(limine_memmap_response* memmap_response, uint64_t hhdm_offset);
        uint64_t alloc(uint64_t bytes);
        uint64_t alloc(uint64_t* bytes);
        void free(uint64_t addr);
    private:
        uint8_t* bitmap;
        uint32_t bitmap_size;
        uint64_t memory_size;
        uint64_t free_memory;

        void set_bitmap_bit(uint64_t bit);
        void clear_bitmap_bit(uint64_t bit);
        bool get_bitmap_bit(uint64_t bit);
};

#endif