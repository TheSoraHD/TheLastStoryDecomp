#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/GimmickCommon.h"

extern "C" int rand();
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: AI helper object (lbl_8087EE68, see AI/AiCommon_unk.cpp); type unknown
extern "C" void* lbl_8087EE68;
extern "C" int fn_80018BAC(void* ai, CharaTask* chara);
extern "C" int fn_80018240(void* ai, CharaTask* chara, atn::Vector3* pos, float* radius);
// inferred: collision world and its ray test (as in gmk/Cannonball.cpp)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg);
// inferred: field (lbl_8087F430): route tables at 0x10D8
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: route tables of the field, null without a field (inlined everywhere; name invented)
static inline RouteTable_unk* GetRouteTable_unk() {
    return lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
}

// inferred: 0x1C-byte record at 0x1470 in the player characters (as PlayerState_unk in
// game/CharaTask.cpp; names invented)
struct LabelState_unk {
    LabelState_unk(short id, int kind) : m00(id), m02(0), m04(0), m08(150), m0C(0.0f, 0.0f, 0.0f), m18(kind) {}
    short m00;
    short m02;
    int m04;
    int m08;
    atn::Vector3 m0C;
    int m18;
};
// inferred: player character as seen from here (names invented)
struct LabelPlayer_unk : public CharaTask {
    void SetLabel_unk(const LabelState_unk& state) { m1470 = state; }

    unsigned char m1424[0x1470 - 0x1424];
    LabelState_unk m1470;
    unsigned char m148C[0x14A8 - 0x148C];
    unsigned int m14A8; // inferred: flags
};

CharaC58_unk::CharaC58_unk()
    : m00(0), m04(0), m08(0), mB4(0), mB8(0), mBC(0), mC0(0), mC4(0), mC8(0), mCC(0), mD0(0), mD4(0), mDC(0),
      mHeld_unk(0), mDone_unk(0), mLoop_unk(0), mE0_12(0), mE0_11(0), mE0_10(0), mMode(0), mE4(0), mStep(0), mE8(0),
      mEC(0), mDest(0.0f, 0.0f, 0.0f), mRadius(0.0f), m10C(0), m110(0), m114(0), m118(0), m2B0(0), m2B4(0), m2B8(0) {
    mD8 = 0;
    memset(m140, 0, sizeof(m140));
    memset(m1A0, 0, sizeof(m1A0));
    memset(m1B0, 0, sizeof(m1B0));
    memset(mMotions, 0, sizeof(mMotions));
    m0C.fn_8012539C();
}

void CharaC58_unk::fn_8011BE54() {
    mD8 &= ~6;
    memset(m140, 0, sizeof(m140));
    memset(m1A0, 0, sizeof(m1A0));
    memset(m1B0, 0, sizeof(m1B0));
}

void CharaC58_unk::fn_8011BEB8(int arg) {
    m00 = 0;
    if (arg != 0) {
        mD0 = 0;
    }
    mBC = 0;
    fn_8011C044(0);
    m0C.fn_8012539C();
    mD8 |= 4;
}

atn::Vector3 CharaC58_unk::fn_8011BF3C(int arg) {
    if (mD0 != 0) {
        return mD0->vf54();
    }
    if (mC4 != 0) {
        return mC4->vf64(arg);
    }
    return lbl_807C7030;
}

void CharaC58_unk::fn_8011BF94() {
    fn_8011BEB8(1);
    mB4 = 0;
    mD8 &= ~1;
    mB8 = 0;
    m118 = 0;
    m2B0 = 0;
    mC4 = 0;
    mC8 = 0;
    mD0 = 0;
    mCC = 0;
    mD4 = 0;
    mDC = 0;
    mC0 = 0;
}

void CharaC58_unk::fn_8011C044(int arg) {
    mE4 = mMode;
    mMode = arg;
    mStep = 0;
    mDone_unk = 0;
    mE8 = 0;
    mEC = 0;
    m0C.fn_8012539C();
}

// inferred: the character holds a ledge (name invented)
static inline bool IsOnLedge_unk(CharaTask* chara) {
    return chara->mFlag31 && chara->mC48 != 0;
}

