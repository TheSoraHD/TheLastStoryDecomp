// Event/Sequence.cpp (0x8053D0F4-0x80546000): Event::Sequence, the task that owns the cuts and the data
// of an event. One TU with the inline virtuals of Event::StringData/DummyData (0x805430E8) and the
// template instances emitted after them (vector growth, DataList_unk::SetOwner_unk...).
#include "Event/Sequence.h"
#include "Event/Cut.h"
#include "Event/Manager.h"
#include "Event/Dialog.h"
#include "atn/Memory.h"
#include "atn/RenderSetting_unk.h"
#include "db/EventIdTable_unk.h"
#include "game/GameField_unk.h"
#include "game/GameScene.h"
#include "game/PlayerManager.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"
#include "game/CrowdManager.h"
#include "game/EnemyTask.h"
#include "game/NpcTask.h"
#include "game/PlayerTask.h"
#include "game/GameConfig_unk.h"
#include "atn/D2A_Task.h"
#include "atn/FadeCommon.h"
#include "atn/MemoryManager.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);

extern "C" PlayerManager* lbl_8087F8A0;         // inferred: the party manager
extern "C" RenderSetting_unk* lbl_8087EFA8;     // inferred: render settings
extern "C" GameConfig_unk* lbl_8087F0A8;        // inferred: game settings
extern "C" void fn_804A0580(GameMap_unk* map, int arg, const atn::Vector3* pos); // inferred
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int fn_8006AA20(const char* path);    // inferred: the file exists
extern "C" void fn_800C3094(const char* name);   // inferred: loads a message group
extern "C" void fn_800C3124(const char* name);   // inferred: releases a message group
extern "C" void fn_800D2338(atn::Task* task);    // inferred: deletes the task
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: layout pane by name and value by name hash (atn::D2A_Layout_unk)
extern "C" void* fn_801FEC74(atn::D2A_Layout_unk* layout, const char* name);
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value);
// inferred: text of a pane (returned by value) and its width with the font lbl_8087EEC8
extern "C" std::basic_string<wchar_t> fn_800A4450(void* pane, int a, int b);
extern "C" void* lbl_8087EEC8;
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b);

