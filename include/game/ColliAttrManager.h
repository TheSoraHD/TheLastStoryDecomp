#ifndef GAME_COLLIATTRMANAGER_H
#define GAME_COLLIATTRMANAGER_H

#include "atn/Array_unk.h"
#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "atn/Task.h"
#include "atn/static_string.h"
#include "math/Vec.h"

class CharaTask;
struct CollResult_unk;

// inferred: effects and sounds of the collision attributes of the ground (footsteps, landing...).
// Singleton task; each entry is loaded from the attribute table (fn_8020B1E0).
class ColliAttrManager : public atn::Task {
public:
    // inferred: data of one attribute (0x70 bytes; names invented)
    struct Entry_unk {
        Entry_unk() : mId(0) {}
        int mId;                    // 0x00: attribute id
        atn::EffFile mRes[1];      // 0x04: effect
        atn::static_string<32> mSe[3];  // 0x10: sounds
    };

    ColliAttrManager(atn::Task* parent) : atn::Task(parent), mEnabled(1), mLoading(0), mForceAttr(-1) {}
    virtual ~ColliAttrManager();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static ColliAttrManager* fn_801839FC(atn::Task* parent);
    void fn_80183D00(int id, int arg1, int arg2); // inferred: loads the attributes of a map (-1: all)
    void fn_80183FFC(int index, int id);
    atn::EffFile* fn_801840D8(int id, int index);
    atn::static_string<32>* FindSe_unk(int id, int index);
    void PlayEffect_unk(atn::EffFile* res, const atn::Vector3* pos, const atn::Vector3* rot,
                        const struct ColorF_unk& color);
    void fn_8018412C(int kind, CharaTask* chara, int useNode, int node, const atn::Vector3* ofs);
    void fn_80184188(int index, CharaTask* chara, int useNode, int node, const atn::Vector3* ofs);
    void fn_80184548(int kind, CharaTask* chara, int useNode, int node, const atn::Vector3* ofs, int side,
                     float volume);
    void fn_801847C0(CharaTask* chara);
    int fn_80184864(CollResult_unk* result, CharaTask* chara, int useNode, int node);

    // inferred: releases the resources of the entries
    void Release_unk() {
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            mEntries.mData[i].mRes->Destroy();
        }
        mEntries.Free_unk();
    }

    int mEnabled;                        // 0x48
    int mLoading;                        // 0x4C
    atn::SimpleArray<Entry_unk> mEntries; // 0x50
    int mForceAttr;                      // 0x58: inferred: attribute used instead of the ground one
    atn::EfpFile mEfp;             // 0x5C

    static ColliAttrManager* sInstance;
};

#endif
