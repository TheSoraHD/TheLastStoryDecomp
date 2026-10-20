#ifndef GMK_CHARAOBJ_H
#define GMK_CHARAOBJ_H

#include "game/GimmickCommon.h"
#include "gmk/GmkChara.h"

namespace gmk {

// inferred: gimmick that places a character ("CHARA" file, "MOTION" and "FACE_MOTION"). The name
// is in the DOL (__vt__Q23gmk8CharaObj, 0x100 bytes).
class CharaObj : public GimmickCommon {
public:
    CharaObj(atn::Task* parent, int id, int arg) : GimmickCommon(parent, id, arg), mChara(0), mHasFace(0), mFC(0.0f) {
        m54 = 0;
    }
    static GimmickCommon* fn_8042CE30(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~CharaObj();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    GmkChara* mChara; // 0xF4
    int mHasFace;     // 0xF8: inferred: a "FACE_MOTION" was loaded
    float mFC;
};

} // namespace gmk

#endif
