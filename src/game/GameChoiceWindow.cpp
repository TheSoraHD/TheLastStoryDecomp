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
