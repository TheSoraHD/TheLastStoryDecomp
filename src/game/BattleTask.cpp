// game/BattleTask.cpp (0x800F13BC-0x8010D98C): battle manager (BattleTask, HomingBullet). Only the
// effect start/stop functions are reconstructed so far.
#include "game/BattleTask.h"
#include "atn/Memory.h"
#include "atn/EffectManager.h"
#include "atn/EffectSystem_unk.h"
#include "math/Vec.h"
#include "atn/SoundRef.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "game/HitInfo_unk.h"
#include "game/LogMessageTask.h"
#include "macros.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/GameTask.h"
#include "game/GameScene.h"

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void* fn_80210220(int id); // inferred: record of an attract-hate parameter (as in game/CharaTask.cpp)
extern "C" PlayerManager* lbl_8087F8A0;
extern "C" GameTask* lbl_8087F430;
extern "C" int rand();
extern "C" void* lbl_8087F610; // inferred: multiplayer manager (null in single player)
extern "C" atn::EfpFile* fn_800EFBC4(int id); // inferred: battle effect of row `id`
extern "C" atn::EfpFile* fn_800EFDA0(int id); // inferred: skill effect of row `id`
extern "C" const char* fn_800EFE3C(int id);   // inferred: sound of row `id`
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_80219558(int id); // inferred: party member kind of a character id
extern "C" int fn_80239D58(atn::EffectManager* mgr, void* owner, int id); // inferred: the effect is playing

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const EffectColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an EFP effect at the model, without offset (as gmk/Coffin.cpp)
inline void PlayEfp_unk(atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    EffectColor_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_800FDE60(int effect, CharaTask* chara, const atn::Vector3& dir) {
    fn_800E2A24(&m38A5C);
    m38A5C.m00 = effect;
    m38A5C.m04 = chara;
    m38A5C.m08 = fn_80219E6C(10000);
    m38A5C.m44 = 3;
    m38A5C.m28 = dir;
    m38A5C.m48 = 0;
    m38A5C.m4C = 0;
    m38A5C.m0C |= 0x10000;
    fn_800FDF14(&m38A5C);
}

int BattleTask::fn_80100BB8(CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range, float theta) {
    return 0;
}

void BattleTask::fn_80100BC0() {
    fn_801055A8(0);
    lbl_8087F3C0->fn_8023A614(5);
}

Bullet* BattleTask::fn_801010A0() {
    for (int i = 0; i < 256; i++) {
        if (!mBullets[i].HasFlag_unk(0x10000)) {
            return &mBullets[i];
        }
    }
    return 0;
}

HomingBullet* BattleTask::fn_801010E8() {
    for (int i = 0; i < 32; i++) {
        if (!mHomings[i].HasFlag_unk(0x10000)) {
            return &mHomings[i];
        }
    }
    return 0;
}

void BattleTask::fn_80101138() {
    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf14(0);
        }
    }
}

void BattleTask::fn_801011B8() {
    for (unsigned int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000)) {
            mBullets[i].vf0C(0, 1);
        }
    }
    for (unsigned int i = 0; i < 32; i++) {
        if (mHomings[i].HasFlag_unk(0x10000)) {
            mHomings[i].vf0C(0, 1);
        }
    }
}

void BattleTask::fn_8010129C() {
    for (unsigned int i = 0; i < 256; i++) {
        mBullets[i].vf18();
    }
    for (unsigned int i = 0; i < 32; i++) {
        mHomings[i].vf18();
    }
}

// inferred: defined in this file (follows fn_8010129C, which calls it on every bullet)
void Bullet::vf18() {
    m04 &= ~0x10000;
}

int BattleTask::fn_80101340(CharaTask* chara) {
    int count = 0;
    for (int i = 0; i < 256; i++) {
        if (mBullets[i].HasFlag_unk(0x10000) && mBullets[i].HasFlag_unk(0x20000) && mBullets[i].m9C == chara) {
            count++;
        }
    }
    return count;
}

void BattleTask::fn_801021B4(RayObj_unk* obj, int a, int b) {
    m3E2E8 = obj;
    m3E2E0 = 0;
    m3E2E4 = 0;
    m3E2EC = b;
    m3E2F0 = a;
    if (obj != 0) {
        if ((obj->m08 & 2) == 2) {
            m3E2E0 = (int)obj->m0C;
        } else if ((obj->m08 & 8) == 8) {
            m3E2E4 = obj->m0C;
        }
    }
}

