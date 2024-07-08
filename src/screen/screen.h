#ifndef SCREEN_SCREEN_H
#define SCREEN_SCREEN_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limine.h>

class Screen {
    public:
        Screen(limine_framebuffer buffer);
        void clear(uint32_t color);
        void clear();
        void put_char(char c, uint32_t color, size_t x, size_t y);
        void put_string(const char* str, uint32_t color, size_t x, size_t y);

        void set_default_color(uint32_t color);
        void get_default_color(uint32_t color);

        void print(const char* str);
        void print(const char* str, uint32_t color);
        void printf(const char* fmt, ...);

        void set_cursor_pos(size_t x, size_t y);
        void get_cursor_pos(size_t x, size_t y);

        void set_font_size(size_t size);
        void get_font_size(size_t size);
    private:
        limine_framebuffer buffer;
        uint32_t default_color;
        size_t font_size;
        size_t cursor_x;
        size_t cursor_y;
};

#endif