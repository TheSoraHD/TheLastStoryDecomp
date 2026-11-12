#ifndef GAME_LASTCOCOONIMAGO_H
#define GAME_LASTCOCOONIMAGO_H

#include "atn/LH_Binary.h"
#include "atn/LH_Xsca.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ColliBase.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "game/GimmickCommon.h"

// inferred from __vt__15LastCocoonImago (0x8078F800): imago form of the last cocoon; the enemy factory
// (fn_805A5374) allocates 0x1990 bytes. Members keep their offsets (names unknown).
class LastCocoonImago : public EnemyTask {
public:
    LastCocoonImago(atn::Task* parent, EnemyInfo_unk* info); // 0x80458270
    virtual ~LastCocoonImago();                              // 0x8045F9A0
    virtual int vf0C();                                      // 0x80458590
    virtual void vf20();                                     // 0x8045B594
    virtual void vf24();                                     // 0x8045B40C
    virtual void vf38(int enable);                           // 0x8045D1EC
    virtual void vf3C(HitInfo_unk* hit);                     // 0x80459358
    virtual void vf54(); // 0x8045A8F0: inferred: places the hit spheres of the attacks
    virtual atn::Vector3 vf64(int center);                   // 0x80459EBC
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& dir); // 0x8045C83C
    virtual float vf90();                                    // 0x8045F998
    virtual int vfB8(wchar_t* name);                         // 0x80459F24
    virtual void vfCC(atn::Vector3* out, const atn::Vector3& dir); // 0x8045F6E0
    virtual atn::Vector3 vfD8(CharaTask* target);            // 0x8045F884
    void fn_8045ABB4(const atn::Vector3& pos, float f1, float f2); // inferred: moves towards pos
    void fn_8045DF34(); // inferred: mode 11: carries the gimmick m18F8, then waits for the thrust
    void fn_8045E548(); // inferred: mode with the motions 0x14B-0x14D, then mode 10
    void fn_8045E780(); // inferred: mode 10: lunges towards m14D4 with the bullet 0x5B6
    inline void ResetAttacks_unk();
    void fn_8045D224(); // inferred: mode update: holds the gimmick motion, frame 120 disables the capsules
    void fn_8045EB60();
    void fn_8045F4CC(); // inferred: sends message 4 to the gimmicks of type 1 touching the body // inferred: mode 2 (motion 0x37, or 0x31 from frame 200 after mode 14)
    void fn_8045ECC4(); // inferred: mode 12 (motion 0x31), stops and sends message 5 to m18F8
    // inferred: starts a mode (names invented)
    void SetMode_unk(int mode);
    // inferred: position of the "neck" node (0x8045F914; the literal is in the .data pool of the
    // inline member, as in Em203GreaterDaemonRed)
    virtual atn::Vector3 GetTacViewPosition() const {
        MtxPtr m = mMotion.GetNodeMtx_unk("neck");
        float z = m[2][3];
        float y = m[1][3];
        float x = m[0][3];
        return atn::Vector3(x, y, z);
    }
    virtual float vfD4();                                    // 0x8045F990
    virtual float vfDC();                                    // 0x8045F90C
    virtual int vfE4();                                      // 0x8045F6A0

    atn::LH_Binary m14B0; // inferred: parameter file
    int m14B8;
    int m14BC;
    int m14C0; // inferred: frame counter of the mode
    int m14C4;
    int m14C8;
    int m14CC;
    int m14D0;
    atn::Vector3 m14D4;
    atn::Vector3 m14E0;
    int m14EC;
    atn::EfpFile m14F0; // inferred: "ef_sk023a.efp"
    unsigned char m14FC[0x1500 - 0x14FC];
    atn::ColliCapsule mCapsules[11]; // 0x1500: inferred: hit capsules (bit 0 of mFlags enables them)
    int m18C8;                       // inferred: sphere (m624) used as the center
    int m18CC;
    int m18D0;
    // inferred: wait and counter of the three attacks of sImagoAttacks_unk (names invented)
    struct AttackTimer_unk {
        int mWait;
        int mCount;
    } m18D4[3];
    int m18EC;
    int m18F0;
    RoutePlace_unk* m18F4; // inferred: place 2 of the field
    GimmickCommon* m18F8; // inferred: gimmick told (message 5) when mode 12 starts
    atn::ColliSphere m18FC; // inferred: a sphere (0x4C bytes; its destructor is inlined)
    atn::Vector3 m1948;
    float m1954;
    int m1958;
    int m195C;
    int m1960;
    atn::LH_Xsca m1964; // inferred: "camera/battle/cm000_em206_00.xsca"
    atn::EfpFile m196C; // inferred: "ef_uc030.efp"
    int m1978;
    unsigned char m197C;
    unsigned char m197D[0x1980 - 0x197D];
    ResHandle m1980; // inferred: debug menu group
    int m1984;
    float m1988;
    unsigned char m198C[0x1990 - 0x198C];
};

#endif
