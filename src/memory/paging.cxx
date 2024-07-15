#include <memory/paging.hxx>

PageTableManager::PageTableManager() {
    uint64_t pml4_address;
    asm volatile("mov %%cr3, %0" : "=r"(pml4_address));

    pml4 = (PageMapLevel4*)pml4_address;
}

