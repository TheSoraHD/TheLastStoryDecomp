#ifndef ATN_MODEL_UNK_H
#define ATN_MODEL_UNK_H

#include "atn/GXDecl.h"
#include "math/Vec.h"
#include "atn/Sphere.h"
#include "atn/Array_unk.h"
#include "atn/IRender.h"
#include "atn/LH_Model.h"
#include "atn/LH_Motion.h"
#include "atn/LH_Material.h"
#include "game/ResHandle.h"
#include "Event/PoseKey_unk.h"
#include "atn/ModelPose_unk.h"
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include "atn/AABBTree.h"

namespace atn {
class LoadHandle;
class LH_Motion;
class Scene;
struct MotionCmd_unk; // inferred: motion command entry of a motion slot (16 bytes)
// inferred: per-node work entry of a model (18 bytes, cleared by its weak ctor 0x800979E8); name invented
struct NodeWork_unk {
    inline NodeWork_unk(); // inferred: defined in atn/Model.cpp
    unsigned short m00[9];
};
class Matrix3x4;
struct NodeChunk;
struct ModelNodeCallbackContext;
struct MotionBuffer; // inferred: node pose passed to the m370 callback (name from the RTTI of CharaFaceControl)
class PBoneInterface; // physics bones owned by a model (include/atn/PBone.h)
struct MotionSlot_unk; // inferred: motion slot of a model (defined in atn/Model.cpp)
struct ModelComposite_unk; // inferred: composite texture of a model (defined in atn/Model.cpp)
struct ModelCompositeTex_unk; // inferred: texture composite object (defined in atn/Model.cpp)
struct Model;
// (AABB_unk in atn/AABB_unk.h)
class CsvTab;
}
// inferred: camera object owned by a model (0x1FC bytes, created on load by CharaTask::fn_8013655C
// and by the equipment at 0x80564108); defined where it is used
struct ModelCamera_unk;

// material of a model node (returned by atn::Model::fn_80092954; include/atn/Material_unk.h)
struct Material_unk;
struct ShaderLOD_unk; // include/atn/Shader_unk.h
#include "std/string.h"
namespace atn {
class Texture;
typedef Texture Texture_unk;
}

// inferred: materials of a model (0x30 bytes, atn::ModelBase+0x104; also owned by CharaTask::m480).
// The destructor is defined first in atn/Model.cpp (0x800899DC) and deletes every material.
// Names invented.
struct MaterialSet_unk {
    ~MaterialSet_unk();  // 0x800899DC
    void fn_80089AD4();  // inferred: releases the textures of every material (CharaManager::vf14)
    int fn_80089B5C();   // inferred: a texture of the materials is ready

    atn::SimpleArray<Material_unk*> mMaterials; // 0x00
    atn::SimpleArray<int> m08[4];    // 0x08: inferred: index in mMaterials of each material of a level of detail
    atn::SimpleArray<int> m28;       // 0x28: inferred: texture stage of each material that takes an external texture (-1: none)
};

namespace atn {

// inferred: one level of detail of a model (0x18 bytes; constructor 0x80089C28 and destructor
// 0x80089CE4 are weak copies of atn/Model.cpp). Names invented.
struct ModelLod_unk {
    ModelLod_unk() : m08(0), m0C(0.0f) {}
    ~ModelLod_unk() { mHandle.fn_80473F88(); }

