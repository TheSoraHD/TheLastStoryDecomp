#include "gmk/SavePoint.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound

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
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id);          // inferred: an effect is playing
extern "C" void fn_80232B7C(void* owner, int arg);

// inferred: field object (class unknown)
struct Field_unk {
    void fn_803750E4(int id);
    unsigned char m00[0x74];
    atn::Vector3 m74; // inferred: camera position
    unsigned char m80[0x54E4 - 0x80];
    int m54E4; // inferred: 8 while the field is paused
    unsigned char m54E8[0x5694 - 0x54E8];
    int m5694;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80375184(Field_unk* field);             // inferred: saving is allowed
extern "C" GimmickCommon* fn_8036E098(Field_unk* field);  // inferred: nearest save point
extern "C" void fn_8036E92C(Field_unk* field, int arg);   // inferred: opens the save menu
extern "C" int fn_80370174(Field_unk* field, int id);     // inferred: field flag
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg3); // inferred: sets a field flag
// inferred: global objects (classes unknown)
struct SaveEvent_unk {
    unsigned char m00[0x48];
    int m48; // inferred: an event is running
};
extern "C" SaveEvent_unk* lbl_8087F098;
extern "C" void* lbl_8087EF68;
extern "C" int fn_800A4228(void* obj, int* out);
struct SaveDebug_unk {
    unsigned char m00[0x18];
    int m18; // inferred: debug draw
    unsigned char m1C[0x198 - 0x1C];
    int m198; // inferred: the gimmick is disabled
};
extern "C" SaveDebug_unk* lbl_8087F0A8;

extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: plays an effect on a model (name invented)
inline void PlayEfp_unk(GimmickCommon* owner, atn::EfpFile* efp, atn::Model* model) {
    fn_80232B7C(owner, 0);
    lbl_8087F3C0->mB8 = 1;
    Color4f_unk color;
    atn::Vector3 rot;
    atn::Vector3 pos;
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
    lbl_8087F3C0->mB8 = 0;
}
// inferred: name invented
inline float LenSq_unk(atn::Vector3 v) {
    return fn_805F9920(&v);
}

namespace gmk {

GimmickCommon* SavePoint::fn_80435CE4(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) SavePoint(parent, id, arg);
    }
    return 0;
}

GimmickCommon* SavePoint::fn_80435DEC(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        SavePoint* gimmick = new (12, "", "", 0) SavePoint(parent, desc->mId + desc->m20, desc->mArg);
        if (gimmick != 0) {
            gimmick->m6C = desc->mPos;
            gimmick->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            gimmick->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
            gimmick->mDesc = desc;
        }
        return gimmick;
    }
    return 0;
}

SavePoint::~SavePoint() {
    if (lbl_8087F3C0 != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
}

int SavePoint::vf0C() {
    return fn_803EC91C() == 0;
}

void SavePoint::Start() {
    if (lbl_8087F0A8->m198 == 0) {
        vf2C();
        fn_803ECBD4(mModel.GetModel_unk(), 1);
        GimmickMsg_unk msg(1);
        if (m58 == 1) {
            msg.mType = 0;
        }
        vf40(&msg);
    }
}

void SavePoint::vf10() {
    if (lbl_8087F0A8->m198 != 0) {
        return;
    }
    int wasEnabled = mEnabled;
    mEnabled = 1;
    if (mDesc != 0 && mDesc->m7C == 0) {
        mEnabled = 0;
    }
    if (lbl_8087F430 != 0) {
        if (lbl_8087F430->m54E4 == 8) {
            mEnabled = 0;
        }
        if (LenSq_unk(lbl_8087F430->m74 - m6C) >= 90000.0f) {
            mEnabled = 0;
        }
        if (lbl_8087F098 != 0 && lbl_8087F098->m48 != 0) {
            mEnabled = 0;
        }
    }
    if (mEnabled == 0) {
        if (lbl_8087F3C0 != 0 && fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        return;
    }
    if (wasEnabled == 0) {
        int shown = mShown;
        GimmickMsg_unk msg(m54);
        vf40(&msg);
        mShown = shown;
    }
    if ((unsigned int)m54 <= 2 && lbl_8087F430 != 0 && mShown != 0) {
        if (fn_80375184(lbl_8087F430) != 0 || lbl_8087F430->m5694 != 0) {
            if (vf60() != 0) {
                GimmickMsg_unk msg(0);
                vf40(&msg);
                mShown = 1;
            }
        } else if (vf60() == 0) {
            GimmickMsg_unk msg(1);
            vf40(&msg);
            mShown = 1;
        }
    }
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player != 0) {
        float dist = LenSq_unk(player->mPos - m6C);
        if (m54 == 0) {
            if (dist < 64.0f && lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) != 0) {
                lbl_8087F430->fn_803750E4(0x145);
            }
        } else if (m54 >= 1) {
            float dist2 = LenSq_unk(player->mPos - m6C);
            if (m54 == 1) {
                if (dist2 < mRadius * mRadius) {
                    GimmickMsg_unk msg(2);
                    vf40(&msg);
                }
            } else if (m54 == 2) {
                if (dist2 >= mRadius * mRadius) {
                    mAlpha -= 0.022222223f;
                }
                if (mAlpha <= 0.0f) {
                    GimmickMsg_unk msg(1);
                    vf40(&msg);
                }
            }
            if (dist2 < 400.0f && lbl_8087F430 != 0 && fn_8036E098(lbl_8087F430) == this) {
                if (mSePlayed == 0) {
                    mSePlayed = 1;
                    fn_800C31F4("SE_UICOM_024", 0, -1, 1.0f);
                }
            } else if (mSePlayed != 0) {
                mSePlayed = 0;
            }
        }
    }
    fn_803ECBD4(mModel.GetModel_unk(), 1);
}

