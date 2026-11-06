#ifndef GAME_LASTCOCOONEGG_H
#define GAME_LASTCOCOONEGG_H

#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: attacker record of the egg (0x18 bytes): the hits whose attacker (HitMsg_unk::mAttacker) matches are
// counted in m0C (LastCocoonEgg::vf40); names invented
class CharaTask;
struct LastCocoonEggPart_unk {
    CharaTask* m00;
    unsigned char m04[0x0C - 0x04];
    int m0C;
    unsigned char m10[0x18 - 0x10];
};

// inferred: shake of the egg (same fields and update as the camera shake of the field, 0x96C); names
// invented
struct LastCocoonEggShake_unk {
    void Shake_unk(int frames, float x, float y) {
        m00 = m00 % 2;
        mFrames = frames;
        mX = x;
        mY = y;
    }
    int m00;
    int mFrames;
    float mX;
    float mY;
};

// inferred from __vt__13LastCocoonEgg (0x8078F630): egg form of the last cocoon; the enemy factory
// (fn_805A5374) allocates 0x2A50 bytes. Members keep their offsets (names unknown).
class LastCocoonEgg : public EnemyTask {
public:
    LastCocoonEgg(atn::Task* parent, EnemyInfo_unk* info); // 0x80452F14
    virtual ~LastCocoonEgg();                              // 0x80453570
    virtual void vf3C(HitInfo_unk* hit);                   // 0x804549E8
    virtual void vf40(HitInfo_unk* hit);                   // 0x804549F8
    virtual atn::Vector3 vf64(int center);                 // 0x80454D08
    void fn_80455D80(); // inferred: plays the effect m1928 (m1920 = 1)
    void fn_8045617C();
    void fn_80456DB8();
    void fn_80456F04();

    atn::LH_Binary m14B0; // inferred: parameter file
    unsigned char m14B8[0x14BC - 0x14B8];
    int m14BC;
    int m14C0;
    unsigned char m14C4[0x14D0 - 0x14C4];
    float m14D0;
    float m14D4;
    unsigned char m14D8[0x14EC - 0x14D8];
    atn::Model m14EC;
    unsigned char m18BC[0x1920 - 0x18BC];
    int m1920;
    int m1924;          // inferred: shield points (vf40)
    atn::EfpFile m1928;
    atn::EfpFile m1934; // inferred: hit effect while the shield holds
    atn::EfpFile m1940; // inferred: effect of the shield break
    int m194C;
    atn::EfpFile m1950;
    atn::EfpFile m195C;
    unsigned char m1968[0x1980 - 0x1968];
    int m1980;          // inferred: the shield is broken
    int m1984;
    atn::EfpFile m1988;
    atn::EffFile m1994;
    std::vector<LastCocoonEggPart_unk> m19A0;
    unsigned char m19AC[0x252C - 0x19AC];
    atn::EfpFile m252C;
    atn::EfpFile m2538;
    unsigned char m2544[0x2548 - 0x2544];
    LastCocoonEggShake_unk m2548;
    int m2558;
    float m255C;
    unsigned char m2560[0x2A40 - 0x2560];
    ResHandle m2A40; // inferred: debug menu group
    unsigned char m2A44[0x2A50 - 0x2A44];
};

#endif
