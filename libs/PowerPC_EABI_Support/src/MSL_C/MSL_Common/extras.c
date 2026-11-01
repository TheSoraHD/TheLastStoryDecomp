#include <types.h>
#include <ctype.h>
#include <wchar.h>

int __msl_strnicmp(const char* s1, const char* s2, int n);
char* __msl_itoa(int val, char* str, int radix);
size_t wcslen(const wchar_t* str);

//unused
void strdup(){
}

//unused
void strlwr(){
}

//unused
void ultoa(){
}

//unused
void gcvt(){
}

//unused
void heapmin(){
}

int stricmp(char* param_1, char* param_2)
{
    s8 a_var;
    s8 b_var;

    do {
        b_var = tolower(*param_1++);
        a_var = tolower(*param_2++);

        if (b_var < a_var) {
            return -1;
        }
        if (b_var > a_var) {
            return 1;
        }
    } while (b_var != 0);
    return 0;
}


char* strupr(char* str)
{
    char* p = str;

    while (*p) {
        *p = toupper(*p);
        p++;
    }

    return str;
}

int strnicmp(const char* s1, const char* s2, int n)
{
    return __msl_strnicmp(s1, s2, n);
}

//unused
void strdate(){
}

//unused
void strset(){
}

//unused
void strnset(){
}

//unused
void strspnp(){
}

//unused
void strncasecmp(){
}

//unused
void strcmpi(){
}

//unused
void strncmpi(){
}

int strcasecmp(char* param_1, char* param_2)
{
    s8 a_var;
    s8 b_var;

    do {
        b_var = tolower(*param_1++);
        a_var = tolower(*param_2++);

        if (b_var < a_var) {
            return -1;
        }
        if (b_var > a_var) {
            return 1;
        }
    } while (b_var != 0);
    return 0;
}

//unused
void stricoll(){
}

//unused
void strncoll(){
}

//unused
void strnicoll(){
}

char* itoa(int val, char* str, int radix)
{
    return __msl_itoa(val, str, radix);
}

//unused
void strrev(){
}

//unused
void filelength(){
}

//unused
void wtoi(){
}

//unused
void wcslwr(){
}

//unused
void wcsupr(){
}

//unused
void wcsicmp(){
}

//unused
void wcsnicmp(){
}

wchar_t* wcsrev(wchar_t* str)
{
    int SmallIndex = 0;
    int BigIndex = wcslen(str) - 1;
    wchar_t Temp;

    while (SmallIndex < BigIndex) {
        Temp = str[SmallIndex];
        str[SmallIndex] = str[BigIndex];
        str[BigIndex] = Temp;
        SmallIndex++;
        BigIndex--;
    }

    return str;
}

//unused
void wcsset(){
}

//unused
void wcsnset(){
}

//unused
void wcsspnp(){
}

//unused
void wcsdup(){
}

//unused
void wstrrev(){
}

//unused
void wcsicoll(){
}

//unused
void wcsncoll(){
}

//unused
void wcsnicoll(){
}

wchar_t* itow(int val, wchar_t* str, int radix)
{
    int is_neg = 0;
    int len = 0;
    unsigned int num;
    int digit;

    if (val < 0) {
        val = -val;
        is_neg = 1;
    }

    num = val;
    do {
        digit = num % radix;
        if (digit > 9) {
            str[len++] = digit + ('A' - 10);
        } else {
            str[len++] = digit + '0';
        }
    } while ((num /= radix) != 0);

    if (is_neg) {
        str[len++] = '-';
    }
    str[len] = 0;

    return wcsrev(str);
}

//unused
void watof(){
}

//unused
void asctime_r(){
}

//unused
void ctime_r(){
}

//unused
void localtime_r(){
}

//unused
void gmtime_r(){
}

//unused
void rand_r(){
}

//unused
void strerror_r(){
}

//unused
void strtok_r(){
}