namespace Event {

// inferred: the assertions put the constructor over the inlining budget, as in the original
inline void Sequence::SetFlag3_unk(int on) {
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    m98_3 = on != 0;
}

// inferred: the constructor passes the inlining budget: every inline it uses is called out of line
// (weak copies 0x8053D3E0-0x8053E658 behind it, in order of first use)
Sequence::Sequence(atn::Task* parent) : atn::Task(parent), m50(0), m54(0), m94(0), mB4(0.0f), m190(-1), m194(1),
    m198(0.0f), m19C(0.0f), m1A0(0), m1A4(-1), mLabelCount(0), m1AC(0), m1BC(0), m1C0(0), m1CC(0), m1D0(0),
    m1EC(0), m280(0), m28C(0), m290(0) {
    m98 = 0;
    SetFlag3_unk(1);
    SetFlag4_unk(1);
    SetFlag7_unk(0);
    SetFlag8_unk(0);
    SetFlag9_unk(0);
    m9C = lbl_807C7030;
    mA8 = lbl_807C7030;
    m1B0 = 0;
    m1B4 = 0;
    m1B8 = 0;
    for (int i = 0; i < 4; i++) {
        m1F0[i].Reset_unk();
    }
    strcpy(m74, "");
    m288 = 0;
    m284 = 0;
    mC4.SetOwner_unk(this);
    mD4.SetOwner_unk(this);
    mE4.SetOwner_unk(this);
    mF4.SetOwner_unk(this);
    m104.SetOwner_unk(this);
    m124.SetOwner_unk(this);
    m134.SetOwner_unk(this);
    m144.SetOwner_unk(this);
    m154.SetOwner_unk(this);
    m164.SetOwner_unk(this);
    m164.Add_unk("!world", 0);
    m184.clear();
    m294.Resize_unk(4, 4, "ContinueEffectList");
}

Sequence::~Sequence() {
    fn_805406BC();
}

Sequence* Sequence::fn_8053E86C(atn::Task* parent) {
    return new ((MemoryTag)4, "", "", 0) Sequence(parent);
}

int Sequence::vf0C() {
    return 1;
}

void Sequence::fn_8053E8C8(int state) {
    m194 = state;
}

// inferred: scene of the current map (the field's, or the event manager's without a field)
static inline GameScene* GetScene_unk() {
    return lbl_8087F430 != 0 ? lbl_8087F430->GetScene_unk() : (lbl_8087F540->m1A6C != 0 ? lbl_8087F540->m1A6C : 0);
}

// inferred: start of the event: stops the characters and the managers the event hides
void Sequence::fn_8053E8D0() {
    if (lbl_8087F540 == 0) {
        return;
    }
    if (m50 == 1 || m50 == 3) {
        EventIdEntry_unk* entry = EventIdTable_unk::fn_8020F130()->fn_8020F71C(m190);
        if (entry != 0) {
            GameScene* scene = GetScene_unk();
            if ((entry->mFlags & 1) == 1 && lbl_8087F8A0 != 0) {
                lbl_8087F8A0->FreezeTask();
            }
            if ((entry->mFlags & 2) == 2 && EnemyManager::sInstance != 0) {
                if (scene != 0) {
                    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
                        if (!enemy->IsFlag_unk(4)) {
                            lbl_8087F430->GetScene_unk()->fn_803CCC84(2, enemy->m58);
                            enemy->fn_8016E4C4(0);
                        }
                    }
                }
                EnemyManager::sInstance->FreezeTask();
            }
            if ((entry->mFlags & 4) == 4 && NpcManager::sInstance != 0) {
                NpcManager::sInstance->FreezeTask();
            }
            if ((entry->mFlags & 8) == 8 && CrowdManager::sInstance != 0) {
                CrowdManager::sInstance->FreezeTask();
            }
        }
        for (PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0; player != 0; player = player->mNext) {
            player->fn_8016E71C(0, 0);
        }
        for (EnemyTask* enemy = EnemyManager::sInstance != 0 ? EnemyManager::sInstance->mHead : 0; enemy != 0;
             enemy = enemy->mNext) {
            enemy->fn_8016E71C(0, 0);
        }
        for (NpcTask* npc = NpcManager::sInstance != 0 ? NpcManager::sInstance->mHead : 0; npc != 0; npc = npc->mNext) {
            npc->fn_8016E71C(0, 0);
        }
    }
    fn_805501DC(0);
    for (int i = 0; i < m164.mSize; i++) {
        m164.mData[i].fn_8000F1EC();
    }
    if (m98_3) {
        for (int i = 0; i < m164.mSize; i++) {
            Actor_unk* actor = &m164.mData[i];
            if (actor->m18_7 != 1) {
                actor->fn_8001282C(-1, 0);
            }
        }
    }
    if (lbl_8087F540->m2400 != 0 && (m190 == 0x270E || m190 == 0x151B)) {
        lbl_8087F540->m2400->fn_800D246C(0);
        lbl_8087F540->m2400->m48 = 0.0f;
    }
    if (m50 == 1 || m50 == 3) {
        lbl_8087F540->fn_80482ED8();
        RenderSetting_unk* rs = lbl_8087EFA8;
        if (rs != 0) {
            rs->m324.mMono = 0;
            rs->m324.mMonoRate = 0.0f;
            lbl_8087EFA8->m324 = rs->m324;
        }
        for (int i = 0; i < 4; i++) {
            lbl_8087F540->mCameras[i].fn_80112784();
            if (m98_4 == 1) {
                lbl_8087F540->mCameras[i].SetFovy_unk(0.5235988f);
            } else {
                lbl_8087F540->mCameras[i].SetFovy_unk(1.0471976f);
            }
        }
        lbl_8087F540->m1A38 = 0;
        if (m280 != 0) {
            m280->fn_80228FD8();
        }
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && lbl_8087F8A0 != 0) {
            PlayerTask* leader = lbl_8087F8A0->mHead;
            if (leader != 0) {
                fn_804A0580(fn_80373148(lbl_8087F430), 1, &leader->mPos);
            }
        }
    }
}

