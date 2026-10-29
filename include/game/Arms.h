#ifndef GAME_ARMS_H
#define GAME_ARMS_H

#include "atn/EffectModel_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "math/Vec.h"
#include <boost/scoped_ptr.hpp>

class CharaTask;
struct HitInfo_unk;

// inferred: condition of an equipment (0x14 bytes, two of them at ArmsData_unk::mC4); m08 is the
// kind tested by Arms::fn_80044FBC / fn_80045510 and m0C its parameter. Names invented.
struct ArmsCond_unk {
    ArmsCond_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
};

// inferred: equipment data returned by fn_80206B14 (names invented)
struct ArmsData_unk {
    unsigned char m00[0x80];
    int m80;          // inferred: compared with the condition 8 parameter + 300
    unsigned char m84[0x8C - 0x84];
    char m8C[0x20];   // inferred: name of the .chr file
    int mAC[3];       // inferred: skill ids
    unsigned char mB8[0xC4 - 0xB8];
    ArmsCond_unk mC4[2];
};

// inferred: effect data returned by fn_802072AC (names invented)
struct ArmsEffect_unk {
    unsigned char m00[0x08];
    int m08; // inferred: condition kind
    int m0C; // inferred: condition parameter
    char m10[0x20]; // inferred: name of the .efp file
};

// inferred: weapon object (class name from RTTI, vtable at 0x80777494); the characters keep it as
// chr::CharaPart_unk (CharaTask::m648). Parameters named by the members they are stored in.
class Arms {
public:
    Arms(void* arg04, int arg08, int arg0C, int arg270); // 0x80043EAC
    virtual ~Arms();                                     // 0x80044134
    virtual int vf0C();                                  // 0x8004424C
    virtual void vf10();                                 // 0x80044780
    virtual void vf14();                                 // 0x80044BB0
    // inferred: called by CharaTask with the zero vector twice, 0 and 200 when an attack starts
    // (see chr::CharaPart_unk); only Gun::vf18 reads an argument (d)
    virtual void vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d); // 0x800457A0

    void fn_8004439C();                 // inferred: reads the .chr file
    atn::Vector3 fn_80044BD4();         // inferred: position of the "shot" node (or of the model)
    HitInfo_unk* fn_80044C30(int i);    // inferred: skill i
    int fn_80044C50(CharaTask* chara);  // inferred: the effect condition holds
    void fn_80044CCC();                 // inferred: starts the effect
    int fn_80044E98(int i, int a, int b, int flags, CharaTask* user, CharaTask* chara, HitInfo_unk* hit);
    int fn_80044F54(int i, CharaTask* chara);
    static int fn_80044FBC(ArmsCond_unk* cond, int a, int b, unsigned int flags, CharaTask* user, CharaTask* chara,
                           HitInfo_unk* hit);
    static int fn_80045510(ArmsCond_unk* cond, CharaTask* chara);
    ArmsCond_unk* fn_80045694(int kind, int value, int kind2, int value2); // inferred: finds a condition

    atn::Model* GetModel_unk() { return mModel.GetModel_unk(); }

    void* m04;
    int m08;
    int m0C;
    EffectModel_unk mModel;   // 0x10
    MtxPtr m224;              // inferred: node matrix followed by the weapon
    atn::Matrix3x4 m228;      // inferred: offset matrix applied on top of m224
    atn::Vector3 m258;        // inferred: rotation (degrees in the file)
    atn::Vector3 m264;        // inferred: scale
    int m270;
    ArmsData_unk* m274;
    int m278;                 // inferred: index of the "shot" node
    atn::LH_Binary m27C;      // inferred: the .chr file
    int m284;                 // inferred: load state (3 = loaded)
    int m288;
    int m28C;
    HitInfo_unk* m290[3];
    boost::scoped_ptr<atn::EfpHandle_unk> m29C;
    int m2A0;                 // inferred: the effect is playing
    ArmsEffect_unk* m2A4;
};

#endif
