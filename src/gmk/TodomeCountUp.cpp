#include "gmk/TodomeCountUp.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"
#include "game/ResHandle.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
// inferred: plays the sound `name` at `pos`
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087F430;
extern "C" void fn_80370AE4(void* field, int index, int value); // inferred: sets a counter

namespace gmk {

GimmickCommon* TodomeCountUp::fn_8042F0B4(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) TodomeCountUp(parent, desc);
    }
    return 0;
}

TodomeCountUp::TodomeCountUp(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc) {
    mNumNames = 0;
    m54 = 0;
    switch (mDesc->m20) {
    case 0:
        mCharaId = -1;
        mLoader.vf0C("/database/chara/special/db_pc_todome.csv");
        break;
    case 1:
        mCharaId = mDesc->m24;
        break;
    case 2:
        mCharaId = mDesc->m24;
        break;
    case 3:
        mCharaId = mDesc->m24;
        mLoader.vf0C("/database/chara/special/db_fr_todome.csv");
        break;
    }
}

int TodomeCountUp::vf1C() {
    return mLoader.fn_80473F50() != 0;
}

void TodomeCountUp::vf24() {
    if (mLoader.fn_80470580() != 0) {
        atn::Csv csv((const char*)mLoader.fn_80470580(), mLoader.fn_8047059C());
        do {
            const char* field = csv.fn_8005B9CC();
            if (field[0] != ';' && field[0] != '\0') {
                Name_unk name;
                strcpy(name.mName, field);
                new (&mNames[mNumNames]) Name_unk(name);
                mNumNames++;
            }
        } while (csv.fn_8005BBF8() != 0);
    }
}

int TodomeCountUp::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 0;
        return 1;
    }
    return 0;
}

void TodomeCountUp::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    switch (mDesc->m20) {
    case 0:
        mTarget = lbl_8087F8A0->mHead;
        break;
    case 1:
        mCharaId = mDesc->m24;
        mTarget = NpcManager::sInstance->Find_unk(mCharaId);
        break;
    case 2:
        mTarget = EnemyManager::sInstance->Find_unk(mCharaId);
        break;
    case 3:
        mTarget = lbl_8087F8A0->Find_unk(mCharaId);
        break;
    }
    m104 = mDesc->m28;
    mCount = 0;
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

// NONMATCHING: the target keeps a stack frame (saves this in r31) and calls Start with bctrl
// instead of a tail call, as if dead code followed. Tried: switch, local copies, empty inline and
// out-of-line calls, dead loads, dead conditions, GimmickCommon::vf38 checks.
void TodomeCountUp::vf10() {
    if (m54 == 0) {
        Start();
    }
}

void TodomeCountUp::vf14() {}

int TodomeCountUp::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        mCount = 0;
        m54 = msg->mType;
        break;
    case 2:
        mCount = 0;
        m54 = msg->mType;
        break;
    case 3:
        if ((void*)msg->m10 == mTarget && m54 == 2) {
            fn_800C344C(mNames[mCount].mName, (char*)msg->m10 + 0x528, 0, -1, 1.0f);
            mCount++;
            if (lbl_8087F430 != 0 && m104 > 0) {
                fn_80370AE4(lbl_8087F430, m104, mCount);
            }
        }
        break;
    }
    return m54;
}

int TodomeCountUp::vf60() {
    int ret = 0;
    if (m54 >= 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

void TodomeCountUp::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
}

} // namespace gmk
