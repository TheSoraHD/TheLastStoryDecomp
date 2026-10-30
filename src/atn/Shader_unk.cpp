// atn/Shader_unk.cpp: shader instances of the models (file name invented). A shader (Shader_unk,
// built by fn_800C41D0 from the material description of a model) keeps its textures sorted by id;
// fn_800C4A90 creates the GPU material from it; ShaderLOD_unk keeps the shaders of a material by
// distance. The shader manager that owns them is atn/ShaderManager_unk.cpp.
#include "atn/Shader_unk.h"
#include "atn/Memory.h"
#include "atn/GXDecl.h"
#include "atn/Material_unk.h"
#include "std/new.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: debug message output (lbl_8087EEB8)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;


extern "C" int fn_8007B078(const char* name);         // inferred: texture id by name, -1 if unknown
extern "C" const char* fn_80081C34(int id);         // inferred: path of the default texture of an id
extern "C" int fn_800D1C60(int mode); // inferred: converts a blend mode

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

// NONMATCHING: 98.3% - the original loads desc->mA48 before the member stores and allocates the
// loop registers in another order; tried locals declared before the loop (with the stage built by a
// new-expression, `name` declared before the loop gives 96.4%).
ShaderTex_unk::ShaderTex_unk(void* model, atn::ShaderDesc_unk* desc) : mDesc(desc) {
    mTextures.Resize_unk(desc->mA48, 6);
    mOrder.Resize_unk(mDesc->mA48, 6);
    for (unsigned int i = 0; i < mTextures.mSize; i++) {
        const char* name = mDesc->mA4C[i];
        int id = fn_8007B078(name);
        if (id == -1) {
            char buf[0x100];
            sprintf(buf, "Shader '%s' has invalid texture '%s'\n", mDesc->m010, name);
            buf[0xFF] = 0;
            lbl_8087EEB8->fn_800697D8(buf);
        }
        ShaderTexEntryData_unk entry = {id, i, 0};
        const char* texture = fn_80081C34(id);
        if (texture != 0) {
            TexStage_unk* bind = new ((MemoryTag)6, "", "", 0) TexStage_unk(-1);
            entry.mBind = bind;
            bind->fn_8007AB38(mDesc, i);
            bind->fn_8007C018(texture, 1);
            bind->fn_8007B100();
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
Material_unk* fn_800C4A90(ShaderTex_unk* shader) {
    Material_unk* mat = new ((MemoryTag)6, "", "", 0) Material_unk(shader);
    mat->m00 = fn_800DC6B4(shader->mDesc->m010);
    mat->m04 = fn_800DC6B4(shader->mDesc->m010);
    mat->mColor = shader->mDesc->m014;
    mat->m20 = shader->mDesc->m01C;
    mat->m1C = shader->mDesc->m018;
    mat->mTevColor[0].r = shader->mDesc->m024[0];
    mat->mTevColor[0].g = shader->mDesc->m024[1];
    mat->mTevColor[0].b = shader->mDesc->m024[2];
    mat->mTevColor[0].a = shader->mDesc->m024[3];
    mat->mTevColor[1].r = shader->mDesc->m024[4];
    mat->mTevColor[1].g = shader->mDesc->m024[5];
    mat->mTevColor[1].b = shader->mDesc->m024[6];
    mat->mTevColor[1].a = shader->mDesc->m024[7];
    mat->mTevColor[2].r = shader->mDesc->m024[8];
    mat->mTevColor[2].g = shader->mDesc->m024[9];
    mat->mTevColor[2].b = shader->mDesc->m024[10];
    mat->mTevColor[2].a = shader->mDesc->m024[11];
    mat->m14_20 = shader->mDesc->m07C != -1;
    mat->m14_19 = shader->mDesc->m064 == 2;
    mat->m4C = -1;
    mat->m4A = shader->mDesc->m0E4;
    int n = shader->mDesc->m0E4;
    mat->m0C = new ((MemoryTag)6, "", "", 0) TexStage_unk[n];
    for (int i = 0; i < mat->m4A; i++) {
        mat->m0C[i].fn_8007AB38(shader->mDesc, i);
    }
    mat->m4E = shader->mDesc->m090;
    mat->m4F = shader->mDesc->m0A8;
    mat->m50 = shader->mDesc->m08C;
    mat->m51 = shader->mDesc->m0A4;
    mat->m38.r = shader->mDesc->m18C[0];
    mat->m38.g = shader->mDesc->m18C[1];
    mat->m38.b = shader->mDesc->m18C[2];
    mat->m38.a = shader->mDesc->m18C[3];
    mat->m3C = shader->mDesc->m054 / 255.0f;
    mat->m14_12 = shader->mDesc->mA38 == 1;
    mat->m14_0 = fn_800D1C60(shader->mDesc->mA14) != 7;
    mat->m14_4 = shader->mDesc->mA18;
    mat->m14_1 = fn_800D1C60(shader->mDesc->mA14);
    mat->m14_21 = shader->mDesc->mA28;
    mat->m14_22 = fn_800D1C60(shader->mDesc->mA2C);
    mat->m14_25 = shader->mDesc->mA30;
    mat->fn_80081D00();
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

void ShaderLOD_unk::fn_800C4F38(Material_unk** out) const {
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
