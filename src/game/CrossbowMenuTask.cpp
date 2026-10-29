#include "game/CrossbowMenuTask.h"
#include "game/Inventory.h"
#include "atn/Memory.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int rand();
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" CrossbowD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: creates a D2A anime
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" void fn_801F4AA0(CrossbowD2A_unk* anime, const char* name, float r, float g, float b);
extern "C" void fn_801F4C14(CrossbowD2A_unk* anime, const char* name, const char* text);
// inferred: sets the value of the pane `name` (name invented)
inline void SetPaneValue2_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// inferred: sets the text of the pane `name` (name invented)
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}

// inferred: item lookups (Inventory::instance_ is probably the inventory); names unknown (as in CharaSub_unk.cpp)
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
extern "C" void* lbl_8087F430;
extern "C" int fn_80370174(void* field, int id);
extern "C" ItemList_unk* fn_8020787C(int id);
extern "C" ItemDef_unk* fn_80219EB4();
extern "C" int fn_80219EBC();
extern "C" int fn_804446BC(Inventory* inventory, int type, int id);
extern "C" int fn_804444E8(Inventory* inventory, int id);
extern "C" void fn_8044441C(Inventory* inventory, int id, int count);
extern "C" CrossbowItem_unk* fn_80211480(int id);
extern "C" int fn_80211734(CrossbowItem_unk* item); // inferred: infinite arrows
extern "C" int lbl_8087DAA0;                        // inferred: id of the last entry
// inferred: bullet data (fn_80219E6C)
struct CrossbowBullet_unk {
    unsigned char m00[0x08];
    const wchar_t* mName; // 0x08
    unsigned char m0C[0x90 - 0x0C];
    int m90;
    unsigned char m94[0xC8 - 0x94];
    int mC8;
};
extern "C" CrossbowBullet_unk* fn_80219E6C(int id);
struct Unk_8087F490 {
    unsigned char m0000[0x10FC];
    int m10FC;
};
extern "C" Unk_8087F490* lbl_8087F490;
extern "C" int lbl_8087F610;
// inferred: player (only the members used here)
struct CrossbowPlayerInfo_unk {
    unsigned char m000[0x11C];
    int m11C;
};
struct CrossbowPlayer_unk {
    unsigned char m000[0x50];
    int m50;
    unsigned char m54[0x5C - 0x54];
    CrossbowPlayerInfo_unk* m5C;
    unsigned char m60[0xAA4 - 0x60];
    int mAA4;
    unsigned char mAA8[0xAAC - 0xAA8];
    int mAAC;
};
struct CrossbowPlayerManager_unk {
    unsigned char m00[0x48];
    CrossbowPlayer_unk* m48;
};
extern "C" CrossbowPlayerManager_unk* lbl_8087F8A0;

static CrossbowIcon_unk sIcons_unk[7] = {
    {0, CrossbowColor_unk(0.74509805f, 0.39215687f, 0.9411765f, 1.0f)},
    {1, CrossbowColor_unk(1.0f, 0.39215687f, 0.39215687f, 1.0f)},
    {2, CrossbowColor_unk(1.0f, 0.7058824f, 0.0f, 1.0f)},
    {3, CrossbowColor_unk(0.8627451f, 0.8627451f, 0.8627451f, 1.0f)},
    {4, CrossbowColor_unk(0.8156863f, 0.9411765f, 0.37254903f, 1.0f)},
    {5, CrossbowColor_unk(0.9411765f, 0.6666667f, 0.6666667f, 1.0f)},
    {6, CrossbowColor_unk(0.9411765f, 0.9411765f, 0.3529412f, 1.0f)},
};

CrossbowMenuTask* CrossbowMenuTask::sInstance;

// inferred: inserts at the front of the list (name invented)
inline void Insert_unk(atn::FixedArray_unk<int, 64>& array, unsigned int pos, const int& value) {
    for (unsigned int j = array.mSize; j > pos; j--) {
        array.mData[j] = array.mData[j - 1];
    }
    array.mData[pos] = value;
    array.mSize++;
}

CrossbowMenuTask* CrossbowMenuTask::fn_801E9398(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) CrossbowMenuTask(parent);
    }
    return sInstance;
}

// inferred: not inline; auto-inlined in fn_801E9398 and stripped by the linker (its path stays in the string pool)
CrossbowMenuTask::CrossbowMenuTask(atn::Task* parent)
    : atn::Task(parent), m150(0), m154(1), m158(0x65), m15C(1), m160(1) {
    m48 = fn_801F3FF8(this, "/data/d2anime/ui_bowgun_select.d2a", 0);
    m48->fn_800D246C(1);
}

CrossbowMenuTask::~CrossbowMenuTask() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int CrossbowMenuTask::vf0C() {
    if (fn_800D3FA4() == 0) {
        return 0;
    }
    m48->fn_800D246C(0);
    m48->mSpeed = 0.0f;
    m48->mFC &= ~0x10000000;
    fn_801E9544();
    return 1;
}

