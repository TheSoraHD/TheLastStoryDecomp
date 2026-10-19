#ifndef CHR_MOTION_UNK_H
#define CHR_MOTION_UNK_H

#include "math/Vec.h"
#include "atn/Model_unk.h"

namespace chr {

// inferred: data of a part (defined in game/CharaStatus.h)
struct CharaPartData_unk;
// inferred: polymorphic (CharaTask deletes parts through the vtable and calls vf0C, 0x8014FAD0)
struct CharaPart_unk {
    virtual ~CharaPart_unk();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    // inferred: called with the zero vector twice, 0 and 200 when an attack starts (fn_80157444)
    virtual void vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d);
    int fn_8013C43C(); // inferred: accessor called out of line from CharaTask

    int m04;
    int m08;
    unsigned char m00C[0x010 - 0x00C];
    unsigned char m010[0x014 - 0x010]; // inferred: model-like object (passed to fn_80094E2C)
    unsigned int m014; // inferred: flags of that model (0x80149A30 sets 0x4 and 0x400)
    unsigned char m018[0x050 - 0x018];
    Model_unk::Float4_unk m050; // inferred: copied from the character model (CharaTask::fn_80148B38)
    unsigned char m060[0x224 - 0x060];
    MtxPtr m224; // inferred: node matrix followed by the weapon
    unsigned char m228[0x274 - 0x228];
    CharaPartData_unk* m274;
};

// inferred: data block pointed to by CharaTask::m5C (defined in game/CharaStatus.h)
struct CharaData_unk;

// The motion controller embedded in characters at +0xB0 is the engine model class (CharaTask
// constructs it with Model_unk::Model_unk, 0x80135588); the old name is kept as an alias.
typedef ::Model_unk Motion_unk;

} // namespace chr

#endif
