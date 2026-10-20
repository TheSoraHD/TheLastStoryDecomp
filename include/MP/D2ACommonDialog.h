#ifndef MP_D2ACOMMONDIALOG_H
#define MP_D2ACOMMONDIALOG_H

#include "MP/D2AMpMenu_unk.h"

namespace MP {

// inferred from __vt__Q22MP15D2ACommonDialog (0x80790888): message/yes-no dialog of the
// multiplayer menus; singleton (0x8087F588). Member names are invented.
class D2ACommonDialog : public atn::Task {
public:
    static D2ACommonDialog* fn_804AC430(atn::Task* parent); // inferred: creates the instance
    D2ACommonDialog(atn::Task* parent);
    virtual ~D2ACommonDialog();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    int fn_804AC734(); // inferred: closed with "yes" or the message acknowledged
    int fn_804AC79C(); // inferred: closed with "yes"
    int fn_804AC7EC(); // inferred: closed with "no"/cancel
    void fn_804AC83C(int repeat, float speed, int type);             // inferred: opens a message
    void fn_804AC96C(int repeat, float speed, int cursor, int type); // inferred: opens a question
    void fn_804ACAF8(); // inferred: resets the animes and texts
    void fn_804ACD10(); // inferred: shows the current window
    void fn_804ACDBC(); // inferred: hides the current window
    void fn_804ACE68(); // inferred: clears every text
    void fn_804AD000(const wchar_t* text, int line); // inferred: sets the text of a line
    void fn_804AD1EC(const wchar_t* text);           // inferred: sets a text split in lines
    void fn_804AD738(); // inferred: closes the dialog (cancel)

    static D2ACommonDialog* sInstance;

    int m48;            // inferred: cursor
    int m4C;            // inferred: state (1/2 message, 3 question, 4 yes, 5 no)
    int m50;            // inferred: type of window
    MpD2A_unk* m54;     // inferred: camp_info.d2a
    MpD2A_unk* m58;     // inferred: camp_info3.d2a
    MpD2A_unk* m5C;     // inferred: camp_info4.d2a
    MpD2A_unk* m60;     // inferred: camp_info2.d2a
    MpD2A_unk* m64;     // inferred: camp_info7.d2a
    MpD2A_unk* m68;     // inferred: online_info_check.d2a
    int m6C;            // inferred: number of choices
    int m70;
    int m74;
};

} // namespace MP

#endif