// NONMATCHING: 99.8% - the first loop keeps the inventory pointer in r28 and the 0xD0 stride in r29 (swapped in
// the target); tried local placement, separate id/def locals, pointer arithmetic
void CrossbowMenuTask::fn_801E9544() {
    m158 = fn_80370174(lbl_8087F430, 0x6D);
    if (m158 == 0) {
        m158 = 0x65;
    } else if (m160 != 0) {
        m150 = 0;
    }
    m4C.mSize = 0;
    m160 = 1;
    ItemList_unk* list = fn_8020787C(fn_80370174(lbl_8087F430, 100));
    if (list == 0 || list->mCount == 0) {
        m4C.push_back(m158);
        Inventory* inventory;
        for (int i = 0; i < fn_80219EBC(); i++) {
            inventory = Inventory::instance_;
            int id = fn_804446BC(inventory, 5, fn_80219EB4()[i].m04);
            if (id != 0 && id != m158) {
                m4C.push_back(id);
                m160 = 0;
            }
        }
        m4C.push_back(lbl_8087DAA0);
    } else {
        int found = 0;
        for (unsigned int i = 0; i < list->mCount; i++) {
            const int& id = list->mIds[i];
            if (m158 == id) {
                Insert_unk(m4C, 0, id);
                found = 1;
            } else {
                m4C.push_back(id);
            }
        }
        if (found == 0 && m4C.mSize != 0) {
            m158 = m4C.mData[0];
        }
    }
}

void CrossbowMenuTask::fn_801E97DC() {
    m150 = 0;
}

void CrossbowMenuTask::vf10() {
    if (m154 != 0) {
        fn_801E9544();
        m154 = 0;
    }
    int count = 1;
    for (unsigned int i = 1; i < m4C.mSize; i++) {
        CrossbowItem_unk* item = fn_80211480(m4C.mData[i]);
        if (item != 0) {
            if (fn_80211734(item) != 0 || fn_804444E8(Inventory::instance_, m4C.mData[i]) > 0) {
                count++;
            }
        }
    }
    m160 = count <= 1;
    if (m15C == 1) {
        if (m48->mFrame >= 10.0f) {
            m15C = 0;
            m48->mFrame = 10.0f;
            m48->mSpeed = 0.0f;
        }
    } else if (m15C == 2) {
        if (m48->mFrame >= 15.0f) {
            m15C = 0;
            m48->mFrame = 10.0f;
            m48->mSpeed = 0.0f;
        }
    } else if (m15C == 3) {
        if (m48->mFrame >= 20.0f) {
            m15C = 0;
            m48->mFrame = 10.0f;
            m48->mSpeed = 0.0f;
        }
    }
}

void CrossbowMenuTask::vf14() {
    if (m4C.mSize == 0 || m154 != 0) {
        return;
    }
    if (m15C == 2) {
        fn_801E9ECC(-1, 0);
        fn_801E9B84(m150, 1);
        fn_801E9ECC(1, 0);
        fn_801E9B84(m150, 2);
        fn_801E9ECC(1, 0);
        fn_801E9B84(m150, 3);
        fn_801E9ECC(1, 0);
        fn_801E9B84(m150, 4);
        fn_801E9ECC(-1, 0);
        fn_801E9ECC(-1, 0);
    } else {
        fn_801E9ECC(-1, 0);
        fn_801E9ECC(-1, 0);
        fn_801E9B84(m150, 1);
        fn_801E9ECC(1, 0);
        fn_801E9B84(m150, 2);
        fn_801E9ECC(1, 0);
        fn_801E9B84(m150, 3);
        fn_801E9ECC(1, 0);
        fn_801E9B84(m150, 4);
        fn_801E9ECC(-1, 0);
    }
    if (m160 != 0) {
        SetPaneValue2_unk(m48->m58, "multiArrow", 0.0f);
    } else {
        SetPaneValue2_unk(m48->m58, "multiArrow", 1.0f);
    }
    if (m4C.mData[m150] == 0x19C) {
        lbl_8087F490->m10FC = 1;
    }
}

