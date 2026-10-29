#include "gmk/TreasureBox.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" void* lbl_8087F430; // inferred: field object
extern "C" int fn_80370174(void* field, int id); // inferred: field flag
// inferred: item tables (names invented)
struct ItemEntry_unk {
    int mId;
    int mCount;
    int m08;
};
struct ItemTable_unk {
    int m00;
    ItemEntry_unk* mEntries; // 0x04
};
extern "C" ItemTable_unk* fn_8021E444(int table);
extern "C" int fn_8021E5A4(ItemTable_unk* table, int flags); // inferred: draws an entry
extern "C" void* lbl_8087F4F0; // inferred: item manager
extern "C" int fn_8044909C(void* mgr);
extern "C" void fn_80445130(void* mgr, int* id, int* count, int arg);
extern "C" int fn_80448F9C(void* mgr, int id);
extern "C" int fn_80450B84(int table, int arg1, int arg2);
extern "C" void* fn_80211480(int id); // inferred: item data
extern "C" int fn_80206B9C(int id);
extern "C" gmk::TreasureBox::Data_unk* fn_8021ECD0(int id);

// inferred: sound of the opening
static const char* const sOpenSound = "SE_DGCOM_006";

// inferred: plays the effect of the model (name invented)
inline void PlayEfp_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
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

namespace gmk {

GimmickCommon* TreasureBox::fn_8040DC30(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) TreasureBox(parent, id, arg);
    }
    return 0;
}

TreasureBox::~TreasureBox() {}

int TreasureBox::vf0C() {
    if (fn_803EC91C() == 0 && fn_800D3FA4() != 0 && mAnime.fn_8044E1BC() == 0) {
        m54 = 1;
        return 1;
    }
    return 0;
}

void TreasureBox::vf10() {
    if (m54 == 0) {
        vf28();
    } else if (m54 == 2) {
        GimmickMsg_unk msg(3);
        if (m58 != 0) {
            msg.mType = 5;
        }
        vf40(&msg);
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
    }
    switch (m54) {
    case 1:
    case 4:
        mAnime.fn_8044F434();
        break;
    case 3: {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        float frame = mModel.mTracks[0].m08;
        if (frame >= mModel.fn_80097D7C(0)) {
            if (mAnime.fn_8044E610() != 0) {
                if (mFirst != 0) {
                    mAnime.fn_8044E644();
                } else {
                    mAnime.fn_8044F2D4();
                }
            } else {
                if (!mAnime.IsBusy_unk()) {
                    GimmickMsg_unk msg(4);
                    vf40(&msg);
                }
            }
        }
        break;
    }
    case 5:
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        if (mModel.fn_80097D40(1) != 0) {
            float frame = mModel.mTracks[0].m08;
            if (frame >= mModel.fn_80097D7C(0)) {
                GimmickMsg_unk msg(1);
                vf40(&msg);
            }
        } else if (mModel.mTracks[0].m08 <= 0.0f) {
            GimmickMsg_unk msg(1);
            vf40(&msg);
        }
        break;
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    if (m54 != 6) {
        mColli.mFlags |= 1;
    } else {
        mColli.mFlags &= ~1;
    }
    switch (m54) {
    case 3:
    case 4:
        mAnime.fn_8044F434();
        break;
    }
}

void TreasureBox::vf14() {
    if (m54 != 6 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int TreasureBox::vf1C() {
    int ret = 0;
    if (mModel.fn_8008B140() != 0 || mColli.fn_800580BC() != 0 || mEfp.fn_80237874() != 0) {
        ret = 1;
    }
    return ret;
}

void TreasureBox::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void TreasureBox::vf28() {
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    if (m54 != 6) {
        mColli.mFlags |= 1;
    } else {
        mColli.mFlags &= ~1;
    }
    GimmickMsg_unk msg(1);
    if (lbl_8087F430 != 0 && mData != 0 && fn_80370174(lbl_8087F430, mData->mFlag) > 0) {
        msg.mType = 4;
    }
    vf40(&msg);
}

int TreasureBox::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 0:
        vf28();
        break;
    case 1: {
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        lbl_8087F3C0->mB8 = 1;
        fn_80232B7C(this, 0);
        PlayEfp_unk(&mEfp, &mModel);
        lbl_8087F3C0->mB8 = 0;
        if (mModel.fn_80097D40(2) != 0) {
            mModel.fn_80097C08(0, 2, 1, 1, 1, 1.0f, 0.1f);
        } else {
            mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        }
        mModel.mTracks[0].m0C = 1.0f;
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    }
    case 3: {
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m0C = 1.0f;
        fn_800C344C(sOpenSound, &m6C, 0, -1, 1.0f);
        ItemTable_unk* table = 0;
        int id = 0;
        void* item = 0;
        int count = 0;
        if (mData != 0) {
            table = fn_8021E444(mData->mTable);
        }
        if (table != 0) {
            mItem = fn_8021E5A4(table, (lbl_8087F4F0 != 0 ? fn_8044909C(lbl_8087F4F0) : 0) | 1);
            if (mItem >= 0) {
                ItemEntry_unk* entry = &table->mEntries[mItem];
                id = entry->mId;
                count = entry->mCount;
                mFirst = fn_80450B84(mData->mTable, 0, 1);
                if (lbl_8087F4F0 != 0) {
                    fn_80445130(lbl_8087F4F0, &id, &count, 1);
                }
                item = fn_80211480(id);
            }
        }
        if (item != 0 && count > 0) {
            int rare = 1;
            if (fn_80206B9C(id) != 0) {
                rare = 0;
                int chance = fn_80448F9C(lbl_8087F4F0, id);
                if (chance > rand() % 100) {
                    rare = 1;
                }
            }
            int tableId = mData != 0 ? mData->mTable : 0;
            atn::Vector3 pos = m6C + atn::Vector3(0.0f, 12.0f, 0.0f);
            mAnime.fn_8044E418(&pos, 30, 15, item, count, rare, tableId);
        }
        break;
    }
    case 4:
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
        mModel.mTracks[0].m0C = 1.0f;
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        break;
    case 5:
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        if (mModel.fn_80097D40(1) != 0) {
            mModel.fn_80097C08(0, 1, 0, 1, 1, 1.0f, 0.1f);
            mModel.mTracks[0].m0C = 1.0f;
        } else {
            mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
            mModel.mTracks[0].m08 = mModel.fn_80097D7C(0);
            mModel.mTracks[0].m0C = -1.0f;
        }
        break;
    default:
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        break;
    }
    return m54 = msg->mType;
}

void TreasureBox::vf70(int state) {
    GimmickMsg_unk msg(1);
    switch (state) {
    case 2:
    case 3:
    case 4:
        msg.mType = 4;
        break;
    case 6:
        msg.mType = 6;
        break;
    }
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

int TreasureBox::vf64(int arg) {
    return mItem;
}

void TreasureBox::vf68(int arg1, int arg2) {
    if (arg1 == 0) {
        mData = fn_8021ECD0(arg2);
    } else {
        mAnime.fn_80450778();
    }
}

atn::Model* TreasureBox::vf38() {
    return &mModel;
}

} // namespace gmk
