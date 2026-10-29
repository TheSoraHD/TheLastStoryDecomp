#include "game/CharaTask.h"
#include "atn/SoundManager.h"
#include "game/MotionCmdData_unk.h"
#include "game/ColliAttrManager.h"
#include "db/MotionCommandCommon.h"
#include "atn/Csv.h"
#include "atn/LH_Motion.h"
#include "std/vector.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s);        // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" unsigned int fn_800DC12C(const char* s); // inferred: string to flags
extern "C" const char* fn_80218218(int motion);     // inferred: motion name

// inferred: effect manager (names invented)
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
    unsigned char mBC[0xC4 - 0xBC];
    int mC4;
    int mC8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, const void* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
// inferred: color of the effect (four floats)
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, const void* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB6B0(atn::SoundRef* sound, int arg, float value);
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound

// inferred: game timer (names invented)
struct Timer_unk {
    float GetTime_unk() const { return m3A4; } // inferred: accessor (name invented)
    unsigned char m00[0x3A4];
    float m3A4; // inferred: frame time
};
extern "C" Timer_unk* lbl_8087EFA8;
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48; // inferred: kind of map (1 town, 2-4 dungeon)
    int m4C; // inferred: area
};
// inferred: camera shake request of the field (names invented)
struct Field_unk {
    void SetShake_unk(int type, int frames, float power, float speed) {
        m96C = type;
        m970 = frames;
        m974 = power;
        m978 = speed;
    }
    unsigned char m00[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);

// inferred: names of the command types and the areas (rodata tables 0x807370E8 and 0x80737118)
static const char* const sTypeNames[8] = {"EFF_ATR", "EFF", "EFP", "EFP_LOOP", "SE_ATR", "SE", "SE_LOOP", "SHAKE"};
static const char* const sAreaNames[3] = {"ALL", "TOWN", "DUNGEON"};

// inferred: index of `name` in `table`, -1 if missing
extern "C" int fn_8011FF78(const char* name, const char* const* table, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(name, table[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// NONMATCHING: 99.86% - registers: the size parameter and the line count take r24/r27 instead of
// r25/r24 (tried locals for the text/size, signed/unsigned count, loop forms). Like the original it
// is over the inlining budget and calls the inlines out of line (weak copies 0x8012044C-0x80120F4C)
extern "C" void fn_8011FFF8(atn::MotionCommandData* data, void* buffer, int size) {
    const char* text = (const char*)buffer;
    atn::Csv csv(text, size);
    int count = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() == ';') {
            continue;
        }
        if (*csv.fn_8005B9CC() == '\0') {
            continue;
        }
        count++;
    }
    data->mEntries.Resize_unk(count, 0, 0);
    std::vector<std::string> effects;
    std::vector<std::string> efps;
    std::vector<std::string> names;
    csv.SetData(text, size);
    unsigned int index = 0;
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';') {
            continue;
        }
        int byName = atoi(field);
        const char* motion = csv.fn_8005B9CC();
        if (*motion == '\0') {
            continue;
        }
        atn::MotionCommandData::Command& entry = data->mEntries[index++];
        entry.mByName = byName;
        entry.m04 = fn_800DC6B4(motion);
        entry.mFrame = atoi(csv.fn_8005B9CC());
        entry.mType = fn_8011FF78(csv.fn_8005B9CC(), sTypeNames, 8);
        entry.mArea = fn_8011FF78(csv.fn_8005B9CC(), sAreaNames, 3);
        const char* res = csv.fn_8005B9CC();
        switch (entry.mType) {
        case 1:
            entry.mIndex = effects.size();
            effects.push_back(std::string(res));
            break;
        case 2:
        case 3:
            entry.mIndex = efps.size();
            efps.push_back(std::string(res));
            break;
        case 0:
        case 4:
        case 5:
        case 6:
            entry.mIndex = names.size();
            names.push_back(std::string(res));
            break;
        case 7:
            entry.mIndex = 0;
            break;
        default:
            entry.mIndex = -1;
            break;
        }
        entry.mOffset.x = fn_800DC288(csv.fn_8005B9CC());
        entry.mOffset.y = fn_800DC288(csv.fn_8005B9CC());
        entry.mOffset.z = fn_800DC288(csv.fn_8005B9CC());
        entry.mScale = fn_800DC288(csv.fn_8005B9CC());
        entry.mUseNode = atoi(csv.fn_8005B9CC());
        entry.mNode = fn_800DC6B4(csv.fn_8005B9CC());
        entry.mFlags = fn_800DC12C(csv.fn_8005B9CC());
    }
    if (!effects.empty()) {
        data->mEffects.Resize_unk(effects.size(), 0, 0);
        for (unsigned int i = 0; i < effects.size(); i++) {
            data->mEffects[i].fn_80237654(effects[i].c_str());
        }
    }
    if (!efps.empty()) {
        data->mEfps.Resize_unk(efps.size(), 0, 0);
        for (unsigned int i = 0; i < efps.size(); i++) {
            data->mEfps[i].fn_8023780C(efps[i].c_str());
        }
    }
    if (!names.empty()) {
        data->mNames.Resize_unk(names.size(), 0, 0);
        for (unsigned int i = 0; i < names.size(); i++) {
            data->mNames[i] = names[i].c_str();
        }
    }
}