    LH_Model mHandle; // 0x00
    void* m08;        // inferred: loaded data (atn::ModelBase::Res_unk)
    float m0C;        // inferred: distance of the level
    SimpleArray<int> m10; // inferred: element type (same constructor copy as MaterialSet_unk::m08)
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
    virtual void vf10(RenderManager* view);         // 0x8008E304: inferred: draw
    virtual int vf14(const Matrix3x4*, const Vector3*, const Frustum_unk*, AABB_unk*); // 0x8008FA24: shadow test
    virtual void vf18(RenderManager* view);         // 0x8008D6C0
    virtual void vf1C();                             // 0x800902BC
    virtual int vf20();                              // 0x8008B96C: inferred: still loading
    virtual void vf24();                             // 0x8008B974
    void CalcMatrixStack(const MotionBuffer* motion, ModelNodeCallback_unk callback); // 0x800902C0
    // RSO: GetNodeNoByName__Q23atn9ModelBaseCFPCci (0x80092814): node index by name, < 0 if missing
    int GetNodeNoByName(const char* name, int arg) const;
    void fn_8008AA90(); // inferred: releases the loaded data (called first by the destructor)
    void fn_8008AD4C(const char* path, const char* node); // inferred: loads the model ("MODEL")
    const char* fn_8008B130();                   // inferred: model name
    unsigned int fn_8008B138();                  // inferred: hash of the model path
    int fn_8008B140();                           // inferred: nonzero while loading
    void fn_8008B978(int unbound);               // inferred: binds the textures of the materials
    void fn_8008C62C(int lod);                   // inferred: maps the materials of a level to the main ones
    void fn_8008C950();                          // inferred: sets up the nodes after loading
    void fn_8008D78C(RenderManager*);            // inferred: visibility (writes mFlag6_unk); view in r4 (left by vf18)
    void fn_8009076C(const Mtx m);               // inferred: applies m to the world and node matrices
    void fn_80091684(const Mtx m, const int* remap); // inferred: builds the skinning matrices
    void fn_80091A90(const Mtx m);                   // inferred: updates the skinning matrices for m
    void fn_80091C10(const ModelBase* src, const int* map, int reset); // inferred: copies node matrices
    void fn_80095CD0();                          // inferred: steps the texture animations
    void fn_80095750(const char* name, ModelCompositeTex_unk* tex); // inferred: adds a composite texture
    void fn_80092EC8(const char* path);          // inferred: loads the shadow model
    void fn_80092F1C(const char* path, const char* node, float distance); // inferred: adds a level of detail
    void fn_80092F90();                          // inferred: releases the levels of detail
    void fn_8009303C();                          // inferred: fetches the loaded levels
    void fn_80093158(ModelLod_unk* lod);         // inferred: sets up a loaded level
    atn::Texture_unk* fn_80095B08(const std::string& name); // inferred: texture of a stage name (bound for the materials)

