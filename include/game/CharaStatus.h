#ifndef GAME_CHARASTATUS_H
#define GAME_CHARASTATUS_H

#include "math/Vec.h"

// inferred: point of the route tables (0x30 bytes)
struct RoutePoint_unk {
    unsigned char m00[0x2C];
    int m2C; // inferred: enabled
};
// inferred: link of the route tables (0x40 bytes)
struct RouteLink_unk {
    unsigned char m00[0x38];
    int m38; // inferred: enabled
    int m3C; // inferred: in use
};
// inferred: route tables of the field (lbl_8087F430->m10D8 or lbl_8087F540->m1A6C); the arrays at
// 0xC4-0xD0 are the spots used by gmk::InteractionSpot
struct RouteTable_unk {
    unsigned char m00[0x74];
    int mNumPoints;          // 0x74
    unsigned char m78[0x9C - 0x78];
    RoutePoint_unk* mPoints; // 0x9C
    unsigned char mA0[0xA8 - 0xA0];
    int mNumLinks;           // 0xA8
    unsigned char mAC[0xB0 - 0xAC];
    RouteLink_unk* mLinks;   // 0xB0
};

// inferred: follows a route of the route tables (class name unknown; first class of the file)
class RouteWalker_unk {
public:
    RouteWalker_unk(); // 0x80125320
    void fn_8012539C(); // inferred: reset
    int fn_80125474();  // inferred: nonzero if the route is valid

    int m00;
    int mPointA;    // 0x04
    int mPointB;    // 0x08
    int mLink;      // 0x0C
    atn::Vector3 m10;
    int m1C;
    int m20;
    int m24;
    float m28;
    int m2C;        // inferred: nonzero while the link is not owned
    unsigned char m30[0x40 - 0x30];
    int mMode;      // 0x40
    unsigned char m44[0x48 - 0x44];
    int m48;
    unsigned char m4C[0x64 - 0x4C];
    atn::Vector3 m64;
    float m70;
    float m74;
    float m78;
    float m7C;
    float m80;
    float m84;
    float m88;
    float m8C;
    int m90;
    int m94;
    int m98;
    unsigned char m9C[0xA0 - 0x9C];
    int mA0;
    int mA4;
};

namespace chr {
class Chara_unk;
}
struct Model_unk;

// inferred: head and eye look-at controller of a character (CharaTask + 0x10D8); class name
// unknown. Member names come from the debug menu ("Sight": type, offset, speed, eyeType,
// eyeOffset, eyeSpeed).
class CharaLook_unk {
public:
    CharaLook_unk();  // 0x8012980C
    ~CharaLook_unk(); // 0x801298AC
    void fn_801298EC(chr::Chara_unk* chara); // inferred: finds the "neck" node
    void fn_80129930(const atn::Vector3& pos, float speed); // inferred: looks at a position (type 2)
    void fn_80129954(const atn::Vector3& pos, float speed); // inferred: type 3
    void fn_80129978(Model_unk* model, const char* node, const atn::Vector3& offset, float speed); // type 1
    void fn_80129A48(float speed);           // inferred: stops looking (type 0)
    void fn_80129A6C();                      // inferred: update (not decompiled yet)
    void fn_8012ABD0(const atn::Vector3& pos, float speed); // inferred: eyes, type 3
    void fn_8012ABF4(const atn::Vector3& pos, float speed); // inferred: eyes, type 2
    void fn_8012AC18(float* target, const atn::Vector3& pos, float speed); // inferred: eyes, type 1
    void fn_8012AC40(float speed);           // inferred: eyes, type 0
    void fn_8012AD20(void* menu);            // inferred: debug menu

    chr::Chara_unk* mChara; // 0x00
    int mType;              // 0x04: "type"
    float* mTarget;         // 0x08: inferred: matrix followed in type 1
    atn::Vector3 mOffset;   // 0x0C: "offset"
    atn::Vector3 mRot;      // 0x18: inferred: current head rotation
    atn::Vector3 mLimit;    // 0x24: inferred: maximum head rotation
    int mNeck;              // 0x30: inferred: node index of "neck"
    unsigned char m34;
    unsigned char m35;
    short m36;
    float mSpeed;           // 0x38: "speed"
    float m3C;
    float m40;
    float m44;
    float m48;
    int mEyeType;           // 0x4C: "eyeType"
    float* mEyeTarget;      // 0x50
    atn::Vector3 mEyeOffset; // 0x54: "eyeOffset"
    atn::Vector3 mEyeRot[2]; // 0x60: inferred: current eye rotations
    float mEyeSpeed;        // 0x78: "eyeSpeed"
};

// The names are in the DOL (__vt__15CharaStatusBase 0x8077A708, __vt__11CharaStatus 0x8077A6E8).
// Layout inferred from the constructor (0x8012AE50). The save data keeps seven of them
// (MP::SaveLoad, 0x539C).

// inferred: two arrays cleared together in their constructor; names invented
struct CharaParamArrays_unk {
    CharaParamArrays_unk() {
        for (int i = 0; i < 7; i++) {
            mA[i] = 0.0f;
            mB[i] = 0.0f;
        }
    }
    float mA[7];
    float mB[7];
};

// inferred: block of parameters, twice in CharaStatusBase (0x28 and 0xE8); names invented
struct CharaParam_unk {
    CharaParam_unk() : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f), m14(1.0f), m18(0.0f), m1C(0.0f),
                       m20(0), m24(0), m28(0.0f), m2C(0.0f), m30(0), m34(0), m78(1), m7C(0), m80(0), m84(0), m88(0),
                       m8C(0.0f), m90(0.0f), m94(0), m98(0), m9C(0) {}
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
    int m30;
    int m34;
    CharaParamArrays_unk m38;
    unsigned char m70[0x78 - 0x70];
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    float m8C;
    float m90;
    int m94;
    int m98;
    int m9C;
    unsigned char mA0[0xC0 - 0xA0];
};

class CharaStatusBase {
public:
    CharaStatusBase(); // 0x8012AE50
    virtual ~CharaStatusBase() {} // weak copy 0x8012AFE8

    unsigned char m04[0x28 - 0x04];
    CharaParam_unk mParamA;    // 0x28
    CharaParam_unk mParamB;    // 0xE8
    unsigned char m1A8[0x7C];  // inferred: cleared with memset
    unsigned char m224[0x240 - 0x224];
};

class CharaStatus : public CharaStatusBase {
public:
    virtual ~CharaStatus(); // 0x8012B3A8
};

#endif
