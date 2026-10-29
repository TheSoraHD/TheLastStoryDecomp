#include "game/NewCampPartsBase.h"
#include "atn/EfpHandle_unk.h"
#include "math/Vec.h"
#include "atn/LH_Scene.h"
#include "atn/Texture_unk.h"
#include "atn/Array_unk.h"
#include "std/vector.h"

extern "C" char* strstr(const char* s, const char* sub);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    // inferred: name invented (the value is set after the manager is loaded, 0x80512CF4)
    void SetBC_unk(int v) { mBC = v; }
    unsigned char m00[0xB8];
    int mB8;
    int mBC;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_8023A108(EffectMgr_unk* mgr, void* owner, int id, int arg);
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: plays an effect with the default color and scale (name invented; the color is a
// temporary of an inlined function, below the others on the stack of 0x80512C90)
inline void PlayEfp_unk(atn::EfpFile* efp, const atn::Vector3& pos, const atn::Vector3& rot) {
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, pos, rot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
}
extern "C" void PSMTX44MultVec(const float m[4][4], const atn::Vector3* src, atn::Vector3* dst);

// inferred: render settings (lbl_8087EFA8); m3C is the clear color
struct Render_unk {
    unsigned char m00[0x3C];
    Color4f_unk mColor; // 0x3C
};
extern "C" Render_unk* lbl_8087EFA8;

// inferred: resource of a model (only the base at 0x10 is used)
struct NewCampModelRes_unk {
    unsigned char m00[0x10];
    unsigned char m10[4];
};
// inferred: model of the scene; the matrix is read through a base at offset 0 (null-checked cast)
struct NewCampModelBase_unk {
    unsigned char m00[4];
    NewCampModelRes_unk* mRes; // 0x04
    unsigned char m08[0x30 - 0x08];
    atn::Matrix3x4 mMtx; // 0x30
};
struct NewCampModel_unk : public NewCampModelBase_unk {
    unsigned char m60[0x88 - 0x60];
    unsigned int mFlags;    // 0x88: inferred: bit 31 hides the model
    unsigned int mHash; // 0x8C: inferred: compared with fn_800DC6B4 of a name
};
// inferred: resource part of a model, null when there is no model (0x80512D24); name invented
inline void* GetModelRes_unk(NewCampModel_unk* model) {
    return model == 0 ? 0 : model->mRes->m10;
}
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

// inferred: engine light (0x44 bytes; see atn/LH_Scene.cpp)
struct Light_unk {
    Light_unk();
    int m00;
    int m04;
    atn::Vector3 m08;
    atn::Vector3 m14;
    int m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    float m38;
    float m3C;
    float m40;
};

// inferred: plane of the view frustum (normal and distance)
struct Plane_unk {
    atn::Vector3 mNormal;
    float mDist;
};

// inferred: engine camera (0x1F4 bytes; constructor 0x8004B290, destructor 0x8004B338). Layout
// from its copy (0x80513F9C).
struct Camera_unk {
    Camera_unk(int a = 0, int b = 0);
    ~Camera_unk();

    int mType;            // 0x00
    int mFovType;         // 0x04
    atn::Vector3 mPos;    // 0x08
    atn::Vector3 mTarget; // 0x14
    atn::Vector3 mUp;     // 0x20
    atn::Vector3 mDir;    // 0x2C
    float m38;
    float m3C;
    float m40;
    float m44;
    float m48;
    float m4C;
    float m50;
    float m54;
    atn::Matrix3x4 m58;
    atn::Matrix44_unk m88;
    float mC8;
    float mCC;
    atn::Matrix3x4 mD0;
    atn::Matrix3x4 m100;
    int m130;
    float m134;
    float m138;
    atn::Vector3 m13C;
    float m148;
    atn::Vector3 m14C;
    float m158;
    atn::Vector3 m15C;
    float m168;
    atn::Vector3 m16C;
    float m178;
    atn::Vector3 m17C;
    atn::Vector3 m188;
    Plane_unk m194[6];
};

// inferred: small records of the scene state (copied by weak copy constructors)
struct SceneRec1C_unk {
    int m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
};
struct SceneRec50_unk {
    int m00;
    atn::Vector2_unk m04;
    atn::Vector2_unk m0C;
    atn::Vector2_unk m14;
    float m1C;
    float m20;
    atn::Vector3 m24;
    atn::Vector3 m30;
    atn::SimpleArray<atn::Texture_unk> m3C;
    int m44;
    int m48;
    int m4C;
};
struct SceneRec48_unk {
    SceneRec48_unk(const SceneRec48_unk& o);
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    atn::Vector3 m38;
    float m44;
};
struct SceneRec3C_unk {
    SceneRec3C_unk(const SceneRec3C_unk& o);
    int m00;
    int m04;
    float m08;
    atn::Vector2_unk m0C[4];
    atn::Quaternion_unk m2C;
};
struct SceneRec28_unk {
    SceneRec28_unk(const SceneRec28_unk& o);
    int m00;
    int m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    atn::Vector3 m1C;
};
struct SceneRec48b_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    atn::Vector3 m38;
    float m44;
};
struct SceneRec28b_unk {
    int m00;
    int m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    atn::Vector3 m1C;
};
// inferred: 0x50-byte record with a copy constructor of its own (0x8037309C)
struct SceneRecExt_unk {
    SceneRecExt_unk(const SceneRecExt_unk& o);
    unsigned char m00[0x50];
};
struct ScenePair_unk {
    int a;
    int b;
};

