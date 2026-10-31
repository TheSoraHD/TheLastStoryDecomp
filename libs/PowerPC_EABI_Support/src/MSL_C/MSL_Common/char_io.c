#include <types.h>

#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include <wchar.h>
#include <stdio.h>

/* inferred: MSL char_io.c; only the functions present in this build
   (__get_char, ungetc, __ungetc) are written. */

int __load_buffer(FILE* file, size_t* bytes_loaded, int alignment);
int __ungetc(int c, FILE* file);

int __get_char(FILE* file)
{
    int ioresult;
    int state;

    file->buffer_len = 0;

    if (file->state.error || file->mode.file_kind == 0) {
        return -1;
    }

    state = file->state.io_state;
    if (state == 1 || !(file->mode.io_mode & 1)) {
        file->state.error = 1;
        file->buffer_len = 0;
        return -1;
    }

    if (state >= 3) {
        file->state.io_state = state - 1;
        if (state == 3) {
            file->buffer_len = file->saved_buffer_len;
        }
        return file->ungetc_buffer[state - 3];
    }

    file->state.io_state = 2;
    ioresult = __load_buffer(file, 0, 0);
    if (ioresult || file->buffer_len == 0) {
        if (ioresult == 1) {
            file->state.error = 1;
            file->buffer_len = 0;
        } else {
            file->state.io_state = 0;
            file->state.eof = 1;
            file->buffer_len = 0;
        }
        return -1;
    }

    file->buffer_len--;
    return *file->buffer_ptr++;
}

int ungetc(int c, FILE* file)
{
    return __ungetc(c, file);
}

int __ungetc(int c, FILE* file)
{
    int state = file->state.io_state;

    if (fwide(file, -1) >= 0) {
        return -1;
    }

    if (state == 1 || state == 4 || c == -1) {
        return -1;
    }

    if (state < 3) {
        file->saved_buffer_len = file->buffer_len;
        file->buffer_len = 0;
        file->state.io_state = 3;
        state = file->state.io_state;
    } else {
        file->state.io_state++;
        state = file->state.io_state;
    }

    file->ungetc_buffer[state - 3] = c;
    file->state.eof = 0;
    return (unsigned char)c;
}
