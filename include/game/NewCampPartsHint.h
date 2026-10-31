#ifndef GAME_NEWCAMPPARTSHINT_H
#define GAME_NEWCAMPPARTSHINT_H

#include "game/NewCampPartsBase.h"
#include "std/vector.h"

namespace atn {
class D3Anime;
class D3AnimeInstance;
} // namespace atn

// inferred: tutorial entry (fn_8021F0D4); names invented
struct NewCampTutorial_unk {
    int mFlag;   // 0x00: inferred: event flag that unlocks it
    int mSeen;   // 0x04
    unsigned char m08[0x28 - 0x08];
    wchar_t mName[0x40]; // 0x28
    int mCategory;       // 0xA8
};

class TutorialTask;

// inferred: colour of the debug menu (4 floats); name invented
struct NewCampColor_unk {
    NewCampColor_unk() {}
    NewCampColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: list of the tutorials (a class derived from std::vector: its destructor tests `this` four
// times); name invented
struct NewCampTutorialList_unk : public std::vector<NewCampTutorial_unk*> {};

// inferred from __vt__16NewCampPartsHint (0x807933C8): tutorial list of the camp
class NewCampPartsHint : public NewCampPartsBase {
public:
    static NewCampPartsHint* fn_80514720(atn::Task* parent); // inferred: creates the parts
    NewCampPartsHint(atn::Task* parent);                       // 0x80514784
    virtual ~NewCampPartsHint();                               // 0x80514AAC
    virtual void vf10();                                       // 0x80514C10
    virtual void vf14();                                       // 0x80514C8C
    virtual void vf1C(NewCampScene_unk* scene);                // 0x80514B40
    virtual void vf20();                                       // 0x80515318
    virtual void vf24();                                       // 0x805159C8
    virtual void vf28();                                       // 0x80515A38
    virtual void vf2C();                                       // 0x80515E44
    virtual void vf30();                                       // 0x80515AC8
    virtual void vf3C(int show);                               // 0x80515954
    virtual void vf40(void* group);                            // 0x8051673C
    virtual void vf48();                                       // 0x80516904
    virtual int vf4C();                                        // 0x80514B38
    void fn_80515F28(int mode); // inferred: changes the mode
    void fn_80516004();         // inferred: input of the categories
    void fn_8051640C();         // inferred: input of the list
    void fn_80516544();         // inferred: draws the list
    void ReadList_unk();        // inferred: inline (name invented)

    int mMode;      // 0xD4
    int mD8;
    int mDC;
    int mE0;
    int mE4;
    int mCategory;  // 0xE8
    int mCursor;    // 0xEC
    int mTop;       // 0xF0
    atn::D3AnimeInstance* mItems[10]; // 0xF4
    atn::D3Anime* mBase;              // 0x11C
    atn::D3Anime* mNoItem;            // 0x120
    atn::D3AnimeInstance* mScroll;    // 0x124
    int m128;                         // inferred: a tutorial was started
    NewCampTutorialList_unk mList;    // 0x12C
    class TutorialTask* mTutorial; // 0x138
    int mWait;                        // 0x13C
    float mOpen;                      // 0x140
    atn::Vector3 mMove;               // 0x144: inferred: "MoveOffset"
    atn::Vector3 mOpenOffset;         // 0x150: inferred: "OpenOffset"
    NewCampColor_unk mTutorialCol;    // 0x15C
    NewCampColor_unk mTutorialShadow; // 0x16C
    NewCampColor_unk mTipsCol;        // 0x17C
    NewCampColor_unk mTipsShadow;     // 0x18C
    int m19C;
};

#endif
