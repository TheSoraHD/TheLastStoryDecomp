#ifndef ATN_MODEL_UNK_H
#define ATN_MODEL_UNK_H

#include "atn/GXDecl.h"
#include "math/Vec.h"
#include "atn/Sphere.h"
#include "atn/Array_unk.h"
#include "atn/IRender.h"
#include "atn/LH_Model.h"
#include "atn/LH_Material.h"
#include "game/ResHandle.h"
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

namespace atn {
class LoadHandle;
class LH_Motion;
struct MotionCmd_unk; // inferred: motion command entry of a motion slot (16 bytes)
class Matrix3x4;
struct NodeChunk;
struct ModelNodeCallbackContext;
struct MotionBuffer; // inferred: node pose passed to the m370 callback (name from the RTTI of CharaFaceControl)
struct ModelChild_unk; // inferred: objects owned by a model (deleted through their virtual destructor)
struct Model;
}
// inferred: camera object owned by a model (0x1FC bytes, created on load by CharaTask::fn_8013655C
// and by the equipment at 0x80564108); defined where it is used
struct ModelCamera_unk;
// inferred: camera lens (defined in game/CharaTask.cpp)
struct Lens_unk;

// material of a model node (returned by atn::Model::fn_80092954; include/atn/Material_unk.h)
struct Material_unk;

// inferred: materials of a model (0x30 bytes, atn::ModelBase+0x104; also owned by CharaTask::m480).
// The destructor is defined first in atn/Model.cpp (0x800899DC) and deletes every material.
// Names invented.
struct MaterialSet_unk {
    ~MaterialSet_unk();  // 0x800899DC
    void fn_80089AD4();  // inferred: releases the textures of every material (CharaManager::vf14)
    int fn_80089B5C();   // inferred: a texture of the materials is ready

    atn::SimpleArray<Material_unk*> mMaterials; // 0x00
    atn::SimpleArray<unsigned short> m08[4];    // 0x08: inferred: element type
    atn::SimpleArray<unsigned short> m28;       // 0x28: inferred: element type
};

namespace atn {

// callback run on the node matrices by CalcMatrixStack (type from the RSO names)
typedef boost::function<void(Matrix3x4*, const NodeChunk&, const ModelNodeCallbackContext&)> ModelNodeCallback_unk;

// inferred: one level of detail of a model (0x18 bytes; constructor 0x80089C28 and destructor
// 0x80089CE4 are weak copies of atn/Model.cpp). Names invented.
struct ModelLod_unk {
    ModelLod_unk() : m08(0), m0C(0.0f) {}
    ~ModelLod_unk() { mHandle.fn_80473F88(); }

    LH_Model mHandle; // 0x00
    void* m08;        // inferred: loaded data (atn::ModelBase::Res_unk)
    float m0C;        // inferred: distance of the level
    SimpleArray<unsigned short> m10; // inferred: element type
};

// atn::ModelBase (vtable __vt__Q23atn9ModelBase, RTTI with atn::IRender as base); 0x214 bytes.
// Constructor 0x8008A4E0 and destructor 0x8008A76C (the members embedded by gmk::SavePoint,
// gmk::SwingObject, gmk::Chain...).
struct ModelBase : public IRender {
    // inferred: arrays of models use the default flags 9 (default constructor 0x803165E4 in
    // game/Em316LaserSphere.cpp)
    ModelBase(unsigned int flags = 9); // 0x8008A4E0
    virtual ~ModelBase();              // 0x8008A76C
    virtual int GetRenderType_unk();                 // 0x80099F68
    virtual void vf10(RenderView_unk* view);         // 0x8008E304: inferred: draw
    virtual int vf14(void* arg1, void* arg2, void* frustum); // 0x8008FA24
    virtual void vf18(RenderView_unk* view);         // 0x8008D6C0
    virtual void vf1C();                             // 0x800902BC
    virtual int vf20();                              // 0x8008B96C: inferred: still loading
    virtual void vf24();                             // 0x8008B974
    void CalcMatrixStack(const MotionBuffer* motion, ModelNodeCallback_unk callback); // 0x800902C0
    // RSO: GetNodeNoByName__Q23atn9ModelBaseCFPCci (0x80092814): node index by name, < 0 if missing
    int GetNodeNoByName(const char* name, int arg) const;

