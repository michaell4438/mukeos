#include <memory/pmm.hxx>

PhysicalMemoryManager::PhysicalMemoryManager(limine_memmap_response* memmap_response, uint64_t hhdm_offset) {
    this->memory_size = 0;
    this->free_memory = 0;

    for (int i = 0; i < memmap_response->entry_count - 1; i++) {
        limine_memmap_entry* entry = memmap_response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            free_memory += entry->length;
        }
        memory_size += entry->length;
    }

    bitmap_size = memory_size / 0x1000 / 8;
    bitmap_size += 1;
    uint64_t phys_bitmap_addr = 0xFFFFFFFFFFFFFFFF;
    uint64_t bitmap_addr = 0xFFFFFFFFFFFFFFFF;

    // find the first free memory region big enough to store the bitmap
    for (int i = 0; i < memmap_response->entry_count; i++) {
        limine_memmap_entry* entry = memmap_response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= bitmap_size) {
            phys_bitmap_addr = entry->base;
            bitmap_addr = phys_bitmap_addr + hhdm_offset;
            bitmap = (uint8_t*)bitmap_addr;
            break;
        }
    }

    if (bitmap_addr == 0xFFFFFFFFFFFFFFFF) {
        // throw a page fault
        // TODO: create custom handler for this
        asm volatile("int $14");
    }

    // go through each memory region and set the bits in the bitmap
    for (int i = 0; i < memmap_response->entry_count; i++) {
        limine_memmap_entry* entry = memmap_response->entries[i];
        if (entry->type == LIMINE_MEMMAP_USABLE) {
            for (uint64_t j = 0; j < entry->length / 0x1000; j++) {
                clear_bitmap_bit(entry->base / 0x1000 + j);
            }
        } else {
            for (uint64_t j = 0; j < entry->length / 0x1000; j++) {
                set_bitmap_bit(entry->base / 0x1000 + j);
            }
        }
    }

    // set the bits for the bitmap itself
    for (uint64_t i = phys_bitmap_addr; i < phys_bitmap_addr + bitmap_size; i += 0x1000) {
        set_bitmap_bit(i / 0x1000);
    }
}

uint64_t PhysicalMemoryManager::alloc(uint64_t bytes) {
    uint64_t best_addr = 0xFFFFFFFFFFFFFFFF;
    uint64_t best_size = 0xFFFFFFFFFFFFFFFF;

    // find the smallest free memory region that can fit the requested size
    for (uint64_t i = 0; i < bitmap_size * 8; i++) {
        if (get_bitmap_bit(i)) {
            continue;
        }

        uint64_t j = i;
        while (j < bitmap_size && !get_bitmap_bit(j)) {
            j++;
        }

        if (j - i < best_size && j - i >= bytes / 0x1000) {
            best_size = j - i;
            best_addr = i;
        }
    }

    if (best_addr == 0xFFFFFFFFFFFFFFFF) {
        // throw a page fault
        asm volatile("int $14");
    }

    for (uint64_t i = best_addr; i < best_addr + bytes / 0x1000; i++) {
        set_bitmap_bit(i);
    }

    return best_addr * 0x1000;
}

void PhysicalMemoryManager::free(uint64_t addr) {
    for (uint64_t i = addr / 0x1000; i < (addr + 0x1000) / 0x1000; i++) {
        clear_bitmap_bit(i);
    }
}

void PhysicalMemoryManager::set_bitmap_bit(uint64_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

void PhysicalMemoryManager::clear_bitmap_bit(uint64_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

bool PhysicalMemoryManager::get_bitmap_bit(uint64_t bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}