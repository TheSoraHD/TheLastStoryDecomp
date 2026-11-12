#ifndef GAME_LASTCOCOONQUADRUPED_H
#define GAME_LASTCOCOONQUADRUPED_H

#include "game/EnemyTask.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ColliBase.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "game/ResHandle.h"
#include "std/vector.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: a vector inside a struct (one more null test in the destructor; name invented)
struct QuadList_unk {
    std::vector<float> mItems;
};
// inferred: cubic of one segment and axis: a0 + a1 u + a2 u^2 + a3 u^3 (as PathCubic_unk of
// game/PathData_unk.cpp)
struct QuadCubic_unk {
    float Eval_unk(float u) const { return u * (u * (m3 * u + m2) + m1) + m0; }
    float m0;
    float m1;
    float m2;
    float m3;
};
// inferred: 0x50 bytes at 0x1504: spline through the places of the route, built by fn_8032AF90 (same
// layout as PathSpline_unk of game/PathData_unk.cpp; names invented)
struct QuadRoutes_unk {
    // inferred: outlined by the constructor of LastCocoonQuadruped (0x8045FE2C)
    QuadRoutes_unk() : mNumSegments(0), mLength(0.0f) {}
    // inferred: point of segment `i` at `u`
    atn::Vector3 GetPoint_unk(int i, float u) const {
        if (i < 0) {
            return mStart;
        }
        if (i >= mNumSegments - 1) {
            return mEnd;
        }
        if (mNumSegments < 2) {
            return lbl_807C7030;
        }
        atn::Vector3 p;
        p.x = mX[i].Eval_unk(u);
        p.y = mY[i].Eval_unk(u);
        p.z = mZ[i].Eval_unk(u);
        return p;
    }
    // inferred: position at `t` (0..1) along the spline
    atn::Vector3 GetPos_unk(float t) const {
        if (t <= 0.0f) {
            return mStart;
        }
        if (t >= 1.0f) {
            return mEnd;
        }
        float d = mLength * t;
        for (int i = 0; i < mNumSegments - 1; i++) {
            float len = mLengths.mItems[i];
            if (d <= len) {
                float u = 0.0f == d ? 0.0f : d / len;
                return GetPoint_unk(i, u);
            }
            d -= len;
        }
        return mStart;
    }
    int mNumSegments;
    float mLength;
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
    std::vector<QuadCubic_unk> mX;
    std::vector<QuadCubic_unk> mY;
    std::vector<QuadCubic_unk> mZ;
    QuadList_unk mLengths;
};

// inferred from __vt__19LastCocoonQuadruped (0x8078FBC0): quadruped form of the last cocoon; the enemy
// factory (fn_805A5374) allocates 0x18F8 bytes. Members keep their offsets (names unknown).
class LastCocoonQuadruped : public EnemyTask {
public:
    LastCocoonQuadruped(atn::Task* parent, EnemyInfo_unk* info); // 0x8045FAB0
    virtual ~LastCocoonQuadruped();                              // 0x80468D48
    virtual int vf0C();                                          // 0x8045FE70
    virtual void vf10();                                         // 0x80460458
    virtual void vf14();                                         // 0x80460A90
    virtual void vf20();                                         // 0x80461520
    virtual void vf24();                                         // 0x804628B4
    virtual void vf3C(HitInfo_unk* hit);                         // 0x80460A94
    virtual void vf40(HitInfo_unk* hit);                         // 0x80460B48
    virtual void vf50();                                         // 0x80460F18
    virtual void vf54(); // 0x80461398: inferred: places the hit spheres on the head, waist and spine
    virtual atn::Vector3 vf64(int center);                       // 0x80460EF0
    virtual float vf90();                                        // 0x80468D40
    virtual int vfE4();                                          // 0x80468D18
    void fn_80465294(); // inferred: mode 13: drains the hp of m1574 every 30 frames
    void fn_804654F0(); // inferred: back to mode 0 at the end of the motion

