#include "gmk/RandomMerchant.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/MarketTask.h"
#include "game/PlayerTask.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: creates the gimmick of an item (kind 10) as a child of parent
extern "C" GimmickCommon* fn_803EC16C(atn::Task* parent, int item, int index);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
// inferred: empty names passed to operator new (in .sdata of another file)
extern "C" char lbl_8087DFD0[4];
extern "C" char lbl_8087DFD4[4];
// inferred: field object (class unknown)
struct Field_unk {
    void fn_803750E4(int id);
    unsigned char m00[0x10D0];
    void* m10D0; // inferred: item lottery of the merchants
    void* m10D4;
    void* m10D8; // inferred: message table
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4; // inferred: nonzero while the field is paused
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
extern "C" void* fn_8021DE88(void* lottery);                              // inferred: next lot (null when none is left)
extern "C" int fn_8021DECC(void* lot, int* item, int* num, int* price);  // inferred: item of the lot
// inferred: item definition (class unknown)
struct MerchantItem_unk {
    unsigned char m00[0x08];
    const wchar_t* mName; // 0x08
    unsigned char m0C[0xC8 - 0x0C];
    int mPrice; // 0xC8
};
extern "C" MerchantItem_unk* fn_80211480(int id);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" void* lbl_8087EEB8; // inferred: debug message output
// inferred: debug message list (atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
// inferred: message of the message table (class unknown)
struct MerchantMsg_unk {
    int m00;
    unsigned char m04[0x08 - 0x04];
    const wchar_t* mFormat; // 0x08
    int m0C;
    unsigned char m10[0x30 - 0x10];
    int m30;
    int m34;
};
extern "C" MerchantMsg_unk* fn_803CC6B4(void* table, int id);                 // inferred: message by id
extern "C" void fn_8068B2A0(int value, wchar_t* buf, int radix);             // inferred: integer to text
extern "C" void fn_800DD8A8(wchar_t* buf, const wchar_t* format, int count, ...); // inferred: formats a message
extern "C" void fn_803E3BE8(void* ui, int* message, float time);              // inferred: shows a message window
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: item manager and shop (classes unknown)
extern "C" void* lbl_8087F4F0;
extern "C" void fn_8044D6AC(void* mgr, int price);
extern "C" void fn_804439FC(void* mgr, int item, int num, int arg3, int arg4, int arg5, int arg6, int arg7);
extern "C" void* lbl_8087F048;
extern "C" void fn_801092C8(void* mgr, int arg1, int arg2, int item, int num);
extern "C" void* lbl_8087F490; // inferred: GameUI
extern "C" void fn_803E5E64(void* ui, int item, int num, int arg3);

// inferred: rotation matrix z * y * x (same as GimmickCommon.cpp)
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}

namespace gmk {

GimmickCommon* RandomMerchant::fn_804310B0(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        char path[0x100];
        sprintf(path, "gm060_%03d.gmk", id % 1000);
        if (fn_8006AA20(path) == 0) {
            id = id / 1000 * 1000;
        }
        return new (12, "", "", 0) RandomMerchant(parent, id, arg);
    }
    return 0;
}

