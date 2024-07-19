#include <memory/pfa.hxx>

PageFrameAllocator::PageFrameAllocator(PhysicalMemoryManager* pmm, PageTableManager* ptm, uint64_t region_start, uint64_t region_end) {
    this->pmm = pmm;
    this->ptm = ptm;
    this->region_start = region_start;
    this->region_end = region_end;
    bitmap_size = (region_end - region_start) / 4096 / 8 + 1;
    uint64_t bitmap_location = pmm->alloc(bitmap_size) + ptm->get_hhdm_offset();
    bitmap = (uint8_t*)bitmap_location;
    for (int i = 0; i < bitmap_size; i++) {
        bitmap[i] = 0;
    }
    init_region(region_start + bitmap_size, region_end - region_start);
}

void PageFrameAllocator::init_region(uint64_t base, uint64_t size) {
    uint64_t base_index = base / 4096;
    uint64_t size_index = size / 4096;
    for (int i = base_index; i < base_index + size_index; i++) {
        set_frame(i * 4096, false);
    }
}

void PageFrameAllocator::set_frame(uint64_t frame_addr, bool used) {
    uint64_t frame_index = frame_addr / 4096;
    uint64_t byte_index = frame_index / 8;
    uint8_t bit_index = frame_index % 8;
    if (used) {
        bitmap[byte_index] |= (0b1 << bit_index);
    } else {
        bitmap[byte_index] &= ~(0b1 << bit_index);
    }
}

bool PageFrameAllocator::get_frame(uint64_t frame_addr) {
    uint64_t frame_index = frame_addr / 4096;
    uint64_t byte_index = frame_index / 8;
    uint8_t bit_index = frame_index % 8;
    return bitmap[byte_index] & (0b1 << bit_index);
}

uint64_t PageFrameAllocator::alloc(uint16_t count, bool phys, bool write, bool user, bool exec) {
    uint16_t sequential_frames = 0;
    for (int i = 0; i < bitmap_size; i++) {
        if (bitmap[i] != 0xFF) {
            for (int j = 0; j < 8; j++) {
                uint64_t addr = region_start + (i * 8 + j) * 4096;
                if (!get_frame(addr)) {
                    sequential_frames++;
                    if (sequential_frames == count) {
                        for (int k = 0; k < count; k++) {
                            set_frame(addr + k * 4096, true);
                            uint16_t flags = 0;
                            if (write) flags |= PTE_WRITE;
                            if (user) flags |= PTE_USER;
                            if (!exec) flags |= PTE_NO_EXECUTE;
                            if (phys) {
                                uint64_t frame_addr = pmm->alloc(4096);
                                ptm->map_page(addr + k * 4096, frame_addr, PTE_PRESENT | flags);
                            } else {
                                ptm->map_page(addr + k * 4096, addr + k * 4096, flags);
                            }
                        }
                        return addr;
                    }
                } else {
                    sequential_frames = 0;
                }
            }
        }
    }
}