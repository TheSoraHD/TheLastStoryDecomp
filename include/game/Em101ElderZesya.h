#ifndef GAME_EM101ELDERZESYA_H
#define GAME_EM101ELDERZESYA_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

#include "atn/ColliBase.h"

// inferred: gimmick of a barrier; only its position is read here (name invented)
struct Gimmick_unk;

// inferred: color edited in the debug menu ("FireBarrier"...); name invented
struct ZesyaColor_unk {
    ZesyaColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred from __vt__15Em101ElderZesya (0x807846A0): Zesya (first form). Data file
// "database/chara/special/" + db_name (default "db_em101_00_00.csv"). The factory allocates 0x1710
// bytes (SpecialEnemyBase.cpp, case 7).
class Em101ElderZesya : public EnemyTask {
public:
    // inferred: one of the four barriers (0x34 bytes; constructor 0x80255F2C and destructor
    // 0x80255F68 are passed to __construct_array). Names invented.
    struct Barrier_unk {
        Barrier_unk();
        ~Barrier_unk();
        // inferred: names invented
        bool IsUp_unk() const { return m04.mState != 2 && m04.mDesc != 0; }
        inline const atn::Vector3& GetPos_unk() const;
        Gimmick_unk* m00; // inferred: warp point next to the barrier (position at 0x04, rotation at 0x14)
        // inferred: state, collision objects (destroyed with the barrier) and gimmick of the barrier; fn_80259AB4
        // reaches the gimmick through a pointer to this part
        struct Colli_unk {
            ~Colli_unk() {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (mObjects[i][j] != 0) {
                            delete mObjects[i][j];
                            mObjects[i][j] = 0;
                        }
                    }
                }
            }
            int mState; // 0x04 in the barrier: inferred: 2 = broken
            int mKind;  // 0x08: inferred: index into the HitRange_unk values of a hit
            atn::ColliObject* mObjects[3][3]; // 0x0C
            Gimmick_unk* mDesc; // 0x30 in the barrier: inferred: gimmick of the barrier (position at 0x04)
        };
        Colli_unk m04;
    };

    Em101ElderZesya(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em101ElderZesya();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf3C(HitInfo_unk* hit);
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir);
    virtual void vf50();
    virtual void vf54();
    virtual atn::Vector3 vf64(int center);

    void fn_80256384(); // inferred: reads the data file (called by vf0C)
    void fn_8025922C(); // inferred: state 9 (camera on the jump)
    void fn_80259AB4(int index, int state);
    void fn_802598DC(); // inferred: picks the chant point // inferred: changes the state of a barrier
    void fn_8025941C(); // inferred: warps next to the current barrier (state 13)
    inline void GetWarp_unk(atn::Vector3* pos, float* rot);
    void fn_80258F1C(); // inferred: state 12 (casts the magic at frame 62)
    void fn_80258A50(); // inferred: state of the light sword magic (10 homing bullets)
    void fn_80258574(); // inferred: starts the next action of the phase (m14CC)
    inline void StartMagic_unk();
    inline void PlayBarrierM_unk(struct EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, const atn::Vector3& pos,
                                 const atn::Vector3& rot, int kind);
    inline void PlayBarrierK_unk(atn::EfpHandle_unk* efp, const atn::Vector3& pos, const atn::Vector3& rot, int kind);
    inline void PlayBarrier_unk(atn::EfpHandle_unk* efp, const atn::Vector3& pos, const atn::Vector3& rot, int kind);

    int m14B0;                          // inferred: loading state (0, 1 collision objects, 2 ready)
    atn::LH_Binary m14B4;               // inferred: data file
    atn::Vector_unk<atn::Vector3> m14BC; // inferred: "HighJumpAttackPointID" positions
    int m14C8;
    int m14CC;                          // inferred: "bossActPhase_" (3-5): next action (0-5)
    int m14D0;
    int m14D4;
    atn::Vector3 m14D8;       // inferred: position saved when the little Zesya is summoned
    atn::Vector3 m14E4;       // inferred: rotation saved with it
    int m14F0;
    int m14F4;
    int m14F8;
    int m14FC;                          // inferred: "ComboWaitTime"
    HitInfo_unk* m1500; // inferred: "NormalAttackID"
    HitInfo_unk* m1504; // inferred: "FlareID"
    HitInfo_unk* m1508; // inferred: "FlarelusID"
    HitInfo_unk* m150C; // inferred: "LightSwordID"
    HitInfo_unk* m1510; // inferred: "HighJumpAttackID"
    HitInfo_unk* m1514; // inferred: "HealID"
    HitInfo_unk* m1518; // inferred: "ThunderID" (cast by state 12)
    Barrier_unk m151C[4];               // 0x151C
    int m15EC;
    int m15F0;
    int m15F4;
    atn::EfpHandle_unk m15F8; // inferred: "ef_bs007a.efp"
    atn::EfpHandle_unk m1604; // inferred: "ef_bs066e.efp"
    atn::EfpHandle_unk m1610; // inferred: "ef_bs066c.efp"
    atn::EfpHandle_unk m161C; // inferred: "ef_sk053a.efp"
    atn::EfpHandle_unk m1628; // inferred: "ef_sk053b.efp"
    atn::EfpHandle_unk m1634; // inferred: "ef_sk053c.efp"
    atn::EfpHandle_unk m1640; // inferred: "ef_sk054d.efp"
    int m164C;
    int m1650;
    atn::Vector3 m1654;
    int m1660;
    int m1664;
    atn::Vector3 m1668;
    atn::Vector3 m1674;       // inferred: position of the magic
    int m1680;                // inferred: current barrier (index into m151C)
    atn::Vector3 m1684;
    float m1690;
    atn::Vector3 m1694;       // inferred: chosen chant point
    int m16A0;                // inferred: the little Zesya (Em101LittleZesya*, "LittleID")
    int m16A4;
    int m16A8;
    int m16AC;
    int m16B0;                // inferred: "WaitFrame"
    float m16B4;              // inferred: "AttackChanceFrame"
    ResHandle m16B8;          // inferred: debug menu group
    int m16BC;                // inferred: "DebugDisp"
    ZesyaColor_unk m16C0;     // inferred: "FireBarrier"
    ZesyaColor_unk m16D0;     // inferred: "IceBarrier"
    ZesyaColor_unk m16E0;
    ZesyaColor_unk m16F0;     // inferred: "ForestBarrier"
    ZesyaColor_unk m1700;     // inferred: "HolyBarrier"
};

#endif