RandomMerchant::RandomMerchant(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mState(0), mLoaded(0), mCur(0), mCount(0), mItemId(0),
      mItemNum(0), mPrice(0), mPosCount(0), mTimer(0), mBlank(0), mFirstTime(0), mLimit(0), mTimeLimit(0),
      mLength2(0.0f), mLength1(0.0f), mBought(0), m3068(0), m306C(0), m3070(0), m3074(0), m3098(0), m309C(0) {
    mE8 = 6;
    mEC = 2;
}

RandomMerchant::~RandomMerchant() {
    fn_800CB5C8(&mSound, 0, 0);
}

// NONMATCHING: 99.77% - the std::string and the Vector3 temporary swap stack slots, register of the
// last loop pointer
int RandomMerchant::vf0C() {
    if (fn_803EC91C() == 0 && fn_800D3FA4() != 0) {
        if (mLoaded == 0) {
            for (int i = 0; i < mCount; i++) {
                MerchantEntry_unk* entry = &mEntries[i];
                {
                    std::string name(entry->mName.mStr);
                    entry->mChara =
                        this != 0 ? new (1, lbl_8087DFD4, lbl_8087DFD0, 0) RandomMerchantChara(this, name) : 0;
                }
                entry->mMotionBase = 0xEF;
                for (int j = 0; j < 3; j++) {
                    fn_80097A88(&entry->mChara->mMotion, entry->mMotionBase + j, entry->mMotions[j].mStr);
                }
                if (entry->mItem / 1000 == 10) {
                    entry->mItemGimmick = fn_803EC16C(this, entry->mItem, i);
                    entry->mItemGimmick->fn_800D246C(1);
                }
            }
            mLoaded = 1;
        } else {
            int ok = 1;
            MerchantEntry_unk* entry = mEntries;
            for (int i = 0; i < mCount; i++) {
                if (entry->mChara->mMotion.fn_8008B140() != 0) {
                    ok = 0;
                    break;
                }
                if (entry->mItemGimmick != 0 && !entry->mItemGimmick->IsFlag_unk(2)) {
                    ok = 0;
                    break;
                }
                entry++;
            }
            if (ok != 0) {
                mState = 1;
                for (int i = 0; i < mCount; i++) {
                    mEntries[i].mChara->m540 = atn::Vector3(1.0f, 1.0f, 1.0f);
                    mEntries[i].mChara->fn_800D246C(0);
                    mEntries[i].mChara->fn_801765D8();
                    if (mEntries[i].mItemGimmick != 0) {
                        mEntries[i].mItemGimmick->fn_800D246C(0);
                        mEntries[i].mItemGimmick->mFlags |= 4;
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

void RandomMerchant::Start() {
    m54 = 1;
    mTimer = mFirstTime;
}

inline void RandomMerchant::PlayMotion2_unk(RandomMerchantChara* chara, int index) {
    chara->mMotion.m34C = 1;
    chara->mMotion.fn_80097C08(0, mEntries[mCur].mMotionBase + index, 1, 0, 1, 0.0f, 0.1f);
    chara->mMotion.mTracks[0].m0C = 1.0f;
}

inline void RandomMerchant::PlayMotion3_unk(Model_unk* motion, int index) {
    motion->m34C = 1;
    motion->fn_80097C08(0, mEntries[mCur].mMotionBase + index, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 98.28% - stack slots of several Vector3 locals, the busy check branches over a b, and a few
// register choices (item kept in r4, motion pointer of state 2)
void RandomMerchant::vf10() {
    if (mState >= 2 && mEntries[mCur].mItemGimmick != 0) {
        atn::Vector3 off(0.0f, 0.0f, -3.0f);
        atn::Matrix3x4 mtx;
        __MakeRotation_unk(mtx, m78);
        PSMTXMultVec(mtx.m, &off, &off);
        mEntries[mCur].mItemGimmick->m6C = mEntries[mCur].mChara->mPos + off;
        mEntries[mCur].mItemGimmick->m78 = mEntries[mCur].mChara->mRot;
    }
    CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 0) {
        return;
    }
    if (player == 0) {
        return;
    }
    if (player->fn_8017C974() != 0) {
        return;
    }
    if (m54 == 0) {
        return;
    }
    if (mState == 1) {
        for (int i = 0; i < mCount; i++) {
            if (mEntries[i].mChara != 0 && !mEntries[i].mChara->IsFlag_unk(4)) {
                mEntries[i].mChara->fn_801765D8();
            }
            if (mEntries[i].mItemGimmick != 0 && !mEntries[i].mItemGimmick->IsFlag_unk(4)) {
                mEntries[i].mItemGimmick->mFlags |= 4;
            }
        }
        if (mTimer <= 0) {
            void* lot = fn_8021DE88(lbl_8087F430->m10D0);
            if (lot != 0) {
                int item;
                int num;
                int price;
                if (fn_8021DECC(lot, &item, &num, &price) != 0 && item > 0) {
                    mItemNum = num;
                    mItemId = item;
                    mPrice = price;
                    if (MarketTask::sInstance != 0 && MarketTask::sInstance->fn_804A24C4(item) != 0) {
                        mPrice = 0.2f * (mItemNum * MarketTask::sInstance->fn_804A251C(mItemId));
                    } else {
                        MerchantItem_unk* def = fn_80211480(item);
                        if (def != 0) {
                            mPrice = 0.2f * (def->mPrice * mItemNum);
                        }
                    }
                }
            } else {
                mTimer = 300;
                return;
            }
            mCur = rand() % mCount;
            atn::FixedArray_unk<int, 16> list;
            atn::Vector3 ppos = player->mPos;
            MerchantPos_unk* pos = mPositions;
            for (int i = 0; i < mPosCount; i++) {
                atn::Vector3 d = pos->mPos - ppos;
                float dist = fn_805F9920(&d);
                if (mLength1 * mLength1 < dist && dist < mLength2 * mLength2) {
                    list.push_back(i);
                }
                pos++;
            }
            unsigned int n = list.size();
            if (list.mSize == 0) {
                int r = rand();
                mLimit = mTimeLimit;
                mTimer = r % (mBlank + 1);
                return;
            }
            int index = list[rand() % n];
            m6C = mPositions[index].mPos;
            m78.Assign_unk(mPositions[index].mRot);
            mDir.Set_unk(0.0f, 0.0f, 1.0f);
            Mtx rot;
            PSMTXRotRad(rot, 'y', m78.y);
            PSMTXMultVec(rot, &mDir, &mDir);
            MerchantEntry_unk* cur = &mEntries[mCur];
            RandomMerchantChara* chara = cur->mChara;
            chara->mPos = m6C;
            chara->mRot = m78;
            chara->mMotion.m34C = 1;
            chara->mMotion.fn_80097C08(0, cur->mMotionBase, 1, 0, 1, 0.0f, 0.1f);
            chara->mMotion.mTracks[0].m0C = 1.0f;
            chara->fn_80176ACC();
            if (cur->mItemGimmick != 0) {
                cur->mItemGimmick->mFlags &= ~4;
                atn::Vector3 off(0.0f, 0.0f, -3.0f);
                atn::Matrix3x4 mtx;
                __MakeRotation_unk(mtx, m78);
                PSMTXMultVec(mtx.m, &off, &off);
                cur->mItemGimmick->m6C = chara->mPos + off;
                cur->mItemGimmick->m78 = chara->mRot;
            }
            mLimit = mTimeLimit;
            wchar_t text[0x100];
            wchar_t name[0x100] = {0};
            MerchantItem_unk* def = fn_80211480(mItemId);
            if (def != 0) {
                wcscpy(name, def->mName);
            } else {
                char buf[0x100];
                sprintf(buf, "cannot found item %d!", mItemId);
                ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(buf);
            }
            MerchantMsg_unk* msg = fn_803CC6B4(lbl_8087F430->m10D8, mEntries[mCur].m290);
            wchar_t numText[0x40] = {0};
            wchar_t priceText[0x40] = {0};
            fn_8068B2A0(mItemNum, numText, 10);
            fn_8068B2A0(mPrice, priceText, 10);
            fn_800DD8A8(text, msg->mFormat, 3, name, numText, priceText);
            m3070 = text;
            m309C = msg->m34;
            m3098 = msg->m30;
            m3068 = msg->m00;
            m3074 = msg->m0C;
            fn_803E3BE8(lbl_8087F490, &m3068, 7.5f);
            if (lbl_8087F430 != 0) {
                lbl_8087F430->fn_803750E4(0xDC);
            }
            m54 = 1;
            mState = 2;
        } else {
            mTimer--;
        }
    } else if (mState == 2) {
        RandomMerchantChara* chara = mEntries[mCur].mChara;
        PlayMotion2_unk(chara, 0);
        Model_unk* motion = &chara->mMotion;
        if (mLimit >= 0) {
            mLimit--;
            if (mTimeLimit - mLimit > mTimeLimit * 2 / 3 && mSound.mPtr != 0) {
                fn_800CB5C8(&mSound, 150, 0);
            }
        } else {
            PlayMotion3_unk(motion, 2);
        }
    } else if (mState == 3) {
        RandomMerchantChara* chara = mEntries[mCur].mChara;
        PlayMotion2_unk(chara, 1);
        if (m54 == 1) {
            mState = 2;
        }
    } else if (mState == 4) {
        RandomMerchantChara* chara = mEntries[mCur].mChara;
        PlayMotion2_unk(chara, 2);
    }
    if ((mState == 2 && mLimit < 0) || mState >= 3) {
        atn::Vector3 ppos = player->mPos;
        atn::Vector3 d = m6C - ppos;
        if (fn_805F9920(&d) >= mLength2 * mLength2) {
            mState = 1;
        }
    }
}

void RandomMerchant::vf14() {}

int RandomMerchant::vf1C() {
    return 0;
}

// NONMATCHING: 99.32% - the entry pointer and the position base swap r25/r26 (and the field registers)
void RandomMerchant::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    MerchantEntry_unk* entry = mEntries;
    int count = 0;
    int posCount = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "CHARA_SET") == 0) {
            if (count < 16) {
                entry->mName = csv.fn_8005B9CC();
                entry->mItem = atoi(csv.fn_8005B9CC());
                for (int j = 0; j < 3; j++) {
                    const char* s = csv.fn_8005B9CC();
                    if (s[0] != 0) {
                        entry->mMotions[j] = s;
                    }

                }
                entry->m290 = atoi(csv.fn_8005B9CC());
                entry->mEvent1 = atoi(csv.fn_8005B9CC());
                entry->mEvent2 = atoi(csv.fn_8005B9CC());
                entry++;
                count++;
            }
        } else if (strcmp(field, "APPEAR_POS") == 0) {
            if (count < 16) {
                float x = fn_800DC288(csv.fn_8005B9CC());
                float y = fn_800DC288(csv.fn_8005B9CC());
                float z = fn_800DC288(csv.fn_8005B9CC());
                mPositions[posCount].mPos.Set_unk(x, y, z);
                float rx = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                float ry = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                float rz = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
                mPositions[posCount].mRot.Set_unk(rx, ry, rz);
                posCount++;
            }
        } else if (strcmp(field, "APPEAR_BRANK") == 0) {
            mBlank = fn_800DC12C(csv.fn_8005B9CC()) * 30;
        } else if (strcmp(field, "TIME_LIMIT") == 0) {
            mTimeLimit = fn_800DC12C(csv.fn_8005B9CC()) * 30;
        } else if (strcmp(field, "APPEAR_LENGTH") == 0) {
            mLength1 = fn_800DC288(csv.fn_8005B9CC());
            mLength2 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "FIRST_APPEAR_TIME") == 0) {
            mFirstTime = fn_800DC12C(csv.fn_8005B9CC()) * 30;
        }
    } while (csv.fn_8005BBF8() != 0);
    mCount = count;
    mPosCount = posCount;
    mTimer = mFirstTime;
    if (mBlank > 300) {
        mTimer = mFirstTime;
    }
    mLimit = mTimeLimit;
}

int RandomMerchant::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 0:
    case 1:
        mState = 1;
        break;
    case 2:
        if (m54 == 1 && mState == 3) {
            fn_8044D6AC(lbl_8087F4F0, mPrice);
            fn_804439FC(lbl_8087F4F0, mItemId, mItemNum, 0, 0, 0, 1, 1);
            if (lbl_8087F048 != 0) {
                fn_801092C8(lbl_8087F048, 1, 0, mItemId, mItemNum);
            }
            fn_803E5E64(lbl_8087F490, mItemId, mItemNum, 1);
            fn_800C31F4("SE_UICOM_008", 0, -1, 1.0f);
        }
        break;
    }
    return m54 = msg->mType;
}

void RandomMerchant::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int RandomMerchant::vf60() {
    int r = 0;
    if (2 <= mState && mState <= 4) {
        r = 1;
    }
    return r;

}

int RandomMerchant::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0 && flag == 0) {
        atn::Vector3 d = m6C - *pos;
        float len = fn_805F9920(&d);
        if (len > 0.0f) {
            PSVECNormalize(&d, &d);
        }
        if (len < 324.0f) {
            return PSVECDotProduct(dir, &d) >= 0.5f;
        }
    }
    return 0;
}

