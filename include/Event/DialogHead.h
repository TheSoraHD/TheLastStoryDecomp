#ifndef EVENT_DIALOGHEAD_H
#define EVENT_DIALOGHEAD_H

#include "game/CharaTask.h"
#include "game/CharaFaceControl.h"
#include "atn/IRender.h"
#include "atn/Camera_unk.h"
#include "atn/Array_unk.h"
#include "atn/LipSync_unk.h"
#include <boost/scoped_ptr.hpp>

struct DialogSide_unk;

namespace Event {

// inferred from __vt__Q25Event10DialogHead (0x807838A0): face of a character drawn in the dialog
// window (created by Event::Dialog, 0x1A18 bytes). It copies the model of a character (m1428) and
// draws it through its own camera (IRender at 0x1424). Member names invented.
class DialogHead : public CharaTask, public atn::IRender {
public:
    DialogHead(atn::Task* parent, CharaTask* chara);
    // inferred: creates the face of `chara` (0 without a parent)
    static DialogHead* fn_802251F8(atn::Task* parent, CharaTask* chara);

    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    // IRender (the primary vtable lists these after the IRender part, in this order)
    virtual int GetRenderType_unk() { return mMotion.GetRenderType_unk(); }
    virtual void vf1C() { mMotion.vf1C(); }
    virtual void vf18(atn::RenderView_unk* view);
    virtual void vf10(atn::RenderView_unk* view);

    void fn_802254B0(float top, float bottom); // inferred: sets the camera used for the parts
    void fn_80225C80(int right);               // inferred: picks the side of the window
    void fn_80226B28(int dark, int right);     // inferred: picks the light set
    void fn_80227434(int kind, int arg, int right); // inferred: plays a face motion
    int fn_80227A94(char* path, int kind, int arg, int right); // inferred: path of a face file
    void fn_80227D2C(int kind, int arg); // inferred: loads the face files of a motion
    void fn_80227F78(LipSyncData_unk* data); // inferred: starts the lip sync

    // inferred: layout of the side the face is drawn on (left by default)
    DialogSide_unk* GetSide_unk();

    CharaTask* m1428;                 // inferred: character shown
    Camera_unk m142C;                 // inferred: camera of the face
    DialogSide_unk* m1620;            // inferred: side layout (0: left)
    ModelCameraParam_unk m1624;       // inferred: camera applied to the model
    float m1820;                      // inferred: fade in (0 to 1)
    int m1824;                        // inferred: "head" node
    atn::Vector3 m1828;               // inferred: position of the head node
    atn::Vector3 m1834;               // inferred: camera offset
    boost::scoped_ptr<FaceData_unk> m1840[7][8][2]; // inferred: face files [kind][arg][right]
    LipSyncData_unk m1A00;            // inferred: not used by this file
    int m1A0C;
    int m1A10; // inferred: frames until the next blink
    int m1A14;
};

} // namespace Event

#endif
