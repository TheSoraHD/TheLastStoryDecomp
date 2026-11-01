#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/buffer_io.h"
#include <stdio.h>

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __convert_from_newlines(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void __convert_to_newlines(){
}
#endif

void __prep_buffer(FILE* file)
{
    file->buffer_ptr = file->buffer;
    file->buffer_len = file->buffer_size;
    file->buffer_len = file->buffer_len - (file->position & file->buffer_alignment);
    file->buffer_pos = file->position;
    return;
}

int __load_buffer(FILE* file, size_t* bytes_loaded, int alignment)
{
    int ioresult;

    __prep_buffer(file);

    if (alignment == 1) {
        file->buffer_len = file->buffer_size;
    }

    ioresult = file->read_proc(file->handle, file->buffer, (size_t*)&file->buffer_len, file->ref_con);

    if (ioresult == 2) {
        file->buffer_len = 0;
    }

    if (bytes_loaded) {
        *bytes_loaded = file->buffer_len;
    }

    if (ioresult) {
        return ioresult;
    }

    file->position += file->buffer_len;

    if (!file->mode.binary_io) {
        int n = file->buffer_len;
        u8* p = file->buffer;

        while (n--) {
            if (*p++ == '\n') {
                file->position++;
            }
        }
    }

    return 0;
}

int __flush_buffer(FILE* file, size_t* length)
{
    size_t bufferLen;
    int writeCode;

    bufferLen = file->buffer_ptr - file->buffer;
    if (bufferLen) {
        file->buffer_len = bufferLen;
        writeCode = file->write_proc(file->handle, file->buffer, &file->buffer_len, file->ref_con);
        if (length) {
            *length = file->buffer_len;
        }
        if (writeCode) {
            return writeCode;
        }
        file->position += file->buffer_len;
    }

    file->buffer_ptr = file->buffer;
    file->buffer_len = file->buffer_size;
    file->buffer_len = file->buffer_len - (file->position & file->buffer_alignment);
    file->buffer_pos = file->position;
    return 0;
}

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void setvbuf(){
}
#endif

//unused
#if 0 // not in the BTE build of this game (its strings are not in main.dol)
void setbuf(){
}
#endif
