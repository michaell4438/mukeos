#ifndef MEMORY_PFA_H
#define MEMORY_PFA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <memory/pmm.hxx>
#include <memory/paging.hxx>

class PageFrameAllocator {
    public:
        void init(PhysicalMemoryManager* pmm, PageTableManager* ptm, uint64_t region_start, uint64_t region_end);
        uint64_t alloc(uint16_t count, bool phys = true, bool write = true, bool user = false, bool exec = false);
        void free(uint64_t addr, uint16_t count);
    private:
        void init_region(uint64_t base, uint64_t size);
        void init_bitmap();
        uint8_t* bitmap;
        uint64_t bitmap_size;
        uint64_t region_start;
        uint64_t region_end;
        PhysicalMemoryManager* pmm;
        PageTableManager* ptm;
        void set_frame(uint64_t frame_addr, bool used);
        bool get_frame(uint64_t frame_addr);
};

#endif