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
