#include <screen/screen.hxx>

Screen::Screen(limine_framebuffer* buffer, struct bitmap_font font) {
    this->buffer = buffer;
    this->font = font;
}

void Screen::clear(uint32_t color) {
    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t*>(this->buffer->address);
    for (size_t i = 0; i < this->buffer->width * this->buffer->height; i++) {
        fb_ptr[i] = color;
    }
}

void Screen::clear() {
    clear(0);
}

void Screen::put_char(char c, uint32_t color, size_t x, size_t y) {
    put_char(c, color, 0, x, y);
}

void Screen::put_char(char c, uint32_t color, uint32_t bg_color, size_t x, size_t y) {
    if (c < this->font.Chars) {
        const unsigned char *bitmap = this->font.Bitmap;
        const unsigned char *widths = this->font.Widths;
        size_t index;
        if (c == 0) index = 0;
        else index = (c - 31)*16;
        for (size_t i = 0; i < 16; i++) {
            for (size_t j = 0; j < 8; j++) {
                if (bitmap[i + index] & (1 << (7 - j))) {
                    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t*>(this->buffer->address);
                    size_t y_coord = y + i;
                    size_t x_coord = x + j;
                    size_t screen_pos = y_coord*(this->buffer->pitch / 4) + x_coord;
                    fb_ptr[screen_pos] = color;
                } else {
                    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t*>(this->buffer->address);
                    size_t y_coord = y + i;
                    size_t x_coord = x + j;
                    size_t screen_pos = y_coord*(this->buffer->pitch / 4) + x_coord;
                    fb_ptr[screen_pos] = bg_color;
                }
            }
        }
    }
}

size_t Screen::put_string(const char* str, uint32_t color, size_t x, size_t y) {
    size_t i = 0;
    while (str[i]) {
        if (str[i] == '\n' || x + this->font.Widths[this->font.Index[str[i]]] >= this->buffer->width) {
            x = 0;
            y += this->font.Height;
            i++;
            if (str[i] == '\n') {
                continue;
            }
        }
        this->put_char(str[i], color, x, y);
        x += this->font.Widths[this->font.Index[str[i]]];
        i++;
    }
    return i;
}

size_t Screen::get_width() {
    return this->buffer->width;
}

size_t Screen::get_height() {
    return this->buffer->height;
}

volatile uint32_t* Screen::get_fb_ptr() {
    return static_cast<volatile uint32_t*>(this->buffer->address);
}