// inferred: scene of the camp (LH_Scene and the data built from it); destructor 0x800C1FB4
struct NewCampScene2_unk {
    ~NewCampScene2_unk();

    atn::LH_Scene m000;
    int m008;
    float m00C[12];
    SceneRec1C_unk m03C;
    std::vector<Light_unk> m058;
    std::vector<Camera_unk> m064;
    int m070;
    int m074;
    SceneRec50_unk m078;
    int m0C8;
    int m0CC;
    SceneRec50_unk m0D0;
    int m120;
    atn::Matrix3x4 m124;
    atn::Vector3 m154;
    float m160;
    atn::Texture_unk m164;
};

// inferred: state of the camp scene (0x6B4 bytes), returned by value by NewCampTask_unk
struct NewCampSceneState_unk {
    Camera_unk mCamera;       // 0x000
    Light_unk mLight;         // 0x1F4
    Color4f_unk mColor;       // 0x238: clear color
    Color4f_unk m248;
    NewCampScene2_unk mScene; // 0x258
    int m3EC;
    SceneRec48_unk m3F0;
    int m438;
    int m43C;
    int m440;
    float m444;
    float m448;
    int m44C;
    SceneRec3C_unk m450;
    int m48C;
    SceneRec28_unk m490;
    int m4B8;
    SceneRecExt_unk m4BC;
    int m50C;
    int m510;
    int m514;
    int m518;
    float m51C;
    float m520;
    float m524;
    float m528;
    int m52C;
    int m530;
    float m534;
    float m538;
    int m53C;
    int m540;
    int m544;
    int m548;
    int m54C;
    float m550;
    float m554;
    float m558;
    float m55C;
    ScenePair_unk m560;
    ScenePair_unk m568;
    int m570;
    float m574;
    float m578;
    float m57C;
    float m580;
    float m584;
    float m588;
    float m58C;
    float m590;
    float m594;
    float m598;
    float m59C;
    float m5A0;
    ScenePair_unk m5A4;
    int m5AC;
    ScenePair_unk m5B0;
    int m5B8;
    SceneRec48b_unk m5BC;
    int m604;
    atn::Quaternion_unk m608[4];
    int m648;
    int m64C;
    float m650;
    SceneRec28b_unk m654;
    int m67C;
    int m680;
    float m684;
    ScenePair_unk m688[4];
    float m6A8;
    float m6AC;
    int m6B0;
};

// inferred: camp task (NewCampPartsBase::m48)
struct NewCampTask_unk {
    // inferred: weak copy 0x805134F4 (returns an object with a destructor: never inlined)
    // NONMATCHING: the original inlines the copies of the camera, the light and the scene, whose
    // own member copies (0x80513CF8-0x805143C8) stay out of line; here the scene copy inlines them
    // (written out, with or without a user copy constructor) and the state copy then calls the
    // camera, light and scene copies
    NewCampSceneState_unk GetState_unk() const { return mState; }

    unsigned char m0000[0x468];
    NewCampSceneState_unk mState; // 0x468
    unsigned char m0B1C[0x1354 - 0xB1C];
    unsigned int m1354;
};

// inferred: clear colors of the camp
static Color4f_unk sColors[8] = {
    Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f),
    Color4f_unk(0.05f, 0.05f, 0.05f, 1.0f),
    Color4f_unk(1.0f, 0.67058825f, 0.38039216f, 1.0f),
    Color4f_unk(0.0f, 0.0f, 0.1254902f, 1.0f),
    Color4f_unk(0.85882354f, 0.8392157f, 0.36078432f, 1.0f),
    Color4f_unk(0.2901961f, 0.13725491f, 0.37254903f, 1.0f),
    Color4f_unk(0.81960785f, 0.6f, 0.6666667f, 1.0f),
    Color4f_unk(0.7019608f, 0.81960785f, 0.6745098f, 1.0f),
};
static Color4f_unk sColors2[8] = {
    Color4f_unk(0.0f, 0.0f, 0.3137f, 1.0f),
    Color4f_unk(0.05f, 0.05f, 0.05f, 1.0f),
    Color4f_unk(1.0f, 0.67058825f, 0.38039216f, 1.0f),
    Color4f_unk(0.0f, 0.0f, 0.1254902f, 1.0f),
    Color4f_unk(0.85882354f, 0.8392157f, 0.36078432f, 1.0f),
    Color4f_unk(0.2901961f, 0.13725491f, 0.37254903f, 1.0f),
    Color4f_unk(0.81960785f, 0.6f, 0.6666667f, 1.0f),
    Color4f_unk(0.7019608f, 0.81960785f, 0.6745098f, 1.0f),
};

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
    void fn_80512C90(int i);
    void fn_80513020(int color, int i);

    ArrayPtr_unk<NewCampGmkInfo_unk> mInfo; // 0xD4
    atn::EfpFile mEfp[8];             // 0xD8
};

