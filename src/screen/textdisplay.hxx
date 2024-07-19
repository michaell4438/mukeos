#ifndef SCREEN_TEXTDISPLAY_H
#define SCREEN_TEXTDISPLAY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <screen/screen.hxx>

struct textbuffer_char {
    char c;
    uint32_t color;
    uint32_t bg_color;

    textbuffer_char() : c(' '), color(0), bg_color(0) {};
};

typedef struct textbuffer_char textbuffer_char;

/*
    * TextDisplay class
    * 
    * A wrapper around the Screen class that provides a higher-level interface for drawing text on the screen.
    * Important difference between Screen and TextDisplay is that TextDisplay uses character-based coordinates, while Screen uses pixel-based coordinates.
    * The screen width and height are calculated based on the font width and height.
*/
class TextDisplay {
    public:
        TextDisplay(Screen* screen, size_t font_width, size_t font_height);
        void clear(uint32_t color);
        void clear();

        void set_default_color(uint32_t color);
        uint32_t get_default_color();

        void set_default_bg_color(uint32_t color);
        uint32_t get_default_bg_color();

        void print(const char* str);
        void print(const char* str, uint32_t color, uint32_t bg_color);
        void itoa(int n, char* str, int base);

        void print(uint64_t n, int base, const char* prefix);

        void println(const char* str);
        void println(const char* str, uint32_t color, uint32_t bg_color);

        uint32_t get_color(uint8_t r, uint8_t g, uint8_t b);

        void set_cursor_pos(size_t x, size_t y);
        size_t get_cursor_x();
        size_t get_cursor_y();

        size_t get_width();
        size_t get_height();

        void scroll_up();
    private:
        Screen* screen;
        uint32_t default_color;
        uint32_t default_bg_color;
        size_t cursor_x;
        size_t cursor_y;

        size_t font_width;
        size_t font_height;

        size_t screen_width;
        size_t screen_height;

        void put_char(char c, uint32_t color, uint32_t bg_color, size_t x, size_t y);
};

void set_textdisplay_instance(TextDisplay* instance);

// declare a global instance of textdisplay
extern TextDisplay* textdisplay_instance;

#endif