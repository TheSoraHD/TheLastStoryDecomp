#ifndef GAME_COSTUMEEDITCHARA_H
#define GAME_COSTUMEEDITCHARA_H

#include "atn/Task.h"
#include "atn/Model_unk.h"
#include "math/Vec.h"

class CharaTask;

// inferred: d2anime task of the face icon ("data/d2anime/equip_face.d2a"); names invented
class CostumeEditFace_unk : public atn::Task {
public:
    unsigned char m48[0x4D - 0x48];
    unsigned char m4D;
    unsigned char m4E[0x50 - 0x4E];
    float m50;
    float m54;
};

// inferred from __vt__16CostumeEditChara (0x807828C8): character shown by the costume edit menu
// (game/CosEditBase.cpp), with its shadow model and face icon
class CostumeEditChara : public atn::Task {
public:
    CostumeEditChara(atn::Task* parent, CharaTask* chara, int face); // 0x801E807C
    virtual ~CostumeEditChara(); // 0x801E81DC
    virtual int vf0C();          // 0x801E8244
    virtual void vf10();         // 0x801E8534
    virtual void vf14();         // 0x801E8F9C

    static CostumeEditChara* fn_801E7FF8(atn::Task* parent, CharaTask* chara, int face); // inferred: creates it
    void fn_801E9164();                            // inferred: empty
    // inferred: the vectors are passed by value (game/CosEditBase.cpp copies them before the call)
    void fn_801E9168(atn::Vector3 pos);            // inferred: sets the target position
    void fn_801E917C(atn::Vector3 pos);            // inferred: sets the position at once
    // inferred: inline by-value setter (game/CosEditBase.cpp fn_801CE148 copies the argument to the stack)
    void SetCur_unk(atn::Vector3 pos) { mCur = pos; }
    void fn_801E91A0(atn::Vector3 rot);            // inferred: sets the rotation
    bool fn_801E91B4();                            // inferred: the character is close to its target
    void fn_801E9218(atn::Vector3 rot, atn::Vector3 scale); // inferred: shows it (0x801E9218, auto split)
    void fn_801E92D4(atn::Vector3 from, atn::Vector3 to);   // inferred: moves it (0x801E92D4, auto split)

    CharaTask* mChara;      // 0x48
    int mFace;              // 0x4C: inferred: passed to fn_804A4CE4
    atn::Matrix3x4 mMtx;    // 0x50
    atn::Vector3 mPos;      // 0x80
    atn::Vector3 mRot;      // 0x8C
    atn::Vector3 mScale;    // 0x98
    atn::Model mShadow;      // 0xA4: inferred: "/map/cosedit/hk_shadow1.model"
    CostumeEditFace_unk* mFaceIcon; // 0x474
    int mVisible;           // 0x478
    atn::Vector3 mTarget;   // 0x47C
    atn::Vector3 mCur;      // 0x488
    atn::Vector3 mOffset;   // 0x494
    atn::Vector3 mOffsetTarget; // 0x4A0
    int m4AC;
};

#endif
