#ifndef MP_MPCAMPMSGTABLE_UNK_H
#define MP_MPCAMPMSGTABLE_UNK_H

#include "MP/D2AMpMenu_unk.h"

// inferred: camp message table (lbl_8087F1E4, see game/MenuCommonTask.cpp)
struct MpCampMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : MP::lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" MpCampMsgTable_unk* lbl_8087F1E4;

#endif