void BattleTask::fn_801027A4(CharaTask* chara) {
    if (FindChara_unk(chara) < 0) {
        mCharas[mCharaCount].mChara = chara;
        mCharaCount++;
    }
}

void BattleTask::fn_80102824(CharaTask* chara, PlayerTask* leader) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        mCharas[i].mChara = (CharaTask*)leader;
    }
}

int BattleTask::fn_80102890(CharaTask* chara) {
    if (chara == 0) {
        return -1;
    }
    for (int i = 0; i < mCharaCount; i++) {
        if (mCharas[i].mChara == chara) {
            return i;
        }
    }
    return -1;
}

void BattleTask::fn_801028E4(CharaTask* chara, CharaTask* other, int value) {
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b < 0) {
        return;
    }
    if (a >= 0) {
        mCharas[a].m08[b] = value;
    } else {
        for (int i = 0; i < 0x48; i++) {
            mCharas[i].m08[b] = value;
        }
    }
}

void BattleTask::fn_801031D0(CharaTask* chara, CharaTask* other, int all) {
    fn_80210220(lbl_8087F0A8->mParamIndex);
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b >= 0 && a >= 0) {
        BattleChara_unk* entry = &mCharas[a];
        if (entry->mChara != 0 && entry->mChara->CanAct_unk()) {
            entry->fn_8010895C(b, all);
        }
    }
}

void BattleTask::fn_8010337C(CharaTask* chara, CharaTask* other) {
    int a = FindChara_unk(chara);
    int b = FindChara_unk(other);
    if (b < 0) {
        return;
    }
    if (a >= 0) {
        mCharas[a].m6E4[b] = 0.0f;
    } else {
        for (int i = 0; i < mCharaCount; i++) {
            mCharas[i].m6E4[b] = 0.0f;
        }
    }
}

// NONMATCHING: 99.5% - registers of the index of `other` and of the count base swapped
int BattleTask::fn_80103484(CharaTask* chara, CharaTask* other) {
    int b;
    int a = FindChara_unk(chara);
    b = FindChara_unk(other);
    if (b < 0) {
        return -1;
    }
    if (a >= 0) {
        BattleChara_unk* entry = &mCharas[a];
        if ((entry->mChara->mFlags & 1) == 1) {
            return -1;
        }
        return entry->fn_801083D8(b);
    }
    BattleChara_unk* entry = mCharas;
    int sum = 0;
    int count = 0;
    for (int i = 0; i < mCharaCount; i++) {
        if ((entry->mChara->mFlags & 1) != 1) {
            sum += entry->fn_801083D8(b);
            count++;
        }
        entry++;
    }
    if (count > 0) {
        return sum / count;
    }
    return -1;
}

// NONMATCHING: 99.3% - registers of the entry pointer and of the flags of the inlined CanAct_unk swapped
void BattleTask::fn_80103E60(CharaTask* target) {
    if (target == 0) {
        return;
    }
    CharaTask* c;
    BattleChara_unk* p;
    int count = 0;
    p = mCharas;
    for (int i = 0; i < mCharaCount; i++, p++) {
        c = p->mChara;
        if (c != 0 && c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000 && (c->mType == 0 || c->mC58.m00 != 0) &&
            c->mC58.mC4 == target) {
            count++;
        }
    }
    target->m102C = count;
}

BattleChara_unk* BattleTask::fn_80103F60(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        return &mCharas[i];
    }
    return 0;
}

void BattleTask::fn_80105510(int value) {
    if (value > 300) {
        if (m363BC < value) {
            m363BC = 9999;
        } else {
            m363BC = 300;
        }
    } else if (value < 20) {
        m363BC = 20;
    } else {
        m363BC = value;
    }
    if (lbl_8087F430 == 0) {
        return;
    }
    if (m363BC == 9999) {
        ((GameTask*)lbl_8087F430)->fn_80370320(10, 1, 0);
    } else {
        ((GameTask*)lbl_8087F430)->fn_80370320(10, 0, 0);
    }
}

void BattleTask::fn_801058F8() {
    fn_80103FD4();
    for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
        fn_80103600(player);
    }
}

void BattleTask::fn_80105950(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0[0], model);
}

void BattleTask::fn_801059F8(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)chara, 0, 1);
}

void BattleTask::fn_80105A08(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0[1], model);
}

void BattleTask::fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363C0[2], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105B3C(CharaTask* chara) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)chara, 0, 0);
}