    void fn_804617A4(); // inferred: chooses the target of the mode
    void fn_80461838();
    void fn_80463174(); // inferred: mode 0: chooses the next mode
    void fn_80463610(); // inferred: mode 6
    void fn_80466CA4(); // inferred: walks along the route (m1504)
    void fn_804672CC(); // inferred: walks along the route (m1504) towards m1554
    void fn_80463F20(); // inferred: mode 10
    void fn_80464608(); // inferred: mode 11
    void fn_80464CF0(); // inferred: mode 12
    void fn_804655AC(); // inferred: mode 15
    void fn_80465B88(); // inferred: mode 16
    // inferred: a place to walk to was found (kind, position and angle)
    int fn_80468038(int* kind, atn::Vector3* pos, float* angle);
    // inferred: of the two sides (base -/+ 90 degrees), the farthest free position at dist from `at`
    void fn_80468534(atn::Vector3* pos, float* angle, const atn::Vector3& at, float base, float dist);
    void fn_80462CCC(atn::Matrix3x4* mtx, const atn::NodeChunk& node); // inferred: node matrix callback
    void fn_804635B4();
    void fn_804638A4(); // inferred: turns to m14B8, then the bullet 0x5B4 at frames 19-26
    void fn_80463C58(); // inferred: mode 9 at frame 5: lands on the ground under m14D4
    void fn_80463DCC(); // inferred: end of the motion: back to mode 0 (states 1, 7) or mode 7
    void fn_80466370(); // inferred: mode 15: jump with the bullet 0x5B5 at frames 20-24
    void fn_804666D8(); // inferred: mode with the moving effect (frames 10-80)
    void fn_804678F4(); // inferred: mode 12: moves towards m1554 in 30 frames
    void fn_80467C18(CharaTask* target, const atn::Vector3& pos); // inferred: mode 15: jumps to pos
    void fn_80466868(); // inferred: end of the motion of mode 6 (shake at frame 3)
    void fn_80466974(); // inferred: back to mode 0
    // inferred: place of the given kind farthest from the players (names invented)
    atn::Vector3 fn_804687B0(int* id, int kind8, int kind4);
    atn::Vector3 fn_80468A08(int id, int* next); // inferred: position of the place after id
    void fn_80467F50(); // inferred: mode 19 (motion 0x14A, stops the effect 1000, SE_EM206_001)
    // inferred: builds the path through the place id and the next two (to 600 when last is set)
    int fn_80468B68(QuadRoutes_unk* path, int id, int* next, int last);
    void fn_80466A04(); // inferred: mode 6 (motion 0x14A and SE_EM206_001)
    void fn_80466AE4(); // inferred: mode 7 (motion 0x156)
    void fn_80466B8C(); // inferred: mode 8 (motion 0x13F and the effect m1790)
    // inferred: starts a mode (names invented)
    void SetMode_unk(int mode);

    atn::LH_Binary m14B0; // inferred: parameter file
    CharaTask* m14B8; // inferred: target
    int m14BC;
    int m14C0; // inferred: frame counter of the mode
    int m14C4; // inferred: state (7: no hit reaction)
    int m14C8;
    int m14CC;
    unsigned char m14D0[0x14D4 - 0x14D0];
    atn::Vector3 m14D4; // inferred: position to land on
    atn::Vector3 m14E0; // inferred: rotation after landing
    int m14EC;
    int m14F0; // inferred: places left on the route
    int m14F4; // inferred: id of the current place

    atn::Vector3 m14F8;
    QuadRoutes_unk m1504;
    CharaTask* m1554; // inferred: target of modes 11 and 12
    int m1558;
    atn::Vector3 m155C; // inferred: position of the target (mode 12)
    atn::Vector3 m1568; // inferred: step towards it (1/30 of the distance)
    CharaTask* m1574; // inferred: target of mode 13
    atn::Matrix3x4 m1578;
    int m15A8;
    CharaTask* m15AC;   // inferred: target of mode 15
    atn::Vector3 m15B0; // inferred: position to jump to (mode 15)
    atn::Vector3 m15BC; // inferred: step added to the position (frames 10-55 of fn_804666D8)
    CharaTask* m15C8; // inferred: target of mode 17
    atn::Vector3 m15CC;
    atn::ColliCapsule mCapsules[5]; // 0x15D8
    atn::EfpFile m1790;
    atn::EfpFile m179C;
    atn::ColliCapsule m17A8; // inferred: enabled (bit 0 of mFlags) when mode 6 starts
    int m1800; // inferred: an effect is attached to the motion
    atn::Vector3 m1804;
    atn::Vector3 m1810;
    unsigned char m181C[0x1824 - 0x181C];
    atn::EffFile m1824;
    atn::EfpFile m1830;
    atn::EffFile m183C;
    atn::EffFile m1848;
    unsigned char m1854[0x1858 - 0x1854];
    atn::Matrix3x4 m1858;
    atn::Matrix3x4 m1888;
    atn::Matrix3x4 m18B8;
    int m18E8; // inferred: hit by the skill 0x5BA
    int m18EC;
    ResHandle m18F0; // inferred: debug menu group
    int m18F4;       // inferred: DebugDisp value
};

#endif
