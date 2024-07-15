#include <memory/paging.hxx>

static inline uint64_t read_cr3(void) {
    uint64_t cr3;
    asm volatile("movq %%cr3, %0" : "=r"(cr3));
    return cr3;
}

uint64_t* get_page_table_entry(uint64_t* base, uint64_t index) {
    return &base[index];
}

uint64_t* get_page_table_entry(uint64_t* pml4_base, uint64_t vaddr) {
    uint64_t pml4_index = (vaddr >> 39) & 0x1FF;
    uint64_t pdpt_index = (vaddr >> 30) & 0x1FF;
    uint64_t pd_index = (vaddr >> 21) & 0x1FF;
    uint64_t pt_index = (vaddr >> 12) & 0x1FF;

    uint64_t* pdpt_base = (uint64_t*)(pml4_base[pml4_index] & ~0xFFF);
    uint64_t* pd_base = (uint64_t*)(pdpt_base[pdpt_index] & ~0xFFF);
    uint64_t* pt_base = (uint64_t*)(pd_base[pd_index] & ~0xFFF);

    return get_page_table_entry(pt_base, pt_index);
}

void modify_page_table_entry(uint64_t* pml4_base, uint64_t vaddr, uint64_t paddr, uint64_t flags) {
    uint64_t pml4_index = (vaddr >> 39) & 0x1FF;
    uint64_t pdpt_index = (vaddr >> 30) & 0x1FF;
    uint64_t pd_index = (vaddr >> 21) & 0x1FF;
    uint64_t pt_index = (vaddr >> 12) & 0x1FF;

    uint64_t* pdpt_base = (uint64_t*)(pml4_base[pml4_index] & ~0xFFF);
    uint64_t* pd_base = (uint64_t*)(pdpt_base[pdpt_index] & ~0xFFF);
    uint64_t* pt_base = (uint64_t*)(pd_base[pd_index] & ~0xFFF);

    uint64_t* pte = get_page_table_entry(pt_base, pt_index);
    *pte = (paddr & ~0xFFF) | flags;
}