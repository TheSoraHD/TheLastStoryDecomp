#ifndef GMK_ITEM_H
#define GMK_ITEM_H

#include "game/GimmickCommon.h"
#include "gmk/DropItem.h"

namespace gmk {

// inferred: gimmick that owns a gmk::DropItem (created with id 18000) and drives its state. The
// name is in the DOL (__vt__Q23gmk4Item, 0x100 bytes).
class Item : public GimmickCommon {
public:
    Item(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDrop(0), mDesc(desc), mTimer(0) {
        m54 = 0;
        mDrop = (DropItem*)fn_804093B4(this, 18000, m4C + 8000);
        mDrop->fn_800D246C(1);
        mDrop->SetPos_unk(mDesc->mPos);
        mDrop->SetRot_unk(atn::Vector3(0.0f, mDesc->mRotY, 0.0f));
        mDrop->vf68(mDesc->m20, 0);
        mDrop->m180 = 0;
    }
    static GimmickCommon* fn_803FBFA8(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Item() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual void vf4C(CharaTask* chara);
    virtual int vf60();

    void fn_803FC65C(); // inferred: resets the drop item (called from 0x804E9360)
    void fn_803FC870(); // inferred: called from BattleCharaTask

    DropItem* mDrop;        // 0xF4
    GimmickDesc_unk* mDesc; // 0xF8
    int mTimer;             // 0xFC: inferred: frames until the item comes back (desc->m28)
};

} // namespace gmk

#endif
