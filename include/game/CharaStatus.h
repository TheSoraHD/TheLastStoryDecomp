#ifndef GAME_CHARASTATUS_H
#define GAME_CHARASTATUS_H

#include "math/Vec.h"
#include "game/CharaSub_unk.h"

#include "std/new.h"

class CharaTask;
struct HitInfo_unk;

extern "C" void* memset(void* dst, int c, unsigned long n);

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
    unsigned char m10[0x14 - 0x10];
    float m14; // inferred: rotation around y
    unsigned char m18[0x28 - 0x18];
};
// inferred: point a character may look at (0x40 bytes, CharaTask::fn_80175B18); names invented
struct RouteLook_unk {
    int mId;
    atn::Vector3 mPos; // 0x04
    unsigned char m10[0x14 - 0x10];
    float mRadius;     // 0x14
    int mPriority;     // 0x18
    unsigned char m1C[0x38 - 0x1C];
    int mEnabled;      // 0x38
    int mLink;         // 0x3C: inferred: id of another look point that replaces this one
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

    unsigned char m00[0x38];
    unsigned int m38; // inferred: flags (bit 1 tested by CharaTask::fn_801765D8)
    unsigned char m3C[0x74 - 0x3C];
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
    unsigned char mB4[0xFC - 0xB4];
    unsigned int mNumLooks;  // 0xFC
    RouteLook_unk* mLooks;   // 0x100
    unsigned char m104[0x134 - 0x104];
    void* m134; // inferred: passed to fn_803AB37C with a character (CharaTask::fn_80171DB0)
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
    void fn_80128930(CharaTask* target, const atn::Vector3* dest, int arg); // inferred: follows a character
    void fn_80128A30(const atn::Vector3* pos, const atn::Vector3* dest, int arg, float radius);
    void fn_80128B20(CharaTask* target, const atn::Vector3* dest, int arg, float radius); // mode 6
    void fn_80128C08(CharaTask* target, const atn::Vector3* dest, int arg, float arg2);  // mode 7
    int fn_80127EF4(); // inferred: stops unless the flag of m48 is set
    int fn_801255C8(); // inferred: starts the planned route (nonzero on success, fn_8016F824)
    void fn_80126214(); // inferred: update (called every frame by CharaTask::fn_80139560)
    float fn_801279E8(); // inferred: distance (compared with 20 by CharaTask::vf34)
    atn::Vector3* fn_80139F58(); // inferred: accessors called out of line from CharaTask
    int fn_8013C3F4();
    CharaSubSeg_unk* fn_8013C3FC();
    int fn_8013C424();

    int mDestPoint; // 0x00
    int mPointA;    // 0x04
    int mPointB;    // 0x08
    int mLink;      // 0x0C
    atn::Vector3 m10;
    int m1C;
    int mPlace;     // 0x20
    CharaTask* mTarget; // 0x24
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
    CharaSubSeg_unk m70; // inferred: passed to CharaSub_unk::fn_80135338 (accessor 0x8013C3FC)
    int m90;
    int m94;
    int m98;
    float m9C;
    int mA0;
    CharaTask* mChara; // 0xA4
};


// inferred: head and eye look-at controller of a character (CharaTask + 0x10D8); class name
// unknown. Member names come from the debug menu ("Sight": type, offset, speed, eyeType,
// eyeOffset, eyeSpeed).
// inferred: node passed to the node matrix hooks (name at 0x10); name invented
struct ModelNodeDesc_unk {
    unsigned char m00[0x10];
    const char* mName;
};

class CharaLook_unk {
public:
    CharaLook_unk();  // 0x8012980C
    ~CharaLook_unk(); // 0x801298AC
    void fn_801298EC(CharaTask* chara); // inferred: finds the "neck" node
    void fn_80129930(const atn::Vector3& pos, float speed); // inferred: looks at a position (type 2)
    void fn_80129954(const atn::Vector3& pos, float speed); // inferred: type 3
    void fn_80129978(Model_unk* model, const char* node, const atn::Vector3& offset, float speed); // type 1
    void fn_80129A48(float speed);           // inferred: stops looking (type 0)
    void fn_80129A6C();                      // inferred: update
    int fn_8012A288(atn::Matrix3x4* mtx, ModelNodeDesc_unk* node); // inferred: node matrix hook
    void fn_8012ABD0(const atn::Vector3& pos, float speed); // inferred: eyes, type 3
    void fn_8012ABF4(const atn::Vector3& pos, float speed); // inferred: eyes, type 2
    void fn_8012AC18(float* target, const atn::Vector3& pos, float speed); // inferred: eyes, type 1
    void fn_8012AC40(float speed);           // inferred: eyes, type 0
    void fn_8012AD20(void* menu);            // inferred: debug menu

