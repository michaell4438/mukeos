#ifndef SCREEN_SCREEN_H
#define SCREEN_SCREEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <bootloader/limine.h>
#include <screen/font.h>

class Screen {
    public:
        Screen(limine_framebuffer* buffer, struct bitmap_font font);
        void clear(uint32_t color);
        void clear();
        void put_char(char c, uint32_t color, size_t x, size_t y);
        size_t put_string(const char* str, uint32_t color, size_t x, size_t y);

        void set_default_color(uint32_t color);
        uint32_t get_default_color();

        void print(const char* str);
        void print(const char* str, uint32_t color);

        void set_cursor_pos(size_t x, size_t y);
        size_t get_cursor_x();
        size_t get_cursor_y();
    private:
        limine_framebuffer* buffer;
        uint32_t default_color;
        size_t cursor_x;
        size_t cursor_y;
        struct bitmap_font font;
};

#endif