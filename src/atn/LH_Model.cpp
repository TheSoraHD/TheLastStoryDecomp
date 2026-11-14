#include "atn/LH_Model.h"
#include "atn/ModelPose_unk.h"
#include "atn/Array_unk.h"
#include "atn/LoadManager.h"
#include "atn/Loader_unk.h"
#include "math/Vec.h"

extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" char* strcat(char* dst, const char* src);
#include "atn/GraphicsDevice.h"
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: graphics device

// ModelNode_unk, ModelJoint_unk, ModelData_unk, ModelPose_unk: atn/ModelPose_unk.h

namespace atn {

// inferred from __vt__Q23atn9LoadModel (0x8078FDD8)
class LoadModel : public LoadCore {
public:
    LoadModel(const char* path) : LoadCore(path), m5C(0), m60(0), m64(0), m78(0) {}
    virtual ~LoadModel();
    virtual void vf0C();

    // inferred: callback of the chunk reader; keeps the chunks it needs
    static void ChunkCallback_unk(void* user, const ChunkHeader* chunk, void* data) {
        LoadModel* self = (LoadModel*)user;
        if (chunk->mTag == 'modl') {
            self->m5C = data;
        }
        if (chunk->mTag == 'omdl') {
            self->m60 = data;
        } else if (chunk->mTag == 'occt') {
            self->m64 = data;
        }
    }
    // inferred: vertex arrays of a mesh of version 1 (fn_8047614C; 0x144 bytes)
    struct MeshArrays_unk {
        DrawArray_unk mList[27];
    };
    void fn_80474F0C(ModelData_unk* model);