    // inferred: world matrix and node matrices (sub-object at 0x08; its implicit destructor frees
    // the node matrices). Names invented.
    struct Mtx_unk {
        Mtx m;                          // 0x00: inferred: world matrix
        SimpleArray<Matrix3x4> mNodes;  // 0x30: inferred: node matrices
    };
    // inferred: four values copied to the parts and equipment of a character (CharaTask::fn_80148B38)
    struct Float4_unk {
        float m00;
        float m04;
        float m08;
        float m0C;
    };
    // inferred: entry of the model in a map tree (set by MapPartsModel::vf24); names invented
    struct TreeEntry_unk {
        void* mTree;
        void* mNode;
    };
    // inferred: node edit (0x2C bytes; fn_80093F58/fn_80094778); names invented
    struct NodeEdit_unk {
        unsigned int mHash;
        unsigned int mFlags; // inferred: 1 offset, 2 rotation, 4 scale (set when not identity)
        atn::Vector3 m08;    // inferred: offset
        atn::Vector3 m14;    // inferred: rotation
        atn::Vector3 m20;    // inferred: scale
    };
    // inferred: resource data of the model (accessor 0x8000DD04); names invented
    struct NodeRes_unk {
        unsigned char m00[0x14];
        unsigned int m14; // inferred: name hash (CharaFaceControl)
        int m18;          // inferred: index of the node in m21C
        unsigned char m1C[0x40 - 0x1C];
        atn::Vector3 m40; // inferred: initial rotation (euler)
    };
    struct Res_unk {
        unsigned char m00[0x10];
        atn::Vector3 mMin; // 0x10: inferred: bounding box (MapPartsModel::vf24)
        atn::Vector3 mMax; // 0x1C
        unsigned char m28[0x44 - 0x28];
        int m44; // inferred: number of nodes
        NodeRes_unk** m48;
    };
    // inferred: loaded data of the first level (the main model)
    Res_unk* GetRes_unk() const { return (Res_unk*)mLods[0].m08; }
    // inferred: the same object seen through the atn::Model declarations (the members embedded as
    // atn::ModelBase by gmk::SavePoint, gmk::Chain, Arms... call methods declared on atn::Model)
    inline Model* GetModel_unk();