void CharaC58_unk::fn_8011C074(CharaTask* chara, int mode) {
    fn_8011C044(mode);
    if (mMode == 4 || (mMode == 0 && fn_80018BAC(lbl_8087EE68, chara) == 0)) {
        chara->mFlag5 = 1;
        chara->mFC0 = mC4;
    } else {
        chara->mFC0 = 0;
        chara->mFlag5 = 0;
    }
    if (chara->mFlag31) {
        if (mMode != 0 && mMode != 6 && mMode != 7) {
            int side = chara->fn_80153B44();
            if ((side == 1 || side == 2) && chara->fn_8014FCC4() && rand() % 100 < 50) {
                const atn::Vector3* dir;
                if (chara->fn_80153B44() == 1) {
                    atn::Vector3 back(-chara->mC14.x, -chara->mC14.y, -chara->mC14.z);
                    dir = &back;
                } else {
                    dir = &chara->mC14;
                }
                chara->fn_80155790(*dir);
            }
            chara->fn_801539E0();
        }
        if (!IsOnLedge_unk(chara) && mMode == 6) {
            chara->fn_80153FA0();
        } else if (IsOnLedge_unk(chara) && mE4 == 6 && mMode != 6) {
            chara->fn_80154344();
        }
    } else if (!chara->mFlag6 && mMode == 6) {
        chara->fn_80155DAC(1);
    } else if (chara->mFlag6 && mE4 == 6 && mMode != 6 && mMode != 0) {
        chara->fn_80155DAC(0);
    }
}

void CharaC58_unk::fn_8011C2D4(CharaTask* chara, int time) {
    fn_8011C074(chara, 0);
    mBC = time;
}

void CharaC58_unk::fn_8011C314(CharaTask* chara, int start) {
    fn_8011C074(chara, 1);
    m0C.fn_80128508(start, &chara->mPos, 0);
    m0C.fn_801255C8();
}

void CharaC58_unk::fn_8011C378(CharaTask* chara, int link) {
    fn_8011C074(chara, 2);
    if ((mD8 & 4) == 4) {
        mD8 &= ~4;
    }
    m0C.fn_801286F0(link, &chara->mPos, 0);
    m0C.fn_801255C8();
}

void CharaC58_unk::fn_8011C3F4(CharaTask* chara, CharaTask* target, float radius) {
    fn_8011C074(chara, 3);
    mRadius = radius;
    m0C.fn_80128930(target, &chara->mPos, 0);
    m0C.mRadius = radius;
    m0C.fn_801255C8();
}

void CharaC58_unk::fn_8011C46C(CharaTask* chara, int time) {
    fn_8011C074(chara, 4);
    mBC = time;
}

void CharaC58_unk::fn_8011C4AC(CharaTask* chara) {
    if (chara->mType == 3 && chara->fn_8016E484() == 0 && chara->mStatus.m228 >= 1.0f &&
        chara->mStatus.m2D0[3].m[0] > 0) {
        if (mC4 != 0 && mC4->fn_8016F4D8(chara)) {
            mC4->fn_8016F5EC(chara);
        }
        CharaTask* target = mC4;
        fn_8011C074(chara, 7);
        m10C = target;
        if ((unsigned int)chara->mStatus.m2CC > 3) {
            mDC = (SkillRef_unk*)&chara->mStatus.m2D0[3];
        }
    } else {
        fn_8011C074(chara, 5);
        if (mC4 != 0) {
            atn::Vector3 from = chara->mColli.m48;
            atn::Vector3 to = mC4->mColli.m48;
            if (fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0) != 0) {
                mStep = 0;
                m0C.fn_80128930(mC4, &chara->mPos, 0);
                m0C.fn_801255C8();
            } else {
                mStep = 1;
            }
        }
    }
}

void CharaC58_unk::fn_8011C610(CharaTask* chara, int count) {
    fn_8011C074(chara, 6);
    mEC = count;
}

void CharaC58_unk::fn_8011C650(CharaTask* chara, unsigned int skill, CharaTask* target) {
    fn_8011C074(chara, 7);
    m10C = target;
    if (skill < (unsigned int)chara->mStatus.m2CC) {
        mDC = (SkillRef_unk*)&chara->mStatus.m2D0[skill];
    }
}

void CharaC58_unk::fn_8011C6C4(CharaTask* chara, int place, float radius) {
    fn_8011C074(chara, 8);
    mPlace = place;
    mRadius = radius;
    RouteTable_unk* table = GetRouteTable_unk();
    RoutePlace_unk* p = table != 0 ? table->FindPlace_unk(place) : 0;
    if (p != 0) {
        mDest = p->mPos;
    } else {
        mDest = chara->mPos;
    }
}