    CharaTask* mChara; // 0x00
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

// inferred: block of parameters, twice in CharaStatusBase (0x28 and 0xE8); names invented
struct CharaParam_unk {
    bool fn_801404E0(unsigned int flag); // inferred: accessor called out of line from CharaTask
    CharaParam_unk() : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f), m14(1.0f), m18(0.0f), m1C(0.0f),
                       m20(0), m24(0), m28(0.0f), m2C(0.0f), m30(0), m34(0) {
        for (int i = 0; i < 7; i++) {
            m38[i] = 0.0f;
            m54[i] = 0.0f;
        }
        m78 = 1;
        m7C = 0;
        m80 = 0;
        m84 = 0;
        m88 = 0;
        m8C = 0.0f;
        m90 = 0.0f;
        m94 = 0;
        m98 = 0;
        m9C = 0;
    }
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
    float m38[7]; // inferred: cleared together with m54
    float m54[7];
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

// inferred: 8-byte block cleared by its constructor (four of them in CharaStatus); name invented
struct CharaStatusPair_unk {
    CharaStatusPair_unk() { memset(this, 0, sizeof(*this)); }
    CharaStatusPair_unk(int id) {
        memset(this, 0, sizeof(*this));
        m[0] = id;
    }
    int m[2]; // inferred: an array, since copies of the pair move it as one block
};

namespace chr {
// inferred: data of an equipped part (CharaPart_unk::m274): starts with the parameters it adds
// (fn_8012B988); mF0 is tested by ClearFlags_unk
struct CharaPartData_unk {
    CharaParam_unk mParam;
    unsigned char mC0[0xF0 - 0xC0];
    int mF0;
};

// inferred: equipment object (CharaTask::m680[]); m414 points to its parameters
struct CharaEquip_unk {
    // inferred: arguments unknown (CharaTask::fn_801371AC passes the slot and the character)
    CharaEquip_unk(int slot, int a, int b, ::CharaTask* chara, int c, int d, int e, int f, int g); // 0x805634D0
    ~CharaEquip_unk(); // 0x805638BC
    void fn_80563A1C(const char* name); // inferred: loads the item
    int fn_8056405C();  // inferred: nonzero while loading
    void fn_80566010(); // inferred: attaches the model
    void fn_805663F8();
    void fn_80566408(); // inferred: undoes fn_805663F8 (CharaTask::fn_80145334)
    int m000; // inferred: kind (4 is handled apart by CharaTask::fn_80149A30)
    int m004; // inferred: item id ((id % 1000) / 10 == 46: coat with "leftcoat"/"rightcoat" nodes)
    unsigned char m008[0x024 - 0x008];
    unsigned char m024[0x028 - 0x024]; // inferred: model-like object (passed to fn_80094E2C)
    unsigned int m028; // inferred: flags of that model
    unsigned char m02C[0x414 - 0x02C];
    CharaParam_unk* m414;
    unsigned char m418[0x43C - 0x418];
};

// inferred: character data pointed to by CharaTask::m5C; starts with the base parameters copied
// into CharaStatusBase::mParamA (fn_8012B3E8). Name invented.
struct CharaData_unk {
    CharaParam_unk mParam; // 0x00
    int mC0;
    int mC4;
    int mC8; // inferred: passed to fn_8020FCE8 and fn_8012C6D4
    unsigned char mCC[0xE8 - 0xCC];
    ::CharaStatusPair_unk mE8[4]; // inferred: added to CharaStatus::m2D0 on load (fn_8013655C)
    int m108[5];                  // inferred: passed to CharaTask::fn_8014FF9C on load
    int m11C;
    unsigned char m120[0x122 - 0x120];
    signed char m122;
    unsigned char m123[0x134 - 0x123];
    float m134; // inferred: copied to CharaTask::m568 by the constructor
};
} // namespace chr

// inferred: block of CharaStatusBase cleared with memset (0x1A8, 0x7C bytes); name invented
struct CharaStatusData_unk {
    int m00;
    int mExp;              // 0x1AC: inferred: experience (fn_8012B028)
    float m08;             // 0x1B0: inferred: bonuses added by fn_8012B3E8
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    unsigned char m24[0x44 - 0x24];
    unsigned int mBits[4]; // 0x1EC: inferred: 128 flags (fn_80134674, fn_801346C8)
    unsigned int m54;      // 0x1FC: inferred: index into a table of 5 factors (fn_8012F034)
    int m58;
    unsigned char m5C[0x7C - 0x5C];
};

class CharaStatusBase {
public:
    CharaStatusBase(); // 0x8012AE50
    virtual ~CharaStatusBase() {} // weak copy 0x8012AFE8