    unsigned int mFlags; // 0x04
    Mtx_unk mMtx;        // 0x08
    Float4_unk m40;
    int m50; // inferred: node index (moved into m20C bits 15-8 while m55C is 8, CharaTask::fn_80145334)
    float m54;       // inferred: largest scale axis
    int m58;
    int m5C;
    unsigned char m60[0x94 - 0x60];
    int m94;
    float m98; // inferred: draw priority ("PRIORITY" of the map parts)
    float m9C[4];
    float mAC; // inferred: draw distance scale (CharaTask sets 1.0 or 1.3, 0x80149A30)
    int mB0; // inferred: compared with 0 by CharaTask::fn_8013655C
    int mB4;
    boost::function<void()> mB8; // inferred: signature
    SimpleVector<int> mCC;       // inferred: element type
    SimpleVector<NodeEdit_unk> mD8; // inferred: node edits
    SimpleArray<int> mE4;           // inferred: element type
    SimpleArray<unsigned char> mEC; // inferred: node visibility (one byte per node)
    SimpleArray<unsigned char> mF4; // inferred: material visibility (one byte per material)
    LH_Material mFC;
    MaterialSet_unk m104; // inferred: materials (passed to MaterialSet_unk::fn_80089AD4 by CharaManager::vf14)
    int m134;
    MaterialSet_unk* m138; // inferred: materials in use (null: m104)
    SimpleArray<int> m13C; // inferred: element type
    SimpleArray<int> m144; // inferred: element type
    float m14C;
    float m150;
    float m154;
    float m158;
    int m15C;
    int m160;
    ModelLod_unk mLods[4]; // 0x164: inferred: levels of detail; the first is the main model
    ModelLod_unk m1C4;     // inferred: shadow model
    struct Lens_unk* m1DC; // inferred: own camera lens, else the global one (CharaTask::fn_8014C540)
    int m1E0;
    SimpleVector<int> m1E4; // inferred: element type
    unsigned char m1F0[0x200 - 0x1F0];
    TreeEntry_unk m200;
    boost::scoped_ptr<ModelCamera_unk> m208; // inferred
    unsigned int m20C_31_24 : 8;
    int m20C_23_16 : 8; // inferred: LOD counter (gmk::SwingObject::vf10 restores m50 from it)
    unsigned int m20C_15_8 : 8; // inferred: saved copy of m50 (CharaTask::fn_80145334)
    unsigned int m20C_7 : 1;
    int mFlag6_unk : 1; // inferred: tested by gmk::SwingObject::vf10
    int mFlag5_unk : 1;          // inferred: tested by CharaTask::fn_8013655C
    unsigned int mFlag4_unk : 1; // inferred: set together with the equipment
    unsigned int m20C_3 : 1;
    int mFlag2_unk : 1; // inferred: CharaManager::vf10 calls fn_8008B140 while it is set
    int mFlag1_unk : 1; // inferred: fn_80093014 (sets m20C_31_24 to 5)
    unsigned int m20C_0 : 1;
    ResHandle m210; // inferred: debug menu group
    // inferred: accessor (the original reads the flag word again before setting mFlag4_unk); name invented
    int IsFlag5_unk() const { return mFlag5_unk; }
};

// atn::Model (vtable __vt__Q23atn5Model; the file keeps its old name)
struct Model : public ModelBase {
    // inferred: default arguments: arrays of models construct them with (8, 32) through a helper
    // (0x80412E94 in gmk::MotionObject)
    Model(int flags = 8, int arg = 32); // 0x80096E94
    virtual ~Model(); // 0x800971D4
    virtual int vf20(); // 0x80097B74: inferred: a motion is still loading
    virtual void vf24(); // 0x800977F0
    using ModelBase::CalcMatrixStack;
    void CalcMatrixStack(ModelNodeCallback_unk callback); // 0x8000D430
    int fn_8009290C(unsigned int hash);          // inferred: material index by name hash
    Material_unk* fn_80092954(const char* name); // inferred: material by name
    Material_unk* fn_800929C0(unsigned int hash); // inferred: material by name hash
    void fn_80092A00(float alpha);               // inferred: alpha of every material
    void fn_800937E8(unsigned int hash, int show); // inferred: shows/hides a node by name hash
    void fn_80093B88(const char* name);            // inferred: shows a node
    void fn_80093C20(unsigned int hash);           // inferred: shows a node by name hash
    int fn_80093C8C(const char* name);             // inferred: the node is shown (1 if missing)
    int fn_80093D28(unsigned int hash);            // inferred: the node is shown (1 if missing)
    void fn_80093E30(unsigned int hash, int show); // inferred: shows/hides a material by name hash
    int fn_80093E90(unsigned int hash);            // inferred: the material is shown (0 if missing)
    void fn_80093EEC(int show);                    // inferred: shows/hides every material
    NodeEdit_unk* fn_800948F0(const char* name);   // inferred: node edit by node name
    void fn_80094E2C(int value);                   // inferred: sets Material_unk::m34 on every material
    void fn_80094F14(float u, float v);            // inferred: UV translation of every material
    Material_unk* fn_80094B6C(const char* name);   // inferred: material by name (second copy)
    void fn_80095CD0();                            // inferred: steps the texture animations
    void fn_8008C828(MaterialSet_unk* list);       // inferred: sets the materials in use
    void fn_80097750(const atn::Vector3& dir, float a, float b, int c); // inferred: direction parameters
    void fn_8008C850(MaterialSet_unk* list);       // inferred: flags from the materials
    void fn_80094D88(int arg, int flag, int a, int b, int c); // inferred: material mode of every material
    void fn_80092A4C();                          // inferred
    const char* fn_8008B130();                   // inferred: model name
    void fn_8008CD60();                          // inferred
    void fn_800954DC(void* owner, atn::Vector2_unk uv, float f); // inferred: UV scroll
    int fn_80097E80(int arg); // inferred: returns nonzero when the motion was updated (CharaTask::fn_80145334)
    void fn_8009385C(int motion, bool play, int arg2, int arg3);  // inferred
    int fn_8008B140();                                // inferred: nonzero while loading
    void fn_8008B978(int arg);                        // inferred
    int fn_800928B0(unsigned int hash, int arg);      // inferred: node index by name hash, < 0 if missing

    // inferred: matrix of node `index`, null for a missing node (inlined at 0x8049CCD4)
    MtxPtr GetNodeMtx_unk(int index) const {
        return (index < 0) ? 0 : mMtx.mNodes.mData[index].m;
    }
    // inferred: matrix of the node called `name` (weak copy 0x800132EC); name invented
    MtxPtr GetNodeMtx_unk(const char* name) const {
        return GetNodeMtx_unk(GetNodeNoByName(name, 0));
    }

    // inferred: also the motion controller characters embed at +0xB0 (chr::Motion_unk is an alias:
    // CharaTask constructs it with this constructor, 0x80135588)
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length
    // inferred: called on load by CharaTask (names keep the address)
    void fn_8008AD4C(const char* path, const char* node); // inferred: loads the model ("MODEL")
    void fn_80092F1C(const char* path, const char* node, float distance); // inferred: adds a LOD
    void fn_80092EC8(const char* path);   // inferred: shadow LOD
    void fn_80097A88(int motion, const char* path); // inferred: loads a motion
    void fn_80097A20(unsigned int motion);          // inferred: releases a motion slot
    void fn_80097A9C(int motion, const LH_Motion* src); // inferred: copies a motion slot
    int fn_80097B34(); // inferred: number of loaded motions
    void fn_80097CCC(int track, float blend); // inferred: stops the motion of a track
    void fn_80092F90();
    void fn_80093014(int arg);
    void fn_8009373C(const char* name, int part); // inferred: shows/hides the named node (option part)
    void fn_80094958(const char* name, const void* a, const void* b, const void* c); // inferred: material edit
    void fn_80093D98(const char* name, int arg); // inferred: material hide
    void fn_80093F58(const void* edit); // inferred: node edit
    // inferred: node edit of `name` (offsets a, b and scale c); flags as fn_80089D70
    void fn_80093F1C(const char* name, const atn::Vector3& a, const atn::Vector3& b, const atn::Vector3& c, int flags);
    void fn_80094778(const char* name); // inferred: removes the node edit of `name`
    void fn_800973C0(void* csv); // inferred: scale
    void fn_80095D44(int node, int parent);
    void fn_80091CFC(const char* node);
    void fn_80099E9C(const char* node, const char* name, float value);
    void fn_80097510();
    atn::LoadHandle* fn_80097D40(int motion); // inferred: handle of the motion (12-byte entries of m224), zero if missing

