// inferred: file boundaries from the command name table (0x80752040, used by fn_803EF590 and the debug
// menu 0x803F11F8) and the string pool 0x80752084 (used from 0x803EF780 to 0x803F1310); see
// docs/splits-auto-C.md. The file holds the motion commands of the gimmicks (GimmickCommonB0_unk) and
// GimmickStateCommand.

#include "game/GimmickCommon.h"
#include "atn/SoundManager.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Memory.h"
#include "atn/ResLoader_unk.h"
#include "game/MotionCmdElemD_unk.h"
#include "macros.h"
#include "std/string_members.h"
#include "std/vector_string.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" int fn_802180A8(const char* name);         // inferred: motion id from its name
extern "C" const char* fn_8021823C(int id);           // inferred: motion name from its id
extern "C" float fn_800DC288(const char* s);          // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s);   // inferred: string hash
extern "C" bool fn_80216AFC(const char* id, int* type, int* map, int* point); // inferred: parses a map id
// inferred: field (lbl_8087F430) and its map information
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48; // inferred: map type
    int m4C; // inferred: area
    int m50; // inferred: room
};
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C; // inferred: camera shake
    int m970;
    float m974;
    float m978;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
// inferred: accessor (weak copy 0x80121F00 kept from another file)
extern "C" inline Field_unk* fn_80121F00() {
    return lbl_8087F430;
}
// inferred: accessors (weak copies, names keep the addresses; also used by gmk::Cannonball)
extern "C" inline int fn_803EFD9C(MapInfo_unk* info) {
    return info->m48;
}
extern "C" inline int fn_803EFDA4(MapInfo_unk* info) {
    return info->m4C;
}
extern "C" inline int fn_803EFDAC(MapInfo_unk* info) {
    return info->m50;
}
// inferred: effects
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8; // inferred: loop flag of the next effect
};
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int id); // inferred: sets the owner of the next effects
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, void* node,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops the effects
extern "C" int fn_800928B0(Model_unk* model, int hash, int arg); // inferred: node index by hash
extern "C" int fn_805A38F0(const char* name, int a, int b); // inferred: the sound is ready
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames);           // inferred: fades the sound in
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
extern "C" void fn_800CB6B0(atn::SoundRef* sound, int arg, float distance); // inferred: sets the distance
// inferred: frame time (lbl_8087EFA8)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: frame speed
};
extern "C" FrameTime_unk* lbl_8087EFA8;
// inferred: player manager (lbl_8087F8A0)
struct ShakePlayer_unk {
    unsigned char m000[0x528];
    atn::Vector3 m528; // inferred: camera position
};
struct ShakePlayerMgr_unk {
    unsigned char m00[0x48];
    ShakePlayer_unk* m48;
};
extern "C" ShakePlayerMgr_unk* lbl_8087F8A0;
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
// inferred: bullets
struct BulletMgr_unk;
struct HitInfo_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_800F8548(BulletMgr_unk* mgr);
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, int arg1, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
// inferred: debug menu
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

// inferred: iterator over a std::vector (weak copies 0x803EFDB4, 0x803F05F0-0x803F0688; name invented)
template <class T>
struct VecIter_unk {
    VecIter_unk(T* const& p) : mPtr(p) {}
    T* operator->() const { return mPtr; }
    T& operator*() const { return *mPtr; }
    T* operator++(int) {
        T* old = mPtr;
        mPtr++;
        return old;
    }
    bool operator!=(T* const& p) const { return mPtr != p; }
    T* mPtr;
};

// inferred: names of the motion commands
static const char* const sCommandNames[] = {
    "EFF_PLAY", "EFP_PLAY", "EFP_LOOP", "SE_PLAY", "SE_LOOP", "CAM_SHAKE", "EXPLODE", 0,
};

// inferred: index of a command name, -1 if unknown
extern "C" int fn_803EF590(const char* name) {
    const char* const* p = sCommandNames;
    int i = 0;
    while (*p != 0) {
        if (strcmp(name, *p) == 0) {
            return i;
        }
        p++;
        i++;
    }
    return -1;
}

