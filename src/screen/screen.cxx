#include <screen/screen.hxx>

Screen::Screen(limine_framebuffer buffer, struct bitmap_font font) {
    this->buffer = buffer;
    this->default_color = 0xffffff;
    this->cursor_x = 0;
    this->cursor_y = 0;
    this->font = font;
}

void Screen::clear(uint32_t color) {
    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t*>(this->buffer.address);
    for (size_t i = 0; i < this->buffer.width * this->buffer.height; i++) {
        fb_ptr[i] = color;
    }
}

void Screen::clear() {
    this->clear(this->default_color);
}

void Screen::put_char(char c, uint32_t color, size_t x, size_t y) {
    if (c < this->font.Chars) {
        const unsigned char *bitmap = this->font.Bitmap + this->font.Index[c];
        const unsigned char *widths = this->font.Widths;
        for (size_t i = 0; i < this->font.Height; i++) {
            for (size_t j = 0; j < this->font.Widths[this->font.Index[c]]; j++) {
                if (bitmap[i] & (1 << (7 - j))) {
                    volatile uint32_t *fb_ptr = static_cast<volatile uint32_t*>(this->buffer.address);
                    fb_ptr[(y + i) * this->buffer.width + x + j] = color;
                }
            }
            bitmap++;
        }
    }
}

size_t Screen::put_string(const char* str, uint32_t color, size_t x, size_t y) {
    size_t i = 0;
    while (str[i]) {
        if (str[i] == '\n' || x + this->font.Widths[this->font.Index[str[i]]] >= this->buffer.width) {
            x = 0;
            y += this->font.Height;
            i++;
            continue;
        }
        this->put_char(str[i], color, x, y);
        x += this->font.Widths[this->font.Index[str[i]]];
        i++;
    }
    return i;
}

void Screen::set_default_color(uint32_t color) {
    this->default_color = color;
}

uint32_t Screen::get_default_color() {
    return this->default_color;
}

void Screen::print(const char* str) {
    this->cursor_x += this->put_string(str, this->default_color, this->cursor_x, this->cursor_y);
}

void Screen::print(const char* str, uint32_t color) {
    this->cursor_x += this->put_string(str, color, this->cursor_x, this->cursor_y);
}

void Screen::set_cursor_pos(size_t x, size_t y) {
    this->cursor_x = x;
    this->cursor_y = y;
}

size_t Screen::get_cursor_x() {
    return this->cursor_x;
}

size_t Screen::get_cursor_y() {
    return this->cursor_y;
}