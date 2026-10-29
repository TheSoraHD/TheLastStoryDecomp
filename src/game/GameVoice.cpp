#include "atn/Task.h"

void operator delete(void* ptr);

extern "C" void* fn_803E8908(void* object, int deleteFlag) {
    if (object != 0 && deleteFlag > 0) {
        operator delete(object);
    }
    return object;
}

extern "C" int fn_803E8A24(atn::Task* self) {
    return self->fn_800D3FA4() != 0;
}

extern "C" void* lbl_8087F490;

extern "C" void* fn_803EAC20() {
    void* object = lbl_8087F490;
    return object != 0 ? *reinterpret_cast<void**>(static_cast<unsigned char*>(object) + 0x2658) : 0;
}

// NONMATCHING: 78.83% - the final pair of flag checks is folded into a boolean expression.
extern "C" int fn_803EB338(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (bytes[0x74] != 0) {
        return 1;
    }
    if (bytes[0xA4] != 0) {
        return 1;
    }
    bytes += 0x60;
    if (bytes[0x74] != 0) {
        return 1;
    }
    if (bytes[0xA4] != 0) {
        return 1;
    }
    return 0;
}

extern "C" void fn_803EBBCC(void* self, void* value) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    volatile unsigned int& count = *reinterpret_cast<volatile unsigned int*>(bytes + 0x394);
    unsigned char* entry = bytes + 0x398;
    unsigned int i = 0;
    while (i < count) {
        if (*reinterpret_cast<void**>(entry + 8) == value) {
            *reinterpret_cast<void**>(entry + 8) = 0;
        }
        entry += 0xE0;
        ++i;
    }
}