void BattleTask::fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363C0[3], model, 0, lbl_807C7030, lbl_807C7030, *color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105BD8(atn::Model* model) {
    lbl_8087F3C0->mSystem.m78 = 1;
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[25], model);
    lbl_8087F3C0->mSystem.m78 = 0;
}

void BattleTask::fn_80105C9C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[1], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[0], model);
    }
}

void BattleTask::fn_80105F3C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 4, 1);
}

void BattleTask::fn_80105F4C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[7], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[6], model);
    }
}

void BattleTask::fn_8010607C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 3, 1);
}

void BattleTask::fn_8010608C(CharaTask* chara, atn::Model* model) {
    fn_80232B7C((unsigned int)chara, 3);
    if (fn_80219558(chara->m50) == 2) {
        PlayEfp_unk(&mEfp38AF4[9], model);
    } else {
        PlayEfp_unk(&mEfp38AF4[8], model);
    }
}

void BattleTask::fn_8010652C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_8010653C(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 6);
    PlayEfp_unk(&mEfp38AF4[29], model);
}

void BattleTask::fn_801065E4(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 6, 1);
}

void BattleTask::fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color) {
    fn_80232B7C((unsigned int)chara, 7);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[30], model, 0, lbl_807C7030, lbl_807C7030, *(const EffectColor_unk*)color,
                -1, -1, 1, 1.0f);
}

void BattleTask::fn_80106680(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 8);
    PlayEfp_unk(&mEfp38AF4[31], model);
}

void BattleTask::fn_80106728(int arg, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[44], model);
}

void BattleTask::fn_801067D0(CharaTask* chara, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[45], model);
}

void BattleTask::fn_80107208(int id, const atn::Vector3* pos, float scale) {
    const char* name = fn_800EFE3C(id);
    if (name != 0) {
        fn_800C344C(name, pos, 0, -1, scale);
    }
}

int BattleTask::fn_80107274(CharaTask* chara) {
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara == chara) {
            return 0;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed == 0) {
            mSlots[i].mUsed = 1;
            mSlots[i].mChara = chara;
            return 1;
        }
    }
    return 0;
}

void BattleTask::fn_80107380(CharaTask* chara) {
    for (int i = 0; i < 8; i++) {
        if (mSlots[i].mUsed != 0 && mSlots[i].mChara == chara) {
            mSlots[i].mUsed = 0;
            mSlots[i].mChara = 0;
        }
    }
}

void BattleTask::fn_80107574() {
    m3E300 = 2;
}

void BattleTask::fn_80107584() {
    lbl_8087F3C0->mSystem.mReplaying = 1;
    switch (m3E300) {
    case 1: {
        m3E2FC = MIN(1.0f, m3E2FC + 0.1f);
        EffectColor_unk color;
        color.r = m3E304;
        color.g = m3E308;
        color.b = m3E30C;
        color.a = m3E2FC;
        lbl_8087F3C0->fn_8023A184((unsigned int)&m389F4, 0, &color);
        break;
    }
    case 2: {
        m3E2FC = MAX(0.0f, m3E2FC - 0.1f);
        EffectColor_unk color;
        color.r = m3E304;
        color.g = m3E308;
        color.b = m3E30C;
        color.a = m3E2FC;
        lbl_8087F3C0->fn_8023A184((unsigned int)&m389F4, 0, &color);
        if (m3E2FC < 0.001f) {
            fn_80239DAC(lbl_8087F3C0, (unsigned int)&m389F4, 0, 0);
            m3E300 = 0;
        }
        break;
    }
    }
    lbl_8087F3C0->mSystem.mReplaying = 0;
}

void BattleTask::fn_801076D0(void* owner, atn::Model* model) {
    if (fn_80239D58(lbl_8087F3C0, owner, 20) == 0) {
        fn_80232B7C((unsigned int)owner, 20);
        PlayEfp_unk(&mEfp38AF4[16], model);
    }
}

void BattleTask::fn_80107798(void* owner, int flag) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 20, flag);
}

void BattleTask::fn_801077A8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 21);
    PlayEfp_unk(&mEfp38AF4[17], model);
}

void BattleTask::fn_80107850(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 21, 1);
}

void BattleTask::fn_80107860(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 22);
    PlayEfp_unk(&mEfp38AF4[19], model);
}

void BattleTask::fn_80107908(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 23);
    PlayEfp_unk(&mEfp38AF4[20], model);
}

void BattleTask::fn_801079B0(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 23, 1);
}

void BattleTask::fn_801079C0(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 24);
    PlayEfp_unk(&mEfp38AF4[22], model);
}