// inferred: end of the event: shows the characters again and resumes the managers
void Sequence::fn_8053ED4C() {
    if (lbl_8087F540 == 0) {
        return;
    }
    if (lbl_8087F540->m2400 != 0 && m190 != 0x151B && m190 != 0x151F && m190 != 0x151E) {
        lbl_8087F540->m2400->fn_800D246C(1);
        lbl_8087F540->m2400->m48 = 0.0f;
    }
    if (m50 == 1 || m50 == 3) {
        if (lbl_8087F0A8->m194 == 0 || m190 != 0x19D) {
            lbl_8087F540->fn_80483714(m98_7, m98_15);
        }
        for (int i = 0; i < 4; i++) {
            lbl_8087F540->mCameras[i].fn_80112784();
        }
        lbl_8087F540->fn_8047F994(1, 0);
        lbl_8087F540->fn_804817D4();
        lbl_8087F540->fn_8047F8B0();
        m1EC->fn_803B57EC();
        lbl_8087F540->m1AA0 = 0;
        lbl_8087F540->fn_8048169C(0);
        lbl_8087F540->fn_8048E904();
        lbl_8087F540->fn_80484FBC();
        lbl_8087F540->fn_8048F184(0);
        lbl_8087F540->fn_8048F184(1);
    }
    if (m280 != 0) {
        m280->fn_80229240();
    }
    for (int i = 0; i < 4; i++) {
        m1F0[i].Reset_unk();
    }
    if (!m98_15) {
        fn_8053EF70();
    }
}

// inferred: shows the characters hidden by fn_8053E8D0 and restarts the managers
void Sequence::fn_8053EF70() {
    if (lbl_8087F540 == 0) {
        return;
    }
    for (int i = 0; i < m164.mSize; i++) {
        m164.mData[i].fn_8000FAE8();
    }
    if (m50 == 1 || m50 == 3) {
        EventIdEntry_unk* entry = EventIdTable_unk::fn_8020F130()->fn_8020F71C(m190);
        if (entry != 0) {
            GameScene* scene = GetScene_unk();
            if (scene != 0) {
                scene->fn_803CCCC0();
            }
            if ((entry->mFlags & 1) == 1 && lbl_8087F8A0 != 0) {
                lbl_8087F8A0->MeltTask();
            }
            if ((entry->mFlags & 2) == 2 && EnemyManager::sInstance != 0) {
                EnemyManager::sInstance->MeltTask();
            }
            if ((entry->mFlags & 4) == 4 && NpcManager::sInstance != 0) {
                NpcManager::sInstance->MeltTask();
            }
            if ((entry->mFlags & 8) == 8 && CrowdManager::sInstance != 0) {
                CrowdManager::sInstance->MeltTask();
            }
        }
        if (lbl_8087F0A8->m194 == 0 || m190 != 0x19D) {
            lbl_8087F540->fn_80483E94(m98_7);
        }
    }
}

void Sequence::vf10() {
    fn_8053F1B4();
    for (int i = 0; i < m164.mSize; i++) {
        m164.mData[i].fn_8000EE8C();
    }
    if (m280 != 0 && m194 >= 5 && !(m194 == 11 || m194 == 12)) {
        m280->fn_802297AC();
    }
}

// inferred: state machine of the sequence
void Sequence::fn_8053F1B4() {
    switch (m194) {
    case 2:
    case 3:
        if (m1C4.fn_80473F50() != 0) {
            break;
        }
        fn_8057D8C0();
        fn_80575FCC();
        fn_80541F18();
        m194 = 4;
        fn_80576258();
        break;
    case 4:
        m194 = 5;
        if (m98_10 == 1) {
            fn_8053F6EC();
            m98_10 = 0;
        } else {
            FreezeTask();
        }
        break;
    case 6:
        lbl_8087F540->fn_80489A50(0, 0);
        if (m98_12) {
            if (m29C.fn_8047054C() != 0) {
                break;
            }
            fn_8053F818();
            m98_12 = 0;
        } else if (fn_8054011C() != 0) {
            m29C.fn_80470528();
            if (m98_11 == 1) {
                m194 = 9;
                m98_11 = 0;
            } else {
                m194 = 8;
            }
        }
        break;
    case 7:
        fn_8053ED4C();
        m194 = 8;
        break;
    case 9: {
        if (lbl_8087F540->m2380 > 0) {
            lbl_8087F540->fn_8047F580();
        }
        fn_8053E8D0();
        m194 = 10;
        if ((unsigned int)(m190 - 0x238D) <= 5) {
            atn::D2A_Task* task = static_cast<atn::D2A_Task*>(mD4.mData[0].m18);
            std::basic_string<wchar_t> text = fn_800A4450(fn_801FEC74(&task->mData.m10, "mapName"), 0, 0);
            float width = fn_8006EF48(lbl_8087EEC8, text.c_str(), 20.0f, 0.0f, 1, 1);
            fn_801FECE0(&task->mData.m10, fn_800DC6B4("strLength"), width);
        }
    }
    case 10:
        fn_80550254();
        break;
    case 11:
        m194 = 12;
        m2C4 = 0;
    case 12:
        if (lbl_8087F0A8->m194 == 0 || m190 != 0x19D) {
            break;
        }
        if (m2C4 > 0) {
            if (--m2C4 == 0) {
                m194 = 13;
            }
            break;
        }
        m2C0->fn_800D246C(0);
        if (!m2C0->IsEnd_unk()) {
            break;
        }
        if (lbl_8087F0A8->mInput.fn_801231D0(0) == 0) {
            break;
        }
        m2C0->SetSpeed_unk(-1.5f);
        m2C4 = 30;
        {
            atn::FadeColor* fade = atn::FadeColor::Create(this, 30, -1, 0xFF000000);
            fade->mActive = 1;
            fade->mWait = 10;
            fade->mDeleteAtEnd = 1;
            fade->mZ = -0.01f;
        }
        break;
    case 13:
        fn_8053ED4C();
        m194 = 14;
        break;
    }
}

