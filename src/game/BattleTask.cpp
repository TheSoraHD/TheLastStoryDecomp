// game/BattleTask.cpp (0x800F13BC-0x8010D98C): battle manager (BattleTask, HomingBullet). Only the
// effect start/stop functions are reconstructed so far.
#include "game/BattleTask.h"
#include "atn/EffectManager.h"
#include "atn/EffectSystem_unk.h"
#include "math/Vec.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "game/HitInfo_unk.h"
#include "game/LogMessageTask.h"
#include "macros.h"
#include "game/PlayerManager.h"

extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" PlayerManager* lbl_8087F8A0;

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

int BattleTask::fn_80100BB8(CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range, float theta) {
    return 0;
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

void BattleTask::fn_80102824(CharaTask* chara, PlayerTask* leader) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        mCharas[i].mChara = (CharaTask*)leader;
    }
}

BattleChara_unk* BattleTask::fn_80103F60(CharaTask* chara) {
    int i = FindChara_unk(chara);
    if (i >= 0) {
        return &mCharas[i];
    }
    return 0;
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

void BattleTask::fn_80105F3C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 4, 1);
}

void BattleTask::fn_8010607C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 3, 1);
}

void BattleTask::fn_8010652C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_8010653C(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 6);
    PlayEfp_unk(&mEfp38BC0[12], model);
}

void BattleTask::fn_801065E4(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 6, 1);
}

void BattleTask::fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color) {
    fn_80232B7C((unsigned int)chara, 7);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38BC0[13], model, 0, lbl_807C7030, lbl_807C7030, *(const EffectColor_unk*)color,
                -1, -1, 1, 1.0f);
}

void BattleTask::fn_80106680(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 8);
    PlayEfp_unk(&mEfp38BC0[14], model);
}

void BattleTask::fn_80106728(int arg, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[44], model);
}

void BattleTask::fn_801067D0(CharaTask* chara, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[45], model);
}

void BattleTask::fn_80107574() {
    m3E300 = 2;
}

void BattleTask::fn_80107798(void* owner, int flag) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 20, flag);
}

void BattleTask::fn_801077A8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 21);
    PlayEfp_unk(&mEfp38BC0[0], model);
}

void BattleTask::fn_80107850(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 21, 1);
}

void BattleTask::fn_80107860(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 22);
    PlayEfp_unk(&mEfp38BC0[2], model);
}

void BattleTask::fn_80107908(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 23);
    PlayEfp_unk(&mEfp38BC0[3], model);
}

void BattleTask::fn_801079B0(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 23, 1);
}

void BattleTask::fn_801079C0(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 24);
    PlayEfp_unk(&mEfp38BC0[5], model);
}

void BattleTask::fn_80107A68(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 24, 1);
}

void BattleTask::fn_80107A78(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 25);
    PlayEfp_unk(&mEfp38BC0[1], model);
}

void BattleTask::fn_80107B20(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 25, 1);
}

void BattleTask::fn_80107B30(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 26);
    PlayEfp_unk(&mEfp38BC0[6], model);
}

void BattleTask::fn_80107BD8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 26, 1);
}

void BattleTask::fn_80107BE8(void* owner, atn::Model* model) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80232B7C((unsigned int)owner, 27);
    PlayEfp_unk(&mEfp38BC0[7], model);
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
    fn_8023A8B4(lbl_8087F3C0, &mEfp38BC0[8], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107D94(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 28, 1);
}

void BattleTask::fn_80107E58(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 29, 1);
}

void BattleTask::fn_80107E68(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 30);
    PlayEfp_unk(&mEfp38BC0[4], model);
}

void BattleTask::fn_80107F10(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_80107F20(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 31);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38BC0[10], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107FC8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 31, 1);
}

void BattleTask::fn_80107FD8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 32);
    PlayEfp_unk(&mEfp38BC0[11], model);
}

void BattleTask::fn_80108080(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 32, 1);
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

float BattleTask::fn_8010CA24() {
    return 2.4f;
}

float BattleTask::fn_8010CA2C() {
    return 0.15f;
}

float BattleTask::fn_8010CA34() {
    return 1.5f;
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
