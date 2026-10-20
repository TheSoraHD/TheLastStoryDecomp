#ifndef MP_D2ACOMMONDIALOG_H
#define MP_D2ACOMMONDIALOG_H

#include "atn/Task.h"

namespace MP {

// inferred from __vt__Q22MP15D2ACommonDialog (0x80790888): yes/no dialog of the multiplayer
// menus; singleton (0x8087F588). Only the members used by the menus so far.
class D2ACommonDialog : public atn::Task {
public:
    static D2ACommonDialog* sInstance;

    unsigned char m48[0x4C - 0x48];
    int m4C; // inferred: nonzero while the dialog is open
};

} // namespace MP

// inferred: methods of the dialog, still called by their automatic names
extern "C" int fn_804AC79C(MP::D2ACommonDialog* dlg); // inferred: "yes" chosen
extern "C" int fn_804AC7EC(MP::D2ACommonDialog* dlg); // inferred: "no" chosen
extern "C" void fn_804AC96C(MP::D2ACommonDialog* dlg, int a, float f, int b, int c); // inferred: opens
extern "C" void fn_804ACAF8(MP::D2ACommonDialog* dlg);
extern "C" void fn_804ACD10(MP::D2ACommonDialog* dlg);
extern "C" void fn_804ACDBC(MP::D2ACommonDialog* dlg);
extern "C" void fn_804AD000(MP::D2ACommonDialog* dlg, const wchar_t* text, int arg); // inferred: sets the text

#endif
