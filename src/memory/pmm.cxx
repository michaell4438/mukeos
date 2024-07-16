#include <memory/pmm.hxx>

PhysicalMemoryManager::PhysicalMemoryManager(struct PhysicalMemoryMap* mmap) {
    this->mmap = mmap;
    this->memory_size = 0;
    this->free_memory = 0;
    this->enable_reclaiming = false;

    init();
}

PhysicalMemoryManager::PhysicalMemoryManager(limine_memmap_response* memmap_response) {
    PhysicalMemoryMap blank = {0, 0, 0, nullptr};
    this->mmap = &blank;
    this->memory_size = 0;
    this->free_memory = 0;
    this->enable_reclaiming = false;

    for (size_t i = 0; i < memmap_response->entry_count; i++) {
        limine_memmap_entry* entry = memmap_response->entries[i];
        add_entry(entry->base, entry->length, limine_type_to_pmm_type(entry->type));
    }

    // remove the first blank entry
    mmap = mmap->next;

    init();
}

void PhysicalMemoryManager::index_memory() {
    while (mmap != nullptr) {
        memory_size += mmap->length;
        if (mmap->type == PMM_MEM_FREE) {
            free_memory += mmap->length;
        }
        mmap = mmap->next;
    }
}

void PhysicalMemoryManager::init() {
    index_memory();
}

void PhysicalMemoryManager::set_reclaiming(bool reclaiming) {
    enable_reclaiming = reclaiming;
}

void PhysicalMemoryManager::add_entry(uint64_t base, uint64_t length, uint8_t type) {
    struct PhysicalMemoryMap* entry = (struct PhysicalMemoryMap*)base;
    entry->base = base;
    entry->length = length;
    entry->type = type;
    entry->next = mmap;
    mmap = entry;
    memory_size += length;
    if (type == PMM_MEM_FREE) {
        free_memory += length;
    }
}

uint64_t PhysicalMemoryManager::alloc(uint64_t* bytes, uint8_t type) {
    // parse the linked list to find a free block of memory which is large enough
    // if enable_reclaiming is true, we can reclaim memory from allocated blocks
    // if the free space next to the desired region is < 4k, do not split (this will prevent fragmentation)
    // search through the entire memory map to find an exact match
    // if no exact match is found, split the largest block and return the address
    // if no matches are found, return 0
    // the value of bytes will be updated to the actual size of the block allocated

    struct PhysicalMemoryMap* entry = mmap;
    struct PhysicalMemoryMap* largest = nullptr;

    while (entry != nullptr) {
        if (entry->type == PMM_MEM_FREE && *bytes <= entry->length) {
            if (entry->length > largest->length) {
                largest = entry;
            }

            if (entry->length == *bytes) {
                entry->type = PMM_MEM_ALLOCATED;
                *bytes = entry->length;
                free_memory -= entry->length;
                return entry->base;
            }
        }
        entry = entry->next;
    }

    if (largest != nullptr) {
        if (largest->length - *bytes < 4096) {
            largest->type = PMM_MEM_ALLOCATED;
            free_memory -= largest->length;
            *bytes = largest->length;
            return largest->base;
        } else {
            struct PhysicalMemoryMap* new_entry = (struct PhysicalMemoryMap*)largest->base + *bytes;
            new_entry->base = largest->base + *bytes;
            new_entry->length = largest->length - *bytes;
            new_entry->type = PMM_MEM_FREE;
            new_entry->next = largest->next;
            largest->length = *bytes;
            largest->type = PMM_MEM_ALLOCATED;
            largest->next = new_entry;
            free_memory -= *bytes;
            return largest->base;
        }
    }

    return 0;
}

void PhysicalMemoryManager::free(uint64_t addr) {
    struct PhysicalMemoryMap* entry = mmap;
    struct PhysicalMemoryMap* left = nullptr;
    struct PhysicalMemoryMap* right = entry->next;

    while (entry != nullptr) {
        if (entry->base <= addr && entry->base + entry->length > addr) {
            if (entry->type == PMM_MEM_ALLOCATED || (entry->type == PMM_MEM_RECLAIMABLE && enable_reclaiming)) {
                entry->type = PMM_MEM_FREE;
                free_memory += entry->length;
            } else {
                return;
            }
        }
    }
}


uint8_t limine_type_to_pmm_type(uint8_t limine_type) {
    switch (limine_type) {
        case LIMINE_MEMMAP_USABLE:
            return PMM_MEM_FREE;
        default:
            return PMM_MEM_RESERVED;
    }
}