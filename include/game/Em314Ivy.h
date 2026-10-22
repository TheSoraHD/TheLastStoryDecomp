#ifndef GAME_EM314IVY_H
#define GAME_EM314IVY_H

#include "atn/Array_unk.h"
#include "game/SpecialEnemyBase.h"

// inferred from __vt__8Em314Ivy (0x807883D0): ivy enemy that hides, appears near the player and
// catches it ("catch_point" node). Data file "db_em314_00_00.csv" (SkillId, HideWaitFrame, ...).
// Only the small functions are written (game/Em314Ivy.cpp). Names invented.
class Em314Ivy : public SpecialEnemyBase {
public:
    Em314Ivy(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em314Ivy();
    virtual int vf0C();                     // 0x803104D4
    virtual void vf10();                    // 0x80310EF8
    virtual void vf14();                    // 0x80311090
    virtual void vf30(void* menu);          // 0x8031348C
    virtual void vf40(HitInfo_unk* hit);    // 0x803111AC
    virtual int vfF8();                     // 0x803130DC
    virtual void vfFC();                    // 0x8031131C
    virtual void vf100(int state);          // 0x80311364
    virtual void vf104();                   // 0x80311BB8
    virtual void vf108();                   // 0x803123D0
    virtual int vf10C(struct HitMsg_unk* msg); // 0x80311130

    void fn_80310C64(const char* text, int length); // inferred: reads the data file
    void fn_80312EF8(); // inferred: picks the nearest player as target (states 9 and 10)
    void fn_8031319C(CharaTask* target); // inferred: catches `target`
    void fn_803132D0(); // inferred: releases the caught character

    float m14D4;
    float m14D8; // inferred: rotation from the spawn description
    float m14DC;
    float m14E0;
    CharaTask* m14E4; // inferred: caught character
    void* m14E8;      // inferred: matrix of the "catch_point" node
    int m14EC;        // inferred: "SkillId"
    int m14F0;        // inferred: "HideWaitFrame"
    int m14F4;        // inferred: "ReleaseWaitFrame"
    int m14F8;        // inferred: "HoldDamageInterval"
    float m14FC;      // inferred: "ReleaseForce"
    float m1500;      // inferred: "AppearDistance"
    float m1504;      // inferred: "HideDistance"
    float m1508;      // inferred: "CatchDistance"
    int m150C;
    // inferred: random appear positions {count, new[] data} (the dtor frees m1518)
    struct Positions_unk {
        Positions_unk() : m00(0), m04(0), mData(0) {}
        ~Positions_unk() {
            if (mData != 0) {
                delete[] mData;
            }
        }
        int m00;
        int m04;
        atn::Vector3* mData;
    } m1510;
    unsigned int m151C; // inferred: number of points drawn by vf14
    struct Point_unk {
        unsigned char m00[0x04];
        atn::Vector3 m04;
        unsigned char m10[0x14 - 0x10];
        float m14;
    }* m1520[0x20];
    int m15A0;        // inferred: "RandomPosition" == 1
    int m15A4;
    int m15A8;
    int m15AC;        // inferred: "Strong"
    int m15B0;        // inferred: "CatchNotifyFlag" (SpecialFlag_unk)
    int m15B4;
    int m15B8; // inferred: DebugDisp
};

#endif
