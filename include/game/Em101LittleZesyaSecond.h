#ifndef GAME_EM101LITTLEZESYASECOND_H
#define GAME_EM101LITTLEZESYASECOND_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__22Em101LittleZesyaSecond (0x80784AC8): second fight of the little Zesya.
// Only the small functions are written (game/Em101LittleZesyaSecond.cpp); the factory allocates
// 0x1558 bytes. Names invented.
class Em101LittleZesyaSecond : public EnemyTask {
public:
    Em101LittleZesyaSecond(atn::Task* parent, EnemyInfo_unk* info); // 0x8025FE60
    virtual ~Em101LittleZesyaSecond();                               // 0x80262840
    virtual int vf0C();                                              // 0x80260218
    virtual int vfA8(int kind);                                      // 0x802627E8

    void fn_80260324(); // inferred: reads the data file

    atn::LH_Binary m14B0;
    int m14B8;
    int m14BC;
    int m14C0;
    int m14C4;
    int m14C8;
    int m14CC;
    int m14D0;
    int m14D4;
    int m14D8;
    unsigned char m14DC[0x14E8 - 0x14DC];
    int m14E8;
    float m14EC[13];
    atn::EfpHandle_unk m1520;
    atn::EfpHandle_unk m152C;
    atn::EfpHandle_unk m1538;
    unsigned char m1544[0x1548 - 0x1544];
    int m1548;
    unsigned char m154C[0x1550 - 0x154C];
    ResHandle m1550; // inferred: debug menu group
    int m1554;
};

#endif
