#include "game/ArenaTask.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "macros.h"

extern "C" int rand();
extern "C" ArenaAnim_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: d2anime task
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a layout value
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" int swprintf(wchar_t* s, unsigned long n, const wchar_t* format, ...);
// inferred: debug drawing (lbl_8087EEB0) and the font used to measure the text (lbl_8087EEC8)
extern "C" void* lbl_8087EEB0;
extern "C" void* lbl_8087EEC8;
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width

// inferred: field (lbl_8087F430)
struct ArenaField_unk {
    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" ArenaField_unk* lbl_8087F430;


// inferred: enemy manager (lbl_8087F048); slots of 0x934 bytes
struct ArenaEnemySlot_unk {
    CharaTask* mChara;
    unsigned char m04[0x934 - 0x4];
};
struct ArenaEnemyMgr_unk {
    unsigned char m00[0xCBF0];
    ArenaEnemySlot_unk mSlots[72]; // 0xCBF0
    unsigned char m36290[0x363B0 - 0x36290];
    int mNumSlots; // 0x363B0
};
extern "C" ArenaEnemyMgr_unk* lbl_8087F048;

// inferred: voice/message window (lbl_8087F490->m2640)
struct ArenaVoiceWin_unk {
    // inferred: no message is shown or it is about to end (name invented)
    bool IsFree_unk() { return m50 == 0 || 6.0f <= m60; }
    unsigned char m00[0x50];
    int m50;   // inferred: nonzero while a message is shown
    unsigned char m54[0x60 - 0x54];
    float m60; // inferred: time left
    unsigned char m64[0xB4 - 0x64];
    int mB4;
};
struct ArenaVoiceOwner_unk {
    unsigned char m00[0x2640];
    ArenaVoiceWin_unk* m2640;
};
extern "C" ArenaVoiceOwner_unk* lbl_8087F490;
extern "C" void* fn_803CC6B4(RouteTable_unk* table, int id); // inferred: message by id
extern "C" void fn_803B6970(ArenaVoiceWin_unk* win, void* msg, int arg, float time); // inferred: shows a message
extern "C" void fn_803B78B4(ArenaVoiceWin_unk* win, void* msg); // inferred: queues a message

static ArenaTask* sInstance;


static const char* const sAnimPaths[4] = {
    "/data/d2anime/arena_ready_go_xx.d2a",
    "/data/d2anime/arena_draw_xx.d2a",
    "/data/d2anime/arena_win_xx.d2a",
    "/data/d2anime/arena_lose_xx.d2a",
};

static const char* const sSe[4] = {0, "SE_DG009_007", "SE_DG009_007", 0};

ArenaTask* ArenaTask::fn_8004318C(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (12, "", "", 0) ArenaTask(parent);
    }
    return sInstance;
}

bool ArenaTask::fn_800431F4(int a, int b) {
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
            mAnims[i]->mFC &= ~0x10000000;
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

// NONMATCHING: 99.4% - in the spawn block the quotient of rand() % n and the character pointer take
// r4/r5/r6 in another order (the erase loop of FixedArray_unk then uses r5/r6 instead of r4/r5).
// Tried: index/iterator/local orders, rand() in the subscript, unsigned index.
void ArenaTask::vf10() {
    if (mState == 1) {
        mScore0 = 0;
        mScore1 = 0;
        Entry_unk* e = mEntries.mData;
        for (unsigned int i = 0; i < mEntries.mCount; i++) {
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
        for (unsigned int i = 0; i < mEntries.mCount; i++) {
            Entry_unk& e = mEntries[i];
            if ((e.mFlags & 1) == 1 && e.mChara->IsGrappled_unk()) {
                unsigned int n = mSpawns.mCount;
                if (n != 0) {
                    int index = rand() % n;
                    atn::Vector3 rot;
                    rot.Assign_unk(e.mChara->mRot);
                    RoutePlace_unk** it = &mSpawns.mData[index];
                    RoutePlace_unk* spawn = *it;
                    rot.y = spawn->m14;
                    e.mChara->mPos.Assign_unk(spawn->mPos);
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
        float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
        fn_80061824(lbl_8087EEB0, 320.0f - w / 2.0f, 50.0f, 0.0f, size, size, 0.0f, 0.0f, 0.0f, text, -86, 1, 1, 0,
                    0, 0xFF000000);
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

// NONMATCHING: 98.8% - the enemy pointer and the slot address (first loop) and the table pointer and
// the place offset (second loop) are allocated to swapped registers. Tried: locals for the chara and
// the place, index or pointer loops, if/else for mTable, PushBack_unk with an implicit conversion.
void ArenaTask::fn_80043A24() {
    mTable = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (mTable == 0) {
        return;
    }
    ArenaEnemyMgr_unk* mgr = lbl_8087F048;
    if (mgr == 0) {
        return;
    }
    ArenaEnemySlot_unk* slot = mgr->mSlots;
    int n = mgr->mNumSlots;
    for (int i = 0; i < n && !mEntries.full(); i++) {
        mEntries.PushBack_unk(Entry_unk(slot->mChara));
        slot++;
    }
    mSpawns.mCount = 0;
    RouteTable_unk* table = mTable;
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        if (table->mPlaces[i].mId >= 50000) {
            mSpawns.PushBack_unk(&table->mPlaces[i]);
        }
    }
}

void ArenaTask::fn_80043B34() {
    if (mState == 1) {
        return;
    }
    mState = 1;
    if (mMode == 0) {
        for (unsigned int i = 0; i < mEntries.mCount; i++) {
            Entry_unk* e = &mEntries.mData[i];
            if (e->mChara->mType == 2) {
                e->mChara->mFlag16 = 1;
            }
        }
    } else if (mMode == 1) {
        for (unsigned int i = 0; i < mEntries.mCount; i++) {
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

// NONMATCHING: 99.7% - the count of the voice group and its address swap r28/r31. Tried: reference,
// pointer, operator[], count read through the group, separate index local.
void ArenaTask::fn_80043D58(int side) {
    if (mVoice == 0) {
        return;
    }
    ArenaVoiceWin_unk* win = lbl_8087F490->m2640;
    if (win == 0 || !win->IsFree_unk()) {
        return;
    }
    RouteTable_unk* table;
    if (lbl_8087F430 != 0) {
        table = lbl_8087F430->m10D8;
    } else {
        table = 0;
    }
    if (table == 0) {
        return;
    }
    FixedArray_unk<Voice_unk, 8>& voices = mVoices[side];
    unsigned int n = voices.mCount;
    if (n == 0) {
        return;
    }
    Voice_unk& v = voices.mData[rand() % n];
    void* msg = fn_803CC6B4(table, v.mFirst);
    if (msg != 0) {
        fn_803B6970(win, msg, 0, 6.0f);
    }
    win->mB4 = 0;
    if (v.mLast > 0) {
        for (int id = v.mFirst + 1; id <= v.mLast; id++) {
            msg = fn_803CC6B4(table, id);
            if (msg != 0) {
                fn_803B78B4(win, msg);
            }
        }
    }
}