void Sequence::vf14() {
    if (m194 == 10) {
        for (int i = 0; i < 4; i++) {
            ScreenEffect_unk& effect = m1F0[i];
            if (effect.m00 == 0) {
                continue;
            }
            float frame = m198;
            float start = effect.m04;
            float end = effect.m08 + start;
            if ((frame < start || end < frame) && !(effect.m20 & 0x1000000)) {
                effect.Reset_unk();
                continue;
            }
            float t = 1.0f;
            if (frame < start + 5.0f && !(effect.m20 & 0x2000000)) {
                t = (frame - start) / 5.0f;
            } else if (frame > end - 5.0f && !(effect.m20 & 0x1000000)) {
                t = (end - frame) / 5.0f;
            }
            fn_80541588(effect.m00->m08, effect.m0C, effect.m10, effect.m14, effect.m18, effect.m1C, effect.m20, t);
        }
    }
    if (m98_6 == 1) {
        fn_805406BC(1);
        m98_6 = 0;
    }
    if (m98_16 == 1) {
        if (m98_1 == 1) {
            for (int i = 0; i < m164.mSize; i++) {
                m164.mData[i].vf0C();
            }
            m98_1 = 0;
            m98_15 = 0;
        }
        m98_16 = 0;
    }
}

// inferred: loads the event pack and the message group of the event
void Sequence::fn_8053F6EC() {
    m284 = (int)atn::MemoryManager::Instance()->mMem1.fn_80084F84() +
           (int)atn::MemoryManager::Instance()->mMem2.fn_80084F84();
    if (m194 < 5) {
        m98_10 = 1;
        return;
    }
    if (m98_1 == 1) {
        return;
    }
    char path[0x100];
    sprintf(path, "/eventpacks/%s", m2A0.mStr);
    if (fn_8006AA20(path) != 0) {
        m29C.fn_80470364(path, 0, 1, 0);
        m98_12 = 1;
    } else {
        fn_8053F818();
        m98_12 = 0;
    }
    sprintf(m74, "GROUP_EV%04d", m190);
    fn_800C3094(m74);
    m98_1 = 1;
    m194 = 6;
    MeltTask();
}

// NONMATCHING: stub - reads the data of the event (the loops over the lists and the string of m68)
void Sequence::fn_8053F818() {
    for (int i = 0; i < mC4.mSize; i++) {
        mC4.mData[i].vf08();
    }
}

// NONMATCHING: stub
int Sequence::fn_8054011C() {
    return 0;
}

void Sequence::fn_80540664(int arg) {
    if (m98_1 == 1) {
        m98_6 = 1;
    }
    if (arg == 0 && m98_1 == 1) {
        m98_16 = 1;
    }
}

void Sequence::fn_805406A0() {
    if (m98_1 == 1) {
        m98_16 = 1;
    }
}

