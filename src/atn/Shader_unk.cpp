// atn/Shader_unk.cpp: shader instances of the models (file name invented). A shader (Shader_unk,
// built by fn_800C41D0 from the material description of a model) keeps its textures sorted by id;
// fn_800C4A90 creates the GPU material from it; ShaderLOD_unk keeps the shaders of a material by
// distance. The shader manager that owns them is atn/ShaderManager_unk.cpp.
#include "atn/Shader_unk.h"
#include "atn/Memory.h"
#include "atn/GXDecl.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: debug message output (lbl_8087EEB8)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;


// inferred: texture stage of a material (0x18 bytes; engine code of atn/MaterialParser_unk:
// constructors 0x8007E9EC and 0x8007AA30, destructor 0x8007AA64)
struct TexStage_unk {
    unsigned char m00[0x18];
};
extern "C" TexStage_unk* fn_8007E9EC(TexStage_unk* stage);
extern "C" TexStage_unk* fn_8007AA30(TexStage_unk* stage, int id);
extern "C" TexStage_unk* fn_8007AA64(TexStage_unk* stage, int del);
extern "C" void fn_8007AB38(TexStage_unk* stage, atn::ShaderDesc_unk* desc, int index); // inferred: setup
extern "C" void fn_8007C018(TexStage_unk* stage, void* texture, int arg);
extern "C" void fn_8007B100(TexStage_unk* stage);
extern "C" int fn_8007B078(const char* name); // inferred: texture id by name, -1 if unknown
extern "C" void* fn_80081C34(int id);         // inferred: texture by id
extern "C" void* __construct_new_array(void* block, void* ctor, void* dtor, unsigned long size, unsigned long n);

// inferred: GPU material created from a shader (0x54 bytes, ctor 0x8007C144); probably the
// ShaderMaterial_unk of include/atn/Model_unk.h
struct ShaderMaterial_unk {
    unsigned int m00; // inferred: name hash
    unsigned int m04;
    unsigned char m08[4];
    TexStage_unk* m0C;
    unsigned char m10[4];
    unsigned int m14_0 : 1;
    unsigned int m14_1 : 3;
    unsigned int m14_4 : 8;
    unsigned int m14_12 : 1;
    unsigned int m14_13 : 6;
    unsigned int m14_19 : 1;
    unsigned int m14_20 : 1;
    unsigned int m14_21 : 1;
    unsigned int m14_22 : 3;
    unsigned int m14_25 : 1;
    unsigned int m14_26 : 6;
    GXColor m18;
    GXColor m1C;
    GXColor m20;
    unsigned char m24[12];
    unsigned char m30[8];
    unsigned char m38[4];
    int m3C;
    unsigned char m40[0x4A - 0x40];
    unsigned char m4A; // inferred: number of texture stages
    unsigned char m4B;
    signed char m4C;
    unsigned char m4D;
    unsigned char m4E;
    unsigned char m4F;
    unsigned char m50;
    unsigned char m51;
    unsigned char m52[2];
};
extern "C" ShaderMaterial_unk* fn_8007C144(ShaderMaterial_unk* mat, struct ShaderTex_unk* shader);
extern "C" int fn_800D1C60(int mode); // inferred: converts a blend mode
extern "C" void fn_80081D00(ShaderMaterial_unk* mat);

// inferred: debug parameter menu (gmk/SwingObject.cpp)
extern "C" void* fn_8008937C(void* parent, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

// inferred: std::less of the entries (the comparison is an operator< of the entry)
struct ShaderTexLess_unk {
    bool operator()(const ShaderTexEntry_unk& a, const ShaderTexEntry_unk& b) const { return a < b; }
};

template <class T>
inline void SwapEntry_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// inferred: median of three of the quicksort below (MSL __sort)
template <class T, class Compare>
void Median3_unk(T* a, T* b, T* c, Compare& comp) {
    bool ca = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        SwapEntry_unk(*a, *b);
    } else {
        if (comp(*b, *a)) {
            SwapEntry_unk(*a, *b);
        }
        if (ca) {
            SwapEntry_unk(*b, *c);
        } else {
            SwapEntry_unk(*a, *c);
        }
    }
}

