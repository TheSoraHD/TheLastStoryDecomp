// inferred: table of the event behaviours (0x8010D98C-0x8010DB94: its __sinit and the destructors
// of the three singletons are here, the behaviours follow in their own files). File name unknown.
#include "Event/Behavior.h"

namespace Event {

static BehaviorRef_unk sBehaviors[3];

void fn_8010D98C() {
    for (int i = 0; i < 3; i++) {
        sBehaviors[i].mBehavior = 0;
    }
    sBehaviors[0].mBehavior = &BehaviorMoveTo::GetInstance_unk();
    sBehaviors[1].mBehavior = &BehaviorFollow::GetInstance_unk();
    sBehaviors[2].mBehavior = &BehaviorGesture::GetInstance_unk();
}

BehaviorMoveTo::~BehaviorMoveTo() {}

BehaviorFollow::~BehaviorFollow() {}

BehaviorGesture::~BehaviorGesture() {}

ActionProcessor* fn_8010DB54(int kind) {
    return sBehaviors[kind].mBehavior;
}

} // namespace Event