    void* m5C;                         // 'modl' chunk
    void* m60;                         // 'omdl' chunk
    void* m64;                         // 'occt' chunk
    SimpleArray<Matrix3x4> mMatrices;   // 0x68: inverse joint matrices
    SimpleArray<unsigned short> mJoints; // 0x70: joints that have m70 > 0
    int m78;
    SimpleArray<int> mTextures;         // 0x7C: inferred: vertex formats of the meshes (fn_80076C44)
    SimpleArray<MeshArrays_unk> m84;    // 0x84
};

LoadModel::~LoadModel() {
    for (unsigned int i = 0; i < mTextures.mSize; i++) {
        fn_80076F88(lbl_8087EEE0, mTextures.mData[i]);
    }
}

void LoadModel::vf0C() {
    if (mState == 2) {
        fn_80473104(3);
        void* data = m54;
        Loader_unk loader;
        fn_804714A4(&loader);
        fn_804714B0(&loader, data, m50, LoaderCallback_unk(&ChunkCallback_unk), this);
        ModelData_unk* model = (ModelData_unk*)m5C;
        if (model != 0) {
            mMatrices.Resize_unk(model->m44, 6);
            ModelPose_unk pose(model, mMatrices.mData, 0, boost::function<void()>(), 0, 0);
            Matrix3x4 mtx;
            mtx.SetIdentity();
            pose.fn_8009A584(mtx);
            for (unsigned int i = 0; i < mMatrices.mSize; i++) {
                PSMTXInverse(mMatrices.mData[i].m, mMatrices.mData[i].m);
            }
            int count = 0;
            for (int i = 0; i < ((ModelData_unk*)m5C)->m44; i++) {
                if (((ModelData_unk*)m5C)->m48[i]->m70 > 0) {
                    count++;
                }
            }
            if (count > 0) {
                mJoints.Resize_unk(count, 6);
                int n = 0;
                for (int i = 0; i < ((ModelData_unk*)m5C)->m44; i++) {
                    if (((ModelData_unk*)m5C)->m48[i]->m70 > 0) {
                        mJoints[n++] = i;
                    }
                }
            }
            fn_80474F0C(model);
            for (int i = 0; i < ((ModelData_unk*)m5C)->m3C; i++) {
                ModelNode_unk* node = ((ModelData_unk*)m5C)->m40[i];
                if (node->m24 != -1 || node->m28 != -1) {
                    m78 = 1;
                    break;
                }
            }
        }
    }
}

// inferred: GX component type and size in bytes of each component type of the file (0 s8, 1 s16,
// 2 u8, 3 u16, 4 f32, 5 rgb8, 6 rgba8, 7 rgb565, 8 rgba4, 9 rgba6, 10 rgbx8)
static const int sTypeGX_unk[11] = {1, 3, 0, 2, 4, 1, 5, 0, 3, 4, 2};
static const int sTypeSize_unk[11] = {1, 2, 1, 2, 4, 3, 4, 2, 2, 3, 4};

// inferred: builds the vertex format of each mesh (shared through the cache of the graphics device)
// and points its vertex arrays to the streams of the file: the slots of the list at m54 from
// version 2, a list of its own (m84) before
// NONMATCHING: 98.55% - stack slots of the aggregate temporaries (the original puts the two list
// terminators right after the initial ones), one mulli scheduled in the normal block of version 1 and
// the load of lbl_8087EEE0 before the VtxFormat_unk temporary
void LoadModel::fn_80474F0C(ModelData_unk* model) {
    mTextures.Resize_unk(model->m3C, 6);
    for (int i = 0; i < model->m3C; i++) {
        VtxDescArray_unk desc;
        VtxAttrFmtArray_unk fmt;
        for (int k = 0; k < 27; k++) {
            GXVtxAttrFmtList f = {0xFF, 0, 0, 0};
            fmt.mList[k] = f;
        }
        for (int k = 0; k < 27; k++) {
            desc.mList[k].attr = 0xFF;
            desc.mList[k].type = 0;
        }
        ModelNode_unk* mesh = model->m40[i];
        if (mesh->m04 >= 2) {
            int n = 0;
            if (mesh->m1C != -1) {
                mesh->m54[n].m04 = model->m2C[mesh->m1C]->mData;
                n++;
            }
            if (mesh->m20 != -1) {
                mesh->m54[n].m04 = model->m2C[mesh->m20]->mData;
                n++;
            }
            for (int k = 0; k < 2; k++) {
                if ((&mesh->m24)[k] != -1) {
                    mesh->m54[n].m04 = model->m2C[(&mesh->m24)[k]]->mData;
                    n++;
                }
            }
            for (int k = 0; k < 8; k++) {
                if (mesh->m2C[k] != -1) {
                    mesh->m54[n].m04 = model->m2C[mesh->m2C[k]]->mData;
                    n++;
                }
            }
        } else {
            if (i == 0) {
                m84.Resize_unk(model->m3C, 6);
            }
            DrawArray_unk* arrays = m84[i].mList;
            for (int k = 0; k < 27; k++) {
                DrawArray_unk a = {0xFF, 0, 0};
                arrays[k] = a;
            }
            int n = 0;
            if (mesh->m1C != -1) {
                LoadVtxStream_unk* stream = model->m2C[mesh->m1C];
                LoadVtxAttr_unk* attr = stream->mDesc->mAttrs;
                int type = attr->mType;
                GXVtxAttrFmtList f = {9, 1, sTypeGX_unk[type], attr->mFrac};
                fmt.mList[n] = f;
                DrawArray_unk a = {9, stream->mData, sTypeSize_unk[type] * 3};
                arrays[n] = a;
                n++;
            }
            if (mesh->m20 != -1) {
                LoadVtxStream_unk* stream = model->m2C[mesh->m20];
                LoadVtxAttr_unk* attr = stream->mDesc->mAttrs;
                int type = attr->mType;
                GXVtxAttrFmtList f = {10, 0, sTypeGX_unk[type], attr->mFrac};
                fmt.mList[n] = f;
                DrawArray_unk a = {10, stream->mData, sTypeSize_unk[type] * 3};
                arrays[n] = a;
                n++;
            }
            for (int k = 0; k < 2; k++) {
                int s = (&mesh->m24)[k];
                if (s != -1) {
                    LoadVtxStream_unk* stream = model->m2C[s];
                    LoadVtxAttr_unk* attr = stream->mDesc->mAttrs;
                    int type = attr->mType;
                    GXVtxAttrFmtList f = {k + 11, 1, sTypeGX_unk[type], attr->mFrac};
                    fmt.mList[n] = f;
                    DrawArray_unk a = {k + 11, 0, 0};
                    a.m04 = stream->mData;
                    a.m08 = sTypeSize_unk[type];
                    arrays[n] = a;
                    n++;
                }
            }
            for (int k = 0; k < 8; k++) {
                if (mesh->m2C[k] != -1) {
                    LoadVtxStream_unk* stream = model->m2C[mesh->m2C[k]];
                    LoadVtxAttr_unk* attr = stream->mDesc->mAttrs;
                    int type = attr->mType;
                    GXVtxAttrFmtList f = {k + 13, 1, sTypeGX_unk[type], attr->mFrac};
                    fmt.mList[n] = f;
                    DrawArray_unk a = {k + 13, 0, 0};
                    a.m04 = stream->mData;
                    a.m08 = sTypeSize_unk[type] * 2;
                    arrays[n] = a;
                    n++;
                }
            }
            DrawArray_unk a = {0xFF, 0, 0};
            arrays[n] = a;
        }
        LoadVtxDesc_unk* vd = mesh->m18;
        for (int k = 0; k < vd->mCount; k++) {
            LoadVtxAttr_unk* a = &vd->mAttrs[k];
            int type = 3;
            if (a->mType == 2) {
                type = 2;
            }
            switch (a->mKind) {
            case 4:
                desc.mList[k].attr = 0;
                desc.mList[k].type = 1;
                break;
            case 5:
                desc.mList[k].attr = a->mIndex + 1;
                desc.mList[k].type = 1;
                break;
            case 0:
                desc.mList[k].attr = 9;
                desc.mList[k].type = type;
                break;
            case 1:
                desc.mList[k].attr = 10;
                desc.mList[k].type = type;
                break;
            case 2:
                desc.mList[k].attr = a->mIndex + 13;
                desc.mList[k].type = type;
                break;
            case 3:
                desc.mList[k].attr = a->mIndex + 11;
                desc.mList[k].type = type;
                break;
            }
        }
        int n = 0;
        if (mesh->m1C != -1) {
            LoadVtxAttr_unk* attr = model->m2C[mesh->m1C]->mDesc->mAttrs;
            GXVtxAttrFmtList f = {9, 1, sTypeGX_unk[attr->mType], attr->mFrac};
            fmt.mList[n] = f;
            n++;
        }
        if (mesh->m20 != -1) {
            LoadVtxAttr_unk* attr = model->m2C[mesh->m20]->mDesc->mAttrs;
            GXVtxAttrFmtList f = {10, 0, sTypeGX_unk[attr->mType], attr->mFrac};
            fmt.mList[n] = f;
            n++;
        }
        for (int k = 0; k < 2; k++) {
            int s = (&mesh->m24)[k];
            if (s != -1) {
                LoadVtxAttr_unk* attr = model->m2C[s]->mDesc->mAttrs;
                GXVtxAttrFmtList f = {k + 11, 1, sTypeGX_unk[attr->mType], attr->mFrac};
                fmt.mList[n] = f;
                n++;
            }
        }
        for (int k = 0; k < 8; k++) {
            if (mesh->m2C[k] != -1) {
                LoadVtxAttr_unk* attr = model->m2C[mesh->m2C[k]]->mDesc->mAttrs;
                GXVtxAttrFmtList f = {k + 13, 1, sTypeGX_unk[attr->mType], attr->mFrac};
                fmt.mList[n] = f;
                n++;
            }
        }
        GXVtxAttrFmtList f = {0xFF, 0, 0, 0};
        fmt.mList[n] = f;
        desc.mList[vd->mCount].attr = 0xFF;
        desc.mList[vd->mCount].type = 0;
        mTextures[i] = fn_80076C44(lbl_8087EEE0, VtxFormat_unk(desc, fmt));
    }
}

void* LH_Model::fn_80475DF8() {
    if (mCore != 0) {
        return ((LoadModel*)mCore)->m5C;
    }
    return 0;
}

void LH_Model::vf0C(const char* path) {
    char buf[0x100];
    const char* ext = strstr(path, ".");
    if (ext == 0) {
        int len = strlen(path);
        memcpy(buf, path, len);
        path = buf;
        buf[len] = '\0';
        strcat(buf, ".model");
    } else if (strcmp(ext, ".model") != 0 && strcmp(ext, ".shader") != 0) {
        int len = ext - path;
        memcpy(buf, path, len);
        path = buf;
        buf[len] = '\0';
        strcat(buf, ".model");
    }
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadModel>(path, 1);
}

} // namespace atn