void BattleTask::fn_80107A68(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 24, 1);
}

void BattleTask::fn_80107A78(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 25);
    PlayEfp_unk(&mEfp38AF4[18], model);
}

void BattleTask::fn_80107B20(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 25, 1);
}

void BattleTask::fn_80107B30(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 26);
    PlayEfp_unk(&mEfp38AF4[23], model);
}

void BattleTask::fn_80107BD8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 26, 1);
}

void BattleTask::fn_80107BE8(void* owner, atn::Model* model) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80232B7C((unsigned int)owner, 27);
    PlayEfp_unk(&mEfp38AF4[24], model);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CA8(void* owner, int flag) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 27, flag);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CEC(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 28);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[25], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107D94(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 28, 1);
}

void BattleTask::fn_80107DA4(void* owner, atn::Model* model, float scale, const atn::Vector3& pos) {
    fn_80232B7C((unsigned int)owner, 29);
    EffectColor_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[26], model, 0, pos, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107E58(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 29, 1);
}

void BattleTask::fn_80107E68(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 30);
    PlayEfp_unk(&mEfp38AF4[21], model);
}

void BattleTask::fn_80107F10(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_80107F20(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 31);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38AF4[27], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107FC8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 31, 1);
}

void BattleTask::fn_80107FD8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 32);
    PlayEfp_unk(&mEfp38AF4[28], model);
}

void BattleTask::fn_80108080(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 32, 1);
}

// NONMATCHING: 99% - the "" literals of the allocations are in the string pool of the file (not complete yet)
BattleChara_unk::BattleChara_unk() : mHateValues(0), mHateIds(0), mHateCount(0) {
    int count = 10;
    if (lbl_8087F610 == 0) {
        count = 5;
    }
    mHateCount = count;
    mHateValues = new ((MemoryTag)3, "", "", 0) short[mHateCount];
    mHateIds = new ((MemoryTag)3, "", "", 0) short[mHateCount];
    mChara = 0;
    m92D = 0;
    m248 = 1.0f;
    m92E = 0;
    m92F = 0;
    fn_801081D8();
}

BattleChara_unk::~BattleChara_unk() {
    if (mHateValues != 0) {
        delete[] mHateValues;
        mHateValues = 0;
    }
    if (mHateIds != 0) {
        delete[] mHateIds;
        mHateIds = 0;
    }
    mHateCount = 0;
}

void BattleChara_unk::fn_801081D8() {
    for (int i = 0; i < 72; i++) {
        m08[i] = 0;
        m128[i] = 1.0f;
        m48C[i] = 0;
        m5AC[i] = 0;
        m36C[i] = 0;
        m24C[i] = 0.0f;
        m80C[i] = 0;
        m6E4[i] = 0.0f;
    }
    for (int i = 0; i < mHateCount; i++) {
        mHateValues[i] = 0;
        mHateIds[i] = -1;
    }
    m6CC = (int)(70.0f * (rand() / 32767.0f));
    m6DC = 0;
    m6E0 = 0;
    m804 = 0;
    m808 = 0;
    m92F = 0;
    m930 = 0;
}

int BattleTask::fn_80108378(CharaTask* chara) {
    BattleTask* battle = lbl_8087F048;
    if (battle != 0) {
        if (chara == 0) {
            return -1;
        }
        for (int i = 0; i < battle->mCharaCount; i++) {
            if (battle->mCharas[i].mChara == chara) {
                return i;
            }
        }
        return -1;
    }
    return -1;
}

void BattleChara_unk::fn_801088B8(int id, int value) {
    int n = 0;
    for (int i = 0; i < mHateCount; i++) {
        if (id == mHateIds[i]) {
            n++;
        }
    }
    if (n >= 10) {
        return;
    }
    for (int i = 0; i < mHateCount - 1; i++) {
        mHateIds[i] = mHateIds[i + 1];
        mHateValues[i] = mHateValues[i + 1];
    }
    mHateIds[mHateCount - 1] = id;
    mHateValues[mHateCount - 1] = value;
}

void BattleChara_unk::fn_8010895C(int id, int all) {
    for (int i = 0; i < mHateCount; i++) {
        if (id == mHateIds[i]) {
            mHateIds[i] = -1;
            mHateValues[i] = 0;
            if (all == 0) {
                break;
            }
        }
    }
    int skip = 1;
    for (int i = 0; i < mHateCount; i++) {
        int hateId = mHateIds[i];
        if (hateId >= 0 && mHateValues[i] > 0) {
            skip = 0;
        }
        if (skip == 0 && (hateId < 0 || mHateValues[i] == 0)) {
            for (int j = i + 1; j < mHateCount; j++) {
                mHateIds[j - 1] = mHateIds[j];
                mHateValues[j - 1] = mHateValues[j];
            }
        }
    }
}