void CharaC58_unk::fn_8011C7C0(CharaTask* chara, CharaTask* target, float radius) {
    fn_8011C074(chara, 9);
    mCC = target;
    mRadius = radius;
    if (target != 0) {
        mDest = target->mPos;
    } else {
        mDest = chara->mPos;
    }
}

void CharaC58_unk::fn_8011C850(CharaTask* chara) {
    fn_8011C074(chara, 15);
    atn::Vector3 pos;
    float radius;
    if (fn_80018240(lbl_8087EE68, chara, &pos, &radius)) {
        mDest = pos;
        mRadius = 0.75f * radius;
    } else {
        mStep = -1;
        mDone_unk = 1;
    }
}

void CharaC58_unk::fn_8011C8E4(CharaTask* chara) {
    fn_8011C074(chara, 10);
}

void CharaC58_unk::fn_8011C8EC(CharaTask* chara, int place) {
    fn_8011C074(chara, 11);
    mPlace = place;
}

void CharaC58_unk::fn_8011C92C(CharaTask* chara, int place, float radius) {
    fn_8011C074(chara, 12);
    mPlace = place;
    mRadius = radius;
}

void CharaC58_unk::fn_8011C97C(CharaTask* chara, CharaTask* target, float radius) {
    fn_8011C074(chara, 13);
    m10C = target;
    mRadius = radius;
    chara->mC58.m0C.fn_80128930(target, &chara->mPos, 0);
    chara->mC58.m0C.mRadius = radius;
    chara->mC58.m0C.fn_801255C8();
}

void CharaC58_unk::fn_8011C9F8(CharaTask* chara) {
    fn_8011C074(chara, 14);
}

void CharaC58_unk::fn_8011CA00(CharaTask* chara) {
    fn_8011C074(chara, 16);
}

void CharaC58_unk::fn_8011CA08(CharaTask* chara, CharaTask* target, float radius) {
    fn_8011C074(chara, 17);
    mCC = target;
    m10C = target;
    mRadius = radius;
}

void CharaC58_unk::fn_8011CA5C(CharaTask* chara, CharaTask* target, float radius, float angle) {
    fn_8011C074(chara, 18);
    mRadius = radius;
    m0C.fn_80128C08(target, &chara->mPos, 0, angle);
    m0C.mRadius = radius;
    m0C.fn_801255C8();
}

// inferred: the type of the character is 1 or 4 (name invented)
static inline bool IsType14_unk(CharaTask* chara) {
    return chara->mType == 1 || chara->mType == 4;
}

// NONMATCHING: 99.46% - registers in the spot search loop (the data pointer and the offset swap
// r5/r6, the character id and the spot id swap r0/r3); tried the comparison both ways, an id local,
// an element pointer and an inline find helper
void CharaC58_unk::fn_8011CAE4(CharaTask* chara, float radius) {
    RouteTable_unk* table = GetRouteTable_unk();
    if (table != 0) {
        RouteSpots_unk* spots = 0;
        if (IsType14_unk(chara)) {
            spots = &table->m80[0];
        } else if (chara->mType == 2) {
            spots = &table->m80[1];
        } else if (chara->mType == 3) {
            spots = &table->m80[2];
        }
        if (spots != 0) {
            RouteSpot_unk* spot = 0;
            for (unsigned int i = 0; i < spots->mCount; i++) {
                if (spots->mSpots[i].mId == chara->m58) {
                    spot = &spots->mSpots[i];
                    break;
                }
            }
            if (spot != 0) {
                fn_8011C074(chara, 19);
                mRadius = radius;
                m0C.fn_80128A30(&spot->mPos, &chara->mPos, 0, spot->mRadius);
                m0C.mRadius = radius;
                m0C.fn_801255C8();
            }
        }
    }
}

void CharaC58_unk::fn_8011CC30(CharaTask* chara, int motion, int loop) {
    fn_8011C074(chara, 20);
    mMotions[0] = motion;
    mMotions[1] = mMotions[2] = mMotions[3] = -1;
    mLoop_unk = loop;
    mEC = 1;
}

void CharaC58_unk::fn_8011CCA0(CharaTask* chara, int motion0, int motion1, int loop) {
    fn_8011C074(chara, 21);
    mMotions[0] = motion0;
    mMotions[1] = motion1;
    mMotions[2] = mMotions[3] = -1;
    mLoop_unk = loop;
    mEC = 2;
}

