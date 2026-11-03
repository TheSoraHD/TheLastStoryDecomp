// Event/Puppeteer_CharaTask.cpp (0x8000CBD0-0x800142C4): Event::Puppeteer<CharaTask>, Event::Puppeteer<atn::Model>
// and Event::Actor, the actors of an event sequence.
#include "Event/Puppeteer.h"
#include "Event/Actor.h"
#include "Event/Sequence.h"
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"
#include "atn/Model_unk.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace Event {


template <>
int Puppeteer<CharaTask>::vf0C() {
    return 1;
}

// NONMATCHING: stub - places the character on the matrix m54 (rotations of the euler angles)
template <>
void Puppeteer<CharaTask>::vf10() {
}

template <>
void Puppeteer<CharaTask>::vf14() {
    if (m50 == 0) {
        return;
    }
    if (m50->m194 != 10) {
        bool ending = m50->m194 == 11 || m50->m194 == 12;
        if (!ending) {
            return;
        }
    }
    CharaTask* chara = m48;
    if (chara != 0 && chara->m55C == 8 && chara->IsFlag_unk(4)) {
        chara->fn_80149A30();
    }
}

template <>
int Puppeteer<atn::Model>::vf0C() {
    return 1;
}

// NONMATCHING: stub - computes the matrix stack of the model
template <>
void Puppeteer<atn::Model>::vf10() {
}

template <>
void Puppeteer<atn::Model>::vf14() {
    if (m4C_0 != 1) {
        return;
    }
    if (m50 == 0) {
        return;
    }
    if (m50->m194 != 10) {
        bool ending = m50->m194 == 11 || m50->m194 == 12;
        if (!ending) {
            return;
        }
    }
    m48->fn_8008CD60();
}

// NONMATCHING: registers of the vtable and of the memset arguments (r3/r4 swapped)
Actor::Actor(Sequence* owner, const char* name, int id)
    : CommonData((DataOwner_unk*)owner, name, id), m20(0), m58(-1) {
    m1C = 0;
    m18 = 0;
    mB8[0] = 0;
    mB8[1] = 0;
    mB8[2] = 0;
    mC4 = lbl_807C7030;
    mD0 = lbl_807C7030;
    mDC = lbl_807C7030;
    mE8 = lbl_807C7030;
    mF4 = lbl_807C7030;
    m100 = lbl_807C7030;
    m10C = lbl_807C7030;
    m130 = -1;
    m134 = -1;
    m144 = 1;
    m145 = 1;
    m146 = 1;
    m188 = 1;
    m154 = 0;
}

// inferred: the character driven by a character actor (kinds 1 and 7)
inline CharaTask* Actor::GetChara_unk() {
    return ((Puppeteer<CharaTask>*)m1C)->m48;
}

void Actor::fn_80012540(int arg) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->fn_80176DFC(arg, arg, 0);
    }
}

// NONMATCHING: registers of the case 3 (r3/r4/r5)
void Actor::fn_8001256C(int show) {
    switch (m20) {
    case 7:
    case 1:
        GetChara_unk()->fn_8016E5F0(show);
        break;
    case 2:
        ((GimmickCommon*)m1C)->m9C = show;
        break;
    case 3:
        ((Puppeteer<atn::Model>*)m1C)->SetFlag0_unk(show);
        break;
    }
}

void Actor::fn_800125C8(int on) {
    if (m20 == 7 || m20 == 1) {
        CharaTask* chara = GetChara_unk();
        if (on != 0) {
            chara->mColli.mFlags |= 1;
        } else {
            chara->mColli.mFlags &= ~1;
        }
    }
}

void* Actor::fn_8001260C() {
    if (m20 == 7 || m20 == 1) {
        return &GetChara_unk()->mColli;
    }
    return 0;
}

void Actor::fn_80012638(int on) {
    if (m20 == 7 || m20 == 1) {
        if (on != 0) {
            GetChara_unk()->vf98();
        } else {
            GetChara_unk()->vf9C(0);
        }
    }
}

void Actor::fn_8001271C(int on) {
    switch (m20) {
    case 7:
    case 1:
        GetChara_unk()->m1220.m10 = on != 0;
        break;
    case 2: {
        GimmickCommon* gimmick = (GimmickCommon*)m1C;
        gimmick->mB0.mEnabled = on;
        if (gimmick->mE4 != 0) {
            gimmick->mE4->m50 = on;
        }
        break;
    }
    }
}

void Actor::fn_80012774(int on) {
    switch (m20) {
    case 7:
    case 1:
        GetChara_unk()->m1220.m11 = on != 0;
        break;
    case 2: {
        GimmickCommon* gimmick = (GimmickCommon*)m1C;
        gimmick->mB0.mSoundOn = on;
        if (gimmick->mE4 != 0) {
            gimmick->mE4->m54 = on;
        }
        break;
    }
    }
}

int Actor::fn_800127CC(int id, int b) {
    if (id == -1 || b == -1) {
        return 0;
    }
    FixedArray_unk<chr::CharaPart_unk*, 8>& parts = GetChara_unk()->mParts;
    for (int i = 0; i < (int)parts.mCount; i++) {
        if (id == parts.mData[(unsigned int)i]->m04) {
            return 1;
        }
    }
    return 0;
}

CharaTask* Actor::fn_800128FC() {
    if (m20 == 7 || m20 == 1) {
        if (m1C != 0) {
            return GetChara_unk();
        }
        return 0;
    }
    return 0;
}

CharaTask* Actor::fn_80012934() {
    if (m20 == 7 || m20 == 1) {
        if (m1C != 0) {
            return GetChara_unk();
        }
        return 0;
    }
    return 0;
}

void Actor::fn_80012CBC(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_80129954(pos, speed);
    }
}

void Actor::fn_80012CE4(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_80129930(pos, speed);
    }
}

void Actor::fn_80012D0C(float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_80129A48(speed);
    }
}

void Actor::fn_800134B8(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012ABD0(pos, speed);
    }
}

void Actor::fn_800134E0(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012ABF4(pos, speed);
    }
}

void Actor::fn_80013508(float* target, const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012AC18(target, pos, speed);
    }
}

void Actor::fn_80013530(float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012AC40(speed);
    }
}

} // namespace Event
