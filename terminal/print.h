// headers to correspond to the print.S file

#ifndef PRINT_H
#define PRINT_H
#include <stdint.h>

void print_char(char c, uint16_t color);
void print_string(char* str, uint16_t color);
void print_int(int num, uint16_t color);
void print_hex(uint32_t num, uint16_t color);
void print_newline();
void clear_screen();
void disable_cursor();
void enable_cursor();
void set_terminal_position(uint8_t x, uint8_t y);
int get_terminal_x();
int get_terminal_y();
void print(char* str);
void print_colored(char* str, uint16_t color);
void print_char_at(char c, uint16_t color, uint8_t x, uint8_t y);
void print_at(char* str, uint16_t color, uint8_t x, uint8_t y);

#define VGA_BLACK_BG 0x00
#define VGA_BLUE_BG 0x10
#define VGA_GREEN_BG 0x20
#define VGA_CYAN_BG 0x30
#define VGA_RED_BG 0x40
#define VGA_MAGENTA_BG 0x50
#define VGA_BROWN_BG 0x60
#define VGA_LIGHT_GREY_BG 0x70
#define VGA_DARK_GREY_BG 0x80
#define VGA_LIGHT_BLUE_BG 0x90
#define VGA_LIGHT_GREEN_BG 0xA0
#define VGA_LIGHT_CYAN_BG 0xB0
#define VGA_LIGHT_RED_BG 0xC0
#define VGA_LIGHT_MAGENTA_BG 0xD0
#define VGA_LIGHT_BROWN_BG 0xE0
#define VGA_WHITE_BG 0xF0

#define VGA_BLACK_FG 0x00
#define VGA_BLUE_FG 0x01
#define VGA_GREEN_FG 0x02
#define VGA_CYAN_FG 0x03
#define VGA_RED_FG 0x04
#define VGA_MAGENTA_FG 0x05
#define VGA_BROWN_FG 0x06
#define VGA_LIGHT_GREY_FG 0x07
#define VGA_DARK_GREY_FG 0x08
#define VGA_LIGHT_BLUE_FG 0x09
#define VGA_LIGHT_GREEN_FG 0x0A
#define VGA_LIGHT_CYAN_FG 0x0B
#define VGA_LIGHT_RED_FG 0x0C
#define VGA_LIGHT_MAGENTA_FG 0x0D
#define VGA_LIGHT_BROWN_FG 0x0E
#define VGA_WHITE_FG 0x0F

static uint8_t terminal_default_color = VGA_LIGHT_GREY_FG | VGA_BLACK_BG;

#endif