GimmickCommonB0_unk::GimmickCommonB0_unk() : mMotion(-1), mEnabled(1), mSoundOn(1) {}

GimmickCommonB0_unk::~GimmickCommonB0_unk() {
    fn_803F10AC(0);
}

// inferred: reads the motion commands ("MOTCMD motion frame command resource x y z scale node", with
// "IF map"/"ELSE"/"ENDIF" blocks) and loads their resources
// NONMATCHING: 0% - the original is over the inlining budget and calls every inline function out of
// line (weak copies 0x803EFCA8-0x803F069C follow it); here they are all inlined. Statement for
// statement the code is the same (with a probe of ~16 extra statements it matches up to registers:
// this in r27, the counters in r29-r31), so the reconstructed MSL/atn inline functions are smaller than
// the originals. Because of that the string pool differs too (the "MotionCommand" literal of the
// resizes is dropped and "vector length error" moves), which costs fn_803F11F8 and fn_803F1310.
void GimmickCommonB0_unk::fn_803EF780(void* data, int size) {
    atn::CsvTab csv(data, size);
    std::vector<std::string> names;
    std::vector<GimmickMotionCmd_unk> cmds;
    int nEff = 0;
    int nEfp = 0;
    int nSe = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';') {
            continue;
        }
        if (strncmp(field, "MOTCMD", 6) == 0) {
            GimmickMotionCmd_unk cmd;
            cmd.mMotion = fn_802180A8(csv.fn_8005B9CC());
            cmd.mFrame = atoi(csv.fn_8005B9CC());
            cmd.mType = fn_803EF590(csv.fn_8005B9CC());
            if (cmd.mType == 0) {
                nEff++;
            } else if (cmd.mType == 1 || cmd.mType == 2) {
                nEfp++;
            } else if (cmd.mType == 3 || cmd.mType == 4 || cmd.mType == 6) {
                nSe++;
            }
            cmd.mIndex = cmds.size();
            names.push_back(std::string(csv.fn_8005B9CC()));
            cmd.mOffset.x = fn_800DC288(csv.fn_8005B9CC());
            cmd.mOffset.y = fn_800DC288(csv.fn_8005B9CC());
            cmd.mOffset.z = fn_800DC288(csv.fn_8005B9CC());
            cmd.mScale = fn_800DC288(csv.fn_8005B9CC());
            cmd.mNode = fn_800DC6B4(csv.fn_8005B9CC());
            cmds.push_back(cmd);
        } else if (strcmp(field, "IF") == 0) {
            int type;
            int map;
            int point;
            if (fn_80216AFC(csv.fn_8005B9CC(), &type, &map, &point) && fn_80121F00() != 0 &&
                fn_80373148(fn_80121F00()) != 0) {
                MapInfo_unk* info = fn_80373148(fn_80121F00());
                if (type != fn_803EFD9C(info) || map != fn_803EFDA4(info) || point != fn_803EFDAC(info)) {
                    while (csv.fn_8005BBF8() != 0) {
                        const char* s = csv.fn_8005B9CC();
                        if (*s == ';') {
                            continue;
                        }
                        if (strcmp(s, "ENDIF") == 0 || strcmp(s, "ELSE") == 0) {
                            break;
                        }
                    }
                }
            }
        } else if (strcmp(field, "ELSE") == 0) {
            while (csv.fn_8005BBF8() != 0) {
                const char* s = csv.fn_8005B9CC();
                if (*s == ';') {
                    continue;
                }
                if (strcmp(s, "ENDIF") == 0) {
                    break;
                }
            }
        }
    } while (csv.fn_8005BBF8() != 0);

    mCmds.Resize_unk(cmds.size(), 3, "MotionCommand");
    mEff.Resize_unk(nEff, 3, "MotionCommand");
    mEfp.Resize_unk(nEfp, 3, "MotionCommand");
    mSounds.Resize_unk(nSe, 3, "MotionCommand");
    mNames.Resize_unk(nSe, 3, "MotionCommand");
    int n = 0;
    int eff = 0;
    int efp = 0;
    int se = 0;
    for (VecIter_unk<GimmickMotionCmd_unk> it(cmds.begin()); it != cmds.end(); it++) {
        if (it->mType == 0) {
            mEff[eff].fn_80237654(names[it->mIndex].c_str());
            it->mIndex = eff++;
        } else if (it->mType == 1 || it->mType == 2) {
            mEfp[efp].fn_8023780C(names[it->mIndex].c_str());
            it->mIndex = efp++;
        } else if (it->mType == 3 || it->mType == 4 || it->mType == 6) {
            mNames[se] = names[it->mIndex].c_str();
            it->mIndex = se++;
        }
        mCmds[n++] = *it;
    }
}