void CharaC58_unk::fn_8011CD1C(CharaTask* chara, int motion0, int motion1, int motion2, int loop) {
    fn_8011C074(chara, 22);
    mMotions[0] = motion0;
    mMotions[1] = motion1;
    mMotions[2] = motion2;
    mMotions[3] = -1;
    mLoop_unk = loop;
    mEC = 3;
}

// inferred: the character is following a route (name invented)
static inline bool IsRouting_unk(CharaTask* chara) {
    return chara->m140C >= 0 && chara->mFlagB14;
}

// NONMATCHING: 97.97% - registers (the copy of the temporary state, the spot loop of kind 6, the
// order of the two collision radius loads) and one dead branch after the radius clamp of kind 4
// that the original keeps (tried if/else and else-block forms of the clamp)
void CharaD74_unk::fn_8011CD84(CharaTask* chara, int kind) {
    if (chara != 0 && chara->mStatus.IsState_unk(0x80)) {
        ((LabelPlayer_unk*)chara)->SetLabel_unk(LabelState_unk(12, kind));
        return;
    }
    int prev = m00;
    m00 = kind;
    m02 = 0;
    m04 = 0;
    m20 &= ~1;
    if (IsRouting_unk(chara)) {
        chara->fn_80171DB0();
        chara->fn_8017BC4C();
    }
    switch (m00) {
    case 0:
        ((LabelPlayer_unk*)chara)->m14A8 |= 0x4000000;
        m01 = m00;
        break;
    case 2:
        if (prev != 3) {
            chara->fn_801656A4(60);
        }
        m06 = 0;
        break;
    case 3:
        chara->fn_801656A4(60);
        chara->m12C8 = 10;
        break;
    case 4: {
        CharaTask* player = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
        if (player != 0) {
            if (m08 > 0) {
                chara->fn_8017039C(m08, 0);
            } else {
                float dist = PSVECMag(&(player->mPos - chara->mPos));
                float radius = 3.0f + (player->m5B0 + chara->m5B0);
                if (chara->mC58.m11C.m1C > 0.0f) {
                    if (m06 < 1) {
                        radius = 0.2f * dist > 30.0f ? 0.2f * dist : 30.0f;
                    }
                } else if (m06 < 1) {
                    if (dist < 60.0f) {
                        radius = 30.0f > radius ? 30.0f : radius;
                    } else {
                        radius = 0.4f * dist > radius ? 0.4f * dist : radius;
                    }
                }
                if (dist < 90.0f) {
                    m20 |= 1;
                }
                chara->fn_80170F20(&player->mPos, 8, 0.0f, radius);
            }
            chara->m12C8 = 10;
            m06++;
        } else {
            fn_8011CD84(chara, 3);
        }
        break;
    }
    case 5:
        chara->fn_801656A4(90);
        break;
    case 6:
        if (chara->m1400.mSize >= 2 && chara->fn_8017B3C0()) {
            chara->fn_8017B434();
            m00 = 2;
            m06 = 0;
        } else {
            RouteTable_unk* table = GetRouteTable_unk();
            if (table != 0) {
                for (unsigned int i = 0; i < table->m80[1].mCount; i++) {
                    RouteSpot_unk* spot = &table->m80[1].mSpots[i];
                    if (spot->mId == chara->m58) {
                        chara->fn_80170F20(&spot->mPos, 0, spot->mRadius, -1.0f);
                        m20 |= 1;
                        break;
                    }
                }
            }
        }
        break;
    case 1:
        chara->fn_80171DB0();
        if (chara->mStatus.IsState_unk(0x10)) {
            chara->fn_8016EB48(0, 0, 0);
            chara->fn_80167C34();
        } else if (!chara->mStatus.IsState_unk(0x40)) {
            chara->fn_8016EB48(0, 0, 0);
        }
        ((LabelPlayer_unk*)chara)->m14A8 |= 0x4000000;
        if (IsFlag_unk(2)) {
            SetFlag_unk(4);
        } else {
            ClearFlag_unk(4);
        }
        if (!IsFlag_unk(4)) {
            chara->m12C8 = 10;
        }
        break;
    }
}

int CharaD74_unk::fn_8011D1FC() {
    return 1 <= m00;
}

int CharaD74_unk::fn_8011D21C() {
    return !fn_8011D1FC() || m00 == 1;
}
