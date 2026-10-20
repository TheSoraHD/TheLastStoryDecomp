// inferred file name (splits-auto): AI helpers of the scripts of AI/AI_archer_attack01.cpp. They
// work on the current character (lbl_8087EE60) and on lists of 16 characters.
#include "game/CharaTask.h"
#include "game/NpcManager.h"

extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_802180A8(const char* name);        // inferred: motion id from its name
extern "C" atn::Vector3 lbl_807C7030;                // inferred: zero vector

// inferred: current character and its list of 16 known characters (set by AI/AiUtil_unk)
struct AiList_unk {
    unsigned char m000[0x24C];
    unsigned char mCount; // 0x24C
    CharaTask* mCharas[16]; // 0x250
};
extern "C" CharaTask* lbl_8087EE60;
extern "C" AiList_unk* lbl_8087EE64;
// inferred: AI helper object (type unknown)
extern "C" void* lbl_8087EE68;
extern "C" int fn_800197A4(void* ai, atn::Vector3* out, int* count, CharaTask* chara, const atn::Vector3* dir,
                           int arg, float angle);
extern "C" int fn_80018C80(void* ai, int arg, float angle);
// inferred: game settings (lbl_8087F0A8), only the field read here
struct AiSettings_unk {
    unsigned char m000[0x468];
    float m468; // inferred: factor applied to the wait time of characters with CharaTask::mFlag10
};
extern "C" AiSettings_unk* lbl_8087F0A8;
// inferred: field (lbl_8087F430)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int index); // inferred: reads a value of the field
extern "C" int fn_803C1560(RouteTable_unk* table, int a, int b, int c, int d, int e, float f);
// inferred: enemy manager (lbl_8087F048); slots of 0x934 bytes (as in game/ArenaTask.cpp)
struct AiEnemySlot_unk {
    CharaTask* mChara;
    unsigned char m04[0x934 - 0x4];
};
struct AiEnemyMgr_unk {
    unsigned char m00[0xCBF0];
    AiEnemySlot_unk mSlots[72]; // 0xCBF0
    unsigned char m36290[0x363B0 - 0x36290];
    int mNumSlots; // 0x363B0
};
extern "C" AiEnemyMgr_unk* lbl_8087F048;
// inferred: same object as lbl_8087F048 seen from game/CharaTask.cpp (names invented)
struct Effect_unk {
    CharaTask* fn_8010C948(CharaTask* chara); // inferred: a target for the character
};
// inferred: team manager (lbl_8087FA00); members of 0x37C bytes
struct AiTeamMember_unk {
    unsigned char m000[0x218];
    struct Ref_unk {
        unsigned char m00[0x8C];
        CharaTask* m8C;
    }* m218;
    unsigned char m21C[0x37C - 0x21C];
    // inferred: character of the member (name invented)
    CharaTask* GetChara_unk() { return m218 != 0 ? m218->m8C : 0; }
};
struct AiTeam_unk {
    unsigned char m00[0x48];
    int mCount;               // 0x48
    AiTeamMember_unk* mMembers; // 0x4C
};
extern "C" AiTeam_unk* lbl_8087FA00;
extern "C" int fn_8003F440(CharaTask* chara, CharaStatusPair_unk* skill);

// inferred: moves the characters of a list of 16 to the front (names invented)
static inline void Compact_unk(CharaTask** list) {
    for (int i = 0; i < 16; i++) {
        if (list[i] == 0) {
            for (int j = i + 1; j < 16; j++) {
                if (list[j] != 0) {
                    list[i] = list[j];
                    list[j] = 0;
                    break;
                }
            }
            if (list[i] == 0) {
                return;
            }
        }
    }
}

extern "C" int fn_8001A444(int index) {
    if (lbl_8087F430 != 0) {
        return fn_80370174(lbl_8087F430, index);
    }
    return 0;
}

// inferred: accessor of the list (it is reloaded for every element)
static inline AiList_unk* GetList_unk() {
    return lbl_8087EE64;
}