int GimmickCommonB0_unk::fn_803F06B0() {
    for (int i = 0; i < (unsigned int)mEff.mSize; i++) {
        if (mEff[i].fn_802376D0() != 0) {
            return 1;
        }
    }
    for (int i = 0; i < (unsigned int)mEfp.mSize; i++) {
        if (mEfp[i].fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

// inferred: fires the commands of the current motion of the gimmick model
// NONMATCHING: 94.61% - stack slots of the int->float temporaries and of the vectors/colour of the
// effect calls, saved registers of motion/model/loop, and the early return after vf38 (bne + b)
void GimmickCommonB0_unk::fn_803F0764(GimmickCommon* gimmick) {
    if (gimmick == 0) {
        return;
    }
    if (gimmick->vf38() == 0) {
        return;
    }
    if (mEnabled == 0) {
        fn_803F10AC(1);
        return;
    }
    Model_unk* model = gimmick->vf38();
    int motion = model->mTracks[0].m00;
    float frame = model->mTracks[0].m08;
    float length = model->fn_80097D7C(0);
    float step = model->mTracks[0].m0C * lbl_8087EFA8->m3A4;
    unsigned char loop = model->mTracks[0].m18;
    if (step <= 0.0f) {
        return;
    }
    if (mMotion != motion) {
        fn_803F10AC(0);
        mMotion = motion;
    }
    for (int i = 0; i < (unsigned int)mCmds.mSize; i++) {
        GimmickMotionCmd_unk* cmd = &mCmds[i];
        if (cmd->mMotion != motion) {
            continue;
        }
        if (!((float)__fabs((float)cmd->mFrame - frame) < 1.1920929e-7f ||
              (frame < (float)cmd->mFrame && frame + step > (float)cmd->mFrame && (!loop || frame + step < length)) ||
              (frame >= 0.0f && frame - step < 0.0f &&
               (length + (frame - step) < (float)cmd->mFrame || (float)cmd->mFrame < frame)))) {
            continue;
        }
        switch (cmd->mType) {
        case 0: {
            MtxPtr node = model->GetNodeMtx_unk(fn_800928B0(model, cmd->mNode, 0));
            if (node != 0) {
                fn_80232B7C(this, motion);
                EffectMgr_unk* mgr = lbl_8087F3C0;
                fn_8023A680(mgr, &mEff[cmd->mIndex], -1, 5, node, &cmd->mOffset, 0, 0, 0, -1, 1, cmd->mScale);
            } else {
                atn::Vector3 pos = gimmick->m6C + cmd->mOffset;
                atn::Vector3 rot = gimmick->m78;
                fn_80232B7C(this, motion);
                EffectMgr_unk* mgr = lbl_8087F3C0;
                fn_8023A680(mgr, &mEff[cmd->mIndex], -1, 0, 0, &pos, &rot, 0, 0, -1, 1, cmd->mScale);
            }
            cmd->mDone = 1;
            break;
        }
        case 1:
        case 2:
            if (cmd->mType == 1 || cmd->mDone == 0) {
                atn::Vector3 rot(0.0f, 0.0f, 0.0f);
                if (cmd->mType == 2) {
                    lbl_8087F3C0->mB8 = 1;
                }
                fn_80232B7C(this, motion);
                fn_8023A8B4(lbl_8087F3C0, &mEfp[cmd->mIndex], model, 0, cmd->mOffset, rot,
                            Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, cmd->mScale);
                if (cmd->mType == 2) {
                    lbl_8087F3C0->mB8 = 0;
                }
                cmd->mDone = 1;
            }
            break;
        case 3:
        case 4:
            if (mSoundOn != 0 && lbl_8087EFE8 != 0 && (cmd->mType == 3 || cmd->mDone == 0)) {
                atn::Vector3 pos = cmd->mOffset;
                Mtx mtx;
                PSMTXRotRad(mtx, 'y', gimmick->m78.y);
                PSMTXMultVec(mtx, &pos, &pos);
                MtxPtr node = model->GetNodeMtx_unk(fn_800928B0(model, cmd->mNode, 0));
                if (node != 0) {
                    pos += atn::Vector3(node[0][3], node[1][3], node[2][3]);
                } else {
                    pos += gimmick->m6C;
                }
                if (fn_805A38F0(mNames[cmd->mIndex].mStr, 7, 0) != 0) {
                    lbl_8087EFE8->m34D0 = 7;
                    atn::SoundRef sound = fn_800C344C(mNames[cmd->mIndex].mStr, &pos, 0, -1, cmd->mScale);
                    lbl_8087EFE8->m34D0 = 0;
                    if (cmd->mType == 4) {
                        mSounds[cmd->mIndex] = sound;
                    }
                }
                cmd->mDone = 1;
            }
            break;
        case 5:
            if (lbl_8087F430 != 0 && lbl_8087F8A0 != 0) {
                atn::Vector3 pos = cmd->mOffset;
                Mtx mtx;
                PSMTXRotRad(mtx, 'y', gimmick->m78.y);
                PSMTXMultVec(mtx, &pos, &pos);
                MtxPtr node = model->GetNodeMtx_unk(fn_800928B0(model, cmd->mNode, 0));
                if (node != 0) {
                    pos += atn::Vector3(node[0][3], node[1][3], node[2][3]);
                } else {
                    pos += gimmick->m6C;
                }
                atn::Vector3 camera = lbl_8087F8A0->m48->m528;
                int frames = cmd->mOffset.z;
                atn::Vector3 d(pos.x - camera.x, pos.y - camera.y, pos.z - camera.z);
                float dist = PSVECMag(&d);
                float power;
                if (cmd->mScale > 0.0f) {
                    power = 1.0f - MIN(MAX(dist / cmd->mScale, 0.0f), 1.0f);
                } else {
                    power = 1.0f;
                }
                if (power > 0.0f) {
                    Field_unk* field = lbl_8087F430;
                    field->m96C %= 2;
                    field->m970 = frames;
                    field->m974 = cmd->mOffset.x * power;
                    field->m978 = cmd->mOffset.y * power;
                }
                cmd->mDone = 1;
            }
            break;
        case 6: {
            atn::Vector3 pos = cmd->mOffset;
            Mtx mtx;
            PSMTXRotRad(mtx, 'y', gimmick->m78.y);
            PSMTXMultVec(mtx, &pos, &pos);
            MtxPtr node = model->GetNodeMtx_unk(fn_800928B0(model, cmd->mNode, 0));
            if (node != 0) {
                pos += atn::Vector3(node[0][3], node[1][3], node[2][3]);
            } else {
                pos += gimmick->m6C;
            }
            int id = atoi(mNames[cmd->mIndex].mStr);
            BulletMgr_unk* mgr = lbl_8087F048;
            if (mgr != 0 && id > 0) {
                fn_800FAB80(mgr, 0, fn_80219E6C(id), fn_800F8548(mgr), &pos, &gimmick->m78, 0, 30, -1, -1,
                            cmd->mScale, 1.0f);
            }
            cmd->mDone = 1;
            break;
        }
        }
    }
}

void GimmickCommonB0_unk::fn_803F10AC(int arg) {
    if (arg != 0) {
        if ((mEff.mSize != 0 || mEfp.mSize != 0) && lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, mMotion, 0);
        }
        for (int i = 0; i < (unsigned int)mSounds.mSize; i++) {
            fn_800CB5C8(&mSounds[i], 0, 0);
        }
    } else {
        if ((mEff.mSize != 0 || mEfp.mSize != 0) && lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, mMotion, 1);
        }
        for (int i = 0; i < (unsigned int)mSounds.mSize; i++) {
            fn_800CB5C8(&mSounds[i], 15, 0);
        }
    }
    for (int i = 0; i < (unsigned int)mCmds.mSize; i++) {
        mCmds[i].mDone = 0;
    }
    mMotion = -1;
}

// inferred: debug menu of the commands
// NONMATCHING: 99.57% - only the offsets of "Motcmd".."Scale" in the string pool (see fn_803EF780)
void GimmickCommonB0_unk::fn_803F11F8(void* menu) {
    void* group = fn_8008937C(menu, "Motcmd");
    for (int i = 0; i < (unsigned int)mCmds.mSize; i++) {
        GimmickMotionCmd_unk* cmd = &mCmds[i];
        char name[0x100];
        // inferred: the pool has a second zero after "%s %s" (offset 0x44), as if the literal ended in \0
        sprintf(name, "%s %s\0", fn_8021823C(cmd->mMotion), sCommandNames[cmd->mType]);
        void* sub = fn_8008937C(group, name);
        fn_800874C8(sub, "Frame", &cmd->mFrame, 0, 9999, 1, 0, 0);
        fn_80087E9C(sub, "Offset", &cmd->mOffset, 0, 0, -9999.0f, 9999.0f, 1.0f);
        fn_8008771C(sub, "Scale", &cmd->mScale, 0, 0, 0.0f, 9999.0f, 0.5f);
    }
}

// inferred: creates the state command task of a gimmick
// NONMATCHING: 99.78% - only the offset of "" in the string pool (see fn_803EF780)
extern "C" GimmickStateCommand* fn_803F1310(GimmickCommon* gimmick) {
    if (gimmick != 0) {
        return new (0xC, "", "", 0) GimmickStateCommand(gimmick);
    }
    return 0;
}

inline GimmickStateCommand::GimmickStateCommand(GimmickCommon* gimmick)
    : atn::Task(gimmick), mGimmick(gimmick), mState(0), m50(1), m54(1), m58(0) {}

GimmickStateCommand::~GimmickStateCommand() {
    fn_803F1A38(0);
}

// inferred: starts the sounds of the gimmick sound table when the state changes
// NONMATCHING: 94.46% - register allocation of the table loop (the original keeps the zero of the new
// entry and the index in separate registers, one more saved register)
void GimmickStateCommand::vf10() {
    if (m58 > 0) {
        m58--;
        mState = mGimmick->m54;
    } else {
        int prev = mState;
        int state = mGimmick->m54;
        if (prev != state) {
            fn_803F1A38(0);
            GimmickManager* mgr = lbl_8087F4A0;
            for (int i = 0; i < mgr->mSeTables.mSize; i++) {
                const gmk::SeTable* table = &mgr->mSeTables.mData[i];
                if (table->mId == mGimmick->mId && table->m08 == state && (table->m04 == 0 || table->m04 == prev)) {
                    if (mEntries.size() < 16) {
                        Entry_unk entry;
                        entry.mTable = table;
                        mEntries.push_back(entry);
                    }
                }
            }
            mState = state;
        }
    }
    if (m50 == 0) {
        fn_803F1A38(1);
        return;
    }
    for (Entry_unk* it = &mEntries.mData[0]; it != &mEntries.mData[mEntries.mSize]; it++) {
        if (it->mDone != 0) {
            continue;
        }
        const gmk::SeTable* table = it->mTable;
        if (table != 0 && table->m08 != mState) {
            continue;
        }
        if (m54 != 0 && lbl_8087EFE8 != 0 && table != 0 && table->m0C <= it->mFrame) {
            atn::SoundRef sound;
            if (fn_805A38F0(table->mName.mStr, 7, 0) != 0) {
                Entry_unk* end = &mEntries.mData[mEntries.mSize];
                for (Entry_unk* o = &mEntries.mData[0]; o != end; o++) {
                    if (it != o && (o->mTable->mFlags & 0x20) == 0x20) {
                        sound = o->mSound;
                        o->mSound.Reset();
                        break;
                    }
                }
                if (sound.mPtr == 0) {
                    for (Entry_unk* o = &mEntries.mData[0]; o != &mEntries.mData[mEntries.mSize]; o++) {
                        if ((o->mTable->mFlags & 2) == 2) {
                            fn_800CB5C8(&o->mSound, 15, 0);
                        }
                    }
                    lbl_8087EFE8->m34D0 = 7;
                    if ((table->mFlags & 1) != 1) {
                        atn::Vector3 pos = table->mPos;
                        Mtx mtx;
                        PSMTXRotRad(mtx, 'y', mGimmick->m78.y);
                        PSMTXMultVec(mtx, &pos, &pos);
                        MtxPtr node = 0;
                        if (mGimmick->vf38() != 0) {
                            node = mGimmick->vf38()->GetNodeMtx_unk(fn_800928B0(mGimmick->vf38(), table->mHash, 0));
                        }
                        if (node != 0) {
                            pos += atn::Vector3(node[0][3], node[1][3], node[2][3]);
                        } else {
                            pos += mGimmick->m6C;
                        }
                        sound = fn_800C344C(table->mName.mStr, &pos, 0, -1, table->mVolume);
                        if (table->m48 != 0) {
                            fn_800CB6B0(&sound, 1, (unsigned int)table->m48);
                        }
                    } else {
                        sound = fn_800C31F4(table->mName.mStr, 0, -1, table->mVolume);
                    }
                }
                if ((table->mFlags & 4) == 4) {
                    fn_800CB518(&sound, 30);
                }
                if ((table->mFlags & 8) == 8) {
                    fn_800CB688(&sound, 0xF0, 2.0f);
                }
                if ((table->mFlags & 0x10) == 0x10) {
                    fn_800CB688(&sound, 0xF0, 0.5f);
                }
                lbl_8087EFE8->m34D0 = 0;
            }
            if ((table->mFlags & 2) == 2) {
                it->mSound = sound;
            }
            it->mDone = 1;
        }
        it->mFrame++;
    }
}

// inferred: removes element i of a fixed array (as in game/MapManager.cpp; name invented)
template <class T, int N>
inline void Erase_unk(atn::FixedArray_unk<T, N>& a, int i) {
    for (; i < a.mSize - 1; i++) {
        a.mData[i] = a.mData[i + 1];
    }
    a.mSize = a.mSize - 1;
}

void GimmickStateCommand::fn_803F1A38(int arg) {
    if (arg != 0) {
        for (Entry_unk* it = &mEntries.mData[0]; it != &mEntries.mData[mEntries.mSize]; it++) {
            fn_800CB5C8(&it->mSound, 0, 0);
        }
        mEntries.mSize = 0;
    } else {
        Entry_unk* it = &mEntries.mData[0];
        while (it != &mEntries.mData[mEntries.mSize]) {
            if ((it->mTable->mFlags & 0x20) != 0x20 || it->mSound.mPtr == 0) {
                fn_800CB5C8(&it->mSound, 15, 0);
                Erase_unk(mEntries, it - &mEntries.mData[0]);
                for (Entry_unk* p = it; p != &mEntries.mData[mEntries.mSize]; p++) {
                    if (p->mSound.mPtr != 0) {
                        p->mSound.mPtr->mRefCount--;
                    }
                }
            } else {
                it++;
            }
        }
    }
    mState = 0;
}
