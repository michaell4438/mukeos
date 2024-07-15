#ifndef MEMORY_PAGING_H
#define MEMORY_PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline uint64_t read_cr3(void);

#define PTE_PRESENT (1 << 0)
#define PTE_WRITE (1 << 1)
#define PTE_USER (1 << 2)

uint64_t* get_page_table_entry(uint64_t* base, uint64_t index);

uint64_t* get_page_table_entry(uint64_t* pml4_base, uint64_t vaddr);

void modify_page_table_entry(uint64_t* pml4_base, uint64_t vaddr, uint64_t paddr, uint64_t flags);

#endif