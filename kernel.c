#include <stdint.h>
#include "terminal/print.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"

// must be named "kernel_main"

void kernel_main(void) {
    disable_cursor();
    clear_screen();

    print("Muke OS\nKernel 1.0");
    print("Initializing kernel...");
    print_newline();
    print_string("Loading GDT...", VGA_LIGHT_GREY_FG);
    gdt_init();
    print("Done");
    print_string("Loading IDT...", VGA_LIGHT_GREY_FG);
    idt_init();
    print("Done");

    // Intentionally cause a divide by zero exception to test the IDT
    int a = 1;
    int b = 0;
    int c = a / b;
    print_int(c, VGA_WHITE_FG);
}
