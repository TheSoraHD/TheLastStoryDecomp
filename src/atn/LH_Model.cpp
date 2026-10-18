#include "atn/LH_Model.h"
#include "atn/Array_unk.h"
#include "atn/LoadManager.h"
#include "atn/Loader_unk.h"
#include "math/Vec.h"

extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" char* strcat(char* dst, const char* src);
extern "C" void fn_80076F88(void* manager, void* texture); // inferred: releases a texture
extern "C" void* lbl_8087EEE0;                              // inferred: texture manager

// inferred: node of the model data
struct ModelNode_unk {
    unsigned char m00[0x24];
    int m24;
    int m28;
};
struct ModelJoint_unk {
    unsigned char m00[0x70];
    int m70;
};

// inferred: model data of the loaded file (the 'modl' chunk)
struct ModelData_unk {
    unsigned char m00[0x3C];
    int mNumNodes;             // 0x3C
    ModelNode_unk** mNodes;    // 0x40
    int mNumJoints;            // 0x44
    ModelJoint_unk** mJoints;  // 0x48
};

// inferred: pose of a model, built on the stack to compute the joint matrices
// (constructor 0x800904E0, fn_8009A584); its two callbacks are destroyed inline
struct ModelPose_unk {
    ModelPose_unk(ModelData_unk* model, atn::Matrix3x4* matrices, int a, boost::function<void()> callback,
                  int b, int c);
    void fn_8009A584(const atn::Matrix3x4& mtx);

    unsigned char m00[0x18];
    boost::function<void()> m18;
    boost::function<void()> m2C;
    unsigned char m40[0x58 - 0x40];
};

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
    // NONMATCHING: not decompiled yet (0x80474F0C)
    void fn_80474F0C(ModelData_unk* model);

    void* m5C;                         // 'modl' chunk
    void* m60;                         // 'omdl' chunk
    void* m64;                         // 'occt' chunk
    Buffer_unk<Matrix3x4> mMatrices;   // 0x68: inverse joint matrices
    Buffer_unk<unsigned short> mJoints; // 0x70: joints that have m70 > 0
    int m78;
    Buffer_unk<void*> mTextures;       // 0x7C
    Buffer_unk<int> m84;
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
            mMatrices.Resize_unk(model->mNumJoints, 6);
            ModelPose_unk pose(model, mMatrices.mData, 0, boost::function<void()>(), 0, 0);
            Matrix3x4 mtx;
            mtx.SetIdentity();
            pose.fn_8009A584(mtx);
            for (unsigned int i = 0; i < mMatrices.mSize; i++) {
                PSMTXInverse(mMatrices.mData[i].m, mMatrices.mData[i].m);
            }
            int count = 0;
            for (int i = 0; i < ((ModelData_unk*)m5C)->mNumJoints; i++) {
                if (((ModelData_unk*)m5C)->mJoints[i]->m70 > 0) {
                    count++;
                }
            }
            if (count > 0) {
                mJoints.Resize_unk(count, 6);
                int n = 0;
                for (int i = 0; i < ((ModelData_unk*)m5C)->mNumJoints; i++) {
                    if (((ModelData_unk*)m5C)->mJoints[i]->m70 > 0) {
                        mJoints[n++] = i;
                    }
                }
            }
            fn_80474F0C(model);
            for (int i = 0; i < ((ModelData_unk*)m5C)->mNumNodes; i++) {
                ModelNode_unk* node = ((ModelData_unk*)m5C)->mNodes[i];
                if (node->m24 != -1 || node->m28 != -1) {
                    m78 = 1;
                    break;
                }
            }
        }
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
