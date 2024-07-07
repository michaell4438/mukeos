#include <stdint.h>
#include "terminal/print.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "multiboot/multiboot.h"

// must be named "kernel_main"
void kernel_main(void) {
    disable_cursor();
    clear_screen();

    print("Muke OS\nKernel 1.0");
    print("Initializing kernel...");
    print_newline();
    print_string("Loading GDT...", VGA_LIGHT_GREY_FG);
    gdt_init();
    print_colored("Done", VGA_GREEN_FG);
    print_string("Loading IDT...", VGA_LIGHT_GREY_FG);
    idt_init();
    enable_nmi();
    print_colored("Done", VGA_GREEN_FG);
    print_string("Loading multiboot info...", VGA_LIGHT_GREY_FG);
    init_multiboot_info();
    print_colored("Done", VGA_GREEN_FG);
}
