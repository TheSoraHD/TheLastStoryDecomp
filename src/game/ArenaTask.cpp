#include "game/GameTask.h"
#include "game/ArenaTask.h"
#include "game/GameScene.h"
#include "game/GameUI.h"
#include "game/CharaTask.h"
#include "game/BattleTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "macros.h"
#include "atn/DrawHelper.h"
#include "game/GameSceneFind_unk.h"

extern "C" int rand();
extern "C" ArenaAnim_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: d2anime task
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a layout value
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" int swprintf(wchar_t* s, unsigned long n, const wchar_t* format, ...);
// inferred: debug drawing (lbl_8087EEB0) and the font used to measure the text (lbl_8087EEC8)
namespace atn {
class Font_unk;
}
extern "C" atn::Font_unk* lbl_8087EEC8;
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float f2, int a, int b); // inferred: width

extern "C" GameTask* lbl_8087F430;


extern "C" BattleTask* lbl_8087F048; // inferred: the battle manager

ArenaTask* ArenaTask::sInstance;


static const char* const sAnimPaths[4] = {
    "/data/d2anime/arena_ready_go_xx.d2a",
    "/data/d2anime/arena_draw_xx.d2a",
    "/data/d2anime/arena_win_xx.d2a",
    "/data/d2anime/arena_lose_xx.d2a",
};

static const char* const sSe[4] = {0, "SE_DG009_007", "SE_DG009_007", 0};

ArenaTask* ArenaTask::fn_8004318C(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)12, "", "", 0) ArenaTask(parent);
    }
    return sInstance;
}

bool ArenaTask::fn_800431F4(int a, int b, int) {
    if (a == 2 && b == 9) {
        return true;
    }
    return false;
}

ArenaTask::ArenaTask(atn::Task* parent)
    : atn::Task(parent), mState(0), mTable(0), m50(0), m54(0), mMode(0), m5C(0), mScore0(0), mScore1(0),
      mTimer(0), mResult(0), mFirstResult(1), mSe(1), mVoice(1) {
    for (int i = 0; i < 4; i++) {
        if (sAnimPaths[i] != 0) {
            mAnims[i] = fn_801F3FF8(this, sAnimPaths[i], 0);
            mAnims[i]->mRepeat = 0;
        } else {
            mAnims[i] = 0;
        }
    }
}

ArenaTask::~ArenaTask() {
    sInstance = 0;
}

int ArenaTask::vf0C() {
    return fn_800D3FA4();
}

void ArenaTask::vf10() {
    if (mState == 1) {
        mScore0 = 0;
        mScore1 = 0;
        Entry_unk* e = mEntries.mData;
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            if (e->mChara->mType == 2) {
                mScore0 += e->mScore;
            } else {
                mScore1 += e->mScore;
            }
            e++;
        }
    }
    if (mTimer > 0) {
        mTimer--;
    }
    if (mMode == 0) {
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            Entry_unk& e = mEntries[i];
            if ((e.mFlags & 1) == 1 && e.mChara->IsGrappled_unk()) {
                unsigned int n = mSpawns.mSize;
                if (n != 0) {
                    int index = rand() % n;
                    atn::Vector3 rot;
                    // inferred: the character in a local for the rotation and the position; the original
                    // reloads e.mChara for the rotation store (all three through the local: 98.0%)
                    CharaTask* chara = e.mChara;
                    rot.Assign_unk(chara->mRot);
                    gs::WarpPoint** it = &mSpawns.mData[index];
                    gs::WarpPoint* spawn = *it;
                    rot.y = spawn->m14;
                    chara->mPos.Assign_unk(spawn->mPos);
                    e.mChara->mRot.Assign_unk(rot);
                    e.mSpawn = spawn;
                    mSpawns.erase(it);
                }
                e.mFlags &= ~1;
            }
            if (e.mChara->CanAct_unk() && e.mSpawn != 0) {
                mSpawns.PushBack_unk(e.mSpawn);
                e.mSpawn = 0;
            }
        }
    }
}

