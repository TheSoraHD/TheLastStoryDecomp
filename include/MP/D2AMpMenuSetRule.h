#ifndef MP_D2AMPMENUSETRULE_H
#define MP_D2AMPMENUSETRULE_H

#include "MP/D2AMpMenu_unk.h"

namespace MP {

// inferred from __vt__Q22MP16D2AMpMenuSetRule (0x80790F30): rule settings of the multiplayer
// lobby (three rows: lives, team mode and time limit)
class D2AMpMenuSetRule : public atn::Task {
public:
    static D2AMpMenuSetRule* fn_804CD97C(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuSetRule(atn::Task* parent);
    virtual ~D2AMpMenuSetRule();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804CDE1C(int state);        // inferred: changes the state
    int fn_804CE2EC(int count);         // inferred: cursor input, returns 1 (decide) or 2 (cancel)
    void fn_804CE61C();                 // inferred: input of the open state
    void fn_804CE82C(int row, int before, int after, int dir); // inferred: shows a value change
    void fn_804CECEC();                 // inferred: reads the current rules
    void fn_804CEF84();                 // inferred: default rules
    void fn_804CF078(int arg);          // inferred: writes the rules

    void Add_unk(MpD2A_unk* task) {
        if (m7C.size() < 8) {
            m7C.push_back(task);
        }
    }
    void SetAll_unk(int arg) {
        for (MpD2A_unk** it = &m7C[0]; it != &m7C[m7C.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(arg);
            }
        }
    }
    void UpdateListPos_unk(); // inferred: not inline (its literals are in @stringBase0)
    // inferred: vf10 uses this inline helper (the registers of the inlined loop differ otherwise)
    void UpdateOpen_unk() {
        if (m48->mFrame >= 10.0f) {
            fn_804CDE1C(2);
        }
        UpdateListPos_unk();
    }
    int GetMaxLives_unk();  // inferred: inline (MP/D2AMpMenuSetRule.cpp)
    void ReadTime_unk();    // inferred: inline (MP/D2AMpMenuSetRule.cpp)
    void RefreshRows_unk() {
        for (int i = 0; i < 3; i++) {
            fn_804CE82C(i, m6C[i], m6C[i], 0);
        }
    }

    MpD2A_unk* m48;    // inferred: online_rule_base.d2a
    MpD2A_unk* m4C[3]; // inferred: option_rule_list.d2a (one per row)
    MpD2A_unk* m58;    // inferred: online_info_back.d2a
    int m5C;           // inferred: state
    int m60;           // inferred: previous state
    int m64;
    int m68;           // inferred: selected row
    int m6C[3];        // inferred: values of the rows (lives, team mode, time limit index)
    int m78;           // inferred: game mode when opened
    atn::FixedArray_unk<MpD2A_unk*, 8> m7C;

    static D2AMpMenuSetRule* sInstance;
    static const int sTimes[4];
};

} // namespace MP

#endif
