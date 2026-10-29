#include "game/MapPartsGimmickLoc_unk.h"

void* MapPartsGimmickLoc::fn_80498E7C(int index) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(this);
    if (*reinterpret_cast<unsigned int*>(bytes) == 0x6C6F6373) {
        unsigned char* entries = *reinterpret_cast<unsigned char**>(bytes + 0x18);
        return entries + index * 0x3C;
    }
    unsigned char* entries = *reinterpret_cast<unsigned char**>(bytes + 0x18);
    return entries + index * 0x30;
}

void MapPartsGimmickLoc::fn_80498EAC(const atn::Vector3& value) {
    m6C = value;
}

void MapPartsGimmickLoc::fn_80498EC0(const atn::Vector3& value) {
    m78 = value;
}

void MapPartsGimmickLoc::fn_80498ED4(const atn::Vector3& value) {
    m90 = value;
}

void MapPartsGimmickLoc::fn_80498EE8(int value) {
    m58 = value;
}

void MapPartsGimmickLoc::fn_80498EF0(int value) {
    mA4 = value;
}

extern "C" void PSMTXScale(void* matrix, float x, float y, float z);

void MapPartsGimmickLoc::fn_80498EF8(const atn::Vector3& scale) {
    PSMTXScale(this, scale.x, scale.y, scale.z);
}

void MapPartsGimmickLoc::fn_80498F08() {}

void MapPartsGimmickLoc::fn_80498F0C(int value) {
    *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(this) + 0x7C) = value;
}

void MapPartsGimmickLoc::fn_80498F4C() {}
