#include <types.h>

#ifndef _MSL_WIDE_CHAR
#define _MSL_WIDE_CHAR
#endif

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/buffer_io.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/misc_io.h"


size_t __fread(void* pPtr, size_t memb_size, size_t num_memb, FILE* file);
size_t __fwrite(const void* pPtr, size_t memb_size, size_t num_memb, FILE* file);
int __load_buffer(FILE* file, size_t* bytes_loaded, int alignment);
int __flush_line_buffered_output_files(void);

size_t fread(void* pPtr, size_t memb_size, size_t num_memb, FILE* file) {
    return __fread(pPtr, memb_size, num_memb, file);
}

size_t __fread(void* pPtr, size_t memb_size, size_t num_memb, FILE* file) {
    unsigned char* cur_ptr;
    size_t num_bytes, rem_bytes, bytes_read;
    int res, buff;

    if (fwide(file, 0) == 0) {
        fwide(file, -1);
    }

    rem_bytes = memb_size * num_memb;

    if (rem_bytes == 0 || file->state.error || file->mode.file_kind == 0) {
        return 0;
    }

    buff = (!file->mode.binary_io || file->mode.buffer_mode == 2);

    if (file->state.io_state == 0 && file->mode.io_mode & 1) {
        file->state.io_state = 2;
        file->buffer_len = 0;
    }

    if (file->state.io_state < 2) {
        file->state.error = 1;
        file->buffer_len = 0;
        return 0;
    }

    if (file->mode.buffer_mode & 1) {
        if (__flush_line_buffered_output_files()) {
            file->state.error = 1;
            file->buffer_len = 0;
            return 0;
        }
    }

    cur_ptr = (unsigned char*)pPtr;
    bytes_read = 0;

    if (rem_bytes && file->state.io_state >= 3) {
        do {
            if (fwide(file, 0) == 1) {
                *(wchar_t*)cur_ptr = file->ungetwc_buffer[file->state.io_state - 3];
                cur_ptr += sizeof(wchar_t);
                bytes_read += sizeof(wchar_t);
                rem_bytes -= sizeof(wchar_t);
            } else {
                *cur_ptr = file->ungetc_buffer[file->state.io_state - 3];
                cur_ptr++;
                bytes_read++;
                rem_bytes--;
            }
            file->state.io_state--;
        } while (rem_bytes && file->state.io_state >= 3);

        if (file->state.io_state == 2) {
            file->buffer_len = file->saved_buffer_len;
        }
    }

    if (rem_bytes && (file->buffer_len || buff)) {
        do {
            if (!file->buffer_len) {
                res = __load_buffer(file, 0, 0);
                if (res) {
                    if (res == 1) {
                        file->state.error = 1;
                        file->buffer_len = 0;
                    } else {
                        file->state.io_state = 0;
                        file->state.eof = 1;
                        file->buffer_len = 0;
                    }
                    rem_bytes = 0;
                    break;
                }
            }

            num_bytes = file->buffer_len;
            if (num_bytes > rem_bytes) {
                num_bytes = rem_bytes;
            }

            memcpy(cur_ptr, file->buffer_ptr, num_bytes);
            cur_ptr += num_bytes;
            bytes_read += num_bytes;
            rem_bytes -= num_bytes;
            file->buffer_ptr += num_bytes;
            file->buffer_len -= num_bytes;
        } while (rem_bytes && buff);
    }

    if (rem_bytes && !buff) {
        unsigned char* save_buf = (unsigned char*)file->buffer;
        size_t save_size = file->buffer_size;

        file->buffer = cur_ptr;
        file->buffer_size = rem_bytes;

        res = __load_buffer(file, &num_bytes, 1);
        if (res) {
            if (res == 1) {
                file->state.error = 1;
                file->buffer_len = 0;
            } else {
                file->state.io_state = 0;
                file->state.eof = 1;
                file->buffer_len = 0;
            }
        }

        bytes_read += num_bytes;
        file->buffer = save_buf;
        file->buffer_size = save_size;
        __prep_buffer(file);
        file->buffer_len = 0;
    }

    return bytes_read / memb_size;
}

size_t fwrite(const void* pPtr, size_t memb_size, size_t num_memb, FILE* file) {
    return __fwrite(pPtr, memb_size, num_memb, file);
}

size_t __fwrite(const void* pPtr, size_t memb_size, size_t num_memb, FILE* file) {
    unsigned char* cur_ptr;
    size_t num_bytes, rem_bytes, bytes_written;
    int res, buff;

    if (fwide(file, 0) == 0) {
        fwide(file, -1);
    }

    rem_bytes = memb_size * num_memb;

    if (rem_bytes == 0 || file->state.error || file->mode.file_kind == 0) {
        return 0;
    }

    if (file->mode.file_kind == 2) {
        __stdio_atexit();
    }

    buff = (!file->mode.binary_io || file->mode.buffer_mode == 2 || file->mode.buffer_mode == 1);

    if (file->state.io_state == 0 && file->mode.io_mode & 2) {
        if (file->mode.io_mode & 4) {
            if (_fseek(file, 0, 2)) {
                return 0;
            }
        }

        file->state.io_state = 1;
        __prep_buffer(file);
    }

    if (file->state.io_state != 1) {
        file->state.error = 1;
        file->buffer_len = 0;
        return 0;
    }

    cur_ptr = (unsigned char*)pPtr;
    bytes_written = 0;

    if (rem_bytes && (file->buffer_ptr != file->buffer || buff)) {
        file->buffer_len = file->buffer_size - (file->buffer_ptr - file->buffer);

        do {
            unsigned char* nw = 0;
            num_bytes = file->buffer_len;

            if (num_bytes > rem_bytes) {
                num_bytes = rem_bytes;
            }

            if (file->mode.buffer_mode == 1 && num_bytes) {
                if ((nw = (unsigned char*)__memrchr(cur_ptr, '\n', num_bytes)) != 0) {
                    num_bytes = nw + 1 - cur_ptr;
                }
            }

            if (num_bytes != 0) {
                memcpy(file->buffer_ptr, cur_ptr, num_bytes);
                cur_ptr += num_bytes;
                rem_bytes -= num_bytes;
                file->buffer_ptr += num_bytes;
                file->buffer_len -= num_bytes;
            }

            if (file->buffer_len == 0 || nw != 0 || (!file->mode.buffer_mode)) {
                res = __flush_buffer(file, 0);

                if (res != 0) {
                    file->state.error = 1;
                    file->buffer_len = 0;
                    rem_bytes = 0;
                    break;
                }
            }

            bytes_written += num_bytes;

        } while(rem_bytes && buff);
    }

    if (rem_bytes && buff == 0) {
        unsigned char* save_buf = (unsigned char*)file->buffer;
        size_t save_size = file->buffer_size;

        file->buffer = cur_ptr;
        file->buffer_size = rem_bytes;
        file->buffer_ptr = cur_ptr + rem_bytes;

        if (__flush_buffer(file, &num_bytes) != 0) {
            file->state.error = 1;
            file->buffer_len = 0;
        }
        else {
            bytes_written += num_bytes;
        }
        
        file->buffer = save_buf;
        file->buffer_size = save_size;
        __prep_buffer(file);
        file->buffer_len = 0;
    }

    if (file->mode.buffer_mode != 2) {
        file->buffer_len = 0;
    }

    return bytes_written / memb_size;
}
