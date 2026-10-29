extern "C" void fn_803B3AA0(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<int*>(bytes + 0x00) = 0;
    *reinterpret_cast<int*>(bytes + 0x04) = 0;
    *reinterpret_cast<int*>(bytes + 0x08) = 0;
    *reinterpret_cast<int*>(bytes + 0x0C) = 0;
    bytes[0x10] = 0;
    *reinterpret_cast<int*>(bytes + 0x30) = 0;
    *reinterpret_cast<int*>(bytes + 0x34) = 0;
}

extern "C" int fn_803B4C24(void* self) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 0x78) == 3;
}

extern "C" volatile const float lbl_80885C40 = -1.5f;

// inferred: the mode is at +0x50 and the associated task pointer at +0x48.
extern "C" void fn_803B57B0(void* self, int enabled) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (enabled == 0) {
        *reinterpret_cast<int*>(bytes + 0x50) = 3;
        unsigned char* task = *reinterpret_cast<unsigned char**>(bytes + 0x48);
        *reinterpret_cast<float*>(task + 0x54) = lbl_80885C40;
    } else {
        *reinterpret_cast<int*>(bytes + 0x50) = 0;
        unsigned char* task = *reinterpret_cast<unsigned char**>(bytes + 0x48);
        *reinterpret_cast<unsigned int*>(task + 0x38) |= 4;
    }
}

extern "C" void fn_803B57EC(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    unsigned char* task = *reinterpret_cast<unsigned char**>(bytes + 0x54);
    *reinterpret_cast<int*>(task + 0x78) = 4;
    *reinterpret_cast<int*>(task + 0x74) = 0;
    int mode = *reinterpret_cast<int*>(bytes + 0x50);
    int shouldSet = 1;
    if (mode != 2 && mode != 4) {
        shouldSet = 0;
    }
    if (shouldSet == 0) {
        return;
    }
    *reinterpret_cast<int*>(bytes + 0x50) = 2;
}

extern "C" void fn_803B5974(void* self, int arg1, int arg2);

extern "C" void fn_803B5830(void* self, int arg1, int arg2) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    *reinterpret_cast<int*>(bytes + 0x4C) = arg1;
    *reinterpret_cast<int*>(bytes + 0x58) = arg2;
    fn_803B5974(self, arg1, arg2);
}

extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* object, int arg1, int arg2);

extern "C" int fn_803B5950(void* self) {
    if (*reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 0x50) == 2) {
        return fn_800A555C(lbl_8087EF70, 0, 4);
    }
    return 0;
}

// inferred: a 64-entry FIFO stores values from the head offset at +0x1B8.
extern "C" void fn_803B78B4(void* self, int value) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    volatile unsigned int& count = *reinterpret_cast<unsigned int*>(bytes + 0xB4);
    if (count < 0x40) {
        unsigned int index = (*reinterpret_cast<unsigned int*>(bytes + 0x1B8) + count) & 0x3F;
        *reinterpret_cast<int*>(bytes + 0xB8 + index * 4) = value;
        ++count;
    }
}

extern "C" void* lbl_8087F430;
extern "C" void fn_80370AE4(void* manager, int sound, int flags);
extern "C" void fn_80370320(void* manager, int sound, int flags, int arg3);

// NONMATCHING: 98.48% - manager pointer register allocation differs at both sound calls.
extern "C" void fn_803B78E8(void* self) {
    unsigned char* bytes = static_cast<unsigned char*>(self);
    if (bytes[0x71] == 0) {
        return;
    }
    bytes[0x71] = 0;
    register void* manager = lbl_8087F430;
    if (manager == 0) {
        return;
    }

    int mode = *reinterpret_cast<int*>(bytes + 0x74);
    if (mode == 0) {
        fn_80370AE4(manager, *reinterpret_cast<int*>(bytes + 0x78), 1);
    } else if (mode == 1) {
        fn_80370320(manager, *reinterpret_cast<int*>(bytes + 0x78), 1, 0);
    }
}
