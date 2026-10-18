#ifndef STD_MSL_COMMON_H
#define STD_MSL_COMMON_H

// Declarations shared by the MSL C++ reconstructions (std/string.h, std/vector.h).

void* operator new(unsigned long size);
void operator delete(void* p);

struct _FILE_unk {
    unsigned char m00[0x50];
};
extern "C" _FILE_unk __files[];
extern "C" int fprintf(_FILE_unk* file, const char* format, ...);
// inferred: abort is declared noreturn (0x80013F78 reuses sz - rlen computed only on the
// non-error path of an || condition)
extern "C" __declspec(noreturn) void abort();
extern "C" unsigned long strlen(const char* s);
extern "C" void* memmove(void* dst, const void* src, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int memcmp(const void* a, const void* b, unsigned long n);

// inferred: MSL reports errors with fprintf(stderr, ...) + abort() when exceptions are off. An
// inline function, not a macro: the message is loaded before the stream (0x800DB3EC, 0x80013F78)
inline void __msl_error(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

#endif