    // inferred: accessor (name invented); keeps the flag word reloaded in fn_8012E540
    bool IsState_unk(unsigned int flag) const { return (m0C & flag) == flag; }
    // inferred: bit 11 of m0C (weak copy 0x80151188); name unknown
    int fn_80151188() const { return (m0C >> 11) & 1; }

    float m04; // inferred: current value capped by mParamB.m84 (hit points?)
    float m08; // inferred: current value capped by mParamB.m88

    unsigned int m0C; // inferred: flags
    unsigned int m10; // inferred: flags (set together with m0C by fn_8013310C)
    int m14;
    unsigned int m18; // inferred: flags (fn_8012DB04)
    int m1C; // inferred: level (fn_80133B30)
    float m20; // inferred: set from CharaTask::mFC8.x by CharaTask::fn_80164B00, cleared by fn_80164D24
    float m24; // inferred: set together with m20 by CharaTask::fn_80164B00
    CharaParam_unk mParamA;    // 0x28
    CharaParam_unk mParamB;    // 0xE8
    CharaStatusData_unk m1A8;  // inferred: cleared with memset
    int m224; // inferred: counter decremented by fn_8012DD70
    float m228; // inferred: gauge raised by fn_8012F034
    unsigned char m22C[0x230 - 0x22C];
    unsigned char m230; // inferred: four flags (fn_80134774, fn_801347C8)
    unsigned char m231[0x240 - 0x231];
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
    unsigned int m04;
    CharaTask* m08; // inferred: compared with CharaStatus::mChara (fn_8013407C)
    int m0C;
    int m10;
};

// inferred: array with a count and a fixed capacity (erase shifts the elements down, push_back
// constructs in place); name invented
template <class T, int N>
struct FixedArray_unk {
    FixedArray_unk() : mCount(0) {}
    T* begin() { return mData; }
    T* end() { return mData + mCount; }
    T& operator[](int i) { return mData[i]; }
    void erase(T* it) {
        for (unsigned int i = it - mData; i < mCount - 1; i++) {
            mData[i] = mData[i + 1];
        }
        mCount = mCount - 1;
    }
    bool full() const { return mCount >= N; }
    // inferred: inserts before `it` (CharaTask::fn_8014F2B4)
    void insert(T* it, const T& x) {
        unsigned int pos = it - mData;
        for (unsigned int i = mCount; i > pos; i--) {
            mData[i] = mData[i - 1];
        }
        mData[pos] = x;
        mCount++;
    }
    // inferred: without the capacity check (CharaTask::fn_801371AC); name invented
    void PushBack_unk(const T& x) {
        new (end()) T(x);
        mCount++;
    }
    void push_back(const T& x) {
        if (!full()) {
            new (end()) T(x);
            mCount++;
        }
    }

    unsigned int mCount;
    T mData[N];
};

// inferred: hit kept by a character (CharaTask::m12D0); name invented
struct CharaHit_unk {
    CharaHit_unk() : mHit(0), m04(0), m08(0), m0C(0), m0E(0), m10(0) {}
    // inferred: CharaTask::fn_80178668
    CharaHit_unk(HitInfo_unk* hit, int parent, int time, unsigned short flags, unsigned char flag, int attacker)
        : mHit(hit), m04(parent), m08(time), m0C(flags), m0E(flag), m10(attacker) {}
    HitInfo_unk* mHit;
    int m04; // inferred: bullet whose second bullet is mHit (CharaTask::fn_801784BC)
    int m08; // inferred: frames left (CharaTask::fn_80178BFC)
    unsigned short m0C; // inferred: flags from CharaTask::fn_80179730
    unsigned char m0E;
    int m10; // inferred: attacker (CharaTask*; CharaTask::fn_80178668, also passed to fn_800FDE60 by fn_8012E540)
};

// Embedded in CharaTask at 0x7D4 (callers pass CharaTask + 2004).
class CharaStatus : public CharaStatusBase {
public:
    CharaStatus(); // 0x8012B0B0
    virtual ~CharaStatus(); // 0x8012B3A8

