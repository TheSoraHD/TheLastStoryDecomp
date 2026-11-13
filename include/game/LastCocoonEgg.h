#ifndef GAME_LASTCOCOONEGG_H
#define GAME_LASTCOCOONEGG_H

#include "atn/LH_Binary.h"
#include "atn/Model_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"
#include "atn/RenderSetting_unk.h"

// inferred: attacker record of the egg (0x18 bytes): the hits whose attacker (HitMsg_unk::mAttacker) matches are
// counted in m0C (LastCocoonEgg::vf40); names invented
class CharaTask;
struct LastCocoonEggPart_unk {
    CharaTask* m00;
    float m04;
    int m08;
    int m0C;
    int m10;
    int m14; // inferred: motion id by character (0xB4-0xB8, vf0C)
};

// inferred: 0x5C-byte record of the egg (32 at 0x19AC; the constructor, weak copy 0x804533A8, only clears the
// first word); names invented
struct LastCocoonEggSeg_unk {
    LastCocoonEggSeg_unk() : m00(0) {}
    int m00;
    unsigned char m04[0x5C - 0x04];
};

// inferred: post effect settings of the egg (bloom and color matrix of the render settings; constructor
// 0x804533E4); name invented
struct LastCocoonEggPostFx_unk {
    BloomParam_unk mBloom;              // 0x00
    ColorMatrixParam_unk mColorMatrix;  // 0x80
};

// inferred: shake of the egg (same fields and update as the camera shake of the field, 0x96C); names
// invented
struct LastCocoonEggShake_unk {
    LastCocoonEggShake_unk() : m00(0), mFrames(0), mX(0.0f), mY(0.0f) {} // inferred: weak copy 0x804533B4
    // inferred: counts the frames of the shake (weak copy 0x80454538, called by LastCocoonEgg::vf10)
    void fn_80454538();
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
// inferred: called out of line by LastCocoonEgg::vf10 (over the inlining budget in the original)
__declspec(noinline) inline void LastCocoonEggShake_unk::fn_80454538() {
    if (m00 < mFrames) {
        m00++;
    }
}

// inferred from __vt__13LastCocoonEgg (0x8078F630): egg form of the last cocoon; the enemy factory
// (fn_805A5374) allocates 0x2A50 bytes. Members keep their offsets (names unknown).
class LastCocoonEgg : public EnemyTask {
public:
    LastCocoonEgg(atn::Task* parent, EnemyInfo_unk* info); // 0x80452F14
    virtual ~LastCocoonEgg();                              // 0x80453570
    virtual int vf0C();                                    // 0x8045374C
    virtual void vf10();                                   // 0x804541A0
    virtual void vf14();                                   // 0x8045456C
    virtual void vf3C(HitInfo_unk* hit);                   // 0x804549E8
    virtual void vf40(HitInfo_unk* hit);                   // 0x804549F8
    virtual atn::Vector3 vf64(int center);                 // 0x80454D08
    void fn_80455D80();
    void fn_80455E20(); // inferred: targets the farthest attacker (or the leader)
    void fn_80456194();
    void fn_804560A4(); // inferred: collision capsule scaled by m14D0 // inferred: aims and fires the bullet 0x5AA at the target // inferred: plays the effect m1928 (m1920 = 1)
    void fn_8045617C();
    void fn_80456DB8();
    void fn_80456F04();
    void fn_80456F94(); // inferred: the shield breaks
    void fn_80454D4C();
    void fn_80454F08();
    void fn_8045531C();
    void fn_80455638();
    void fn_80455F2C();
    void fn_804562E4();
    void fn_80456E1C(CharaTask* target);
    void fn_804570F4(CharaTask* target);
    void fn_804576D4();
    void fn_80457C48();
    void fn_8045817C(); // inferred: debug menu (called by vf0C)
    void fn_80457E98();

    atn::LH_Binary m14B0; // inferred: parameter file
    CharaTask* m14B8;     // inferred: target
    int m14BC;
    int m14C0;
    int m14C4;
    int m14C8;
    int m14CC;
    float m14D0;
    float m14D4;
    float m14D8;
    float m14DC;
    int m14E0;
    int m14E4; // inferred: debug action (vf10 runs fn_804576D4/fn_80457C48/fn_80457E98)
    int m14E8; // inferred: frame counter
    atn::Model m14EC;
    atn::Matrix3x4 m18BC;
    atn::Vector3 m18EC;
    atn::Vector3 m18F8;
    MtxPtr m1904; // inferred: node matrix "catch_point" (vf0C)
    CharaTask* m1908; // inferred: character caught by the egg (released by fn_80457C48)
    CharaTask* m190C; // inferred: last attacker that came near (vf10)
    int m1910;        // inferred: frames before m190C is forgotten
    atn::Vector3 m1914;
    int m1920;
    int m1924;          // inferred: shield points (vf40)
    atn::EfpFile m1928;
    atn::EfpFile m1934; // inferred: hit effect while the shield holds
    atn::EfpFile m1940; // inferred: effect of the shield break
    int m194C;
    atn::EfpFile m1950;
    atn::EfpFile m195C;
    atn::Vector3 m1968; // inferred: position of the shot
    atn::Vector3 m1974; // inferred: direction of the shot

    int m1980;          // inferred: the shield is broken
    int m1984;
    atn::EfpFile m1988;
    atn::EffFile m1994;
    std::vector<LastCocoonEggPart_unk> m19A0;
    LastCocoonEggSeg_unk m19AC[32];
    atn::EfpFile m252C;
    atn::EfpFile m2538;
    unsigned char m2544[0x2548 - 0x2544];
    LastCocoonEggShake_unk m2548;
    int m2558;
    float m255C;
    LastCocoonEggPostFx_unk m2560;
    LastCocoonEggPostFx_unk m2630;
    LastCocoonEggPostFx_unk m2700[4];
    ResHandle m2A40; // inferred: debug menu group
    int m2A44; // inferred: debug keys enabled
    float m2A48;
    int m2A4C;
};

#endif
