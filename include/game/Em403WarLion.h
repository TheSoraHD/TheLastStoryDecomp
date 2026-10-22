#ifndef GAME_EM403WARLION_H
#define GAME_EM403WARLION_H

#include "atn/EfpHandle_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

namespace atn {
struct MotionBuffer;
struct NodeChunk;
}

// inferred: cubic path through points (0x50 bytes; same layout and reset as gmk::PathData_unk of
// gmk/CharaPathMove.h without its last word). The vector at 0x44 is a class derived from
// std::vector (its destructor tests `this` four times). Names invented.
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: cubic polynomial of one axis of a segment (16 bytes, read by fn_803272A8); names invented
struct WarLionCubic_unk {
    float Eval_unk(float u) const { return m00 + u * (m04 + u * (m08 + m0C * u)); }
    float m00;
    float m04;
    float m08;
    float m0C;
};
// inferred: length of a segment (a float element would keep the destroy loop of the vector)
struct WarLionLen_unk {
    float m00;
};
// inferred: lengths of the segments
struct WarLionPathList_unk : public std::vector<WarLionLen_unk> {};
struct WarLionPath_unk {
    WarLionPath_unk() : m00(0), m04(0.0f) {}
    inline void Reset_unk();
    // inferred: point of segment `i` at `u` in [0, 1] (names invented)
    atn::Vector3 Segment_unk(int i, float u) {
        if (i < 0) {
            return m08;
        }
        if (i >= m00 - 1) {
            return m14;
        }
        if (m00 < 2) {
            return lbl_807C7030;
        }
        atn::Vector3 r;
        r.x = m20[i].Eval_unk(u);
        r.y = m2C[i].Eval_unk(u);
        r.z = m38[i].Eval_unk(u);
        return r;
    }
    // inferred: point at `t` in [0, 1] of the whole path
    atn::Vector3 GetPos_unk(float t) {
        if (t <= 0.0f) {
            return m08;
        }
        if (t >= 1.0f) {
            return m14;
        }
        t = m04 * t;
        for (int i = 0; i < m00 - 1; i++) {
            float len = m44[i].m00;
            if (t <= len) {
                return Segment_unk(i, t == 0.0f ? 0.0f : t / len);
            }
            t -= len;
        }
        return m08;
    }
    int m00;     // inferred: number of points
    float m04;   // inferred: total length
    atn::Vector3 m08; // inferred: first point
    atn::Vector3 m14; // inferred: last point
    std::vector<WarLionCubic_unk> m20; // inferred: x
    std::vector<WarLionCubic_unk> m2C; // inferred: y
    std::vector<WarLionCubic_unk> m38; // inferred: z
    WarLionPathList_unk m44;
};

// inferred: empties the vectors (as gmk/CharaPathMove.cpp)
inline void WarLionPath_unk::Reset_unk() {
    m00 = 0;
    m04 = 0.0f;
    m08 = lbl_807C7030;
    m14 = lbl_807C7030;
    m20.__shrink_unk(0);
    m2C.__shrink_unk(0);
    m38.__shrink_unk(0);
    m44.__shrink_unk(0);
}

// inferred: same as ResHandle, but cleared in the body of the constructor (name invented)
struct WarLionRes_unk {
    ~WarLionRes_unk() {
        if (mRes != 0 && lbl_8087EF10 != 0) {
            fn_800897D8(lbl_8087EF10, mRes);
        }
    }
    void* mRes;
};

// inferred from __vt__12Em403WarLion (0x80788BD0): war lion (boss). The factory allocates 0x1610
// bytes (SpecialEnemyBase.cpp, case 48) and passes the character data. Effect
// "/effect/ef_bs001a.efp", debug menu "em403_war_lion". Members keep their offsets (names unknown).
class Em403WarLion : public EnemyTask {
public:
    Em403WarLion(atn::Task* parent, void* data);
    virtual ~Em403WarLion();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf24();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf50();
    virtual void vf54();
    virtual float vf90();
    virtual atn::Vector3 vf94();
    virtual int vfE4();
    virtual void vfEC();

    // inferred: nonzero while the counter m15EC runs (weak copy 0x80323C18)
    int IsRaging_unk() { return m15EC > 0; }
    // inferred: name invented
    bool HasTarget_unk() { return mC58.mC0 != 0 && mC58.mC4 != 0; }

    inline void SetState_unk(int state);
    void fn_803241E8();
    void fn_80324A38(const atn::Vector3& rot, int arg, float a, float b);
    void fn_803256F8(atn::MotionBuffer* nodes, int count); // inferred: node pose callback
    void fn_803258D4(atn::Matrix3x4* mtx, const atn::NodeChunk& node); // inferred: node matrix callback
    void fn_803267E8();
    void fn_80326E60(int fast);
    void fn_80326F0C();
    void fn_80327018();
    void fn_80327118();
    void fn_803271A4();
    void fn_803272A8();
    void fn_803278B4();
    void fn_803279D4();
    void fn_80327A64();
    void fn_80327E1C();
    void fn_80327E78();
    void fn_80327EF0();
    void fn_80328270();
    void fn_8032853C();
    void fn_803288C4();
    void fn_803289EC();
    void fn_80328B74();
    void fn_80328C6C();
    void fn_8032932C();
    void fn_8032943C();
    void fn_80329744();
    void fn_80329A20();
    void fn_80329C7C();
    void fn_80329CFC();
    void fn_80329D7C();
    int fn_80329ED8(int* count, atn::Vector3* pos, float* angle);
    void fn_8032A3D4(atn::Vector3* pos, float* angle, const atn::Vector3* target, float rot, float dist);
    void fn_8032A9F4(WarLionPath_unk* path, int id, int* next);
    atn::Vector3 fn_8032A650(int* id, int a, int b);
    atn::Vector3 fn_8032A894(int id, int* next);
    // inferred: next point of the circuit (weak copy at the end of the file)
    int fn_8032C3CC(int* index, int id);

    float m14B0;
    float m14B4; // inferred: smoothed turn of the waist
    float m14B8;
    CharaTask* m14BC; // inferred: target (mC58.mC4)
    int m14C0;
    int m14C4; // inferred: step of the state
    int m14C8; // inferred: frame counter
    int m14CC; // inferred: mode
    int m14D0;
    int m14D4;
    int m14D8;
    atn::Vector3 m14DC;
    atn::Vector3 m14E8;
    int m14F4;
    int m14F8;
    int m14FC;
    atn::Vector3 m1500;
    atn::Vector3 m150C;
    atn::Vector3 m1518;
    WarLionPath_unk m1524;
    atn::Vector3 m1574;
    atn::Vector3 m1580;
    unsigned char m158C[0x1598 - 0x158C];
    CharaTask* m1598; // inferred: held character
    atn::Matrix3x4 m159C;
    int m15CC;
    int m15D0;
    int m15D4;
    int m15D8;
    int m15DC;
    int m15E0;
    int m15E4;
    int m15E8;
    int m15EC; // inferred: counter (see IsRaging_unk)
    int m15F0;
    int m15F4;
    int m15F8; // inferred: bullet id
    atn::EfpHandle_unk m15FC; // inferred: "/effect/ef_bs001a.efp"
    WarLionRes_unk m1608;     // inferred: debug menu group
    int m160C;
};

#endif