    int fn_8012B028(); // inferred: experience still needed for the next level
    void fn_8012B3E8();
    void fn_8012B988();
    void fn_8012C6D4(CharaParam_unk* param, int level, int arg, int arg2);
    void fn_8012D180();
    void UpdatePartFlag_unk(); // inferred: name invented
    bool IsWeak_unk(); // inferred: name invented
    void fn_8012CE4C(int kind);
    void fn_8012E244(HitInfo_unk* hit);
    void fn_8012E540();
    bool HasTimer_unk() const { return m2F4 > 0.0f; } // inferred: accessor (name invented)
    int fn_8013354C(int exp, struct LevelUpResult_unk* result); // inferred: adds experience
    void fn_8012DF7C(int damage, unsigned int flags, HitInfo_unk* hit);
    int fn_8012F440(int flags, int power, int remove, int damage, int strong, int both, int effect);
    void ApplyRates_unk(HitInfo_unk* hit);
    void fn_801342D0(struct StatusItem_unk* item, int stat);
    int fn_80133BD8(int force); // inferred: level of the character
    void ClearBit_unk(int bit);
    int fn_8012E534();
    int fn_80139F4C(); // inferred: accessor called out of line from CharaTask
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
    int fn_801334CC(int level); // inferred: experience needed for `level`
    void fn_80134674(int bit);
    bool fn_801346C8(int bit, int arg = 0); // inferred: every caller passes 0 as a second argument (unused)
    void fn_80134724(int add);
    void fn_80134774(int index, int on);
    bool fn_801347C8(int index);
    void fn_80133B30(int level);
    void fn_80133E24(int id, int sub);
    void fn_80133FBC(const CharaStatusRef_unk& ref, int replace);
    void fn_8013407C();
    void fn_8012D628(CharaParam_unk* dst, const CharaParam_unk* src);
    void fn_80133130(unsigned int flags, int value, int both);
    void fn_801333E4();
    void fn_8013459C();
    void fn_8012D714(CharaParam_unk* dst, const CharaParam_unk* a, const CharaParam_unk* b);
    void fn_8012F034(float amount);
    void fn_8013322C(unsigned int flags);
    // inferred: inlined in fn_8012D8B8 and fn_8012DD70 (defined after them as fn_8013322C, too big to
    // be inlined from below), so it is an inline member; name invented.
    // NONMATCHING (in every user): our compiler turns the parts loop into a ctr loop with the count
    // read once; the original reloads mChara->m650 each time and keeps an index and an offset.
    // Tried: int and unsigned index, break instead of return, a local chara pointer, a bool helper,
    // a found flag, nested inline helpers.
    void ClearFlags_unk(unsigned int flags);
    void fn_8012D8B8();
    void fn_8012DD70();
    float fn_8012DB04();
    void fn_8012F188(float amount);
    CharaStatusRef_unk* FindRef_unk(int id, unsigned int key);

    int m240;
    float m244; // inferred: copy of m04 (fn_8012D8B8)
    int m248;
    int m24C[32]; // inferred: one value per flag bit (fn_801333A4)
    int m2CC;
    CharaStatusPair_unk m2D0[4];
    CharaTask* mChara; // 0x2F0
    float m2F4;
    float m2F8;
    float m2FC;
    int m300;
    FixedArray_unk<CharaStatusEntry_unk, 6> mEntries; // 0x304
    FixedArray_unk<CharaStatusRef_unk, 8> mRefs;      // 0x380
    int m424; // inferred: value picked at random by fn_8013459C
    float m428;
    float m42C; // inferred: compared with 0, 3 and 5 by fn_8012F3D8
    int m430;
    float m434;
    float m438;
};

#endif
