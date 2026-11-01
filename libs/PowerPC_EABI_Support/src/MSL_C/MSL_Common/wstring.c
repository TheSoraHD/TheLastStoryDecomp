#include "PowerPC_EABI_Support/MSL_C/MSL_Common/wstring.h"
#include <string.h>

size_t wcslen(const wchar_t *str) {
    size_t len = -1;
    wchar_t* p = (wchar_t*)str - 1;

    do {
        len++;
    } while(*++p);

    return len;
}

//unused
void wcsnlen_s(){
}

wchar_t* wcscpy(wchar_t *dest, const wchar_t *src) {
    const wchar_t* p = (wchar_t*)src - 1;
    wchar_t* q = (wchar_t*)dest - 1;

    while (*++q = *++p);
    return dest;
}

//unused
void wcscpy_s(){
}

wchar_t* wcsncpy(wchar_t *dest, const wchar_t *src, size_t num) {
    const wchar_t* p = (const wchar_t*)src - 1;
    wchar_t* q = (wchar_t*)dest - 1;
    wchar_t zero = 0;

    num++;

    while (--num) {
        if (!(*++q = *++p)) {
            while (--num) {
                *++q = 0;
            }

            break;
        }
    }

    return dest;
}

//unused
void wcsncpy_s(){
}

wchar_t* wcscat(wchar_t *dest, const wchar_t *src) {
    const wchar_t* p = (wchar_t*)src - 1;
    wchar_t* q = (wchar_t*)dest - 1;

    while (*++q);

    q--;

    while (*++q = *++p);

    return dest;
}

//unused
void wcscat_s(){
}

//unused
void wcsncat(){
}

//unused
void wcsncat_s(){
}

int wcscmp(const wchar_t *str1, const wchar_t *str2) {
    const wchar_t* p1 = (wchar_t *)str1 - 1;
    const wchar_t* p2 = (wchar_t *)str2 - 1;
    wchar_t c1, c2;

    while ((c1 = *++p1) == (c2 = *++p2)) {
        if (!c1) {
            return 0;
        }
    }

    return c1 - c2;
}

int wcsncmp(const wchar_t *str1, const wchar_t *str2, size_t num) {
    const wchar_t* p1 = (wchar_t*)str1 - 1;
    const wchar_t* p2 = (wchar_t*)str2 - 1;
    wchar_t c1, c2;

    num++;

    while (--num) {
        if ((c1 = *++p1) != (c2 = *++p2)) {
            return (c1 - c2);
        } else if (!c1) {
            break;
        }
    }

    return 0;
}

wchar_t* wcschr(const wchar_t *str, const wchar_t chr) {
    const wchar_t* p = (wchar_t*)str - 1;
    wchar_t c = (chr & 0xFFFF);
    wchar_t ch;

    while (ch = (*++p& 0xFFFF)) {
        if (ch == c) {
            return ((wchar_t*)p);
        }
    }
    
    return (c ? 0 : (wchar_t*)p);
}

//unused
void wcscoll(){
}

//unused
void wcsxfrm(){
}

//unused
void wcsrchr(){
}

//unused
void wcspbrk(){
}

//unused
void wcsspn(){
}

//unused
void wcscspn(){
}

#define set_wchar_map(map, ch) map[(ch) >> 3] |= (1 << ((ch) & 7))
#define tst_wchar_map(map, ch) (map[(ch) >> 3] & (1 << ((ch) & 7)))

wchar_t* wcstok(wchar_t* str, const wchar_t* set, wchar_t** ptr)
{
    wchar_t* p;
    wchar_t* q;
    wchar_t* next;
    wchar_t* s;
    wchar_t c;
    static wchar_t* n = L"";
    char map[0x2000];

    memset(map, 0, sizeof(map));

    if (str) {
        s = str;
    } else if (*ptr) {
        s = *ptr;
    } else {
        return 0;
    }

    p = (wchar_t*)set - 1;
    while (c = *++p) {
        set_wchar_map(map, c);
    }

    p = s - 1;
    while (c = *++p) {
        if (!tst_wchar_map(map, c)) {
            break;
        }
    }

    if (!c) {
        *ptr = 0;
        return 0;
    }

    q = p;
    while (c = *++p) {
        if (tst_wchar_map(map, c)) {
            break;
        }
    }

    if (!c) {
        next = n;
    } else {
        *p = 0;
        next = p + 1;
    }

    if (q == 0) {
        *ptr = 0;
    } else {
        *ptr = next;
    }

    return q;
}

wchar_t* wcsstr(const wchar_t* str, const wchar_t* pat)
{
    wchar_t* s1 = (wchar_t*)str - 1;
    wchar_t* p1 = (wchar_t*)pat - 1;
    wchar_t firstc, c1, c2;

    if (!(firstc = *++p1)) {
        return (wchar_t*)str;
    }

    while ((c1 = *++s1)) {
        if (c1 == firstc) {
            const wchar_t* s2 = s1 - 1;
            const wchar_t* p2 = p1 - 1;

            while ((c1 = *(++s2)) == (c2 = *(++p2)) && c1);

            if (!c2) {
                return (wchar_t*)s1;
            }
        }
    }

    return 0;
}
