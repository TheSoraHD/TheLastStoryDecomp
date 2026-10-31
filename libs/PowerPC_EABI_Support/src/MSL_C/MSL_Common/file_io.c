#include "PowerPC_EABI_Support/MSL_C/MSL_Common/buffer_io.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/ansi_files.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//unused
void remove(){
}

//unused
void rename(){
}

//unused
void tmpnam(){
}

//unused
void tmpnam_s(){
}

//unused
void tmpfile(){
}

//unused
void tmpfile_s(){
}

int fclose(FILE* file)
{
    int flush_result, close_result;

    if (file == nullptr)
        return (-1);
    if (file->mode.file_kind == __closed_file)
        return (0);

    flush_result = fflush(file);

    close_result = (*file->close_proc)(file->handle);

    file->mode.file_kind = __closed_file;
    file->handle = 0;

    if (file->state.free_buffer)
        free(file->buffer);
    return ((flush_result || close_result) ? -1 : 0);
}

int fflush(FILE* file)
{
    int pos;

    if (file == nullptr) {
        return __flush_all();
    }

    if (file->state.error != 0 || file->mode.file_kind == __closed_file) {
        return -1;
    }

    if (file->mode.io_mode == 1) {
        return 0;
    }

    if (file->state.io_state >= 3) {
        file->state.io_state = 2;
    }

    if (file->state.io_state == 2) {
        file->buffer_len = 0;
    }

    if (file->state.io_state != 1) {
        file->state.io_state = 0;
        return 0;
    }

    if (file->mode.file_kind != __disk_file) {
        pos = 0;
    } else {
        pos = ftell(file);
    }

    if (__flush_buffer(file, 0) != 0) {
        file->state.error = 1;
        file->buffer_len = 0;
        return -1;
    }

    file->state.io_state = 0;
    file->position = pos;
    file->buffer_len = 0;
    return 0;
}

//unused
void fopen(){
}

//unused
void fopen_s(){
}

//unused
void freopen(){
}

//unused
void freopen_s(){
}

//unused
void __reopen(){
}

//unused
void __handle_open(){
}

//unused
void __handle_reopen(){
}

//unused
void __get_file_modes(){
}

//unused
void __set_ref_con(){
}

//unused
void __set_idle_proc(){
}

int __msl_strnicmp(const char* s1, const char* s2, int n)
{
    int i;
    char c1, c2;

    for (i = 0; i < n; i++) {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);

        if (c1 < c2) {
            return -1;
        }
        if (c1 > c2) {
            return 1;
        }
        if (c1 == 0) {
            return 0;
        }
    }
    return 0;
}

char* __msl_strrev(char* str)
{
    int SmallIndex = 0;
    int BigIndex = strlen(str) - 1;
    char Temp;

    while (SmallIndex < BigIndex) {
        Temp = str[SmallIndex];
        str[SmallIndex++] = str[BigIndex];
        str[BigIndex--] = Temp;
    }

    return str;
}

char* __msl_itoa(int val, char* str, int radix)
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

    return __msl_strrev(str);
}

//unused
void __msl_strdup(){
}
