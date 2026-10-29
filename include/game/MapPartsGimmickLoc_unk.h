#ifndef GAME_MAPPARTSGIMMICKLOC_UNK_H
#define GAME_MAPPARTSGIMMICKLOC_UNK_H

#include "game/MapPartsBase.h"

// inferred: three-word vector view of 8-byte records in MapPartsGimmickLoc's temporary CSV data.
struct MapPartsGimmickLocArray8_unk {
    bool fn_80499684() const;
    unsigned int fn_80499694() const;
    void* fn_80499724(int index) const;

    void* mData;
    unsigned int mSize;
    unsigned int mCapacity;
};

// inferred: three-word vector view of pointer entries in the same CSV parser.
struct MapPartsGimmickLocArray4_unk {
    void* fn_8049981C(int index) const;

    void* mData;
    unsigned int mSize;
    unsigned int mCapacity;
};

// inferred: map-part location controller (RTTI at 0x80790560; fields are established by local
// offset accessors in game/MapPartsGimmickLoc.cpp).
class MapPartsGimmickLoc : public MapPartsBase {
public:
    void* fn_80498E7C(int index);
    void fn_80498EAC(const atn::Vector3& value);
    void fn_80498EC0(const atn::Vector3& value);
    void fn_80498ED4(const atn::Vector3& value);
    void fn_80498EE8(int value);
    void fn_80498EF0(int value);
    void fn_80498EF8(const atn::Vector3& scale);
    void fn_80498F08();
    void fn_80498F0C(int value);
    void fn_80498F4C();
    void fn_80499388(int value);

    unsigned char m50[8];
    int m58;
    unsigned char m5C[0x10];
    atn::Vector3 m6C;
    atn::Vector3 m78;
    unsigned char m84[0x0C];
    atn::Vector3 m90;
    unsigned char m9C[8];
    int mA4;
};

#endif
