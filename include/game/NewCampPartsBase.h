#ifndef GAME_NEWCAMPPARTSBASE_H
#define GAME_NEWCAMPPARTSBASE_H

#include "atn/Task.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "math/Vec.h"
#include "atn/LH_Xsca.h"

#include "atn/Array_unk.h"
#include "atn/LH_Motion.h"

namespace atn {
class D3Anime;
class D3AnimeInstance;
struct ModelBase;
class D2A_Task;
template <int N, int M>
class D2A_TaskInstanceBase;

// inferred: view of atn::ModelInstance (src/atn/ModelInstance.cpp) shared by the camp parts; only the
// members used by them
class ModelInstance {
public:
    void fn_8009EE30(const Matrix3x4& mtx); // inferred: sets the matrix
    unsigned char* m00;
    unsigned char* m04; // inferred: resource; its model (a ModelBase) starts at 0x10
    unsigned char m08[0x30 - 0x08];
    Matrix3x4 mMtx; // 0x30
    unsigned char m60[0x88 - 0x60];
};
} // namespace atn

// inferred: model of a camp scene node (an atn::ModelInstance with flags and a name hash); the parts
// keep it through a null-checked cast to the instance (names invented)
struct NewCampPartsModel_unk : public atn::ModelInstance {
    union {
        unsigned int mFlags; // 0x88: inferred: bit 31 hides the model
        struct {
            unsigned int mHidden : 1;
            unsigned int m88_1 : 31;
        };
    };
    unsigned int mHash; // 0x8C: inferred: compared with fn_800DC6B4 of a name
};
// inferred: the null-checked cast of the model (name invented)
inline atn::ModelInstance* GetModel_unk(NewCampPartsModel_unk* model) {
    return model != 0 ? static_cast<atn::ModelInstance*>(model) : 0;
}
// inferred: model data of the resource of the model, null without a model (name invented)
inline atn::ModelBase* GetModelData_unk(NewCampPartsModel_unk* model) {
    return model != 0 ? (atn::ModelBase*)(model->m04 + 0x10) : 0;
}

// inferred: node of the camp scene list (type 4 nodes hold models; names invented)
struct NewCampNode_unk {
    unsigned char m00[0x48];
    int mType;                       // 0x48
    NewCampNode_unk* mNext;          // 0x4C
    unsigned char m50[0x5C - 0x50];
    char mName[0x344 - 0x5C];        // 0x5C
    int mModelNum;                   // 0x344
    NewCampPartsModel_unk** mModels; // 0x348
};
// inferred: scene whose nodes are searched by the parts (NewCampPartsGmk::vf1C, 0x80512B44)
struct NewCampScene_unk {
    unsigned char m00[0x70];
    NewCampNode_unk* mNodes; // 0x70
};

// inferred: array owned through a pointer, replaced with new[] and freed with delete[] (pattern of
// NewCampPartsBase::m50 in its constructor 0x8050F93C and NewCampPartsGmk::mD4); names invented
template <class T>
class ArrayPtr_unk {
public:
    ArrayPtr_unk() : mData(0) {}
    ~ArrayPtr_unk() { delete[] mData; }
    void Reset_unk(T* data) {
        T* old = mData;
        mData = data;
        delete[] old;
    }
    T& operator[](int i) { return mData[i]; }

    T* mData;
};

// inferred: entry of the model tables of the parts (0x40 bytes, array NewCampPartsBase::m50); the
// constructor is a weak copy at 0x801D082C (game/CosEditProtector.cpp), same body as
// NewCampGmkInfo_unk (game/NewCampPartsGmk.cpp); names invented
struct NewCampPartsEntry_unk {
    NewCampPartsEntry_unk() {
        mAnime = 0;
        mModel = 0;
        m3C = 0;
        mMtx.SetIdentity();
        m04 = 0;
    }

    atn::D3Anime* mAnime;          // 0x00
    short m04;
    NewCampPartsModel_unk* mModel; // 0x08
    atn::Matrix3x4 mMtx;           // 0x0C
    unsigned char m3C;
};

// inferred: table given to fn_8050FB38/fn_8050FD38 (model and d2anime names); names invented
struct NewCampPartsFile_unk {
    const char* mModel;
    const char* mAnime;
    unsigned char m08; // inferred: flag (1 for some status boards)
};

// inferred: motion of an entry (0x54 bytes, array NewCampPartsBase::m54; constructor 0x801D087C and
// destructor 0x801D0960 are weak copies in game/CosEditProtector.cpp); names invented
struct NewCampPartsMotion_unk {
    NewCampPartsMotion_unk() : mFrame(-1.0f) {
        memset(mWork, 0, sizeof(mWork));
        mHash = 0;
    }
    atn::LH_Motion mMotion; // 0x00
    unsigned int mHash;     // 0x0C: inferred: node name hash
    MotionPose_unk mPose;   // 0x10
    char mWork[0x12];       // 0x3C
    float mFrame;           // 0x50
};

// inferred: 2D point of a d2anime pane (0x14 bytes; fn_801F4E8C)
struct NewCampPartsPoint_unk {
    NewCampPartsPoint_unk() {}
    float x;
    float y;
    float m08;
    float m0C;
    float m10;
};

// inferred: motion table given to fn_8050FC04 (node name and motion path); names invented
struct NewCampPartsMotionFile_unk {
    const char* mNode;
    const char* mPath;
};

