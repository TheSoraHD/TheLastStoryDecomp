#include "atn/LH_Material.h"
#include "atn/LoadManager.h"
#include "atn/Material_unk.h"
#include "std/vector.h"

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

LoadMaterial::~LoadMaterial() {
    for (Material_unk** it = mMaterials.begin(); it != mMaterials.end(); it++) {
        delete *it;
    }
}

// NONMATCHING: 95.5% - the inlined std::vector::push_back loads the capacity before the size (see
// std/vector.h), and the original reloads m50 for the loop condition where we reuse the value
// loaded for `size` (tried int/unsigned casts and the order of the three locals).
void LoadMaterial::vf0C() {
    if (mState == 2) {
        const char* start = (const char*)m54;
        int size = (int)m50;
        const char* p = start;
        while (p != 0 && p - start < (int)m50) {
            Material_unk* material = new ((MemoryTag)6, "", "", 0) Material_unk(0);
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

void* LH_Material::fn_80474860() {
    return &((LoadMaterial*)mCore)->mMaterials;
}

unsigned int LH_Material::fn_8047486C() {
    if (mCore != 0) {
        return ((LoadMaterial*)mCore)->mMaterials.size();
    }
    return 0;
}

} // namespace atn
