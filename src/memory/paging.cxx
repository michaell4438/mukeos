#include <memory/paging.hxx>

PageTableManager::PageTableManager(uint64_t hhdm_offset) {
    uint64_t pml4_address;
    asm volatile("mov %%cr3, %0" : "=r"(pml4_address));

    pml4_address = (uint64_t) pml4_address + hhdm_offset;

    pml4 = (PageMapLevel4*)pml4_address;
}

PageDirectoryPointerTable* PageTableManager::get_directory_pointer_table(uint64_t virtual_addr) {
    uint64_t pml4_index = (virtual_addr >> 39) & 0x1FF;
    return pml4->tables[pml4_index];
}

PageDirectory* PageTableManager::get_directory(uint64_t virtual_addr) {
    PageDirectoryPointerTable* pdpt = get_directory_pointer_table(virtual_addr);
    if (!pdpt) return nullptr;
    else {
        uint64_t pdpt_index = (virtual_addr >> 30) & 0x1FF;
        return pdpt->directories[pdpt_index];
    }
}

PageTable* PageTableManager::get_table(uint64_t virtual_addr) {
    PageDirectory* pd = get_directory(virtual_addr);
    if (!pd) return nullptr;
    else {
        uint64_t pd_index = (virtual_addr >> 21) & 0x1FF;
        return pd->tables[pd_index];
    }
}

PageTableEntry* PageTableManager::get_page(uint64_t virtual_addr) {
    PageTable* pt = get_table(virtual_addr);
    if (!pt) return nullptr;
    else {
        uint64_t pt_index = (virtual_addr >> 12) & 0x1FF;
        return &pt->entries[pt_index];
    }
}

void PageTableManager::map_page(uint64_t virtual_addr, uint64_t phys_addr, uint64_t flags) {
    PageTableEntry* page = get_page(virtual_addr);
    if (!page) return;

    page->address = phys_addr >> 12;
    page->present = 1;
    page->write = (flags & PTE_WRITE) > 0;
    page->user = (flags & PTE_USER) > 0;
    page->write_through = (flags & PTE_WRITE_THROUGH) > 0;
    page->cache_disable = (flags & PTE_CACHE_DISABLE) > 0;
    page->accessed = (flags & PTE_ACCESSED) > 0;
    page->dirty = (flags & PTE_DIRTY) > 0;
    page->huge_page = (flags & PTE_HUGE_PAGE) > 0;
    page->global = (flags & PTE_GLOBAL) > 0;
    page->no_execute = (flags & PTE_NO_EXECUTE) > 0;

    asm volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
}

void PageTableManager::map_range(uint64_t virtual_addr, uint64_t phys_addr, uint64_t size, uint64_t flags) {
    for (uint64_t i = 0; i < size; i += FRAME_SIZE) {
        map_page(virtual_addr + i, phys_addr + i, flags);
    }
}

void PageTableManager::unmap_page(uint64_t virtual_addr) {
    PageTableEntry* page = get_page(virtual_addr);
    if (!page) return;

    page->address = 0;
    page->present = 0;

    asm volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
}

void PageTableManager::modify_page(uint64_t virtual_addr, uint64_t phys_addr, uint64_t flags) {
    PageTableEntry* page = get_page(virtual_addr);
    if (!page) return;

    page->address = phys_addr >> 12;
    page->write = (flags & PTE_WRITE) > 0;
    page->user = (flags & PTE_USER) > 0;
    page->write_through = (flags & PTE_WRITE_THROUGH) > 0;
    page->cache_disable = (flags & PTE_CACHE_DISABLE) > 0;
    page->accessed = (flags & PTE_ACCESSED) > 0;
    page->dirty = (flags & PTE_DIRTY) > 0;
    page->huge_page = (flags & PTE_HUGE_PAGE) > 0;
    page->global = (flags & PTE_GLOBAL) > 0;
    page->no_execute = (flags & PTE_NO_EXECUTE) > 0;

    asm volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
}

void PageTableManager::update_cr3() {
    asm volatile("mov %0, %%cr3" : : "r"(pml4) : "memory");
}


