#ifndef GAME_LASTCOCOONIMAGO_H
#define GAME_LASTCOCOONIMAGO_H

#include "atn/LH_Binary.h"
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
    virtual void vf24();                                     // 0x8045B40C
    virtual void vf38(int enable);                           // 0x8045D1EC
    virtual atn::Vector3 vf64(int center);                   // 0x80459EBC
    virtual float vf90();                                    // 0x8045F998
    virtual int vfB8(wchar_t* name);                         // 0x80459F24
    virtual void vfCC(atn::Vector3* out, const atn::Vector3& dir); // 0x8045F6E0
    virtual atn::Vector3 vfD8(CharaTask* target);            // 0x8045F884
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
    unsigned char m14B8[0x14BC - 0x14B8];
    int m14BC;
    int m14C0; // inferred: frame counter of the mode
    unsigned char m14C4[0x14C8 - 0x14C4];
    int m14C8;
    unsigned char m14CC[0x14F0 - 0x14CC];
    atn::EfpFile m14F0;
    unsigned char m14FC[0x1500 - 0x14FC];
    atn::ColliCapsule mCapsules[11]; // 0x1500: inferred: hit capsules (bit 0 of mFlags enables them)
    int m18C8;                       // inferred: sphere (m624) used as the center
    unsigned char m18CC[0x18D0 - 0x18CC];
    int m18D0;
    unsigned char m18D4[0x18F8 - 0x18D4];
    GimmickCommon* m18F8; // inferred: gimmick told (message 5) when mode 12 starts
    atn::ColliSphere m18FC; // inferred: a sphere (0x4C bytes; its destructor is inlined)
    unsigned char m1948[0x1964 - 0x1948];
    atn::LH_Binary m1964;
    atn::EfpFile m196C;
    unsigned char m1978[0x197C - 0x1978];
    unsigned char m197C;
    unsigned char m197D[0x1980 - 0x197D];
    ResHandle m1980; // inferred: debug menu group
    unsigned char m1984[0x1990 - 0x1984];
};

#endif
