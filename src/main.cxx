#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bootloader/limine.h>
#include <screen/screen.hxx>
#include <screen/font.h>
#include <screen/textdisplay.hxx>
#include <interrupts/interrupts.hxx>
#include <memory/paging.hxx>
#include <memory/pmm.hxx>
#include <memory/pfa.hxx>

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

__attribute__((used, section(".requests_start_marker")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".requests_end_marker")))
static volatile LIMINE_REQUESTS_END_MARKER;

static void hcf(void) {
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

extern "C"
void _start(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    Screen screen(framebuffer, font);
    TextDisplay textdisplay(&screen, 8, 16);

    set_textdisplay_instance(&textdisplay);

    textdisplay_instance->clear();
    textdisplay_instance->println("Hello, World!");

    InterruptManager interrupts;
    interrupts.init();

    uint64_t hhdm_offset = 0;
    if (hhdm_request.response != NULL) {
        hhdm_offset = hhdm_request.response->offset;
    }

    PhysicalMemoryManager pmm(memmap_request.response, hhdm_offset);
    PageTableManager ptm(hhdm_offset, &pmm);

    uint64_t addr = pmm.alloc(4096);

    textdisplay_instance->print(addr, 16, "0x");
    textdisplay_instance->println("");

    pmm.free(addr);

    PageFrameAllocator pfa(&pmm, &ptm, kernel_end, 0xFFFFFFFFFFFFFFFF);

    addr = pfa.alloc(1);

    textdisplay_instance->print(addr, 16, "0x");
    textdisplay_instance->println("");

    hcf();
}