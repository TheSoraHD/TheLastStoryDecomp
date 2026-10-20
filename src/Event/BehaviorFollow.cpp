#include "Event/Behavior.h"
#include "game/CharaTask.h"

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length (PSVECSquareMag)

// inferred: same helper as in game/CharaTask.cpp
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

namespace Event {

// NONMATCHING: 97.7% - in case 6 the offset and the pointer of the inlined actor search swap
// registers (r4/r5)
int BehaviorFollow::vf0C(ActionArg_unk* arg, int phase) {
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
            walker->fn_80128A30(&pos, &pos, 8, 0.0f);
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

int BehaviorFollow::vf08(ActionArg_unk* arg) {
    ActionOwner_unk* owner = arg->fn_805381A0();
    EventManager_unk* mgr = arg->fn_805381C8();
    int id = owner->m10;
    Actor_unk* actor = mgr->FindActor_unk(id);
    CharaTask* chara = actor->fn_800128FC();
    if (chara == 0) {
        if (actor->GetWait_unk() != 1) {
            actor->SetWait_unk(1);
        }
        return 0;
    }
    RouteWalker_unk* walker = &chara->mC58.m0C;
    if (actor->GetWait_unk() == 1) {
        atn::Vector3 pos = arg->fn_805382BC(1);
        float radius = arg->GetParam_unk(5)->m04.f;
        if (LengthSq_unk(pos - actor->fn_80010374()) > radius * radius) {
            walker->fn_80128A30(&pos, &pos, 8, 0.0f);
            walker->mRadius = radius;
            walker->fn_801255C8();
            actor->SetWait_unk(0);
        }
    }
    atn::Vector3 rot(chara->mRot);
    float speed = 0.0f;
    if (actor->GetWait_unk() != 1) {
        if (walker->m2C != 0) {
            actor->SetWait_unk(1);
        }
        walker->m30 = arg->fn_805382BC(1);
        walker->fn_80126214();
        atn::Vector3 move(walker->m58);
        speed = move.Length_unk();
        if (speed > 1.0f) {
            speed = 1.0f;
        }
        if (speed > 0.1f) {
            rot = move.GetNormalized_unk().ToRotation_unk();
        }
    } else {
        atn::Vector3 diff = walker->m30 - chara->mPos;
        if (!(atn::Abs_unk(LengthSq_unk(diff)) < 1.1920929e-7f)) {
            rot = diff.GetNormalized_unk().ToRotation_unk();
        }
    }
    speed *= arg->GetParam_unk(6)->m04.i == 0 ? 0.5f : 1.0f;
    chara->fn_8013CB68(&rot, 0, speed, 1.0f);
    return 0;
}

} // namespace Event