    // inferred: per-node data (0x2C bytes)
    struct Node_unk {
        unsigned int m00; // inferred: flags (2: the rotation m10 is set, CharaTask::fn_80148CC4)
        atn::Vector3 m04;
        atn::Quaternion_unk m10;
        atn::Vector3 m20; // inferred: scale (copied to CharaTask::m540 by fn_80145334)
    };
    Res_unk* fn_8000DD04(); // inferred: returns the loaded data of the first level
    typedef Node_unk Entry_unk;
    // inferred: motion track (0x30 bytes, six of them at 0x22C; accessors 0x80139550-0x8013C554)
    struct Track_unk {
        int m00;   // inferred: motion id
        int m04;   // inferred: compared with CharaTask::m518 (fn_80145334)
        float m08; // inferred: frame
        float m0C; // inferred: speed
        float m10; // inferred: set to 1.0 when CharaTask::mFlag7 is set (fn_80145334)
        float m14; // inferred: weight (set by the lip sync, Chara1154_unk::fn_8007A7BC)
        unsigned char m18; // inferred: nonzero = the end is tested one step early (CharaTask::fn_8015EB2C)
        unsigned char m19[0x1C - 0x19];
        float m1C; // inferred: current value (moved towards the key by the lip sync)
        float m20; // inferred: weight
        int m24;   // inferred: previous motion slot (m04)
        float m28; // inferred: previous frame (m08)
        float m2C; // inferred: previous m10
    };

    // inferred: accessors called out of line from CharaTask (names keep the address)
    Node_unk* fn_80147A00();
    typedef atn::Sphere Sphere_unk;
    Sphere_unk fn_80094F98(); // inferred: bounding sphere (passed to MapManager::fn_804918A4)
    float GetMotionFrame(int track) const; // 0x80139550
    void fn_80139F24(int value);
    int fn_80139F2C(int track);
    void SetMotionSpeed(int track, float speed); // 0x80139F3C
    void SetMotionFrame(int track, float frame); // 0x80357F38 (defined in game/EnemyTasha.cpp)
    int fn_8013C3DC();
    float fn_8013C3E4(int track);
    void fn_8013C554(int track, float weight);
    float GetMotionSpeed(int track) const { return mTracks[track].m0C; } // weak copy 0x80224628

    SimpleArray<Matrix3x4> m214;     // inferred: element type
    SimpleArray<Node_unk> m21C;      // inferred: per-node data
    SimpleArray<LH_Motion> m224;     // inferred: motion slots (mSize: NpcManager::fn_80547B54)
    Track_unk mTracks[6]; // 0x22C (track 0: frame at 0x234, speed at 0x238)
    int m34C;
    float m350; // inferred: time scale of the character (multiplied with the frame time, CharaTask::fn_8013CB68)
    SimpleArray<MotionCmd_unk> m354; // inferred: motion commands, one 16-byte entry per motion slot
    // inferred: second node pose callback (set by fn_8031F2A4 of game/Em401_01_WhiteLeoPard.cpp, used by the
    // white leopard and the war lion)
    boost::function<void(atn::MotionBuffer*, int)> m35C;
    // inferred: callback run on the node poses (set by CharaFaceControl::Initialize)
    boost::function<void(atn::MotionBuffer*, int)> m370;
    unsigned int m384;          // inferred: number of objects in m388
    ModelChild_unk* m388[8];    // inferred: owned objects (deleted by the destructor)
    atn::Vector3 m3A8; // inferred: direction (normalized, fn_80097750)
    float m3B4;
    float m3B8;
    int m3BC;
    int m3C0;     // inferred: node index (fn_80099E9C)
    float m3C4;
    char m3C8[8]; // inferred: name copied by fn_80099E9C
};

inline Model* ModelBase::GetModel_unk() {
    return (Model*)this;
}

} // namespace atn

#endif