// NONMATCHING: 92.79% - the original reloads lbl_8087EE64 inside the copy loop (ours reuses the
// load of the condition); tried an inline accessor and a pointer walk
extern "C" void fn_8001A464(CharaTask** list) {
    int i;
    for (i = 0; i < GetList_unk()->mCount && i < 16; i++) {
        list[i] = GetList_unk()->mCharas[i];
    }
    for (; i < 16; i++) {
        list[i] = 0;
    }
}

extern "C" void fn_8001A510(CharaTask** list) {
    CharaTask* self = lbl_8087EE60;
    int count = 0;
    AiEnemySlot_unk* slots = lbl_8087F048->mSlots;
    for (int i = 0; i < lbl_8087F048->mNumSlots; i++) {
        CharaTask* chara = slots[i].mChara;
        if (chara != 0 && chara->IsActive_unk() && self->fn_8016F3D0(chara, 1) &&
            (self->mC58.mB4 <= 0 || chara->mC58.mB4 <= 0 || self->mC58.mB4 == chara->mC58.mB4)) {
            list[count] = chara;
            count++;
            if (count >= 16) {
                break;
            }
        }
    }
    for (; count < 16; count++) {
        list[count] = 0;
    }
}

// NONMATCHING: 98.87% - registers (the character and the three inline bools)
extern "C" void fn_8001A65C(CharaTask** list) {
    for (int i = 0; i < 16; i++) {
        if (list[i] == 0 || !list[i]->CanAct_unk()) {
            list[i] = 0;
        }
    }
    Compact_unk(list);
}

