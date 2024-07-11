#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bootloader/limine.h>
#include <screen/screen.hxx>
#include <screen/font.h>
#include <screen/textdisplay.hxx>
#include <interrupts/interrupts.hxx>

__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(2);

__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
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
    textdisplay_instance->print("Hello, World!");

    InterruptManager interrupts;
    interrupts.init();

    // intentionally cause a divide by zero exception
    int a = 1;
    int b = 0;
    int c = a / b;
    textdisplay_instance->clear(c);

    hcf();
}