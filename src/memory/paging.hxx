#ifndef MEMORY_PAGING_H
#define MEMORY_PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bootloader/limine.h>
#include <bootloader/constants.h>

#define PTE_PRESENT (1 << 0)
#define PTE_WRITE (1 << 1)
#define PTE_USER (1 << 2)
#define PTE_WRITE_THROUGH (1 << 3)
#define PTE_CACHE_DISABLE (1 << 4)
#define PTE_ACCESSED (1 << 5)
#define PTE_DIRTY (1 << 6)
#define PTE_HUGE_PAGE (1 << 7)
#define PTE_GLOBAL (1 << 8)
#define PTE_NO_EXECUTE (1 << 63)

struct PageTableEntry {
    uint64_t present : 1;
    uint64_t write : 1;
    uint64_t user : 1;
    uint64_t write_through : 1;
    uint64_t cache_disable : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t huge_page : 1;
    uint64_t global : 1;
    uint64_t available : 3;
    uint64_t address : 40;
    uint64_t reserved : 11;
    uint64_t no_execute : 1;
} __attribute__((packed));

// define structs which contain the heirarchy of page tables
// each struct contains 512 pointers to the next level of page tables
// with the exception of the PageTable struct which contains 512 page table entries
struct PageTable {
    PageTableEntry entries[512];
} __attribute__((packed));

struct PageDirectory {
    PageTable* tables[512];
} __attribute__((packed));

struct PageDirectoryPointerTable {
    PageDirectory* directories[512];
} __attribute__((packed));

struct PageMapLevel4 {
    PageDirectoryPointerTable* tables[512];
} __attribute__((packed));

#define FRAME_SIZE 4096

class PageTableManager {
    public:
        PageTableManager(uint64_t hhdm_offset);

        PageDirectoryPointerTable* get_directory_pointer_table(uint64_t virtual_addr);
        PageDirectory* get_directory(uint64_t virtual_addr);
        PageTable* get_table(uint64_t virtual_addr);
        PageTableEntry* get_page(uint64_t virtual_addr);

        void map_page(uint64_t virtual_addr, uint64_t phys_addr, uint64_t flags);
        void map_range(uint64_t virtual_addr, uint64_t phys_addr, uint64_t size, uint64_t flags);
        void unmap_page(uint64_t virtual_addr);

        void modify_page(uint64_t virtual_addr, uint64_t phys_addr, uint64_t flags);
        void update_cr3();
    private:
        PageMapLevel4* pml4;
};

#endif