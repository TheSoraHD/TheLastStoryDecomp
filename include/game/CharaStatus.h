#ifndef GAME_CHARASTATUS_H
#define GAME_CHARASTATUS_H

#include "math/Vec.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace chr {
class Chara_unk;
}
struct Model_unk;
class CharaTask;

// inferred: point of the route tables (0x30 bytes)
struct RoutePoint_unk {
    int m00;
    atn::Vector3 mPos; // 0x04
    unsigned char m10[0x14 - 0x10];
    float mRadius;     // 0x14
    unsigned char m18[0x2C - 0x18];
    int m2C; // inferred: enabled
};
// inferred: link of the route tables (0x40 bytes)
struct RouteLink_unk {
    int m00;
    atn::Vector3 mPos; // 0x04
    unsigned char m10[0x38 - 0x10];
    int m38; // inferred: enabled
    int m3C; // inferred: in use
};
// inferred: route tables of the field (lbl_8087F430->m10D8 or lbl_8087F540->m1A6C); the arrays at
// 0xC4-0xD0 are the spots used by gmk::InteractionSpot
// inferred: named place of the route tables (0x28 bytes)
struct RoutePlace_unk {
    int mId;
    atn::Vector3 mPos; // 0x04
    unsigned char m10[0x28 - 0x10];
};
// inferred: route between two points (8 bytes)
struct RoutePath_unk;
struct RouteTable_unk {
    // inferred: names invented
    RouteLink_unk* GetLink_unk(int link) { return &mLinks[link - 1]; }
    RoutePlace_unk* FindPlace_unk(int id) {
        for (unsigned int i = 0; i < mNumPlaces; i++) {
            if (id == mPlaces[i].mId) {
                return &mPlaces[i];
            }
        }
        return 0;
    }

    unsigned char m00[0x74];
    int mNumPoints;          // 0x74
    unsigned int mNumPlaces; // 0x78
    RoutePlace_unk* mPlaces; // 0x7C
    unsigned char m80[0x9C - 0x80];
    RoutePoint_unk* mPoints; // 0x9C
    unsigned char mA0[0xA4 - 0xA0];
    RoutePath_unk* mPaths;   // 0xA4
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
    void fn_80127D8C(int a, int b, int c); // inferred: stops (arguments passed to CharaTask::fn_8016EB48)
    void fn_80128508(int start, const atn::Vector3* dest, int arg); // inferred: plans a route
    void fn_801286F0(int link, const atn::Vector3* dest, int arg); // inferred: follows a link
    void fn_80128818(int id, const atn::Vector3* dest, int arg);   // inferred: goes to a named place
    void fn_80128930(chr::Chara_unk* target, const atn::Vector3* dest, int arg); // inferred: follows a character
    void fn_80128A30(const atn::Vector3* pos, const atn::Vector3* dest, int arg, float radius);
    void fn_80128B20(chr::Chara_unk* target, const atn::Vector3* dest, int arg, float radius); // mode 6
    void fn_80128C08(chr::Chara_unk* target, const atn::Vector3* dest, int arg, float arg2);  // mode 7
    int fn_80127EF4(); // inferred: stops unless the flag of m48 is set

    int mDestPoint; // 0x00
    int mPointA;    // 0x04
    int mPointB;    // 0x08
    int mLink;      // 0x0C
    atn::Vector3 m10;
    int m1C;
    int mPlace;     // 0x20
    chr::Chara_unk* mTarget; // 0x24
    float mRadius;  // 0x28
    int m2C;        // inferred: nonzero while the link is not owned
    atn::Vector3 m30;
    float m3C;
    int mMode;      // 0x40
    int m44;
    int m48;
    atn::Vector3 m4C;
    atn::Vector3 m58;
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
    float m9C;
    int mA0;
    chr::Chara_unk* mChara; // 0xA4
};


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

    unsigned char m04[0x0C - 0x04];
    unsigned int m0C; // inferred: flags
    unsigned int m10; // inferred: flags (set together with m0C by fn_8013310C)
    unsigned char m14[0x28 - 0x14];
    CharaParam_unk mParamA;    // 0x28
    CharaParam_unk mParamB;    // 0xE8
    unsigned char m1A8[0x7C];  // inferred: cleared with memset; 0x1AC is read by fn_8012B028
    unsigned char m224[0x240 - 0x224];
};

// inferred: 8-byte block cleared by its constructor (four of them in CharaStatus); name invented
struct CharaStatusPair_unk {
    CharaStatusPair_unk() { memset(this, 0, sizeof(*this)); }
    int m00;
    int m04;
};

// inferred: 20-byte entry of the first table in CharaStatus (id at 0, value at 4, sub id at 8)
struct CharaStatusEntry_unk {
    CharaStatusEntry_unk() : mId(0), mValue(0), mSub(0), m0C(0), m10(0) {}
    int mId;
    int mValue;
    int mSub;
    int m0C;
    int m10;
};

// inferred: 20-byte entry of the second table; points to an entry of the first kind
struct CharaStatusRef_unk {
    CharaStatusRef_unk() : mEntry(0), m04(0), m08(0), m0C(-1), m10(0) {}
    CharaStatusEntry_unk* mEntry;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// Embedded in CharaTask at 0x7D4 (callers pass CharaTask + 2004).
class CharaStatus : public CharaStatusBase {
public:
    CharaStatus(); // 0x8012B0B0
    virtual ~CharaStatus(); // 0x8012B3A8

    int fn_8012B028(); // inferred: experience still needed for the next level
    int fn_8012E534();
    int fn_8012F3D8();
    void fn_8013310C(unsigned int flags, int both);
    int fn_801333A4(unsigned int mask);
    CharaStatusEntry_unk* fn_80133EE8(int id);
    int fn_80133F18(int id, int sub);
    int fn_80133F6C(int id, int sub);
    CharaStatusRef_unk* fn_80134134(int id);
    int fn_80134168(int id, int sub);
    int fn_801341B8(int id, int sub);
    bool fn_80134208();
    bool fn_80134234();
    bool fn_80134250();
    bool fn_80134270();
    bool fn_80134290();
    bool fn_801342B0();

    int m240;
    unsigned char m244[0x248 - 0x244];
    int m248;
    int m24C[32]; // inferred: one value per flag bit (fn_801333A4)
    int m2CC;
    CharaStatusPair_unk m2D0[4];
    chr::Chara_unk* mChara; // 0x2F0
    float m2F4;
    float m2F8;
    float m2FC;
    int m300;
    unsigned int mNumEntries; // 0x304
    CharaStatusEntry_unk mEntries[6]; // 0x308
    unsigned int mNumRefs; // 0x380
    CharaStatusRef_unk mRefs[8]; // 0x384
    unsigned char m424[0x428 - 0x424];
    float m428;
    float m42C; // inferred: compared with 0, 3 and 5 by fn_8012F3D8
    int m430;
    float m434;
    float m438;
};

#endif
