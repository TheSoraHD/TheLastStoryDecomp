#include "atn/LH_Material.h"
#include "atn/LoadManager.h"
#include "std/vector.h"

// inferred: material read from the loaded file (0x54 bytes; constructor 0x8007C144, destructor
// 0x8007C3F8); name invented
struct Material_unk {
    Material_unk(int arg);
    ~Material_unk();
    // inferred: reads one material from `data` and returns the end of what it read
    const char* fn_8007C46C(const char* path, const char* data, int size);
    unsigned char m00[0x54];
};

namespace atn {

// inferred: list of the materials (name invented). The destructor of LoadMaterial has four `this`
// null checks for it, one more level than std::vector alone.
class MaterialList_unk : public std::vector<Material_unk*> {
public:
    ~MaterialList_unk() {}
};

// inferred from __vt__Q23atn12LoadMaterial (0x8078FD80)
class LoadMaterial : public LoadCore {
public:
    LoadMaterial(const char* path) : LoadCore(path) {}
    virtual ~LoadMaterial();
    virtual void vf0C();

    MaterialList_unk mMaterials; // 0x5C
};

// NONMATCHING: the original does not fold size - size when the vector is destroyed (lwz + subf
// where we store 0); tried clear(), an inline loop and pointer differences in std::vector.
LoadMaterial::~LoadMaterial() {
    for (Material_unk** it = mMaterials.begin(); it != mMaterials.end(); it++) {
        delete *it;
    }
}

// NONMATCHING: the inlined std::vector::push_back differs (see the draft in std/vector.h).
void LoadMaterial::vf0C() {
    if (mState == 2) {
        const char* start = (const char*)m54;
        int size = (int)m50;
        const char* p = start;
        while (p != 0 && p - start < (int)m50) {
            Material_unk* material = new (6, "", "", 0) Material_unk(0);
            p = material->fn_8007C46C(mPath, p, size);
            mMaterials.push_back(material);
            size = (int)m50 - (p - start);
        }
        fn_80473130();
        fn_80473104(3);
    }
}

void LH_Material::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadMaterial>(path, 1);
}

} // namespace atn
