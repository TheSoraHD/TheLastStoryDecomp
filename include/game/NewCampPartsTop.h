#ifndef GAME_NEWCAMPPARTSTOP_H
#define GAME_NEWCAMPPARTSTOP_H

#include "game/NewCampPartsBase.h"
#include "atn/FixedArray.h"

namespace atn {
class D3AnimeInstance;
class D2A_Task;
} // namespace atn
struct CampSaveData_unk;

// inferred: menu entry of the camp top (names invented)
struct NewCampTopInfo_unk {
    int mIndex;     // inferred: position in the list
    int mTitle;     // inferred: message id of the title
    int mTitleLang; // inferred: message id of the title in the other language
};
struct NewCampTopEntry_unk {
    int mId;                       // 0x00
    NewCampTopInfo_unk mInfo;      // 0x04
    atn::D3AnimeInstance* mList;   // 0x10
    atn::D3AnimeInstance* mShine;  // 0x14
};

// inferred from __vt__15NewCampPartsTop (0x80793740): main menu of the camp
class NewCampPartsTop : public NewCampPartsBase {
public:
    static NewCampPartsTop* fn_80529118(atn::Task* parent); // inferred: creates the parts
    NewCampPartsTop(atn::Task* parent);                       // 0x8052917C
    virtual ~NewCampPartsTop();                               // 0x80529370
    virtual void vf10();                                      // 0x805294F8
    virtual void vf14();                                      // 0x80529524
    virtual void vf1C(NewCampScene_unk* scene);               // 0x805293C8
    virtual void vf20();                                      // 0x80529B44
    virtual void vf24();                                      // 0x80529B48
    virtual void vf28();                                      // 0x80529C10
    virtual void vf2C();                                      // 0x80529C70
    virtual void vf30();                                      // 0x8052A668
    int fn_805294E4(); // inferred: nonzero while the menu is in use

    NewCampTopEntry_unk mEntries[8];                     // 0xD4
    atn::FixedArray_unk<NewCampTopEntry_unk*, 8> mList; // 0x194: inferred: shown entries
    atn::D2A_Task* mInfoAnime;                           // 0x1B8: inferred: camp_info3
    int mCursor;                                         // 0x1BC: inferred: cursor of the sub menu
    int mTimer;                                          // 0x1C0
    int m1C4;
    CampSaveData_unk* mSave;                             // 0x1C8
    int mCanLoad;                                        // 0x1CC
    int m1D0;
    int m1D4;
};

#endif