inline void RandomMerchant::PlayMotion_unk(int motion) {
    RandomMerchantChara* chara = mEntries[mCur].mChara;
    chara->mMotion.m34C = 1;
    chara->mMotion.fn_80097C08(0, mEntries[mCur].mMotionBase + motion, 0, 0, 1, 0.0f, 0.1f);
    chara->mMotion.mTracks[0].m0C = 1.0f;
}

void RandomMerchant::vf4C(CharaTask* chara) {
    if (m54 == 1) {
        if (mState == 2) {
            if (mLimit >= 0) {
                PlayMotion_unk(1);
                if (lbl_8087F430 != 0) {
                    fn_80370320(lbl_8087F430, mEntries[mCur].mEvent1, 1, 0);
                }
                if (mSound.mPtr != 0) {
                    fn_800CB5C8(&mSound, 15, 0);
                }
                mBought = 1;
                mState = 3;
            } else {
                PlayMotion_unk(2);
                if (lbl_8087F430 != 0) {
                    fn_80370320(lbl_8087F430, mEntries[mCur].mEvent2, 1, 0);
                }
                if (mSound.mPtr != 0) {
                    fn_800CB5C8(&mSound, 15, 0);
                }
                mBought = 0;
                mState = 4;
            }
        } else if (mState == 3) {
            if (lbl_8087F430 != 0) {
                fn_80370320(lbl_8087F430, mEntries[mCur].mEvent1, 1, 0);
            }
        } else if (mState == 4) {
            if (lbl_8087F430 != 0) {
                fn_80370320(lbl_8087F430, mEntries[mCur].mEvent2, 1, 0);
            }
        }
    } else if (mState == 3) {
        if (lbl_8087F430 != 0) {
            fn_80370320(lbl_8087F430, mEntries[mCur].mEvent1, 1, 0);
        }
    } else if (mState == 4) {
        if (lbl_8087F430 != 0) {
            fn_80370320(lbl_8087F430, mEntries[mCur].mEvent2, 1, 0);
        }
    }
}

int RandomMerchant::vf64(int arg) {
    if (arg == 0) {
        return mItemId;
    }
    if (arg == 1) {
        return mPrice;
    }
    return 0;
}

} // namespace gmk