// Accessors of the loaded model, called as free functions (atn/ModelDraw_unk, atn/LayerOcclusionCPU...).
// inferred: inverse joint matrices
extern "C" atn::Matrix3x4* fn_80476130(atn::LH_Model* model) {
    if (model->mCore != 0) {
        return ((atn::LoadModel*)model->mCore)->mMatrices.mData;
    }
    return 0;
}

// inferred: vertex arrays of mesh `mesh` (0x144 bytes each)
extern "C" void* fn_8047614C(atn::LH_Model* model, int mesh) {
    if (model->mCore != 0) {
        return (char*)((atn::LoadModel*)model->mCore)->m84.mData + mesh * 0x144;
    }
    return 0;
}

// inferred: vertex description of mesh `mesh` (index into a table of the texture manager)
extern "C" void* fn_80476170(atn::LH_Model* model, int mesh) {
    int index = ((int*)((atn::LoadModel*)model->mCore)->mTextures.mData)[mesh];
    return *(void**)((char*)lbl_8087EEE0 + index * 8 + 0xE0);
}

// inferred: 'omdl' chunk (occlusion meshes)
extern "C" void* fn_80476194(atn::LH_Model* model) {
    if (model->mCore != 0) {
        return ((atn::LoadModel*)model->mCore)->m60;
    }
    return 0;
}

// inferred: 'occt' chunk (occlusion tree)
extern "C" void* fn_804761B0(atn::LH_Model* model) {
    if (model->mCore != 0) {
        return ((atn::LoadModel*)model->mCore)->m64;
    }
    return 0;
}

// inferred: joints with a parent (m70 > 0)
extern "C" unsigned short* fn_804761CC(atn::LH_Model* model) {
    if (model->mCore != 0) {
        return ((atn::LoadModel*)model->mCore)->mJoints.mData;
    }
    return 0;
}

// inferred: number of those joints
extern "C" int fn_804761E8(atn::LH_Model* model) {
    if (model->mCore != 0) {
        return ((atn::LoadModel*)model->mCore)->mJoints.mSize;
    }
    return 0;
}

extern "C" int fn_80476204(atn::LH_Model* model) {
    if (model->mCore != 0) {
        return ((atn::LoadModel*)model->mCore)->m78;
    }
    return 0;
}