    // inferred: world matrix (the inherited m) and node matrices (sub-object at 0x08; its implicit
    // destructor frees the node matrices). Names invented.
    struct Mtx_unk : public Matrix3x4 {
        Mtx_unk() { SetIdentity(); }
        SimpleArray<Matrix3x4> mNodes;  // 0x30: inferred: node matrices
    };
    // inferred: four values copied to the parts and equipment of a character (CharaTask::fn_80148B38)
    struct Float4_unk {
        Float4_unk() {}
        Float4_unk(float a, float b, float c, float d) : m00(a), m04(b), m08(c), m0C(d) {}
        float m00;
        float m04;
        float m08;
        float m0C;
    };
    // inferred: node edit (0x2C bytes; fn_80093F58/fn_80094778): the pose key of Event (same
    // SimpleVector<Event::PoseKey_unk> as CharaFaceControl, read with fn_80089EE4 from "NODE_EDIT" lines).
    // m00: node name hash, m04: flags (1 offset, 2 rotation, 4 scale), m08 offset, m14 rotation, m20 scale.
    typedef Event::PoseKey_unk NodeEdit_unk;
    // inferred: loaded data of the model (atn/ModelPose_unk.h)
    typedef ModelJoint_unk NodeRes_unk;
    typedef ModelMtxBlock_unk ResMtx_unk;
    typedef ModelData_unk Res_unk;
    // inferred: world matrix block (weak copies 0x8008D784 and 0x800952F8); name invented
    Mtx_unk* GetMtx_unk() { return &mMtx; }
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
    void* m58; // inferred: block of the memory manager (freed by fn_8008AA90)
    int m5C;
    Matrix3x4 m60; // inferred: matrix the skinning matrices were last built with
    Matrix3x4* m90; // inferred: inverse joint matrices of the loaded data
    int m94;
    float m98; // inferred: draw priority ("PRIORITY" of the map parts)
    Vector3 m9C; // inferred: center of the bounding box
    float mA8;   // inferred: radius of the bounding sphere
    float mAC; // inferred: draw distance scale (CharaTask sets 1.0 or 1.3, 0x80149A30)
    int mB0; // inferred: compared with 0 by CharaTask::fn_8013655C
    int mB4;
    boost::function<void()> mB8; // inferred: signature
    SimpleVector<unsigned int> mCC; // inferred: sorted nodes with the node callback enabled (fn_800920B8)
    SimpleVector<NodeEdit_unk> mD8; // inferred: node edits
    SimpleArray<short> mE4; // inferred: one entry per node (-1 at load)
    SimpleArray<unsigned char> mEC; // inferred: node visibility (one byte per node)
    SimpleArray<unsigned char> mF4; // inferred: material visibility (one byte per material)
    LH_Material mFC;
    MaterialSet_unk m104; // inferred: materials (passed to MaterialSet_unk::fn_80089AD4 by CharaManager::vf14)
    int m134;
    MaterialSet_unk* m138; // inferred: materials in use (null: m104)
    SimpleArray<ShaderLOD_unk*> m13C; // inferred: own shader LODs (deleted with their first shader)
    SimpleArray<ShaderLOD_unk*> m144; // inferred: shared shader LODs (reference counted)
    float m14C;
    float m150;
    float m154;
    float m158;
    MaterialSet_unk* m15C; // inferred: owned
    MaterialSet_unk* m160; // inferred: owned
    ModelLod_unk mLods[4]; // 0x164: inferred: levels of detail; the first is the main model
    ModelLod_unk m1C4;     // inferred: shadow model
    atn::Scene* m1DC;      // inferred: own scene (lights, reflection plane), else the one of RenderManager
    int m1E0;
    SimpleVector<ModelComposite_unk*> m1E4; // inferred: composite textures
    int m1F0;
    int m1F4;
    int m1F8;
    int m1FC;
    AABBTree::Handle m200; // inferred: entry of the model in a map tree (set by MapPartsModel::vf24)
    boost::scoped_ptr<ModelCamera_unk> m208; // inferred
    int m20C_31_24 : 8; // inferred: load state (8 = ready)
    int m20C_23_16 : 8; // inferred: LOD counter (gmk::SwingObject::vf10 restores m50 from it)
    unsigned int m20C_15_8 : 8; // inferred: saved copy of m50 (CharaTask::fn_80145334)
    unsigned int m20C_7 : 1;
    int mFlag6_unk : 1; // inferred: tested by gmk::SwingObject::vf10
    int mFlag5_unk : 1;          // inferred: tested by CharaTask::fn_8013655C
    int mFlag4_unk : 1; // inferred: set together with the equipment
    int mFlag3_unk : 1; // inferred: the textures of the materials are not bound (fn_8008B978)
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
    // inferred: atn::Model declares its own accessor of the matrix block (second weak copy 0x800952F8, used
    // by fn_80095300; defined in atn/Model.cpp)
    inline Mtx_unk* GetMtx_unk();
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
    void fn_8008C828(MaterialSet_unk* list);       // inferred: sets the materials in use
    void fn_80097750(atn::Vector3& dir, float a, float b, int c); // inferred: direction parameters
    void fn_8008C850(MaterialSet_unk* list);       // inferred: flags from the materials
    void fn_80094D88(int arg, int flag, int a, int b, int c); // inferred: material mode of every material
    void fn_80094BD8(int mode); // inferred: blend mode of every material
    void fn_80092A4C();                          // inferred
    void fn_8008CD60();                          // inferred
    void fn_800954DC(void* owner, atn::Vector2_unk uv, float f); // inferred: UV scroll
    int fn_80097E80(int arg); // inferred: returns nonzero when the motion was updated (CharaTask::fn_80145334)
    void fn_8009385C(unsigned int hash, bool show, int children, int siblings); // inferred: shows/hides a node tree
    void fn_80093A1C(int node, bool show, int children, int siblings);           // inferred: same from a node index
    int fn_800928B0(unsigned int hash, int arg);      // inferred: node index by name hash, < 0 if missing