NewCampPartsGmk* NewCampPartsGmk::fn_805128C0(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) NewCampPartsGmk(parent);
    }
    return 0;
}

// inferred: not inline (its literals are in the pool, not @STRING@); inlined into fn_805128C0 and
// its out-of-line copy stripped by the linker
NewCampPartsGmk::NewCampPartsGmk(atn::Task* parent) : NewCampPartsBase(parent) {
    m98 = "menu_back";
    m4C = 13;
    if (m4C > 0) {
        mInfo.Reset_unk(new ((MemoryTag)1, "", "", 0) NewCampGmkInfo_unk[m4C]);
    }
    mEfp[0].LoadRequest("/effect/be010_007.efp");
}

NewCampPartsGmk::~NewCampPartsGmk() {}

int NewCampPartsGmk::vf4C() {
    return mEfp[0].IsLoading() != 0;
}

// inferred: inline helper; it gives the register order of the original (name invented)
inline void NewCampPartsGmk::SetModel_unk(NewCampModel_unk* model, int i) {
    unsigned int hash = model->mHash;
    if (hash == fn_800DC6B4(sGmkNames[i].mName)) {
        mInfo[i].mModel = model;
        mInfo[i].mMtx = GetBase_unk(mInfo[i].mModel)->mMtx;
    }
}

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

// inferred: plays the effect on gimmick `i` (name unknown)
void NewCampPartsGmk::fn_80512C90(int i) {
    fn_80239DAC(lbl_8087F3C0, this, 1, 0);
    lbl_8087F3C0->SetBC_unk(1);
    lbl_8087F3C0->mB8 = 6;
    fn_80232B7C(this, 1);
    if (GetModelRes_unk(mInfo[i].mModel) != 0) {
        atn::Matrix3x4 mtx(mInfo[i].mMtx.GetRotation_unk());
        atn::Vector3 rot(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &rot, &rot);
        rot.fn_80139F60();
        PlayEfp_unk(&mEfp[0], mInfo[i].mMtx.GetTrans_unk(), rot);
    }
    fn_8023A108(lbl_8087F3C0, this, 1, 0x1E);
    lbl_8087F3C0->mB8 = 0;
    lbl_8087F3C0->mBC = 0;
}

// inferred: sets the clear color (`color` 0: the one of the scene, else sColors[color]) and plays
// the effect on gimmick `i`; models 11 and 12 are shown only with the scene color
// NONMATCHING: 99.98% - only the call to the state getter (0x805134F4, not matched yet)
void NewCampPartsGmk::fn_80513020(int color, int i) {
    if (color == 0) {
        NewCampGmkInfo_unk* info = mInfo.mData;
        NewCampModel_unk* model = info[11].mModel;
        if (model != 0) {
            model->mFlags &= ~0x80000000;
        }
        model = info[12].mModel;
        if (model != 0) {
            model->mFlags &= ~0x80000000;
        }
        NewCampSceneState_unk state = ((NewCampTask_unk*)m48)->GetState_unk();
        lbl_8087EFA8->mColor = state.mColor;
    } else {
        NewCampGmkInfo_unk* info = mInfo.mData;
        NewCampModel_unk* model = info[11].mModel;
        if (model != 0) {
            model->mFlags |= 0x80000000;
        }
        model = info[12].mModel;
        if (model != 0) {
            model->mFlags |= 0x80000000;
        }
        lbl_8087EFA8->mColor = sColors[color];
    }
    if (((NewCampTask_unk*)m48)->m1354 <= 1) {
        fn_80512C90(i);
    } else {
        fn_80239DAC(lbl_8087F3C0, this, 1, 0);
    }
}



// inferred: copies of the records called out of line by 0x805134F4 (weak in the original; written
// out here so that they are not inlined into the copy of the state)
SceneRec48_unk::SceneRec48_unk(const SceneRec48_unk& o)
    : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C), m20(o.m20), m24(o.m24), m28(o.m28), m2C(o.m2C), m30(o.m30), m34(o.m34), m38(o.m38), m44(o.m44) {}

SceneRec3C_unk::SceneRec3C_unk(const SceneRec3C_unk& o) : m00(o.m00), m04(o.m04), m08(o.m08) {
    m0C[0] = o.m0C[0];
    m0C[1] = o.m0C[1];
    m0C[2] = o.m0C[2];
    m0C[3] = o.m0C[3];
    m2C = o.m2C;
}

SceneRec28_unk::SceneRec28_unk(const SceneRec28_unk& o)
    : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C) {}
