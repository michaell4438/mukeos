#include <screen/textdisplay.hxx>

TextDisplay::TextDisplay(Screen* screen, size_t font_width, size_t font_height) {
    this->screen = screen;
    this->default_color = 0xffffff;
    this->default_bg_color = 0;
    this->cursor_x = 0;
    this->cursor_y = 0;
    this->font_width = font_width;
    this->font_height = font_height;
    this->screen_width = screen->get_width() / font_width;
    this->screen_height = screen->get_height() / font_height;
}

void TextDisplay::clear(uint32_t color) {
    for (size_t y = 0; y < screen_height; y++) {
        for (size_t x = 0; x < screen_width; x++) {
            put_char(' ', color, color, x, y);
        }
    }
}

void TextDisplay::clear() {
    clear(0);
}

void TextDisplay::put_char(char c, uint32_t color, uint32_t bg_color, size_t x, size_t y) {
    if (x >= screen_width || y >= screen_height) {
        return;
    }
    screen->put_char(c, color, bg_color, x * font_width, y * font_height);
}

void TextDisplay::set_default_color(uint32_t color) {
    default_color = color;
}

uint32_t TextDisplay::get_default_color() {
    return default_color;
}

void TextDisplay::set_default_bg_color(uint32_t color) {
    default_bg_color = color;
}

uint32_t TextDisplay::get_default_bg_color() {
    return default_bg_color;
}

void TextDisplay::print(const char* str) {
    print(str, default_color, default_bg_color);
}

void TextDisplay::print(const char* str, uint32_t color, uint32_t bg_color) {
    size_t i = 0;
    while (str[i]) {
        if (str[i] == '\n' || cursor_x + 1 >= screen_width) {
            cursor_x = 0;
            cursor_y++;
            if (str[i] == '\n') {
                i++;
                continue;
            }
            if (cursor_y >= screen_height) {
                scroll_up();
                cursor_y--;
            }
        }
        put_char(str[i], color, bg_color, cursor_x, cursor_y);
        i++;
        cursor_x++;
    }
}

void TextDisplay::println(const char* str) {
    println(str, default_color, default_bg_color);
}

void TextDisplay::println(const char* str, uint32_t color, uint32_t bg_color) {
    print(str, color, bg_color);
    cursor_x = 0;
    cursor_y++;
    if (cursor_y >= screen_height) {
        scroll_up();
        cursor_y--;
    }
}

void TextDisplay::set_cursor_pos(size_t x, size_t y) {
    cursor_x = x;
    cursor_y = y;
}

size_t TextDisplay::get_cursor_x() {
    return cursor_x;
}

size_t TextDisplay::get_cursor_y() {
    return cursor_y;
}

size_t TextDisplay::get_width() {
    return screen_width;
}

size_t TextDisplay::get_height() {
    return screen_height;
}

void TextDisplay::scroll_up() {
    // move all pixels up by (font_height) pixels
    volatile uint32_t *fb_ptr = screen->get_fb_ptr();
    for (size_t y = font_height; y < screen->get_height() - 1; y++) {
        for (size_t x = 0; x < screen->get_width(); x++) {
            fb_ptr[(y - font_height) * (screen->get_width()) + x] = fb_ptr[y * (screen->get_width()) + x];
        }
    }
}

uint32_t TextDisplay::get_color(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 16) | (g << 8) | b;
}

TextDisplay* textdisplay_instance;

void set_textdisplay_instance(TextDisplay* instance) {
    textdisplay_instance = instance;
}

void TextDisplay::itoa(int n, char* str, int base) {
    int i = 0;
    bool is_negative = false;
    if (n < 0) {
        is_negative = true;
        n = -n;
    }
    while (n) {
        int rem = n % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / base;
    }
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void TextDisplay::print(uint64_t n, int base, const char* prefix) {
    char str[23];
    itoa(n, str, base);
    print(prefix);
    print(str);
}