template <class T, class Compare>
inline T* MinElement_unk(T* first, T* last, Compare& comp) {
    T* result = first;
    if (first != last) {
        for (T* it = first + 1; it != last; it++) {
            if (comp(*it, *result)) {
                result = it;
            }
        }
    }
    return result;
}

static int sSortOffset = -4; // inferred: pivot offset of the quicksort, -4 to 4

// NONMATCHING: 90.3% - quicksort of the textures (MSL __sort, same reconstruction as game/CharaTask
// Sort_unk): the original materializes the negated comparisons of the partition loops as bools
// (xor/srawi/and) and reloads the pivot key; tried std::less-like comparator with operator< of the
// entry, int keys.
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare& comp) {
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            if (first != last) {
                for (T* end = last - 1; first != end; first++) {
                    T* min = MinElement_unk(first, last, comp);
                    if (min != first) {
                        SwapEntry_unk(*min, *first);
                    }
                }
            }
            return;
        }
        T* a = first + (n / 4 + sSortOffset % 5);
        if (++sSortOffset >= 5) {
            sSortOffset = -4;
        }
        T* b = first + (n * 3 / 4 + sSortOffset % 5);
        if (++sSortOffset >= 5) {
            sSortOffset = -4;
        }
        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        T* i = first;
        j = pivot;
        while (comp(*i, *pivot)) {
            i++;
        }
        while (i != --j && !comp(*j, *pivot)) {
        }
        if (i < j) {
            SwapEntry_unk(*i, *j);
            i++;
            while (true) {
                while (comp(*i, *pivot)) {
                    i++;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                SwapEntry_unk(*i, *j);
                i++;
            }
        }
        if (i == first) {
            T* back = last - 1;
            SwapEntry_unk(*i, *pivot);
            i++;
            if (!comp(*first, *back)) {
                while (i != last && !comp(*first, *i)) {
                    i++;
                }
                if (i < back) {
                    SwapEntry_unk(*i, *back);
                }
            }
            j = back;
            if (i < j) {
                while (true) {
                    while (!comp(*first, *i)) {
                        i++;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    SwapEntry_unk(*i, *j);
                    i++;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            Sort_unk(first, i, comp);
            first = i;
        } else {
            Sort_unk(i, last, comp);
            last = i;
        }
    }
}

// NONMATCHING: 98.0% - the original loads desc->mA48 before the member stores and allocates the
// loop registers in another order (index r24, name/stage r27); tried locals declared before the loop.
ShaderTex_unk::ShaderTex_unk(void* model, atn::ShaderDesc_unk* desc) : mDesc(desc) {
    mTextures.Resize_unk(desc->mA48, 6);
    mOrder.Resize_unk(mDesc->mA48, 6);
    const char* name;
    for (unsigned int i = 0; i < mTextures.mSize; i++) {
        name = mDesc->mA4C[i];
        int id = fn_8007B078(name);
        if (id == -1) {
            char buf[0x100];
            sprintf(buf, "Shader '%s' has invalid texture '%s'\n", mDesc->m010, name);
            buf[0xFF] = 0;
            lbl_8087EEB8->fn_800697D8(buf);
        }
        ShaderTexEntryData_unk entry = {id, i, 0};
        void* texture = fn_80081C34(id);
        if (texture != 0) {
            TexStage_unk* bind = (TexStage_unk*)operator new(sizeof(TexStage_unk), 6, "", "", 0);
            if (bind != 0) {
                bind = fn_8007AA30(bind, -1);
            }
            entry.mBind = bind;
            fn_8007AB38(bind, mDesc, i);
            fn_8007C018(bind, texture, 1);
            fn_8007B100(bind);
        }
        static_cast<ShaderTexEntryData_unk&>(mTextures.mData[i]) = entry;
    }
    ShaderTexLess_unk comp;
    Sort_unk(mTextures.mData, mTextures.mData + mTextures.mSize, comp);
    for (unsigned int i = 0; i < mTextures.mSize; i++) {
        mOrder.mData[mTextures.mData[i].mIndex] = i;
    }
}

ShaderTex_unk::~ShaderTex_unk() {}

// inferred: creates the GPU material of a shader
ShaderMaterial_unk* fn_800C4A90(ShaderTex_unk* shader) {
    ShaderMaterial_unk* mat = (ShaderMaterial_unk*)operator new(sizeof(ShaderMaterial_unk), 6, "", "", 0);
    if (mat != 0) {
        mat = fn_8007C144(mat, shader);
    }
    mat->m00 = fn_800DC6B4(shader->mDesc->m010);
    mat->m04 = fn_800DC6B4(shader->mDesc->m010);
    mat->m18 = shader->mDesc->m014;
    mat->m20 = shader->mDesc->m01C;
    mat->m1C = shader->mDesc->m018;
    for (int i = 0; i < 12; i++) {
        mat->m24[i] = shader->mDesc->m024[i];
    }
    mat->m14_20 = shader->mDesc->m07C != -1;
    mat->m14_19 = shader->mDesc->m064 == 2;
    mat->m4C = -1;
    mat->m4A = shader->mDesc->m0E4;
    int n = shader->mDesc->m0E4;
    mat->m0C = (TexStage_unk*)__construct_new_array(operator new[](n * sizeof(TexStage_unk) + 0x10, 6, "", "", 0),
                                                   fn_8007E9EC, fn_8007AA64, sizeof(TexStage_unk), n);
    for (int i = 0; i < mat->m4A; i++) {
        fn_8007AB38(&mat->m0C[i], shader->mDesc, i);
    }
    mat->m4E = shader->mDesc->m090;
    mat->m4F = shader->mDesc->m0A8;
    mat->m50 = shader->mDesc->m08C;
    mat->m51 = shader->mDesc->m0A4;
    mat->m38[0] = shader->mDesc->m18C[0];
    mat->m38[1] = shader->mDesc->m18C[1];
    mat->m38[2] = shader->mDesc->m18C[2];
    mat->m38[3] = shader->mDesc->m18C[3];
    mat->m3C = shader->mDesc->m054 / 255.0f;
    mat->m14_12 = shader->mDesc->mA38 == 1;
    mat->m14_0 = fn_800D1C60(shader->mDesc->mA14) != 7;
    mat->m14_4 = shader->mDesc->mA18;
    mat->m14_1 = fn_800D1C60(shader->mDesc->mA14);
    mat->m14_21 = shader->mDesc->mA28;
    mat->m14_22 = fn_800D1C60(shader->mDesc->mA2C);
    mat->m14_25 = shader->mDesc->mA30;
    fn_80081D00(mat);
    return mat;
}

ShaderLOD_unk::ShaderLOD_unk() : m00(0) {}

ShaderLOD_unk::~ShaderLOD_unk() {}

void ShaderLOD_unk::fn_800C4ECC(ShaderTex_unk* shader, float distance) {
    ShaderLODEntry_unk* it;
    for (it = mLODs.begin(); it != mLODs.end(); it++) {
        if (distance < it->mDistance) {
            break;
        }
    }
    ShaderLODEntry_unk entry;
    entry.mShader = shader;
    entry.mDistance = distance;
    mLODs.insert(it, entry);
}

void ShaderLOD_unk::fn_800C4F38(ShaderMaterial_unk** out) const {
    *out = fn_800C4A90(mLODs.mData[0].mShader);
}

void ShaderLOD_unk::fn_800C4F70(void* parent) {
    void* group = fn_8008937C(parent, mLODs.begin()->mShader->mDesc->m010);
    fn_800874C8(group, "RefCount", &m00, 0, 10000000, 0, 0, 0);
    for (unsigned int i = 0; i < mLODs.size(); i++) {
        fn_8008771C(group, mLODs[i].mShader->mDesc->m010, &mLODs[i].mDistance, 0, 0, 0.0f, 10000.0f, 1.0f);
    }
    std::vector<int> unused;
}
