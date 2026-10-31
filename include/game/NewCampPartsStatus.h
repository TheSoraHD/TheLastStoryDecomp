#ifndef GAME_NEWCAMPPARTSSTATUS_H
#define GAME_NEWCAMPPARTSSTATUS_H

#include "game/NewCampPartsBase.h"
#include "game/GameUI.h"
#include "atn/FixedArray.h"

namespace atn {
class D3Anime;
class D3AnimeInstance;
struct D2ATask_unk;
} // namespace atn
class CharaStatus;

// inferred: skill shown on a status page (0x10 bytes); names invented
struct NewCampStatusSkill_unk {
    int mIndex;           // inferred: slot on the page
    int mHelp;            // inferred: tutorial shown for the skill (-1: none)
    int mTension;         // inferred: tension skill (the cursor uses the last name)
    const wchar_t* mText; // inferred: help text
};

// inferred: skills of a page (clear() and size() of the list are called out of line as
// GameUICounter_unk::fn_803CF734 and fn_80372574)
typedef atn::FixedArray_unk<NewCampStatusSkill_unk, 10> NewCampStatusSkillList_unk;

// inferred: status page of a party member (0xAC bytes); names invented
struct NewCampStatusSet_unk {
    int mIndex; // inferred: position in the party
    int mChara; // inferred: character id
    NewCampStatusSkillList_unk mSkills;
};

typedef atn::FixedArray_unk<NewCampStatusSet_unk, 7> NewCampStatusSetList_unk;

// inferred: the two layouts of a page
struct NewCampStatusPage_unk {
    NewCampStatusPage_unk() : mBase(0), mParam(0) {}
    atn::D3Anime* mBase;          // inferred: "status_base"
    atn::D3AnimeInstance* mParam; // inferred: "camp_status"
};

struct NewCampStatusD2A_unk;

// inferred from __vt__18NewCampPartsStatus (0x80793698): status pages of the camp
class NewCampPartsStatus : public NewCampPartsBase {
public:
    static NewCampPartsStatus* fn_805268C8(atn::Task* parent); // inferred: creates the parts
    NewCampPartsStatus(atn::Task* parent);                       // 0x8052692C
    virtual ~NewCampPartsStatus();                               // 0x80526BBC
    virtual int vf4C();                                          // 0x80526C14
    virtual void vf1C(NewCampScene_unk* scene);                  // 0x80526C1C
    virtual void vf10();                                         // 0x80526CBC
    virtual void vf14();                                         // 0x80526D94
    virtual void vf20();                                         // 0x80527410
    virtual void vf24();                                         // 0x805275E8
    virtual void vf28();                                         // 0x80527708
    virtual void vf2C();                                         // 0x805277C0
    virtual void vf30();                                         // 0x8052791C
    virtual void vf3C(int show);                                 // 0x80528DF8
    virtual void vf40(void* group);                              // 0x80528ED4
    virtual void vf48();                                         // 0x80528FE8
    void fn_80527938(); // inferred: fills the pages
    void fn_80528310(atn::D2ATask_unk* base, CharaStatus* status, int skill, int* count, NewCampStatusSet_unk* set,
                     int help); // inferred: adds a skill
    void fn_80528454(atn::D2ATask_unk* base, int* count, NewCampStatusSet_unk* set);
    void fn_80528574(atn::D2ATask_unk* base, int* count, NewCampStatusSet_unk* set);
    void fn_80528694(); // inferred: input of the page cursor
    void fn_805288BC(int state);
    void fn_805289A8(); // inferred: input of the pages
    void fn_80528A90(); // inferred: input of the skills
    void fn_80528BF0(); // inferred: places the pages

    NewCampStatusPage_unk mPages[7]; // 0xD4
    NewCampStatusD2A_unk* mSelect;   // 0x10C: inferred: "status_select"
    float mArrow;                    // 0x110
    int mPrevState;                  // 0x114
    int mPage;                       // 0x118
    NewCampStatusSetList_unk mSets;  // 0x11C
    float mScroll;                   // 0x5D4
    int mSkill;                      // 0x5D8
    class TutorialTask* mTutorial; // 0x5DC
    int mWait;                       // 0x5E0
    atn::Vector3 mOffset;            // 0x5E4
    atn::Vector3 mMove;              // 0x5F0: inferred: "MoveOffset"
    int m5FC;
};

#endif
