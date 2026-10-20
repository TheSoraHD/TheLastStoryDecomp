#include "Event/Behavior.h"

extern "C" void* lbl_8087EF70;                                // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered

namespace Event {

// inferred: test behaviour, plays one of four motions when its pad button is pressed
int BehaviorGesture::vf08(ActionArg_unk* arg) {
    ActionOwner_unk* owner = arg->fn_805381A0();
    EventManager_unk* mgr = arg->fn_805381C8();
    int id = owner->m10;
    Actor_unk* actor = mgr->FindActor_unk(id);
    int buttons[4];
    int motions[4];
    buttons[0] = arg->GetParam_unk(0)->m04.i;
    motions[0] = arg->GetParam_unk(1)->m04.i;
    buttons[1] = arg->GetParam_unk(2)->m04.i;
    motions[1] = arg->GetParam_unk(3)->m04.i;
    buttons[2] = arg->GetParam_unk(4)->m04.i;
    motions[2] = arg->GetParam_unk(5)->m04.i;
    buttons[3] = arg->GetParam_unk(6)->m04.i;
    motions[3] = arg->GetParam_unk(7)->m04.i;
    for (int i = 0; i < 4; i++) {
        if (fn_800A555C(lbl_8087EF70, 0, buttons[i]) != 0) {
            actor->fn_800119C0(0, motions[i], 0, 10, 0, 0, 1, 1.0f, 0.0f);
        }
    }
    return 0;
}

int BehaviorGesture::vf0C(ActionArg_unk* arg, int phase) {
    return 0;
}

} // namespace Event
