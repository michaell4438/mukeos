#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <stdbool.h>

void idt_init(void);

__attribute__((noreturn))
void exception_handler(void);

void enable_nmi(void);
void disable_nmi(void);

#endif