// inferred: releases the data of the event
void Sequence::fn_805406BC(int keep) {
    if (m98_1 != 1) {
        return;
    }
    mNoKill_unk = keep != 0;
    m29C.fn_80470528();
    for (int i = 0; i < mC4.mSize; i++) {
        mC4.mData[i].vf0C();
    }
    for (int i = 0; i < mD4.mSize; i++) {
        mD4.mData[i].vf0C();
    }
    for (int i = 0; i < mE4.mSize; i++) {
        mE4.mData[i].vf0C();
    }
    for (int i = 0; i < mF4.mSize; i++) {
        mF4.mData[i].vf0C();
    }
    for (int i = 0; i < m104.mSize; i++) {
        m104.mData[i].vf0C();
    }
    for (int i = 0; i < m124.mSize; i++) {
        m124.mData[i].vf0C();
    }
    for (int i = 0; i < m134.mSize; i++) {
        m134.mData[i].vf0C();
    }
    for (int i = 0; i < m144.mSize; i++) {
        m144.mData[i].vf0C();
    }
    for (int i = 0; i < m154.mSize; i++) {
        m154.mData[i].vf0C();
    }
    if (!m98_15) {
        for (int i = 0; i < m164.mSize; i++) {
            m164.mData[i].vf0C();
        }
    }
    m1D4.fn_803B3AC4();
    if (keep != 0) {
        if (m1EC != 0) {
            fn_800D2338(m1EC);
            m1EC = 0;
        }
        if (m280 != 0) {
            fn_800D2338(m280);
            m280 = 0;
        }
        if (lbl_8087F540->m2400 != 0 && m190 != 0x151B && m190 != 0x151F) {
            lbl_8087F540->fn_8048FEF0();
        }
    }
    fn_800C3124(m74);
    m198 = 0.0f;
    m19C = 0.0f;
    if (!m98_15) {
        m98_1 = 0;
    }
    m194 = 4;
    m98_14 = 0;
}

// NONMATCHING: stub - builds the cuts of the event from the parsed file
void Sequence::fn_80540A08() {
}

// inferred: index of a cut (-1 if missing)
int Sequence::fn_80541198(Cut* cut) {
    for (unsigned int i = 0; i < mB8.size(); i++) {
        if (cut == mB8[i].get()) {
            return i;
        }
    }
    return -1;
}

// inferred: current cut (same code as fn_80541210)
Cut* Sequence::fn_805411DC() {
    return GetCut_unk(m1A0);
}

// inferred: current cut
Cut* Sequence::fn_80541210() {
    return GetCut_unk(m1A0);
}

// inferred: goes to cut `index`
void Sequence::fn_80541244(int index) {
    m1A0 = index;
    m1A4 = -1;
    m198 = 0.0f;
    m19C = 0.0f;
    if (m194 != 10) {
        return;
    }
    for (int i = 0; i < m164.mSize; i++) {
        Actor_unk* actor = &m164.mData[i];
        if (actor != 0) {
            actor->mB8[0] = 0;
            actor->mB8[1] = 0;
            actor->mB8[2] = 0;
        }
    }
}

// inferred: label `index` of any cut
Label* Sequence::fn_805412A8(int index) {
    int n = mB8.size();
    for (int i = 0; i < n; i++) {
        Cut* cut = IsCutIndex_unk(i) ? mB8[i].get() : 0;
        for (int j = 0; j < cut->mLabels.size(); j++) {
            Label* label = cut->IsLabelIndex_unk(j) ? cut->mLabels[j].get() : 0;
            if (index == label->mIndex) {
                return label;
            }
        }
    }
    return 0;
}

// inferred: jumps to a label (0x8000: to the start of a cut)
void Sequence::fn_8054136C(int label) {
    if (label == -1) {
        return;
    }
    if (label & 0x8000) {
        int index = label & ~0x8000;
        Cut* cut = 0;
        for (int i = 0; i < mB8.size(); i++) {
            if (index == mB8[i]->mIndex) {
                cut = mB8[i].get();
                break;
            }
        }
        fn_80541520(fn_80541198(cut), 0);
    } else {
        Label* found = fn_805412A8(label);
        if (found != 0) {
            fn_80541520(fn_80541198(found->mCut), found->mFrame);
        }
    }
}

void Sequence::fn_80541520(int index, int frame) {
    m1B0 = 1;
    m1B4 = index;
    m1B8 = frame;
}

void Sequence::fn_80541534(int index, int frame) {
    m1B0 = 2;
    m1B4 = index;
    m1B8 = frame;
}

void Sequence::fn_80541548(int slot, ScreenEffectObj_unk* obj, float start, float length, float a, float b, int c,
                           int d, int e, unsigned int flags) {
    ScreenEffect_unk& effect = m1F0[slot];
    effect.m00 = obj;
    effect.m04 = start;
    effect.m08 = length;
    effect.m0C = a;
    effect.m10 = b;
    effect.m14 = c;
    effect.m18 = d;
    effect.m1C = e;
    if (effect.m20 & 0x2000000) {
        flags |= 0x2000000;
    }
    effect.m20 = flags;
}