MotionCommand_unk::MotionCommand_unk()
    : mChara(0), m0C(0), m10(1), m11(1), m12(1), m20(0), m24(0), m2C(0), m30(0) {}

MotionCommand_unk::~MotionCommand_unk() {}

void MotionCommand_unk::fn_801210B4(const char* path) {
    mLoader.vf0C(path);
}

int MotionCommand_unk::fn_801210C4() {
    if (mLoader.fn_80473F50() == 0) {
        m20 = (atn::MotionCommandData*)mLoader.fn_80476CE4();
        return 0;
    }
    return 1;
}

void MotionCommand_unk::fn_80121114(CharaTask* chara) {
    mChara = chara;
}

// inferred: accessors of the motion tracks (weak copies called out of line from CharaTask)
inline float atn::Model::GetMotionFrame(int track) const {
    return mTracks[track].m08;
}
inline int atn::Model::fn_80139F2C(int track) {
    return mTracks[track].m00;
}
inline int atn::Model::fn_8013C3DC() {
    return m34C;
}
inline float atn::Model::fn_8013C3E4(int track) {
    return mTracks[track].m20;
}

// inferred: hashes of node names (.sbss 0x8087F078-0x8087F080, set by the static initializer)
static unsigned int sLeftFoot = fn_800DC6B4("leftfoot");
static unsigned int sRightFoot = fn_800DC6B4("rightfoot");
static unsigned int sParts[1] = {fn_800DC6B4("$weapon")};

// inferred: 0 for the weapon node, -1 for the body (name invented)
inline int GetPart_unk(unsigned int node) {
    for (int i = 0; i < 1; i++) {
        if (node == sParts[i]) {
            return i;
        }
    }
    return -1;
}

void MotionCommand_unk::fn_8012111C() {
    if (m10 == 0) {
        return;
    }
    atn::Model* model = mChara->GetModel_unk();
    int count = model->fn_8013C3DC();
    m08 = 0;
    if (count > 1) {
        float max = 0.0f;
        for (int i = 0; i < count; i++) {
            if (model->fn_8013C3E4(i) > max) {
                max = model->fn_8013C3E4(i);
                m08 = i;
            }
        }
    }
    int motion = model->fn_80139F2C(m08);
    if (motion != m04) {
        if (m24 != 0) {
            if (lbl_8087F3C0 != 0) {
                fn_80239DAC(lbl_8087F3C0, m24, m04, 1);
            }
            m24 = 0;
        }
        if (m28.mPtr != 0) {
            fn_800CB5C8(&m28, 15, 0);
            m2C = 0;
        }
        m04 = motion;
    }
    if (mChara->mType == 0 && lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34C8 = 0;
    }
    if (model->mTracks[m08].m04 != 0) {
        unsigned int res = ((atn::LH_Motion*)model->mTracks[m08].m04)->fn_804768B4();
        if (m30 != res || m04 == 0x58 || m12 != 0) {
            int found = 0;
            int ran = fn_8012133C(m20, &found);
            if (db::MotionCommandCommon::sInstance != 0 && ran == 0) {
                fn_8012133C((atn::MotionCommandData*)db::MotionCommandCommon::sInstance->mData, &found);
            }
            m30 = res;
            m12 = (found != 0);
        }
    }
    if (mChara->mType == 0 && lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34C8 = 1;
    }
    if (m28.mPtr != 0 && m2C != 0) {
        float z = m2C[2][3];
        float y = m2C[1][3];
        float x = m2C[0][3];
        atn::Vector3 pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        fn_800CB6E4(&m28, &pos);
    }
}

