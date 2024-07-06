#ifndef PRINT_C
#define PRINT_C
#include <stdint.h>
#include "print.h"
#include "../io.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

uint16_t* terminal_buffer = (uint16_t*)0xC03FF000;
uint8_t terminal_x = 0;
uint8_t terminal_y = 0;

void print_char(char c, uint16_t color) {
    if (c == '\n') {
        terminal_x = 0;
        terminal_y++;
    } else {
        terminal_buffer[terminal_y * SCREEN_WIDTH + terminal_x] = (color << 8) | c;
        terminal_x++;
    }

    if (terminal_x >= SCREEN_WIDTH) {
        terminal_x = 0;
        terminal_y++;
    }

    if (terminal_y >= SCREEN_HEIGHT) {
        terminal_y = 0;
    }
}

void print_string(char* str, uint16_t color) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i], color);
    }
}

void print_int(int num, uint16_t color) {
    if (num == 0) {
        print_char('0', color);
        return;
    }

    char buffer[32];
    int i = 0;
    while (num > 0) {
        buffer[i] = num % 10 + '0';
        num /= 10;
        i++;
    }

    for (int j = i - 1; j >= 0; j--) {
        print_char(buffer[j], color);
    }
}

void print_hex(uint32_t num, uint16_t color) {
    print_string("0x", color);
    char buffer[32];
    int i = 0;
    while (num > 0) {
        int digit = num % 16;
        if (digit < 10) {
            buffer[i] = digit + '0';
        } else {
            buffer[i] = digit - 10 + 'A';
        }
        num /= 16;
        i++;
    }

    for (int j = i - 1; j >= 0; j--) {
        print_char(buffer[j], color);
    }
}

void print_newline() {
    terminal_x = 0;
    terminal_y++;
}

void clear_screen() {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        terminal_buffer[i] = 0;
    }
    terminal_x = 0;
    terminal_y = 0;
}

void disable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void enable_cursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | 0x0F);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | 0x0F);
}

void set_terminal_position(uint8_t x, uint8_t y) {
    terminal_x = x;
    terminal_y = y;
}

int get_terminal_x() {
    return terminal_x;
}

int get_terminal_y() {
    return terminal_y;
}

void print(char* str) {
    print_colored(str, terminal_default_color);
}

void print_colored(char* str, uint16_t color) {
    print_string(str, color);
    print_newline();
}

void print_char_at(char c, uint16_t color, uint8_t x, uint8_t y) {
    uint8_t old_x = terminal_x;
    uint8_t old_y = terminal_y;
    set_terminal_position(x, y);
    print_char(c, color);
    set_terminal_position(old_x, old_y);
}

void print_at(char* str, uint16_t color, uint8_t x, uint8_t y) {
    uint8_t old_x = terminal_x;
    uint8_t old_y = terminal_y;
    set_terminal_position(x, y);
    print_colored(str, color);
    set_terminal_position(old_x, old_y);
}

#endif