#include <stdarg.h>

// Wide string formatting of the game texts (a reduced swprintf: %s %c %b %o %x %d %f %%, flags '+'
// and ' ', width/precision digits). Name of the file invented.

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wcschr(const wchar_t* s, wchar_t c);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void* memmove(void* dst, const void* src, unsigned long n);
extern "C" wchar_t* fn_80686AC4(wchar_t* dst, const wchar_t* src);  // inferred: wcscat
extern "C" int fn_800DC1DC(const wchar_t* s);                       // inferred: wide string to int
extern "C" int fn_8006BA30();                                       // inferred: language of the game
extern "C" wchar_t* itow(int value, wchar_t* buf, int radix);

// inferred: pads the number `str` to the width written in `width` (the buffer is reused)
extern "C" wchar_t* fn_800DD1EC(wchar_t* width, wchar_t* str) {
    int len = wcslen(str);
    int n = fn_800DC1DC(width);
    if (n > len) {
        bool zero = width[0] == L'0';
        wchar_t* p = width;
        int count = n - len;
        for (int i = 0; i < count; i++) {
            if (zero) {
                *p = L'0';
            } else {
                *p = L' ';
            }
            p++;
        }
        width[count] = 0;
        fn_80686AC4(width, str);
        str = width;
    }
    return str;
}

// inferred: pads and cuts the decimal number `str` following "width.precision" in `width`
extern "C" wchar_t* fn_800DD2A8(wchar_t* width, wchar_t* str) {
    // inferred: languages 2-5 use a decimal comma
    wchar_t point = (unsigned int)(fn_8006BA30() - 2) <= 3 ? L',' : L'.';
    int len = wcslen(str);
    wchar_t* frac = wcschr(str, point);
    if (frac != 0) {
        len -= wcslen(frac);
    }
    int fracLen = frac != 0 ? wcslen(frac + 1) : 0;
    wchar_t* dot = wcschr(width, L'.');
    int precision = dot != 0 ? fn_800DC1DC(dot + 1) : 5;
    int n = width[0] != L'.' ? fn_800DC1DC(width) : len;
    if (precision < fracLen) {
        bool zero = width[0] == L'0';
        wchar_t* p = width;
        int count = n - len;
        for (int i = 0; i < count; i++) {
            if (zero) {
                *p = L'0';
            } else {
                *p = L' ';
            }
            p++;
        }
        width[count] = 0;
        fn_80686AC4(width, str);
        width[wcslen(str) - (fracLen - precision)] = 0;
        str = width;
    }
    return str;
}

// inferred: formats `format` into `buf`; returns the number of characters
// NONMATCHING: 99.39% - only register allocation: `text`, `num` and `number` get r29/r24-r26
// instead of r24/r25/r26 (the live ranges of text/num are split into several registers here)
extern "C" int fn_800DD3FC(wchar_t* buf, const wchar_t* format, ...) {
    // inferred: C-style declarations at the top (their order gives the registers of the original)
    va_list args;
    wchar_t tmp[0x40];
    wchar_t width[0x40];
    wchar_t* number;
    int num;
    wchar_t* text;
    wchar_t* widthEnd;
    const wchar_t* src;
    wchar_t* out;
    int plus;
    int space;
    int widthLen;

    va_start(args, format);
    number = tmp;
    src = format;
    out = buf;
    while (*src != 0) {
        if (*src != L'%') {
            *out++ = *src++;
            continue;
        }
        widthEnd = width;
        text = 0;
        plus = 0;
        space = 0;
        widthLen = 0;
        src++;
        while (text == 0) {
            wchar_t c = *src;
            switch (c) {
            case L'%':
                tmp[0] = L'%';
                tmp[1] = 0;
                text = tmp;
                break;
            case L's':
                text = va_arg(args, wchar_t*);
                break;
            case L'c':
                tmp[0] = va_arg(args, wchar_t);
                tmp[1] = 0;
                text = tmp;
                break;
            case L'b':
                text = itow(va_arg(args, int), tmp, 2);
                if (widthLen > 0) {
                    text = fn_800DD1EC(width, text);
                }
                break;
            case L'o':
                text = itow(va_arg(args, int), tmp, 8);
                if (widthLen > 0) {
                    text = fn_800DD1EC(width, text);
                }
                break;
            case L'x':
                text = itow(va_arg(args, int), tmp, 16);
                if (widthLen > 0) {
                    text = fn_800DD1EC(width, text);
                }
                break;
            case L'd':
                num = va_arg(args, int);
                text = itow(num, tmp, 10);
                if (widthLen > 0) {
                    memcpy(tmp, fn_800DD1EC(width, text), sizeof(tmp));
                    text = tmp;
                }
                if ((plus != 0 || space != 0) && num >= 0) {
                    memmove(tmp + 1, tmp, sizeof(tmp) - sizeof(wchar_t));
                    tmp[0] = plus != 0 ? L'+' : L' ';
                    plus = 0;
                    space = 0;
                }
                break;
            case L'f': {
                float value = va_arg(args, double);
                int integer = value;
                float fraction = __fabs((double)(10000000.0f * (value - integer)));
                num = fraction;
                if (num > 0) {
                    num += 5;
                }
                text = itow(integer, tmp, 10);
                int len = wcslen(text);
                text[len] = (unsigned int)(fn_8006BA30() - 2) <= 3 ? L',' : L'.';
                itow(num, &text[len + 1], 10);
                text = number;
                if (widthLen > 0) {
                    text = fn_800DD2A8(width, number);
                }
                break;
            }
            case L'.':
            case L'0':
            case L'1':
            case L'2':
            case L'3':
            case L'4':
            case L'5':
            case L'6':
            case L'7':
            case L'8':
            case L'9':
                src++;
                width[widthLen++] = c;
                *++widthEnd = 0;
                break;
            case L'+':
                if (src[-1] == L'%') {
                    plus = 1;
                }
                src++;
                break;
            case L' ':
                if (src[-1] == L'%') {
                    space = 1;
                }
                src++;
                break;
            default:
                tmp[0] = 0;
                text = tmp;
                break;
            }
        }
        src++;
        while (*text != 0) {
            *out++ = *text++;
        }
    }
    *out = 0;
    return out - buf;
}

// inferred: replaces "%s<n>" in `format` by the n-th of the `count` strings that follow
extern "C" int fn_800DD8A8(wchar_t* buf, const wchar_t* format, int count, ...) {
    va_list args;
    va_start(args, count);
    wchar_t tmp[0x40];
    wchar_t* strings[0x20];
    wchar_t** p = strings;
    for (int i = 0; i < count; i++) {
        *p++ = va_arg(args, wchar_t*);
    }
    wchar_t* out = buf;
    while (*format != 0) {
        if (*format != L'%') {
            *out++ = *format++;
            continue;
        }
        wchar_t* text = 0;
        format++;
        while (text == 0) {
            switch (*format) {
            case L'%':
                tmp[0] = L'%';
                tmp[1] = 0;
                text = tmp;
                break;
            case L's':
                text = *(strings + *++format - L'0');
                break;
            default:
                tmp[0] = 0;
                text = tmp;
                break;
            }
        }
        format++;
        while (*text != 0) {
            *out++ = *text++;
        }
    }
    *out = 0;
    return out - buf;
}
