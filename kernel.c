#include <stdint.h>
#include "terminal/print.h"

// must be named "kernel_main"

void kernel_main(void) {
    disable_cursor();

    uint16_t color = VGA_LIGHT_GREY_FG | VGA_BLACK_BG;
    clear_screen();

    print_string("Muke OS\nKernel 1.0", color);
    while (1) {}
}
