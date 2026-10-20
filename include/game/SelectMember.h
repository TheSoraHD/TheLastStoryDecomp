#ifndef GAME_SELECTMEMBER_H
#define GAME_SELECTMEMBER_H

#include "atn/Task.h"
#include "game/CharaStatus.h"

// inferred: d2anime task of the member selection (fn_801F3FF8/fn_801F64D0); names invented
class SelectMemberAnim_unk : public atn::Task {
public:
    unsigned char m48[0x4D - 0x48];
    unsigned char m4D;
    unsigned char m4E[0x54 - 0x4E];
    float m54;
    unsigned char m58[0xFC - 0x58]; // 0x58: inferred: layout
    void* GetLayout_unk() { return m58; } // inferred: name invented
    // 0xFC: inferred: flags
    unsigned int mFC_31_29 : 3;
    unsigned int mVisible_unk : 1; // inferred: bit 28
    unsigned int mFC_27_0 : 28;
    float m100;                     // 0x100: inferred: frame
    float m104;                     // 0x104: inferred: speed
};

// inferred from __vt__12SelectMember (0x80795850): menu that chooses the party members
// ("/data/d2anime/member_select_*.d2a")
class SelectMember : public atn::Task {
public:
    // inferred: member of the list (8 bytes)
    struct Member_unk {
        Member_unk() : mId(0), mSelected(0), mFixed(0) {}
        Member_unk(int id, unsigned char selected, unsigned char fixed) : mId(id), mSelected(selected), mFixed(fixed) {}
        int mId;
        unsigned char mSelected;
        unsigned char mFixed;
    };

    SelectMember(atn::Task* parent, int type, unsigned char save); // 0x805716B0
    virtual ~SelectMember();                                      // 0x80571800
    virtual int vf0C();                                           // 0x80571858
    virtual void vf10();                                          // 0x8057197C
    virtual void vf14();                                          // 0x80571A28

    static SelectMember* fn_8057162C(atn::Task* parent, int type, unsigned char save); // inferred: creates the menu
    void fn_80571C64(int state); // inferred: changes the state
    void fn_80571DB8();          // inferred: state 2, choosing the members
    void fn_80571FD8();          // inferred: state 3, confirmation
    void fn_80572194();          // inferred: state 4, save question
    void fn_80572290();          // inferred: reads the available members
    void fn_80572450();          // inferred: updates the panes

    // inferred: number of selected members (inlined three times); name invented
    int CountSelected_unk() {
        int n = 0;
        for (unsigned int i = 0; i < mMembers.mCount; i++) {
            if (mMembers.mData[i].mSelected != 0) {
                n++;
            }
        }
        return n;
    }

    SelectMemberAnim_unk* mAnims[9]; // 0x48: base, list x7 (0x4C), info (0x68)
    FixedArray_unk<Member_unk, 7> mMembers; // 0x6C
    int mMax;       // 0xA8: inferred: number of members to choose
    int mCursor;    // 0xAC
    int mDecided;   // 0xB0
    unsigned char mSave; // 0xB4: inferred: asks to save after the choice
    int mType;      // 0xB8
    int mState;     // 0xBC
    int mPrevState; // 0xC0
    int mCursor2;   // 0xC4: inferred: cursor of the yes/no questions
    int mC8;
    int mCC;
};

#endif
