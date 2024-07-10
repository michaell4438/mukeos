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
        void put_char(char c, uint32_t color, uint32_t bg_color, size_t x, size_t y);
        size_t put_string(const char* str, uint32_t color, size_t x, size_t y);

        size_t get_width();
        size_t get_height();

        volatile uint32_t* get_fb_ptr();
    private:
        limine_framebuffer* buffer;
        struct bitmap_font font;
};

#endif