// NONMATCHING: 98.10% - registers of the team loop (list pointer, team, offset)
extern "C" void fn_8001A788(CharaTask** list) {
    for (int i = 0; i < 16; i++) {
        bool found = false;
        if (lbl_8087EE60 != 0 && !lbl_8087EE60->mStatus.IsState_unk(0x800)) {
            AiTeam_unk* team = lbl_8087FA00;
            for (int j = 0; j < team->mCount; j++) {
                if (list[i] == team->mMembers[j].GetChara_unk()) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            list[i] = 0;
        }
    }
    Compact_unk(list);
}

// inferred: hit points (0) or the second gauge (1) in percent (name invented)
static inline int GetRate_unk(CharaTask* chara, int kind) {
    int rate = -1;
    switch (kind) {
    case 0:
        rate = 100.0f * (chara->mStatus.m04 / chara->mStatus.mParamB.m84);
        break;
    case 1:
        rate = 100.0f * (chara->mStatus.m08 / chara->mStatus.mParamB.m88);
        break;
    }
    return rate;
}

// inferred: keeps only the character with the lowest (mode 0) or highest (mode 1) rate
extern "C" void fn_8001A8A4(int kind, int mode, CharaTask** list) {
    int best = -1;
    switch (mode) {
    case 0: {
        int min = 99999;
        for (int i = 0; i < 16; i++) {
            if (list[i] != 0 && GetRate_unk(list[i], kind) < min) {
                best = i;
                min = GetRate_unk(list[i], kind);
            }
        }
        break;
    }
    case 1: {
        int max = 0;
        for (int i = 0; i < 16; i++) {
            if (list[i] != 0 && GetRate_unk(list[i], kind) > max) {
                best = i;
                max = GetRate_unk(list[i], kind);
            }
        }
        break;
    }
    }
    if (best >= 0) {
        for (int i = 0; i < 16; i++) {
            if (i != best) {
                list[i] = 0;
            }
        }
    }
    Compact_unk(list);
}

// inferred: hit points of the 16 characters in percent of their maximum
extern "C" int fn_8001AC54(CharaTask** list) {
    float hp = 0.0f;
    float max = 0.0f;
    for (int i = 0; i < 16; i++) {
        if (list[i] != 0) {
            hp += list[i]->mStatus.m04;
            max += list[i]->mStatus.mParamB.m84;
        }
    }
    if (atn::Abs_unk(max) < 1.1920929e-7f) {
        return 0;
    }
    return 100.0f * (hp / max);
}

extern "C" float fn_8001AD80(CharaTask* a, CharaTask* b) {
    float dist = 0.0f;
    if (a != 0 && b != 0) {
        dist = PSVECMag(&(a->mPos - b->mPos));
    }
    return dist;
}

// inferred: distance from the character to the place `id` of the route tables
extern "C" float fn_8001ADE8(int id, CharaTask* chara) {
    float dist = 0.0f;
    RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    RoutePlace_unk* place = table != 0 ? table->FindPlace_unk(id) : 0;
    if (place != 0 && chara != 0) {
        dist = PSVECMag(&(chara->mPos - place->mPos));
    }
    return dist;
}

extern "C" NpcTask* fn_8001AEBC(int id) {
    NpcTask* npc = 0;
    if (NpcManager::sInstance != 0) {
        npc = NpcManager::sInstance->Find_unk(id);
    }
    return npc;
}

extern "C" int fn_800142D4(CharaTask* chara);
extern "C" int fn_800143E8(CharaTask* chara);

#define AI_MAX(a, b) ((a) > (b) ? (a) : (b))

// inferred: runs the order `order` of the AI script on the current character
// NONMATCHING: 98.96% - registers: the character and its CharaC58_unk swap r30/r31 (tried both
// declaration orders, a reference and no local), and the original reads b/a from r5/r4 in some cases
extern "C" void fn_8001AEDC(unsigned int order, int* a, int* b, int* c, int* d) {
    CharaTask* self = lbl_8087EE60;
    CharaC58_unk* ai = &self->mC58;
    switch (order) {
    case 0: {
        CharaStatus* status;
        int time = *a;
        if (self->mType == 3) {
            time = AI_MAX(time + fn_800142D4(self), 0);
        } else if (self->mType == 2) {
            time = AI_MAX(time + fn_800143E8(self), 0);
        }
        status = &self->mStatus;
        if (status->fn_80134270() || status->fn_80134290() || status->fn_801342B0()) {
            time += status->fn_80151188() ? 60 : 0;
        }
        if (status->fn_80134134(0x1A) != 0) {
            time = AI_MAX((int)(1.5f * time), 60);
        }
        ai->fn_8011C2D4(self, time);
        break;
    }
    case 1:
        ai->fn_8011C314(self, *a);
        break;
    case 2:
        ai->fn_8011C378(self, *a);
        break;
    case 3:
        ai->fn_8011C3F4(self, (CharaTask*)*a, *b);
        break;
    case 4: {
        CharaTask* other = (CharaTask*)*a;
        int time = *b;
        if (other != 0 && other->mFlag10) {
            time = time * lbl_8087F0A8->m468;
        }
        if (self->mType == 3) {
            time = AI_MAX(time + fn_800142D4(self), 0);
        } else if (self->mType == 2) {
            time = AI_MAX(time + fn_800143E8(self), 30);
        }
        if (self->mStatus.fn_80134134(0x1A) != 0) {
            time = AI_MAX((int)(1.5f * time), 60);
        }
        ai->fn_8011C46C(self, time);
        break;
    }
    case 5:
        ai->fn_8011C4AC(self);
        break;
    case 6:
        ai->fn_8011C610(self, *a);
        break;
    case 7:
        ai->fn_8011C650(self, *a, (CharaTask*)*b);
        break;
    case 8:
        ai->fn_8011C6C4(self, *a, *b);
        break;
    case 9:
        ai->fn_8011C7C0(self, (CharaTask*)*a, *b);
        break;
    case 15:
        ai->fn_8011C850(self);
        break;
    case 10:
        ai->fn_8011C8E4(self);
        break;
    case 11:
        ai->fn_8011C8EC(self, *a);
        break;
    case 12:
        ai->fn_8011C92C(self, *a, *b);
        break;
    case 13:
        ai->fn_8011C97C(self, (CharaTask*)*a, *b);
        break;
    case 14:
        ai->fn_8011C9F8(self);
        break;
    case 16:
        ai->fn_8011CA00(self);
        break;
    case 17:
    {
        float radius = *a;
        ai->fn_8011CA08(self, ((Effect_unk*)lbl_8087F048)->fn_8010C948(self), radius);
    }
        break;
    case 20:
        ai->fn_8011CC30(self, fn_802180A8((const char*)*a), *b);
        break;
    case 21: {
        int motion0 = fn_802180A8((const char*)*a);
        int motion1 = fn_802180A8((const char*)*b);
        ai->fn_8011CCA0(self, motion0, motion1, *c);
        break;
    }
    case 22: {
        int motion0 = fn_802180A8((const char*)*a);
        int motion1 = fn_802180A8((const char*)*b);
        int motion2 = fn_802180A8((const char*)*c);
        ai->fn_8011CD1C(self, motion0, motion1, motion2, *d);
        break;
    }
    case 18:
        ai->fn_8011CA5C(self, (CharaTask*)*a, *b, 0.017453292f * *c);
        break;
    case 19:
        ai->fn_8011CAE4(self, *a);
        break;
    }
}

extern "C" int fn_8001B4A4() {
    float rate = lbl_8087EE60->m568;
    return 100.0f * rate;
}

extern "C" void fn_8001B4CC(int value) {
    lbl_8087EE60->m568 = value / 100.0f;
}

// inferred: plays the motion `motion` on the current character, looking at the neck of `target`
// NONMATCHING: 99.88% - .sdata2 order: the original has -1.0f before the 1.5f of fn_8001AEDC
extern "C" void fn_8001B508(const char* motion, CharaTask* target) {
    if (target != 0) {
        MtxPtr neck = target->GetModel_unk()->GetNodeMtx_unk("neck");
        lbl_8087EE60->fn_80161880(fn_802180A8(motion), (int)neck, 0);
    } else {
        lbl_8087EE60->fn_80161570(fn_802180A8(motion), 0, 0, -1.0f, -1.0f);
    }
}

// NONMATCHING: 99.50% - .sdata2 order of -1.0f (see fn_8001B508)
extern "C" int fn_8001B5B4(int a, int b) {
    return fn_803C1560(lbl_8087F430->m10D8, a, 0, b, 0, 1, -1.0f);
}

extern "C" int fn_8001B5DC(CharaTask* chara) {
    atn::Vector3 out(0.0f, 0.0f, 0.0f);
    int count = 0;
    return fn_800197A4(lbl_8087EE68, &out, &count, chara, &lbl_807C7030, 0, 180.0f);
}

// inferred: position of the character the AI follows (its own position without one)
static inline const atn::Vector3& GetTargetPos_unk(CharaTask* chara) {
    return chara->mC58.mC4 != 0 ? chara->mC58.mC4->mPos : chara->mPos;
}

extern "C" int fn_8001B634(CharaTask* chara) {
    atn::Vector3 out(0.0f, 0.0f, 0.0f);
    int count = 0;
    atn::Vector3 target = GetTargetPos_unk(chara);
    atn::Vector3 dir = target - chara->mPos;
    if (!(atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
    }
    return fn_800197A4(lbl_8087EE68, &out, &count, chara, &dir, 0, 30.0f);
}

extern "C" int fn_8001B714(CharaTask* chara) {
    atn::Vector3 out(0.0f, 0.0f, 0.0f);
    int count = 0;
    atn::Vector3 target = GetTargetPos_unk(chara);
    atn::Vector3 dir = target - chara->mPos;
    if (!(atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
    }
    atn::Vector3 side;
    atn::Vector3 pos;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    PSVECCrossProduct(&up, &dir, &side);
    float sign = 2.0f * (rand() % 2) - 1.0f;
    pos = side * 2.0f * sign + dir;
    int ret = fn_800197A4(lbl_8087EE68, &out, &count, chara, &pos, 0, 30.0f);
    if (ret == 0) {
        pos = side * 2.0f * -sign + dir;
        ret = fn_800197A4(lbl_8087EE68, &out, &count, chara, &pos, 0, 30.0f);
    }
    return ret;
}

extern "C" int fn_8001B984(CharaTask* chara) {
    atn::Vector3 out(0.0f, 0.0f, 0.0f);
    int count = 0;
    atn::Vector3 target = GetTargetPos_unk(chara);
    atn::Vector3 dir = target - chara->mPos;
    if (!(atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
    }
    atn::Vector3 back(-dir.x, -dir.y, -dir.z);
    return fn_800197A4(lbl_8087EE68, &out, &count, chara, &back, 0, 60.0f);
}

extern "C" int fn_8001BA88(int arg, float angle) {
    if (angle < 0.0f) {
        angle = 360.0f;
    }
    return fn_80018C80(lbl_8087EE68, arg, angle);
}

extern "C" int fn_8001BAA4(CharaTask* chara, int bit) {
    bool ret = false;
    if (chara != 0 && bit >= 0) {
        ret = (chara->mC58.mD8 & (1 << bit)) == (1 << bit);
    }
    return ret;
}

extern "C" void fn_8001BADC(CharaTask* chara, int bit) {
    if (chara != 0 && bit >= 0) {
        chara->mC58.mD8 |= 1 << bit;
    }
}

// inferred: 0 when `target` is in reach of one of the two skills of slot `slot`
extern "C" int fn_8001BB04(CharaTask* chara, int slot, CharaTask* target) {
    int ret = 1;
    if (chara != 0 && target != 0 && slot < 4) {
        float dist = PSVECMag(&(target->mPos - chara->mPos));
        const int* ids = chara->mStatus.m2D0[slot].m;
        for (int i = 0; i < 2; i++) {
            int id = ids[i];
            if (id > 0) {
                HitInfo_unk* skill = fn_80219E6C(id);
                if (skill != 0) {
                    float reach = skill->m01 == 0 ? skill->m40 + chara->mStatus.mParamB.m28 : skill->m44;
                    if (dist >= 0.9f * reach) {
                        ret = 0;
                        break;
                    }
                }
            }
        }
    }
    return ret;
}

extern "C" int fn_8001BC38(CharaTask* chara, int slot) {
    int ret = 0;
    if (chara != 0 && slot < 4) {
        ret = fn_8003F440(chara, &chara->mStatus.m2D0[slot]);
    }
    return ret;
}

extern "C" int fn_8001BC80(CharaTask* chara, int index) {
    int ret = 0;
    if (chara != 0) {
        ret = chara->fn_80176428(index);
    }
    return ret;
}

extern "C" int fn_8001BCB4(CharaTask* chara, int slot) {
    bool ret = false;
    if (chara != 0 && slot < 4 && chara->mStatus.m2D0[slot].m[0] > 0 && fn_80219E6C(chara->mStatus.m2D0[slot].m[0]) != 0) {
        ret = true;
    }
    return ret;
}

// inferred: reach of the weapon in hand
extern "C" float fn_8001BD14(CharaTask* chara) {
    float reach = 0.0f;
    if (chara != 0 && chara->m648 != 0 && chara->m64 != 0) {
        switch (chara->m648->m274->mParam.m78) {
        case 0:
            reach = chara->m64->m30;
            break;
        case 1:
            reach = chara->m64->m34;
            break;
        case 2:
            reach = chara->m64->m38;
            break;
        }
        if (chara->mType == 3 && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 10) > 0 && chara->m5C != 0 &&
            (chara->m5C->m122 == 4 || chara->m5C->m122 == 2)) {
            reach *= 3.0f;
        }
    }
    return reach;
}

extern "C" int fn_8001BE00(CharaTask* chara) {
    bool ret = false;
    if (chara != 0) {
        ret = chara->CanAct_unk();
    }
    return ret;
}

extern "C" int fn_8001BE90(CharaTask* chara) {
    bool ret = false;
    if (chara != 0) {
        ret = (chara->mFlags & 4) == 0;
    }
    return ret;
}

extern "C" int fn_8001BEB0(CharaTask* chara) {
    int ret = 0;
    if (chara != 0) {
        ret = chara->mFlag31;
    }
    if (chara->m50 == 0x18709) {
        ret = 1;
    }
    return ret;
}

// inferred: hit points of the character in percent of its maximum
extern "C" int fn_8001BEE0(CharaTask* chara) {
    int rate = 0;
    if (chara != 0 && chara->mStatus.mParamB.m84 > 0) {
        rate = 100.0f * (chara->mStatus.m04 / chara->mStatus.mParamB.m84);
    }
    return rate;
}
