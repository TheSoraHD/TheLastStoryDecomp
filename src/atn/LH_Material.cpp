#include "atn/LH_Material.h"
#include "atn/LoadManager.h"
#include "atn/Material_unk.h"
#include "std/vector.h"

namespace atn {

// inferred: list of the materials (name invented)
typedef std::vector<Material_unk*> MaterialList_unk;

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

// inferred: size of the loaded data (name invented)
static inline int SizeOf_unk(const atn::LoadCore* core) { return (int)core->m50; }

// NONMATCHING: 97.7% - the inlined std::vector::push_back loads the capacity before the size (see
// std/vector.h) and the saved registers are one apart (95.5 -> 97.7 reading the first size through SizeOf_unk;
// tried int/unsigned casts and the order of the three locals).
void LoadMaterial::vf0C() {
    if (mState == 2) {
        const char* start = (const char*)m54;
        int size = SizeOf_unk(this);
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
