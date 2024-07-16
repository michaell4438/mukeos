#ifndef MEMORY_PMM_H
#define MEMORY_PMM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bootloader/limine.h>

#define PMM_MEM_FREE            1 // able to be allocated
#define PMM_MEM_ALLOCATED       2 // already allocated (note: this flag should only be set by the PMM itself)
#define PMM_MEM_RESERVED        3 // will never be freed (this flag should be set by the kernel; the PMM will never set this flag)
#define PMM_MEM_RECLAIMABLE     4 // can be reallocated if enable_reclaiming is true

struct PhysicalMemoryMap {
    uint64_t base;
    uint64_t length;
    uint8_t type;
    struct PhysicalMemoryMap* next;
} __attribute__((packed));

class PhysicalMemoryManager {
    public:
        PhysicalMemoryManager(struct PhysicalMemoryMap* mmap);
        PhysicalMemoryManager(limine_memmap_response* memmap_response);
        uint64_t alloc(uint64_t* bytes, uint8_t type);
        void free(uint64_t addr);
        void set_reclaiming(bool reclaiming);
        void add_entry(uint64_t base, uint64_t length, uint8_t type);
    private:
        struct PhysicalMemoryMap* mmap;
        uint64_t memory_size;
        uint64_t free_memory;
        bool enable_reclaiming;

        uint8_t limine_type_to_pmm_type(uint8_t limine_type);

        void index_memory();
        void init();
};

#endif