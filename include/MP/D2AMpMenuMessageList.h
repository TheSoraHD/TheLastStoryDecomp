#ifndef MP_D2AMPMENUMESSAGELIST_H
#define MP_D2AMPMENUMESSAGELIST_H

#include "MP/D2AMpMenu_unk.h"

namespace MP {

// inferred from __vt__Q22MP20D2AMpMenuMessageList (0x80790DB8): list of the fixed chat messages
class D2AMpMenuMessageList : public atn::Task {
public:
    static D2AMpMenuMessageList* fn_804C2FF8(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuMessageList(atn::Task* parent);
    virtual ~D2AMpMenuMessageList();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void SetTexts_unk();
    void fn_804C35F0(int state);  // inferred: changes the state
    void fn_804C38C0(int show);   // inferred: shows the button marks
    int fn_804C4150(int count);   // inferred: list input
    void fn_804C4530();           // inferred: update of the open state
    void fn_804C50C0();           // inferred: draw of the list
    void fn_804C5638();           // inferred: closes the task

    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x10) {
            mTasks.push_back(task);
        }
    }
    void SetAll_unk(int arg) {
        for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(arg);
            }
        }
    }

    MpD2A_unk* m48; // inferred: online_message_base2.d2a
    MpD2A_unk* m4C; // inferred: camp_cursor.d2a
    atn::FixedArray_unk<MpD2A_unk*, 0x10> mTasks; // 0x50
    int m94;        // inferred: state
    int m98;        // inferred: previous state
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;        // inferred: cursor
    float mB0;      // inferred: scroll timer of the selected message
    int mB4;

    static D2AMpMenuMessageList* sInstance;
    static int sPage;
};

} // namespace MP

#endif