void BattleTask::fn_80108A54(CharaTask* chara) {
    fn_80108A5C(chara, &chara->mRot);
}

// NONMATCHING: 93% - the original tests the log instance with bne over a blr
void BattleTask::fn_80109828(const wchar_t* text) {
    if (lbl_8087F0A8->m06C > 0) {
        if (LogMessageTask::sInstance == 0) {
            return;
        }
        if (text != 0) {
            LogMessageTask::sInstance->fn_8047961C(text, -0x3301, 0, 0);
        }
    }
}

// NONMATCHING: 98% - f2/f3 swapped (increment/result)
void BattleTask::fn_8010AB34() {
    float add = m38A28;
    m38A04 = MAX(0.0f, MIN(m38A04 + add, m38A0C));
}

// NONMATCHING: 98% - f2/f3 swapped, as fn_8010AB34
void BattleTask::fn_8010AB94() {
    float add = m38A2C;
    m38A04 = MAX(0.0f, MIN(m38A04 + add, m38A0C));
}

void BattleTask::fn_8010ABF4() {
    m38A04 = 0.0f;
    fn_8010ACCC();
    m38CB8 = m38C90;
    fn_8010AEF4();
}

void BattleTask::fn_8010AC60() {
    m38CB8.m00 = 0;
    m38CB8.m04 = 0;
    m38CB8.m08 = 0;
    m38CB8.m0C = 0;
    m38CB8.m10 = 0;
    BattleChara_unk* p = mCharas;
    for (int i = 0; i < mCharaCount; i++) {
        CharaTask* c = p->mChara;
        if (c != 0 && c->mC58.mD0 != 0) {
            c->mC58.mD0 = 0;
        }
        p->m92E = 0;
        p++;
    }
    fn_80105510(40);
}

int BattleTask::fn_8010B180(CharaTask* a, CharaTask* b) {
    if (a == 0 || b == 0 || lbl_8087F430->fn_80376210() == 0) {
        return 1;
    }
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (scene == 0) {
        return 1;
    }
    int areaA = a->mC58.mB4;
    int areaB = b->mC58.mB4;
    if (areaA > 0 && areaB > 0 && areaA != areaB) {
        gs::AiArea* area = scene->fn_803C1884(areaA);
        if (area != 0 && area->m28 >= 0 && area->m28 != areaB) {
            return 0;
        }
    }
    return 1;
}

float BattleTask::fn_8010CA24() {
    return 2.4f;
}

float BattleTask::fn_8010CA2C() {
    return 0.15f;
}

float BattleTask::fn_8010CA34() {
    return 1.5f;
}

float BattleTask::fn_8010CA3C(CharaTask* chara, float rate) {
    rate += chara->mStatus.fn_80134168(27, -1) / 100.0f;
    if (chara->mStatus.fn_80134134(46) != 0) {
        rate *= 0.7f;
    }
    if (((unsigned int)chara->mStatus.m14 & 0x2000) == 0x2000) {
        rate *= 1.5f;
        if (chara->mType == 2 && chara->mFlag5) {
            rate *= 1.5f;
        }
    }
    if (chara->mType == 0) {
        rate = MIN(2.2f, rate);
    }
    return rate;
}

float BattleTask::fn_8010CCD0() {
    return 0.75f;
}

float BattleTask::fn_8010CCD8(CharaTask* chara) {
    if (chara == 0 || chara->mType == 2) {
        return 0.0f;
    }
    float gain = 2.0f * m38A48;
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0 && leader->mFlag28) {
        gain *= 1.5f;
    }
    return gain;
}

int BattleTask::fn_8010D770(HitInfo_unk* hit) {
    if (hit != 0) {
        int id = hit->m04;
        if (id == 0xCC || id == 0xD1 || id == 0xD5 || id == 0x5BC || id == 0x658) {
            return 1;
        }
    }
    return 0;
}

int BattleTask::fn_8010D7B4(HitInfo_unk* hit) {
    if (hit == 0) {
        return 0;
    }
    int id = hit->m04;
    if (id == 0x136 || id == 0x139 || id == 0x13C || id == 0x146 || id == 0x15A || id == 0x232C) {
        return 1;
    }
    return 0;
}