// inferred from __vt__16NewCampPartsBase (0x807932B8), its constructor (0x8050F93C) and its
// destructor (0x8050FA7C). The inline virtuals have weak copies in game/CosEditBase.cpp
// (0x801CFC28-0x801CFC44).
class NewCampPartsBase : public atn::Task {
public:
    NewCampPartsBase(atn::Task* parent); // 0x8050F93C
    virtual ~NewCampPartsBase();         // 0x8050FA7C
    virtual int vf0C();                  // 0x8051007C
    virtual void vf10();                 // 0x805100F8
    virtual void vf14();                 // 0x80510CB0
    virtual void vf1C(NewCampScene_unk* scene) {} // 0x801CFC44
    virtual void vf20();                 // 0x80512024
    virtual void vf24() {}               // 0x801CFC40
    virtual void vf28() {}               // 0x801CFC3C
    virtual void vf2C();                 // 0x80512020
    virtual void vf30() {}               // 0x801CFC38
    virtual void vf34(int state);        // 0x80512018: inferred: m7C = state
    virtual unsigned char vf38() { return m94; } // 0x801CFC30
    virtual void vf3C(int arg);          // 0x80511208
    virtual void vf40(void* group);      // 0x805112F8: inferred: adds the debug menu items
    virtual void vf44();                 // 0x80511714
    virtual void vf48();                 // 0x805117BC
    virtual int vf4C() { return 0; }     // 0x801CFC28
    // inferred: places the entry model and its d2anime at the node `node` of `model` (0x8051012C)
    virtual void vf50(NewCampPartsEntry_unk* entry, atn::ModelBase* model, const char* node);
    virtual void vf54(NewCampPartsEntry_unk* entry, const char* node); // 0x805100FC
    virtual void vf58(NewCampPartsEntry_unk* entry, const char* node); // 0x80510720
    virtual void vf5C(NewCampPartsEntry_unk* entry);                   // 0x80510564
    // inferred: plays `motion` on the entry model (flags: 1 translation, 2 rotation, 4 scale) (0x805109CC)
    virtual void vf60(NewCampPartsEntry_unk* entry, NewCampPartsMotion_unk* motion, int flags, float step);

    // inferred: creates the d2anime of each entry (kind 1: instances) (0x8050FB38)
    void fn_8050FB38(int count, NewCampPartsEntry_unk* entries, const NewCampPartsFile_unk* file, int load,
                     short kind);
    // inferred: starts loading the motions (0x8050FC04)
    void fn_8050FC04(int count, NewCampPartsMotion_unk* motions, const NewCampPartsMotionFile_unk* file);
    void fn_8050FC88(int count, NewCampPartsEntry_unk* entries, int load); // inferred: loads the d2anime
    int fn_8050FD20(); // inferred: these parts are the current ones
    // inferred: finds the entry models in the scene nodes whose name contains `name` (0x8050FD38)
    void fn_8050FD38(NewCampScene_unk* scene, NewCampPartsEntry_unk* entries, const NewCampPartsFile_unk* file,
                     int count, const char* name);
    void fn_8050FE44(NewCampNode_unk* node, NewCampPartsEntry_unk* entry, const NewCampPartsFile_unk* file);
    void fn_80510D64(int wrap, int se); // inferred: moves the cursor up/down (m80/m84)
    void fn_80510E94(int wrap, int se); // inferred: moves the cursor left/right
    void fn_80510FB8(int wrap, int se); // inferred: moves the cursor in a grid of m8C columns
    void fn_80511258(NewCampPartsEntry_unk* entry, atn::D3Anime* anime, const atn::Vector3& pos,
                     const atn::Vector3& scale, const char* node);
    void fn_805112A8(NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime, const atn::Vector3& pos,
                     const atn::Vector3& scale, const char* node);
    void fn_805118B4(atn::Task* task, const char* name, NewCampPartsEntry_unk* entry, const NewCampPartsPoint_unk& pos,
                     int b); // inferred: cursor of a model
    void fn_805119C8(atn::Task* task, const char* name, NewCampPartsEntry_unk* entry, const NewCampPartsPoint_unk& pos,
                     int b);
    void fn_80512028(NewCampPartsEntry_unk* entry, atn::D3AnimeInstance* anime, const atn::Vector3& pos,
                     const atn::Vector3& scale, const char* node);
    void fn_80512474(NewCampPartsEntry_unk* entry, atn::D3Anime* anime, const atn::Vector3& pos,
                     const atn::Vector3& scale, const char* node);

    atn::Task* m48;                      // inferred: the parent given to the constructor
    int m4C;                             // inferred: number of entries of the derived tables
    ArrayPtr_unk<NewCampPartsEntry_unk> m50; // inferred: entries (m4C)
    ArrayPtr_unk<NewCampPartsMotion_unk> m54; // inferred: motions
    float m58;
    atn::LH_Xsca m5C;                    // 0x5C: inferred: camera
    float m64[6];
    int m7C;                             // inferred: state of the derived parts
    int m80;                             // inferred: cursor
    int m84;                             // inferred: number of cursor positions
    int m88;
    int m8C;
    int m90;
    unsigned char m94;
    unsigned char m95;
    unsigned char m96; // inferred: the list must be read again
    unsigned char m97;
    std::string m98;
    atn::Vector3 mA4;                    // inferred: "OffsetPos"
    atn::Vector3 mB0;                    // inferred: "OffsetScl"
    atn::Vector3 mBC;                    // inferred: "LocatorScl"
    atn::Vector3 mC8;                    // inferred: "BasePos"
};

#endif
