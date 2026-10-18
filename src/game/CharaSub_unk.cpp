#include "game/CharaSub_unk.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

CharaSub_unk::CharaSub_unk(CharaTask* owner)
    : mOwner(owner), m3C(1.0f), m40(0), m44(0) {
    m54.Assign_unk(lbl_807C7030);
    m60.Assign_unk(lbl_807C7030);
    m6C.Assign_unk(lbl_807C7030);
    m78.Assign_unk(lbl_807C7030);
    m84.Assign_unk(lbl_807C7030);
}

CharaSub_unk::~CharaSub_unk() {}

void CharaSub_unk::fn_80135338(const CharaSubSeg_unk& seg, const atn::Vector3& pos) {
    m04 = seg;
    m24 = pos;
}

void CharaSub_unk::fn_8013537C() {}

void CharaSub_unk::fn_80135380() {}

bool fn_80135384(const float& a, const float& b) {
    return a < b;
}

// inferred: item lookups (lbl_8087F4F0 is probably the inventory); names unknown
struct ItemList_unk {
    int m00;
    unsigned int mCount;
    int* mIds;
};
struct ItemDef_unk {
    int m00;
    int m04;
    unsigned char m08[0xD0 - 0x08];
};
struct Inventory_unk;
extern "C" void* lbl_8087F430;
extern "C" Inventory_unk* lbl_8087F4F0;
extern "C" int fn_80370174(void* field, int id);
extern "C" ItemList_unk* fn_8020787C(int id);
extern "C" ItemDef_unk* fn_80219EB4();
extern "C" int fn_80219EBC();
extern "C" int fn_804446BC(Inventory_unk* inventory, int type, int id);
extern "C" int fn_804444E8(Inventory_unk* inventory, int id);

int fn_8013539C() {
    ItemList_unk* list = fn_8020787C(fn_80370174(lbl_8087F430, 100));
    if (list == 0 || list->mCount == 0) {
        ItemDef_unk* def = fn_80219EB4();
        int num = fn_80219EBC();
        for (int i = 0; i < num; def++, i++) {
            int id = fn_804446BC(lbl_8087F4F0, 5, def->m04);
            if (id != 0 && fn_804444E8(lbl_8087F4F0, id) > 0) {
                return 1;
            }
        }
        if (fn_804444E8(lbl_8087F4F0, 412) > 0) {
            return 1;
        }
    } else {
        for (unsigned int i = 0; i < list->mCount; i++) {
            if (fn_804444E8(lbl_8087F4F0, list->mIds[i]) > 0) {
                return 1;
            }
        }
    }
    return 0;
}
