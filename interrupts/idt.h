#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <stdbool.h>

void idt_init(void);

__attribute__((noreturn))
void exception_handler(void);

#endif