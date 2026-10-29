#include "game/EquipItemData.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/function.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" float fn_800DC288(const char* s);         // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s);  // inferred: string hash
extern "C" int fn_800DC12C(const char* s);           // inferred: string to int
extern "C" std::string fn_8006AC08(const std::string& path); // inferred: extension of a path
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* name);        // inferred: motion id
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: adds a motion
extern "C" void fn_80097D9C(atn::Model* model, atn::Model* source);  // inferred: shares the motions
extern "C" int fn_800928B0(atn::Model* model, int hash, int arg);   // inferred: node index by hash
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" float PSVECMag(const atn::Vector3* v);

// inferred: node edit read from a CSV line (0x2C bytes)
struct NodeEdit_unk {
    unsigned char m00[0x2C];
};
extern "C" void fn_80089EE4(NodeEdit_unk* edit, atn::CsvTab* csv);
extern "C" void fn_80093F58(atn::Model* model, NodeEdit_unk* edit);

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

inline void __Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

EquipItemData::EquipItemData(atn::CsvTab* csv)
    : mModel(0x505, 0x20), mTarget(0), mNode(-1), mScale(1.0f, 1.0f, 1.0f) {
    const char* path = csv->fn_8005B9CC();
    bool chr = fn_8006AC08(std::string(path)) == "chr";
    if (chr) {
        mLoader.vf0C(path);
        mState = 0;
    } else {
        fn_8008AD4C(&mModel, path, 0);
        mNode = fn_800DC6B4(csv->fn_8005B9CC());
        if (strcmp(csv->fn_8005B9CC(), "") != 0) {
            mScale.x = fn_800DC288(csv->mField);
            mScale.y = fn_800DC288(csv->fn_8005B9CC());
            mScale.z = fn_800DC288(csv->fn_8005B9CC());
        }
        if (strcmp(csv->fn_8005B9CC(), "") != 0) {
            mFlags.mFlag = fn_800DC12C(csv->mField) != 0;
        }
        mState = 1;
    }
}

int EquipItemData::fn_8036354C() {
    switch (mState) {
    case 0:
        if (mLoader.fn_80473F50() != 0) {
            break;
        }
        fn_803635E8();
        mState = 1;
    case 1:
        if (mModel.fn_8008B140() != 0) {
            break;
        }
        fn_803637D4();
        mLoader.fn_80473F88();
        mState = 2;
    }
    return mState != 2;
}

void EquipItemData::fn_803635E8() {
    atn::CsvTab csv((const char*)mLoader.fn_80470580(), mLoader.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            char path[0x100];
            char texture[0x100];
            strcpy(path, csv.fn_8005B9CC());
            strcpy(texture, csv.fn_8005B9CC());
            if (strcmp(texture, "") == 0) {
                fn_8008AD4C(&mModel, path, 0);
            } else {
                fn_8008AD4C(&mModel, path, texture);
            }
        } else if (strcmp(field, "COPY_MOTION") == 0) {
            mFlags.mCopyMotion = 1;
            mModel.mFlags &= ~1;
        } else if (strcmp(field, "MOTION") == 0) {
            if ((mModel.mFlags & 1) != 1) {
                char path[0x100];
                strcpy(path, csv.fn_8005B9CC());
                fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

void EquipItemData::fn_803637D4() {
    atn::CsvTab csv((const char*)mLoader.fn_80470580(), mLoader.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "SCALE") == 0) {
            mScale.x = fn_800DC288(csv.fn_8005B9CC());
            mScale.y = fn_800DC288(csv.fn_8005B9CC());
            mScale.z = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "CONSTRAIN_NODE") == 0) {
            mNode = fn_800DC6B4(csv.fn_8005B9CC());
        } else if (strcmp(field, "NODE_EDIT") == 0) {
            NodeEdit_unk edit;
            fn_80089EE4(&edit, &csv);
            fn_80093F58(&mModel, &edit);
        }
    } while (csv.fn_8005BBF8() != 0);
}

void EquipItemData::fn_80363960(atn::Model* target) {
    mTarget = target;
    mNode = fn_800928B0(target, mNode, 0);
}

void EquipItemData::vf08() {
    if (mNode < 0) {
        return;
    }
    atn::Matrix3x4 mtx;
    __MtxCopy(mtx.m, mTarget->GetNodeMtx_unk(mNode));
    Mtx scale;
    PSMTXScale(scale, mScale.x, mScale.y, mScale.z);
    Mtx t;
    PSMTXConcat(mtx.m, scale, t);
    __MtxCopy(mtx.m, t);
    __MtxCopy(mModel.mMtx, mtx.m);
    atn::Vector3 ax;
    atn::Vector3 ay;
    atn::Vector3 az;
    __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
    float sz = PSVECMag(&az);
    __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
    float sy = PSVECMag(&ay);
    __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
    float sx = PSVECMag(&ax);
    atn::Vector3 s(sx, sy, sz);
    mModel.m54 = __MAX_unk(s.x, __MAX_unk(s.y, s.z));
    if (mFlags.mCopyMotion) {
        fn_80097D9C(&mModel, mTarget);
        atn::Model::Node_unk* nodes = mModel.m220;
        nodes[mModel.GetNodeNoByName("head", 0)].m00 = 0;
    }
    mModel.CalcMatrixStack(atn::ModelNodeCallback_unk());
}

void EquipItemData::vf0C() {
    if (mNode >= 0) {
        mModel.fn_8008CD60();
    }
}