void ArenaTask::vf14() {
    if (mState == 1 && mMode == 0) {
        float t = MIN((float)mTimer / 5.0f, 1.0f);
        float size = 36.0f * (1.0f + t);
        wchar_t text[256];
        swprintf(text, 256, L"%d - %d", mScore0, mScore1);
        float w = fn_8006EF48(lbl_8087EEC8, size, text, 0.0f, 1, 1);
        lbl_8087EEB0->fn_80061824(320.0f - w / 2.0f, 50.0f, 0.0f, size, size, 0.0f, 0.0f, 0.0f, text, -86, 1, 1, 0, 0, 0xFF000000);
    }
    for (int i = 0; i < 4; i++) {
        if (mAnims[i] != 0) {
            mAnims[i]->mFlags |= 4;
        }
    }
    if (0 <= mResult && mResult < 5) {
        ArenaAnim_unk* anim = 0;
        int index = 0;
        switch (mResult) {
        case 1:
            anim = mAnims[0];
            index = -1;
            break;
        case 2:
            anim = mAnims[2];
            index = 0;
            break;
        case 3:
            anim = mAnims[3];
            index = 2;
            break;
        case 4:
            anim = mAnims[1];
            index = 1;
            break;
        }
        if (anim != 0) {
            anim->mFlags &= ~4;
            fn_801FECE0(anim->m58, fn_800DC6B4("resultIndex"), index);
            if (mFirstResult != 0) {
                anim->m100 = 0.0f;
                mFirstResult = 0;
            }
        }
    }
}

// inferred: adds the character of a battle slot (name invented)
static inline void AddEntry_unk(ArenaTask* self, BattleChara_unk* slot) {
    self->mEntries.PushBack_unk(ArenaTask::Entry_unk(slot->mChara));
}

// inferred: collects the spawn points of the arena (ids from 50000; name invented). The counter is the one
// of the caller's first loop (same register in both loops in the original)
static inline void AddSpawns_unk(ArenaTask* self, GameScene* table, int& i) {
    for (i = 0; i < table->m78.size(); i++) {
        gs::WarpPoint& place = table->m78.mData[i];
        if (place.mId >= 50000) {
            self->mSpawns.PushBack_unk(&place);
        }
    }
}

void ArenaTask::fn_80043A24() {
    mTable = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (mTable == 0) {
        return;
    }
    BattleTask* mgr = lbl_8087F048;
    if (mgr == 0) {
        return;
    }
    BattleChara_unk* slot = mgr->mCharas;
    int n = mgr->mCharaCount;
    int i;
    for (i = 0; i < n && !mEntries.full(); i++) {
        AddEntry_unk(this, slot);
        slot++;
    }
    mSpawns.mSize = 0;
    AddSpawns_unk(this, mTable, i);
}

void ArenaTask::fn_80043B34() {
    if (mState == 1) {
        return;
    }
    mState = 1;
    if (mMode == 0) {
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            Entry_unk* e = &mEntries.mData[i];
            if (e->mChara->mType == 2) {
                e->mChara->mFlag16 = 1;
            }
        }
    } else if (mMode == 1) {
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            Entry_unk* e = &mEntries.mData[i];
            if (e->mChara->mType == 2) {
                e->mChara->mFlag16 = 0;
            }
        }
    }
}

void ArenaTask::fn_80043BDC() {
    if (mState == 1) {
        mState = 2;
    }
}

void ArenaTask::fn_80043BF4(int type, CharaTask* victim, CharaTask* attacker) {
    Entry_unk* a;
    Entry_unk* v = Find_unk(victim);
    a = Find_unk(attacker);
    if (type == 1) {
        if (mSe != 0) {
            fn_800C31F4(sSe[mSe], 0, -1, 1.0f);
        }
        if (v != 0) {
            v->mDefeated++;
        }
        if (a != 0) {
            a->mScore++;
            if (mMode == 0) {
                a->mFlags |= 1;
            }
        }
        mTimer = 15;
    }
    if (attacker->mType == 0 || attacker->mType == 3) {
        fn_80043D58(0);
    } else if (attacker->mType == 2) {
        fn_80043D58(1);
    }
}

void ArenaTask::fn_80043D58(int side) {
    if (mVoice == 0) {
        return;
    }
    GameTextMessageWindow* win = lbl_8087F490->m2640;
    if (win == 0 || !win->IsFree_unk(6.0f)) {
        return;
    }
    GameScene* table;
    if (lbl_8087F430 != 0) {
        table = lbl_8087F430->m10D8;
    } else {
        table = 0;
    }
    if (table == 0) {
        return;
    }
    FixedArray_unk<Voice_unk, 8>& voices = mVoices[side];
    if (voices.mSize == 0) {
        return;
    }
    unsigned int n = voices.mSize;
    Voice_unk& v = voices.mData[rand() % n];

    GameMessage* msg = (GameMessage*)((GameScene*)table)->fn_803CC6B4(v.mFirst);
    if (msg != 0) {
        win->fn_803B6970(msg, 0, 6.0f);
    }
    win->mCount = 0;
    if (v.mLast > 0) {
        for (int id = v.mFirst + 1; id <= v.mLast; id++) {
            msg = (GameMessage*)((GameScene*)table)->fn_803CC6B4(id);
            if (msg != 0) {
                win->fn_803B78B4(msg);
            }
        }
    }
}
