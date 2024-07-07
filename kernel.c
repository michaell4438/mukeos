#include <stdint.h>
#include "terminal/print.h"
#include "interrupts/idt.h"
#include "gdt/gdt.h"
#include "multiboot/multiboot.h"
#include "memory/kmalloc.h"

// must be named "kernel_main"
// use cdecl calling convention
__attribute__((cdecl))
void kernel_main(struct multiboot_info* mbi) {
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
    init_multiboot_info(mbi);
    print_colored("Done", VGA_GREEN_FG);
    print_string("Initializing memory manager...", VGA_LIGHT_GREY_FG);
    kmalloc_init(get_mmap_entries(), get_mmap_entry_count());
    print_colored("Done", VGA_GREEN_FG);
}
