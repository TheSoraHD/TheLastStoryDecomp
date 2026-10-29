#include "atn/Task.h"
#include "atn/LoadHandle.h"
#include "game/GameVoice_unk.h"

void operator delete(void* ptr);

extern "C" void* fn_803E8908(void* object, int deleteFlag) {
    if (object != 0 && deleteFlag > 0) {
        operator delete(object);
    }
    return object;
}

int GameVoice::vf0C() {
    return fn_800D3FA4() != 0;
}

extern "C" void* lbl_8087F490;

extern "C" void* fn_803EAC20() {
    void* object = lbl_8087F490;
    return object != 0 ? *reinterpret_cast<void**>(static_cast<unsigned char*>(object) + 0x2658) : 0;
}

extern "C" int fn_800CB4EC(void* object);
extern "C" int fn_80049B74(void* object);
extern "C" void* lbl_8087EE90;
extern "C" unsigned int fn_800CB6F8(void* object);

extern "C" int fn_803EB234(void* self) {
    unsigned char* entry = static_cast<unsigned char*>(self) + 0x48;
    for (int i = 0; i < 4; ++i, entry += 0x30) {
        if (fn_800CB4EC(entry) == 0) {
            if (reinterpret_cast<atn::LoadHandle*>(entry + 0x10)->fn_80473F50() == 0) {
                continue;
            }
        }
        {
            return 1;
        }
    }
    return 0;
}

extern "C" int fn_803EB2A4(void* self, int enabled) {
    unsigned int key = enabled != 0 ? static_cast<unsigned int>(fn_80049B74(lbl_8087EE90)) : 0;
    unsigned char* entry = static_cast<unsigned char*>(self) + 0x48;
    for (int i = 0; i < 4; ++i, entry += 0x30) {
        if (entry[0x2C] != 0 && fn_800CB6F8(entry) == key) {
            return 1;
        }
    }
    return 0;
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

extern "C" int fn_800CB5C8(void* object, int arg1, int arg2);
extern "C" void* lbl_807C7030;
extern "C" volatile const float lbl_80885F4C = 0.0f;

// NONMATCHING: 72.67% - sound-reset loop is reconstructed; register and vector-copy codegen differs.
extern "C" int fn_803EB394(void* self, int enabled) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned int key = enabled != 0 ? static_cast<unsigned int>(fn_80049B74(lbl_8087EE90)) : 0;
    for (int i = 0; i < 4; ++i) {
        unsigned char* sound = object + 0x48 + i * 0x30;
        if (sound[0x2C] == 0 || fn_800CB6F8(sound) != key) {
            continue;
        }

        fn_800CB5C8(sound, 0, 0);
        void* core = *reinterpret_cast<void**>(sound + 0x18);
        if (core != 0 && *reinterpret_cast<void**>(sound + 4) != 0 &&
            *reinterpret_cast<void**>(static_cast<unsigned char*>(core) + 0x1154) == sound + 4) {
            *reinterpret_cast<void**>(static_cast<unsigned char*>(core) + 0x1154) = 0;
            *reinterpret_cast<float*>(static_cast<unsigned char*>(core) + 0x1158) = lbl_80885F4C;
        }
        reinterpret_cast<atn::LoadHandle*>(sound + 0x10)->fn_80473F88();
        *reinterpret_cast<void**>(sound + 0x18) = 0;
        float* source = static_cast<float*>(lbl_807C7030);
        float* destination = reinterpret_cast<float*>(sound + 0x1C);
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        *reinterpret_cast<int*>(sound + 0x28) = 0;
        sound[0x2C] = 0;
        sound[0x2D] = 0;
    }
    return 0;
}

extern "C" int fn_800CB58C(void* object);

extern "C" int fn_803EBA54(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* entry = bytes + 0x48;
    volatile int& count = *reinterpret_cast<volatile int*>(bytes + 0x10C);
    int i = 0;
    while (i < count) {
        if (fn_800CB58C(entry) == 0) {
            return 1;
        }
        entry += 0x30;
        ++i;
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

extern "C" int fn_803EBC04() {
    if (lbl_8087EE90 != 0) {
        return fn_80049B74(lbl_8087EE90) != 0;
    }
    return 0;
}
