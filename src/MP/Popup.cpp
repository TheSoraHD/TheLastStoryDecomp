#include "MP/Popup.h"
#include "atn/Memory.h"

// inferred: creates a d2anime task playing `path` (returns the task)
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg);

static const char sPathStr[] = "/data/d2a/ui_item_pop2.d2a";
static const char* const sPath = sPathStr; // inferred: separate .rodata array + .sdata2 pointer

namespace MP {

Popup* Popup::sInstance;

inline Popup::Popup(atn::Task* parent) : atn::Task(parent) {
    for (unsigned int i = 0; i < 4; i++) {
        PopupD2A_unk* item = (PopupD2A_unk*)fn_801F3FF8(this, sPath, 0);
        if (m48.size() < 4) {
            m48.push_back(item);
        }
    }
    for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
        if (*it != 0) {
            (*it)->fn_800D246C(1);
        }
    }
}

Popup* Popup::fn_805012C4(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) Popup(parent);
    }
    return sInstance;
}

Popup::~Popup() {
    sInstance = 0;
}

int Popup::vf0C() {
    if (fn_800D3FA4() != 0) {
        for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(0);
            }
        }
        for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
            if (*it != 0) {
                (*it)->mFlags |= 4;
            }
        }
        for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
            if (*it != 0) {
                (*it)->mFC &= ~0x10000000;
            }
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: vf10 (0x80501588, 0xB74 bytes: placement of the popups over the characters) is not
// written yet

} // namespace MP