    // inferred: matrix of node `index`, null for a missing node (inlined at 0x8049CCD4)
    MtxPtr GetNodeMtx_unk(int index) const {
        return (index < 0) ? 0 : mMtx.mNodes.mData[index].m;
    }
    // inferred: matrix of the node with name hash `hash` (weak copy 0x80012C3C, Event/Puppeteer_CharaTask);
    // name invented
    MtxPtr GetNodeMtxByHash_unk(unsigned int hash) { return GetNodeMtx_unk(fn_800928B0(hash, 0)); }
    // inferred: matrix of the node called `name` (weak copy 0x800132EC); name invented
    MtxPtr GetNodeMtx_unk(const char* name) const {
        return GetNodeMtx_unk(GetNodeNoByName(name, 0));
    }

    // inferred: also the motion controller characters embed at +0xB0 (chr::Motion_unk is an alias:
    // CharaTask constructs it with this constructor, 0x80135588)
    void fn_80097C08(int arg1, int motion, int arg3, int arg4, int arg5, float blend, float speed); // inferred: play motion
    float fn_80097D7C(int track); // inferred: motion length
    // inferred: called on load by CharaTask (names keep the address)
    void fn_80097A88(int motion, const char* path); // inferred: loads a motion
    void fn_80097A20(unsigned int motion);          // inferred: releases a motion slot
    void fn_80097A9C(int motion, const LH_Motion* src); // inferred: copies a motion slot
    int fn_80097B34(); // inferred: number of loaded motions
    void fn_80097CCC(int track, float blend); // inferred: stops the motion of a track
    void fn_80093014(int arg);
    void fn_8009373C(const char* name, int part); // inferred: shows/hides the named node (option part)
    void fn_80094958(const char* name, const void* a, const void* b, const void* c); // inferred: material edit
    void fn_80093D98(const char* name, int arg); // inferred: material hide
    void fn_80093F58(const void* edit); // inferred: node edit
    // inferred: node edit of `name` (offsets a, b and scale c); flags as fn_80089D70
    void fn_80093F1C(const char* name, const atn::Vector3& a, const atn::Vector3& b, const atn::Vector3& c, int flags);
    void fn_80094778(const char* name); // inferred: removes the node edit of `name`
    void fn_800973C0(CsvTab* csv); // inferred: adds a physics bone from a CSV line
    void fn_80095D44(int node, int parent);
    // inferred: adds the hashes of the resources of the model to `list` (CharaTask::fn_8017AD44)
    void fn_80096218(SimpleVector<unsigned int>* list, void* arg);
    void fn_80091CFC(const char* node);
    void fn_80099E9C(const char* node, const char* name, float value);
    void fn_80097510(); // inferred: resets the physics bones
    void fn_8009757C(); // inferred: wind on the physics bones
    void fn_80097D9C(Model* src); // inferred: copies the node poses of src by node name
    atn::LoadHandle* fn_80097D40(int motion); // inferred: handle of the motion (12-byte entries of m224), zero if missing