// NONMATCHING: stub - applies a screen effect
void Sequence::fn_80541588(int arg, float a, float b, int c, int d, int e, unsigned int f, float t) {
}

// inferred: event position to world
atn::Vector3 Sequence::fn_80541BD8(const atn::Vector3& pos) {
    atn::Vector3 v = pos;
    v.x -= m9C.x;
    v.y -= m9C.y;
    v.z -= m9C.z;
    Mtx m;
    PSMTXRotRad(m, 'y', mB4);
    PSMTXMultVec(m, &v, &v);
    v.x += mA8.x;
    v.y += mA8.y;
    v.z += mA8.z;
    return v;
}

// inferred: rotates `v` by the yaw of the event
atn::Vector3 Sequence::fn_80541C94(const atn::Vector3& v) {
    atn::Vector3 r = v;
    Mtx m;
    PSMTXRotRad(m, 'y', mB4);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: event yaw to world
float Sequence::fn_80541CF0(float yaw) {
    return yaw + mB4;
}

// inferred: an actor named after a character of kind `kind` ("np", "em", "fr") and number `num` exists
int Sequence::fn_80541CFC(int kind, int num) {
    for (int i = 0; i < m164.mSize; i++) {
        Actor_unk* actor = &m164.mData[i];
        if (actor->m08.size() == 0) {
            continue;
        }
        if (actor->m08.c_str()[0] == '!') {
            continue;
        }
        int match = 0;
        switch ((unsigned short)((actor->m08.c_str()[0] << 8) | actor->m08.c_str()[1])) {
        case 'em':
            match = kind == 2;
            break;
        case 'fr':
            match = kind == 3;
            break;
        case 'np':
            match = kind == 1;
            break;
        }
        if (match == 0) {
            continue;
        }
        char buf[0x20];
        memset(buf, 0, sizeof(buf));
        char* p = buf;
        int n = 0;
        for (int j = 2; j < actor->m08.size(); j++) {
            if (actor->m08.c_str()[j] >= '0' && actor->m08.c_str()[j] <= '9') {
                n++;
                *p++ = actor->m08.c_str()[j];
            }
        }
        if (atoi(buf) == num) {
            return 1;
        }
    }
    return 0;
}

// inferred: marks the actors that no character section of a cut uses
void Sequence::fn_80541F18() {
    for (int i = 0; i < m164.mSize; i++) {
        m164.mData[i].m18_7 = 1;
    }
    for (int i = 0; i < mB8.size(); i++) {
        Cut* cut = IsCutIndex_unk(i) ? mB8[i].get() : 0;
        for (int j = 0; j < cut->mSections.size(); j++) {
            Section* section = cut->IsSectionIndex_unk(j) ? cut->mSections[j].get() : 0;
            if (section->m0C == 3) {
                Actor_unk* actor = FindActor_unk(section->m10);
                if (actor != 0) {
                    actor->m18_7 = 0;
                }
            }
        }
    }
}

// NONMATCHING: stub
void Sequence::fn_8054204C() {
}

// NONMATCHING: stub
void Sequence::fn_80542684() {
}

// NONMATCHING: stub
void Sequence::fn_805428A8() {
}

// inferred: effect lists of the entry `id` of the list at 0x184
EffectLists_unk* Sequence::fn_80543060(int id) {
    for (int i = 0; i < m184.size(); i++) {
        if (id == m184[i].m00) {
            return &m184[i].m04;
        }
    }
    return 0;
}

// inferred: sets the continue effect `index`
void Sequence::fn_805430A0(unsigned int index, int key, int id) {
    if (index > 3) {
        return;
    }
    m294[index].m00 = key;
    m294[index].m04 = id;
}

// inferred: id of the continue effect `index` with the key `key` (-1 if none)
int Sequence::fn_805430BC(unsigned int index, int key) {
    if (index <= 3 && key == m294[index].m00) {
        return m294[index].m04;
    }
    return -1;
}

// TAIL: the functions below come after all the others of the file

// inferred: returns 1 (li r3, 0x1)
int StringData::vf10() const { return 1; }

void StringData::vf0C() {}

void StringData::vf08() {}

// inferred: returns 1 (li r3, 0x1)
int DummyData::vf10() const { return 1; }

void DummyData::vf0C() {}

void DummyData::vf08() {}

template <>
void DataList_unk<Actor_unk>::Add_unk(const char* name, int arg) {
    push_back(Actor_unk(mOwner, name, arg));
}

} // namespace Event