// NONMATCHING: 93.92% - the string pool starts with "vector length error" in the original (the
// generic push_back of include/std/vector.h is instantiated at the end of the file here), so the
// SE_ATTR literals use other offsets; also the order of the stack slots of the vector temporaries,
// the scheduling of the arguments of fn_8023A680/fn_8023A8B4, fabs into a temporary and a few
// register choices
int MotionCommand_unk::fn_8012133C(atn::MotionCommandData* data, int* found) {
    if (data == 0 || data->mEntries.mSize == 0) {
        return 0;
    }
    atn::Model* model = mChara->GetModel_unk();
    if (model->mTracks[m08].m04 == 0) {
        return 0;
    }
    unsigned int res = ((atn::LH_Motion*)model->mTracks[m08].m04)->fn_804768B4();
    unsigned int hash = fn_800DC6B4(fn_80218218(m04));
    float frame = model->GetMotionFrame(m08);
    float length = model->fn_80097D7C(m08);
    float step = model->GetMotionSpeed(m08) * lbl_8087EFA8->GetTime_unk();
    unsigned char loop = model->mTracks[m08].m18;
    if (m04 == 0x58) {
        if (step < 0.0f) {
            step = __fabs(step);
            frame -= step;
        }
        if (step <= 0.0001f) {
            return 0;
        }
    } else if (step <= 0.0f) {
        return 0;
    }
    int area = 0;
    int ruins = 0;
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        MapInfo_unk* info = fn_80373148(lbl_8087F430);
        switch (info->m48) {
        case 1:
            area = 1;
            break;
        case 2:
        case 3:
        case 4:
            area = 2;
            if (info->m4C == 0x12) {
                ruins = 1;
            }
            break;
        }
    }
    int ran = 0;
    for (unsigned int i = 0; i < data->mEntries.mSize; i++) {
        atn::MotionCommandData::Command& entry = data->mEntries[i];
        if (entry.mByName == 0) {
            if (res != entry.m04) {
                continue;
            }
        } else if (hash != entry.m04) {
            continue;
        }
        *found = 1;
        if (entry.mArea != 0 && area != 0 && entry.mArea != area) {
            continue;
        }
        if (!(atn::Abs_unk(frame - entry.mFrame) < 1.1920929e-7f)) {
            if (frame < entry.mFrame && frame + step > entry.mFrame && (!loop || frame + step < length)) {
            } else if (frame >= 0.0f && frame - step < 0.0f &&
                       (length + (frame - step) < entry.mFrame || entry.mFrame < frame)) {
            } else {
                continue;
            }
        }
        switch (entry.mType) {
        case 0:
            ColliAttrManager::sInstance->fn_8018412C((int)data->mNames[entry.mIndex].mStr, mChara, entry.mUseNode,
                                                     entry.mNode, &entry.mOffset);
            ran = 1;
            break;
        case 1:
            fn_80232B7C(this, m04);
            if (entry.mUseNode != 0) {
                MtxPtr mtx = model->GetNodeMtx_unk(model->fn_800928B0(entry.mNode, 0));
                if (mtx != 0) {
                    fn_8023A680(lbl_8087F3C0, &data->mEffects[entry.mIndex], -1, 5, mtx, &entry.mOffset, 0, 0, 0,
                                -1, 1, entry.mScale);
                }
            } else {
                atn::Vector3 pos = mChara->mPos + entry.mOffset;
                atn::Vector3 rot = mChara->mRot;
                fn_8023A680(lbl_8087F3C0, &data->mEffects[entry.mIndex], -1, 0, 0, &pos, &rot, 0, 0, -1, 1,
                            entry.mScale);
            }
            ran = 1;
            break;
        case 2:
        case 3:
            if (entry.mType == 3 && m24 != 0) {
                break;
            }
            if (entry.mType == 3) {
                lbl_8087F3C0->mB8 = 2;
            }
            if ((entry.mFlags & 1) == 1) {
                lbl_8087F3C0->mC4 = 1;
            } else if ((entry.mFlags & 2) == 2) {
                lbl_8087F3C0->mC4 = 1;
                lbl_8087F3C0->mC8 = 1;
            }
            fn_80232B7C(&data->mEfps[entry.mIndex], m04);
            if (entry.mUseNode != 0) {
                atn::Vector3 rot(0.0f, 0.0f, 0.0f);
                int part = GetPart_unk(entry.mNode);
                const void* target;
                if (part == 0) {
                    target = (mChara->m648 != 0) ? mChara->m648->m010 : 0;
                } else {
                    target = &mChara->mMotion;
                }
                if (target != 0) {
                    fn_8023A8B4(lbl_8087F3C0, &data->mEfps[entry.mIndex], target, 0, entry.mOffset, rot,
                                Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, entry.mScale);
                }
            } else {
                Mtx mtx;
                PSMTXRotRad(mtx, 'y', mChara->mRot.y);
                atn::Vector3 ofs = entry.mOffset;
                PSMTXMultVec(mtx, &ofs, &ofs);
                atn::Vector3 pos = mChara->mPos + ofs;
                atn::Vector3 rot = mChara->mRot;
                fn_8023A8B4(lbl_8087F3C0, &data->mEfps[entry.mIndex], 0, 0, pos, rot,
                            Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, entry.mScale);
            }
            if ((entry.mFlags & 1) == 1) {
                lbl_8087F3C0->mC4 = 0;
            } else if ((entry.mFlags & 2) == 2) {
                lbl_8087F3C0->mC4 = 0;
                lbl_8087F3C0->mC8 = 0;
            }
            if (entry.mType == 3) {
                lbl_8087F3C0->mB8 = 0;
                m24 = &data->mEfps[entry.mIndex];
            }
            ran = 1;
            break;
        case 4:
            if (m11 != 0) {
                float volume = 1.0f;
                if (mChara->m55C != 8) {
                    bool silent = mChara->mType == 1 || mChara->mType == 4;
                    if (silent) {
                        volume = 0.0f;
                    } else if (mChara->mType == 3 || (mChara->mType == 2 && mChara->mFlag22 != 1)) {
                        volume = 0.7f;
                    }
                }
                volume *= entry.mScale;
                if (volume > 0.1f && step <= 5.0f) {
                    int side = 1;
                    if (entry.mNode == sRightFoot) {
                        side = 2;
                    }
                    ColliAttrManager::sInstance->fn_80184548((int)data->mNames[entry.mIndex].mStr, mChara,
                                                             entry.mUseNode, entry.mNode, &entry.mOffset, side,
                                                             volume);
                }
            }
            ran = 1;
            break;
        case 5:
        case 6: {
            if (m11 == 0) {
                break;
            }
            if (entry.mType == 6 && m28.mPtr != 0) {
                break;
            }
            MtxPtr node = 0;
            atn::SoundRef sound;
            if (step <= 5.0f) {
                const char* name = data->mNames[entry.mIndex].mStr;
                if (ruins != 0 && m04 >= 0x55) {
                    if (m04 <= 0x5A) {
                    if (strncmp(name, "SE_ATTR100_L", 12) == 0) {
                        name = "SE_ATTR102_L";
                    } else if (strncmp(name, "SE_ATTR100_R", 12) == 0) {
                        name = "SE_ATTR102_R";
                    }
                    }
                }
                if (entry.mUseNode != 0) {
                    node = model->GetNodeMtx_unk(model->fn_800928B0(entry.mNode, 0));
                    if (node != 0) {
                        atn::Vector3 pos(node[0][3], node[1][3], node[2][3]);
                        sound = fn_800C344C(name, &pos, 0, -1, entry.mScale);
                    }
                } else {
                    Mtx mtx;
                    PSMTXRotRad(mtx, 'y', mChara->mRot.y);
                    atn::Vector3 ofs = entry.mOffset;
                    PSMTXMultVec(mtx, &ofs, &ofs);
                    atn::Vector3 pos = mChara->mPos + ofs;
                    sound = fn_800C344C(name, &pos, 0, -1, entry.mScale);
                }
            }
            if (entry.mFlags != 0) {
                fn_800CB6B0(&sound, 1, entry.mFlags);
            }
            if (entry.mType == 6) {
                m28 = sound;
                m2C = node;
            }
            ran = 1;
            break;
        }
        case 7: {
            Field_unk* field = lbl_8087F430;
            if (field != 0) {
                field->SetShake_unk(field->m96C % 2, entry.mOffset.z, entry.mOffset.x, entry.mOffset.y);
            }
            ran = 1;
            break;
        }
        }
    }
    return ran;
}

void MotionCommand_unk::fn_80121E14(int arg) {
    if (m24 != 0) {
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, m24, m04, arg == 0);
        }
        m24 = 0;
    }
    if (m28.mPtr != 0) {
        fn_800CB5C8(&m28, arg ? 0 : 15, 0);
        m2C = 0;
    }
}