    // inferred: per-node data (0x2C bytes)
    // inferred: node pose of the model (0x2C bytes): a MotionPose_unk (its constructor is called by the weak
    // constructor 0x8008A4B0); m00 flags (1 position, 2 rotation, 4 scale; CharaTask::fn_80148CC4)
    struct Node_unk : public MotionPose_unk {
        inline Node_unk(); // inferred: defined in atn/Model.cpp
    };
    Res_unk* fn_8000DD04(); // inferred: returns the loaded data of the first level
    typedef Node_unk Entry_unk;
    // inferred: motion track (0x30 bytes, six of them at 0x22C; accessors 0x80139550-0x8013C554)
    struct Track_unk {
        int m00;   // inferred: motion id
        LH_Motion* m04; // inferred: motion played (kept by CharaTask::m518 to detect a change, fn_80145334)
        float m08; // inferred: frame
        float m0C; // inferred: speed
        float m10; // inferred: set to 1.0 when CharaTask::mFlag7 is set (fn_80145334)
        float m14; // inferred: weight (set by the lip sync, Chara1154_unk::fn_8007A7BC)
        unsigned char m18; // inferred: nonzero = the end is tested one step early (CharaTask::fn_8015EB2C)
        unsigned char m19[0x1C - 0x19];
        float m1C; // inferred: current value (moved towards the key by the lip sync)
        float m20; // inferred: weight
        LH_Motion* m24; // inferred: previous motion (m04)
        float m28; // inferred: previous frame (m08)
        float m2C; // inferred: previous m10
    };
    // inferred: replaces the node edits and rebuilds the node -> edit table mE4
    void fn_80094420(const SimpleVector<NodeEdit_unk>& edits);
    // inferred: samples the motion of a track on a node and its children (fn_80098DD8)
    void fn_80098C90(int node, int siblings, int replace, Res_unk* res, Track_unk* track, const int* remap);
    // inferred: advances the motion tracks without sampling them (fn_80097E80)
    void fn_80098698();
    // inferred: enables the node callback of a node (by hash / by name; the list mCC is kept sorted)
    void fn_800920B8(unsigned int hash);
    // inferred: samples the fading track 3 (fn_80097E80)
    void fn_80098DD8(int advance);

    // inferred: accessors called out of line from CharaTask (names keep the address)
    Node_unk* fn_80147A00();
    typedef atn::Sphere Sphere_unk;
    Sphere_unk fn_80094F98(); // inferred: bounding sphere (passed to MapManager::FindArea)
    AABB_unk fn_80095300();   // inferred: bounding box in world space
    float GetMotionFrame(int track) const; // 0x80139550
    void fn_80139F24(int value);
    int fn_80139F2C(int track);
    void SetMotionSpeed(int track, float speed); // 0x80139F3C
    void SetMotionFrame(int track, float frame); // 0x80357F38 (defined in game/EnemyTasha.cpp)
    int fn_8013C3DC();
    float fn_8013C3E4(int track);
    void fn_8013C554(int track, float weight);
    float GetMotionSpeed(int track) const { return mTracks[track].m0C; } // weak copy 0x80224628

    SimpleArray<NodeWork_unk> m214;  // inferred: 18-byte per-node work entries (cleared by their ctor 0x800979E8)
    SimpleArray<Node_unk> m21C;      // inferred: per-node data
    SimpleArray<MotionSlot_unk> m224; // inferred: motion slots (mSize: NpcManager::fn_80547B54)
    Track_unk mTracks[6]; // 0x22C (track 0: frame at 0x234, speed at 0x238)
    int m34C;
    float m350; // inferred: time scale of the character (multiplied with the frame time, CharaTask::fn_8013CB68)
    SimpleArray<MotionCmd_unk> m354; // inferred: motion commands, one 16-byte entry per motion slot
    // inferred: second node pose callback (set by fn_8031F2A4 of game/Em401_01_WhiteLeoPard.cpp, used by the
    // white leopard and the war lion)
    boost::function<void(atn::MotionBuffer*, int)> m35C;
    // inferred: callback run on the node poses (set by CharaFaceControl::Initialize)
    boost::function<void(atn::MotionBuffer*, int)> m370;
    unsigned int m384;          // inferred: number of bones in m388
    PBoneInterface* m388[8];    // inferred: physics bones (deleted by the destructor)
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