int SavePoint::vf60() {
    int ret = 0;
    if (m54 >= 1 && mEnabled != 0) {
        ret = 1;
    }
    return ret;
}

void SavePoint::vf14() {
    if (lbl_8087F0A8->m18 != 0 && lbl_8087F0A8->m198 == 0) {
        mModel.GetModel_unk()->fn_8008CD60();
    }
}

int SavePoint::vf1C() {
    if (mModel.GetModel_unk()->fn_8008B140() != 0) {
        return 1;
    }
    for (int i = 0; i < 3; i++) {
        if (mEfp[i].fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

void SavePoint::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    int count = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(mModel.GetModel_unk(), csv.fn_8005B9CC(), 0);
            fn_803EC7A0(mModel.GetModel_unk(), &csv);
        } else if (strcmp(field, "EFP") == 0 && count < 3) {
            mEfp[count++].fn_8023780C(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void SavePoint::vf24() {}

int SavePoint::vf40(GimmickMsg_unk* msg) {
    if (lbl_8087F0A8->m198 != 0) {
        return 0;
    }
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 0: {
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        atn::EfpFile* efp = &mEfp[msg->mType];
        if (lbl_8087F3C0 != 0) {
            PlayEfp_unk(this, efp, mModel.GetModel_unk());
        }
        mShown = 0;
        break;
    }
    case 1: {
        if (m54 == 0) {
            if (lbl_8087F3C0 != 0) {
                fn_80239DAC(lbl_8087F3C0, this, 0, 0);
            }
        } else if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        atn::EfpFile* efp = &mEfp[msg->mType];
        if (lbl_8087F3C0 != 0) {
            PlayEfp_unk(this, efp, mModel.GetModel_unk());
        }
        mShown = 1;
        mAlpha = 0.0f;
        break;
    }
    case 2: {
        if (m54 == 0) {
            if (lbl_8087F3C0 != 0) {
                fn_80239DAC(lbl_8087F3C0, this, 0, 0);
            }
        } else if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        atn::EfpFile* efp = &mEfp[msg->mType];
        if (lbl_8087F3C0 != 0) {
            PlayEfp_unk(this, efp, mModel.GetModel_unk());
        }
        mAlpha = 1.0f;
        break;
    }
    }
    return m54 = msg->mType;
}

void SavePoint::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int SavePoint::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() == 0 || flag != 0) {
        return 0;
    }
    int out;
    if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &out) != 0) {
        return 0;
    }
    if (LenSq_unk(m6C - *pos) < 64.0f) {
        if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x92) == 0) {
            fn_80370320(lbl_8087F430, 0x92, 1, 0);
        }
        return 1;
    }
    return 0;
}

void SavePoint::vf4C(CharaTask* chara) {
    if (lbl_8087F430 != 0) {
        fn_8036E92C(lbl_8087F430, 1);
        fn_800C31F4("SE_UICOM_049", 0, -1, 1.0f);
    }
}

void SavePoint::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
}

} // namespace gmk
