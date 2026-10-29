#include "Event/Behavior.h"
#include "game/CharaTask.h"

namespace Event {

// NONMATCHING: 97.8% - in case 6 the offset and the pointer of the inlined actor search swap registers
// (r4/r5), same as BehaviorFollow::vf0C (tried: shared locals, no actor local, param pointer local,
// direct chained call, actor declared outside the loop of FindActor_unk)
// inferred: walks the actor to the position of parameter 1 (radius in parameter 5)
int BehaviorMoveTo::vf0C(ActionArg_unk* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* owner = arg->fn_805381A0();
        EventManager_unk* mgr = arg->fn_805381C8();
        int id = owner->m10;
        Actor_unk* actor = mgr->FindActor_unk(id);
        if (actor->fn_800128FC() != 0) {
            atn::Vector3 pos = arg->fn_805382BC(1);
            float radius = arg->GetParam_unk(5)->m04.f;
            RouteWalker_unk* walker = &actor->fn_800128FC()->mC58.m0C;
            walker->fn_80128A30(&pos, &actor->fn_800128FC()->mPos, 8, 0.0f);
            walker->mRadius = radius;
            walker->fn_801255C8();
            actor->SetWait_unk(0);
        }
        break;
    }
    case 6: {
        EventManager_unk* mgr = arg->fn_805381C8();
        int id = arg->GetParam_unk(1)->m04.i;
        Actor_unk* actor = mgr->FindActor_unk(id);
        if (actor->m20 == 6) {
            arg->fn_80538DC4(2);
        }
        break;
    }
    }
    return 0;
}

int BehaviorMoveTo::vf08(ActionArg_unk* arg) {
    ActionOwner_unk* owner = arg->fn_805381A0();
    EventManager_unk* mgr = arg->fn_805381C8();
    int id = owner->m10;
    Actor_unk* actor = mgr->FindActor_unk(id);
    CharaTask* chara = actor->fn_800128FC();
    if (chara == 0) {
        actor->SetWait_unk(1);
        return 0;
    }
    atn::Vector3 rot(chara->mRot);
    RouteWalker_unk* walker = &chara->mC58.m0C;
    float speed = 0.0f;
    if (walker->m2C != 0 || actor->GetWait_unk() == 1) {
        actor->SetWait_unk(1);
    } else {
        walker->fn_80126214();
        atn::Vector3 move(walker->m58);
        speed = move.Length_unk();
        if (speed > 1.0f) {
            speed = 1.0f;
        }
        if (speed > 0.1f) {
            rot = move.GetNormalize().ToRotation_unk();
        }
    }
    speed *= arg->GetParam_unk(6)->m04.i == 0 ? 0.5f : 1.0f;
    chara->fn_8013CB68(rot, 0, speed, 1.0f);
    return 0;
}

} // namespace Event
