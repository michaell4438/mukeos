#include <stdint.h>
#include "terminal/print.h"
#include "gdt/build_gdt.h"

// must be named "kernel_main"

void kernel_main(void) {
    disable_cursor();
    clear_screen();

    print("Muke OS\nKernel 1.0");
    print("Initializing kernel...");
    print_newline();
    print("Loading GDT...");
    build_default_gdt();
    print("GDT loaded successfully!");
}
