#include <wchar.h>
#include <string.h>

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n){
    memcpy(dest,src,n * sizeof(wchar_t));
}

//unused
void wmemcpy_s(){
}

wchar_t* wmemmove(wchar_t* dest, const wchar_t* src, size_t n){
    return (wchar_t*)memmove(dest,src,n * sizeof(wchar_t));
}

//unused
void wmemmove_s(){
}

//unused
void wmemset(){
}

wchar_t* wmemchr(wchar_t* s, wchar_t c, int n) {
    s32 i;

    for(i = 0; i != n; s++, i++){
        if(*s == c) return s;
    }

    return 0;
}

int wmemcmp(const wchar_t* s1, const wchar_t* s2, int n) {
    int diff = 0;
    s32 i;

    for(i = 0; i != n; i++){
        diff = *s1 - *s2;
        if(diff != 0) break;
        s1++;
        s2++;
    }

    return diff;
}
