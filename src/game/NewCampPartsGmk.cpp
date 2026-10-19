// NOTE: the split of this unit is shorter than the source file: its string pool (0x8075B510),
// the name table (0x8075B32C), "menu_back" (0x8075B320), the identity floats (0x808877E4/E8) and
// the functions from 0x80512C90 (which use mInfo) lie in the auto units that follow. Until the
// split is extended, the references to them cannot match by name.
#include "game/NewCampPartsBase.h"
#include "atn/EfpHandle_unk.h"
#include "math/Vec.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

// inferred: model of the scene; the matrix is read through a base at offset 0 (null-checked cast)
struct NewCampModelBase_unk {
    unsigned char m00[0x30];
    atn::Matrix3x4 mMtx; // 0x30
};
struct NewCampModel_unk : public NewCampModelBase_unk {
    unsigned char m60[0x8C - 0x60];
    unsigned int mHash; // 0x8C: inferred: compared with fn_800DC6B4 of a name
};
// inferred: the null-checked cast of 0x80512BE0 (name invented)
inline NewCampModelBase_unk* GetBase_unk(NewCampModel_unk* model) {
    return model != 0 ? static_cast<NewCampModelBase_unk*>(model) : 0;
}
// inferred: node of the scene list (type 4 nodes hold models)
struct NewCampNode_unk {
    unsigned char m00[0x48];
    int mType;               // 0x48
    NewCampNode_unk* mNext;  // 0x4C
    unsigned char m50[0x5C - 0x50];
    char mName[0x344 - 0x5C]; // 0x5C
    int mModelNum;           // 0x344
    NewCampModel_unk** mModels; // 0x348
};
struct NewCampScene_unk {
    unsigned char m00[0x70];
    NewCampNode_unk* mNodes; // 0x70
};

// inferred: one gimmick of the camp (0x40 bytes)
struct NewCampGmkInfo_unk {
    // NONMATCHING: 99.50% - only the floats 0x808877E4/E8 (outside the split)
    NewCampGmkInfo_unk() {
        m00 = 0;
        mModel = 0;
        m3C = 0;
        mMtx.SetIdentity();
        m04 = 0;
    }

    int m00;
    short m04;
    NewCampModel_unk* mModel; // 0x08
    atn::Matrix3x4 mMtx;      // 0x0C
    unsigned char m3C;
};

// inferred: model names searched in the "menu_back" nodes (0x8075B32C)
struct NewCampGmkName_unk {
    char mName[0x20];
    int m20;
};
static const NewCampGmkName_unk sGmkNames[13] = {
    {"menu_back30_0000", 0}, {"menu_back30_0007", 0}, {"menu_back30_0004", 0}, {"menu_back30_0009", 0},
    {"menu_back30_0001", 0}, {"menu_back30_0002", 0}, {"menu_back30_0008", 0}, {"menu_back30_0006", 0},
    {"", 0},                 {"", 0},                 {"menu_back20_0001", 1}, {"menu_back21_0001", 1},
    {"", 0},
};
static const char* const sMenuBack = "menu_back";

class NewCampPartsGmk : public NewCampPartsBase {
public:
    NewCampPartsGmk(atn::Task* parent);
    static NewCampPartsGmk* fn_805128C0(atn::Task* parent);
    virtual ~NewCampPartsGmk();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C(NewCampScene_unk* scene);
    virtual void vf24();
    virtual void vf28();
    virtual int vf4C();
    void SetModel_unk(NewCampModel_unk* model, int i);

    ArrayPtr_unk<NewCampGmkInfo_unk> mInfo; // 0xD4
    atn::EfpHandle_unk mEfp[8];             // 0xD8
};

// NONMATCHING: 99.19% - only relocations: pool 0x8075B510 is outside the split and the base
// constructor is still fn_8050F93C in symbols.txt
NewCampPartsGmk* NewCampPartsGmk::fn_805128C0(atn::Task* parent) {
    if (parent != 0) {
        return new (1, "", "", 0) NewCampPartsGmk(parent);
    }
    return 0;
}

// inferred: not inline (its literals are in the pool, not @STRING@); inlined into fn_805128C0 and
// its out-of-line copy stripped by the linker
NewCampPartsGmk::NewCampPartsGmk(atn::Task* parent) : NewCampPartsBase(parent) {
    m98 = "menu_back";
    m4C = 13;
    if (m4C > 0) {
        mInfo.Reset_unk(new (1, "", "", 0) NewCampGmkInfo_unk[m4C]);
    }
    mEfp[0].fn_8023780C("/effect/be010_007.efp");
}

// NONMATCHING: 99.86% - only the call to the base destructor, still fn_8050FA7C in symbols.txt
NewCampPartsGmk::~NewCampPartsGmk() {}

int NewCampPartsGmk::vf4C() {
    return mEfp[0].fn_80237874() != 0;
}

// inferred: inline helper; it gives the register order of the original (name invented)
inline void NewCampPartsGmk::SetModel_unk(NewCampModel_unk* model, int i) {
    unsigned int hash = model->mHash;
    if (hash == fn_800DC6B4(sGmkNames[i].mName)) {
        mInfo[i].mModel = model;
        mInfo[i].mMtx = GetBase_unk(mInfo[i].mModel)->mMtx;
    }
}

// NONMATCHING: 99.80% - only the reference to the name table 0x8075B32C (outside the split)
void NewCampPartsGmk::vf1C(NewCampScene_unk* scene) {
    for (NewCampNode_unk* node = scene->mNodes; node != 0; node = node->mNext) {
        if (node->mType == 4) {
            const char* name = node->mName;
            for (int i = 0; i < m4C; i++) {
                if (strstr(name, sMenuBack) != 0) {
                    for (int j = 0; j < node->mModelNum; j++) {
                        SetModel_unk(node->mModels[j], i);
                    }
                }
            }
        }
    }
}

void NewCampPartsGmk::vf10() {}

void NewCampPartsGmk::vf14() {}

void NewCampPartsGmk::vf24() {}

void NewCampPartsGmk::vf28() {
    fn_80239DAC(lbl_8087F3C0, this, 1, 0);
}