void CrossbowMenuTask::fn_801E9B84(int index, int slot) {
    wchar_t label[0x40];
    char name[0x40];
    char num[0x40];
    char icon[0x40];
    char show[0x40];
    char color[0x40];
    sprintf(name, "arrowName%02d", slot);
    sprintf(num, "arrowNum%02d", slot);
    sprintf(icon, "iconIndex%02d", slot);
    sprintf(show, "showIcon%02d", slot);
    sprintf(color, "iconColor%02d", slot);
    CrossbowItem_unk* item = fn_80211480(m4C.mData[index]);
    if (item != 0) {
        int count = fn_804444E8(Inventory::instance_, m4C.mData[index]);
        CrossbowIcon_unk info = fn_801EA11C(item);
        float visible;
        if (info.mIndex < 0) {
            visible = 0.0f;
        } else {
            visible = 1.0f;
        }
        SetPaneValue2_unk(m48->m58, show, visible);
        SetPaneValue2_unk(m48->m58, icon, (float)info.mIndex);
        fn_801F4AA0(m48, color, 255.0f * info.mColor.r, 255.0f * info.mColor.g, 255.0f * info.mColor.b);
        char text[0x20];
        sprintf(text, "%d", count);
        if (fn_80211734(item) != 0) {
            strcpy(text, "--");
        }
        int level = fn_80370174(lbl_8087F430, 0xDA);
        if (level > 0 && item->mId == 0x65) {
            fn_800DD3FC(label, L"%s + %d", item->mName, level / 10);
        } else {
            fn_800DD3FC(label, L"%s", item->mName);
        }
        if (lbl_8087F610 != 0 && item->mId == 0x65) {
            CrossbowPlayer_unk* player = lbl_8087F8A0->m48;
            int bullet = 0;
            if (player->m5C->m11C == 3 || player->m50 == 0xAAE72) {
                CrossbowBullet_unk* data = fn_80219E6C(player->mAA4);
                if (data != 0 && data->mC8 > 0) {
                    bullet = data->mC8;
                }
            } else if ((unsigned int)(player->m50 - 0xAAE76) <= 1) {
                bullet = player->mAAC;
            }
            if (bullet > 0) {
                fn_800DD3FC(label, L"%s", fn_80219E6C(bullet)->mName);
            }
        }
        SetPaneText_unk(m48->m58, name, label);
        fn_801F4C14(m48, num, text);
    }
}

void CrossbowMenuTask::fn_801E9EA4() {
    m154 = 1;
    m48->mFrame = 0.0f;
    m48->mSpeed = 1.0f;
    m15C = 1;
}

void CrossbowMenuTask::fn_801E9ECC(int dir, int anim) {
    if (anim != 0 && m160 != 0) {
        return;
    }
    if (dir != 0) {
        int step = -1;
        if (dir > 0) {
            step = 1;
        }
        if (anim != 0) {
            if (step < 0) {
                m15C = 2;
                m48->mSpeed = 1.0f;
                m48->mFrame = 10.0f;
            } else if (step > 0) {
                m15C = 3;
                m48->mSpeed = 1.0f;
                m48->mFrame = 15.0f;
            }
        }
        for (unsigned int i = 0; i < m4C.mSize; i++) {
            m150 += step;
            if (m150 < 0) {
                m150 = m4C.mSize - 1;
            }
            if ((unsigned int)m150 >= m4C.mSize) {
                m150 = 0;
            }
            if (m150 == 0) {
                return;
            }
            CrossbowItem_unk* item = fn_80211480(m4C.mData[m150]);
            if (item != 0) {
                if (fn_80211734(item) != 0 || fn_804444E8(Inventory::instance_, item->mId) > 0) {
                    return;
                }
            }
        }
    }
    m150 = 0;
}

int CrossbowMenuTask::fn_801EA01C() const {
    int ret = -1;
    int id = m4C.mData[m150];
    if (id != 0 && fn_804444E8(Inventory::instance_, id) > 0) {
        ret = fn_80211480(id)->mC4;
        if (id == 0x71) {
            ret += rand() % 3;
        }
    }
    return ret;
}

void CrossbowMenuTask::fn_801EA0B4() {
    int id = m4C.mData[m150];
    if (id != 0) {
        CrossbowItem_unk* item = fn_80211480(id);
        if (item != 0 && fn_80211734(item) == 0) {
            fn_8044441C(Inventory::instance_, id, 1);
        }
    }
}

CrossbowIcon_unk CrossbowMenuTask::fn_801EA11C(CrossbowItem_unk* item) {
    if (item != 0) {
        if (item->mId == 0x69) {
            return sIcons_unk[1];
        }
        if (item->mId == 0x71) {
            return sIcons_unk[4];
        }
        if (item->mId == 0x6F) {
            return sIcons_unk[5];
        }
        if (item->mId == 0x70) {
            return sIcons_unk[6];
        }
        if (item->mId == 0x6C) {
            return sIcons_unk[1];
        }
        if (item->mId == 0x72) {
            return sIcons_unk[4];
        }
        for (int i = 0; i < 2; i++) {
            if ((unsigned int)(item->mF4[i].mKind - 5) <= 1) {
                return sIcons_unk[0];
            }
        }
        CrossbowBullet_unk* data = fn_80219E6C(item->mC4);
        if (data != 0 && data->m90 != 0) {
            return sIcons_unk[2];
        }
    }
    return sIcons_unk[3];
}
