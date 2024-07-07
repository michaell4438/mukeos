#ifndef PAGE_MANAGER_C
#define PAGE_MANAGER_C
#include <stdint.h>
#include "page_manager.h"
#include "../terminal/print.h"

#define PAGE_SIZE 4096

extern uint32_t bootloader_end;
uint32_t kernel_begin = 0xC0000000; // everything after this is reserved for the kernel



#endif